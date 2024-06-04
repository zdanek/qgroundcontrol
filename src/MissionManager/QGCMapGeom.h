/****************************************************************************
*
 * (c) 2009-2020 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/


#pragma once

#include <QObject>

enum QGCMapGeomType {
    Polygon,
    Polyline,
    Circle,
    Point,
    Svg,
    Invalid
};

class QGCMapGeom {
public:
    virtual ~QGCMapGeom() {}
    virtual QGCMapGeomType geomType() const = 0;

};