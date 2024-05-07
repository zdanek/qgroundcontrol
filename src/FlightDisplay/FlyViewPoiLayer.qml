/****************************************************************************
 *
 * (c) 2009-2022 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

import QtQuick                  2.12
import QtQuick.Controls         2.4
import QtQuick.Dialogs          1.3
import QtQuick.Layouts          1.12

import QtLocation               5.3
import QtPositioning            5.3
import QtQuick.Window           2.2
import QtQml.Models             2.1

import QGroundControl               1.0
import QGroundControl.Airspace      1.0
import QGroundControl.Airmap        1.0
import QGroundControl.Controllers   1.0
import QGroundControl.Controls      1.0
import QGroundControl.FactSystem    1.0
import QGroundControl.FlightDisplay 1.0
import QGroundControl.FlightMap     1.0
import QGroundControl.Palette       1.0
import QGroundControl.ScreenTools   1.0
import QGroundControl.Vehicle       1.0

Item {
    id: _root

//    property var parentToolInsets               // These insets tell you what screen real estate is available for positioning the controls in your overlay
//    property var totalToolInsets:   _toolInsets // These are the insets for your custom overlay additions
    property var mapControl
    property color  interiorColor:      "transparent"
    property color  altColor:           "transparent"
    property real   interiorOpacity:    1
    property int    borderWidth:        0
    property color  borderColor:        "black"
    property var    poiLayerControler:  _poiLayerController
    property var    poiLayers:          poiLayerControler.poiLayers

    readonly property real  _margin:            ScreenTools.defaultFontPixelWidth / 2
    readonly property real  _rightPanelWidth:   Math.min(parent.width / 3, ScreenTools.defaultFontPixelWidth * 30)
    readonly property real  _toolsMargin:       ScreenTools.defaultFontPixelWidth * 0.75

    PoiLayerController {
        id: _poiLayerController

//        Component.onCompleted: start()
    }

//    Connections{
//        target: poiLayerControler
//        function onPoiLayerAdded(poiLayer) {
//            console.log("layer added " + poiLayer.id + " " + poiLayer.name);
//            addLayer(poiLayer);
//        }
//    }

    function addLayer(poiLayer) {
        console.log("addLayer " + poiLayer.name);

        var poiLayerVis = Qt.createComponent("PoiLayerVisual.qml").createObject(_root, {poiLayer: poiLayer, mapControl: mapControl});
        poiLayerVis.anchors.left = layersListBackPane.left;
        poiLayerVis.anchors.right = layersListBackPane.right;
        poiLayerVis.anchors.top = _poiRepeater.itemAt(_poiRepeater.count - 1).bottom;
        poiLayerVis.anchors.topMargin = _margin;
        poiLayerVis.anchors.bottom = layersListBackPane.bottom;
        poiLayerVis.visible = true;

    }

    KMLOrSHPFileDialog {
        id:             kmlOrSHPLoadDialog
        title:          qsTr("Select Polygon File")
        selectExisting: true

        onAcceptedForLoad: {
            mapPolygon.loadKMLOrSHPFile(file)
            //mapFitFunctions.fitMapViewportToMissionItems()
            close()
        }
    }

    Button {
        id:             loadPolygonButton
        text:           qsTr("Load Polygon")
        anchors {
            right:          parent.right
            leftMargin:     _toolsMargin
            top:            parent.top
            topMargin:      _toolsMargin
        }
        onClicked: {
            poiLayerControler.start();
//            kmlOrSHPLoadDialog.openForLoad()
        }

    }

    Rectangle {
        id:             layersListBackPane
        //todo dobry kolor
        color:          "black"
        opacity:        0.5
        anchors.top:    parent.top
        anchors.bottom: parent.bottom
        anchors.right:  parent.right
        width:          _rightPanelWidth + 2 * _margin

    }

    Rectangle {
        id:                     layersListHeader
        width:                  layersListBackPane.width
        height:                 childrenRect.height + 2 * _margin
        anchors.verticalCenter: layersListBackPane.verticalCenter
        anchors.left:          layersListBackPane.left
        opacity:                1
        color:                  "black"
        radius:                 ScreenTools.defaultFontPixelWidth / 2

        QGCLabel {
            id:             layersListTitle
            text:           qsTr("POI Layers")

            anchors {
                top:            parent.top
                topMargin:      _margin
                leftMargin:     _margin
                //horizontalCenter: parent.horizontalCenter
            }
            color:          "white"
            font.bold:      true
        }
    }

    Rectangle {
        width:                  layersListBackPane.width
        height:                 childrenRect.height + 2 * _margin
        anchors.top:            layersListHeader.bottom
        anchors.left:           layersListHeader.left
        opacity:                1
        //todo lepszy kolor
        color:                  "steelblue"
        radius:                 ScreenTools.defaultFontPixelWidth

        Column {

            width: parent.width
            spacing: _margin

            anchors {
                top:            layersListTitle.bottom
                topMargin:      _margin
                horizontalCenter: parent.horizontalCenter
            }

            Button {
                id:             addLayerButton
                text:           qsTr("Load Layer")
                width: parent.width
                anchors {
//                    top:            layersListTitle.bottom
//                    topMargin:      _margin
                    horizontalCenter: parent.horizontalCenter

                }
                onClicked: {
                    poiLayerControler.addLayer()
                }
            }

            Repeater {
                id:                 _poiRepeater
                model:              _poiLayerController.poiLayers
                //_missionController.missionItemsModel
                delegate: PoiLayerMenuWidget {
                    modelData: object

                }
            }
        }
    }

    Repeater {
       model:  _poiLayerController.poiLayers

        delegate: PoiLayerVisual {
            id:             poiLayerVis
            poiLayer:       object
            mapControl:     _root.mapControl
        }
    }


/*

    ColumnLayout {
            id:                 valuesColumn
            anchors.margins:    _margin
            anchors.left:       parent.left
            anchors.right:      parent.right
            anchors.top:        parent.top
            spacing:            _margin
            visible:            true


              Column {
                Layout.fillWidth:   true
                spacing:            _margin
                visible:            true


                SectionHeader {
                    id:             vehicleInfoSectionHeader
                    anchors.left:   parent.left
                    anchors.right:  parent.right
                    text:           qsTr("Vehicle Info")
                    visible:        true
                    checked:        false
                }

                GridLayout {
                    anchors.left:   parent.left
                    anchors.right:  parent.right
                    columnSpacing:  ScreenTools.defaultFontPixelWidth
                    rowSpacing:     columnSpacing
                    columns:        2
                    visible:        vehicleInfoSectionHeader.visible && vehicleInfoSectionHeader.checked

                    QGCLabel {
                        text:               _firmwareLabel
                        Layout.fillWidth:   true
                        visible:            _multipleFirmware
                    }
                    FactComboBox {
                        fact:                   QGroundControl.settingsManager.appSettings.offlineEditingFirmwareClass
                        indexModel:             false
                        Layout.preferredWidth:  _fieldWidth
                        visible:                _multipleFirmware && _allowFWVehicleTypeSelection
                    }
                    QGCLabel {
                        text:       _controllerVehicle.firmwareTypeString
                        visible:    _multipleFirmware && !_allowFWVehicleTypeSelection
                    }

                    QGCLabel {
                        text:               _vehicleLabel
                        Layout.fillWidth:   true
                        visible:            _multipleVehicleTypes
                    }
                    FactComboBox {
                        fact:                   QGroundControl.settingsManager.appSettings.offlineEditingVehicleClass
                        indexModel:             false
                        Layout.preferredWidth:  _fieldWidth
                        visible:                _multipleVehicleTypes && _allowFWVehicleTypeSelection
                    }
                    QGCLabel {
                        text:       _controllerVehicle.vehicleTypeString
                        visible:    _multipleVehicleTypes && !_allowFWVehicleTypeSelection
                    }

                    QGCLabel {
                        Layout.columnSpan:      2
                        Layout.alignment:       Qt.AlignHCenter
                        Layout.fillWidth:       true
                        wrapMode:               Text.WordWrap
                        font.pointSize:         ScreenTools.smallFontPointSize
                        text:                   qsTr("The following speed values are used to calculate total mission time. They do not affect the flight speed for the mission.")
                        visible:                _showCruiseSpeed || _showHoverSpeed
                    }

                    QGCLabel {
                        text:               qsTr("Cruise speed")
                        visible:            _showCruiseSpeed
                        Layout.fillWidth:   true
                    }
                    FactTextField {
                        fact:                   QGroundControl.settingsManager.appSettings.offlineEditingCruiseSpeed
                        visible:                _showCruiseSpeed
                        Layout.preferredWidth:  _fieldWidth
                    }

                    QGCLabel {
                        text:               qsTr("Hover speed")
                        visible:            _showHoverSpeed
                        Layout.fillWidth:   true
                    }
                    FactTextField {
                        fact:                   QGroundControl.settingsManager.appSettings.offlineEditingHoverSpeed
                        visible:                _showHoverSpeed
                        Layout.preferredWidth:  _fieldWidth
                    }
                } // GridLayout


            } // Column
        } // Column
                    */
}
