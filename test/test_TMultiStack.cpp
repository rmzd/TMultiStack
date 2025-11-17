#include <gtest.h>
#include "TMultiStack.h"

TEST(TMultiStack, can_create_multistack_with_positive_size) {
    ASSERT_NO_THROW(TMultiStack<int> ms(20, 3));
}

TEST(TMultiStack, throws_when_create_multistack_with_zero_stack_count) {
    ASSERT_ANY_THROW(TMultiStack<int> ms(10, 0));
}

TEST(TMultiStack, fresh_stacks_are_empty) {
    TMultiStack<int> ms(10, 3);
    for (int i = 0; i < 3; i++) {
        EXPECT_TRUE(ms.IsEmpty(i));
    }
}

TEST(TMultiStack, can_push_and_pop_elements) {
    TMultiStack<int> ms(10, 2);
    ms.Push(0, 42);
    ms.Push(1, 24);
    EXPECT_FALSE(ms.IsEmpty(0));
    EXPECT_FALSE(ms.IsEmpty(1));
    EXPECT_EQ(ms.Pop(0), 42);
    EXPECT_EQ(ms.Pop(1), 24);
}

TEST(TMultiStack, pop_returns_correct_element) {
    TMultiStack<int> ms(10, 2);
    ms.Push(0, 100);
    ms.Push(1, 200);
    EXPECT_EQ(ms.Pop(0), 100);
    EXPECT_EQ(ms.Pop(1), 200);
}

TEST(TMultiStack, throws_when_push_to_invalid_stack_index) {
    TMultiStack<int> ms(10, 2);
    ASSERT_ANY_THROW(ms.Push(5, 42));
}

TEST(TMultiStack, throws_when_pop_from_invalid_stack_index) {
    TMultiStack<int> ms(10, 2);
    ASSERT_ANY_THROW(ms.Pop(5));
}

TEST(TMultiStack, throws_when_pop_from_empty_stack) {
    TMultiStack<int> ms(10, 2);
    ASSERT_ANY_THROW(ms.Pop(0));
}

TEST(TMultiStack, can_create_copied_multistack) {
    TMultiStack<int> ms(10, 2);
    ms.Push(0, 5);
    ASSERT_NO_THROW(TMultiStack<int> ms1(ms));
}

TEST(TMultiStack, copied_multistack_has_same_content) {
    TMultiStack<int> ms(10, 2);
    ms.Push(0, 5);
    ms.Push(1, 10);
    TMultiStack<int> ms2(ms);
    EXPECT_EQ(ms.Pop(0), ms2.Pop(0));
    EXPECT_EQ(ms.Pop(1), ms2.Pop(1));
}

TEST(TMultiStack, original_unaffected_by_copy_modification) {
    TMultiStack<int> source(10, 2);
    source.Push(0, 15);
    source.Push(1, 25);
    TMultiStack<int> replica(source);
    replica.Pop(0);
    EXPECT_FALSE(source.IsEmpty(0));
    EXPECT_TRUE(replica.IsEmpty(0));
}

TEST(TMultiStack, can_create_moved_multistack) {
    TMultiStack<int> ms1(10, 2);
    ms1.Push(0, 1);
    ASSERT_NO_THROW(TMultiStack<int> ms2(std::move(ms1)));
}

TEST(TMultiStack, moved_multistack_has_correct_state) {
    TMultiStack<int> ms1(10, 2);
    ms1.Push(0, 42);
    ms1.Push(1, 24);
    TMultiStack<int> ms2(std::move(ms1));
    EXPECT_EQ(ms2.Pop(0), 42);
    EXPECT_EQ(ms2.Pop(1), 24);
}

TEST(TMultiStack, stack_isolation_maintained) {
    TMultiStack<int> ms(15, 3);
    ms.Push(0, 10);
    ms.Push(1, 20);
    ms.Push(2, 30);
    ms.Pop(0);
    EXPECT_FALSE(ms.IsEmpty(1));
    EXPECT_FALSE(ms.IsEmpty(2));
    EXPECT_EQ(ms.Pop(1), 20);
    EXPECT_EQ(ms.Pop(2), 30);
}

TEST(TMultiStack, multiple_push_pop_operations) {
    TMultiStack<int> ms(20, 3);
    for (int stack = 0; stack < 3; stack++) {
        for (int i = 1; i <= 5; i++) {
            ms.Push(stack, stack * 10 + i);
        }
    }
    for (int stack = 2; stack >= 0; stack--) {
        for (int i = 5; i >= 1; i--) {
            EXPECT_EQ(ms.Pop(stack), stack * 10 + i);
        }
    }
}

TEST(TMultiStack, can_push_after_pop) {
    TMultiStack<int> ms(5, 2);
    ms.Push(0, 1);
    ms.Push(0, 2);
    ms.Pop(0);
    ASSERT_NO_THROW(ms.Push(0, 3));
    EXPECT_EQ(ms.Pop(0), 3);
    EXPECT_EQ(ms.Pop(0), 1);
}

TEST(TMultiStack, empty_stack_is_not_full) {
    TMultiStack<int> ms(10, 2);
    EXPECT_FALSE(ms.IsFull(0));
}

TEST(TMultiStack, full_stack_is_not_empty) {
    TMultiStack<int> ms(3, 2);
    ms.Push(0, 1);
    if (!ms.IsFull(0)) {
        ms.Push(0, 2);
    }
    if (!ms.IsEmpty(0)) {
        EXPECT_FALSE(ms.IsEmpty(0));
    }
}

TEST(TMultiStack, can_use_double_type) {
    TMultiStack<double> ms(10, 2);
    ms.Push(0, 3.14);
    ms.Push(1, 2.71);
    EXPECT_DOUBLE_EQ(ms.Pop(0), 3.14);
    EXPECT_DOUBLE_EQ(ms.Pop(1), 2.71);
}

TEST(TMultiStack, can_use_string_type) {
    TMultiStack<std::string> ms(10, 2);
    ms.Push(0, "hello");
    ms.Push(1, "world");
    EXPECT_EQ(ms.Pop(0), "hello");
    EXPECT_EQ(ms.Pop(1), "world");
}

TEST(TMultiStack, single_stack_operations) {
    TMultiStack<int> ms(5, 1);
    for (int i = 0; i < 5; i++) {
        ms.Push(0, i);
    }
    for (int i = 4; i >= 0; i--) {
        EXPECT_EQ(ms.Pop(0), i);
    }
}

TEST(TMultiStack, large_multistack_creation) {
    ASSERT_NO_THROW(TMultiStack<int> ms(1000, 10));
}

TEST(TMultiStack, repeated_operations_work_correctly) {
    TMultiStack<int> ms(15, 2);
    for (int cycle = 0; cycle < 3; cycle++) {
        for (int i = 0; i < 5; i++) {
            ms.Push(0, i);
            ms.Push(1, i * 10);
        }
        for (int i = 4; i >= 0; i--) {
            EXPECT_EQ(ms.Pop(0), i);
            EXPECT_EQ(ms.Pop(1), i * 10);
        }
        EXPECT_TRUE(ms.IsEmpty(0));
        EXPECT_TRUE(ms.IsEmpty(1));
    }
}

TEST(TMultiStack, mixed_operations_across_stacks) {
    TMultiStack<int> ms(20, 3);
    ms.Push(0, 1);
    ms.Push(1, 10);
    ms.Push(2, 100);
    ms.Push(0, 2);
    ms.Push(1, 20);
    ms.Pop(0);
    ms.Push(0, 3);
    ms.Push(1, 30);
    EXPECT_EQ(ms.Pop(0), 3);
    EXPECT_EQ(ms.Pop(0), 1);
    EXPECT_EQ(ms.Pop(1), 30);
    EXPECT_EQ(ms.Pop(1), 20);
    EXPECT_EQ(ms.Pop(1), 10);
    EXPECT_EQ(ms.Pop(2), 100);
}

TEST(TMultiStack, can_get_total_size) {
    TMultiStack<int> ms(15, 3);
    EXPECT_EQ(ms.GetLen(), 15);
}

TEST(TMultiStack, can_get_stack_count) {
    TMultiStack<int> ms(15, 3);
    EXPECT_EQ(ms.GetStackCount(), 3);
}

TEST(TMultiStack, can_access_stack_via_operator) {
    TMultiStack<int> ms(10, 2);
    ASSERT_NO_THROW(ms[0]);
    ASSERT_NO_THROW(ms[1]);
}

TEST(TMultiStack, throws_when_access_invalid_stack_via_operator) {
    TMultiStack<int> ms(10, 2);
    ASSERT_ANY_THROW(ms[5]);
}

TEST(TMultiStack, is_full_method_does_not_throw) {
    TMultiStack<int> ms(5, 2);
    EXPECT_NO_THROW(ms.IsFull(0));
    EXPECT_NO_THROW(ms.IsFull(1));
    ms.Push(0, 1);
    EXPECT_NO_THROW(ms.IsFull(0));
}

TEST(TMultiStack, stack_becomes_non_empty_after_push) {
    TMultiStack<int> ms(5, 2);
    EXPECT_TRUE(ms.IsEmpty(0));
    ms.Push(0, 42);
    EXPECT_FALSE(ms.IsEmpty(0));
}

TEST(TMultiStack, multiple_stacks_independent) {
    TMultiStack<int> ms(12, 4);
    ms.Push(0, 1);
    ms.Push(0, 2);
    ms.Push(1, 10);
    ms.Push(1, 20);
    ms.Push(1, 30);
    ms.Push(2, 100);
    EXPECT_EQ(ms.Pop(0), 2);
    EXPECT_EQ(ms.Pop(1), 30);
    EXPECT_EQ(ms.Pop(2), 100);
    EXPECT_EQ(ms.Pop(0), 1);
    EXPECT_EQ(ms.Pop(1), 20);
    EXPECT_EQ(ms.Pop(1), 10);
}

TEST(TMultiStack, memory_distribution_works) {
    TMultiStack<int> ms(9, 3);
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            ASSERT_NO_THROW(ms.Push(i, i * 10 + j));
        }
    }
    for (int i = 0; i < 3; i++) {
        EXPECT_FALSE(ms.IsEmpty(i));
    }
}

TEST(TMultiStack, comparison_operators_work) {
    TMultiStack<int> ms1(10, 2);
    TMultiStack<int> ms2(10, 2);

    ms1.Push(0, 1);
    ms1.Push(1, 2);
    ms2.Push(0, 1);
    ms2.Push(1, 2);

    EXPECT_TRUE(ms1 == ms2);
    EXPECT_FALSE(ms1 != ms2);

    ms2.Pop(0);
    EXPECT_FALSE(ms1 == ms2);
    EXPECT_TRUE(ms1 != ms2);
}

TEST(TMultiStack, assignment_operator_works) {
    TMultiStack<int> ms1(10, 2);
    ms1.Push(0, 42);
    ms1.Push(1, 24);

    TMultiStack<int> ms2(5, 1);
    ms2 = ms1;

    EXPECT_EQ(ms2.Pop(0), 42);
    EXPECT_EQ(ms2.Pop(1), 24);
}

TEST(TMultiStack, move_assignment_operator_works) {
    TMultiStack<int> ms1(10, 2);
    ms1.Push(0, 42);
    ms1.Push(1, 24);

    TMultiStack<int> ms2(5, 1);
    ms2 = std::move(ms1);

    EXPECT_EQ(ms2.Pop(0), 42);
    EXPECT_EQ(ms2.Pop(1), 24);
    EXPECT_EQ(ms1.GetLen(), 0);
}
