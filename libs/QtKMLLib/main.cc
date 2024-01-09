#include <QFile>
#include <QString>

#include "qtkml.h"
#include <iostream>
#include <kml/dom/kml22.h>
#include <qloggingcategory.h>

void loadKML(const QString &kmlFile)
{
    qDebug() << "PoiLayerController::loadKML:" << kmlFile;
    // kml lib to load

    QFile kml(kmlFile);
    Q_ASSERT(kml.exists());
    kml.open(QIODevice::ReadOnly | QIODevice::Text);
    QScopedPointer<QtKml::KmlDocument> document(new QtKml::KmlDocument());
    QString errorString;
    bool result = document->open(kml, &errorString);
    kml.close();

    if (result) {
        qDebug() << "KML document loaded successfully";
    } else {
        qDebug() << "KML document failed to load:" << errorString;
    }

    QVector<QtKml::KmlElement> elements = document->elements();
    // QmlObjectListModel *qmlObjectListModel = new QmlObjectListModel();
    for (auto element : elements) {
        std::cout << "element:" << element.type().toStdString() << std::endl;
        // std::cout << "Unknown attributes count:" << element
    }
}

int main(int argc, char *argv[])
{
    std::cout << "Hello KML!" << std::endl;

    loadKML("/opt/workspace/projects/drones/qgmewamed/kml/ext_milstd.kml");
    return 0;
}