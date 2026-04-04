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

/**
 * @brief Expresses an interval on an index
 *
 * Both first and last are included in the range of the interval.
 */
struct Interval {
    const Index& origin;
    const int first;
    const int last;
};

/**
 * @brief Convenience aliases for setting index size
 */
enum IndexSizes {LATIN = 3, GREEK = 4};

/**
 * @brief Represents an index of a tensor
 */
class Index {
public:
/**
 * @brief Initialise an index
 *
 * @param size Index size; must be greater than 1.
 */
///@{
    explicit Index(int size);
    explicit Index(std::string  name, int size);
///@}

/**
 * @brief Create a new index from an interval
 */
    Index(const Interval& interval); // NOLINT - we want implicit conversion

/**
 * @brief Compare two indices
 *
 * Note that for indices the name is included in the comparison.
 */
    bool operator==(const Index& other) const;

    [[nodiscard]] const int& size() const;
    [[nodiscard]] const int& id() const;
    [[nodiscard]] std::string name() const;
    void set_name(const std::string& name);

/**
 * @brief Use when slicing a tensor to select an interval of an index
 *
 * e.g. T[ index(1) ] will get a slice ignoring the 0th elements of T.
 *
 * @param first The first value to include; must be greater than 0.
 * @param last The last index to include; takes the maximum index value by default.
 */
///@{
    [[nodiscard]] Interval operator()(int first, int last=impl::MAX_INTERVAL) const;
    [[nodiscard]] Interval interval(int first, int last=impl::MAX_INTERVAL) const;
///@}

private:
    int m_size;
    int m_id;
    std::string m_name{};
    int m_interval_start{-1};

    inline static int s_next_id{0};
};

} // namespace varitensor

#endif // VARITENSOR_INDEX_H
