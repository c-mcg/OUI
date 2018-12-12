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
ENDIF(APPLE)

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