//
// Created by zdanek on 07.08.24.
//

#ifndef SRC_FLIGHTDISPLAY_POI_DYNAMICPOIMANAGER_H_
#define SRC_FLIGHTDISPLAY_POI_DYNAMICPOIMANAGER_H_

#include "PoiLayer.h"
#include "QGCToolbox.h"

class QGCToolbox;

class DynamicPoiManager : public QGCTool
{
    Q_OBJECT

public:
    DynamicPoiManager(QGCApplication* app, QGCToolbox* toolbox);
    virtual ~DynamicPoiManager() {}
    virtual void setToolbox(QGCToolbox *toolbox);

    Q_PROPERTY(QList<PoiLayer*>          poiLayers       READ poiLayers      NOTIFY poiLayersChanged)

    QList<PoiLayer*> poiLayers(void) const { return _layers; }

public slots:
    void addLayer(PoiLayer *layer);
    void removeLayer(PoiLayer *layer);

signals:
    void poiLayersChanged();

private:
    QList<PoiLayer*> _layers;

    QGCToolbox* _toolbox;


};

#endif //SRC_FLIGHTDISPLAY_POI_DYNAMICPOIMANAGER_H_
