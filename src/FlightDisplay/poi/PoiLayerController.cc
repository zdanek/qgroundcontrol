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
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtLocation/private/qgeojson_p.h>

#include "PoiGeom.h"
#include "PoiPoint.h"
#include "PoiPolygon.h"
#include "PoiPolyline.h"
#include "PoiSvg.h"
#include "QGCApplication.h"
#include "QGCLoggingCategory.h"
#include "QGCMapLayer.h"
#include "QGCMapPolygon.h"
#include "QGCMapPolyline.h"
#include "kmldocument.h"
#include "qmlkml.h"

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

//    addPoiLayer(loadGeoJson("/opt/workspace/projects/drones/qgmewamed/kml/vienna-streets.geojson"));



    if (true)
        return;

    QList<QGCMapGeom *> elements = QList<QGCMapGeom *>();
//    QGCMapPolygon *poly = new QGCMapPolygon(this);

//    poly->loadFromJson("{\"type\":\"Polygon\",\"coordinates\":[[[19.938,50.061],[19.938,50.062],[19.939,50.062],[19.939,50.061],[19.938,50.061]]]}", false , "error");
//    elements.append();
//     QSharedPointer<QtKml::KmlQmlGraphics> poiGraphics = loadKML("/opt/workspace/projects/drones/qgmewamed/kml/ext_milstd.kml");
    QSharedPointer<QtKml::KmlQmlGraphics> poiGraphics = loadKML("/opt/workspace/projects/drones/qgmewamed/kml/tic~mip31_29100019301000000001.kml");
//    QSharedPointer<QtKml::KmlQmlGraphics> poiGraphics = loadKML("http://localhost:8080/rest/layers/kml/get/mip31~29100019300000000000");
//    QSharedPointer<QtKml::KmlQmlGraphics> poiGraphics = loadKML("http://localhost:8080/rest/layers/kml/get/mip31~29100019300000000005");
    PoiLayer * poiLayer = new PoiLayer(this);
    QString id = poiGraphics->documents()[0];
    QString name;

    QtKml::KmlDocument *doc = poiGraphics->document(id);
    if (doc) {
        kmldom::FeaturePtr root = doc->rootFeature();
        if (root->IsA(kmldom::KmlDomType::Type_Folder)) {
            kmldom::FolderPtr folder = kmldom::AsFolder(root);
            name = folder->get_name().c_str();
        }
    } else {
        name = QString("Unknown %1").arg(rand());
    }

    if (id.isEmpty()) {
        id = name;
    }

    qCDebug(PoiLayerControllerLog) << "Layer name " << name << " layer ID " << id;
    poiLayer->setId(id);
    poiLayer->append(elements);
    poiLayer->setKmlGraphics(poiGraphics);
    poiLayer->setName(name);

    poiLayer->setVisible(true);

    // mapPolygon->loadKMLOrSHPFile("/opt/workspace/projects/drones/qgmewamed/kml/polygon_mined-area.kml");
    // poiLayer->append(loadKML("/opt/workspace/projects/drones/qgmewamed/kml/ext_milstd.kml"));
    // poiLayer->append(loadKML("/opt/workspace/projects/drones/qgmewamed/kml/tic~mip31_29100019301000000001.kml"));

    addPoiLayer(poiLayer);

    // PoiLayer *poiLayer2 = new PoiLayer(this);
    // poiLayer2->setName("Most Poniatowskiego");
    // poiLayer2->setVisible(true);
    //
    // // poiLayer2->append(loadKML("/opt/workspace/projects/drones/qgmewamed/kml/bridge-area.kml"));
    // poiLayer2->append(loadKML("/opt/workspace/projects/drones/qgmewamed/kml/bridge.kml"));
    // // poiLayer2->polylines()->append(loadKML("/opt/workspace/projects/drones/qgmewamed/kml/bridge.kml"));
    // poiLayer2->append(loadKML("/opt/workspace/projects/drones/qgmewamed/kml/bridge2.kml"));

    // loadKML("/opt/workspace/projects/drones/qgmewamed/kml/polygon_mined-area.kml");
    // loadKML("/opt/workspace/projects/drones/qgmewamed/kml/lake.kml");

    // _poiLayers.append(poiLayer2);
    // emit poiLayerAdded(poiLayer);

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

QList<QGeoCoordinate> PoiLayerController::map(const QtKml::KmlElement::KmlVertices &vector) const
{
    QList<QGeoCoordinate> coordinates;
    for (auto vertex : vector) {
        // qCDebug(PoiLayerControllerLog) << "vertex:" << vertex;
        QGeoCoordinate coordinate(vertex.latitude, vertex.longitude);
        coordinates.append(coordinate);
    }
    return coordinates;
}

QSharedPointer<QtKml::KmlQmlGraphics> PoiLayerController::loadKML(const QString &kmlFile)
{
    QList<QtKml::KmlElement *> mapElements;
    qCDebug(PoiLayerControllerLog) << "PoiLayerController::loadKML:" << kmlFile;

    QFile kml(kmlFile);
//    Q_ASSERT(kml.exists());
    kml.open(QIODevice::ReadOnly | QIODevice::Text);
    QtKml::KmlDocument* document(new QtKml::KmlDocument());
    QSharedPointer<QtKml::KmlQmlGraphics> graphics(new QtKml::KmlQmlGraphics());

    QString errorString;
    bool result = document->open(kml, &errorString);

    if (result) {
        qCDebug(PoiLayerControllerLog) << "KML document loaded successfully";
    } else {
        qCDebug(PoiLayerControllerLog) << "KML document failed to load:" << errorString;
    }

    if (!graphics->append(document)) {
        qCWarning(PoiLayerControllerLog) << "Failed to append document to graphics from " << kmlFile;
    }
    kml.close();

    // document->elements();
    return graphics;

    /*
    QScopedPointer<QtKml::KmlQmlGraphics>::pointer x = graphics.data();

    // QList<QtKml::KmlElement *> mapElements;

    // Assuming graphics is a QScopedPointer<QtKml::KmlQmlGraphics>
    for (QtKml::KmlQmlRenderer r : graphics->renderers()) {
        for (int i  = 0; i < r.elements().count();)
    }
    auto elements = graphics->getElements();

    for (auto element : elements) {
        mapElements.append(element);
    }



   // return graphics;
    // graphics->renderers

    QVector<QtKml::KmlElement> elements = document->elements();

    kmldom::FeaturePtr root = document->rootFeature();

    if (!root->IsA(kmldom::KmlDomType::Type_Folder)) {
        qCCritical(PoiLayerControllerLog) << "Root is not a folder";
        return nullptr;
    }
    kmldom::FolderPtr folder = kmldom::AsFolder(root);

    // iterate over elements and add element pointer to mapElements
    for (auto element : elements) {

        mapElements.append(&element);
    }


    for (int i = 0; i < folder->get_feature_array_size(); i++) {
        kmldom::FeaturePtr feature = folder->get_feature_array_at(i);

        // create QGCMapPolyline or GCMapPolygon based on element.type()
        // add to mapElements
        // qCDebug(PoiLayerControllerLog) << "element:" << element.name();
        if (feature->Type() == kmldom::KmlDomType::Type_Polygon) {
        //     QGCMapPolyline *mapPolyline = new QGCMapPolyline(this);
        //     mapPolyline->appendVertices(map(element.vertices()));
        //     mapElements.append(mapPolyline);
        // } else if (element.type() == QtKml::KmlElement::POLYGON) {
            kmldom::PolygonPtr polygon = kmldom::AsPolygon(feature);

            QGCMapPolygon *mapPolygon = new QGCMapPolygon(this);
            mapPolygon->appendVertices(polygon->get_tessellate()
            mapElements.append(mapPolygon);
        }
        // point as circle
        else if (element.type() == QtKml::KmlElement::POINT) {
            qCDebug(PoiLayerControllerLog) << "element.center():" << element.center();
            qCDebug(PoiLayerControllerLog) << "element.vertices():" << element.vertices();
            QGCMapCircle *mapCircle = new QGCMapCircle(element.center(), 10, this);
            mapElements.append(mapCircle);
        } else {
            qCDebug(PoiLayerControllerLog) << "unsupported element type" << element.type();
        }


    }
*/

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
                        PoiSvg *pp = new PoiSvg(poiL);
                        pp->setCenter(gc.center());
                        pp->setSrc("http://127.0.0.1:8080/rest/symbol/" + code);
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
