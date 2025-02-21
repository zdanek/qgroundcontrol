add_library(Orders
        OrdersController.cc)

target_link_libraries(Orders
        PRIVATE
        qgc
)

target_include_directories(Orders
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
