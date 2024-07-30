//
// Created by zdanek on 30.07.24.
//

#include "PoiPoint.h"

#include "PoiLayerController.h"

PoiPoint::PoiPoint(QObject *parent)
    : QObject(parent)
{
    setLineWidth(2);
    setLineColor("black");

}

void PoiPoint::appendVertices(const QList<QGeoCoordinate> &coordinates) {

    qCDebug(PoiLayerControllerLog) << "PoiPoint::appendVertices coordinates:" << coordinates;
    QList<QObject*> objects;

    for (const QGeoCoordinate& coordinate: coordinates) {
        //        objects.append(new QGCQGeoCoordinate(coordinate, this));
        _polygonPath.append(QVariant::fromValue(coordinate));
    }

    emit pathChanged();
}

void PoiPoint::appendVertices(const QVariantList& varCoords)
{
    qCDebug(PoiLayerControllerLog) << "PoiPoint::appendVertices varCoords:" << varCoords;
    QList<QGeoCoordinate> rgCoords;
    for (const QVariant& varCoord: varCoords) {
        rgCoords.append(varCoord.value<QGeoCoordinate>());
    }
    appendVertices(rgCoords);
}