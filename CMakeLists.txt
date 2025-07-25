cmake_minimum_required(VERSION 3.25)

project(usart_plotter LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

find_package(Qt6 REQUIRED COMPONENTS 
	Core Widgets Gui PrintSupport SerialPort
)

include(FetchContent)
FetchContent_Declare(
    qcustomplot QUIET
    GIT_REPOSITORY https://github.com/legerch/QCustomPlot-library
    GIT_TAG 2.1.1.1
)
FetchContent_MakeAvailable(qcustomplot)
FetchContent_Declare(
    exprtk QUIET
    GIT_REPOSITORY https://github.com/ArashPartow/exprtk
    GIT_TAG 0.0.3-cmake
)
FetchContent_MakeAvailable(exprtk)

set(UPLT_VIRTUAL_SERIAL_PORT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/virtual_tty/)

qt_standard_project_setup()

qt_add_executable(uplt)
target_sources(uplt 
	PRIVATE 
		src/main.cpp
		src/main_window.cpp
		src/port_table_model.cpp
		src/port_table_view.cpp
		src/port_spec_dialog.cpp
		src/graph.cpp
		src/graph_spec_dialog.cpp
	PUBLIC FILE_SET HEADERS 
	BASE_DIRS include/ FILES
		include/uplt/main_window.hpp
		include/uplt/port_table_model.hpp
		include/uplt/port_table_view.hpp
		include/uplt/port_spec.hpp
		include/uplt/port_spec_dialog.hpp
		include/uplt/graph.hpp
		include/uplt/graph_spec.hpp
		include/uplt/graph_spec_dialog.hpp
		include/uplt/dialog.hpp
)
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	target_compile_definitions(uplt 
		PRIVATE 
			UPLT_DEBUG
			UPLT_VIRTUAL_SERIAL_PORT_DIRECTORY="${UPLT_VIRTUAL_SERIAL_PORT_DIRECTORY}"
	)
endif()

target_link_libraries(uplt PRIVATE
	Qt6::Core Qt6::Widgets Qt6::Gui 
	Qt6::PrintSupport Qt6::SerialPort
	exprtk::exprtk qcustomplot
)


add_subdirectory(scripts)
