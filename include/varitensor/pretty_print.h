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

void set_print_data_width(int width);
void set_print_precision(int precision);

} // namespace varitensor

std::ostream& operator<<(std::ostream& stream, const Tensor& tensor);

#endif // VARITENSOR_PRETTY_PRINT_H
