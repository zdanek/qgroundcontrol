import QtQuick              2.3
import QtLocation           5.3
import QtPositioning        5.3
import QtGraphicalEffects   1.0

import QGroundControl               1.0
import QGroundControl.ScreenTools   1.0
import QGroundControl.Vehicle       1.0
import QGroundControl.Controls      1.0

/// Marker for displaying a vehicle location on the map
MapQuickItem {
    property string pointLabel:       ""                                              ///< Vehicle callsign
    property real   size:           _uavSize

    anchorPoint.x:  vehicleItem.width  / 2
    anchorPoint.y:  vehicleItem.height / 2
    visible:        coordinate.isValid

    property real   _uavSize:       ScreenTools.defaultFontPixelHeight * 5

    sourceItem: Item {
        id:         vehicleItem
        width:      vehicleIcon.width
        height:     vehicleIcon.height
        opacity:    _adsbVehicle || vehicle === _activeVehicle ? 1.0 : 0.5

        Image {
            id:                 vehicleIcon
            source:             "/qmlimages/AlertAircraft.svg"
            mipmap:             true
            width:              size
            sourceSize.width:   size
            fillMode:           Image.PreserveAspectFit
            transform: Rotation {
                origin.x:       vehicleIcon.width  / 2
                origin.y:       vehicleIcon.height / 2
                angle:          isNaN(heading) ? 0 : heading
            }
        }

        Text {
            id:                         vehicleLabel
            font.pointSize:             ScreenTools.defaultFontPointSize //: ScreenTools.smallFontPointSize
            font.family:    ScreenTools.normalFontFamily
            color:          qgcPal.text
            antialiasing:   true
            QGCPalette { id: qgcPal; colorGroupEnabled: enabled }
            anchors.top:                parent.bottom
            anchors.horizontalCenter:   parent.horizontalCenter
            text:                       vehicleLabelText
            visible:                    vehicleLabelText.length > 0

            property string vehicleLabelText: pointLabel
        }
    }
}
