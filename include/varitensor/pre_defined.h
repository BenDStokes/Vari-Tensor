/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * Licence, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef VARITENSOR_PRE_DEFINED_H
#define VARITENSOR_PRE_DEFINED_H

#include "impl/common.h"

namespace varitensor {

/**
 * @brief Basic Euclidean metric function, used as the default for metric_tensor(...)
 */
const MetricFunction EUCLIDEAN_METRIC = [](const int i, const int j) {
    return i == j ? 1 : 0;
};

/**
 * @brief Creates a metric tensor that can be used to raise and lower indices
 *
 * Metric tensors function like normal tensors, but when used in einstein summation, their non-summed index positionally
 * replaces the summed one in the new tensor. This is commonly used in differential geometry to raise and lower indices.
 *
 * e.g. B_i_j * g^i^k = B^k_j
 *
 * This tensor will always use a lowercase g symbol when pretty printing.
 *
 * @param indices The indices with which to initialise the metric. There must be two indices, both of the same length.
 *
 * @param metric_function A user-defined function to populate the metric. The function must accept two ints giving
 * the position in the metric and return a double giving the value; varitensor::MetricFunction is provided as a
 * helper alias.
 */
Tensor metric_tensor(
    std::initializer_list<VarianceQualifiedIndex> indices,
    const MetricFunction& metric_function = EUCLIDEAN_METRIC
);

/**
 * @brief Creates an n-dimensional Levi-Civita symbol
 *
 * This tensor will always use an epsilon symbol when pretty-printing.
 *
 * @param indices The indices with which to initialise the symbol. There must be at least 2 indices, and they must all
 * be the same length.
 */
///@{
Tensor levi_civita_symbol(std::initializer_list<Index> indices);
Tensor levi_civita_symbol(std::initializer_list<VarianceQualifiedIndex> indices);
Tensor antisymmetric_symbol(std::initializer_list<Index> indices);
Tensor antisymmetric_symbol(std::initializer_list<VarianceQualifiedIndex> indices);
///@}

/**
 * @brief Creates an n-dimensional Kronecker delta
 *
 * This tensor will always use an delta symbol when pretty-printing.
 *
 * @param indices The indices with which to initialise the delta. There must be at least 2 indices, and they must all
 * be the same length.
 */
///@{
Tensor kronecker_delta(std::initializer_list<Index> indices);
Tensor kronecker_delta(std::initializer_list<VarianceQualifiedIndex> indices);
///@}

} // namespace varitensor

#endif // VARITENSOR_PRE_DEFINED_H
