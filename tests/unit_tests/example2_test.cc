#include "gtest/gtest.h"

#include <queue>

double sqrt(double num)
{
    if (num < 0.0)
    {
        std::cerr << "Error: Negative Input\n";
        exit(1);
    }
    return 0.0f;
}

TEST(Testname, testcae)
{
    // ASSERT_* stops right away
    // EXPECT_* continues, can see more error

    // Equal
    EXPECT_EQ(1, 1);
    EXPECT_FLOAT_EQ(0.0f, 0.0f);
    EXPECT_DOUBLE_EQ(0.0, 0.0);

    // Different
    int val1, val2;
    EXPECT_EQ(val1, val2); // val1 == val2
    EXPECT_NE(val1, val2); // val1 != val2
    EXPECT_LT(val1, val2); // val1 < val2
    EXPECT_LE(val1, val2); // val1 <= val2
    EXPECT_GT(val1, val2); // val1 > val2
    EXPECT_GE(val1, val2); // val1 >= val2

    // String
    const char* str1, *str2;
    std::string stdstr1, stdstr2;
    EXPECT_STREQ(str1,str2);     //the two C strings have the same content
    EXPECT_STRNE(str1,str2);     //the two C strings have different contents
    EXPECT_STRCASEEQ(str1,str2); //the two C strings have the same content, ignoring case
    EXPECT_STRCASENE(str1,str2); //the two C strings have different contents, ignoring case
    EXPECT_EQ(stdstr1, stdstr2); //the two C++ string have the same content

    // Return status & error message
    ASSERT_EXIT(sqrt(-22.0),
                ::testing::ExitedWithCode(1),
                "Error: Negative Input");

}

// Test class with setup/teardown
class QueueTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        q1_.emplace(1);
        q2_.emplace(2);
        q2_.emplace(3);
    }

    void TearDown() override
    {

    }

    std::queue<int> q0_;
    std::queue<int> q1_;
    std::queue<int> q2_;
};

TEST_F(QueueTest, DequeueWorks) {
  int* n = nullptr; //q0_.pop();
  EXPECT_EQ(n, nullptr);
}

// To start tests
int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}