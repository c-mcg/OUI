set(CMAKE_MACOSX_RPATH 1)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(CMAKE_GENERATOR_PLATFORM x64)
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
    # 32 bit
endif()

set(CMAKE_CXX_STANDARD 17)

cmake_minimum_required (VERSION 3.14.0)

project(OUI)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

file(GLOB_RECURSE OUI_src
    "source/*.cpp"
)

file(GLOB_RECURSE OUI_src2
    "source/**/**.cpp"
)

IF(APPLE)
	file(GLOB_RECURSE OUI_src3
		"source/**/**.mm"
    )

    # Gtest Params
    add_definitions(-DGTEST_USE_OWN_TR1_TUPLE)
    add_definitions(-D__GLIBCXX__)

ENDIF(APPLE)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/gtest)
enable_testing()
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/gtest/include ${CMAKE_CURRENT_SOURCE_DIR}/lib/gtest)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

add_library (OUI_lib SHARED ${OUI_src} ${OUI_src2} ${OUI_src3})

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
	${SDL2_INCLUDE_DIR}
	${SDL2_TTF_INCLUDE_DIR}
	${SDL2_IMAGE_INCLUDE_DIRS}
)

target_link_libraries(
	OUI_lib
	${SDL2_LIBRARY}
	${SDL2_TTF_LIBRARY}
	${SDL2_IMAGE_LIBRARIES}
)

add_subdirectory (tests)