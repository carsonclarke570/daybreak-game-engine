#ifndef _DESCRIPTOR_SET_H_
#define _DESCRIPTOR_SET_H_

#include "Common.h"

namespace daybreak {

    class DescriptorSet {
    private:
        VkDescriptorSet m_set;
        VkPipeline m_pipeline;
    public:
        DescriptorSet(VkPipeline pipelin);
    };
}

#endif
