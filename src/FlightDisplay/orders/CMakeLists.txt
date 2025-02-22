add_library(Orders
        OrdersChecker.cc
        OrdersController.cc)

target_link_libraries(Orders
        PRIVATE
        qgc
)

target_include_directories(Orders
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

add_custom_target(OrdersQml
        SOURCES
        OrdersViewLayer.qml)
