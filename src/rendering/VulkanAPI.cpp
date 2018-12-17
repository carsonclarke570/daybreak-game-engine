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

#include "../../include/VulkanAPI.h"

#include "../../include/RenderPass.h"
#include "../../include/Shader.h"
#include "../../include/Buffer.h"

namespace daybreak {

    static VkInstance m_instance;
    static VkPhysicalDevice m_physical_device;
    static VkDevice m_device;
    static VkSurfaceKHR m_surface;
    static VkQueue m_present, m_graphics;
    static VkSurfaceFormatKHR m_surface_format;
    static VkPresentModeKHR m_present_mode;
    static VkSwapchainKHR m_swapchain;
    static VkViewport m_viewport = {};
    static VkImage m_swapchain_images[2];
    static VkImageView m_swapchain_image_views[2];
    static VkFramebuffer m_framebuffers[2];
    static VkCommandPool *m_command_pool;
    static VkFence *m_command_fence;
    static VkCommandBuffer m_render_command_buffers[2];
    static VkSemaphore m_image_available;
    static VkSemaphore m_render_finished;
    static VkDescriptorPool m_descriptor_pool;
    static VkSurfaceCapabilitiesKHR m_surface_caps;
    static VkExtent2D m_extent;
    static VkRect2D m_scissor;

    static std::vector<FlyingBuffer> used_buffers, free_buffers;
    static std::vector<FlyingSet> used_sets, free_sets;
    static std::vector<VkFramebuffer> m_garbage_framebuffers;
    static std::vector<GarbageImage> m_garbage_images;

    void API::init_vulkan() {
#ifdef API_DEBUG
        const char *validation_layers[] = {
                "VK_LAYER_LUNARG_standard_validation",
        };
#endif
        LOG("Vulkan - Initializing");
        { // Create Instance
            VkApplicationInfo app_info = {};
            app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
            app_info.pEngineName = "Daybreak Engine";
            app_info.apiVersion = VK_API_VERSION_1_0;

            uint32_t glfw_exts_count = 0;
            const char **glfw_exts;
            glfw_exts = glfwGetRequiredInstanceExtensions(&glfw_exts_count);

            std::vector<const char *> extensions(glfw_exts, glfw_exts + glfw_exts_count);
#ifdef API_DEBUG
            extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif
            VkInstanceCreateInfo create_info = {};
            create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
            create_info.pApplicationInfo = &app_info;
            create_info.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
            create_info.ppEnabledExtensionNames = extensions.data();
#ifdef API_DEBUG
            create_info.enabledLayerCount = COUNT_OF(validation_layers);
            create_info.ppEnabledLayerNames = validation_layers;
#endif
            if (vkCreateInstance(&create_info, nullptr, &m_instance) != VK_SUCCESS) {
                throw std::runtime_error("Failed to create instance!");
            }
        }

#ifdef API_DEBUG
        { // Create Debug callback
            VkDebugUtilsMessengerCreateInfoEXT create_info = {};
            create_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
            create_info.messageSeverity =
                    VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                    VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
            create_info.messageType =
                    VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                    VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
            create_info.pfnUserCallback = debugCallback;

            if (CreateDebugUtilsMessengerEXT(m_instance, &create_info, nullptr, &callback) != VK_SUCCESS) {
                throw std::runtime_error("failed to set up debug callback!");
            }
        }
#endif
        { // Create surface
            if (glfwCreateWindowSurface(m_instance, Window::get_window(), nullptr, &m_surface) != VK_SUCCESS) {
                throw std::runtime_error("failed to create window surface!");
            }
        }

        { // Pick Physical Device
            uint32_t count = 0;
            vkEnumeratePhysicalDevices(m_instance, &count, nullptr);
            if (count == 0) {
                throw std::runtime_error("failed to find GPUs with Vulkan support");
            }

            std::vector<VkPhysicalDevice> devices(count);
            vkEnumeratePhysicalDevices(m_instance, &count, devices.data());
            for (const auto &d : devices) {
                if (is_device_suitable(d, m_surface)) {
                    m_physical_device = d;
                    break;
                }
            }
            if (m_physical_device == VK_NULL_HANDLE) {
                throw std::runtime_error("failed to find suitable GPU!");
            }
        }

        QueueFamilyIndices queue_indices = find_queue_families(m_physical_device, m_surface);
        { // Create logical device
            std::vector<VkDeviceQueueCreateInfo> queue_create_infos;
            std::set<uint32_t> unique_families = {
                    queue_indices.graphics_family.value(),
                    queue_indices.present_family.value()
            };

            float priority = 1.0f;
            for (uint32_t family : unique_families) {
                VkDeviceQueueCreateInfo queue_create_info = {};
                queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
                queue_create_info.queueFamilyIndex = family;
                queue_create_info.queueCount = 1;
                queue_create_info.pQueuePriorities = &priority;
                queue_create_infos.push_back(queue_create_info);
            }
            const char *device_exts[] = {
                    VK_KHR_SWAPCHAIN_EXTENSION_NAME
            };

            VkPhysicalDeviceFeatures features = {};
            features.samplerAnisotropy = static_cast<VkBool32>(true);
            features.textureCompressionBC = static_cast<VkBool32>(true);

            VkDeviceCreateInfo create_info = {};
            create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
            create_info.pQueueCreateInfos = queue_create_infos.data();
            create_info.queueCreateInfoCount = static_cast<uint32_t>(queue_create_infos.size());
            create_info.pEnabledFeatures = &features;
            create_info.enabledExtensionCount = COUNT_OF(device_exts);
            create_info.ppEnabledExtensionNames = device_exts;
#ifdef API_DEBUG
            create_info.enabledLayerCount = COUNT_OF(validation_layers);
            create_info.ppEnabledLayerNames = validation_layers;
#endif

            if (vkCreateDevice(m_physical_device, &create_info, nullptr, &m_device) != VK_SUCCESS) {
                throw std::runtime_error("failed to create logical device");
            }

            vkGetDeviceQueue(m_device, queue_indices.graphics_family.value(), 0, &m_graphics);
            vkGetDeviceQueue(m_device, queue_indices.present_family.value(), 0, &m_present);
        }

        SwapChainSupportDetails support_details = query_swapchain_support(m_physical_device, m_surface);
        { // Get swap chain details
            m_surface_format = choose_surface_format(support_details.formats);
            m_present_mode = choose_present_mode(support_details.present_modes);
        }

        { // Create command pool
            const uint32_t pool_count = 12; // TO-DO: Determine actual number
            m_command_pool = new VkCommandPool[pool_count]; //TO-DO: Create memory allocateo

            VkCommandPoolCreateInfo pool_info = {};
            pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
            pool_info.queueFamilyIndex = queue_indices.present_family.value();
            pool_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

            for (int i = 0; i < pool_count; i++) {
                if (vkCreateCommandPool(m_device, &pool_info, nullptr, &m_command_pool[i]) != VK_SUCCESS) {
                    throw std::runtime_error("failed to create command pool");
                }
            }
        }

        { // Create command fences
            const uint32_t fence_count = 12;
            m_command_fence = new VkFence[fence_count];

            VkFenceCreateInfo create_info = {};
            create_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;

            for (int i = 0; i < fence_count; i++) {
                if (vkCreateFence(m_device, &create_info, nullptr, &m_command_fence[i])) {
                    throw std::runtime_error("failed to create command fence");
                }
            }
        }

        { // Create command buffers
            VkCommandBufferAllocateInfo alloc_info = {};
            alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
            alloc_info.commandPool = *m_command_pool;
            alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
            alloc_info.commandBufferCount = COUNT_OF(m_render_command_buffers);

            if (vkAllocateCommandBuffers(m_device, &alloc_info, m_render_command_buffers) != VK_SUCCESS) {
                throw std::runtime_error("failed to allocate command buffer!");
            }
        }

        { // Create semaphores
            VkSemaphoreCreateInfo semaphore_info = {};
            semaphore_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

            if (vkCreateSemaphore(m_device, &semaphore_info, nullptr, &m_image_available) != VK_SUCCESS ||
                vkCreateSemaphore(m_device, &semaphore_info, nullptr, &m_render_finished)) {
                throw std::runtime_error("failed to create semaphores");
            }
        }

        { // Create descriptor pool
            VkDescriptorPoolSize pool_sizes[] = {
                    {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,         1 << 12},
                    {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1 << 10},
            };

            VkDescriptorPoolCreateInfo create_info = {};
            create_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
            create_info.poolSizeCount = COUNT_OF(pool_sizes);
            create_info.pPoolSizes = pool_sizes;
            create_info.maxSets = 1 << 16;
            create_info.flags = VkDescriptorPoolCreateFlagBits::VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;

            if (vkCreateDescriptorPool(m_device, &create_info, nullptr, &m_descriptor_pool)) {
                throw std::runtime_error("failed to create descriptor pool");
            }
        }

        recreate_swapchain();
        LOG("Vulkan - Initialization successful");
    }

    void API::recreate_swapchain() {
        int width = 0, height = 0;
        while (width == 0 || height == 0) {
            glfwGetFramebufferSize(Window::get_window(), &width, &height);
            glfwWaitEvents();
        }

        vkDeviceWaitIdle(m_device);

        { // Cleanup
            for (VkFramebuffer buffer : m_framebuffers) {
                vkDestroyFramebuffer(m_device, buffer, nullptr);
            }

            for (VkImageView image : m_swapchain_image_views) {
                vkDestroyImageView(m_device, image, nullptr);
            }
            vkDestroySwapchainKHR(m_device, m_swapchain, nullptr);
        }


        { // Surface capabilities
            vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_physical_device, m_surface, &m_surface_caps);
            ASSERT((m_surface_caps.currentExtent.width != -1), "VulkanAPI - Could not obtain valid extent");
            m_extent = choose_extent(m_surface_caps);
        }

        { // Viewport
            m_viewport.x = 0.0f;
            m_viewport.y = 0.0f;
            m_viewport.width = (float) m_extent.width;
            m_viewport.height = (float) m_extent.height;
            m_viewport.minDepth = 0.0f;
            m_viewport.maxDepth = 1.0f;
        }

        { // Scissor
            m_scissor.offset = {0,0};
            m_scissor.extent = m_extent;
        }

        { // Create swapchain
            VkSwapchainCreateInfoKHR create_info = {};
            create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
            create_info.surface = m_surface;
            create_info.minImageCount = 2;
            create_info.imageFormat = m_surface_format.format;
            create_info.imageColorSpace = m_surface_format.colorSpace;
            create_info.imageExtent = m_extent;
            create_info.imageArrayLayers = 1;
            create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

            QueueFamilyIndices indices = API::find_queue_families(m_physical_device, m_surface);
            uint32_t family_indices[] = {
                    indices.graphics_family.value(),
                    indices.present_family.value()
            };

            if (indices.graphics_family != indices.present_family) {
                create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
                create_info.queueFamilyIndexCount = 2;
                create_info.pQueueFamilyIndices = family_indices;
            } else {
                create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
                create_info.queueFamilyIndexCount = 0;
                create_info.pQueueFamilyIndices = nullptr;
            }
            create_info.preTransform = m_surface_caps.currentTransform;
            create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
            create_info.presentMode = m_present_mode;
            create_info.clipped = VK_TRUE;
            create_info.oldSwapchain = VK_NULL_HANDLE;

            if (vkCreateSwapchainKHR(m_device, &create_info, nullptr, &m_swapchain) != VK_SUCCESS) {
                throw std::runtime_error("failed to create swap chain!");
            }
        }

        { // Get swapchain images
            uint32_t image_count;
            vkGetSwapchainImagesKHR(m_device, m_swapchain, &image_count, nullptr);
            ASSERT((image_count == 2), "Vulkan API - Could not establish enough swapchain images");
            vkGetSwapchainImagesKHR(m_device, m_swapchain, &image_count, m_swapchain_images);
        }

        { // Create swapchain image views
            for (int i = 0; i < 2; i++) {
                VkImageViewCreateInfo view_create_info = {};
                view_create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
                view_create_info.image = m_swapchain_images[i];
                view_create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
                view_create_info.format = m_surface_format.format;
                view_create_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
                view_create_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
                view_create_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
                view_create_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
                view_create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
                view_create_info.subresourceRange.baseMipLevel = 0;
                view_create_info.subresourceRange.levelCount = 1;
                view_create_info.subresourceRange.baseArrayLayer = 0;
                view_create_info.subresourceRange.layerCount = 1;

                if (vkCreateImageView(m_device, &view_create_info, nullptr, &m_swapchain_image_views[i]) !=
                    VK_SUCCESS) {
                    throw std::runtime_error("failed to create image views!");
                }
            }
        }

        { // Create framebuffers
            for (int i = 0; i < COUNT_OF(m_swapchain_images); i++) {
                VkImageView attachments[] = {
                        m_swapchain_image_views[i],
                };

                VkFramebufferCreateInfo framebuffer_info = {};
                framebuffer_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
                framebuffer_info.renderPass = RenderPass::present_pass();
                framebuffer_info.attachmentCount = COUNT_OF(attachments);
                framebuffer_info.pAttachments = attachments;
                framebuffer_info.width = m_extent.width;
                framebuffer_info.height = m_extent.height;
                framebuffer_info.layers = 1;

                if (vkCreateFramebuffer(m_device, &framebuffer_info, nullptr, &m_framebuffers[i]) != VK_SUCCESS) {
                    throw std::runtime_error("failed to create framebuffer");
                }
            }
        }
    }

    bool API::find_buffer(VkDeviceSize size, VkBufferUsageFlagBits usage, VkBuffer &buffer, VkDeviceMemory &memory) {
        for (uint32_t i = 0; i < free_buffers.size(); i++) {
            const FlyingBuffer &flying_buffer = free_buffers[i];
            if (flying_buffer.size == size && flying_buffer.usage == usage) {
                buffer = flying_buffer.buffer;
                memory = flying_buffer.memory;
                std::swap(free_buffers[i], free_buffers.back());
                free_buffers.pop_back();
                return true;
            }
        }
        return false;
    }

    void API::destroy_buffer(VkDeviceSize size, VkBufferUsageFlagBits usage, VkBuffer buffer, VkDeviceMemory memory) {
        used_buffers.push_back(FlyingBuffer{buffer, memory, size, usage});
    }

    bool API::find_desc_set(VkPipeline pipeline, VkDescriptorSet& set) {
        for (uint32_t i = 0; i < free_sets.size(); i++) {
            const FlyingSet& flying_set = free_sets[i];
            if (flying_set.pipeline == pipeline) {
                set = flying_set.set;
                std::swap(free_sets[i], free_sets.back());
                free_sets.pop_back();
                return true;
            }
        }
        return false;
    }

    void API::destroy_desc_set(VkPipeline pipeline, VkDescriptorSet set) {
        used_sets.push_back(FlyingSet{set, pipeline});
    }

    uint32_t API::find_memory_type(uint32_t filter, VkMemoryPropertyFlags props) {
        VkPhysicalDeviceMemoryProperties mem_props;
        vkGetPhysicalDeviceMemoryProperties(m_physical_device, &mem_props);
        for (uint32_t i = 0; i < mem_props.memoryTypeCount; i++) {
            if ((filter & (1 << i)) && (mem_props.memoryTypes[i].propertyFlags & props) == props) {
                return i;
            }
        }
        throw std::runtime_error("failed to find suitable memory type!");
    }

    bool API::is_depth_format(VkFormat format) {
        switch (format) {
            case VK_FORMAT_D16_UNORM:
            case VK_FORMAT_D32_SFLOAT:
            case VK_FORMAT_D16_UNORM_S8_UINT:
            case VK_FORMAT_D24_UNORM_S8_UINT:
            case VK_FORMAT_D32_SFLOAT_S8_UINT:
                return true;
            default:
                return false;
        }
    }

    static uint32_t image_index;

    VkCommandBuffer API::begin_render_command_buffer() {
        vkAcquireNextImageKHR(m_device, m_swapchain, uint64_t(-1), m_image_available, VK_NULL_HANDLE, &image_index);
        VkCommandBuffer cmd_buffer = m_render_command_buffers[image_index];

        VkCommandBufferBeginInfo begin_info = {};
        begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        begin_info.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;

        if (vkBeginCommandBuffer(cmd_buffer, &begin_info) != VK_SUCCESS) {
            throw std::runtime_error("could not create command buffer");
        }
        return cmd_buffer;
    }

    void API::end_render_command_buffer() {
        VkCommandBuffer cmd_buffer = m_render_command_buffers[image_index];

        if (vkEndCommandBuffer(cmd_buffer) != VK_SUCCESS) {
            throw std::runtime_error("could not end command buffer");
        }

        VkSubmitInfo submit_info = {};
        submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkSemaphore wait_semaphores[] = {m_image_available};
        VkPipelineStageFlags wait_stages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        submit_info.waitSemaphoreCount = 1;
        submit_info.pWaitSemaphores = wait_semaphores;
        submit_info.pWaitDstStageMask = wait_stages;

        submit_info.commandBufferCount = 1;
        submit_info.pCommandBuffers = &cmd_buffer;

        VkSemaphore signal_semaphores[] = {m_render_finished};
        submit_info.signalSemaphoreCount = 1;
        submit_info.pSignalSemaphores = signal_semaphores;

        VkPresentInfoKHR present_info = {};
        present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

        present_info.waitSemaphoreCount = 1;
        present_info.pWaitSemaphores = signal_semaphores;

        VkSwapchainKHR swapChains[] = {m_swapchain};
        present_info.swapchainCount = 1;
        present_info.pSwapchains = swapChains;
        present_info.pImageIndices = &image_index;

        {
            if (vkQueueSubmit(m_graphics, 1, &submit_info, VK_NULL_HANDLE) != VK_SUCCESS) {
                throw std::runtime_error("could not submit to queue");
            }
            if (VkResult result = vkQueuePresentKHR(m_present, &present_info)) {
                if ((result == VK_ERROR_OUT_OF_DATE_KHR) || (result == VK_SUBOPTIMAL_KHR) || Window::should_resize()) {
                    recreate_swapchain();
                } else {
                    throw std::runtime_error("could not submit to queue");
                }
            }
            vkQueueWaitIdle(m_graphics);
        }

        for (const FlyingBuffer &buffer : used_buffers) {
            free_buffers.push_back(buffer);
        }
        used_buffers.clear();

        for (const FlyingSet &set : used_sets) {
            free_sets.push_back(set);
        }
        used_sets.clear();

        for (const VkFramebuffer &framebuffer : m_garbage_framebuffers) {
            vkDestroyFramebuffer(API::device(), framebuffer, nullptr);
        }
        m_garbage_framebuffers.clear();
        for (const GarbageImage &image : m_garbage_images) {
            vkDestroyImage(API::device(), image.image, nullptr);
            vkFreeMemory(API::device(), image.memory, nullptr);
        }
        m_garbage_images.clear();
    }

    void API::begin_present_pass() {
        VkCommandBuffer cmd_buffer = m_render_command_buffers[image_index];
        VkRenderPassBeginInfo renderpass_info = {};
        renderpass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderpass_info.renderPass = RenderPass::present_pass();
        renderpass_info.framebuffer = m_framebuffers[image_index];
        renderpass_info.renderArea.offset = {0, 0};
        renderpass_info.renderArea.extent = VkExtent2D{uint32_t(m_viewport.width), uint32_t(m_viewport.height)};

        VkClearValue clear_value{0.f, 0.f, 0.f, 1.f};
        renderpass_info.clearValueCount = 1;
        renderpass_info.pClearValues = &clear_value;

        vkCmdBeginRenderPass(cmd_buffer, &renderpass_info, VK_SUBPASS_CONTENTS_INLINE);
    }

    void API::end_present_pass() {
        VkCommandBuffer cmd_buffer = m_render_command_buffers[image_index];
        vkCmdEndRenderPass(cmd_buffer);
    }

    VkCommandBuffer API::begin_command_buffer() {
        VkCommandBufferAllocateInfo alloc_info = {};
        alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        alloc_info.commandPool = m_command_pool[0];
        alloc_info.commandBufferCount = 1;

        VkCommandBuffer cmd_buffer;
        vkAllocateCommandBuffers(m_device, &alloc_info, &cmd_buffer);

        VkCommandBufferBeginInfo begin_info = {};
        begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        vkBeginCommandBuffer(cmd_buffer, &begin_info);

        return cmd_buffer;
    }

    void API::end_command_buffer(VkCommandBuffer cmd_buffer) {
        vkEndCommandBuffer(cmd_buffer);

        VkSubmitInfo submit_info = {};
        submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submit_info.commandBufferCount = 1;
        submit_info.pCommandBuffers = &cmd_buffer;

        {
            VkFence fence = m_command_fence[0];
            vkQueueSubmit(m_present, 1, &submit_info, fence);
            vkResetFences(m_device, 1, &fence);
        }

        vkFreeCommandBuffers(m_device, m_command_pool[0], 1, &cmd_buffer);
    }

    bool API::is_device_suitable(VkPhysicalDevice device, VkSurfaceKHR surface) {
        QueueFamilyIndices indices = find_queue_families(device, surface);
        bool ext_support = API::check_device_extension_support(device);
        bool swapchain_adequate = false;
        if (ext_support) {
            SwapChainSupportDetails swapchain_support = API::query_swapchain_support(device, surface);
            swapchain_adequate = !swapchain_support.formats.empty() && !swapchain_support.present_modes.empty();
        }

        return indices.isComplete() && ext_support && swapchain_adequate;
    }

    QueueFamilyIndices API::find_queue_families(VkPhysicalDevice device, VkSurfaceKHR surface) {
        QueueFamilyIndices indices;
        uint32_t count = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &count, nullptr);
        std::vector<VkQueueFamilyProperties> queue_families(count);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &count, queue_families.data());
        uint32_t i = 0;
        for (const auto &family : queue_families) {
            if (family.queueCount > 0 && family.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                indices.graphics_family = i;
            }
            VkBool32 present_support;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &present_support);
            if (family.queueCount > 0 && present_support && family.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                indices.present_family = i;
            }
            if (indices.isComplete()) {
                break;
            }
            i++;
        }
        return indices;
    }

    SwapChainSupportDetails API::query_swapchain_support(VkPhysicalDevice device, VkSurfaceKHR surface) {
        SwapChainSupportDetails details;
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

        uint32_t count;
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &count, nullptr);

        if (count != 0) {
            details.formats.resize(count);
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &count, details.formats.data());
        }
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &count, nullptr);

        if (count != 0) {
            details.present_modes.resize(count);
            vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &count, details.present_modes.data());
        }

        return details;
    }

    VkSurfaceFormatKHR API::choose_surface_format(const std::vector<VkSurfaceFormatKHR> &available_formats) {
        if (available_formats.size() == 1 && available_formats[0].format == VK_FORMAT_UNDEFINED) {
            return {VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};
        }
        for (const auto &format : available_formats) {
            if (format.format == VK_FORMAT_B8G8R8A8_UNORM && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
                return format;
            }
        }

        return available_formats[0];
    }

    VkPresentModeKHR API::choose_present_mode(const std::vector<VkPresentModeKHR> available_modes) {
        VkPresentModeKHR best = VK_PRESENT_MODE_FIFO_KHR;
        for (const auto &mode: available_modes) {
            if (mode == VK_PRESENT_MODE_MAILBOX_KHR) {
                return mode;
            } else if (mode == VK_PRESENT_MODE_IMMEDIATE_KHR) {
                best = mode;
            }
        }
        return best;
    }

    VkExtent2D API::choose_extent(const VkSurfaceCapabilitiesKHR &capabilities) {
        if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
            return capabilities.currentExtent;
        } else {
            int width, height;
            glfwGetFramebufferSize(Window::get_window(), &width, &height);
            VkExtent2D extent = {
                    static_cast<uint32_t>(width),
                    static_cast<uint32_t>(height)
            };
            extent.width = std::max(capabilities.minImageExtent.width,
                                    std::min(capabilities.maxImageExtent.width, extent.width));
            extent.height = std::max(capabilities.minImageExtent.height,
                                     std::min(capabilities.maxImageExtent.height, extent.height));
            return extent;
        }
    }

    VkDevice API::device() {
        return m_device;
    }

    VkViewport API::viewport() {
        return m_viewport;
    }

    VkRect2D& API::scissor() {
        return m_scissor;
    }

    bool API::check_device_extension_support(VkPhysicalDevice device) {
        const std::vector<const char *> device_exts = {
                VK_KHR_SWAPCHAIN_EXTENSION_NAME,
        };

        uint32_t ext_count;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &ext_count, nullptr);

        std::vector<VkExtensionProperties> available_exts(ext_count);
        vkEnumerateDeviceExtensionProperties(device, nullptr, &ext_count, available_exts.data());

        std::set<std::string> required_exts(device_exts.begin(), device_exts.end());

        for (const auto &extension : available_exts) {
            required_exts.erase(extension.extensionName);
        }

        return required_exts.empty();
    }

#ifdef API_DEBUG
    VkDebugUtilsMessengerEXT API::callback;

    VkResult API::CreateDebugUtilsMessengerEXT(VkInstance instance,
                                               const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
                                               const VkAllocationCallbacks *pAllocator,
                                               VkDebugUtilsMessengerEXT *pCallback) {
        auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance,
                                                                               "vkCreateDebugUtilsMessengerEXT");
        if (func != nullptr) {
            return func(instance, pCreateInfo, pAllocator, pCallback);
        }
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }

#endif
}