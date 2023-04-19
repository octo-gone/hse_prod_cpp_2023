#include <gtest/gtest.h>
#include "inline_vector/inline_vector.hpp"

template <typename T>
class TestAllocator {
public:
    using value_type = T;

    TestAllocator() = default;
    ~TestAllocator() = default;

    T* allocate(std::size_t n) {
        if (n > std::numeric_limits<std::size_t>::max() / sizeof(T)) {
            throw std::bad_alloc();
        }
        return static_cast<T*>(::operator new(n * sizeof(T)));
    }

    void deallocate(T* p, std::size_t n) {
        for (std::size_t i = 0; i < n; ++i) {
            p[i].~T();
        }
        ::operator delete(p);
    }

    template<typename U>
    void destroy(U* p) {
        p->~U();
    }
};


TEST(InlinedVectorTest, InitList) {
    {
        InlineVector<int, 4> vec = {};


        ASSERT_EQ(vec.size(), 0);
        ASSERT_EQ(vec.capacity(), 4);
    }

    {
        InlineVector<int, 4> vec = {1, -2, 300};


        ASSERT_EQ(vec.size(), 3);
        ASSERT_EQ(vec.capacity(), 4);

        ASSERT_EQ(vec[0], 1);
        ASSERT_EQ(vec[1], -2);
        ASSERT_EQ(vec[2], 300);
    }

    {
        InlineVector<int, 4> vec = {1, -2, 300, 400000};


        ASSERT_EQ(vec.size(), 4);
        ASSERT_EQ(vec.capacity(), 4);

        ASSERT_EQ(vec[0], 1);
        ASSERT_EQ(vec[1], -2);
        ASSERT_EQ(vec[2], 300);
        ASSERT_EQ(vec[3], 400000);
    }

    {
        InlineVector<int, 4> vec = {1, -2, 300, 400000, 50, -60};


        ASSERT_EQ(vec.size(), 6);
        ASSERT_EQ(vec.capacity(), 8);

        ASSERT_EQ(vec[0], 1);
        ASSERT_EQ(vec[1], -2);
        ASSERT_EQ(vec[2], 300);
        ASSERT_EQ(vec[3], 400000);
        ASSERT_EQ(vec[4], 50);
        ASSERT_EQ(vec[5], -60);
    }
}

TEST(InlinedVectorTest, Copy) {
    {
        InlineVector<int, 4> vec = {1, -2, 300};
        InlineVector<int, 4> vec_copy_1(vec);
        InlineVector<int, 4> vec_copy_2 = vec;

        ASSERT_EQ(vec, vec_copy_1);
        ASSERT_EQ(vec, vec_copy_2);
        ASSERT_EQ(vec_copy_1, vec_copy_2);
    }

    {
        InlineVector<int, 4> vec = {1, -2, 300, 400000, 50, -60};
        InlineVector<int, 4> vec_copy_1(vec);
        InlineVector<int, 4> vec_copy_2 = vec;

        ASSERT_EQ(vec, vec_copy_1);
        ASSERT_EQ(vec, vec_copy_2);
        ASSERT_EQ(vec_copy_1, vec_copy_2);
    }
}

TEST(InlinedVectorTest, Empty) {
    {
        InlineVector<int, 4> vec = {1, -2, 300};

        ASSERT_FALSE(vec.empty());
    }

    {
        InlineVector<int, 4> vec = {};

        ASSERT_TRUE(vec.empty());
    }
}

TEST(InlinedVectorTest, OperatorBrackets) {
    {
        InlineVector<int, 4> vec = {1, -2, 300, 400000};
        InlineVector<int, 4> empty_vec = {};

        ASSERT_EQ(vec[0], 1);
        ASSERT_EQ(vec[1], -2);
        ASSERT_EQ(vec[2], 300);
        ASSERT_EQ(vec[3], 400000);
        ASSERT_THROW(vec[10], std::out_of_range);
        ASSERT_THROW(empty_vec[3], std::out_of_range);
    }

    {
        InlineVector<int, 4> vec = {1, -2, 300, 400000};
        vec[1] = 10;

        ASSERT_EQ(vec[0], 1);
        ASSERT_EQ(vec[1], 10);
        ASSERT_EQ(vec[2], 300);
        ASSERT_EQ(vec[3], 400000);
        ASSERT_THROW(vec[10] = 4, std::out_of_range);
    }

    {
        InlineVector<int, 4> vec = {1, -2, 300, 400000, -50, 60, 70};
        vec[5] = 6666;

        ASSERT_EQ(vec[0], 1);
        ASSERT_EQ(vec[1], -2);
        ASSERT_EQ(vec[2], 300);
        ASSERT_EQ(vec[3], 400000);
        ASSERT_EQ(vec[4], -50);
        ASSERT_EQ(vec[5], 6666);
        ASSERT_EQ(vec[6], 70);
        ASSERT_THROW(vec[10] = 4, std::out_of_range);
    }
}

TEST(InlinedVectorTest, Reserve) {
    {
        InlineVector<int, 4> vec = {};

        ASSERT_EQ(vec.size(), 0);
        ASSERT_EQ(vec.capacity(), 4);

        for (int i : { 1, 2, 3 })
            vec.push_back(i);

        ASSERT_EQ(vec.size(), 3);
        ASSERT_EQ(vec.capacity(), 4);

        for (int i : { 4, 5, 6 })
            vec.push_back(i);

        ASSERT_EQ(vec.size(), 6);
        ASSERT_EQ(vec.capacity(), 8);

        for (int i : { 7, 8, 9 })
            vec.push_back(i);

        ASSERT_EQ(vec.size(), 9);
        ASSERT_EQ(vec.capacity(), 16);

        for (int i : { 10, 11, 12, 13, 14 })
            vec.push_back(i);

        ASSERT_EQ(vec.size(), 14);
        ASSERT_EQ(vec.capacity(), 16);

        for (int i : { 15, 16, 17, 18, 19, 20 })
            vec.push_back(i);

        ASSERT_EQ(vec.size(), 20);
        ASSERT_EQ(vec.capacity(), 32);
    }
}

TEST(InlinedVectorTest, FrontBack) {
    {
        InlineVector<int, 4> vec = {};

        ASSERT_THROW(vec.front(), std::out_of_range);
        ASSERT_THROW(vec.back(), std::out_of_range);
    }

    {
        InlineVector<int, 4> vec = {1, -2, 300};

        ASSERT_EQ(vec.front(), 1);
        ASSERT_EQ(vec.back(), 300);
    }

    {
        InlineVector<int, 4> vec = {1, -2, 300, 400000, -50, 60, 777};

        ASSERT_EQ(vec.front(), 1);
        ASSERT_EQ(vec.back(), 777);
    }
}

TEST(InlinedVectorTest, PushBack) {
    {
        InlineVector<int, 4> vec;
        InlineVector<int, 4> ideal_vec = {1, 2, 3, 4};

        vec.push_back(1);
        vec.push_back(2);
        vec.push_back(3);
        vec.push_back(4);

        ASSERT_EQ(vec.size(), 4);
        ASSERT_EQ(vec.capacity(), 4);
        ASSERT_EQ(vec, ideal_vec);
    }

    {
        InlineVector<int, 4> vec = {1, 2, 3, 4};
        InlineVector<int, 4> ideal_vec = {1, 2, 3, 4, 5};

        vec.push_back(5);

        ASSERT_EQ(vec.size(), 5);
        ASSERT_EQ(vec.capacity(), 8);
        ASSERT_EQ(vec, ideal_vec);
    }
}

TEST(InlinedVectorTest, PopBack) {
    {
        InlineVector<int, 2> vec = {1, 2, 3, 4};
        InlineVector<int, 2> ideal_vec = {1, 2, 3};

        vec.pop_back();

        ASSERT_EQ(vec.size(), 3);
        ASSERT_EQ(vec.capacity(), 4);
        ASSERT_EQ(vec, ideal_vec);

        vec.pop_back();
        vec.pop_back();
        vec.pop_back();

        ASSERT_TRUE(vec.empty());
        ASSERT_THROW(vec.pop_back(), std::out_of_range);
    }

    {
        InlineVector<int, 4> vec = {1, 2, 3, 4};
        InlineVector<int, 4> ideal_vec = {1, 2};

        vec.pop_back();
        vec.pop_back();

        ASSERT_EQ(vec.size(), 2);
        ASSERT_EQ(vec.capacity(), 4);
        ASSERT_EQ(vec, ideal_vec);

        vec.pop_back();
        vec.pop_back();

        ASSERT_TRUE(vec.empty());
        ASSERT_THROW(vec.pop_back(), std::out_of_range);
    }

    {
        InlineVector<int, 4> vec = {1, 2, 3, 4, 5, 6, 7, 8};
        InlineVector<int, 4> ideal_vec_1 = {1, 2, 3, 4, 5, 6};
        InlineVector<int, 4> ideal_vec_2 = {1, 2};

        vec.pop_back();
        vec.pop_back();

        ASSERT_EQ(vec.size(), 6);
        ASSERT_EQ(vec.capacity(), 8);
        ASSERT_EQ(vec, ideal_vec_1);

        vec.pop_back();
        vec.pop_back();

        ASSERT_EQ(vec.size(), 4);
        ASSERT_EQ(vec.capacity(), 4);

        vec.pop_back();
        vec.pop_back();

        ASSERT_EQ(vec, ideal_vec_2);

        vec.pop_back();
        vec.pop_back();

        ASSERT_TRUE(vec.empty());
        ASSERT_THROW(vec.pop_back(), std::out_of_range);
    }
}

TEST(InlinedVectorTest, Insert) {
    {
        InlineVector<int, 4> vec;
        InlineVector<int, 4> ideal_vec_1 = {1, 3, 4, 2};
        InlineVector<int, 4> ideal_vec_2 = {1, -3, -4, 3, 4, 2};

        vec.insert(vec.begin(), 1);
        vec.insert(vec.begin() + 1, 2);
        vec.insert(vec.begin() + 1, 3);
        vec.insert(vec.begin() + 2, 4);

        ASSERT_EQ(vec.size(), 4);
        ASSERT_EQ(vec.capacity(), 4);
        ASSERT_EQ(vec, ideal_vec_1);

        vec.insert(vec.begin() + 1, -3);
        vec.insert(vec.begin() + 2, -4);

        ASSERT_EQ(vec.size(), 6);
        ASSERT_EQ(vec.capacity(), 8);
        ASSERT_EQ(vec, ideal_vec_2);
    }

    {
        InlineVector<int, 4> vec;
        InlineVector<int, 4> ideal_vec = {4, 3, 2, 1};

        vec.insert(vec.begin(), 1);
        vec.insert(vec.begin(), 2);
        vec.insert(vec.begin(), 3);
        vec.insert(vec.begin(), 4);

        ASSERT_EQ(vec.size(), 4);
        ASSERT_EQ(vec.capacity(), 4);
        ASSERT_EQ(vec, ideal_vec);
    }
}

TEST(InlinedVectorTest, Erase) {
    {
        InlineVector<int, 4> vec = {1, 3, 4, 2};
        InlineVector<int, 4> ideal_vec = {3, 2};

        vec.erase(vec.begin());
        vec.erase(vec.begin() + 1);

        ASSERT_EQ(vec.size(), 2);
        ASSERT_EQ(vec.capacity(), 4);
        ASSERT_EQ(vec, ideal_vec);

        vec.clear();
        ASSERT_EQ(vec.size(), 0);
        ASSERT_EQ(vec.capacity(), 4);
        ASSERT_TRUE(vec.empty());
    }

    {
        InlineVector<int, 1> vec = {1};
        InlineVector<int, 1> ideal_vec = {};
        vec.erase(vec.begin());

        ASSERT_EQ(vec.size(), 0);
        ASSERT_EQ(vec.capacity(), 1);
        ASSERT_EQ(vec, ideal_vec);

        vec.clear();
        ASSERT_EQ(vec.size(), 0);
        ASSERT_EQ(vec.capacity(), 1);
        ASSERT_TRUE(vec.empty());
    }

    {
        InlineVector<int, 1> vec = {1, 2};
        InlineVector<int, 1> ideal_vec = {2};
        vec.erase(vec.begin());

        ASSERT_EQ(vec.size(), 1);
        ASSERT_EQ(vec.capacity(), 1);
        ASSERT_EQ(vec, ideal_vec);

        vec.clear();
        ASSERT_EQ(vec.size(), 0);
        ASSERT_EQ(vec.capacity(), 1);
        ASSERT_TRUE(vec.empty());
    }

    {
        InlineVector<int, 2> vec = {1, 2, 3};
        InlineVector<int, 2> ideal_vec = {2, 3};
        vec.erase(vec.begin());

        ASSERT_EQ(vec.size(), 2);
        ASSERT_EQ(vec.capacity(), 2);
        ASSERT_EQ(vec, ideal_vec);

        vec.clear();
        ASSERT_EQ(vec.size(), 0);
        ASSERT_EQ(vec.capacity(), 2);
        ASSERT_TRUE(vec.empty());
    }
}

TEST(InlinedVectorTest, CustomAllocator) {
    {
        InlineVector<int, 4, TestAllocator<int>> vec;
        InlineVector<int, 4, TestAllocator<int>> ideal_vec_1 = {1, 2, 3, 4};
        InlineVector<int, 4, TestAllocator<int>> ideal_vec_2 = {1, 2, 3, 4, 5};

        vec.push_back(1);
        vec.push_back(2);
        vec.push_back(3);
        vec.push_back(4);

        ASSERT_EQ(vec.size(), 4);
        ASSERT_EQ(vec.capacity(), 4);
        ASSERT_EQ(vec, ideal_vec_1);

        vec.push_back(5);

        ASSERT_EQ(vec.size(), 5);
        ASSERT_EQ(vec.capacity(), 8);
        ASSERT_EQ(vec, ideal_vec_2);
    }
}

TEST(InlinedVectorTest, Types) {
    {
        InlineVector<double, 4> vec;
        InlineVector<double, 4> ideal_vec_1 = {1.2, 3.2, 3.4444, -4.111};
        InlineVector<double, 4> ideal_vec_2 = {1.2, 3.2, 3.4444, -4.111, 0.5};

        vec.push_back(1.2);
        vec.push_back(3.2);
        vec.push_back(3.4444);
        vec.push_back(-4.111);

        ASSERT_EQ(vec.size(), 4);
        ASSERT_EQ(vec.capacity(), 4);
        ASSERT_EQ(vec, ideal_vec_1);

        vec.push_back(0.5);

        ASSERT_EQ(vec.size(), 5);
        ASSERT_EQ(vec.capacity(), 8);
        ASSERT_EQ(vec, ideal_vec_2);
    }

    {
        InlineVector<std::size_t, 4> vec;
        InlineVector<std::size_t, 4> ideal_vec_1 = {2, 0, 3, 4111};
        InlineVector<std::size_t, 4> ideal_vec_2 = {2, 0, 3, 4111, 5};

        vec.push_back(2);
        vec.push_back(0);
        vec.push_back(3);
        vec.push_back(4111);

        ASSERT_EQ(vec.size(), 4);
        ASSERT_EQ(vec.capacity(), 4);
        ASSERT_EQ(vec, ideal_vec_1);

        vec.push_back(5);

        ASSERT_EQ(vec.size(), 5);
        ASSERT_EQ(vec.capacity(), 8);
        ASSERT_EQ(vec, ideal_vec_2);
    }

    {
        InlineVector<std::array<int, 3>, 2> vec;
        InlineVector<std::array<int, 3>, 2> ideal_vec_1 = {{0, 1, 2}, {3, 4, 5}};
        InlineVector<std::array<int, 3>, 2> ideal_vec_2 = {{0, 1, 2}, {3, 4, 5}, {6, 7, 8}};
        vec.push_back({0, 1, 2});
        vec.push_back({3, 4, 5});

        ASSERT_EQ(vec.size(), 2);
        ASSERT_EQ(vec.capacity(), 2);
        ASSERT_EQ(vec, ideal_vec_1);

        vec.push_back({6, 7, 8});

        ASSERT_EQ(vec.size(), 3);
        ASSERT_EQ(vec.capacity(), 4);
        ASSERT_EQ(vec, ideal_vec_2);
    }
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}