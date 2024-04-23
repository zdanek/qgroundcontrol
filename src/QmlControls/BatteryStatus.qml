/****************************************************************************
 *
 * (c) 2009-2023 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

import QtQuick          2.11
import QtQuick.Layouts  1.11

import QGroundControl                       1.0
import QGroundControl.Controls              1.0
import QGroundControl.MultiVehicleManager   1.0
import QGroundControl.ScreenTools           1.0
import QGroundControl.Palette               1.0
import MAVLink                              1.0

Item {
    id: batteryVisual
    implicitWidth: batteryRow.implicitWidth

    property var battery: undefined

    function getBatteryColor() {
        if (!battery) {
            return qgcPal.text
        }
        switch (battery.chargeState.rawValue) {
        case MAVLink.MAV_BATTERY_CHARGE_STATE_OK:
            return qgcPal.text
        case MAVLink.MAV_BATTERY_CHARGE_STATE_LOW:
            return qgcPal.colorOrange
        case MAVLink.MAV_BATTERY_CHARGE_STATE_CRITICAL:
        case MAVLink.MAV_BATTERY_CHARGE_STATE_EMERGENCY:
        case MAVLink.MAV_BATTERY_CHARGE_STATE_FAILED:
        case MAVLink.MAV_BATTERY_CHARGE_STATE_UNHEALTHY:
            return qgcPal.colorRed
        default:
            return qgcPal.text
        }
    }

    function getBatteryPercentageText() {
        if (!battery) {
            return "?%"
        }
        if (!isNaN(battery.percentRemaining.rawValue)) {
            return battery.percentRemaining.valueString + battery.percentRemaining.units
        } else if (!isNaN(battery.voltage.rawValue)) {
            return battery.voltage.valueString + battery.voltage.units
        } else if (battery.chargeState.rawValue !== MAVLink.MAV_BATTERY_CHARGE_STATE_UNDEFINED) {
            return battery.chargeState.enumStringValue
        }
        return ""
    }

    function getBatteryVoltageText() {
        if (!battery) {
            return ""
        }
        if (!isNaN(battery.voltage.rawValue)) {
            return battery.voltage.valueString + " " + battery.voltage.units
        } else if (battery.chargeState.rawValue !== MAVLink.MAV_BATTERY_CHARGE_STATE_UNDEFINED) {
            return battery.chargeState.enumStringValue
        }
        return "?"
    }

    Row {
        id:             batteryRow
        anchors.top:    parent.top
        anchors.bottom: parent.bottom
        width:          children.width

        QGCColoredImage {
            id:                 battIcon
            anchors.top:        parent.top
            anchors.bottom:     parent.bottom
            width:              height
            sourceSize.height:  height
            source:             "/qmlimages/Battery.svg"
            fillMode:           Image.PreserveAspectFit
            color:              getBatteryColor()
        }

        Column {
            id:                     batteryInfoColumn
            anchors.top:            parent.top
            anchors.bottom:         parent.bottom
            anchors.leftMargin:     ScreenTools.defaultFontPixelWidth / 2
            width:                  children.width

            QGCLabel {
                verticalAlignment:      Text.AlignLeft
                color:                  getBatteryColor()
                text:                   getBatteryPercentageText()
                font.pointSize:         ScreenTools.defaultFontPointSize
            }

            QGCLabel {
                id:                     voltValue
                verticalAlignment:      Text.AlignLeft
                font.pointSize:         ScreenTools.defaultFontPointSize
                color:                  getBatteryColor()
                text:                   getBatteryVoltageText()
            }
        }
    }
}
