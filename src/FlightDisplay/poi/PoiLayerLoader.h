//
// Created by zdanek on 07.08.24.
//

#ifndef SRC_FLIGHTDISPLAY_POI_POILAYERLOADER_H_
#define SRC_FLIGHTDISPLAY_POI_POILAYERLOADER_H_

#include "PoiLayer.h"

#include <QString>

class PoiLayerLoader {

public:
    explicit PoiLayerLoader(const QString imagesBasePath);
    static PoiGeom *processLineString(const QVariantMap &featureMap, const QVariantMap &styles, QObject *parent);
    PoiLayer *loadGeoJson(const QString &geoJsonFile, QObject *parent);

private:
    const QString _imagesBasePath;
    QString getImagePath(const QString &code, const QString &standard, const QString &aa, const QString &w, const QString &t);
    QString crc(const QString &text);
};

#endif //SRC_FLIGHTDISPLAY_POI_POILAYERLOADER_H_
