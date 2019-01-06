#ifndef biginteger_h
#define biginteger_h

#include <iostream>
#include <vector>
#include <string>

long long max (long long a, long long b) {
    if (a > b) {
        return a;
    } else {
        return b;
    }
}

long long min (long long a, long long b){
    if (a < b) {
        return a;
    } else {
        return b;
    }
}

class BigInteger {
    
private:
    
    short isNegative;
    std::vector <short> digits;
    
    void clearzero () {
        while (!digits.back() && (digits.end() - 1 != digits.begin())) {
            digits.pop_back();
        }
    }
    
    void regulation () {
        for (size_t i = 0; i < digits.size(); ++i) {
            digits[i + 1] += digits[i] / 10;
            digits[i] %= 10;
        }
    }
    
    const BigInteger half () {
        BigInteger ans = *this;
        for (unsigned long long i = 1; i < ans.digits.size(); ++i) {
            ans.digits[ans.digits.size() - i - 1] += 10*(ans.digits[ans.digits.size() - i] % 2);
            ans.digits[ans.digits.size() - i] /= 2;
        }
        ans.digits[0] /= 2;
        ans.clearzero();
        return ans;
    }
    
    const BigInteger binaryFind (const BigInteger& left, const BigInteger& right, const BigInteger& dividend, const BigInteger& divisor) {
        BigInteger middle = left;
        middle += right;
        middle = middle.half();
        BigInteger ans = middle;
        ans *= divisor;
        ans -= dividend;
        BigInteger ratio;
        if (ans.abs() < divisor) {
            return ans > 0 ? --middle : middle;
        }
        ratio = ans >= divisor ? binaryFind(left, middle, dividend, divisor) : binaryFind(++middle, right, dividend, divisor);
        return ratio;
    }
    
public:
    
    BigInteger () {
        isNegative = 0;
    }
    
    BigInteger (int i) {
        isNegative = i < 0 ? 1 : 0;
        i = std::abs(i);
        do {
            digits.push_back(i % 10);
            i /= 10;
        } while (i);
    }
    
    BigInteger (const BigInteger& bi) {
        isNegative = bi.isNegative;
        digits = bi.digits;
    }
    
    const std::string toString () const {
        std::string s = isNegative ? "-" : "";
        for (size_t i = 1; i <= digits.size(); ++i) s += digits[digits.size() - i] + '0';
        return s;
    }
    
    const BigInteger abs () const {
        BigInteger ans = *this;
        ans.isNegative = 0;
        return ans;
    }
    
    explicit operator bool () const {
        return bool(digits.back());
        
    }
    
    friend std::istream& operator >> (std::istream& in, BigInteger& bi);
    
    bool operator == (const BigInteger& bi) const {
        if (digits.size() != bi.digits.size() || isNegative != bi.isNegative) {
            return false;
        }
        for (size_t i = 0; i < digits.size(); ++i) {
            if (digits[i] != bi.digits[i]) {
                return false;
            }
        }
        return true;
    }
    
    bool operator != (const BigInteger& bi) const {
        bool ans = *this == bi;
        return !ans;
    }
    
    bool operator < (const BigInteger & bi) const {
        if (isNegative != bi.isNegative) {
            return isNegative > bi.isNegative;
        }
        if (digits.size() != bi.digits.size()) {
            return bool(isNegative) ^ (digits.size() < bi.digits.size()) ? true : false;
        }
        for (size_t i = 1; i < digits.size(); ++i){
            if (digits[digits.size() - i] < bi.digits[digits.size() - i]) {
                return !bool(isNegative);
            } else if (digits[digits.size() - i] > bi.digits[digits.size() - i]) {
                return bool(isNegative);
            }
        }
        return ((digits[0] < bi.digits[0]) ^ bool(isNegative)) && digits[0] != bi.digits[0];
    }
    
    bool operator <= (const BigInteger & bi2) const {
        return !(*this > bi2);
    }
    
    bool operator > (const BigInteger & bi2) const {
        return bi2 < *this;
    }
    
    bool operator >= (const BigInteger & bi2) const {
        return !(*this < bi2);
    }
    
    const BigInteger operator - () const {
        BigInteger ans = *this;
        if (bool(ans)) {
            ans.isNegative = bool(ans.isNegative) ? 0 : 1;
        }
        return ans;
    }
    
    BigInteger operator += (const BigInteger& bi) {
        if (!bool(bi)) {
            return *this;
        }
        if (isNegative != bi.isNegative) {
            return *this -= -bi;
        }
        if (this == &bi) {
            digits.resize(digits.size() + 1);
            for (size_t i = 0; i < digits.size(); ++i) {
                digits[i] += digits[i];
            }
            regulation();
            clearzero();
            return *this;
        }
        bi.digits.size() > digits.size() ? digits.resize(bi.digits.size() + 1) : digits.resize(digits.size() + 1);
        for (size_t i = 0; i < bi.digits.size(); ++i) {
            digits[i] += bi.digits[i];
        }
        regulation();
        clearzero();
        return *this;
    }
    
    BigInteger operator -= (const BigInteger& bi) {
        if (isNegative != bi.isNegative) {
            return *this += -bi;
        }
        if (abs() < bi.abs()) {
            digits.resize(bi.digits.size());
            isNegative = bool(isNegative) ? 0 : 1;
            for (size_t i = 0; i < digits.size(); ++i) {
                digits[i] = bi.digits[i] - digits[i];
                if (digits[i] < 0) {
                    digits[i] += 10;
                    ++digits[i+1];
                }
            }
        } else {
            for (size_t i = 0; i < digits.size(); ++i) {
                if (i < bi.digits.size()) {
                    digits[i] -= bi.digits[i];
                }
                if (digits[i] < 0) {
                    digits[i] += 10;
                    --digits[i+1];
                }
            }
        }
        clearzero();
        if (abs() == 0) {
            *this = abs();
        }
        return *this;
    }
    
    BigInteger& operator -- () {
        *this -= 1;
        return *this;
    }
    
    BigInteger operator -- (int) {
        BigInteger ans = *this;
        --*this;
        return ans;
    }
    
    BigInteger& operator ++ () {
        *this += 1;
        return *this;
    }
    
    BigInteger operator ++ (int) {
        BigInteger ans = *this;
        ++*this;
        return ans;
    }
    
    BigInteger operator *= (const BigInteger& bi) {
        if (*this == 0 || bi == 0) {
            *this = 0;
            return *this;
        }
        size_t size1 = digits.size(), size2 = bi.digits.size();
        BigInteger ans;
        ans.isNegative = bi.isNegative == isNegative ? 0 : 1;
        ans.digits.resize(size1 + size2 + 1);
        long long s = 0;
        for (unsigned long long j = 0; j < size1 + size2; ++j) {
            for (long long i = max(0, j - size1 + 1); i <= min(size2 - 1, j); ++i) {
                s += digits[j - i] * bi.digits[i];
            }
            ans.digits[j] = s % 10;
            s /= 10;
        }
        *this = ans;
        clearzero();
        return *this;
    }
    
    BigInteger operator /= (const BigInteger& bi) {
        if (*this == 0) {
            return *this;
        }
        if (abs() < bi.abs()) {
            *this = 0;
            return *this;
        }
        BigInteger ans = binaryFind(0, abs(), abs(), bi.abs());
        ans.isNegative = isNegative == bi.isNegative ? 0 : 1;
        *this = ans;
        return *this;
    }
    
    BigInteger operator %= (const BigInteger& bi) {
        BigInteger help = *this;
        *this /= bi;
        *this *= bi;
        help -= *this;
        *this = help;
        return *this;
    }
};

std::ostream& operator << (std::ostream& out, const BigInteger& bi) {
    out << bi.toString();
    return out;
}

std::istream& operator >> (std::istream& in, BigInteger& bi) {
    bi.digits.resize(0);
    bi.isNegative = 0;
    std::string s = "";
    in >> s;
    if (s[0] == '-') {
        bi.isNegative = 1;
        s.erase(s.begin());
    }
    for (size_t i = 1; i <= s.size(); ++i) {
        bi.digits.push_back(s[s.size() - i] - '0');
    }
    return in;
}

const BigInteger operator - (const BigInteger& bi1, const BigInteger& bi2) {
    BigInteger dif = bi1;
    dif -= bi2;
    return dif;
}

const BigInteger operator + (const BigInteger& bi1, const BigInteger& bi2) {
    BigInteger sum = bi1;
    sum += bi2;
    return sum;
}

const BigInteger operator * (const BigInteger& bi1, const BigInteger& bi2) {
    BigInteger ans = bi1;
    ans *= bi2;
    return ans;
}

const BigInteger operator / (const BigInteger& bi1, const BigInteger& bi2) {
    BigInteger ans = bi1;
    ans /= bi2;
    return ans;
}

const BigInteger operator % (const BigInteger& bi1, const BigInteger& bi2) {
    BigInteger ans = bi1;
    ans %= bi2;
    return ans;
}

#endif /* biginteger_h */


