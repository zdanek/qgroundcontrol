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
#include "qmlkml.h"

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
    Q_PROPERTY(QtKml::KmlQmlGraphics* kmlGraphics READ kmlGraphics NOTIFY kmlGraphicsChanged)
    // Q_PROPERTY()
    // Q_PROPERTY(QmlObjectListModel* polylines READ polylines NOTIFY polylinesChanged)

    // Q_INVOKABLE void clear(void);

    QString id() const { return _id;}
    void setId(const QString& id) { _id = id; }

    QString name() const { return _name; }
    void setName(const QString& name);

    bool visible() const { return _visible; }
    void setVisible(bool visible);

    void append(QList<PoiGeom *> poi_geoms);
    void append(QList<QGCMapGeom *> map_geoms);
    void setKmlGraphics(QSharedPointer<QtKml::KmlQmlGraphics> &kml_graphics);

    QtKml::KmlQmlGraphics* kmlGraphics() { return _kmlGraphics.data(); }

signals:
    void nameChanged(const QString& name);
    void visibleChanged(bool visible);
    void elementsChanged(const QmlObjectListModel &elements);
    void kmlGraphicsChanged();
    void deleted();
    // void polylinesChanged(QmlObjectListModel* polylines);

private:
    QmlObjectListModel* elements() { return &_elements;}

    // QmlObjectListModel* polygons() { return &_polygons; }
    // QmlObjectListModel* polylines() { return &_polylines; }

    QString _name;
    QString _id;
    bool _visible = true;
    QmlObjectListModel _elements;
    QSharedPointer<QtKml::KmlQmlGraphics> _kmlGraphics;
};
