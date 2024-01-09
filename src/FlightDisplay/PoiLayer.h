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
 *   @brief POI Layer with POI items
 *
 *   @author Bartek Zdanowski <bartek.zdanowski@pelixar.com>
 */

#pragma once

#include <QGCMapGeom.h>
#include <QObject>

#include "QmlObjectListModel.h"

class PoiLayer : public QObject
{
    Q_OBJECT

public:
    PoiLayer(QObject* parent = nullptr);
    ~PoiLayer();

    const PoiLayer& operator=(const PoiLayer& other);

    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(bool visible READ visible WRITE setVisible NOTIFY visibleChanged)
    Q_PROPERTY(QmlObjectListModel* polygons READ polygons NOTIFY polygonsChanged)
    Q_PROPERTY(QmlObjectListModel* polylines READ polylines NOTIFY polylinesChanged)

    Q_INVOKABLE void clear(void);

    QString name() const { return _name; }
    void setName(const QString& name);

    bool visible() const { return _visible; }
    void setVisible(bool visible);

    void append(QList<QGCMapGeom*> mapObjects);

signals:
    void nameChanged(const QString& name);
    void visibleChanged(bool visible);
    void polygonsChanged(QmlObjectListModel* polygons);
    void polylinesChanged(QmlObjectListModel* polylines);

private:
    QmlObjectListModel* polygons() { return &_polygons; }
    QmlObjectListModel* polylines() { return &_polylines; }

    QString _name;
    bool _visible = true;
    QmlObjectListModel _polygons;
    QmlObjectListModel _circles;
    QmlObjectListModel _polylines;
    QmlObjectListModel _points;

};
