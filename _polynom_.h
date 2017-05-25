//
//  Polynom.h
//  B
//
//  Created by Shepelev Alexey on 20.12.16.
//  Copyright © 2016 AShepelev. All rights reserved.
//

#ifndef Polynom_h
#define Polynom_h

#include <string>
#include <iostream>


template <typename value_type>
class Equal {
public:
    bool operator () (const value_type& a, const value_type& b) const {
        return a == b;
    }
};

template <typename value_type>
value_type pow(value_type x, int degree) {
    value_type ans = 1;
    for (int i = 0; i < degree; ++i) {
        ans *= x;
    }
    return ans;
}

template <typename value_type>
void swap_ (value_type& a, value_type& b) {
    value_type copy = a;
    a = b;
    b = copy;
}

template <typename value_type, typename Comp = Equal<value_type>>
class Polynomial {
    
    value_type* coef;
    int polDegree;
    
    void cleanZero() {
        for (int i = degree(); i > 0; --i) {
            if (coef[i] == value_type()) {
                --polDegree;
            } else {
                break;
            }
        }
        value_type* copy = new value_type[degree() + 1];
        for (int i = 0; i <= degree(); ++i) {
            copy[i] = coef[i];
        }
        delete [] coef;
        coef = copy;
    }
    
    Comp comp;
    
public:
    
    ~Polynomial () {
        delete [] coef;
    }
    
    Polynomial (value_type* coefArray, int number, Comp comparator = Equal<value_type>()) : comp(comparator) {
        polDegree = number - 1;
        coef = new value_type[number];
        for (int i = 0; i < number; ++i) {
            //coef[i] = coefArray[number - i - 1];
            coef[i] = coefArray[i];
        }
        cleanZero();
    }
    
    Polynomial (const Polynomial<value_type>& another, Comp comparator = Equal<value_type>()) : comp(comparator) {
        polDegree = another.degree();
        coef = new value_type[degree() + 1];
        for (int i = 0; i <= degree(); ++i) {
            coef[i] = another.coef[i];
        }
        comp = another.comp;
        cleanZero();
    }
    
    Polynomial& operator = (const Polynomial<value_type>& another) {
        polDegree = another.degree();
        delete [] coef;
        coef = new value_type[degree() + 1];
        for (int i = 0; i <= degree(); ++i) {
            coef[i] = another.coef[i];
        }
        comp = another.comp;
        return *this;
    }
    
    const value_type operator [] (int coefDegree) {
        return coef[coefDegree];
    }
    
    int degree () const {
        if (polDegree == 0 && coef[0] == value_type()) {
            return -1;
        }
        return polDegree;
    }
    
    std::string toString () const {
        std::string s = "";
        for (int i = polDegree; i >= 0; --i) {
            s += std::to_string(coef[i]) + " ";
        }
        return s;
    }
    
    Polynomial& operator += (const Polynomial<value_type>& another) {
        std::cout << degree() << " " <<  another.degree() << std::endl;
        if (degree() < another.degree()) {
            value_type * copy = new value_type[another.degree() + 1];
            for (int i = 0; i <= degree(); ++i) {
                copy[i] = coef[i];
            }
            for (int i = degree() + 1; i <= another.degree(); ++i) {
                copy[i] = value_type();
            }
            polDegree = another.degree();
            delete [] coef;
            coef = copy;
            
        }
        for (int i = 0; i <= another.degree(); ++i) {
            coef[i] += another.coef[i];
        }
        cleanZero();
        return *this;
    }
    
    const Polynomial operator - () const {
        Polynomial<value_type> copy = *this;
        for (int i = 0; i <= degree(); ++i) {
            copy.coef[i] = -copy.coef[i];
        }
        return copy;
    }
    
    Polynomial& operator -= (const Polynomial<value_type>& another) {
        return *this += -another;
    }
    
    const Polynomial operator - (const Polynomial<value_type>& another) const {
        Polynomial<value_type> copy = *this;
        copy -= another;
        return copy;
    }
    
    const Polynomial operator + (const Polynomial<value_type>& another) const {
        Polynomial<value_type> copy = *this;
        copy += another;
        return copy;
    }
    
    Polynomial<value_type>& operator *= (const Polynomial<value_type>& another) {
        value_type* copy = new value_type[degree() + another.degree() + 1];
        for (int i = 0; i <= degree() + another.degree(); ++i) {
            copy[i] = value_type();
        }
        for (int i = 0; i <= degree(); ++i) {
            for (int j = 0; j <= another.degree(); ++j) {
                copy[i + j] += coef[i] * another.coef[j];
            }
        }
        delete [] coef;
        coef = copy;
        polDegree = degree() + another.degree();
        return *this;
    }
    
    const Polynomial operator * (const Polynomial<value_type>& another) const {
        Polynomial<value_type> copy = *this;
        copy *= another;
        return copy;
    }
    
    Polynomial& operator /= (const Polynomial<value_type>& another) {
        if (another.degree() > degree()) {
            delete [] coef;
            coef = new value_type[1];
            coef[0] = 0;
            polDegree = 0;
            return *this;
        }
        int newDegree = degree() - another.degree();
        value_type* copy = new value_type[newDegree + 1];
        int y = newDegree;
        for (int i = degree(); i >= another.degree(); --i) {
            copy[y] = coef[i] / another.coef[another.degree()];
            for (int j = 0; j <= another.degree(); ++j) {
                set(i - j, get(i - j) - copy[y] * another.coef[another.degree() - j]);
            }
            --y;
        }
        delete [] coef;
        coef = copy;
        polDegree = newDegree;
        cleanZero();
        return *this;
    }
    
    const Polynomial operator / (const Polynomial<value_type>& another) const {
        Polynomial<value_type> copy = *this;
        copy /= another;
        return copy;
    }
    
    Polynomial& operator %= (const Polynomial<value_type>& another) {
        return *this -= (*this / another) * another;
    }
    
    const Polynomial operator % (const Polynomial<value_type>& another) const {
        return *this - (*this / another) * another;
    }
    
    bool operator == (const Polynomial<value_type>& another) const {
        if (degree() != another.degree()) {
            return false;
        }
        for (int i = 0; i <= degree(); ++i) {
            if (!comp(coef[i], another.coef[i])) {
                return false;
            }
        }
        return true;
    }
    
    bool operator != (const Polynomial<value_type>& another) const {
        return !(*this == another);
    }
    
    void set (int degree, const value_type& value) {
        coef[degree] = value;
    }
    
    const value_type get (int degree) const {
        return coef[degree];
    }
    
    const value_type operator () (const value_type& x) const {
        value_type ans = value_type();
        for (int i = 0; i <= degree(); ++i) {
            ans += coef[i] * pow(x, i);
        }
        return ans;
    }
    
    void swap (Polynomial<value_type>& another) {
        swap_(polDegree, another.polDegree);
        swap_(coef, another.coef);
    }
    
    Polynomial& operator += (const value_type& value) {
        for (int i = 0; i <= degree(); ++i) {
            coef[i] += value;
        }
        return *this;
    }
    
    Polynomial& operator -= (const value_type& value) {
        return *this += -value;
    }
    
    const Polynomial operator - (const value_type& value) const {
        Polynomial<value_type> copy = *this;
        copy -= value;
        return copy;
    }
    
    const Polynomial operator + (const value_type& value) const {
        Polynomial<value_type> copy = *this;
        copy += value;
        return copy;
    }
    
    Polynomial<value_type>& operator *= (const value_type& value) {
        for (int i = 0; i <= degree(); ++i) {
            coef[i] *= value;
        }
        return *this;
    }
    
    const Polynomial operator * (const value_type& value) const {
        Polynomial<value_type> copy = *this;
        copy *= value;
        return copy;
    }
    
    Polynomial& operator /= (const value_type& value) {
        for (int i = 0; i <= degree(); ++i) {
            coef[i] /= value;
        }
        return *this;
    }
    
    const Polynomial operator / (const value_type& value) const {
        Polynomial<value_type> copy = *this;
        copy /= value;
        return copy;
    }
};

template <typename value_type>
std::ostream& operator << (std::ostream& out, const Polynomial<value_type>& pol) {
    out << pol.toString();
    return out;
}

#endif /* Polynom_h */
