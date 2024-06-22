cmake_minimum_required(VERSION 3.0.0)

# set(VCPKG_LIBRARY_LINKAGE static)
# set(VCPKG_CRT_LINKAGE static)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS_RELEASE "-Ofast")
  # add_compile_options(-Ofast)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS_RELEASE "-Ofast")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
  # using Intel C++
else ()
  set(CMAKE_CXX_FLAGS_RELEASE "-Ox")
endif()

project(skillup VERSION 0.1.0 LANGUAGES CXX)


# Packagegs

find_package(imgui CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(GLEW CONFIG REQUIRED)
find_package(OpenGL REQUIRED)

# Include dir
add_subdirectory(Core)
add_subdirectory(App)

include(CTest)
enable_testing()

# sym link assets
set (assetSource "${CMAKE_SOURCE_DIR}/Assets")
set (assetDest "${CMAKE_BINARY_DIR}/Assets")

add_custom_target(assets
        COMMAND ${CMAKE_COMMAND} -E make_directory ${assetDest}
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${assetSource} ${assetDest}
        COMMENT "copying Assets: ${assetSource} to ${assetDest}"
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)
add_dependencies(skillup assets)