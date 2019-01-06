#include <iostream>
#include "node.h"

int main() {
    srand(time(NULL));
    ImplicitTreap<long long> t;
    std::size_t n = 0;

    std::ifstream fin;
    fin.open("input.txt");

    fin >> n;
    long long tmp = 0;
    for (size_t i = 0; i < n; ++i) {
        fin >> tmp;
        t.insert(tmp, i);
    }
    std::size_t q = 0;
    fin >> q;
    std::size_t type = 0;
    for (std::size_t i = 0; i < q; ++i) {
        fin >> type;
        switch (type) {
            case 1: {
                std::size_t l = 0, r = 0;
                fin >> l >> r;
                std::cout << t.getSum(l, r) << std::endl;
                break;
            }

            case 2: {
                long long x = 0;
                std::size_t pos = 0;
                fin >> x >> pos;
                t.insert(x, pos);
                break;
            }

            case 3: {
                std::size_t pos = 0;
                fin >> pos;
                t.remove(pos);
                break;
            }

            case 4: {
                long long x = 0;
                std::size_t l = 0, r = 0;
                fin >> x >> l >> r;
                t.assignValue(l, r, x);
                break;
            }

            case 5: {
                long long x = 0;
                std::size_t l = 0, r = 0;
                fin >> x >> l >> r;
                t.addValue(l, r, x);
                break;
            }

            case 6: {
                std::size_t l = 0, r = 0;
                fin >> l >> r;
                t.nextPermutationOfSubsegment(l, r);
                break;
            }

            case 7: {
                std::size_t l = 0, r = 0;
                fin >> l >> r;
                t.prevPermutationOfSubsegment(l, r);
                break;
            }

            default:
                break;
        }
    }
    t.print();
}