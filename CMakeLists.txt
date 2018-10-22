file(GLOB_RECURSE OUI_src
    "source/**/*.cpp"
)

file(GLOB OUI_include
    "include/**/*.h"
)

add_library (OUI ${OUI_src})
target_include_directories (OUI PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)