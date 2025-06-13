#include "fm_index.hpp"

FMIndex::FMIndex(std::string s, const size_t sampleRate) : sampleRate(sampleRate) {
    this->build(std::move(s));
}

size_t FMIndex::getC(const char key) const noexcept {
    if (!this->c.contains(key)) return 0;
    return this->c.at(key);
}

size_t FMIndex::getOcc(const char key, const size_t index) const noexcept {
    if (!this->occ.contains(key)) return 0;
    return index < this->textSize ? this->occ.at(key)[index] : 0;
}

size_t FMIndex::getSampleToText(const size_t index) const noexcept {
    return this->sampleToText[index];
}

size_t FMIndex::lfMapping(const size_t index) const noexcept {
    char key = this->bwt[index];
    return this->getC(key) + this->getOcc(key, index) - 1;
}

size_t FMIndex::count(std::string_view pattern) const noexcept {
    const auto range = getMatchRange(pattern);
    return range ? range->second - range->first : 0;
}

std::vector<size_t> FMIndex::locate(std::string_view pattern) const noexcept {
    const auto range = getMatchRange(pattern);
    if (!range) return {};

    std::vector<size_t> result;
    result.reserve(range->second - range->first);

    for (size_t i = range->second; i-- > range->first;) {
        size_t pos = i, steps = 0;
        while (this->sampleToText[pos] == npos) {
            pos = lfMapping(pos);
            ++steps;
        }
        result.emplace_back(this->sampleToText[pos] + steps);
    }
    return result;
}

void FMIndex::build(std::string&& s) {
    auto sa = SAIS::buildSA(std::vector<char>(s.begin(), s.end()));
    s.push_back(0);
    this->textSize = s.size();
    this->sampleToText.assign(textSize, npos);
    this->bwt.resize(textSize);
    for (size_t i = 0; i < this->textSize; ++i) {
        size_t j = sa[i];
        this->bwt[i] = j > 0 ? s[j - 1] : 0;
        if (j % this->sampleRate == 0) {
            this->sampleToText[i] = j;
        }
    }

    std::set<char> charset = std::set(s.begin(), s.end());
    for (const auto& key : charset) {
        this->occ[key].assign(this->textSize, 0);
    }
    for (size_t i = 0; i < this->textSize; ++i) {
        for (auto& [key, vec] : this->occ) {
            vec[i] += (i > 0 ? vec[i - 1] : 0) + (key == bwt[i]);
        }
    }

    size_t sum = 0;
    for (const auto& key : charset) {
        this->c[key] = sum;
        sum += this->getOcc(key, this->textSize - 1);
    }
}

std::optional<std::pair<size_t, size_t>> FMIndex::getMatchRange(std::string_view pattern) const noexcept {
    size_t l = 0, r = this->textSize;
    for (const auto& key : pattern | std::views::reverse) {
        if (!this->c.contains(key)) return std::nullopt;
        l = this->getC(key) + (l > 0 ? this->getOcc(key, l - 1) : 0);
        r = this->getC(key) + (r > 0 ? this->getOcc(key, r - 1) : 0);
        if (l >= r) return std::nullopt;
    }
    return std::pair{l, r};
}