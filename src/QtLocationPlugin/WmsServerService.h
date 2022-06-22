
#pragma once

#include <QList>
#include <QNetworkAccessManager>
#include <QObject>
#include <QString>

class WmsServerService : public QObject
{
    Q_OBJECT

public:
    explicit WmsServerService(const QString &map_base_url);
    ~WmsServerService();
    bool checkConnection();
    QList<QString> getLayers();

private:
    const QString _mapBaseUrl;
    QNetworkAccessManager *_networkManager;
};

