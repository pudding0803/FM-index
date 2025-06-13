#include <iostream>
#include "fm_index.hpp"
#include "bi_fm_index.hpp"

void printIndexResult(size_t, std::vector<size_t>);

int main() {
    std::string s = "mmiissiissiippii";
    std::cout << "Reference: " << s << std::endl << std::endl;

    FMIndex index(s);
    std::string_view pattern = "issii";
    std::cout << "Pattern for FM-index: " << pattern << std::endl;
    printIndexResult(index.count(pattern), index.locate(pattern));

    BiFMIndex biIndex(s);
    auto state = biIndex.initState();

    biIndex.extendRight(state, 's');    // s
    biIndex.extendLeft(state, 'i');     // is
    biIndex.extendRight(state, 's');    // iss
    biIndex.extendRight(state, 'i');    // issi
    biIndex.extendLeft(state, 'i');     // iissi
    biIndex.extendLeft(state, 's');     // siissi

    std::cout << "Pattern for bi-directional FM-index: " << "siissi" << std::endl;
    printIndexResult(biIndex.count(state), biIndex.locate(state));

    return 0;
}

void printIndexResult(size_t count, std::vector<size_t> positions) {
    std::cout << "Count: " << count << std::endl;
    std::cout << "Locate: [";
    for (const auto& pos : positions) {
        std::cout << ' ' << pos;
    }
    std::cout << " ]" << std::endl << std::endl;
}