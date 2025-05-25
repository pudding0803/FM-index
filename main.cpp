#include <iostream>
#include "index.hpp"

int main() {
    std::string s = "mmiissiissiippii";
    FMIndex index(s);

    std::string_view pattern = "issii";
    const auto count = index.count(pattern);
    const auto positions = index.locate(pattern);

    std::cout << "Count: " << count << std::endl;
    std::cout << "Locate: [";
    for (const auto& pos : positions) {
        std::cout << ' ' << pos;
    }
    std::cout << " ]" << std::endl;
    return 0;
}