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

#pragma once

#include "MapProvider.h"

namespace {
    const float maxWebMerkator = 20037508.342789f;
}

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
    bool TileToBBox(const int x,
                    const int y,
                    const int zoom,
                    float *bbox_min_x,float *bbox_min_y, float *bbox_max_x, float *bbox_max_y);

//    QString layer_ = "klokantech-basic";
//    QString layer_ = "osm-bright";
    QString layer_ = "Polska";
//    QString query_srs_ = "EPSG:3857";
    QString query_srs_ = "EPSG:4326";
    float TileXToLon(const int x, const int zoom, QString q_string);
    float TileYToLat(const int y, const int zoom, QString q_string);
};
