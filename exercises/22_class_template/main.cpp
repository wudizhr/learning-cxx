#include "../exercise.h"
#include <cstring>
// READ: 类模板 <https://zh.cppreference.com/w/cpp/language/class_template>

template<class T>
struct Tensor4D {
    unsigned int shape[4];
    T *data;

    Tensor4D(unsigned int const shape_[4], T const *data_) {
        unsigned int size = 1;
        // TODO: 填入正确的 shape 并计算 size
        std::memcpy(shape, shape_, 4 * sizeof(unsigned int));
        size = shape[0]*shape[1]*shape[2]*shape[3];
        data = new T[size];
        std::memcpy(data, data_, size * sizeof(T));
    }
    ~Tensor4D() {
        delete[] data;
    }

    // 为了保持简单，禁止复制和移动
    Tensor4D(Tensor4D const &) = delete;
    Tensor4D(Tensor4D &&) noexcept = delete;

    // 这个加法需要支持“单向广播”。
    // 具体来说，`others` 可以具有与 `this` 不同的形状，形状不同的维度长度必须为 1。
    // `others` 长度为 1 但 `this` 长度不为 1 的维度将发生广播计算。
    // 例如，`this` 形状为 `[1, 2, 3, 4]`，`others` 形状为 `[1, 2, 1, 4]`，
    // 则 `this` 与 `others` 相加时，3 个形状为 `[1, 2, 1, 4]` 的子张量各自与 `others` 对应项相加。
    Tensor4D &operator+=(Tensor4D const &others) {
        // TODO: 实现单向广播的加法
        if(this != &others)
        {
            int diffshape[4];
            int pre[4];
            int opre[4];
            pre[3] = 1;
            opre[3] = 1;
            for(int i=2;i>=0;i--)
            {
                pre[i] = pre[i+1]*shape[i+1];
            }
            for(int i=2;i>=0;i--)
            {
                opre[i] = opre[i+1]*others.shape[i+1];
            }
            for(int i=0;i<4;i++)
            {
                diffshape[i] = shape[i] - others.shape[i];
            }
            for(int i=0;i<shape[0];i++)
            {
                for(int j=0;j<shape[1];j++)
                {
                    for(int k=0;k<shape[2];k++)
                    {
                        for(int l=0;l<shape[3];l++)
                        {
                            int idx = i*pre[0] + j*pre[1] + k*pre[2] + l*pre[3] ;
                            int oidx = 0;
                            std::cout << idx << std::endl;
                            for(int p=0;p<4;p++)
                            {
                                switch (p)
                                {
                                case 0:
                                    /* code */
                                    if(diffshape[p]==0)
                                        oidx += i*opre[0];
                                    break;
                                case 1:
                                    /* code */
                                    if(diffshape[p]==0)
                                        oidx += j*opre[1];
                                    break;
                                case 2:
                                    /* code */
                                    if(diffshape[p]==0)
                                        oidx += k*opre[2];
                                    break;
                                case 3:
                                    /* code */
                                    if(diffshape[p]==0)
                                        oidx += l*opre[3];
                                    break;
                                default:
                                    break;
                                }
                            }
                            //std::cout << oidx << std::endl;
                            
                            data[idx] = data[idx] + others.data[oidx];
                        }
                    }
                }
            }
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
            //std::cout << t0.data[i] << std::endl;
            ASSERT(t0.data[i] == data[i] * 2, "Tensor doubled by plus its self.");
        }
    }
    {
        std::cout << "case2!" << std::endl;
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
            //std::cout << t0.data[i] << std::endl;
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
