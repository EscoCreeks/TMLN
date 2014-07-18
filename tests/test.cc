#include <gtest/gtest.h>

TEST(DummyTest, DummyOK)
{
  ASSERT_TRUE(true);
}

TEST(DummyTest, DummyKO)
{
  ASSERT_TRUE(false);
}

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
