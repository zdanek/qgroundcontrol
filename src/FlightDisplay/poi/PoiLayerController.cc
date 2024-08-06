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
 *   @brief POI Layer Controller
 *
 *   @author Bartek Zdanowski <bartek.zdanowski@pelixar.com>
 */

#include <QObject>

#include "PoiLayer.h"
#include "PoiLayerController.h"

#include <QGeoCircle>
#include <QGeoPath>
#include <QGeoPolygon>
#include <QtLocation/private/qgeojson_p.h>

#include "PoiGeom.h"
#include "PoiPoint.h"
#include "PoiPolygon.h"
#include "PoiPolyline.h"
#include "PoiSvg.h"
#include "QGCApplication.h"
#include "QGCLoggingCategory.h"

QGC_LOGGING_CATEGORY(PoiLayerControllerLog, "PoiLayerControllerLog")

PoiLayerController::PoiLayerController(QObject *parent)
    : QObject(parent)
{}

PoiLayerController::~PoiLayerController() {}

void PoiLayerController::start()
{
    qCDebug(PoiLayerControllerLog) << "PoiLayerController::start";

//    PoiLayer *poiL = loadGeoJson("/opt/workspace/projects/drones/qgmewamed/kml/MERGED_tic~mip31_29100019301000000001.geojson");
    PoiLayer *poiL = loadGeoJson("/opt/workspace/projects/drones/qgmewamed/kml/tic~mip31_29100019301000000001.geojson");
    //    loadGeoJson("/opt/workspace/projects/drones/qgmewamed/kml/line-samples.geojson");
    //    loadGeoJson("/opt/workspace/projects/drones/qgmewamed/kml/example1.geojson");


    if (poiL) {
        poiL->setVisible(true);
        addPoiLayer(poiL);
        emit poiLayersChanged();
    }

}

void PoiLayerController::deletePoiLayer(QString id)
{
    qCDebug(PoiLayerControllerLog) << "PoiLayerController::deletePoiLayer:" << id;
    for (int i = 0; i < _poiLayers.count(); i++) {
        PoiLayer *poiLayer = dynamic_cast<PoiLayer *>(_poiLayers.get(i));
        if (poiLayer->id() == id) {
            poiLayer->setVisible(false);
            emit poiLayer->deleted();
            _poiLayers.removeAt(i);
            poiLayer->deleteLater();
            emit poiLayersChanged();
            return;
        }
    }
}

void PoiLayerController::addPoiLayer(PoiLayer *pLayer) {
    for (int i = 0; i < _poiLayers.count(); i++) {
        PoiLayer *poiLayer = dynamic_cast<PoiLayer *>(_poiLayers.get(i));
        if (poiLayer->id() == pLayer->id()) {
            qCDebug(PoiLayerControllerLog) << "PoiLayerController::addPoiLayer: Layer with ID " << pLayer->id() << " already exists";
            deletePoiLayer(pLayer->id());
        }
    }

    _poiLayers.append(pLayer);
    emit poiLayersChanged();

}
PoiLayer *PoiLayerController::loadGeoJson(const QString &geoJsonFile)
{
    qCDebug(PoiLayerControllerLog) << "PoiLayerController::loadGeoJson:" << geoJsonFile;

    PoiLayer *poiL = new PoiLayer(this);
    QList<PoiGeom *> geoms = QList<PoiGeom *>();


    QFile geoJson(geoJsonFile);
    geoJson.open(QIODevice::ReadOnly | QIODevice::Text);
    QString geoJsonString = geoJson.readAll();
    geoJson.close();

    QJsonDocument doc = QJsonDocument::fromJson(geoJsonString.toUtf8());
//    qCDebug(PoiLayerControllerLog) << "doc:" << doc;

    if (!doc["name"].isUndefined()) {
            const QString &name = doc["name"].toString();
            qCDebug(PoiLayerControllerLog) << "name:" << name;
            poiL->setName(name);
            poiL->setId(name);
    } else {
        qCDebug(PoiLayerControllerLog) << "name: not found";
        const QString &random = QString::number(rand());
        poiL->setName("Layer " + random);
        poiL->setId("layer_" + random);
    }

    const QVariantList &x = QGeoJson::importGeoJson(doc);

    qCDebug(PoiLayerControllerLog) << "x size" << x.size();
    if (x.size() == 0) {
        qCCritical(PoiLayerControllerLog) << "No features found in GeoJSON";
        return nullptr;
    }
    const QVariant &qv = x.at(0);
    const QVariantMap &qvm = qv.toMap();

    qCDebug(PoiLayerControllerLog) << "qvm: " << qvm;
    qCDebug(PoiLayerControllerLog) << qvm["type"].toString() << " " << qvm["data"];

//    QMap(
//        ("data", QVariant(QVariantList, (QVariant(QVariantMap, QMap(("data", QVariant(QGeoPath, ))("type", QVariant(QString, "LineString")))), QVariant(QVariantMap, QMap(("data", QVariant(QGeoPath, ))("type", QVariant(QString, "LineString")))), QVariant(QVariantMap, QMap(("data", QVariant(QGeoPath, ))("type", QVariant(QString, "LineString")))), QVariant(QVariantMap, QMap(("data", QVariant(QGeoPath, ))("type", QVariant(QString, "LineString")))), QVariant(QVariantMap, QMap(("data", QVariant(QGeoPath, ))("type", QVariant(QString, "LineString")))))))
//        ("properties", QVariant(QVariantMap, QMap(("description", QVariant(QString, "<b>TE_002</b><br/>TE_002"))("stroke", QVariant(QString, "#80e0ff"))("stroke-opacity", QVariant(qlonglong, 1))("stroke-width", QVariant(qlonglong, 3)))))
//        ("type", QVariant(QString, "GeometryCollection")))

    const QString &type = qvm["type"].toString();
    if (type == "FeatureCollection") {
        const QVariantList &features = qvm["data"].toList();
        for (const QVariant &feature : features) {
//            qCDebug(PoiLayerControllerLog) << "Feature:" << feature;
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
                    qCDebug(PoiLayerControllerLog) << "fill:" << properties["fill"].toString();
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
//                qCDebug(PoiLayerControllerLog) << "GeometryCollection";
//                qCDebug(PoiLayerControllerLog) << "GeometryCollection data:" << data;
                const QVariantList &geometries = featureMap["data"].toList();
                for (const QVariant &geometry : geometries) {
//                     qCDebug(PoiLayerControllerLog) << " geometry qVariant type:" << geometry.typeName();
                     //const QVariant &data = geometry["data"];
                    const QMap<QString, QVariant> &geometryMap = geometry.toMap();
//                    qCDebug(PoiLayerControllerLog) << " :" << geometryMap["type"];
                    if (geometryMap["type"] == "LineString") {
                        geoms.append(processLineString(geometryMap, styles, poiL));
                    } else {
                        qCCritical(PoiLayerControllerLog) << "Unsupported type:" << geometryMap["type"];
                    }
                }
                continue;
            }

            if (type == "Polygon") {
                const QGeoPolygon &gp = featureMap["data"].value<QGeoPolygon>();
                qCDebug(PoiLayerControllerLog) << "Polygon:" << gp;

                PoiPolygon *pp = new PoiPolygon(poiL);
                pp->setStyles(styles);
                pp->appendVertices(gp.path());

                geoms.append(pp);
                continue;
            }
            if (type == "MultiLineString") {
                qCDebug(PoiLayerControllerLog) << "MultiLineString";

                const QVariantList &mls = featureMap["data"].toList();
                for (const QVariant &line : mls) {
                    const QMap<QString, QVariant> &lineMap = line.toMap();
                    qCDebug(PoiLayerControllerLog) << " :" << lineMap["type"];
                    if (lineMap["type"].toString() == "LineString") {
                        geoms.append(processLineString(lineMap, styles, poiL));

                    } else {
                        qCCritical(PoiLayerControllerLog) << "Unsupported type:" << lineMap["type"];
                    }


                }
                continue;
            }
            if (type == "LineString") {
                geoms.append(processLineString(featureMap, styles, poiL));

                continue;
            }
            if (type == "Point") {
                qCDebug(PoiLayerControllerLog) << featureMap;
                qCDebug(PoiLayerControllerLog) << "Point";
                const QGeoCircle &gc = featureMap["data"].value<QGeoCircle>();
                qCDebug(PoiLayerControllerLog) << "GeoCoordinate: " << gc;
                const QMap<QString, QVariant> &properties = featureMap["properties"].toMap();
                if (properties.contains("code")) {
                    qCDebug(PoiLayerControllerLog) << "code:" << properties["code"];
                    QString code = properties["code"].toString();
                    QString standard;
                    if (properties.contains("standard")) {
                        standard = properties["standard"].toString();
                    } else {
                        standard = "milstd_2525c";
                    }
                    qCDebug(PoiLayerControllerLog) << "code standard: " << standard;
                    if (standard == "milstd_2525c") {
                        QString aa = properties["aa"].toString();
                        QString w = properties["w"].toString();
                        QString t = properties["t"].toString();
                        PoiSvg *pp = new PoiSvg(poiL);
                        pp->setCenter(gc.center());
                        pp->setSrc("http://127.0.0.1:8080/rest/symbol/" + code + "?aa=" + aa + "&w=" + w + "&t=" + t);
                        geoms.append(pp);
                        continue;
                    } else {
                        qCritical(PoiLayerControllerLog) << "Unsupported code standard:" << standard;
                    }
                } else {
                    PoiPoint *pp = new PoiPoint(poiL);
                    pp->setCenter(gc.center());
                    geoms.append(pp);
                }
                continue;
            }
            qCCritical(PoiLayerControllerLog) << "Unsupported type:" << type;
        }
    }
    else {
        qCCritical(PoiLayerControllerLog) << "Unsupported type: " << type;
    }
    poiL->append(geoms);
    poiL->setVisible(true);

    return poiL;
}

PoiGeom *PoiLayerController::processLineString(const QVariantMap &featureMap, const QVariantMap &styles, QObject *parent) const
{
//    qCDebug(PoiLayerControllerLog) << "Processing LineString";
    const QGeoPath &gp = featureMap["data"].value<QGeoPath>();
//    qCDebug(PoiLayerControllerLog) << "GeoPath type: " << gp.type();
//    qCDebug(PoiLayerControllerLog) << "GeoPath path: " << gp.path();

    PoiPolyline *poly = new PoiPolyline(parent);
    poly->setStyles(styles);
    poly->appendVertices(gp.variantPath());

    return poly;
}
