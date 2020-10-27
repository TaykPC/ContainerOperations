#include <iostream>
#include <iomanip>

#include "demo.h"


int main() {
    try {
        Demo::run1(); // Sequenzoperationen
        Demo::run2(); // Mengen und Maps
        Demo::run3(); // Wachstum von std::vector
    }
    catch (std::exception & e) {
        std::cerr << "EXCEPTION: " << e.what() << std::endl;
    }

    std::cin.ignore();
}
