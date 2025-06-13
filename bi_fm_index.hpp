#pragma once

#include <string>
#include <vector>
#include <set>
#include <optional>
#include "fm_index.hpp"

class BiFMIndex {
public:
    struct State {
        std::pair<size_t, size_t> fwdRange;
        std::pair<size_t, size_t> revRange;
        size_t len{};
    };

    explicit BiFMIndex(std::string);

    [[nodiscard]] State initState() const noexcept;
    void extendLeft(State&, char) const noexcept;
    void extendRight(State&, char) const noexcept;

    [[nodiscard]] size_t count(const State&) const noexcept;
    [[nodiscard]] std::vector<size_t> locate(const State&) const noexcept;

private:
    size_t textSize;
    std::set<char> charset;
    FMIndex fwdIndex;
    FMIndex revIndex;
};