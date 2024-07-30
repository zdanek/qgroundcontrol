//
// Created by zdanek on 30.07.24.
//

#ifndef SRC_FLIGHTDISPLAY_POIPOLYGON_H_
#define SRC_FLIGHTDISPLAY_POIPOLYGON_H_

#include <QGeoCoordinate>
#include <QObject>

#include "PoiGeom.h"
#include "QmlObjectListModel.h"


class PoiPolygon : public QObject, public PoiGeom {
    Q_OBJECT

public:
    explicit PoiPolygon(QObject* parent = nullptr);

    Q_PROPERTY(QVariantList         path            READ path                                   NOTIFY pathChanged)
    Q_PROPERTY(int                  type            READ type                                   CONSTANT)

    void appendVertices(const QList<QGeoCoordinate>& coordinates);
    Q_INVOKABLE void appendVertices(const QVariantList& varCoords);

    PoiGeomType geomType() const override { return gPoiPolygon; }
    QVariantList        path        (void) const { return _polygonPath; }

signals:
    void pathChanged        (void);


private:
    QVariantList        _polygonPath;

};

#endif //SRC_FLIGHTDISPLAY_POIPOLYGON_H_
