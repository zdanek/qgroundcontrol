add_library(PoiLayers
        PoiGeom.cc
        PoiPolygon.cc
        PoiPoint.cc
        PoiPolyline.cc
        PoiLayerController.cc
        PoiLayer.cc
        PoiSvg.cc)

target_link_libraries(PoiLayers
        PRIVATE
        qgc
        qtkml
)

target_include_directories(PoiLayers
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

add_custom_target(PoiQml
    SOURCES
        PoiLayerMenuWidget.qml)
