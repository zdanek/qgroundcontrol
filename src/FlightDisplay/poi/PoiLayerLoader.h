//
// Created by zdanek on 07.08.24.
//

#ifndef SRC_FLIGHTDISPLAY_POI_POILAYERLOADER_H_
#define SRC_FLIGHTDISPLAY_POI_POILAYERLOADER_H_

#include "PoiLayer.h"

#include <QString>

class PoiLayerLoader {

public:
    static PoiGeom *processLineString(const QVariantMap &featureMap, const QVariantMap &styles, QObject *parent);
    static PoiLayer *loadGeoJson(const QString &geoJsonFile, QObject *parent);
};

#endif //SRC_FLIGHTDISPLAY_POI_POILAYERLOADER_H_
