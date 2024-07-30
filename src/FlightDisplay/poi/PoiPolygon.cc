//
// Created by zdanek on 30.07.24.
//

#include "PoiPolygon.h"

#include "PoiLayerController.h"

PoiPolygon::PoiPolygon(QObject *parent)
    : QObject(parent)
{

}

void PoiPolygon::appendVertices(const QList<QGeoCoordinate> &coordinates) {

    qCDebug(PoiLayerControllerLog) << "PoiPolygon::appendVertices coordinates:" << coordinates;
    QList<QObject*> objects;

    for (const QGeoCoordinate& coordinate: coordinates) {
        //        objects.append(new QGCQGeoCoordinate(coordinate, this));
        _polygonPath.append(QVariant::fromValue(coordinate));
    }

    emit pathChanged();
}

void PoiPolygon::appendVertices(const QVariantList& varCoords)
{
    qCDebug(PoiLayerControllerLog) << "PoiPolygon::appendVertices varCoords:" << varCoords;
    QList<QGeoCoordinate> rgCoords;
    for (const QVariant& varCoord: varCoords) {
        rgCoords.append(varCoord.value<QGeoCoordinate>());
    }
    appendVertices(rgCoords);
}