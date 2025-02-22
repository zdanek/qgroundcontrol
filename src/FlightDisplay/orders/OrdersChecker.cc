//
// Created by zdanek on 22.02.25.
//

#include "OrdersChecker.h"

#include <QEventLoop>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>

OrdersChecker::OrdersChecker(OrdersController *parent, const QString orders_url) : QThread(parent), _orders_url(orders_url)
{
    connect(this, &OrdersChecker::newOrdersAvailable, parent, &OrdersController::newOrdersAvailable);
}

void OrdersChecker::shutdown()
{
    qDebug(MewaMedLog) << "Shutting down OrdersChecker";
    _run = false;
}

void OrdersChecker::run()
{
    QThread::sleep(3);
    qCDebug(MewaMedLog) << "OrdersChecker::run() started";

    while (_run) {
        int sync_point = _getCurrentSyncPoint();
        if (sync_point > _latest_sync_point) {
            _latest_sync_point = sync_point;
            qCDebug(MewaMedLog) << "New orders available, sync point:" << _latest_sync_point;
            emit newOrdersAvailable();
        }
        QThread::sleep(10);
    }
}

int OrdersChecker::_getCurrentSyncPoint()
{
   QNetworkAccessManager manager;
    QEventLoop loop;
    QNetworkRequest request(QUrl(_orders_url + "/orders/order?syncPoint=" + QString::number(_latest_sync_point)));
    QNetworkReply *reply = manager.get(request);

    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (reply->error() != QNetworkReply::NoError) {
        qCWarning(MewaMedLog) << "Error fetching orders:" << reply->errorString();
        reply->deleteLater();
        return _latest_sync_point;
    }

    QByteArray response_data = reply->readAll();
    reply->deleteLater();

    QJsonDocument json_doc = QJsonDocument::fromJson(response_data);
    if (!json_doc.isArray()) {
        qCWarning(MewaMedLog) << "Invalid JSON response - expected array";
        return _latest_sync_point;
    }

    int max_sync_point = _latest_sync_point;
    QJsonArray orders = json_doc.array();

    for (const QJsonValue &order : orders) {
        if (!order.isObject()) continue;

        QJsonObject order_obj = order.toObject();
        int sync_point = order_obj["syncPoint"].toInt();
        max_sync_point = qMax(max_sync_point, sync_point);
    }

    return max_sync_point;
}