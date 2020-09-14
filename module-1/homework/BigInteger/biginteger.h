#ifndef BIGINTEGER__BIGINTEGER_H_
#define BIGINTEGER__BIGINTEGER_H_

#include <vector>
#include <string>


class BigInteger {
public:
    BigInteger();
    BigInteger(int);
    BigInteger(int64_t);
    explicit BigInteger(const std::string&);

    BigInteger& operator++();
    BigInteger& operator--();

    BigInteger operator++(int);
    BigInteger operator--(int);

    BigInteger& operator-();

    BigInteger& operator=(const BigInteger&);
    BigInteger& operator=(int);
    BigInteger& operator=(const std::string&);

    friend std::istream& operator>>(std::istream&, BigInteger&);
    friend std::ostream& operator<<(std::ostream&, const BigInteger&);

    friend BigInteger operator+(const BigInteger&, const BigInteger&);
    friend BigInteger operator-(const BigInteger&, const BigInteger&);
    friend BigInteger operator*(const BigInteger&, const BigInteger&);
    friend BigInteger operator/(const BigInteger&, const BigInteger&);
    friend BigInteger operator%(const BigInteger&, const BigInteger&);

    BigInteger& operator+=(const BigInteger&);
    BigInteger& operator-=(const BigInteger&);
    BigInteger& operator*=(const BigInteger&);
    BigInteger& operator/=(const BigInteger&);
    BigInteger& operator%=(const BigInteger&);

    friend bool operator>(const BigInteger&, const BigInteger&);
    friend bool operator>=(const BigInteger&, const BigInteger&);
    friend bool operator<(const BigInteger&, const BigInteger&);
    friend bool operator<=(const BigInteger&, const BigInteger&);

    friend bool operator==(const BigInteger&, const BigInteger&);
    friend bool operator!=(const BigInteger&, const BigInteger&);

    operator bool() const;

    void clear();

    static BigInteger abs(const BigInteger &);

    std::string toString() const;

    static const BigInteger ZERO;
    static const BigInteger ONE;

private:
    std::vector<int> num_;
    signed char sign_;

    explicit BigInteger(signed char sign, const std::vector<int>& nums);

    void crop();

    static std::pair<BigInteger, BigInteger> division(const BigInteger&, const BigInteger&);
    static int64_t binarySearchDivision(const BigInteger&, const BigInteger&);

    static std::string int_to_string_helper(int number, bool pad_with_zeroes);

    static const int BASE = 10000000;
    static const int BASE_LENGTH = 7;
};

#endif //BIGINTEGER__BIGINTEGER_H_
