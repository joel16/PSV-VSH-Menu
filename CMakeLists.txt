cmake_minimum_required(VERSION 2.8)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
	if(DEFINED ENV{VITASDK})
		set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake" CACHE PATH "toolchain file")
	else()
		message(FATAL_ERROR "Please define VITASDK to point to your SDK path!")
	endif()
endif()

project(vsh)
include("${VITASDK}/share/vita.cmake" REQUIRED)
	
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wl,-q -Wall -O3 -std=gnu99")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fno-rtti -fno-exceptions")

include_directories(
	include
)

link_directories(
	${CMAKE_CURRENT_BINARY_DIR}
)

if (NOT ${RELEASE})
	add_definitions(-DENABLE_LOGGING)
endif()

add_executable(${PROJECT_NAME}.elf
	source/vsh.c
	source/config.c
	source/draw.c
	source/font.c
	source/fs.c
	source/menus.c
	source/power.c
	source/utils.c
)

target_link_libraries(${PROJECT_NAME}.elf
	taihen_stub
	SceAppMgr_stub_weak
	SceDisplay_stub_weak
	SceLibKernel_stub_weak
	ScePower_stub_weak
	SceSysmem_stub_weak
	kuio_stub_weak
	k
	gcc
)

set_target_properties(${PROJECT_NAME}.elf
	PROPERTIES LINK_FLAGS "-nostdlib"
)

add_custom_target(${PROJECT_NAME}.suprx ALL
	COMMAND vita-elf-create -e ${CMAKE_SOURCE_DIR}/exports/${PROJECT_NAME}.yml ${PROJECT_NAME}.elf ${PROJECT_NAME}.velf
	COMMAND vita-make-fself -c ${PROJECT_NAME}.velf ${PROJECT_NAME}.suprx
)
add_dependencies(${PROJECT_NAME}.suprx ${PROJECT_NAME}.elf)