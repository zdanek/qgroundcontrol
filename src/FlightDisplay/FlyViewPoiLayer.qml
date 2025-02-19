/****************************************************************************
 *
 * (c) 2009-2022 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

import QtQuick 2.12
import QtQuick.Controls 2.4
import QtQuick.Dialogs 1.3
import QtQuick.Layouts 1.12

import QtLocation 5.3
import QtPositioning 5.3
import QtQuick.Window 2.2
import QtQml.Models 2.1

import QGroundControl 1.0
import QGroundControl.Airspace 1.0
import QGroundControl.Airmap 1.0
import QGroundControl.Controllers 1.0
import QGroundControl.Controls 1.0
import QGroundControl.FactSystem 1.0
import QGroundControl.FlightDisplay 1.0
import QGroundControl.FlightMap 1.0
import QGroundControl.Palette 1.0
import QGroundControl.ScreenTools 1.0
import QGroundControl.Vehicle 1.0
// import QGroundControl.PoiLayerController 1.0

Item {
    id: _root

    property var mapControl
    property color  interiorColor: "transparent"
    property color  altColor: "transparent"
    property real   interiorOpacity: 1
    property int    borderWidth: 0
    property color  borderColor: "black"
    property var    poiLayerControler: _poiLayerController
    property var    poiLayers: poiLayerControler.poiLayers

    readonly property real  _margin: ScreenTools.defaultFontPixelWidth / 2
    readonly property real  _rightPanelWidth: Math.min(parent.width / 3, ScreenTools.defaultFontPixelWidth * 30)
    readonly property real  _toolsMargin: ScreenTools.defaultFontPixelWidth * 0.75

    PoiLayerController {
        id: _poiLayerController

        // //        Component.onCompleted: start()
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

        var poiLayerVis = Qt.createComponent("PoiLayerVisual.qml").createObject(_root, {
            poiLayer: poiLayer,
            mapControl: mapControl
        });
        poiLayerVis.anchors.left = layersListBackPane.left;
        poiLayerVis.anchors.right = layersListBackPane.right;
        poiLayerVis.anchors.top = _poiRepeater.itemAt(_poiRepeater.count - 1).bottom;
        poiLayerVis.anchors.topMargin = _margin;
        poiLayerVis.anchors.bottom = layersListBackPane.bottom;
        poiLayerVis.visible = true;

    }


    Rectangle {
        id: layersListBackPane

        color: "black"
        opacity: 0
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        width: _rightPanelWidth + 2 * _margin
        // height:         parent.height 0.5

    }

    Rectangle {
        id: ordersAppPane
        width: ordersAppRow.width + _margin * 2
        height: openOrdersAppButton.height + 2 * _margin
        anchors.top: slidingContainer.top
        anchors.left: layersListBackPane.left
        opacity: 0.8
        color: "black"
        radius: ScreenTools.defaultFontPixelWidth / 2

        Row {
            id: ordersAppRow
            spacing: _margin * 2

            Image {
                id: openOrdersAppButton
                //anchors.verticalCenter: parent.verticalCenter
                height: ScreenTools.defaultFontPixelHeight * 1.5
                width: height
                sourceSize.height: height
                fillMode: Image.PreserveAspectFit
                mipmap: true
                smooth: true
                // color: qgcPal.text
                // visible: modelData.canBeDeleted
                source: "/qmlimages/Analyze.svg"

                QGCMouseArea {
                    fillItem: parent
                    onClicked: {
                        poiLayerControler.openPage("https://onet.pl");
                    }
                }
            }

            Image {
                id: otherOrdersAppButton
                //anchors.verticalCenter: parent.verticalCenter
                height: ScreenTools.defaultFontPixelHeight * 1.5
                width: height
                sourceSize.height: height
                fillMode: Image.PreserveAspectFit
                mipmap: true
                smooth: true
                // color: qgcPal.text
                opacity: 1
                visible: false
                SequentialAnimation {
                    id: opacityAnimation
                    loops: Animation.Infinite
                    NumberAnimation {
                        target: otherOrdersAppButton; property: "opacity";
                        from: 0;
                        to: 1; duration: 1000
                    }
                    NumberAnimation {
                        target: otherOrdersAppButton; property: "opacity";
                        from: 1;
                        to: 0; duration: 1000
                    }
                }
                // Component.onCompleted: opacityAnimation.start()

                source: "/qmlimages/Yield.svg"
            }
            // Add more QGCColoredImage elements here in the future
        }
    }
    Item {
        id: slidingContainer
        width: layersListBackPane.width
        height: parent.height
        visible: true



        Rectangle {
            id: layersListHeader
            width: layersListBackPane.width
            height: layersListTitle.height + _margin * 2
            anchors.bottom: layersListPane.top
            anchors.left: parent.left
            opacity: 1
            color: "black"
            radius: ScreenTools.defaultFontPixelWidth / 2


            QGCLabel {
                id: layersListTitle
                text: qsTr("POI Layers")
                anchors {
                    top: parent.top
                    left: parent.left
                    margins: _margin
                }
                color: "white"
                font.bold: true
            }

            Rectangle {
                id: toggleButton
                width: ScreenTools.defaultFontPixelWidth * 3
                height: width
                anchors.right: parent.right
                anchors.rightMargin: -width / 2
                anchors.verticalCenter: parent.verticalCenter
                radius: width / 2
                border.color: "white"
                color: "black"
                z: 999

                QGCButton {
                    showBorder: true
                    anchors.fill: parent
                    text: slidingContainer.x == 0 ? qsTr("<<") : qsTr(">>")
                    onClicked: slideAnimation.running = true
                }
            }
        }

        Rectangle {
            id: layersListPane
            width: layersListBackPane.width
            height: Math.min(childrenRect.height + 2 * _margin, parent.height * 0.5)
            anchors.bottom: parent.bottom
            anchors.left: layersListHeader.left
            opacity: 1
            color: "steelblue"
            radius: ScreenTools.defaultFontPixelWidth

            Column {
                width: parent.width
                spacing: _margin

                anchors {
                    bottom: parent.bottom
                    bottomMargin: _margin
                    horizontalCenter: parent.horizontalCenter
                }

                Repeater {
                    id: _poiRepeater
                    model: _poiLayerController.poiLayers

                    delegate: PoiLayerMenuWidget {
                        modelData: object
                        layerController: _poiLayerController
                    }
                }
            }
        }

        Repeater {
            model: _poiLayerController.poiLayers

            delegate: PoiLayerVisual {
                id: poiLayerVis
                poiLayer: object
                mapControl: _root.mapControl
            }
        }

        PropertyAnimation {
            id: slideAnimation
            target: slidingContainer
            property: "x"
            from: slidingContainer.x
            to: slidingContainer.x === 0 ? -slidingContainer.width + toggleButton.width / 2 : 0
            duration: 300
            easing.type: Easing.InOutQuad
        }
    }
}
