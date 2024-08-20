//
// Created by zdanek on 07.08.24.
//

#include "PoiLayerLoader.h"

#include <QtLocation/private/qgeojson_p.h>
#include <QFile>
#include <QGeoCircle>
#include <QGeoPath>
#include <QGeoPolygon>

#include "PoiPolygon.h"
#include "PoiPolyline.h"
#include "PoiSvg.h"
#include "QGCLoggingCategory.h"
#include "PoiLayer.h"
#include "PoiLayerController.h"
#include "PoiGeom.h"
#include "PoiPoint.h"
#include "QGCApplication.h"

QGC_LOGGING_CATEGORY(PoiLayerLoaderLog, "PoiLayerLoaderLog")


PoiLayer *PoiLayerLoader::loadGeoJson(const QString &geoJsonFile, QObject *parent)
{
    qCDebug(PoiLayerLoaderLog) << "PoiLayerController::loadGeoJson:" << geoJsonFile;

    PoiLayer *poiL = new PoiLayer(parent);
    QList<PoiGeom *> geoms = QList<PoiGeom *>();

    QFile geoJson(geoJsonFile);
    geoJson.open(QIODevice::ReadOnly | QIODevice::Text);
    QString geoJsonString = geoJson.readAll();
    geoJson.close();

    QJsonDocument doc = QJsonDocument::fromJson(geoJsonString.toUtf8());
    const QString &random = QString::number(rand());
    poiL->setName("Layer " + random);
    poiL->setId("layer_" + random);

    if (!doc["properties"].isUndefined()) {
        const QVariantMap &properties = doc["properties"].toObject().toVariantMap();
        if (properties.contains("name")) {
            const QString &name = properties["name"].toString();
            qCDebug(PoiLayerLoaderLog) << "name:" << name;
            poiL->setName(name);
            poiL->setId(name);
        }
        if (properties.contains("ext:id")) {
            const QString &id = properties["ext:id"].toString();
            qCDebug(PoiLayerLoaderLog) << "id:" << id;
            poiL->setId(id);
        }
        if (properties.contains("ext:syncpoint")) {
            const int syncpoint = properties["ext:syncpoint"].toInt();
            qCDebug(PoiLayerLoaderLog) << "syncpoint:" << syncpoint;
            poiL->setSyncpoint(syncpoint);
        }
    }

    const QVariantList &docs = QGeoJson::importGeoJson(doc);
    if (docs.size() == 0) {
        qCCritical(PoiLayerLoaderLog) << "No features found in GeoJSON";
        return nullptr;
    }
    const QVariant &qv = docs.at(0);
    const QVariantMap &qvm = qv.toMap();

    const QString &type = qvm["type"].toString();
    if (type == "FeatureCollection") {
        const QVariantList &features = qvm["data"].toList();
        for (const QVariant &feature : features) {
            const QVariantMap &featureMap = feature.toMap();
            const QString &type = featureMap["type"].toString();

            QVariantMap styles;
            if (featureMap.contains("properties")) {
                const QVariantMap &properties = featureMap["properties"].toMap();
                if (properties.contains("stroke")) {
                    styles["line_color"] = properties["stroke"].toString();
                }
                if (properties.contains("stroke-width")) {
                    styles["line_width"] = properties["stroke-width"].toInt();
                }
                if (properties.contains("stroke-opacity")) {
                    styles["line_opacity"] = properties["stroke-opacity"].toDouble();
                    if (styles["line_opacity"] == 0) {
                        styles["line_width"] = 0;
                    }
                }
                if (properties.contains("fill")) {
                    qCDebug(PoiLayerLoaderLog) << "fill:" << properties["fill"].toString();
                    styles["fill_color"] = properties["fill"].toString();
                }
                if (properties.contains("fill-opacity")) {
                    styles["fill_opacity"] = properties["fill-opacity"].toDouble();
                    if (styles["fill_opacity"] == 0) {
                        styles["fill_color"] = "transparent";
                    }
                }
            }

            if (type == "GeometryCollection") {
                //                qCDebug(PoiLayerLoaderLog) << "GeometryCollection";
                //                qCDebug(PoiLayerLoaderLog) << "GeometryCollection data:" << data;
                const QVariantList &geometries = featureMap["data"].toList();
                for (const QVariant &geometry : geometries) {
                    //                     qCDebug(PoiLayerLoaderLog) << " geometry qVariant type:" << geometry.typeName();
                    //const QVariant &data = geometry["data"];
                    const QMap<QString, QVariant> &geometryMap = geometry.toMap();
                    //                    qCDebug(PoiLayerLoaderLog) << " :" << geometryMap["type"];
                    if (geometryMap["type"] == "LineString") {
                        geoms.append(processLineString(geometryMap, styles, poiL));
                    } else {
                        qCCritical(PoiLayerLoaderLog) << "Unsupported type:" << geometryMap["type"];
                    }
                }
                continue;
            }

            if (type == "Polygon") {
                const QGeoPolygon &gp = featureMap["data"].value<QGeoPolygon>();
                qCDebug(PoiLayerLoaderLog) << "Polygon:" << gp;

                PoiPolygon *pp = new PoiPolygon(poiL);
                pp->setStyles(styles);
                pp->appendVertices(gp.path());

                geoms.append(pp);
                continue;
            }
            if (type == "MultiLineString") {
                qCDebug(PoiLayerLoaderLog) << "MultiLineString";

                const QVariantList &mls = featureMap["data"].toList();
                for (const QVariant &line : mls) {
                    const QMap<QString, QVariant> &lineMap = line.toMap();
                    qCDebug(PoiLayerLoaderLog) << " :" << lineMap["type"];
                    if (lineMap["type"].toString() == "LineString") {
                        geoms.append(processLineString(lineMap, styles, poiL));
                    } else {
                        qCCritical(PoiLayerLoaderLog) << "Unsupported type:" << lineMap["type"];
                    }
                }
                continue;
            }
            if (type == "LineString") {
                geoms.append(processLineString(featureMap, styles, poiL));

                continue;
            }
            if (type == "Point") {
                qCDebug(PoiLayerLoaderLog) << featureMap;
                qCDebug(PoiLayerLoaderLog) << "Point";
                const QGeoCircle &gc = featureMap["data"].value<QGeoCircle>();
                qCDebug(PoiLayerLoaderLog) << "GeoCoordinate: " << gc;
                const QMap<QString, QVariant> &properties = featureMap["properties"].toMap();
                if (properties.contains("code")) {
                    qCDebug(PoiLayerLoaderLog) << "code:" << properties["code"];
                    QString code = properties["code"].toString();
                    QString standard;
                    if (properties.contains("standard")) {
                        standard = properties["standard"].toString();
                    } else {
                        standard = "";
                    }
                    qCDebug(PoiLayerLoaderLog) << "code standard: " << standard;

                    QString aa = properties["aa"].toString();
                    QString w = properties["w"].toString();
                    QString t = properties["t"].toString();
                    PoiSvg *pp = new PoiSvg(poiL);
                    pp->setCenter(gc.center());
                    pp->setSrc("http://127.0.0.1:8080/rest/symbol/" + code + "?aa=" + aa + "&w=" + w + "&t=" + t);
                    geoms.append(pp);
                    continue;
                } else {
                    PoiPoint *pp = new PoiPoint(poiL);
                    pp->setCenter(gc.center());
                    geoms.append(pp);
                }
                continue;
            }
            qCCritical(PoiLayerLoaderLog) << "Unsupported type:" << type;
        }
    } else {
        qCCritical(PoiLayerLoaderLog) << "Unsupported root type: " << type;
    }
    poiL->append(geoms);

    return poiL;
}

PoiGeom *PoiLayerLoader::processLineString(const QVariantMap &featureMap, const QVariantMap &styles, QObject *parent)
{
    const QGeoPath &gp = featureMap["data"].value<QGeoPath>();
    PoiPolyline *poly = new PoiPolyline(parent);
    poly->setStyles(styles);
    poly->appendVertices(gp.variantPath());

    return poly;
}