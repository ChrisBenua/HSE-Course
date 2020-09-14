#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <string>
#include <sstream>
#include <vector>

#include "biginteger.h"
#include "gtest/gtest.h"

TEST(AssignmentFromInt, Test1) {
    int value = 42;
    BigInteger bigint_val;
    bigint_val = value;

    std::ostringstream oss;
    oss << bigint_val;
    ASSERT_EQ(oss.str(), std::to_string(value));
}

TEST(UnaryMinus, Test1) {
    int value = 42;
    BigInteger bigint_val = value;
    bigint_val = -bigint_val;

    std::ostringstream oss;
    oss << bigint_val;
    ASSERT_EQ(oss.str(), std::to_string(-value));
}

TEST(ToString, Test1) {
    int value = 42;
    BigInteger bigint_val = value;

    std::string testString = bigint_val.toString();
    ASSERT_EQ(testString, std::to_string(value));
}

TEST(ToString, Test1_1) {
    int64_t value = 4242424242424242;
    BigInteger bigint_val = value;

    std::string testString = bigint_val.toString();
    ASSERT_EQ(testString, std::to_string(value));
}

TEST(ToAssignment, Test1) {
    int val = 42;
    BigInteger bigint_val = val;
    bigint_val += bigint_val += bigint_val += bigint_val;
    val += val += val += val;

    std::ostringstream oss;
    oss << bigint_val;
    ASSERT_EQ(oss.str(), std::to_string(val));
}

TEST(ToAssignment, Test1_1) {
    int64_t val = 4242424242424242;
    BigInteger bigint_val = val;
    bigint_val += bigint_val += bigint_val += bigint_val;
    val += val += val += val;

    std::ostringstream oss;
    oss << bigint_val;
    ASSERT_EQ(oss.str(), std::to_string(val));
}

TEST(ToAssignment, Test1_2) {
    int64_t val = 4242424242424242;
    int64_t val2 = 424242;
    BigInteger bigint_val = val;
    BigInteger bigint_val2 = val2;
    bigint_val += bigint_val += bigint_val2 += bigint_val2;
    val += val += val2 += val2;

    std::ostringstream oss;
    oss << bigint_val;
    ASSERT_EQ(oss.str(), std::to_string(val));
    ASSERT_EQ(bigint_val2.toString(), std::to_string(val2));
}

TEST(Arithmetic, Test1) {
    int64_t a = 42;
    int64_t b = 431;
    BigInteger bigint_a = a;
    BigInteger bigint_b = b;

    ++bigint_a -= bigint_b++;
    ++a -= b++;

    std::ostringstream oss;
    oss << bigint_a;
    ASSERT_EQ(oss.str(), std::to_string(a));

    //TODO
//    ((++bigint_a) *= (bigint_a *= bigint_b));
//    ((++a) *= (a *= b));
    bigint_a *= (bigint_a *= bigint_b);
    a *= (a *= b);

    oss.str("");
    oss.clear();
    oss << bigint_a;
    ASSERT_EQ(oss.str(), std::to_string(a));

    bigint_a /= 42;
    a /= 42;
    oss.str("");
    oss.clear();
    oss << bigint_a;
    ASSERT_EQ(oss.str(), std::to_string(a));

    bigint_a %= 100;
    a %= 100;

    oss.str("");
    oss.clear();
    oss << bigint_a;
    ASSERT_EQ(oss.str(), std::to_string(a));
}

TEST(Arithmetic, Test2) {
    int64_t a = 42424242424242424;
    int64_t b = 12345678901234;
    BigInteger bigint_a = a;
    BigInteger bigint_b = b;

    ASSERT_EQ((bigint_a / bigint_b).toString(), std::to_string(a / b));
    ASSERT_EQ((bigint_a % bigint_b).toString(), std::to_string(a % b));
}

TEST(Arithmetic, Test3) {
    int64_t a = -42424242424242424;
    BigInteger bigint_a = a;

    ASSERT_EQ((BigInteger::abs(bigint_a)).toString(), std::to_string(abs(a)));
    ASSERT_EQ((BigInteger::abs(BigInteger::abs(bigint_a))).toString(), std::to_string(abs(a)));
}

TEST(TypeCast, Test1) {
    BigInteger bigint_val = 42;
    ASSERT_TRUE(bool(bigint_val));

    bigint_val = 0;
    ASSERT_FALSE(bool(bigint_val));
}

TEST(InStream, Test1) {
    int value = 42;
    BigInteger bigint_val = value;

    std::istringstream iss(std::to_string(value));
    std::ostringstream oss;
    iss >> bigint_val;
    oss << bigint_val;

    ASSERT_EQ(oss.str(), std::to_string(value));
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}