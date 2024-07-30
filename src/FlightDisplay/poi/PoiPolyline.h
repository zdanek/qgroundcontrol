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
    Q_PROPERTY(QVariantMap          styles          READ styles                                 WRITE setStyles)

    void appendVertices(const QList<QGeoCoordinate>& coordinates);
    Q_INVOKABLE void appendVertices(const QVariantList& varCoords);

    PoiGeomType geomType() const override { return gPoiPolyline; }
    QVariantList        path        (void) const { return _polygonPath; }
    QVariantMap        styles      (void) const { return _styles; }
    void setStyles(const QVariantMap& styles) { _styles = styles; emit pathChanged(); }
    void setLineWidth(int width) { _styles["line_width"] = width; emit pathChanged(); }
    void setLineColor(const QString& color) { _styles["line_color"] = color; emit pathChanged(); }

signals:
    void pathChanged        (void);


private:
    QVariantList        _polygonPath;
    QVariantMap         _styles;
};

#endif //SRC_FLIGHTDISPLAY_POI_POIPOLYLINE_H_
