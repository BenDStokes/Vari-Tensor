/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * Licence, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef VARITENSOR_INDEX_H
#define VARITENSOR_INDEX_H

#include <string>

namespace varitensor {

class Index;

namespace impl {
constexpr int MAX_INTERVAL = -1;

} // namespace impl

struct Interval {
    const Index& origin;
    const int first;
    const int last;
};

// for nice initialisation of indices with standard sizes
enum IndexSizes {LATIN = 3, GREEK = 4};

class Index {
public:
    explicit Index(int size);
    explicit Index(std::string  name, int size);
    Index(const Interval& interval); // NOLINT - we want implicit conversion

    bool operator==(const Index& other) const;

    [[nodiscard]] const int& size() const;
    [[nodiscard]] const int& id() const;
    [[nodiscard]] std::string name() const;
    void set_name(const std::string& name);


    [[nodiscard]] Interval operator()(int first, int last=impl::MAX_INTERVAL) const;
    [[nodiscard]] Interval interval(int first, int last=impl::MAX_INTERVAL) const;

private:
    int m_size;
    int m_id;
    std::string m_name{};
    int m_interval_start{-1};

    inline static int s_next_id{0};
};

} // namespace varitensor

#endif // VARITENSOR_INDEX_H
