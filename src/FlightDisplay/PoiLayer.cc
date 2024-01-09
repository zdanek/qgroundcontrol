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

#include "PoiLayerController.h"
#include "QGCMapCircle.h"
#include "QGCMapPolygon.h"
#include "QGCMapPolyline.h"

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

void PoiLayer::append(QList<QGCMapGeom *> mapObjects)
{
    // appends map objects, POLYLINE to _polylines, POLYGON to _polygons, POINT to _points and emits signals
    for (QGCMapGeom *mapObject : mapObjects) {

        QGCMapGeomType type = mapObject->geomType();

        switch (type) {
            case QGCMapGeomType::Polygon: {
                // c++ cast to QGCMapPolyline
                QGCMapPolygon *polygonX = dynamic_cast<QGCMapPolygon *>(mapObject);
                QGCMapPolygon *polygon = qobject_cast<QGCMapPolygon *>(polygonX);
                _polygons.append(polygon);
                break;
            }
            case QGCMapGeomType::Circle: {
                QGCMapCircle *polygonX = dynamic_cast<QGCMapCircle *>(mapObject);
                QGCMapCircle *polygon = qobject_cast<QGCMapCircle *>(polygonX);
                _circles.append(polygon);
                break;
            }
            case QGCMapGeomType::Point: {
                QGCMapCircle *polygonX = dynamic_cast<QGCMapCircle *>(mapObject);
                QGCMapCircle *polygon = qobject_cast<QGCMapCircle *>(polygonX);
                _circles.append(polygon);
                break;
            }
            case QGCMapGeomType::Polyline: {
                QGCMapPolyline *polylineX = dynamic_cast<QGCMapPolyline *>(mapObject);
                QGCMapPolyline *polyline = qobject_cast<QGCMapPolyline *>(polylineX);
                _polylines.append(polyline);
                break;
            }
            default:
                qCWarning(PoiLayerControllerLog) << "PoiLayer " << _name << " append: unknown map object type " << type;
        }
    }
}

const PoiLayer& PoiLayer::operator=(const PoiLayer& other)
{
    clear();

    _name = other._name;
    _visible = other._visible;
    //TODO(bzd) copy polygons?
    _polygons.append(other._polygons.children());
    _polylines.append(other._polylines.children());

    //TODO(bzd) czy ustawiac dirty jak np w QGCMapPolygon::operator= ?
    return *this;
}

void PoiLayer::clear(void)
{
    _polygons.clearAndDeleteContents();
    _polylines.clearAndDeleteContents();
    emit polygonsChanged(&_polygons);
    emit polylinesChanged(&_polylines);

    //TODO(bzd) dlaczego jest w innych klasach ustawiane dirtyy? np w GQCMapPolygon:95
}