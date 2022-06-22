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

WmsMapProvider::WmsMapProvider(QObject *parent)
    : MapProvider(QString(), QString(), AVERAGE_TILE_SIZE, QGeoMapType::StreetMap, parent)
{}

//http://localhost:8080/services/wms?service=wms&version=1.1.0&request=GetMap&srs=EPSG:3857&bbox=2341777.349002,6846082.438362,2343253.539110,6847119.115558&width=256&height=256&layers=klokantech-basic-2&styles=default&format=image/png
static const QString MapBaseUrl = QStringLiteral(
    "http://192.168.10.25:8080/geoserver/wms?service=wms&version=1.1.0&request=GetMap&srs=%1&width=256&height=256&layers=%2&"
    "styles=&format=image/png&bbox=%3,%4,%5,%6");

QString WmsMapProvider::_getURL(const int x,
                                const int y,
                                const int zoom,
                                QNetworkAccessManager *networkManager) {
    Q_UNUSED(networkManager)
    float bbox_min_x, bbox_min_y, bbox_max_x, bbox_max_y;

    if (!TileToBBox(x, y, zoom, &bbox_min_x, &bbox_min_y, &bbox_max_x, &bbox_max_y)) {
        return QString();
    }

    //TODO(bzd) set desired WMS server float precision
    return MapBaseUrl.arg(query_srs_,
                          layer_,
                          QString::number(bbox_min_x, 'f', 8),
                          QString::number(bbox_min_y, 'f', 8),
                          QString::number(bbox_max_x, 'f', 8),
                          QString::number(bbox_max_y, 'f', 8));
}

bool WmsMapProvider::TileToBBox(const int x,
                                const int y,
                                const int zoom,
                                float *bbox_min_x,
                                float *bbox_min_y,
                                float *bbox_max_x,
                                float *bbox_max_y) {
    float x1, x2, y1, y2;

    if (query_srs_.endsWith(":3857") || query_srs_.endsWith(":4326")) {
        x1 = TileXToLon(x, zoom, query_srs_);
        x2 = TileXToLon(x + 1, zoom, query_srs_);

        y1 = TileYToLat(y, zoom, query_srs_);
        y2 = TileYToLat(y + 1, zoom, query_srs_);

        *bbox_min_x = fmin(x1, x2);
        *bbox_min_y = fmin(y1, y2);
        *bbox_max_x = fmax(x1, x2);
        *bbox_max_y = fmax(y1, y2);

        return true;
    }

    return false;
}

float WmsMapProvider::TileXToLon(const int x, const int zoom, QString srs) {
    const int n = (1 << zoom);
    float wgs_84_lon = (((float) x / n) * 360) - 180;

    if (srs.endsWith(":4326")) {
        return wgs_84_lon;
    }

    if (srs.endsWith(":3857")) {
        return (maxWebMerkator * wgs_84_lon) / 180;
    }

    return 0;
}

float WmsMapProvider::TileYToLat(const int y, const int zoom, QString srs) {
    const int n = (1 << zoom);
    float wgs_84_lat = atan(sinh(M_PI * (1 - ((float) 2 * y / n)))) * 180 / M_PI;

    if (srs.endsWith(":4326")) {
        return wgs_84_lat;
    }

    if (srs.endsWith(":3857")) {
        return (log(tan((90 + wgs_84_lat) * M_PI / 360)) / (M_PI / 180)) * maxWebMerkator / 180;
    }
    return 0;
}
