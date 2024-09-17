//
// Created by zdanek on 07.08.24.
//

#include "DynamicPoiManager.h"
#include <QCoreApplication>

QGC_LOGGING_CATEGORY(DynamicPoiManagerLog, "DynamicPoiManagerLog")

DynamicPoiManager::DynamicPoiManager(QGCApplication *app, QGCToolbox *toolbox)
    : QGCTool(app, toolbox)
{
    qCDebug(DynamicPoiManagerLog) << "DynamicPoiManager::DynamicPoiManager";
}

void DynamicPoiManager::setToolbox(QGCToolbox *toolbox)
{
    QGCTool::setToolbox(toolbox);
}

void DynamicPoiManager::addLayer(PoiLayer *pLayer) {
    addLayer(pLayer, true);
}

void DynamicPoiManager::removeLayer(PoiLayer *layer) {
    _layers.removeOne(layer);

    //emit poiLayersChanged();
}

void DynamicPoiManager::removeLayerById(QString id) {
    deletePoiLayerById(id);
    emit poiLayersChanged();
}

void DynamicPoiManager::deletePoiLayerById(QString id)
{
    qCDebug(DynamicPoiManagerLog) << "DynamicPoiManager::deletePoiLayerById: " << id;
    for (int i = 0; i < _layers.count(); i++) {
        PoiLayer *poiLayer = dynamic_cast<PoiLayer *>(_layers.at(i));
        if (poiLayer->id() == id) {
            poiLayer->setVisible(false);
            _layers.removeAt(i);
            poiLayer->deleteLater();
            emit poiLayer->deleted();
            emit poiLayerRemoved(poiLayer);
            return;
        }
    }
}

void DynamicPoiManager::addLayers(QList<PoiLayer *> layers) {
    for (int i = 0; i < layers.count(); i++) {
        addLayer(layers.at(i), false);
    }
    emit poiLayersChanged();
}

void DynamicPoiManager::addLayer(PoiLayer *pLayer, bool emitSignal) {
    qCDebug(DynamicPoiManagerLog) << "DynamicPoiManager::addPoiLayer _layers.count():" << _layers.count();
    for (int i = 0; i < _layers.count(); i++) {
        PoiLayer *poiLayer = dynamic_cast<PoiLayer *>(_layers.at(i));
        if (poiLayer->id() == pLayer->id()) {
            qCDebug(DynamicPoiManagerLog) << "DynamicPoiManager::addPoiLayer: Layer with ID " << pLayer->id() << " already exists";
            deletePoiLayerById(pLayer->id());
        }
    }

    _layers.append(pLayer);
    if (emitSignal) {
        emit poiLayersChanged();
    }
}
