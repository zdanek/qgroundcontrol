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

#include "DynamicPoiManager.h"
#include "PoiLayer.h"
#include "QGCApplication.h"
#include "QGCLoggingCategory.h"
#include "QGCToolbox.h"
#include "QmlObjectListModel.h"
#include "qgeocoordinate.h"

Q_DECLARE_LOGGING_CATEGORY(PoiLayerControllerLog)

class PoiLayerController : public QObject
{
    Q_OBJECT

public:
    explicit PoiLayerController(QObject *parent = nullptr);

    virtual ~PoiLayerController() {}

    Q_PROPERTY(QmlObjectListModel* poiLayers READ poiLayers NOTIFY poiLayersChanged)

    Q_INVOKABLE void start();

signals:
    void poiLayersChanged();
    void addPoiLayer(PoiLayer *pLayer);
    void deletePoiLayer(PoiLayer *pLayer);
    void deletePoiLayerById(QString id);

public slots:
    void poiLayerRemoved(const PoiLayer *layer);


private:
    QmlObjectListModel *poiLayers();

    const QGCToolbox *_toolbox;
    DynamicPoiManager *_dynamicPoiManager;
    QGCApplication *_app;

    QmlObjectListModel _poiLayers;
};

