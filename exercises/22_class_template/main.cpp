#include "../exercise.h"
#include <cstring>
// READ: 类模板 <https://zh.cppreference.com/w/cpp/language/class_template>

#include <iostream>
#include <stdexcept>

template<class T>
struct Tensor4D {
    unsigned int shape[4];
    T *data;

    Tensor4D(unsigned int const shape_[4], T const *data_) {
        unsigned int size = 1;
        for (auto i = 0; i < 4; ++i) {
            shape[i] = shape_[i];
            size *= shape[i];
        }
        data = new T[size];
        std::memcpy(data, data_, size * sizeof(T));
    }

    ~Tensor4D() {
        delete[] data;
    }

    Tensor4D(Tensor4D const &) = delete;
    Tensor4D(Tensor4D &&) noexcept = delete;

Tensor4D &operator+=(Tensor4D const &other) {
    // 检查形状兼容性
    for (int i = 0; i < 4; ++i) {
        if (other.shape[i] != 1 && other.shape[i] != shape[i]) {
            throw std::invalid_argument("Shape mismatch for broadcasting");
        }
    }
    
    // 直接使用取模运算的魔法！
    for (unsigned int idx = 0; idx < shape[0] * shape[1] * shape[2] * shape[3]; ++idx) {
        // 将扁平索引分解为4维索引
        unsigned int temp = idx;
        unsigned int l = temp % shape[3];
        temp /= shape[3];
        unsigned int k = temp % shape[2];
        temp /= shape[2];
        unsigned int j = temp % shape[1];
        temp /= shape[1];
        unsigned int i = temp;
        
        // 计算other的对应索引（使用取模实现广播）
        unsigned int idx_other = 
            (i % other.shape[0]) * (other.shape[1] * other.shape[2] * other.shape[3]) +
            (j % other.shape[1]) * (other.shape[2] * other.shape[3]) +
            (k % other.shape[2]) * other.shape[3] +
            (l % other.shape[3]);
        
        data[idx] += other.data[idx_other];
    }
    
    return *this;
}
};


// ---- 不要修改以下代码 ----
int main(int argc, char **argv) {
    {
        unsigned int shape[]{1, 2, 3, 4};
        // clang-format off
        int data[]{
             1,  2,  3,  4,
             5,  6,  7,  8,
             9, 10, 11, 12,

            13, 14, 15, 16,
            17, 18, 19, 20,
            21, 22, 23, 24};
        // clang-format on
        auto t0 = Tensor4D(shape, data);
        auto t1 = Tensor4D(shape, data);
        t0 += t1;
        for (auto i = 0u; i < sizeof(data) / sizeof(*data); ++i) {
            ASSERT(t0.data[i] == data[i] * 2, "Tensor doubled by plus its self.");
        }
    }
    {
        unsigned int s0[]{1, 2, 3, 4};
        // clang-format off
        float d0[]{
            1, 1, 1, 1,
            2, 2, 2, 2,
            3, 3, 3, 3,

            4, 4, 4, 4,
            5, 5, 5, 5,
            6, 6, 6, 6};
        // clang-format on
        unsigned int s1[]{1, 2, 3, 1};
        // clang-format off
        float d1[]{
            6,
            5,
            4,

            3,
            2,
            1};
        // clang-format on

        auto t0 = Tensor4D(s0, d0);
        auto t1 = Tensor4D(s1, d1);
        t0 += t1;
        for (auto i = 0u; i < sizeof(d0) / sizeof(*d0); ++i) {
            ASSERT(t0.data[i] == 7.f, "Every element of t0 should be 7 after adding t1 to it.");
        }
    }
    {
        unsigned int s0[]{1, 2, 3, 4};
        // clang-format off
        double d0[]{
             1,  2,  3,  4,
             5,  6,  7,  8,
             9, 10, 11, 12,

            13, 14, 15, 16,
            17, 18, 19, 20,
            21, 22, 23, 24};
        // clang-format on
        unsigned int s1[]{1, 1, 1, 1};
        double d1[]{1};

        auto t0 = Tensor4D(s0, d0);
        auto t1 = Tensor4D(s1, d1);
        t0 += t1;
        for (auto i = 0u; i < sizeof(d0) / sizeof(*d0); ++i) {
            ASSERT(t0.data[i] == d0[i] + 1, "Every element of t0 should be incremented by 1 after adding t1 to it.");
        }
    }
}
