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

qt_standard_project_setup()

qt_add_executable(uplt)
target_sources(uplt 
	PRIVATE 
		src/main.cpp
		src/main_window.cpp
		src/port_table_model.cpp
		src/port_table_view.cpp
	PUBLIC FILE_SET HEADERS BASE_DIRS include/ FILES
		include/uplt/main_window.hpp
		include/uplt/port_table_model.hpp
		include/uplt/port_table_view.hpp
)

target_link_libraries(uplt PRIVATE
	Qt6::Core Qt6::Widgets Qt6::Gui 
	Qt6::PrintSupport Qt6::SerialPort
	exprtk::exprtk qcustomplot
)
