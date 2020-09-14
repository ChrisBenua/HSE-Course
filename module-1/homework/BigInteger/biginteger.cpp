#include "biginteger.h"
#include <iostream>
#include <cassert>

const BigInteger BigInteger::ZERO = BigInteger(0);
const BigInteger BigInteger::ONE = BigInteger(1);


BigInteger::BigInteger() {
    sign_ = 0;
    this->num_ = {};
}

void BigInteger::clear() {
    this->sign_ = 0;
    this->num_.clear();
}

BigInteger::BigInteger(int number): BigInteger((int64_t) number) {}

BigInteger::BigInteger(int64_t number) {
    num_.clear();
    if (number == 0) {
        sign_ = 0;
    } else {
        if (number < 0) {
            sign_ = -1;
            number *= -1;
        } else {
            sign_ = 1;
        }

        while (number) {
            num_.emplace_back(number % BASE);
            number /= BASE;
        }
    }
    this->crop();
}

BigInteger::BigInteger(const std::string &s) {
    sign_ = 1;
    int number_start = 0;
    for (int i = 0; i < s.size(); ++i) {
        char digit = s[i];
        if (digit == '-' || digit == '+') {
            if (i) {
                throw std::invalid_argument("String contains multiple sign characters");
            }
            number_start = i + 1;
            sign_ = digit == '-' ? -1 : 1;
        }
    }
    if (number_start >= s.size()) {
        return;
    }

    int string_size = s.size();
    while (true) {
        if (string_size <= number_start) {
            break;
        }
        int batch_length = 0;
        int number_part = 0;
        int prefix = 1;
        for (int i = string_size - 1; i >= number_start && i >= string_size - BASE_LENGTH; --i) {
            number_part += (static_cast<int>(s[i] - '0')) * prefix;
            prefix *= 10;
            ++batch_length;
        }
        num_.emplace_back(number_part);
        string_size -= batch_length;
    }
    this->crop();
}

std::istream& operator>>(std::istream& in, BigInteger& number) {
    std::string s;
    in >> s;
    number = s;
    return in;
}


std::ostream& operator<<(std::ostream& out, const BigInteger& number) {
    out << number.toString();
    return out;
}

void BigInteger::crop() {
    while (!num_.empty() && num_.back() == 0) {
        this->num_.pop_back();
    }
    if (this->num_.empty()) {
        this->sign_ = 0;
    }
}
std::string BigInteger::int_to_string_helper(int number, bool pad_with_zeros) {
    std::string s;
    while (number != 0) {
        s += static_cast<char>(number % 10) + '0';
        number /= 10;
    }

    for (int i = 0; i < s.size() / 2; ++i) {
        std::swap(s[i], s[s.size() - 1 - i]);
    }
    return (pad_with_zeros ? std::string(BASE_LENGTH - s.size(), '0') : "") + s;
}

std::string BigInteger::toString() const {
    if (sign_ == 0) {
        return "0";
    }
    std::string res;
    if (sign_ == -1) {
        res += '-';
    }
    for (size_t i = num_.size() - 1; i > 0; i--) {
        res += int_to_string_helper(num_[i], i != num_.size() - 1);
    }
    res += int_to_string_helper(num_[0], num_.size() != 1);
    return res;
}

BigInteger& BigInteger::operator-() {
    return *(new BigInteger(static_cast<signed char>(sign_ * -1), num_));
}

BigInteger::BigInteger(signed char sign, const std::vector<int>& nums) {
    sign_ = sign;
    num_ = nums;
}

BigInteger& BigInteger::operator=(int number) {
    *this = BigInteger(number);
    return *this;
}

BigInteger& BigInteger::operator=(const std::string &s) {
    *this = BigInteger(s);
    return *this;
}

BigInteger operator+(const BigInteger& lhs, const BigInteger& rhs) {
    BigInteger res(lhs);
    res += rhs;
    return res;
}

BigInteger& BigInteger::operator+=(const BigInteger& other) {
    BigInteger res;
    if (sign_ == 0) {
        res = other;
    } else if (other.sign_ == 0) {
        return *this;
    } else if (sign_ == other.sign_) {
        res.sign_ = sign_;
        size_t self_size = num_.size();
        size_t operand_size = other.num_.size();
        int carry = 0;
        for (size_t i = 0; i < std::max(self_size, operand_size); ++i) {
            int sum = 0;
            if (i < self_size) {
                sum += num_[i];
            }
            if (i < operand_size) {
                sum += other.num_[i];
            }
            sum += carry;
            carry = sum / BASE;
            res.num_.emplace_back(sum % BASE);
        }
        while (carry != 0) {
            res.num_.emplace_back(carry / BASE);
            carry /= BASE;
        }
    } else {
        if (sign_ == 1) {
            res = *this - BigInteger::abs(other);
        } else {
            res = other - BigInteger::abs(*this);
        }
    }
    res.crop();
    *this = res;
    return *this;
}

BigInteger operator-(const BigInteger& lhs, const BigInteger& rhs) {
    BigInteger res(lhs);
    res -= rhs;
    return res;
}

BigInteger& BigInteger::operator-=(const BigInteger& other) {
    BigInteger res;
    if (sign_ == 0) {
        *this = BigInteger(static_cast<signed char>(other.sign_ * (-1)), other.num_);
    } else if (sign_ == other.sign_ && sign_ > 0) {
        if (*this >= other) {
            int carry = 0;
            for (int i = 0; i < this->num_.size() || carry; ++i) {
                num_[i] -= carry + (i < other.num_.size() ? other.num_[i] : 0);
                if (num_[i] < 0) {
                    num_[i] += BASE;
                    carry++;
                } else {
                    carry = 0;
                }
            }
        } else {
            *this = other - *this;
            this->sign_ *= (signed char)-1;
        }
    } else if (this->sign_ == 1 && other.sign_ == -1) {
        *this = *this + BigInteger::abs(other);
    } else if (this->sign_ == -1 && other.sign_ == 1) {
        *this = BigInteger::abs(*this) + other;
        this->sign_ *= (signed char)-1;
    } else if (this->sign_ == other.sign_ && this->sign_ == -1) {
        *this = BigInteger::abs(other) - BigInteger::abs(*this);
    }
    this->crop();
    return *this;
}

BigInteger operator*(const BigInteger& lhs, const BigInteger& rhs) {
    BigInteger temp(lhs);
    temp *= rhs;
    return temp;
}

BigInteger& BigInteger::operator*=(const BigInteger& other) {
    this->sign_ = static_cast<signed char>(other.sign_ * this->sign_);
    if (sign_ == 0) {
        *this = ZERO;
        return *this;
    }
    std::vector<int64_t> parts(num_.size() + other.num_.size());
    for (size_t self_index = 0; self_index < num_.size(); ++self_index) {
        for (size_t other_index = 0; other_index < other.num_.size(); ++other_index) {
            parts[self_index + other_index] += static_cast<int64_t>(num_[self_index]) * static_cast<int64_t>(other.num_[other_index]);
        }
    }
    int64_t carry = 0;
    for (size_t i = 0; i < parts.size(); ++i) {
        parts[i] += carry;
        carry = parts[i] / BASE;
        parts[i] %= BASE;
    }
    parts.push_back(carry);
    this->num_ = std::vector<int>(parts.size());
    for (size_t i = 0; i < parts.size(); ++i) {
        this->num_[i] = (int)parts[i];
    }
    this->crop();
    return *this;
}

bool operator>=(const BigInteger& lhs, const BigInteger& rhs) {
    return lhs == rhs || lhs > rhs;
}

bool operator>(const BigInteger& lhs, const BigInteger& rhs) {
    if (lhs.sign_ == rhs.sign_) {
        if (lhs.sign_ == 0) {
            return false;
        }
        if (lhs.num_.size() != rhs.num_.size()) {
            if (lhs.sign_ == 1) {
                return (lhs.num_.size() > rhs.num_.size());
            } else {
                return (lhs.num_.size() < rhs.num_.size());
            }
        }
        for (size_t i = lhs.num_.size() - 1; i >= 0; i--) {
            if (lhs.num_[i] != rhs.num_[i]) {
                if (lhs.sign_ == 1) {
                    return (lhs.num_[i] > rhs.num_[i]);
                } else {
                    return (lhs.num_[i] < rhs.num_[i]);
                }
            }
        }
    }
    return lhs.sign_ > rhs.sign_;
}

bool operator<(const BigInteger& lhs, const BigInteger& rhs) {
    return !(lhs >= rhs);
}

bool operator<=(const BigInteger& lhs, const BigInteger& rhs) {
    return !(lhs > rhs);
}

bool operator==(const BigInteger& lhs, const BigInteger& rhs) {
    bool isOk = lhs.sign_ == rhs.sign_ && lhs.num_.size() == rhs.num_.size();

    for (int i = 0; i < lhs.num_.size() && isOk; ++i) {
        isOk &= lhs.num_[i] == rhs.num_[i];
    }

    return isOk;
}


BigInteger BigInteger::abs(const BigInteger& number) {
    return BigInteger(number.sign_ < 0 ? static_cast<signed char>(1) : number.sign_, number.num_);
}

BigInteger::operator bool() const {
    return sign_ != 0;
}

BigInteger& BigInteger::operator++() {
    if (sign_ == 0) {
        this->num_.push_back(1);
        this->sign_ = 1;
    } else if (sign_ == 1) {
        int i = 0;
        for (i = 0; i < num_.size() && num_[i] == BASE - 1; ++i) {}
        if (i < num_.size()) {
            num_[i]++;
        } else {
            num_.push_back(1);
        }
        for (int j = 0; j < i; ++j) {
            num_[i] = 0;
        }
    } else { //sign == -1
        sign_ = 1;
        --(*this);
        sign_ = -1;
    }
    crop();
    return *this;
}

BigInteger& BigInteger::operator--() {
    if (sign_ == 0) {
        this->num_.push_back(1);
        this->sign_ = 1;
    } else if (sign_ == 1) {
        int i = 0;
        for (i = 0; i < num_.size() && num_[i] == 0; ++i) {}
        if (i < num_.size()) {
            num_[i]--;
        } else {
            assert(false);
        }
        for (int j = 0; j < i; ++j) {
            num_[i] = BASE - 1;
        }
    } else { //sign == -1
        sign_ = 1;
        ++(*this);
        sign_ = -1;
    }
    crop();
    return *this;
}

BigInteger BigInteger::operator++(int) {
    BigInteger copy = *this;
    ++(*this);
    return copy;
}

BigInteger BigInteger::operator--(int) {
    BigInteger copy = *this;
    --(*this);
    return copy;
}
int64_t BigInteger::binarySearchDivision(const BigInteger& divisible, const BigInteger& divider) {
    if (divisible.num_.size() == 1 && divider.num_.size() == 1) {
        return static_cast<int64_t>(divisible.num_[0] / divider.num_[0]);
    }
    if (divisible.num_.size() == 2 && divider.num_.size() == 1) {
        return (static_cast<int64_t>(divisible.num_[0]) + static_cast<int64_t>(divisible.num_[1] * BASE)) / static_cast<int64_t>(divider.num_[0]);
    }
    int l = 0, r = BASE;
    while (r - l > 1) {
        int mid_int = (r + l) / 2;
        BigInteger mid_big_int = BigInteger(mid_int);
        BigInteger _res = divider * mid_big_int;
        if (_res == divisible) {
            return mid_int;
        }
        if (_res < divisible) {
            l = mid_int;
        } else {
            r = mid_int;
        }
    }
    return static_cast<int64_t>(l);
}
std::pair<BigInteger, BigInteger> BigInteger::division(const BigInteger& divisible, const BigInteger& divider) {
    if (divider.sign_ == 0) {
        return {ZERO, ZERO};
    }
    if (divisible.num_.size() < divider.num_.size()) {
        return {ZERO, divisible};
    }
    if (divisible.num_.size() == 1) {
        return {BigInteger((divisible.sign_ * divisible.num_[0]) / divider.num_[0]), BigInteger((divisible.sign_ * divisible.num_[0]) % divider.num_[0])};
    }
    BigInteger quotionent, remainder;
    quotionent.sign_ = static_cast<signed char>(divisible.sign_ * divider.sign_);
    BigInteger TEN(BASE);
    BigInteger d = BigInteger::abs(divider);
    for (size_t i = divisible.num_.size(); i >= 1; i--) {
        remainder *= TEN;

        if (remainder.num_.empty()) {
            remainder.num_.emplace_back(0);
            remainder.sign_ = 1;
        }
        remainder.num_[0] = divisible.num_[i - 1];
        int64_t cnt = BigInteger::binarySearchDivision(remainder, d);
        remainder -= d * BigInteger(cnt);
        while (quotionent.num_.size() < i) {
            quotionent.num_.emplace_back(0);
        }
        quotionent.num_[i - 1] = cnt;
    }
    remainder.sign_ = divisible.sign_;
    quotionent.crop();
    remainder.crop();
    return {quotionent, remainder};
}

BigInteger operator/(const BigInteger& lhs, const BigInteger& rhs) {
    BigInteger res(lhs);
    res /= rhs;
    return res;
}

BigInteger& BigInteger::operator/=(const BigInteger& other) {
    *this = division(*this, other).first;
    return *this;
}

BigInteger operator%(const BigInteger& lhs, const BigInteger& rhs) {
    BigInteger res(lhs);
    res %= rhs;
    return res;
}

BigInteger& BigInteger::operator%=(const BigInteger& other) {
    *this = division(*this, other).second;
    return *this;
}
bool operator!=(const BigInteger& lhs, const BigInteger& rhs) {
    return !(lhs == rhs);
}


BigInteger& BigInteger::operator=(const BigInteger &other) = default;


