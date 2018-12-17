#include "../../include/Allocator.h"

namespace daybreak {

        Allocator::Allocator(size_t size) :
            m_used(0), m_size(size), m_start(new uint8_t[size]) {
        }

        Allocator::Allocator(const Allocator& other) :
                m_used(other.m_used), m_size(other.m_size), m_start(new uint8_t[other.m_size]){
            std::copy(other.m_start, other.m_start + other.m_size, m_start);
        }

        Allocator::Allocator(Allocator&& other) noexcept :
            m_used(0), m_size(0), m_start(nullptr) {
            m_used = other.m_used;
            m_size = other.m_size;
            m_start = other.m_start;

            other.m_start = nullptr;
            other.m_used = 0;
            other.m_size = 0;
        }

        Allocator& Allocator::operator=(const Allocator& other) {
            if (this != &other) {
                delete[] m_start;

                m_used = other.m_used;
                m_size = other.m_size;
                m_start = new uint8_t[m_size];
                std::copy(other.m_start, other.m_start + other.m_size, m_start);
            }
            return *this;
        }

        Allocator& Allocator::operator=(Allocator&& other) noexcept {
            if (this != &other) {
                delete[] m_start;

                m_used = other.m_used;
                m_size = other.m_size;
                m_start = other.m_start;

                other.m_start = nullptr;
                other.m_used = 0;
                other.m_size = 0;
            }
            return *this;
        }

        Allocator::~Allocator() {
            free(m_start);
        }
}