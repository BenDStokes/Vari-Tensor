/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * Licence, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef VARITENSOR_PRETTY_PRINT_H
#define VARITENSOR_PRETTY_PRINT_H

#include <iomanip>

#include "Tensor.h"

namespace varitensor {

/**
 * @brief Set the width of the fields when geometric pretty-printing
 *
 * Any values on the tensor longer than this width will be truncated with a ~ symbol.
 */
void set_print_data_width(int width);

/**
 * @brief Set the number of significant figures to round values to when pretty-printing
 */
void set_print_precision(int precision);

} // namespace varitensor

std::ostream& operator<<(std::ostream& stream, const Tensor& tensor);

#endif // VARITENSOR_PRETTY_PRINT_H
