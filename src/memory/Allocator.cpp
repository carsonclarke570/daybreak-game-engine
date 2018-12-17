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