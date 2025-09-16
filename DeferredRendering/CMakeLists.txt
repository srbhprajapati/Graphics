
cmake_minimum_required(VERSION 3.25)

# Main Project related declaration
project(DeferredRendering)
set(CMAKE_CXX_STANDARD 17)


# Source Files
set(SOURCE_FILES	src/main.cpp
					src/AssetLoader/FBXLoader.h
					src/AssetLoader/FBXLoader.cpp
					src/Common/SceneData.h
					src/Common/SceneData.cpp
					src/Engine/Core/SceneGraph.h
					src/Engine/Core/SceneGraph.cpp
				    src/Engine/Core/Entity.h
				    src/Engine/Core/Entity.cpp
					src/Engine/Core/Components.h)


	
source_group("AssetLoader" FILES	src/AssetLoader/FBXLoader.h
									src/AssetLoader/FBXLoader.cpp)
									
source_group("Common" FILES	src/Common/SceneData.h
							src/Common/SceneData.cpp)

source_group("Engine/Core" FILES	src/Engine/Core/SceneGraph.h
									src/Engine/Core/SceneGraph.cpp
									src/Engine/Core/Entity.h
									src/Engine/Core/Entity.cpp
									src/Engine/Core/Components.h)





# Main Executable
add_executable(DeferredRendering ${SOURCE_FILES})




##### 3rd party libraries 

#FBX
set(FBX_SDK_DIR "C:/Program Files/Autodesk/FBX/FBX SDK/2020.3.7")
add_library(FbxSdk SHARED IMPORTED)
set_target_properties(FbxSdk PROPERTIES 
    IMPORTED_LOCATION "${FBX_SDK_DIR}/lib/x64/debug/libfbxsdk.dll"
    IMPORTED_IMPLIB "${FBX_SDK_DIR}/lib/x64/debug/libfbxsdk.lib"
    INTERFACE_INCLUDE_DIRECTORIES "${FBX_SDK_DIR}/include"
)
add_compile_definitions(FBXSDK_SHARED)

#GLFW
add_library(GLFW SHARED IMPORTED)
set_target_properties(GLFW PROPERTIES 
    IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/ThirdParty/glfw-3.4/lib-vc2019/glfw3.dll"
    IMPORTED_IMPLIB "${CMAKE_SOURCE_DIR}/ThirdParty/glfw-3.4/lib-vc2019/glfw3.lib"
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/ThirdParty/glfw-3.4/include"
)


#ENTT
add_library(Entt INTERFACE IMPORTED)
set_target_properties(Entt PROPERTIES 
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/ThirdParty/entt"
)

#GLM
add_library(GLM INTERFACE IMPORTED)
set_target_properties(GLM PROPERTIES 
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/ThirdParty/glm"
)


set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#Include Directories
target_include_directories(DeferredRendering PRIVATE $(CMAKE_CURRENT_SOURCE_DIR)/src)



target_link_libraries(DeferredRendering PRIVATE GLFW
												FbxSdk
												Entt
												GLM)


if(WIN32)

add_custom_command(TARGET DeferredRendering POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:FbxSdk> $<TARGET_FILE_DIR:DeferredRendering>
	COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:GLFW> $<TARGET_FILE_DIR:DeferredRendering>
    COMMENT "Copying runtime DLLs for DeferredRendering"
)
endif()