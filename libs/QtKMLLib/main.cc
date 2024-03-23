#include <QFile>
#include <QString>

#include "qtkml.h"
#include <iostream>
#include <kml/dom.h>
#include <qloggingcategory.h>

void dump_attributes(const kmldom::ElementPtr element)
{
    if (element->GetUnknownAttributes() == nullptr) {
        return;
    }

    std::cout << "Has unknown attributes " << element->GetUnknownAttributes()->GetSize() << std::endl;

    kmlbase::StringMapIterator iterator = element->GetUnknownAttributes()->CreateIterator();
    for (; !iterator.AtEnd(); iterator.Advance()) {
        std::cout << "attribute: " << iterator.Data().first << "= " << iterator.Data().second << std::endl;
    }
}
void dump_unknown_elements(const kmldom::ElementPtr & element)
{
    std::cout << "Has unknown elements " << element->get_unknown_elements_array_size() << std::endl;
    for (int i = 0; i < element->get_unknown_elements_array_size(); i++) {
        std::cout << "Unknown element: " << element->get_unknown_elements_array_at(i) << std::endl;
    }

}

void dump_misplaced_elements(const kmldom::ElementPtr &element)
{
    std::cout << "Has misplaced elements " << element->get_misplaced_elements_array_size() << std::endl;
    for (int i = 0; i < element->get_misplaced_elements_array_size(); i++) {
        std::cout << "Misplaced element: " << element->get_misplaced_elements_array_at(i) << std::endl;
        std::cout << "Misplaced element data: " << element->get_misplaced_elements_array_at(i)->get_char_data() << std::endl;
    }
}

void dump_symbol_info(const kmldom::ExtSymbolInfoPtr & element)
{
    std::cout << "Symbol info has standard " << element->has_standard() << std::endl;
    if (element->has_standard()) {
        std::cout << "Symbol info standard " << element->get_standard() << std::endl;
    }
    std::cout << "Symbol info has code " << element->has_code() << std::endl;
    if (element->has_code()) {
        std::cout << "Symbol info code " << element->get_code() << std::endl;
    }

    if (element->has_coordinates()) {
        std::cout << "Symbol info has coordinates: " << element->get_coordinates().get_latitude() << ", " << element->get_coordinates().get_longitude() << std::endl;
    }

    std::cout << "Symbol info has ext data " << element->has_extendeddata() << std::endl;

    dump_attributes(element);
    dump_unknown_elements(element);
    dump_misplaced_elements(element);
}

void dump_folder(kmldom::FolderPtr folder)
{
    std::cout << "Folder name:" << folder->get_name() << std::endl;
    std::cout << "Folder has ext data " << folder->has_extendeddata() << std::endl;
    std::cout << "Folder has features " << folder->get_feature_array_size() << std::endl;

    dump_attributes(folder);
    dump_unknown_elements(folder);
    dump_misplaced_elements(folder);

    for (int i = 0; i < folder->get_feature_array_size(); i++) {
        kmldom::FeaturePtr feature = folder->get_feature_array_at(i);
        std::cout << "Feature name: " << feature->get_name() << std::endl;
        std::cout << "Feature has type  " << kmldom::kmlDomTypeToString(feature->Type()) << std::endl;

        if (feature->IsA(kmldom::KmlDomType::Type_Folder)) {
            kmldom::FolderPtr featureFolder = kmldom::AsFolder(feature);
            std::cout << "Feature is a folder" << std::endl;
            dump_folder(featureFolder);
        } else if (feature->IsA(kmldom::KmlDomType::Type_ExtSymbolInfo)) {
            kmldom::ExtSymbolInfoPtr symbolInfo = kmldom::AsExtSymbolInfo(feature);
            std::cout << "Feature is a symbol info" << std::endl;
            dump_symbol_info(symbolInfo);
        }
        else {
            std::cout << "Unknown feature type: " << feature->Type() << std::endl;
        }

        std::cout << ". "<< std::endl;
    }
}





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
        qDebug() << "Potential errors: " << errorString;
    } else {
        qDebug() << "KML document failed to load:" << errorString;
    }

    kmldom::FeaturePtr root = document->rootFeature();
    // QmlObjectListModel *qmlObjectListModel = new QmlObjectListModel();
    // iterate over the root feature
    if (root) {
        std::cout << "Root feature:" << root->get_name() << std::endl;
    } else {
        std::cout << "Root feature is null" << std::endl;
    }

    std::cout << "has ext data" << root->has_extendeddata() << std::endl;
    std::cout << "root attributes " << root->GetUnknownAttributes()->GetSize();

    // for (int i = 0; i < root->GetUnknownAttributes()->GetSize(); i++) {
    //     std::cout << "element:" << root->GetUnknownAttributes()->GetValue(i) << std::endl;
    // }
    kmlbase::StringMapIterator iterator = root->GetUnknownAttributes()->CreateIterator();

    for (; !iterator.AtEnd(); iterator.Advance()) {
        std::cout << "attribute:" << iterator.Data().first <<  " = " << iterator.Data().second << std::endl;
    }

    if (root->IsA(kmldom::KmlDomType::Type_Folder)) {
        std::cout << "Root is a folder" << std::endl;
    } else {
        std::cout << "Root is not a folder" << std::endl;
        return;
    }
    kmldom::FolderPtr folder = kmldom::AsFolder(root);
    std::cout << "Folder name:" << folder->get_name() << std::endl;
    std::cout << "Folder has ext data " << folder->has_extendeddata() << std::endl;
    std::cout << "Folder has features " << folder->get_feature_array_size() << std::endl;

    for (int i = 0; i < folder->get_feature_array_size(); i++) {
        kmldom::FeaturePtr feature = folder->get_feature_array_at(i);
        std::cout << "Feature name: " << feature->get_name() << std::endl;
        std::cout << "Feature has type  " << kmldom::kmlDomTypeToString(feature->Type()) << std::endl;


        if (feature->IsA(kmldom::KmlDomType::Type_Folder)) {
            kmldom::FolderPtr featureFolder = kmldom::AsFolder(feature);
            std::cout << "Feature is a folder" << std::endl;
            std::cout << "Feature folder name:" << featureFolder->get_name() << std::endl;
            std::cout << "Feature folder has ext data " << featureFolder->has_extendeddata() << std::endl;
            std::cout << "Feature folder has features " << featureFolder->get_feature_array_size() << std::endl;
            std::cout << "Feature has unknown attributes " << featureFolder->GetUnknownAttributes()->GetSize() << std::endl;
            kmlbase::StringMapIterator iterator = featureFolder->GetUnknownAttributes()->CreateIterator();
            for (; !iterator.AtEnd(); iterator.Advance()) {
                std::cout << "attribute: " << iterator.Data().first <<  "= " << iterator.Data().second << std::endl;
            }

            std::cout << "Feature has unknown elements " << featureFolder->get_unknown_elements_array_size() << std::endl;
            for (int i = 0; i < featureFolder->get_unknown_elements_array_size(); i++) {
                std::cout << "Unknown element: " << featureFolder->get_unknown_elements_array_at(i) << std::endl;
            }

            std::cout << "Feature has misplaced elements " << featureFolder->get_misplaced_elements_array_size() << std::endl;
            for (int i = 0; i < featureFolder->get_misplaced_elements_array_size(); i++) {
                std::cout << "Misplaced element: " << featureFolder->get_misplaced_elements_array_at(i) << std::endl;
            }
        }

    }


    // for (auto element : root->get_name()
    //     std::cout << "element:" << element->type().toStdString() << std::endl;
    //     // std::cout << "Unknown attributes count:" << element
    // }
    //
    //
    // for (auto element : root->) {
    //     std::cout << "element:" << element.type().toStdString() << std::endl;
    //     // std::cout << "Unknown attributes count:" << element
    // }
}

int main(int argc, char *argv[])
{
    std::cout << "Hello KML!" << std::endl;

    loadKML("/opt/workspace/projects/drones/qgmewamed/kml/ext_milstd.kml");
    // loadKML("/opt/workspace/projects/drones/qgmewamed/kml/tic~mip31_29100019301000000001.kml");
    return 0;
}