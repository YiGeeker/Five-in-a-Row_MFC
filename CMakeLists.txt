cmake_minimum_required(VERSION 2.8.0)

project(Five-in-a-Row CXX RC)

set(CMAKE_MFC_FLAG 2)		# Use MFC in a Shared DLL

add_definitions(-D_AFXDLL)
include_directories(inc)
aux_source_directory(src SRC_LIST)
set(RES_LIST res/Five-in-a-Row.rc)

add_executable(${PROJECT_NAME} WIN32 ${SRC_LIST} ${RES_LIST})

install(
  TARGETS ${PROJECT_NAME}
  CONFIGURATIONS Debug
  DESTINATION ${PROJECT_SOURCE_DIR}/bin/Debug
  )

install(
  TARGETS ${PROJECT_NAME}
  CONFIGURATIONS Release
  DESTINATION ${PROJECT_SOURCE_DIR}/bin/Release
  )
