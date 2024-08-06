//
// Created by zdanek on 30.07.24.
//

#ifndef SRC_FLIGHTDISPLAY_PoiPoint_H_
#define SRC_FLIGHTDISPLAY_PoiPoint_H_

#include <QGeoCoordinate>
#include <QObject>

#include "PoiGeom.h"


class PoiPoint : public QObject, public PoiGeom {
    Q_OBJECT

public:
    explicit PoiPoint(QObject* parent = nullptr);

    Q_PROPERTY(QGeoCoordinate       center          READ center                                 NOTIFY centerChanged)
    Q_PROPERTY(int                  type            READ type                                   CONSTANT)

    PoiGeomType geomType() const override { return gPoiPoint; }
    void setCenter(const QGeoCoordinate &coordinate) { _coordinate = coordinate; emit centerChanged(); }
    const QGeoCoordinate center() const { return _coordinate; }

signals:
    void centerChanged(void);

private:
    QGeoCoordinate _coordinate;
};


#endif //SRC_FLIGHTDISPLAY_PoiPoint_H_
