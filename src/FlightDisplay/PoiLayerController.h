/****************************************************************************
*
* (c) 2009-2022 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
*
* QGroundControl is licensed according to the terms in the file
* COPYING.md in the root of the source code directory.
*
****************************************************************************/

/**
 * @file
 *   @brief POI Layer Controller
 *
 *   @author Bartek Zdanowski <bartek.zdanowski@pelixar.com>
 */

#pragma once

#include <QImage>
#include <QObject>

#include "QGCLoggingCategory.h"
#include "QGCMapLayer.h"
#include "QmlObjectListModel.h"
#include "qtkml.h"
#include <qgeocoordinate.h>
#include "PoiLayer.h"

Q_DECLARE_LOGGING_CATEGORY(PoiLayerControllerLog)

class PoiLayerController : public QObject
{
    Q_OBJECT

public:
    PoiLayerController(QObject* parent = nullptr);
    ~PoiLayerController();

    Q_PROPERTY(bool poiLayerVisible READ poiLayerVisible WRITE setPoiLayerVisible NOTIFY poiLayerVisibleChanged)
    Q_PROPERTY(QmlObjectListModel* poiLayers READ poiLayers NOTIFY poiLayersChanged)

    /// Should be called immediately upon Component.onCompleted.
    Q_INVOKABLE void start();

    bool poiLayerVisible() const;
    void setPoiLayerVisible(bool poiLayerVisible);

signals:
    void poiLayerVisibleChanged(bool poiLayerVisible);
    void poiLayersChanged();
    void poiLayerAdded(PoiLayer* poiLayer);

private:
    //return poiLayers
    QmlObjectListModel *poiLayers() { return &_poiLayers; }
    QList<QGeoCoordinate> map(const QtKml::KmlElement::KmlVertices & vector) const;
    QGeoCoordinate map(const QGeoCoordinate &coordinate) const;
    QSharedPointer<QtKml::KmlQmlGraphics> loadKML(const QString &kmlFile);

    bool _poiLayerVisible = true;
    QmlObjectListModel _poiLayers;
};

