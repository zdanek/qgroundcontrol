//
// Created by zdanek on 07.08.24.
//

#ifndef SRC_FLIGHTDISPLAY_POI_DYNAMICPOIMANAGER_H_
#define SRC_FLIGHTDISPLAY_POI_DYNAMICPOIMANAGER_H_

#include "PoiLayer.h"
#include "QGCToolbox.h"
#include "QGCLoggingCategory.h"

class QGCToolbox;

Q_DECLARE_LOGGING_CATEGORY(DynamicPoiManagerLog)

class DynamicPoiManager : public QGCTool
{
    Q_OBJECT

public:
    DynamicPoiManager(QGCApplication* app, QGCToolbox* toolbox);
    virtual ~DynamicPoiManager() {}
    virtual void setToolbox(QGCToolbox *toolbox);

    Q_PROPERTY(QList<PoiLayer*>          poiLayers       READ poiLayers      NOTIFY poiLayersChanged)

    QList<PoiLayer*> poiLayers(void) const {
        return _layers;
    }

public slots:
    void addLayer(PoiLayer *layer);
    void addLayers(QList<PoiLayer *>layers);
    void removeLayer(PoiLayer *layer);
    void removeLayerById(QString id);

signals:
    void poiLayersChanged();
    void poiLayerRemoved(const PoiLayer *layer);

private:
    /**
     * Deletes a POI layer by its ID
     *
     * @param id
     */
    void deletePoiLayerById(QString id);
    void addLayer(PoiLayer *pLayer, bool emitSignal);

    QList<PoiLayer*> _layers;
//    QmlObjectListModel _poiLayers;
    QGCToolbox* _toolbox;


};

#endif //SRC_FLIGHTDISPLAY_POI_DYNAMICPOIMANAGER_H_
