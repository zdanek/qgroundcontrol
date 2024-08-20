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

            var poiElements = poiLayer.elements
            var poiLength = poiLayer.elements.count

            for (var i = 0; i < poiLength; i++) {
                var element = poiElements.get(i)
                switch (element.type) {
                    case 0:
                    case "polygon":
                        var polygon = Qt.createQmlObject('import QtLocation 5.5; MapPolygon{smooth:true;antialiasing:true}', mapControl, "mapPolygon")

                        if (element.styles["fill_color"]) {
                            polygon.color = _torealColor(element.styles["fill_color"])
                        }
                        if (element.styles["line_width"]) {
                            polygon.border.width = element.styles["line_width"]
                        }
                        if (element.styles["line_color"]) {
                            polygon.border.color = _torealColor(element.styles["line_color"])
                        }

                        _drawVertices(element, polygon)
                        _addMapElement(polygon)
                        break
                    case 1:
                    case "polyline":
                        var polyline = Qt.createQmlObject('import QtLocation 5.5; MapPolyline{smooth:true;antialiasing:true}', mapControl, "mapPolyline")
                        polyline.line.width = element.styles["line_width"]
                        polyline.line.color = _torealColor(element.styles["line_color"])

                        _drawVertices(element, polyline)
                        _addMapElement(polyline)
                        break
                    case 3:
                    case "point":
                        var item = Qt.createQmlObject('import QtLocation 5.5; import QtQuick 2.4; MapQuickItem {  }', mapControl, "mapCircle")
                        item.anchorPoint = Qt.point(2.5,2.5)
                        item.coordinate = element.center
                        item.zoomLevel = 10
                        var circle = Qt.createQmlObject('import QtQuick 2.7; Rectangle{ width: 10; height: 10; radius: 5}', mapControl);
                        circle.color = "red"
                        item.sourceItem = circle
                        _addMapElement(item)

                        break;
                    case 4:
                    case "svgWithLabel":
                        var point = Qt.createQmlObject('import QtLocation 5.5; import QtQuick 2.4; MapQuickItem{ smooth:true; antialiasing:true; anchorPoint.x: p_icon.width / 2; anchorPoint.y: p_icon.height; zoomLevel: 14; sourceItem:Image { id:p_icon; } }', mapControl, "mapQuickItem")
                        point.sourceItem.source = element.src;
                        point.coordinate = element.center
                        _addMapElement(point)

                        break;
                    default:
                        console.log("unsupported element type " + element.type)
                        break
                }
            }
        }
    }
}