#include "stack.h"
#include <gtest/gtest.h>

TEST(Stack, stack_0) {
    std::size_t size = 10;
    Stack<int> st(size);
    st.push(3);
    st.push(2);
    ASSERT_EQ(st.top(), 2);
    ASSERT_NO_THROW(st.pop());
    ASSERT_EQ(st.top(), 3);
    ASSERT_NO_THROW(st.pop());
}