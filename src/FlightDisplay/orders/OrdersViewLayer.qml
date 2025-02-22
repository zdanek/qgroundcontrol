// view that is 0.2 transparent, contains header with title "MewaMed Orders" and pane that will contain embedded WebView
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

import QtQuick.Window 2.2
import QtQml.Models 2.1

import QGroundControl 1.0
import QGroundControl.Controllers 1.0
import QGroundControl.ScreenTools 1.0
import QGroundControl.OrdersController 1.0
import QtWebView 1.1



Item {
    id: _root
    property color  interiorColor: "transparent"
    property color  altColor: "transparent"
    property real   interiorOpacity: 1
    property int    borderWidth: 0
    property color  borderColor: "black"
    // property var    ordersController: _ordersController

    readonly property real  _margin: ScreenTools.defaultFontPixelWidth / 2
    readonly property real  _rightPanelWidth: Math.min(parent.width / 3, ScreenTools.defaultFontPixelWidth * 30)
    readonly property real  _toolsMargin: ScreenTools.defaultFontPixelWidth * 0.75

    // OrdersController {
    //     id: _ordersController
    // }

    Rectangle {
        id: layersListBackPane
        // width: parent.width
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        color: "black"
        opacity: 0.8
        border.color: "red"
        visible: OrdersController.ordersPanelVisible


        Rectangle {
            id: header
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            height: 50
            color: "black"
            opacity: 1
            border.color: "green"

            Label {
                id: headerLabel
                anchors.fill: parent
                text: "MewaMed Orders"
                color: "white"
                font.pixelSize: ScreenTools.defaultFontPixelWidth * 3
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
        }
        // Label with X button to close the panel, onClick: OrdersController.hideOrdersPanel()
        Rectangle {
            id: closeButton
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.margins: 1
            width: 40
            height: 40
            color: "transparent"

            Label {
                anchors.centerIn: parent
                text: "✕"
                color: "white"
                font.pixelSize: ScreenTools.defaultFontPixelWidth * 4
            }

            MouseArea {
                anchors.fill: parent
                onClicked: OrdersController.hideOrdersPanel()
                hoverEnabled: true
                onEntered: parent.color = Qt.rgba(255, 255, 255, 0.1)
                onExited: parent.color = "transparent"
            }
        }

        Rectangle {
            id: webViewPane
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: header.bottom
            anchors.bottom: parent.bottom
            // color: "blue"
            opacity: 1

            WebView {
                id: webView
                anchors.fill: parent
                url: OrdersController.url
            }
        }
    }
}
