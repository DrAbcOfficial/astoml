cmake_minimum_required(VERSION 3.16.0 FATAL_ERROR)

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

project(ascron C CXX)

################################################################################
# Set target arch type if empty. Visual studio solution generator provides it.
################################################################################
if(NOT CMAKE_VS_PLATFORM_NAME)
    set(CMAKE_VS_PLATFORM_NAME "Win32")
endif()
message("${CMAKE_VS_PLATFORM_NAME} architecture in use")

if(NOT ("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "Win32"))
    message(FATAL_ERROR "${CMAKE_VS_PLATFORM_NAME} arch is not supported!")
endif()

################################################################################
# Global configuration types
################################################################################
set(CMAKE_CONFIGURATION_TYPES
    "Debug"
    "Release"
    CACHE STRING "" FORCE
)

################################################################################
# Global compiler options
################################################################################
set(CMAKE_C_FLAGS "-m32")
set(CMAKE_C_FLAGS_DEBUG "-m32")
set(CMAKE_C_FLAGS_RELEASE "-m32")
set(CMAKE_CXX_FLAGS "-m32")
set(CMAKE_CXX_FLAGS_DEBUG "-m32")
set(CMAKE_CXX_FLAGS_RELEASE "-m32")

################################################################################
# Global linker options
################################################################################
if(MSVC)
    # remove default flags provided with CMake for MSVC
    set(CMAKE_EXE_LINKER_FLAGS "")
    set(CMAKE_MODULE_LINKER_FLAGS "")
    set(CMAKE_SHARED_LINKER_FLAGS "")
    set(CMAKE_STATIC_LINKER_FLAGS "")
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS}")
    set(CMAKE_MODULE_LINKER_FLAGS_DEBUG "${CMAKE_MODULE_LINKER_FLAGS}")
    set(CMAKE_SHARED_LINKER_FLAGS_DEBUG "${CMAKE_SHARED_LINKER_FLAGS}")
    set(CMAKE_STATIC_LINKER_FLAGS_DEBUG "${CMAKE_STATIC_LINKER_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS}")
    set(CMAKE_MODULE_LINKER_FLAGS_RELEASE "${CMAKE_MODULE_LINKER_FLAGS}")
    set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS}")
    set(CMAKE_STATIC_LINKER_FLAGS_RELEASE "${CMAKE_STATIC_LINKER_FLAGS}")
endif()

################################################################################
# Nuget packages function stub.
################################################################################
function(use_package TARGET PACKAGE VERSION)
    message(WARNING "No implementation of use_package. Create yours. "
                    "Package \"${PACKAGE}\" with version \"${VERSION}\" "
                    "for target \"${TARGET}\" is ignored!")
endfunction()

################################################################################
# Common utils
################################################################################
include(CMake/Utils.cmake)

################################################################################
# Additional Global Settings(add specific info there)
################################################################################
include(CMake/GlobalSettingsInclude.cmake OPTIONAL)

################################################################################
# Use solution folders feature
################################################################################
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

################################################################################
# Sub-projects
################################################################################

set(PROJECT_NAME ascron)

################################################################################
# Source groups
################################################################################
set(no_group_source_files
    "src/angelscript/angelscriptlib.h"
    "src/header/angelscript.h"
    "src/header/CASBaseObject.h"
    "src/header/CASCron.h"
    "src/header/dlldef.h"
    "src/header/enginedef.h"
    "src/header/info_name.h"
    "src/header/signatures.h"
    "src/header/utility.h"
    "src/source/angelscript.cpp"
    "src/source/CASBaseObject.cpp"
    "src/source/CASCron.cpp"
    "src/source/dllapi.cpp"
    "src/source/engine_api.cpp"
    "src/source/h_export.cpp"
    "src/source/meta_api.cpp"
    "src/source/sdk_util.cpp"
)
source_group("" FILES ${no_group_source_files})

set(detour
    "../CDetour/asm/asm.c"
    "../CDetour/detours.cpp"
    "../CDetour/libudis86/decode.c"
    "../CDetour/libudis86/itab.c"
    "../CDetour/libudis86/udis86.c"
)
source_group("detour" FILES ${detour})

set(libcron
    "src/libcron/Cron.h"
    "src/libcron/CronClock.cpp"
    "src/libcron/CronClock.h"
    "src/libcron/CronData.cpp"
    "src/libcron/CronData.h"
    "src/libcron/CronRandomization.cpp"
    "src/libcron/CronRandomization.h"
    "src/libcron/CronSchedule.cpp"
    "src/libcron/CronSchedule.h"
    "src/libcron/DateTime.h"
    "src/libcron/Task.cpp"
    "src/libcron/Task.h"
    "src/libcron/TaskQueue.h"
    "src/libcron/TimeTypes.h"
)
source_group("libcron" FILES ${libcron})

set(date
    "src/date/date.h"
)
source_group("date" FILES ${date})


set(ALL_FILES
    ${no_group_source_files}
    ${detour}
    ${libcron}
    ${date}
)

################################################################################
# Target
################################################################################
add_library(${PROJECT_NAME} SHARED ${ALL_FILES})

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE ascron)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_GLOBAL_KEYWORD "Win32Proj"
)
set_target_properties(${PROJECT_NAME} PROPERTIES
    INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
)
################################################################################
# MSVC runtime library
################################################################################
get_property(MSVC_RUNTIME_LIBRARY_DEFAULT TARGET ${PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY)
string(CONCAT "MSVC_RUNTIME_LIBRARY_STR"
    $<$<CONFIG:Debug>:
        MultiThreadedDebug
    >
    $<$<CONFIG:Release>:
        MultiThreaded
    >
    $<$<NOT:$<OR:$<CONFIG:Debug>,$<CONFIG:Release>>>:${MSVC_RUNTIME_LIBRARY_DEFAULT}>
)
set_target_properties(${PROJECT_NAME} PROPERTIES MSVC_RUNTIME_LIBRARY ${MSVC_RUNTIME_LIBRARY_STR})

################################################################################
# Include directories
################################################################################
target_include_directories(${PROJECT_NAME} PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/src/;"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/header;"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/angelscript;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../hlsdk/common;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../hlsdk/dlls;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../hlsdk/pm_shared;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../hlsdk/engine;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../metamod;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../CDetour;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../asext/include"
)

################################################################################
# Compile definitions
################################################################################
target_compile_definitions(${PROJECT_NAME} PRIVATE
    "$<$<CONFIG:Debug>:"
        "_DEBUG"
    ">"
    "$<$<CONFIG:Release>:"
        "NDEBUG"
    ">"
    "_USRDLL;"
    "_CRT_SECURE_NO_WARNINGS;"
    "UNICODE;"
    "_UNICODE"
)

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:
            /permissive-;
            /Od
        >
        $<$<CONFIG:Release>:
            /O2;
            /Oi;
            /Gy
        >
        /sdl;
        /W3;
        ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
        ${DEFAULT_CXX_EXCEPTION_HANDLING}
    )
    target_link_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:
            /SUBSYSTEM:WINDOWS;
            /INCREMENTAL
        >
        $<$<CONFIG:Release>:
            /OPT:REF;
            /OPT:ICF
        >
        /DEBUG
    )
else()
	set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
    	set(CMAKE_CXX_FLAGS_RELEASE "-Os")
endif()

################################################################################
# Post build events
################################################################################
add_custom_command_if(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMANDS
    COMMAND $<CONFIG:Release> copy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>${TARGET_NAME}$ENV{TargetExt}" "$<SHELL_PATH:${CMAKE_CURRENT_SOURCE_DIR}/>/../build/addons/metamod/dlls/"
)

