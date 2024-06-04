//
// Created by zdanek on 15.05.24.
//

#ifndef MISSIONMANAGER_QGC_MAP_SVG_ICON_H_
#define MISSIONMANAGER_QGC_MAP_SVG_ICON_H_

#include "QGCMapGeom.h"
#include <QObject>

class QGCMapSvgIcon: public QObject, public QGCMapGeom
{
    Q_OBJECT
public:
    QGCMapSvgIcon();

};

#endif //MISSIONMANAGER_QGC_MAP_SVG_ICON_H_
