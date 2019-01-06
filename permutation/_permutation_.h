//
//  permutation.h
//  Permutation
//
//  Created by Shepelev Alexey on 18.10.16.
//  Copyright © 2016 AShepelev. All rights reserved.
//

#ifndef permutation_h
#define permutation_h

#include <iostream>

void mergeSort(int* start, int* finish, int* buffer) {
    if (start + 1 >= finish) {
        return;
    }
    int* middle = start + (finish - start) / 2;
    mergeSort(start, middle, buffer);
    mergeSort(middle, finish, buffer);
    int * left = start;
    int * right = middle;
    for (int  i = 0; i < finish - start; ++i) {
        if (left < middle && right < finish) {
            buffer[i] = *left < *right ? *(left++) : *(right++);
        } else {
            buffer[i] = left >= middle ? *(right++) : *(left++);
        }
    }
    for (int  i = 0; i < finish - start; ++i) {
        start[i] = buffer[i];
    }
}

void swap (int & a, int & b) {
    int c = a;
    a = b;
    b = c;
}

void reverse (int* start, int* finish) {
    for (int i = 0; i < (finish - start) / 2; ++i) {
        swap(*(start + i), *(finish - i - 1));
    }
}

class Permutation {
    
private:
    
    int size;
    int * p;
    
public:
    
    ~ Permutation () {
        delete [] p;
    }
    
    Permutation () {
        size = 0;
        p = new int[size];
    }
    
    explicit Permutation (const unsigned int  i) {
        size = i;
        p = new int [size];
        for(int i = 0; i < size; ++i) {
            p[i] = i;
        }
    }
    
    Permutation (const unsigned int i, const int* arr) {
        size = i;
        p = new int [size];
        for (int i = 0; i < size; ++i) {
            p[i] = arr[i];
        }
    }
    
    Permutation (const Permutation& per) {
        size = per.size;
        p = new int [size];
        for (int  i = 0; i < size; ++i) {
            this->p[i] = per.p[i];
        }
    }
    
    Permutation& operator = (const Permutation& per) {
        if (this != &per) {
            delete[] p;
            p = new int[per.size];
            size = per.size;
            for (int i = 0; i < per.size; ++i) {
                p[i] = per.p[i];
            }
        }
        return *this;
    }
    
    const Permutation inverse () const {
        Permutation ans = Permutation(size);
        for (int i = 0; i < size; ++i) {
           for (int j = 0; j < size; ++j) {
               if(p[j] == i) {
                   ans.p[i] = j;
               }
           }
        }
        return ans;
    }
    /*const Permutation inverse () const {
        Permutation ans = Permutation(size);
        for (int  i = 0; i < size; ++i) {
            ans.p[p[i]] = i;
        }
        return ans;
    }*/
    
    Permutation& operator *= (const Permutation& per) {
        Permutation copy = *this;
        for (int  i = 0; i < size; ++i) {
            p[i] = copy.p[per.p[i]];
        }
        return *this;
    }
    
    bool operator < (const Permutation& per) const{
        if (size < per.size ) {
            return true;
        }
        for (int  i = 0; i < size; ++i) {
            if (p[i] > per.p[i]) {
                return false;
            } else if (p[i] < per.p[i]) {
                return true;
            }
        }
        return false;
    }
    
    bool operator == (const Permutation& per) const {
        if (size != per.size) {
            return false;
        }
        bool ans = true;
        for (int  i = 0; i < size; ++i) {
            if (p[i] != per.p[i]) {
                return false;
            }
        }
        return ans;
    }
    
    int operator [] (const int index) const {
        return p[index];
    }
    
    void operator () (int* arr) const {
        int* ans = new int[size];
        for (int i = 0; i < size; ++i)
            ans[p[i]] = arr[i];
        for (int i = 0; i < size; ++i)
            arr[i] = ans[i];
        delete[] ans;
    }

    
    const Permutation next () const {

        Permutation ans = *this;
        return ++ans;
    }
    
    const Permutation previous () const {

        Permutation ans = *this;
        return --ans;
    }
    
    Permutation& operator ++ () {
        int indexMin = 0;
        for (int i = size - 2; i >= 0; --i) {
            if (p[i] < p[i + 1]) {
                indexMin = i + 1;
                for (int j = i + 1; j < size; ++j) {
                    if (p[j] < p[indexMin] && p[j] > p[i]) {
                        indexMin = j;
                    }
                }
                swap(p[i], p[indexMin]);
                reverse(p + i + 1, p + size);
                return *this;
            }
            
        }
        reverse(p, p + size);
        return *this;
    }
    
    const Permutation operator ++ (int) {
        Permutation p = *this;
        ++(*this);
        return p;
    }
    
    Permutation& operator -- () {
        int indexMax = 0;
        for (int i = size - 2; i >= 0; --i) {
            if (p[i] > p[i + 1]) {
                indexMax = i + 1;
                for (int j = i + 1; j < size; ++j) {
                    if (p[j] > p[indexMax] && p[j] < p[i]) {
                        indexMax = j;
                    }
                }
                swap(p[i], p[indexMax]);
                reverse(p + i + 1, p + size);
                return *this;
            }
        }
        reverse(p, p + size);
        return *this;
    }
    
    const Permutation operator -- (int ) {
        Permutation ans = *this;
        --(*this);
        return ans;
    }
    
    const std::string toString () const {
        std::string ans = "";
        for (int i = 0; i < size; ++i) {
            ans += std::to_string(p[i]);
            if (i < size - 1) ans += " ";
        }
        return ans;
    }
};

const Permutation operator * (const Permutation& per1, const Permutation& per2) {
    Permutation ans = per1;
    ans *= per2;
    return ans;
}

bool operator > (const Permutation& per1, const Permutation& per2) {
    return per2 < per1;
}

bool operator != (const Permutation& per1, const Permutation& per2) {
    return !(per1 == per2);
}

bool operator <= (const Permutation& per1, const Permutation& per2) {
    return !(per1 > per2);
}

bool operator >= (const Permutation& per1, const Permutation& per2) {
    return !(per1 < per2);
  
}

std::ostream& operator << (std::ostream& out, const Permutation& per) {
    out << per.toString();
    return out;
}

#endif /* permutation_h */
