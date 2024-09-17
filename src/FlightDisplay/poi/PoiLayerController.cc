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
#include "PoiLayerController.h"

#include <QGeoCircle>
#include <QGeoPath>
#include <QGeoPolygon>
#include <QObject>
#include <QtLocation/private/qgeojson_p.h>

#include "PoiGeom.h"
#include "PoiLayer.h"
#include "PoiLayerLoader.h"
#include "PoiPoint.h"
#include "PoiPolygon.h"
#include "PoiPolyline.h"
#include "PoiSvg.h"
#include "QGCApplication.h"
#include "QGCLoggingCategory.h"
#include "DynamicPoiManager.h"

QGC_LOGGING_CATEGORY(PoiLayerControllerLog, "PoiLayerControllerLog")


PoiLayerController::PoiLayerController( QObject *parent)
    : QObject(parent)
{
    qCDebug(PoiLayerControllerLog) << "PoiLayerController::PoiLayerController";
    _dynamicPoiManager = qgcApp()->toolbox()->dynamicPoiManager();
//    _dynamicPoiManager = toolbox->dynamicPoiManager();
//    _poiLayers.setParent(this);
    connect(_dynamicPoiManager, &DynamicPoiManager::poiLayersChanged, this, &PoiLayerController::poiLayersChanged, Qt::QueuedConnection);
    connect(this, &PoiLayerController::deletePoiLayerById, _dynamicPoiManager, &DynamicPoiManager::removeLayerById);
    connect(_dynamicPoiManager, &DynamicPoiManager::poiLayerRemoved, this, &PoiLayerController::poiLayerRemoved);
//    connect(_dynamicPoiManager, &DynamicPoiManager::addLayer, this, &PoiLayerController::addPoiLayer);
//    connect(_dynamicPoiManager, &DynamicPoiManager::removeLayer, this, &PoiLayerController::deletePoiLayer);
}

void PoiLayerController::start()
{
//    qCDebug(PoiLayerControllerLog) << "PoiLayerController::start";
//
//    PoiLayerLoader pl("/opt/workspace/projects/drones/qgmewamed/geojsons/images");
//    QStringList files = QDir("/opt/workspace/projects/drones/qgmewamed/geojsons").entryList(QDir::Files);
//    for (const QString &file : files) {
//        qCDebug(PoiLayerControllerLog) << "file:" << file;
//        //bzd poi
//        PoiLayer *layer = pl.loadGeoJson("/opt/workspace/projects/drones/qgmewamed/geojsons/" + file, this);
//        if (layer) {
//            layer->setVisible(true);
//            _dynamicPoiManager->addLayer(layer);
//        }
//        //        layer = loadGeoJsonFile("/opt/workspace/projects/drones/qgmewamed/geojsons/" + file);
//    }

}
QmlObjectListModel *PoiLayerController::poiLayers()
{
    qCDebug(PoiLayerControllerLog) << "PoiLayerController::poiLayers - _dynamicPoiManager->poiLayers().count():"
                                   << _dynamicPoiManager->poiLayers().count();
    for (PoiLayer *layer : _dynamicPoiManager->poiLayers()) {
        if (_poiLayers.contains(layer)) {
            continue;
        }
        _poiLayers.append(layer);
    }
    return &_poiLayers;
}
void PoiLayerController::poiLayerRemoved(const PoiLayer *layer) {
    qCDebug(PoiLayerControllerLog) << "PoiLayerController::poiLayerRemoved";
    _poiLayers.removeOne(const_cast<PoiLayer *>(layer));
    emit poiLayersChanged();
}

/*
void PoiLayerController::deletePoiLayerById(QString id)
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
    qCDebug(PoiLayerControllerLog) << "PoiLayerController::addPoiLayer _poiLayers.count():" << _poiLayers.count();
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

void PoiLayerController::loadGeoJsonFile(const QString &geoJsonFile) {
    qCDebug(PoiLayerControllerLog) << "Signal loadGeoJsonFile:" << geoJsonFile;
    PoiLayer *poiL = PoiLayerLoader::loadGeoJson(geoJsonFile, this);
    if (poiL) {
        poiL->setVisible(true);
        addPoiLayer(poiL);
        _file_to_layer.insert(geoJsonFile, poiL->id());
    }
    emit poiLayersChanged();
}

void PoiLayerController::removeGeoJsonFile(const QString &geoJsonFile) {
    qCDebug(PoiLayerControllerLog) << "Signal removeGeoJsonFile:" << geoJsonFile;
    if (_file_to_layer.contains(geoJsonFile)) {
        deletePoiLayer(_file_to_layer[geoJsonFile]);
        _file_to_layer.remove(geoJsonFile);
    } else {
        qCDebug(PoiLayerControllerLog) << "File not found in map:" << geoJsonFile;
    }
    emit poiLayersChanged();

}
*/