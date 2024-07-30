//
// Created by zdanek on 30.07.24.
//

#ifndef SRC_FLIGHTDISPLAY_POI_POIPOLYLINE_H_
#define SRC_FLIGHTDISPLAY_POI_POIPOLYLINE_H_

#include <QGeoCoordinate>
#include <QObject>

#include "PoiGeom.h"
#include "QmlObjectListModel.h"

class PoiPolyline : public QObject, public PoiGeom {
    Q_OBJECT

public:
    explicit PoiPolyline(QObject* parent = nullptr);

    Q_PROPERTY(QVariantList         path            READ path                                   NOTIFY pathChanged)
    Q_PROPERTY(int                  type            READ type                                   CONSTANT)

    void appendVertices(const QList<QGeoCoordinate>& coordinates);
    Q_INVOKABLE void appendVertices(const QVariantList& varCoords);

    PoiGeomType geomType() const override { return gPoiPolyline; }
    QVariantList        path        (void) const { return _polygonPath; }

signals:
    void pathChanged        (void);


private:
    QVariantList        _polygonPath;
};

#endif //SRC_FLIGHTDISPLAY_POI_POIPOLYLINE_H_
