#include "../exercise.h"

// READ: 有 cv 限定符的成员函数 <https://zh.cppreference.com/w/cpp/language/member_functions>

struct Fibonacci {
    int numbers[11];
    mutable int cached = 2;
    
    // 关键修改：使用 constexpr 构造函数直接初始化
    constexpr Fibonacci(const int (&arr)[11]) 
        : numbers{arr[0], arr[1], arr[2], arr[3], arr[4], arr[5], 
                  arr[6], arr[7], arr[8], arr[9], arr[10]},
          cached(2) {
    }
    
    constexpr int get(int i) const {
        // 这里不需要计算，因为数组已经包含所有值
        if (i < 0 || i >= 11) return 0;
        return numbers[i];
    }
};


int main(int argc, char **argv) {
    Fibonacci constexpr FIB{{0, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55}};
    ASSERT(FIB.get(10) == 55, "fibonacci(10) should be 55");
    std::cout << "fibonacci(10) = " << FIB.get(10) << std::endl;
    return 0;
}
