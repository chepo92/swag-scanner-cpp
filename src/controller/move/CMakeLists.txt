target_sources(swag_scanner_lib PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/MoveController.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/MoveController.h
        ${CMAKE_CURRENT_SOURCE_DIR}/MoveControllerGUI.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/MoveControllerGUI.h
        ${CMAKE_CURRENT_SOURCE_DIR}/MoveMethod.h
        )

target_include_directories(swag_scanner_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})