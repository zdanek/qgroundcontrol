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
#include <qgeocoordinate.h>

#include "QGCLoggingCategory.h"
#include "QmlObjectListModel.h"
#include "PoiLayer.h"

Q_DECLARE_LOGGING_CATEGORY(PoiLayerControllerLog)

class PoiLayerController : public QObject
{
    Q_OBJECT

public:
    PoiLayerController(QObject* parent = nullptr);
    virtual ~PoiLayerController() {}

    Q_PROPERTY(QmlObjectListModel* poiLayers READ poiLayers NOTIFY poiLayersChanged)

    /// Should be called immediately upon Component.onCompleted.
    Q_INVOKABLE void start();
    Q_INVOKABLE void deletePoiLayer(QString id);

signals:
    void poiLayersChanged();

public slots:
    void loadGeoJsonFile(const QString &geoJsonFile);
    void removeGeoJsonFile(const QString &geoJsonFile);

private:
    QmlObjectListModel *poiLayers() { return &_poiLayers; }
    void addPoiLayer(PoiLayer *pLayer);

    QmlObjectListModel _poiLayers;
    QMap<QString, QString> _file_to_layer;
};

