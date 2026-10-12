set (TARGET tep70-iocontroller)
project (${TARGET})

set(CMAKE_AUTOMOC ON)
set (LIBRARY_OUTPUT_PATH "../../modules/tep70")
set (CMAKE_SHARED_MODULE_PREFIX "")

find_package(Qt6 COMPONENTS Xml REQUIRED)

find_package(RRS ${RRS_VERSION} REQUIRED)

file (GLOB INCLUDES "./include/*.h")
file (GLOB SOURCES "./src/*.cpp")

add_library (${TARGET} SHARED ${SOURCES} ${INCLUDES})

target_include_directories (${TARGET} PRIVATE ./include/)
target_include_directories (${TARGET} PRIVATE ../tep70-equipment/include)

target_link_libraries (${TARGET} Qt6::Xml)
target_link_libraries (${TARGET} CfgReader)
target_link_libraries (${TARGET} core)
target_link_libraries (${TARGET} io-controller)