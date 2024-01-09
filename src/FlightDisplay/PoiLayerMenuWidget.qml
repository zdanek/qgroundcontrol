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

    readonly property real  _margin:            ScreenTools.defaultFontPixelWidth / 2
    readonly property real  _radius:            ScreenTools.defaultFontPixelWidth / 2
    readonly property real  _trashSize:         ScreenTools.defaultFontPixelHeight * 1.5

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
          visible:                true
          source:                 "/res/TrashDelete.svg"

          QGCMouseArea {
              fillItem:   parent
              onClicked:  remove()
          }
      }

      QGCCheckBox {
          id:         flightSpeedCheckbox
          text:       modelData.name
          checked:    modelData.visible
          onClicked:  modelData.visible = checked
      }

    }
}