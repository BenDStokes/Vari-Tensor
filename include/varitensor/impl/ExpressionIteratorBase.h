/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * Licence, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef VARITENSOR_EXPRESSION_ITERATOR_H
#define VARITENSOR_EXPRESSION_ITERATOR_H

#include "common.h"

namespace varitensor::impl {

struct Preparatory;

/**
 * @brief Increments the positions vector in accordance with the dimensions vector
 *
 * The iterator provided will be told which index to increment, or reset if a dimension is overflown. Returns true if
 * the increment was successful, false if the end of the dimensions has been reached.
 */
template <ExpressionIterator_c E>
bool increment_positions( // static to avoid having to use virtual functions
    std::vector<int>& positions,
    const std::vector<Dimension>& dimensions,
    const E& iterator // not really const, but we have to pretend to maintain the constness of the * operators
) {
    for (size_t i=0; i<positions.size(); ++i) {
        // check if we're about to overflow the next index
        if (positions[i] + 1 == dimensions[i].index.size()) [[unlikely]] {
            // if so, reset it and move on to the next one
            positions[i] = 0;
            iterator.reset(dimensions[i].index.id());
            continue;
        }

        // once we've found an index we can increment, do so
        ++positions[i];
        iterator.increment(dimensions[i].index.id());

        return true;
    }

    // note that if we reach the end, all the indices will have been reset to 0
    return false;
}

/**
 * @brief Class to reduce code duplication in expression iterators
 *
 * Virtual functions have been omitted for performance.
 */
class ExpressionIteratorBase {
public:
    using iterator_category = std::forward_iterator_tag;
    using difference_type   = std::ptrdiff_t;
    using value_type        = double;

    ExpressionIteratorBase() = default;
    explicit ExpressionIteratorBase(Preparatory& preparatory);

    [[nodiscard]] std::vector<int>& positions();
    [[nodiscard]] int positions(int index) const;
    [[nodiscard]] int positions(const Index& index) const;
    [[nodiscard]] const Dimensions& dimensions() const;
    [[nodiscard]] size_t size() const;
    [[nodiscard]] bool is_scalar() const;

protected:
    friend class Tensor;

    size_t m_size{1};
    Dimensions m_dimensions;
    std::vector<int> m_positions;
};

} // namespace varitensor::impl

#endif // VARITENSOR_EXPRESSION_ITERATOR_H
