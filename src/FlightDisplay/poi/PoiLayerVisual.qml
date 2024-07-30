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
    property var poiLayerController

    property var lvisible: poiLayer.visible
    property var __items: []

    onLvisibleChanged: function (sth, visible) {
        console.log("onVisibleChanged " + lvisible)
        for (var i = 0; i < __items.length; i++) {
            __items[i].visible = lvisible
        }
    }

    Connections {
        target: poiLayer
        function onDeleted() {
            console.log("Layer deleted " + poiLayer.id)
            console.log("Removing all items")
            for (var i = 0; i < __items.length; i++) {
                mapControl.removeMapItem(__items[i])
            }
            __items = []
        }
    }

    function _addMapElement(element){
        __items.push(element)
        mapControl.addMapItem(element)
    }

    function _drawVertices(element, item){
//        console.log("element.vertices.length " + element.vertices.length + " color " + element.styles["fill_color"] + " " + element.styles["line_color"] + " " + element.styles["line_width"])
        var vertices = element.path;
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

            var poiElements = []
            var poiLength = 0
            if (!poiLayer.kmlgraphics) {
                console.log("Using poiLayer.elements ", poiLayer.elements, poiLayer.elements.count)
                poiLength = poiLayer.elements.count
                poiElements = poiLayer.elements
                console.log("first element", poiElements.get(0));
            }


            if (poiLayer.kmlgraphics && poiLayer.kmlgraphics.renderers) {
                console.log("Using poiLayer.kmlGraphics.renderers ", poiLayer.kmlGraphics.renderers)
                poiElements = poiLayer.kmlGraphics.renderers[0].elements;
                poiLength = poiLayer.kmlGraphics.renderers[0].elements.length
            }

                //            console.log("gra", poiLayer.kmlGraphics.renderers)
            for (var i = 0; i < poiLength; i++) {
                var element = poiElements.get(i)
                console.log("element", element)
                console.log("element type", element.type)
                switch (element.type) {
                    case 0:
                    case "polygon":
                    case "gPoiPolygon":
                        console.log("polygon")
                        var polygon = Qt.createQmlObject('import QtLocation 5.5; MapPolygon{smooth:true;antialiasing:true}', mapControl, "mapPolygon")
                        //bzd wypelnic ze styli
                        if (element.styles["fill"]) {
                            polygon.color = _torealColor(element.styles["fill_color"])
                        }
                        if (element.styles["line_width"]) {
                            polygon.border.width = element.styles["line_width"]
                            polygon.border.color = _torealColor(element.styles["line_color"])
                        }

                        _drawVertices(element, polygon)
                        _addMapElement(polygon)
                        break
                    case 1:
                    case "gPoiPolyline":
                    case "polyline":
                        console.log("polyline")
                        var polyline = Qt.createQmlObject('import QtLocation 5.5; MapPolyline{smooth:true;antialiasing:true}', mapControl, "mapPolyline")
//                        console.log("style " +element.styles["line_width"] + " " + element.styles["line_color"]);
                        polyline.line.width = element.styles["line_width"]
                        polyline.line.color = _torealColor(element.styles["line_color"])

                        _drawVertices(element, polyline)
                        _addMapElement(polyline)
                        break
                    case "point":
                        console.log("point")
                        var point = Qt.createQmlObject('import QtLocation 5.5; import QtQuick 2.4; MapQuickItem { smooth:true; antialiasing:true; anchorPoint.x: p_icon.width / 2; anchorPoint.y: p_icon.height; sourceItem:Image { id:p_icon; } }', mapControl, "mapQuickItem")
                        point.sourceItem.source = element.styles["icon"]
                        point.coordinate = QtPositioning.coordinate(element.vertices[0].latitude, element.vertices[0].longitude)
                        _addMapElement(point)
                        console.log(element.styles["icon"] + " f4 " + point.coordinate + " --> " + point.sourceItem.source)
                        break;
                    case "svgWithLabel":
                        console.log("svgWithLabel")
//                        SvgLabelledPoint {
//                            id: svgLabelledPoint
//                            svgSource: element.styles["icon"]
//                            label: element.styles["label"]
//                            coordinate: QtPositioning.coordinate(element.vertices[0].latitude, element.vertices[0].longitude)
//                        }
                        var point = Qt.createQmlObject('import QtLocation 5.5; import QtQuick 2.4; MapQuickItem{ smooth:true; antialiasing:true; anchorPoint.x: p_icon.width / 2; anchorPoint.y: p_icon.height; zoomLevel: 15; sourceItem:Image { id:p_icon; } }', mapControl, "mapQuickItem")
                        // point.sourceItem.source = "http://10.0.0.100/rest/symbol/" + element.extraData;
                        point.sourceItem.source = "http://127.0.0.1:8080/rest/symbol/" + element.extraData;
                        //"file:///tmp/mewa/SFGCEVCA-------.svg"
                        point.coordinate = QtPositioning.coordinate(element.vertices[0].latitude, element.vertices[0].longitude)
                        _addMapElement(point)
                        console.log(element.styles["icon"] + " f4 " + point.coordinate + " --> " + point.sourceItem.source)
                        break;
                    default:
                        console.log("unsupported element type " + element.type)
                        break
                }
            }
        }
    }

}