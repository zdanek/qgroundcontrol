//
// Created by zdanek on 30.07.24.
//

#include "PoiPolyline.h"
#include "PoiLayerController.h"

PoiPolyline::PoiPolyline(QObject *parent)
    : QObject(parent)
{

}

void PoiPolyline::appendVertices(const QList<QGeoCoordinate> &coordinates) {

    qCDebug(PoiLayerControllerLog) << "PoiPolyline::appendVertices coordinates:" << coordinates;
    QList<QObject*> objects;

    for (const QGeoCoordinate& coordinate: coordinates) {
//        objects.append(new QGCQGeoCoordinate(coordinate, this));
        _polygonPath.append(QVariant::fromValue(coordinate));
    }

    emit pathChanged();
}

void PoiPolyline::appendVertices(const QVariantList& varCoords)
{
    qCDebug(PoiLayerControllerLog) << "PoiPolyline::appendVertices varCoords:" << varCoords;
    QList<QGeoCoordinate> rgCoords;
    for (const QVariant& varCoord: varCoords) {
        rgCoords.append(varCoord.value<QGeoCoordinate>());
    }
    appendVertices(rgCoords);
}