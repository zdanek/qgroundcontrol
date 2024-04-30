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

#ifndef WMSMAPPROVIDER_H
#define WMSMAPPROVIDER_H


#include "MapProvider.h"

namespace {
    const float maxWebMerkator = 20037508.342789f;
}

enum class Projection {
    EPSG_3857,
    EPSG_4326,
    unknown
};

class WmsMapProvider : public MapProvider {
    Q_OBJECT

public:
    WmsMapProvider(QObject *parent);
    ~WmsMapProvider() = default;

protected:
    QString _getURL(const int x,
                    const int y,
                    const int zoom,
                    QNetworkAccessManager *networkManager) override;

private:
    /**
     * Returns (via references) bounding boxes set in query_srs_ projection.
     *
     * If conversion is not possible, due to unknown projection or arguments out of range
     * false is returned.
     */
    bool _tileToBBox(const int x,
                    const int y,
                    const int zoom,
                    float *bbox_min_x,float *bbox_min_y, float *bbox_max_x, float *bbox_max_y);
    float _tileXToLon(const int x, const int zoom);
    float _tileYToLat(const int y, const int zoom);
    void _loadConfig();

    Projection projection_ = Projection::unknown;

    QString wmsServiceUrl_ = "http://localhost:8080/services/wms";
    QString layer_ = "klokantech-basic";    //klokantech-basic-2
    QString style_ = "default";
    QString query_srs_ = "EPSG:3857";

    // Kenbit
//    QString wmsServiceUrl_ = "http://192.168.10.25:8080/geoserver/wms";
//    QString layer_ = "Polska";
//    QString style_ = "";
//    QString query_srs_ = "EPSG:4326";
    void _saveConfig();
};

#endif