set(BUILD_TESTING OFF)


find_package(ZLIB REQUIRED)
find_package(Boost REQUIRED)
find_library(MiniZip NAMES minizip REQUIRED)
find_library(UriParser NAMES uriparser REQUIRED)
find_library(Expat NAMES expat REQUIRED)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/QtKMLLib/kmllib/third_party/libkml/cmake;${CMAKE_MODULE_PATH}")
add_subdirectory(QtKMLLib/kmllib/third_party)

find_package(Qt5 REQUIRED COMPONENTS Gui Quick Positioning REQUIRED)

#qt_standard_project_setup()


set(HEADERS
        QtKMLLib/kmllib/qtkml.h
        QtKMLLib/kmllib/rendervisitor.h
        QtKMLLib/kmllib/graphicsvisitor.h
        QtKMLLib/kmllib/stylevisitor.h
        QtKMLLib/kmllib/mercatorprojection.h
        QtKMLLib/kmllib/kmldocument.h
        QtKMLLib/kmllib/coord.h
        QtKMLLib/kmllib/kmlelement.h
        QtKMLLib/kmllib/kmlgraphics.h
        QtKMLLib/kmllib/kmlrenderer.h
        QtKMLLib/kmllib/qmlkml.h)

set(SOURCES
        QtKMLLib/kmllib/rendervisitor.cpp
        QtKMLLib/kmllib/graphicsvisitor.cpp
        QtKMLLib/kmllib/stylevisitor.cpp
        QtKMLLib/kmllib/mercatorprojection.cpp
        QtKMLLib/kmllib/kmldocument.cpp
        QtKMLLib/kmllib/kmlelement.cpp
        QtKMLLib/kmllib/kmlgraphics.cpp
        QtKMLLib/kmllib/qmlimage.cpp
        QtKMLLib/kmllib/kmlrenderer.cpp
        QtKMLLib/kmllib/kmlitem.cpp)

add_library(qtkml
        ${HEADERS}
        ${SOURCES}
)

if(NOT APPLE)
    message(STATUS "Enabling PIC")
    set_target_properties(qtkml PROPERTIES CMAKE_POSITION_INDEPENDENT_CODE ON)
    message(STATUS "Disabling RTTI")
    target_compile_options(qtkml PRIVATE -fno-rtti)
endif()


target_include_directories(qtkml PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/QtKMLLib/kmllib
        ${CMAKE_CURRENT_SOURCE_DIR}/QtKMLLib/kmllib/third_party/libkml/src
        ${Boost_INCLUDE_DIRS}
)

target_link_libraries(qtkml
        Qt::Core
        Qt::Gui
        Qt::Positioning
        Qt::Quick
        ZLIB::ZLIB
        ${Expat}
        ${UriParser}
        ${MiniZip}
        $<TARGET_OBJECTS:kmlbase>
        $<TARGET_OBJECTS:kmlconvenience>
        $<TARGET_OBJECTS:kmldom>
        $<TARGET_OBJECTS:kmlengine>
        $<TARGET_OBJECTS:kmlregionator>
        $<TARGET_OBJECTS:kmlxsd>
)

add_executable(qtkmltest main.cc)

target_link_libraries(qtkmltest
        qtkml
)
