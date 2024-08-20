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
 *   @brief POI Layer with POI items
 *
 *   @author Bartek Zdanowski <bartek.zdanowski@pelixar.com>
 */

#include "PoiLayer.h"

#include "PoiGeom.h"
#include "PoiLayerController.h"
#include "QGCMapCircle.h"
#include "QGCMapSvgIcon.h"

PoiLayer::PoiLayer(QObject* parent)
    : QObject(parent)
{
}

PoiLayer::~PoiLayer()
{
}

void PoiLayer::setName(const QString& name)
{
    if (_name != name) {
        _name = name;
        emit nameChanged(name);
    }
}

void PoiLayer::setVisible(bool visible)
{
    qCDebug(PoiLayerControllerLog) << "PoiLayer " << _name << " setVisible:" << visible;
    if (_visible != visible) {
        _visible = visible;
        emit visibleChanged(visible);
    }
}

void PoiLayer::append(QList<PoiGeom *> poi_geoms)
{
    // appends map objects, POLYLINE to _polylines, POLYGON to _polygons, POINT to _points and emits signals
    for (PoiGeom *mapObject : poi_geoms) {
        QObject *mo = dynamic_cast<QObject *>(mapObject);
        _elements.append(mo);
    }

    if (true) {
        emit elementsChanged(&_elements);
        return;
    }
}

void PoiLayer::append(QList<QGCMapGeom *> map_geoms)
{
    // appends map objects, POLYLINE to _polylines, POLYGON to _polygons, POINT to _points and emits signals
    for (QGCMapGeom *mapObject : map_geoms) {
        QObject *mo = dynamic_cast<QObject *>(mapObject);
        _elements.append(mo);
    }

    if (true) {
        emit elementsChanged(&_elements);
        return;
    }
}

void PoiLayer::setSyncpoint(const int syncpoint) {
    if (_syncpoint != syncpoint) {
        _syncpoint = syncpoint;
        emit syncpointChanged();
    }
}
