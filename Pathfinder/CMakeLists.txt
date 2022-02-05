# CMakeList.txt : CMake project for Pathfinder, include source and define
# project specific logic here.
#

## If you want to link SFML statically
 set(SFML_STATIC_LIBRARIES TRUE)

## In most cases better set in the CMake cache
set(SFML_DIR "D:\\MyStorage\\Static_Libraries\\SFML-2.5.1\\lib\\cmake\\SFML")

find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)


# Add source to this project's executable.
add_executable (Pathfinder "Source_Files/Pathfinder.cpp" "Header_Files/Pathfinder.h" "Header_Files/Button.h" "Source_Files/Button.cpp" "Source_Files/main.cpp" "Header_Files/MazeGenerator.h" "Source_Files/MazeGenerator.cpp" "Header_Files/TextBox.h" "Source_Files/TextBox.cpp")
target_link_libraries(Pathfinder sfml-graphics sfml-audio)

# TODO: Add tests and install targets if needed.
