/**
 * Copyright 2018 Carson Clarke-Magrab
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
**/

#ifndef _VULKAN_API_H
#define _VULKAN_API_H

#include "Common.h"
#include "Window.h"

#ifdef API_DEBUG
static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
        void *pUserData) {
    std::cerr << "validation layer: " << pCallbackData->pMessage << pCallbackData->pMessageIdName << std::endl;

    return VK_FALSE;
}
#endif

namespace daybreak {

    struct QueueFamilyIndices {
        std::optional<uint32_t> graphics_family;
        std::optional<uint32_t> present_family;

        bool isComplete() {
            return graphics_family.has_value() && present_family.has_value();
        }
    };

    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> present_modes;
    };

    struct FlyingBuffer {
        VkBuffer buffer;
        VkDeviceMemory memory;
        VkDeviceSize size;
        VkBufferUsageFlagBits usage;
    };

    struct FlyingSet {
        VkDescriptorSet set;
        VkPipeline pipeline;
    };

    struct GarbageImage {
        VkImage image;
        VkDeviceMemory memory;
    };

    class API {
    private:
#ifdef API_DEBUG
        static VkDebugUtilsMessengerEXT callback;

        static VkResult CreateDebugUtilsMessengerEXT(VkInstance instance,
                                                     const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
                                                     const VkAllocationCallbacks *pAllocator,
                                                     VkDebugUtilsMessengerEXT *pCallback);
#endif
    public:
        static void init_vulkan();
        static void recreate_swapchain();

        static bool find_buffer(VkDeviceSize size, VkBufferUsageFlagBits usage, VkBuffer& buffer, VkDeviceMemory& memory);
        static void destroy_buffer(VkDeviceSize size, VkBufferUsageFlagBits usage, VkBuffer buffer, VkDeviceMemory memory);
        static bool find_desc_set(VkPipeline pipeline, VkDescriptorSet& set);
        static void destroy_desc_set(VkPipeline pipeline, VkDescriptorSet set);
        static uint32_t find_memory_type(uint32_t filter, VkMemoryPropertyFlags props);
        static bool is_depth_format(VkFormat format);

        static VkCommandBuffer begin_render_command_buffer();
        static void end_render_command_buffer();
        static void begin_present_pass();
        static void end_present_pass();
        static VkCommandBuffer begin_command_buffer();
        static void end_command_buffer(VkCommandBuffer cmd_buffer);

        static VkDevice device();
        static VkViewport viewport();
        static VkRect2D& scissor();
        static VkDescriptorPool descriptor_pool();
    private:
        static bool is_device_suitable(VkPhysicalDevice device, VkSurfaceKHR surface);
        static QueueFamilyIndices find_queue_families(VkPhysicalDevice device, VkSurfaceKHR surface);
        static SwapChainSupportDetails query_swapchain_support(VkPhysicalDevice device, VkSurfaceKHR surface);
        static VkSurfaceFormatKHR choose_surface_format(const std::vector<VkSurfaceFormatKHR>& available_formats);
        static VkPresentModeKHR choose_present_mode(const std::vector<VkPresentModeKHR> available_modes);
        static VkExtent2D choose_extent(const VkSurfaceCapabilitiesKHR& capabilities);
        static bool check_device_extension_support(VkPhysicalDevice device);
    };
}

#endif
