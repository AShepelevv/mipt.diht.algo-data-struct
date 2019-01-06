//
// Created by Shepelev Alexey on 20.05.17.
//

#include <iostream>
#include <string>
#include <tuple>
#include <boost/type_index.hpp>

#include "tuple.h"

using boost::typeindex::type_id_with_cvr;


int foo(int i, int i1, int i2, int i3)
{
    std::cout << "foo: " << i << " " << i1 << " " << i2 << " " << i3 << " " << std::endl;
    return i;
}
void g(int i) {
    std::cout << i;
}



int main() {
    Tuple<int, int, int> t1(1, 2, 3);
    auto t2 = t1;
    //auto t3 = tupleCat(t1, t2);
    //g(unpack(foo, 99, t2));
    auto t3 = tupleCat(t1, t2, t2, t1, t2);
    std::cout << 0;
    //std::cout << test_int;
}
