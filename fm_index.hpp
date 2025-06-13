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
    static constexpr size_t npos = std::numeric_limits<size_t>::max();

    explicit FMIndex(std::string, const size_t = 1);
    
    size_t getC(const char) const noexcept;
    size_t getOcc(const char, const size_t) const noexcept;
    size_t getSampleToText(const size_t) const noexcept;
    size_t lfMapping(const size_t) const noexcept;

    [[nodiscard]] size_t count(std::string_view) const noexcept;
    [[nodiscard]] std::vector<size_t> locate(std::string_view) const noexcept;

private:
    size_t sampleRate;
    size_t textSize;
    std::vector<size_t> sampleToText;
    std::string bwt;
    std::unordered_map<char, size_t> c;
    std::unordered_map<char, std::vector<size_t>> occ;

    void build(std::string&&);
    std::optional<std::pair<size_t, size_t>> getMatchRange(std::string_view) const noexcept;
};