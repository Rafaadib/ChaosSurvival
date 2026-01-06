
cmake_minimum_required(VERSION 3.11)

#nama project
project(ChaosKnight)

#set standar versi cpp
set(CMAKE_CXX_STANDARD 17)

#donload raylib
include(FetchContent)
FetchContent_Declare(
	raylib
	GIT_REPOSITORY https://github.com/raysan5/raylib.git
	GIT_TAG		5.5
)
FetchContent_MakeAvailable(raylib)



#file program
file(GLOB_RECURSE SOURCES "ChaosKnight/Src/*.cpp" "ChaosKnight/Src/*.h")

#ambil main
list(APPEND SOURCES "ChaosKnight/main.cpp")

#Executable
add_executable(${PROJECT_NAME} ${SOURCES})

#letak headerfile
target_include_directories(${PROJECT_NAME} PRIVATE "ChaosKnight/Src")

#link ke raylib
target_link_libraries(${PROJECT_NAME} PRIVATE raylib)

#sembunyikan konsol
#if(MSVC)
	#set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE ON) 
#endif()

#asset
file(COPY "${CMAKE_SOURCE_DIR}/ChaosKnight/GameAsset" DESTINATION "${CMAKE_BINARY_DIR}")
file(COPY "${CMAKE_SOURCE_DIR}/ChaosKnight/Sound" DESTINATION "${CMAKE_BINARY_DIR}")

message(STATUS "Copy asset ka dulu wak....")








