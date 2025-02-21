//
// Created by zdanek on 20.02.25.
//

#include "OrdersController.h"

#include <QObject>

QGC_LOGGING_CATEGORY(OrdersControllerLog, "OrdersControllerLog")


OrdersController::OrdersController(QObject *parent)
    : QObject(parent)
{
    connect(this, &OrdersController::openPage, qgcApp(), &QGCApplication::openWebPage);

    _loadConfig();
    _saveConfig();

}

void OrdersController::openOrdersPage()
{
    qCDebug(OrdersControllerLog) << "OrdersController::openOrdersPage:" << _orders_main_page_url;
    emit openPage(_orders_main_page_url);
}

void OrdersController::_loadConfig()
{
    qCDebug(OrdersControllerLog) << "OrdersController::_loadConfig()";
    QSettings settings;
    settings.beginGroup("MewaMed");

    _orders_main_page_url = settings.value("orders_main_page_url", "http://84.10.54.242:25200/orders/").toString();

    qCDebug(OrdersControllerLog) << "Orders main page url: " << _orders_main_page_url;
    settings.endGroup();
}

void OrdersController::_saveConfig() const
{
    qCDebug(OrdersControllerLog) << "OrdersController::_saveConfig()";
    QSettings settings;
    settings.beginGroup("MewaMed");
    settings.setValue("orders_main_page_url", _orders_main_page_url);
    settings.endGroup();
}