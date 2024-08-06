//
// Created by zdanek on 30.07.24.
//

#ifndef SRC_FLIGHTDISPLAY_POISVG_H_
#define SRC_FLIGHTDISPLAY_POISVG_H_

#include <QGeoCoordinate>
#include <QObject>

#include "PoiGeom.h"


class PoiSvg : public QObject, public PoiGeom {
    Q_OBJECT

public:
    explicit PoiSvg(QObject* parent = nullptr);

    Q_PROPERTY(QGeoCoordinate       center          READ center                                 NOTIFY centerChanged)
    Q_PROPERTY(int                  type            READ type                                   CONSTANT)
    Q_PROPERTY(QString              src             MEMBER _src                                     NOTIFY srcChanged)

    PoiGeomType geomType() const override { return gPoiSvg; }
    void setCenter(const QGeoCoordinate &coordinate) { _coordinate = coordinate; emit centerChanged(); }
    const QGeoCoordinate center() const { return _coordinate; }
    void setSrc(const QString& src) { _src = src; emit srcChanged(); }
    const QString& src() const { return _src; }

signals:
    void centerChanged(void);
    void srcChanged(void);

private:
    QGeoCoordinate _coordinate;
    QString _src;
};


#endif //SRC_FLIGHTDISPLAY_POISVG_H_
