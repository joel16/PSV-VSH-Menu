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
)

target_link_libraries(${PROJECT_NAME}.elf
	taihenForKernel_stub
	SceModulemgrForDriver_stub
	SceIofilemgrForDriver_stub
	SceSysmemForDriver_stub
	SceSysrootForKernel_stub
)

set_target_properties(${PROJECT_NAME}.elf
	PROPERTIES LINK_FLAGS "-nostdlib" COMPILE_FLAGS "-std=gnu99 -D__VITA_KERNEL__ ${KERNEL_DEBUG_DEFS}"
)

vita_create_self(${PROJECT_NAME}.skprx ${PROJECT_NAME}.elf UNSAFE CONFIG ${CMAKE_SOURCE_DIR}/exports/${PROJECT_NAME}.yml)
vita_create_stubs(${PROJECT_NAME}-stubs ${PROJECT_NAME}.elf ${CMAKE_SOURCE_DIR}/exports/${PROJECT_NAME}.yml KERNEL)

add_dependencies(${PROJECT_NAME}.skprx ${PROJECT_NAME}.elf)