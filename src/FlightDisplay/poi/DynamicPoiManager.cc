//
// Created by zdanek on 07.08.24.
//

#include "DynamicPoiManager.h"

DynamicPoiManager::DynamicPoiManager(QGCApplication *app, QGCToolbox *toolbox)
    : QGCTool(app, toolbox)
{}

void DynamicPoiManager::setToolbox(QGCToolbox *toolbox)
{
    QGCTool::setToolbox(toolbox);
}

void DynamicPoiManager::addLayer(PoiLayer *layer) {
    _layers.append(layer);
    emit poiLayersChanged();
}

void DynamicPoiManager::removeLayer(PoiLayer *layer) {
    _layers.removeOne(layer);
    emit poiLayersChanged();
}
