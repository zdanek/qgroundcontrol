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
    GeomTypePolygon,
    GeomTypePolyline,
    GeomTypeCircle,
    GeomTypePoint,
    GeomTypeSvg,
    GeomTypeInvalid
};

class QGCMapGeom {
public:
    virtual ~QGCMapGeom() {}
    virtual QGCMapGeomType geomType() const = 0;

    int type() const{
        return (int) geomType();
    }

};