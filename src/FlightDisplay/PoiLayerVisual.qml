/****************************************************************************
 *
 * (c) 2009-2022 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

import QtQuick                  2.12
import QtPositioning 5.5
import QtLocation 5.5


Item {
    id: _root
    property var mapControl             /// Map control to place item in
    property var poiLayer               /// poi layer with POI items - PoiLayer object
    visible: poiLayer.visible

    function _drawVertices(element, item){
        var vertices = element.vertices;
        for(var k = 0; k < vertices.length; k++){
            item.addCoordinate(QtPositioning.coordinate(vertices[k].latitude, vertices[k].longitude))
        }
    }


    function _torealColor(color){
        return color;
        return Qt.rgba(
                    (0xFF & (color >> 16)) / 255.0,
                    (0xFF & (color >> 8)) / 255.0,
                    (0xFF & color) / 255.0,
                    (0xFF & (color >> 24)) / 255.0);
    }

    Component.onCompleted: {
        console.log("poilayer " + poiLayer.id)

        if (mapControl) {
            console.log('Creating POI polygons for layer "' + poiLayer.name + '"')
            console.log("poiLayer.kmlGraphics ", poiLayer.kmlGraphics)
            var kmlgraphics = poiLayer.kmlGraphics
            console.log("graphics renderers[0] " + kmlgraphics.renderers[0]);
            for(var g = 0; g < kmlgraphics.renderers.length; g++){
                var graphics = kmlgraphics.renderers[g]
                console.log("graphic.elements.length " + graphics.elements.length);
                for(var i = 0; i < graphics.elements.length; i++){
                    var element = graphics.elements[i]
                    console.log("center Point: " + element.center)
                }
            }

            console.log("gra", poiLayer.kmlGraphics.renderers)
            for(var g = 0; g < poiLayer.kmlGraphics.renderers.length; g++){
                var renderer = poiLayer.kmlGraphics.renderers[g]
                console.log("renderere " + renderer);
                console.log("elements " + renderer.elements.length);
                for(var i = 0; i < renderer.elements.length; i++){
                    var element = renderer.elements[i]
                    console.log("center Point: " + element.center)
                    console.log("element.type: " + element.type)
                    switch(element.type){
                    case "polygon":
                        var polygon = Qt.createQmlObject('import QtLocation 5.5; MapPolygon{smooth:true;antialiasing:true}', mapControl, "mapPolygon")
                        if(element.styles["fill"])
                            polygon.color = _torealColor(element.styles["fill_color"])
                        polygon.border.width = element.styles["line_width"]
                        polygon.border.color = _torealColor(element.styles["line_color"])
                        _drawVertices(element, polygon)
                        mapControl.addMapItem(polygon)
                        break
                    case "polyline":
                        var polyline = Qt.createQmlObject('import QtLocation 5.5; MapPolyline{smooth:true;antialiasing:true}', mapControl, "mapPolyline")
                        console.log("style " +element.styles["line_width"] + " " + element.styles["line_color"]);
                        polyline.line.width = element.styles["line_width"]
                        polyline.line.color = _torealColor(element.styles["line_color"])
                        _drawVertices(element, polyline)
                        mapControl.addMapItem(polyline)
                        break
                    case "point":
                        var point = Qt.createQmlObject('import QtLocation 5.5; import QtQuick 2.4; MapQuickItem{
                                                        smooth:true;
                                                        antialiasing:true;
                                                        anchorPoint.x: p_icon.width / 2;
                                                        anchorPoint.y: p_icon.height;
                                                        sourceItem:Image{
                                                            id:p_icon
                                                            }
                                                        }', mapControl, "mapQuickItem")
                        point.sourceItem.source = element.styles["icon"]
                        point.coordinate = QtPositioning.coordinate(element.vertices[0].latitude, element.vertices[0].longitude)
                        mapControl.addMapItem(point)
                        console.log(element.styles["icon"] + " f4 " + point.coordinate + " --> " + point.sourceItem.source)
                        break;
                    }
                }
            }

            // contains list of QGCMapPolygon
            for (var i = 0; i < poiLayer.elements.count; i++) {
                var poi = poiLayer.elements.get(i);
                if (poi.type === "polygon") {
                    var polygon = polygonDelegate.createObject(mapControl, {
                        id: "poiPolygon" + i,
                        color: "red",
                        opacity: 0.5,
                        path: poi.path,
                        visible: poi.visible
                    });
                    mapControl.addMapItem(polygon)
                } else if (poi.type === "polyline") {
                    var polyline = polylineDelegate.createObject(mapControl, {
                        id: "poiPolyline" + i,
                        color: "green",
                        opacity: 1,
                        path: poi.path,
                        visible: poi.visible
                    });
                    mapControl.addMapItem(polyline)
                } else if (poi.type === "point") {
                    var cricle = pointDelegate.createMapItem(mapControl, {
                        id: "poiPoint" + i,
                        color: "blue",
                        opacity: 1,
                        center: poi.center,
                        visible: poi.visible
                    });
                    mapControl.addMapItem(cricle)
                } else if (poi.type === "circle") {
                    var cricle = circleDelegate.createMapItem(mapControl, {
                        id: "poiCircle" + i,
                        color: "blue",
                        opacity: 1,
                        center: poi.center,
                        radius: poi.radius,
                        visible: poi.visible
                    });
                    mapControl.addMapItem(cricle)
                }
                /*
                var polygon = polygonDelegate.createObject(mapControl, {
                    id: "poiPolygon" + i,
                    color: "red",
                    opacity: 0.5,
                    path: poi.path,
                    visible: poi.visible
                });
                mapControl.addMapItem(polygon)
                */
            }

            /*
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
*/

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