#pragma once

#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <ranges>

class SAIS {
public:
    template <typename T>
    [[nodiscard]] static std::vector<size_t> buildSA(std::vector<T> v);

private:
    enum class Type : char {
        L, LMS, S
    };

    template <typename T>
    struct Bucket {
        T begin{};
        T end{};
    };

    template <typename T>
    inline static bool isSameSubstring(
        const std::vector<T>&,
        const std::vector<Type>&,
        size_t,
        size_t
    ) noexcept;

    template <typename T>
    inline static void resetBuckets(
        std::map<T, Bucket<T>>&,
        const std::map<T, size_t>&
    ) noexcept;

    template <typename T>
    static void induceSA(
        const std::vector<T>&,
        const std::vector<Type>&,
        const std::vector<size_t>&,
        std::map<T, Bucket<T>>&,
        const std::map<T, size_t>&,
        std::vector<size_t>&
    ) noexcept;
};

#include "sais.tpp"