# Set project name
project(SampleProject)
# Define target name
set(TARGET_NAME SampleProject)
######################################
# Set minimum version

cmake_minimum_required(VERSION 2.8.6)
if (COMMAND cmake_policy)
    cmake_policy(SET CMP0003 NEW)
    if (CMAKE_VERSION VERSION_GREATER 2.8.12 OR CMAKE_VERSION VERSION_EQUAL 2.8.12)
        # INTERFACE_LINK_LIBRARIES defines the link interface
        cmake_policy(SET CMP0022 NEW)
    endif ()
    if (CMAKE_VERSION VERSION_GREATER 3.0.0 OR CMAKE_VERSION VERSION_EQUAL 3.0.0)
        # Disallow use of the LOCATION target property - therefore we set to OLD as we still need it
        cmake_policy(SET CMP0026 OLD)
        # MACOSX_RPATH is enabled by default
        cmake_policy(SET CMP0042 NEW)
    endif ()
endif ()
# Set CMake modules search path
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake/Modules)
# Include Urho3D Cmake common module
include(Urho3D-CMake-common)
# Find Urho3D library

find_package(Urho3D REQUIRED)
include_directories(${URHO3D_INCLUDE_DIRS})
message(STATUS ${URHO3D_INCLUDE_DIRS})

# Define source files
set(SRC_CPP_FILES
        src/emulator/adb.c
        src/emulator/clock.c
        src/emulator/config.c
        src/emulator/dis.c
        src/emulator/engine_c.c
        src/emulator/scc.c
        src/emulator/iwm.c
        src/emulator/joystick_driver.c
        src/emulator/moremem.c
        src/emulator/paddles.c
        src/emulator/sim65816.c
        src/emulator/smartport.c
        src/emulator/sound.c
        src/emulator/video.c
        src/emulator/scc_socket_driver.c
        src/emulator/scc_windriver.c
        src/emulator/urho_driver.c
        src/emulator/urho_sound_driver.c
        src/emulator/sound_driver.c

        src/controls/EmulatorControls.cpp
        src/controls/PlayerControls.cpp

        src/GameCore.cpp
        )
set(SRC_H_FILES
        src/emulator/adb.h
        src/emulator/defc.h
        src/emulator/defcomm.h
        src/emulator/iwm.h
        src/emulator/protos.h
        src/emulator/config.h
        src/emulator/disas.h
        src/emulator/protos_engine_c.h
        src/emulator/size_c.h
        src/emulator/op_routs.h
        src/emulator/defs_instr.h
        src/emulator/8inst_c.h
        src/emulator/16inst_c.h
        src/emulator/scc.h
        src/emulator/iwm_35_525.h
        src/emulator/sound.h
        src/emulator/superhires.h
        src/emulator/kegsfont.h
        src/emulator/scc.h
        src/emulator/protos_urho_driver.h

        src/controls/Controls.h
        src/controls/PlayerControls.h
        src/controls/EmulatorControls.h
        src/controls/EmuDeclares.h

        src/GameCore.h
        )
define_source_files(GROUP EXTRA_CPP_FILES ${SRC_CPP_FILES} EXTRA_H_FILES ${SRC_H_FILES})

# Setup target with resource copying
setup_main_executable()
# Activate C++11
add_compile_options("-std=c++17")

set(BOOST_PATH $ENV{LIBS})
string(REPLACE "\\" "/" BOOST_PATH "${BOOST_PATH}")

set(Boost_ARCHITECTURE -x64) # important with static
set(Boost_USE_STATIC_LIBS ON)
set(BOOST_INCLUDEDIR ${BOOST_PATH}/include/boost-1_74)
set(BOOST_LIBRARYDIR ${BOOST_PATH}/lib)

find_package(Boost COMPONENTS regex thread)
include_directories(${Boost_INCLUDE_DIRS})
target_link_libraries(SampleProject ${Boost_LIBRARIES})