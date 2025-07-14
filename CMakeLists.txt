cmake_minimum_required(VERSION 3.10)
project(vldwm VERSION 1.0.0 LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Find required packages
find_package(PkgConfig REQUIRED)

pkg_check_modules(DRM REQUIRED libdrm)
pkg_check_modules(GBM REQUIRED gbm)
pkg_check_modules(EGL REQUIRED egl)
pkg_check_modules(GLESV2 REQUIRED glesv2)
pkg_check_modules(LIBINPUT REQUIRED libinput)
pkg_check_modules(XKBCOMMON REQUIRED xkbcommon)
pkg_check_modules(UDEV REQUIRED libudev)
pkg_check_modules(SYSTEMD REQUIRED libsystemd)
pkg_check_modules(PIXMAN REQUIRED pixman-1)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${DRM_INCLUDE_DIRS})
include_directories(${GBM_INCLUDE_DIRS})
include_directories(${EGL_INCLUDE_DIRS})
include_directories(${GLESV2_INCLUDE_DIRS})
include_directories(${LIBINPUT_INCLUDE_DIRS})
include_directories(${XKBCOMMON_INCLUDE_DIRS})
include_directories(${UDEV_INCLUDE_DIRS})
include_directories(${SYSTEMD_INCLUDE_DIRS})
include_directories(${PIXMAN_INCLUDE_DIRS})

# Compiler flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wno-unused-parameter")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g -O0 -DDEBUG")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O2 -DNDEBUG")

# Link directories
link_directories(${DRM_LIBRARY_DIRS})
link_directories(${GBM_LIBRARY_DIRS})
link_directories(${EGL_LIBRARY_DIRS})
link_directories(${GLESV2_LIBRARY_DIRS})
link_directories(${LIBINPUT_LIBRARY_DIRS})
link_directories(${XKBCOMMON_LIBRARY_DIRS})
link_directories(${UDEV_LIBRARY_DIRS})
link_directories(${SYSTEMD_LIBRARY_DIRS})
link_directories(${PIXMAN_LIBRARY_DIRS})

# Source files
set(VLDWM_SOURCES
    src/main.c
    src/core/server.c
    src/renderer/display.c
    src/input/input.c
)

# Create executable
add_executable(vldwm ${VLDWM_SOURCES})

# Link libraries
target_link_libraries(vldwm
    ${DRM_LIBRARIES}
    ${GBM_LIBRARIES}
    ${EGL_LIBRARIES}
    ${GLESV2_LIBRARIES}
    ${LIBINPUT_LIBRARIES}
    ${XKBCOMMON_LIBRARIES}
    ${UDEV_LIBRARIES}
    ${SYSTEMD_LIBRARIES}
    ${PIXMAN_LIBRARIES}
    m
)

# Install
install(TARGETS vldwm DESTINATION bin)
install(DIRECTORY include/ DESTINATION include)

# Documentation
find_program(DOXYGEN_EXECUTABLE doxygen)
if(DOXYGEN_EXECUTABLE)
    configure_file(
        ${CMAKE_SOURCE_DIR}/docs/Doxyfile.in
        ${CMAKE_BINARY_DIR}/Doxyfile
        @ONLY
    )
    
    add_custom_target(docs
        ${DOXYGEN_EXECUTABLE} ${CMAKE_BINARY_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Generating API documentation"
        VERBATIM
    )
endif()

# Tests
enable_testing()
add_subdirectory(tests OPTIONAL)
