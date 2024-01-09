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

#include <QQmlContext>
#include <QQmlApplicationEngine>

#include "QGCApplication.h"
#include "QGCLoggingCategory.h"
#include "QGCMapPolygon.h"
#include "QGCMapPolyline.h"
#include "kmldocument.h"
#include "qmlkml.h"

QGC_LOGGING_CATEGORY(PoiLayerControllerLog, "PoiLayerControllerLog")

PoiLayerController::PoiLayerController(QObject* parent)
    : QObject(parent)
{
}

PoiLayerController::~PoiLayerController()
{
}

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

void PoiLayerController::start(void)
{
    qCDebug(PoiLayerControllerLog) << "PoiLayerController::start";

    PoiLayer *poiLayer = new PoiLayer(this);
    poiLayer->setName("Pole minowe");
    poiLayer->setVisible(true);

    // mapPolygon->loadKMLOrSHPFile("/opt/workspace/projects/drones/qgmewamed/kml/polygon_mined-area.kml");
    // poiLayer->polygons()->append(loadKML("/opt/workspace/projects/drones/qgmewamed/kml/polygon_mined-area.kml"));
    // poiLayer->append(loadKML("/opt/workspace/projects/drones/qgmewamed/kml/tic~mip31_29100019301000000001.kml"));
    poiLayer->append(loadKML("/opt/workspace/projects/drones/qgmewamed/kml/ext_milstd.kml"));

    _poiLayers.append(poiLayer);

    PoiLayer *poiLayer2 = new PoiLayer(this);
    poiLayer2->setName("Most Poniatowskiego");
    poiLayer2->setVisible(true);

    // poiLayer2->polygons()->append(loadKML("/opt/workspace/projects/drones/qgmewamed/kml/bridge-area.kml"));
    // poiLayer2->polygons()->append(loadKML("/opt/workspace/projects/drones/qgmewamed/kml/bridge.kml"));
    // poiLayer2->polylines()->append(loadKML("/opt/workspace/projects/drones/qgmewamed/kml/bridge.kml"));
    // poiLayer2->polylines()->append(loadKML("/opt/workspace/projects/drones/qgmewamed/kml/bridge2.kml"));

    // loadKML("/opt/workspace/projects/drones/qgmewamed/kml/polygon_mined-area.kml");
    // loadKML("/opt/workspace/projects/drones/qgmewamed/kml/lake.kml");

    _poiLayers.append(poiLayer2);
    emit poiLayersChanged(&_poiLayers);
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

QList<QGCMapGeom *> PoiLayerController::loadKML(const QString &kmlFile)
{

    QList<QGCMapGeom*> mapElements;
    qCDebug(PoiLayerControllerLog) << "PoiLayerController::loadKML:" << kmlFile;
    // kml lib to load

    QFile kml(kmlFile);
    Q_ASSERT(kml.exists());
    kml.open(QIODevice::ReadOnly | QIODevice::Text);
    QScopedPointer<QtKml::KmlDocument> document(new QtKml::KmlDocument());
    QString errorString;
    bool result = document->open(kml, &errorString);
    kml.close();

    if (result) {
        qCDebug(PoiLayerControllerLog) << "KML document loaded successfully";
    } else {
        qCDebug(PoiLayerControllerLog) << "KML document failed to load:" << errorString;
    }

    QVector<QtKml::KmlElement> elements = document->elements();
    // QmlObjectListModel *qmlObjectListModel = new QmlObjectListModel();
    for (auto element : elements) {

        // create QGCMapPolyline or GCMapPolygon based on element.type()
        // add to mapElements
        // qCDebug(PoiLayerControllerLog) << "element:" << element.name();
        if (element.type() == QtKml::KmlElement::POLYLINE) {
            QGCMapPolyline *mapPolyline = new QGCMapPolyline(this);
            mapPolyline->appendVertices(map(element.vertices()));
            mapElements.append(mapPolyline);
        } else if (element.type() == QtKml::KmlElement::POLYGON) {
            QGCMapPolygon *mapPolygon = new QGCMapPolygon(this);
            mapPolygon->appendVertices(map(element.vertices()));
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

    return mapElements;
}
