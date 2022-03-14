#include "queue.h"
#include <gtest/gtest.h>

TEST(Queue, queue_0) {
    std::size_t size = 10;
    Queue<int> q(size);
    q.push(3);
    q.push(2);
    ASSERT_EQ(q.front(), 3);
    ASSERT_NO_THROW(q.pop());
    ASSERT_EQ(q.front(), 2);
    ASSERT_NO_THROW(q.pop());
}