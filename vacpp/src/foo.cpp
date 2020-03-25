#include <iostream>

extern "C"
void testcall(float value) {
    std::cout << "Hello world" << value << "\n";
}
