#include "QGCMapLayer.h"

QGCMapLayer::QGCMapLayer(QObject *parent)
    : QObject(parent)
{}

void QGCMapLayer::setKmlElements(QList<QtKml::KmlElement> &kml_elements) {
    kml_elements_.clear();
    kml_elements_ = kml_elements;
}