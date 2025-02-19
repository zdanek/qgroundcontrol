/****************************************************************************
 *
 * (c) 2009-2022 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

import QtQuick                  2.12

import QGroundControl.Controls  1.0
import QGroundControl.ScreenTools   1.0

Rectangle {
    id:             pierdu
    color:          true ? qgcPal.missionItemEditor : qgcPal.windowShade
    radius:         _radius
    opacity:        true ? 1.0 : 0.7
    width:              parent.width
    anchors.margins:    _margin
    anchors.left:       parent.left
    height:             childrenRect.height

    property var modelData
    property var layerController

    readonly property real  _margin:            ScreenTools.defaultFontPixelWidth / 2
    readonly property real  _radius:            ScreenTools.defaultFontPixelWidth / 2
    readonly property real  _trashSize:         ScreenTools.defaultFontPixelHeight * 1.5

    function remove(layerId) {
        console.log("remove layerId: ", layerId)
        layerController.deletePoiLayerById(layerId)
    }
    // QGCButton {
    //     text: "Open Page"
    //     onClicked: {
    //         console.log("Open Page button clicked")
    //         layerController.openPage("https://onet.pl")
    //     }
    // }

    Row {
      id:                 topRowLayout
      anchors.margins:    _margin
      anchors.left:       parent.left
    //        anchors.top:        parent.top
      spacing:            _margin


      QGCColoredImage {
          id:                     deleteButton
          //anchors.verticalCenter: parent.verticalCenter
          height:                 _trashSize
          width:                  height
          sourceSize.height:      height
          fillMode:               Image.PreserveAspectFit
          mipmap:                 true
          smooth:                 true
          color:                  qgcPal.text
          visible:                modelData.canBeDeleted
          source:                 "/res/TrashDelete.svg"

          QGCMouseArea {
              fillItem:   parent
              onClicked:  remove(modelData.id)
          }
      }

      QGCCheckBox {
          id:         layerVisibleCheckbox
          text:       modelData.name
          textFontPointSize:  ScreenTools.defaultFontPointSize * 0.75
          checked:    modelData.visible
          onClicked:  modelData.visible = checked
      }

    }
}