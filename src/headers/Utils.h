#pragma once

#include<cinttypes>
#include<chrono>

namespace utils {
    static const uint32_t getCurrentTime() { 
        return std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()
        ).count();
    };
}