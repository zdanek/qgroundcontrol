/****************************************************************************
 *
 * (c) 2009-2022 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

import QtQuick                  2.12
import QtLocation                   5.3


Item {
    id: _root

    property var mapControl             /// Map control to place item in
    property var poiLayer               /// poi layer with POI items - PoiLayer object

    Component.onCompleted: {
        console.log("poilayer")
        console.log(poiLayer)
        if (mapControl) {
            console.log('Creating POI polygons for layer "' + poiLayer.name + '"')
            // contains list of QGCMapPolygon
            for (var i = 0; i < poiLayer.polygons.count; i++) {
                var poi = poiLayer.polygons.get(i);
                var polygon = polygonDelegate.createObject(mapControl, {
                    id: "poiPolygon" + i,
                    color: "red",
                    opacity: 0.5,
                    path: poi.path,
                    visible: poi.visible
                });
                mapControl.addMapItem(polygon)
            }

            for (var i = 0; i < poiLayer.polylines.count; i++) {
                var poi = poiLayer.polylines.get(i);
                var polyline = polylineDelegate.createObject(mapControl, {
                    id: "poiPolyline" + i,
                    color: "green",
                    opacity: 1,
                    path: poi.path,
                    visible: poi.visible
                });
                mapControl.addMapItem(polyline)
            }

            for (var i = 0; i < poiLayer.points.count; i++) {
                var poi = poiLayer.points.get(i);
                var cricle = pointDelegate.createMapItem(mapControl, {
                    id: "poiPoint" + i,
//                    color: "blue",
//                    opacity: 1,
                    center: poi.center,
                    visible: poi.visible
                });
                mapControl.addMapItem(cricle)
            }

            for (var i = 0; i < poiLayer.circles.count; i++) {
                var poi = poiLayer.circles.get(i);
                var cricle = circleDelegate.createMapItem(mapControl, {
                    id: "poiCircle" + i,
                    color: "blue",
//                    opacity: 1,
                    center: poi.center,
                    radius: poi.radius,
                    visible: poi.visible
                });
                mapControl.addMapItem(cricle)
            }


/*
            for (var i = 0; i < poiLayer.polygons.count; i++) {
                var poi = poiLayer.polygons.get(i);
                poi.visibleChanged.connect(function() {
                    console.log("poiLayer.visible: " + poiLayer.visible + " poi.visible: " + poi.visible);
                    mapControl.itemById("poiPolygon" + i).visible = poi.visible;
                });
            }
*/
        }
    }

    Component {
        id: polygonDelegate

        MapPolygon {
            color:          "red"
            opacity:        1.0
            border.color:   "black"
            border.width:   2
        }
    }

    Component {
        id: polylineDelegate

        MapPolyline {
            opacity:        1.0
            line.color:   "black"
            line.width:   2
        }
    }

    Component {
        id: circleDelegate

        MapCircle {
//            opacity:        1.0
            border.color:   "black"
            border.width:   1
            radius: 5
        }
    }

    Component {
        id: pointDelegate

        MapCircle {
//            opacity:        1.0
            border.color:   "black"
            border.width:   1
            radius: 5
        }
    }
}