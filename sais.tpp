template <typename T>
std::vector<size_t> SAIS::buildSA(std::vector<T> v) {
    v.emplace_back(0);
    const size_t size = v.size();

    std::vector<Type> types(size);
    types[size - 1] = Type::S;
    std::vector<size_t> lmsArr;
    for (size_t i = size - 1; i-- > 0;) {
        if (v[i] < v[i + 1]) types[i] = Type::S;
        else if (v[i] > v[i + 1]) types[i] = Type::L;
        else types[i] = types[i + 1];
    }
    for (size_t i = 1; i < size; ++i) {
        if (types[i] == Type::S && types[i - 1] == Type::L) {
            types[i] = Type::LMS;
            lmsArr.emplace_back(i);
        }
    }
    size_t lmsNum = lmsArr.size();

    std::map<T, size_t> freq;
    for (const auto& c : v) {
        ++freq[c];
    }
    std::map<T, Bucket> buckets;

    std::vector<size_t> sa(size, -1);
    SAIS::induceSA(v, types, lmsArr, buckets, freq, sa);

    std::vector<size_t> indices(size, -1);
    for (size_t i = 0; i < lmsNum; ++i) {
        indices[lmsArr[i]] = i;
    }

    std::vector<size_t> ranks(lmsNum, -1);
    size_t rank = 0;
    size_t prev = size;
    for (size_t i = 0; i < size; ++i) {
        if (types[sa[i]] == Type::LMS) {
            if (!SAIS::isSameSubstring(v, types, prev, sa[i])) ++rank;
            ranks[indices[sa[i]]] = rank;
            prev = sa[i];
        }
    }

    if (rank != lmsNum) {
        auto result = SAIS::buildSA(ranks);
        std::vector<size_t> sortedLMS(lmsNum);
        for (size_t i = 0; i < lmsNum; ++i) {
            sortedLMS[i] = lmsArr[result[i + 1]];
        }
        lmsArr = std::move(sortedLMS);
    }
    SAIS::induceSA(v, types, lmsArr, buckets, freq, sa);
    return sa;
}

template <typename T>
bool SAIS::isSameSubstring(
    const std::vector<T>& v,
    const std::vector<Type>& types,
    size_t i,
    size_t j
) noexcept {
    if (i == j) return true;
    const size_t size = v.size();
    while (i < size && j < size) {
        if (v[i++] != v[j++]) return false;

        bool lms1 = types[i] == Type::LMS;
        bool lms2 = types[j] == Type::LMS;
        if (lms1 && lms2) return true;
        if (lms1 != lms2) return false;
    }
    return false;
}

template <typename T>
void SAIS::resetBuckets(
    std::map<T, Bucket>& buckets,
    const std::map<T, size_t>& freq
) noexcept {
    size_t acc = 0;
    for (const auto& [c, count] : freq) {
        buckets[c].begin = acc;
        acc += count;
        buckets[c].end = acc;
    }
}

template <typename T>
void SAIS::induceSA(
    const std::vector<T>& v,
    const std::vector<Type>& types,
    const std::vector<size_t>& lmsArr,
    std::map<T, Bucket>& buckets,
    const std::map<T, size_t>& freq,
    std::vector<size_t>& sa
) noexcept {
    constexpr size_t npos = std::numeric_limits<size_t>::max();

    const size_t size = v.size(), lmsNum = lmsArr.size();
    SAIS::resetBuckets(buckets, freq);
    std::ranges::fill(sa, -1);
    for (size_t i = lmsNum; i-- > 0;) {
        size_t j = lmsArr[i];
        sa[--buckets[v[j]].end] = j;
    }
    for (size_t i = 0; i < size; ++i) {
        size_t j = sa[i] - 1;
        if (j != npos && sa[i] != -1 && types[j] == Type::L) {
            sa[buckets[v[j]].begin++] = j;
        }
    }
    for (size_t i = 0; i < size; ++i) {
        size_t j = sa[i];
        if (j != npos && types[j] != Type::L && types[j - 1] == Type::L) {
            ++buckets[v[j]].end;
        }
    }
    for (size_t i = size; i-- > 0;) {
        size_t j = sa[i] - 1;
        if (j != npos && sa[i] != -1 && types[j] != Type::L) {
            sa[--buckets[v[j]].end] = j;
        }
    }
}