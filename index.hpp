#pragma once

#include <string>
#include <vector>
#include <set>
#include <unordered_map>
#include <limits>
#include <optional>
#include "sais.hpp"

class FMIndex {
public:
    explicit FMIndex(std::string);
    
    [[nodiscard]] size_t count(std::string_view) const noexcept;
    [[nodiscard]] std::vector<size_t> locate(std::string_view) const noexcept;

private:
    static constexpr size_t npos = std::numeric_limits<size_t>::max();
    static constexpr size_t sampleRate = 16;

    size_t textSize;
    std::vector<size_t> sampleToText;
    std::vector<size_t> textToSample;
    std::string bwt;
    std::unordered_map<char, int> c;
    std::unordered_map<char, std::vector<int>> occ;

    void build(std::string&&);
    std::optional<std::pair<size_t, size_t>> getMatchRange(std::string_view) const noexcept;
    size_t lfMapping(size_t) const;
};