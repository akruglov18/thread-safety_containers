#include <iostream>
#include "utils.h"

template<class T, class Enable = void>
class Vector {
public:
    void print() {
        std::cout << "Primary\n";
    }
};

template<class T>
class Vector<T, typename std::enable_if<is_atomic<T>::value>::type> {
public:
    void print() {
        std::cout << "Atomic\n";
    }
};

struct A {
};

int main() {
    Vector<bool> v_atom;
    Vector<A> v_o;
    v_atom.print();
    v_o.print();
}
