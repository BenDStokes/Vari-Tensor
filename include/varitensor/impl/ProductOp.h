/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * Licence, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef VARITENSOR_PRODUCT_OP_H
#define VARITENSOR_PRODUCT_OP_H

#include "common.h"
#include "deny.h"
#include "ExpressionIteratorBase.h"

namespace varitensor::impl {

struct Preparatory;

class ProductOpIterator: public ExpressionIteratorBase {
public:
    ProductOpIterator() = default;
    ProductOpIterator(double modifier, Preparatory& preparatory, bool end=false);

    ProductOpIterator(const ProductOpIterator& other) = default;
    ProductOpIterator& operator=(const ProductOpIterator& other) = default;
    ProductOpIterator(ProductOpIterator&& other) noexcept = default;
    ProductOpIterator& operator=(ProductOpIterator&& other) = default;

    ProductOpIterator& operator++();
    ProductOpIterator operator++(int);
    double operator*() const;
    bool operator==(const ProductOpIterator& other) const;

    [[nodiscard]] static bool is_metric();
    [[nodiscard]] static bool is_contiguous();
    [[nodiscard]] static double* data();

    void increment(int index_id) const;
    void reset(int index_id) const;
    [[nodiscard]] double deref() const;

private:
    double m_modifier{1};
    ExpressionIterators m_sub_iterators{};

    // for index summation
    Dimensions m_repeated;
    mutable std::vector<int> m_repeated_positions; // mutable for deref()

    bool m_end{false};
};

/**
 * @brief Represents an operation between tensor expressions where repeated indices are summed over
 * (einstein summation convention). Supports multiplication and division (though division by
 * non-scalar tensor is not allowed). Will also correctly swap in indices when summing over
 * metric tensors.
 */
class ProductOp {
public:
    template <ExpressionOperand_c OperandType1, ExpressionOperand_c OperandType2>
    ProductOp(const OperandType1& first, const OperandType2& second, Operation operation)
    {
        // order matters - indices need to be added in the order they arrive
        call_add_function(first);
        call_add_function(second, operation);
    }

    void populate(Tensor& tensor);

    [[nodiscard]] bool is_scalar() const;
    [[nodiscard]] double get_scalar() const;

    using iterator = ProductOpIterator;
    [[nodiscard]] iterator begin() const;
    [[nodiscard]] iterator end() const;
    [[nodiscard]] ExpressionIterator vbegin() const;

private:
    double m_modifier{1}; // used to collate double/scalar operands
    Expressions m_sub_expressions{};

    template<ExpressionOperand_c OperandType>
    void call_add_function(const OperandType& operand, const Operation operation=MUL) {
        if constexpr (!std::is_same_v<OperandType, double>) {
            if (operand.is_scalar()) {
                // If we have a scalar Tensor or Expression, we can short-circuit by getting the value
                add_element(operation == MUL ? operand.get_scalar() : 1/operand.get_scalar());
            }
            else {
                deny(operation == DIV, "Cannot divide by non-scalar Tensor/Expression");
                add_element(operand);
            }
        }
        else {
            add_element(operation == MUL ? operand : 1/operand);
        }
    }

    void add_element(const double& value);
    void add_element(const Tensor& tensor);
    void add_element(const LinkedOp& linked_op);
    void add_element(const ProductOp& product_op);
    void add_element(const View& view);
};

} // namespace::impl

template<varitensor::impl::ExpressionOperand_c T, varitensor::impl::ExpressionOperand_c U>
varitensor::impl::ProductOp operator*(const T& first, const U& second) {
    return {first, second, varitensor::impl::MUL};
}
template<varitensor::impl::ExpressionOperand_c T, varitensor::impl::ExpressionOperand_c U>
varitensor::impl::ProductOp operator/(const T& first, const U& second) {
    return {first, second, varitensor::impl::DIV};
}

#endif // VARITENSOR_PRODUCT_OP_H
