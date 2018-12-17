#ifndef _ALLOCATOR_H_
#define _ALLOCATOR_H_

#include "Common.h"

namespace daybreak {
    class Allocator {
    protected:
        uint8_t* m_start;
        size_t m_size;
        size_t m_used;
    public:
        explicit Allocator(size_t size);
        Allocator(const Allocator& other);
        Allocator(Allocator&& other) noexcept;
        Allocator& operator=(const Allocator& other);
        Allocator& operator=(Allocator&& other) noexcept;
        ~Allocator();

        virtual void* allocate(size_t size, uint8_t alignment) = 0;
        virtual void release(void* ptr) = 0;
        virtual void reset() = 0;

        inline size_t size() { return m_size; }
        inline size_t used() { return m_used; }
    };
}

#endif