cmake_minimum_required(VERSION 3.12)
project(VulkanPlayground)

set(CMAKE_CXX_STANDARD 17)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_executable(VulkanPlayground main.cpp include/Common.h include/VulkanAPI.h src/rendering/VulkanAPI.cpp src/rendering/Window.cpp include/Window.h src/rendering/RenderPass.cpp include/RenderPass.h src/util/Util.cpp include/Util.h src/rendering/Buffer.cpp include/Buffer.h src/rendering/Shader.cpp include/Shader.h src/util/Log.cpp include/Log.h src/memory/PoolAllocator.cpp include/PoolAllocator.h src/core/Daybreak.cpp include/Daybreak.h include/Game.h src/rendering/DescriptorSet.cpp include/DescriptorSet.h src/rendering/Mesh.cpp include/Mesh.h src/rendering/UniformBuffer.cpp include/UniformBuffer.h src/rendering/Pipeline.cpp include/Pipeline.h src/rendering/Model.cpp include/Model.h src/asset/AssetManager.cpp include/AssetManager.h src/util/Timer.cpp include/Timer.h src/core/Scene.cpp include/Scene.h include/Entity.h src/core/GameComponent.cpp include/GameComponent.h src/core/GameObject.cpp include/GameObject.h src/components/MeshComponent.cpp include/MeshComponent.h)

# Vulkan
find_package(Vulkan REQUIRED)

#GLFW
add_subdirectory(dependencies/glfw)

#GLM
add_subdirectory(dependencies/glm)

target_link_libraries(VulkanPlayground glm glfw Vulkan::Vulkan)