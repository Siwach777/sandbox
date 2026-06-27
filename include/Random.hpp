#pragma once
#include <random>

namespace Random {
    static thread_local std::mt19937 generator(std::random_device{}());

    inline float getFloat(float min, float max) {
        std::uniform_real_distribution<float> distribution(min, max);
        return distribution(generator);
    }
    inline int getInt(int min, int max) {
        std::uniform_int_distribution<int> distribution(min, max);
        return distribution(generator);
    }
}