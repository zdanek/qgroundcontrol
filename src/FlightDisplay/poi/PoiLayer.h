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

#include <QObject>

#include "PoiGeom.h"
#include "QGCMapGeom.h"
#include "QmlObjectListModel.h"

class PoiLayer : public QObject
{
    Q_OBJECT
public:

    PoiLayer(QObject* parent = nullptr);
    ~PoiLayer();
    const PoiLayer& operator=(const PoiLayer& other);

    QString id() const { return _id;}

    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(QString id READ id)
    Q_PROPERTY(bool canBeDeleted READ canBeDeleted NOTIFY canBeDeletedChanged)
    Q_PROPERTY(bool visible READ visible WRITE setVisible NOTIFY visibleChanged)
    Q_PROPERTY(QmlObjectListModel *elements READ elements NOTIFY elementsChanged)

    void setId(const QString& id) { _id = id; }
    void setSyncpoint(const int syncpoint);

    QString name() const { return _name; }
    void setName(const QString& name);

    bool canBeDeleted() const { return _canBeDeleted; }
    void setCanBeDeleted(bool canBeDeleted);

    bool visible() const { return _visible; }
    void setVisible(bool visible);

    void append(QList<PoiGeom *> poi_geoms);
    void append(QList<QGCMapGeom *> map_geoms);

signals:
    void nameChanged(const QString& name);
    void visibleChanged(bool visible);
    void canBeDeletedChanged(bool canBeDeleted);
    void elementsChanged(const QmlObjectListModel &elements);
    void deleted();
    void syncpointChanged();

private:
    QmlObjectListModel* elements() { return &_elements;}

    QmlObjectListModel _elements;
    QString _name;
    QString _id;
    bool _canBeDeleted = false;
    bool _visible = false;
    int _syncpoint = -1;
};
