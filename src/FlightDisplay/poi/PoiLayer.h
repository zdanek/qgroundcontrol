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

    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(QString id READ id)
    Q_PROPERTY(bool visible READ visible WRITE setVisible NOTIFY visibleChanged)
    Q_PROPERTY(QmlObjectListModel* elements READ elements NOTIFY elementsChanged)

    QString id() const { return _id;}
    void setId(const QString& id) { _id = id; }

    QString name() const { return _name; }
    void setName(const QString& name);

    bool visible() const { return _visible; }
    void setVisible(bool visible);

    void append(QList<PoiGeom *> poi_geoms);
    void append(QList<QGCMapGeom *> map_geoms);

signals:
    void nameChanged(const QString& name);
    void visibleChanged(bool visible);
    void elementsChanged(const QmlObjectListModel &elements);
    void kmlGraphicsChanged();
    void deleted();

private:
    QmlObjectListModel* elements() { return &_elements;}
    QString _name;
    QString _id;
    bool _visible = true;
    QmlObjectListModel _elements;
};
