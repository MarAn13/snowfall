include(FetchContent)

Set(FETCHCONTENT_QUIET FALSE)

# SFML
FetchContent_Declare(
	sfml
	GIT_REPOSITORY https://github.com/SFML/SFML.git
	GIT_TAG 2.6.x
	GIT_PROGRESS TRUE
)
option(SFML_BUILD_AUDIO "Build audio" OFF)
option(SFML_BUILD_NETWORK "Build network" OFF)
add_subdirectory(sfml)

# Dear ImGui
FetchContent_Declare(
	imgui
	GIT_REPOSITORY https://github.com/ocornut/imgui
	GIT_TAG v1.90.1
	GIT_PROGRESS TRUE
)

FetchContent_MakeAvailable(imgui)

# ImGui-SFML
FetchContent_Declare(
	imgui-sfml
	GIT_REPOSITORY https://github.com/SFML/imgui-sfml
	GIT_TAG 2.6.x
	GIT_PROGRESS TRUE
)
add_subdirectory(imgui-sfml)