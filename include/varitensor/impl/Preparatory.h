/*
* This Source Code Form is subject to the terms of the Mozilla Public
 * Licence, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef VARITENSOR_PREPARATORY_H
#define VARITENSOR_PREPARATORY_H

#include "common.h"
#include "varitensor/Tensor.h"

namespace varitensor::impl {

enum PreparatoryType {
    LINKED,
    PRODUCT
};

/**
 * @brief Allows expressions to set up for resolution whilst simultaneously  working out whether we
 * can use a bulk algorithm.
 */
struct Preparatory {
    ExprState state{SCALAR};
    Dimensions dimensions;
    Dimensions repeated; // minor pollution from ProductOpIterator, but unlikely to affect performance
    ExpressionIterators sub_iterators;
    size_t size{1};

    Preparatory() = default;
    Preparatory(const Expressions& sub_expressions, PreparatoryType type);

private:
    void prepare_linked_operation(const Expressions& sub_expressions);
    void prepare_product_operation(const Expressions& sub_expressions);
};

} // namespace varitensor::impl

#endif // VARITENSOR_PREPARATORY_H