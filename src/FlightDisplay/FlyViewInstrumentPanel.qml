/****************************************************************************
 *
 * (c) 2009-2022 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

import QtQuick 2.12
import QtQuick.Layouts              1.12

import QGroundControl               1.0
import QGroundControl.Airspace      1.0
import QGroundControl.Airmap        1.0
import QGroundControl.Controls      1.0
import QGroundControl.ScreenTools   1.0

// This control contains the instruments as well and the instrument pages which include values, camera, ...
Column {
    id:         _root
    spacing:    _toolsMargin
    z:          QGroundControl.zOrderWidgets

    property real availableHeight

    AirspaceControl {
        id:                 airspaceControl
        width:              parent.width
        planView:           false
        visible:            QGroundControl.airmapSupported ? QGroundControl.settingsManager.airMapSettings.enableAirMap.rawValue : false
    }

    FlightDisplayViewWidgets {
        id:                 flightDisplayViewWidgets
        width:              parent.width
        missionController:  _missionController
    }

    /*Column {
        spacing: _margin
        width: parent.width

        Rectangle {
            id: yo
            color:          "steelblue"
            radius:         _radius
            opacity:        true ? 1.0 : 0.7
            height:         childrenRect.height + _margin
            width:          parent.width

            QGCLabel {
                id: label
                anchors.left:   parent.left
                text: "POI items"
                color: "white"
            }

        }


    }*/

}
