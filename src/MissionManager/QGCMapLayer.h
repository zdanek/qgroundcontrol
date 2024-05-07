#pragma once

#include <QGCMapGeom.h>
#include <QString>
#include "qtkml.h"

class QGCMapLayer : public QObject
{
    Q_OBJECT

public:
    QGCMapLayer(QObject *parent = nullptr);
    void setKmlElements(QList<QtKml::KmlElement> &kml_elements);
    QList<QtKml::KmlElement> kmlElements() const { return kml_elements_; }
    QString id() const { return id_; }
    void setId(const QString &id) { id_ = id; }
    bool visible() const { return visible_; }
    void setVisible(bool visible) { visible_ = visible; }
    QString name() const { return name_; }
    void setName(const QString &name) { name_ = name; }

private:
    QString id_;
    QString name_;
    bool visible_;
    QList<QGCMapGeom *> map_geoms_;
    QList<QtKml::KmlElement> kml_elements_;
};
