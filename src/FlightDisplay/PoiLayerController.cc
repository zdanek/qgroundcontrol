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

#include <QQmlApplicationEngine>
#include <QQmlContext>

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

bool PoiLayerController::poiLayerVisible() const
{
    return _poiLayerVisible;
}

void PoiLayerController::setPoiLayerVisible(bool poiLayerVisible)
{
    if (_poiLayerVisible != poiLayerVisible) {
        _poiLayerVisible = poiLayerVisible;
        emit poiLayerVisibleChanged(poiLayerVisible);
    }
}

void PoiLayerController::start()
{
    qCDebug(PoiLayerControllerLog) << "PoiLayerController::start";

//     QSharedPointer<QtKml::KmlQmlGraphics> poiGraphics = loadKML("/opt/workspace/projects/drones/qgmewamed/kml/ext_milstd.kml");
//    QSharedPointer<QtKml::KmlQmlGraphics> poiGraphics = loadKML("/opt/workspace/projects/drones/qgmewamed/kml/tic~mip31_29100019301000000001.kml");
//    QSharedPointer<QtKml::KmlQmlGraphics> poiGraphics = loadKML("http://localhost:8080/rest/layers/kml/get/mip31~29100019300000000000");
    QSharedPointer<QtKml::KmlQmlGraphics> poiGraphics = loadKML("http://localhost:8080/rest/layers/kml/get/mip31~29100019300000000005");
    PoiLayer * poiLayer = new PoiLayer(this);
    poiLayer->setId("tic~mip31:29100019301000000001");
    poiLayer->setKmlGraphics(poiGraphics);
    poiLayer->setName("Pole minowe");
    poiLayer->setVisible(true);

    // mapPolygon->loadKMLOrSHPFile("/opt/workspace/projects/drones/qgmewamed/kml/polygon_mined-area.kml");
    // poiLayer->append(loadKML("/opt/workspace/projects/drones/qgmewamed/kml/ext_milstd.kml"));
    // poiLayer->append(loadKML("/opt/workspace/projects/drones/qgmewamed/kml/tic~mip31_29100019301000000001.kml"));

    _poiLayers.append(poiLayer);

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


    //emit poiLayersChanged();
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
    /*

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
