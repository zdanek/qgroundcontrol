/****************************************************************************
*
* (c) 2009-2022 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
*
* QGroundControl is licensed according to the terms in the file
* COPYING.md in the root of the source code directory.
*
****************************************************************************/

/**
 * @file
 *   @brief WMS map tiles provider
 *
 *   @author Bartek Zdanowski <bartek.zdanowski@gmail.com>
 */

#include "WmsMapProvider.h"

#include <QSettings>

#include "WmsServerService.h"

//http://localhost:8080/services/wms?service=wms&version=1.1.0&request=GetMap&srs=EPSG:3857&bbox=2341777.349002,6846082.438362,2343253.539110,6847119.115558&width=256&height=256&layers=klokantech-basic-2&styles=default&format=image/png
// kenbit     "http://192.168.10.25:8080/geoserver/wms?service=wms&version=1.1.0&request=GetMap&srs=%1&width=256&height=256&layers=%2&"
//    "styles=&format=image/png&bbox=%3,%4,%5,%6");
//static const QString MapBaseUrl = QStringLiteral("http://localhost:8080/services/wms?service=wms&version=1.1.0&request=GetMap&"
//                                                 "srs=%1&bbox=%3,%4,%5,%6&width=256&height=256&layers=%2&styles=default&format=image/png");

// https://mapy.geoportal.gov.pl/wss/service/PZGIK/NMT/GRID1/WMS/ShadedRelief?request=GetCapabilities&service=wms
static const QString wmsServiceQuery = QStringLiteral("?service=wms&version=1.1.0&request=GetMap&srs=%1&width=256&height=256&layers=%2&bbox=%3,%4,%5,%6&styles=%7&format=image/png");
//static const QString wmsServiceUrl = QStringLiteral("http://localhost:8080/services/wms");

WmsMapProvider::WmsMapProvider(QObject *parent)
    : MapProvider(QString(), QString(), AVERAGE_TILE_SIZE, QGeoMapType::StreetMap, parent)
{
    _loadConfig();
    WmsServerService *service = new WmsServerService(wmsServiceUrl_);
    service->checkConnection();

    _saveConfig();
}

QString WmsMapProvider::_getURL(const int x,
                                const int y,
                                const int zoom,
                                QNetworkAccessManager *networkManager) {
    Q_UNUSED(networkManager)
    float bbox_min_x, bbox_min_y, bbox_max_x, bbox_max_y;

    if (!_tileToBBox(x, y, zoom, &bbox_min_x, &bbox_min_y, &bbox_max_x, &bbox_max_y)) {
        return QString();
    }

    //TODO(bzd) set desired WMS server float precision
    return wmsServiceUrl_ +
        wmsServiceQuery.arg(query_srs_,
                          layer_,
                          QString::number(bbox_min_x, 'f', 8),
                          QString::number(bbox_min_y, 'f', 8),
                          QString::number(bbox_max_x, 'f', 8),
                          QString::number(bbox_max_y, 'f', 8),
                          style_);
}

bool WmsMapProvider::_tileToBBox(const int x,
                                const int y,
                                const int zoom,
                                float *bbox_min_x,
                                float *bbox_min_y,
                                float *bbox_max_x,
                                float *bbox_max_y) {
    float x1, x2, y1, y2;

    if (projection_ == Projection::EPSG_3857 || projection_ == Projection::EPSG_4326) {
        x1 = _tileXToLon(x, zoom);
        x2 = _tileXToLon(x + 1, zoom);

        y1 = _tileYToLat(y, zoom);
        y2 = _tileYToLat(y + 1, zoom);

        *bbox_min_x = fmin(x1, x2);
        *bbox_min_y = fmin(y1, y2);
        *bbox_max_x = fmax(x1, x2);
        *bbox_max_y = fmax(y1, y2);

        return true;
    }

    return false;
}

float WmsMapProvider::_tileXToLon(const int x, const int zoom)
{
    const int n = (1 << zoom);
    float wgs_84_lon = (((float) x / n) * 360) - 180;

    if (projection_ == Projection::EPSG_4326) {
        return wgs_84_lon;
    }

    if (projection_ == Projection::EPSG_3857) {
        return (maxWebMerkator * wgs_84_lon) / 180;
    }

    return 0;
}

float WmsMapProvider::_tileYToLat(const int y, const int zoom)
{
    const int n = (1 << zoom);
    float wgs_84_lat = atan(sinh(M_PI * (1 - ((float) 2 * y / n)))) * 180 / M_PI;

    if (projection_ == Projection::EPSG_4326) {
        return wgs_84_lat;
    }

    if (projection_ == Projection::EPSG_3857) {
        return (log(tan((90 + wgs_84_lat) * M_PI / 360)) / (M_PI / 180)) * maxWebMerkator / 180;
    }
    return 0;
}

void WmsMapProvider::_loadConfig()
{
    QSettings settings;
    settings.beginGroup("WMS");
    // wmsServiceUrl_ = settings.value("url", "https://mapy.geoportal.gov.pl/wss/service/PZGIK/NMT/GRID1/WMS/ShadedRelief").toString();

    bool lidar = false;
    if (lidar) {
        // https://mapy.geoportal.gov.pl/wss/service/PZGIK/BDOO/WMTS/aktualne?&SERVICE=WMTS&REQUEST=GetTile&VERSION=1.0.0&LAYER=BDOO&STYLE=default&TILEMATRIXSET=EPSG:2180&TILEMATRIX=EPSG:2180:3&TILEROW=2&TILECOL=3&FORMAT=image/png&rfh=1
        wmsServiceUrl_ = "https://mapy.geoportal.gov.pl/wss/service/img/guest/Ogolnogeograficzna/MapServer/WMSServer";
        wmsServiceUrl_ = "https://mapy.geoportal.gov.pl/wss/service/PZGIK/NMT/GRID1/WMS/ShadedRelief";
        // query_srs_ = settings.value("srs", "EPSG:4326").toString();
        query_srs_ = "EPSG:4326";
        // layer_ = settings.value("layer", "Raster").toString();
        layer_ = "Ogolnogeograficzna";
        layer_ = "Raster";
        style_ = settings.value("style", "default").toString();
    } else {
        wmsServiceUrl_ = settings.value("url", "http://10.0.0.100/geoserver/wms").toString();
        query_srs_ = settings.value("srs", "EPSG:4326").toString();
        layer_ = settings.value("layer", "Polska").toString();
        style_ = settings.value("style", "default").toString();
    }

    if (query_srs_.endsWith(":3857")) {
        projection_ = Projection::EPSG_3857;
    } else if (query_srs_.endsWith(":4326")) {
        projection_ = Projection::EPSG_4326;
    } else {
        projection_ = Projection::unknown;
    }

    settings.endGroup();
}

void WmsMapProvider::_saveConfig() {
    QSettings settings;
    settings.beginGroup("WMS");
    settings.setValue("url", wmsServiceUrl_);
    settings.setValue("srs", query_srs_);
    settings.setValue("layer", layer_);
    settings.setValue("style", style_);
    settings.endGroup();
}
