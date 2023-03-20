//
// Created by zdanek on 17.03.23.
//

#include "WmsServerService.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>

WmsServerService::WmsServerService(const QString &map_base_url)
    : _mapBaseUrl(map_base_url)
{
    _networkManager = new QNetworkAccessManager(this);
}

bool WmsServerService::checkConnection()
{

    QNetworkRequest request(_mapBaseUrl);

    // Send the network request and get a QNetworkReply
    QNetworkReply *reply = _networkManager->get(request);
    //check for error
    if (reply->error() != QNetworkReply::NoError) {
        qWarning() << "Network error:" << reply->errorString();
        reply->deleteLater();
        return false;
    }

    reply->close();
    reply->deleteLater();

    return true;
}
QList<QString> WmsServerService::getLayers()
{

    return QList<QString>();
}
WmsServerService::~WmsServerService() {
    delete _networkManager;
}
