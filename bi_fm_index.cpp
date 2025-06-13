#include "bi_fm_index.hpp"

BiFMIndex::BiFMIndex(std::string s)
    : textSize(s.size() + 1), charset(std::set(s.begin(), s.end())), fwdIndex(s), revIndex(std::string(s.rbegin(), s.rend())) {}

BiFMIndex::State BiFMIndex::initState() const noexcept {
    return State{{0, textSize}, {0, textSize}, 0};
}

void BiFMIndex::extendLeft(State& state, char key) const noexcept {
    auto& [fwdL, fwdR] = state.fwdRange;
    auto& [revL, revR] = state.revRange;
    for (const auto& c : this->charset) {
        if (c >= key) break;
        revL += this->fwdIndex.getOcc(c, fwdR - 1) - this->fwdIndex.getOcc(c, fwdL - 1);
    }
    fwdL = this->fwdIndex.getC(key) + (fwdL > 0 ? this->fwdIndex.getOcc(key, fwdL - 1) : 0);
    fwdR = this->fwdIndex.getC(key) + (fwdR > 0 ? this->fwdIndex.getOcc(key, fwdR - 1) : 0);
    state.len = fwdR - fwdL;
    revR = revL + state.len;
}

void BiFMIndex::extendRight(State& state, char key) const noexcept {
    auto& [revL, revR] = state.revRange;
    auto& [fwdL, fwdR] = state.fwdRange;
    for (const auto& c : this->charset) {
        if (c >= key) break;
        fwdL += this->revIndex.getOcc(c, revR - 1) - this->revIndex.getOcc(c, revL - 1);
    }
    revL = this->revIndex.getC(key) + (revL > 0 ? this->revIndex.getOcc(key, revL - 1) : 0);
    revR = this->revIndex.getC(key) + (revR > 0 ? this->revIndex.getOcc(key, revR - 1) : 0);
    state.len = revR - revL;
    fwdR = fwdL + state.len;
}


size_t BiFMIndex::count(const State& state) const noexcept {
    return state.fwdRange.second - state.fwdRange.first;
}

std::vector<size_t> BiFMIndex::locate(const State& state) const noexcept {
    std::vector<size_t> result;
    auto [l, r] = state.fwdRange;
    result.reserve(r - l);
    for (size_t i = r; i-- > l;) {
        size_t pos = i, steps = 0;
        while (fwdIndex.getSampleToText(pos) == FMIndex::npos) {
            pos = fwdIndex.lfMapping(pos);
            ++steps;
        }
        result.emplace_back(fwdIndex.getSampleToText(pos) + steps);
    }
    return result;
}