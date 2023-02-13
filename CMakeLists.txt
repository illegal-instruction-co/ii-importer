cmake_minimum_required(VERSION 3.20)

project(ii-importer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    if(CMAKE_GENERATOR MATCHES "Visual Studio")
    	set(CMAKE_SUPPRESS_REGENERATION true)
    	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ii-importer)
    endif()

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W3")
endif()

add_executable(${PROJECT_NAME}
    ${CMAKE_SOURCE_DIR}/Example.cpp
)



