#pragma once

#include <string>
#include <vector>
#include <set>
#include <unordered_map>
#include <optional>
#include "sais.hpp"

class FMIndex {
public:
    FMIndex() = default;

    void build(std::string&);
    [[nodiscard]] size_t count(std::string_view) const noexcept;
    [[nodiscard]] std::vector<size_t> locate(std::string_view) const noexcept;

private:
    size_t size;
    std::string bwt;
    std::unordered_map<char, int> c;
    std::unordered_map<char, std::vector<int>> occ;

    std::optional<std::pair<size_t, size_t>> matchRange(std::string_view) const noexcept;
    size_t lf(size_t) const;
};