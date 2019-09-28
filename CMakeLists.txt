file(GLOB SOURCES src/*.cpp)

add_subdirectory(external/jsoncpp)

if (BUILD_LOG_STATIC)
    add_library(log STATIC ${SOURCES})
else ()
    add_library(log SHARED ${SOURCES})
endif ()

add_library(SuperCLine::log ALIAS log)

add_definitions(-DLOG_API=__DLL_EXPORT)
add_definitions(-DCORE_API=__DLL_IMPORT)
if (WIN32)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif ()

# set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_REQUIRED_FLAGS -std=c++17)
# set_property(TARGET log PROPERTY CXX_STANDARD 17)

# include(${CMAKE_ROOT}/Modules/ExternalProject.cmake)
# ExternalProject_Add(jsoncpp
#     GIT_REPOSITORY "https://github.com/open-source-parsers/jsoncpp.git"
#     UPDATE_COMMAND git pull "https://github.com/open-source-parsers/jsoncpp.git"
#     # GIT_TAG 1.9.1
#     SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/jsoncpp
#     INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR})

# find_package(jsoncpp)

add_dependencies(log core)
target_link_libraries(log core jsoncpp_lib)

target_include_directories(log 
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/inc/private" "${CMAKE_SOURCE_DIR}/include"
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/inc/public"
)

# pre compile
set_target_properties(log PROPERTIES
    SOVERSION ${SOVERSION}
    VERSION ${VERSION}
    COTIRE_CXX_PREFIX_HEADER_INIT "${CMAKE_CURRENT_SOURCE_DIR}/inc/private/stdafx.h"
)
cotire(log)

# install
install(TARGETS log
    ARCHIVE DESTINATION lib/log
    LIBRARY DESTINATION lib/log
	RUNTIME DESTINATION bin
    COMPONENT library
)

file(GLOB INCLUDES inc/public/*.h inc/public/*.config)
install(FILES ${INCLUDES} DESTINATION include/log)