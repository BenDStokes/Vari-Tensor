/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * Licence, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef VARITENSOR_LINKED_OP_H
#define VARITENSOR_LINKED_OP_H

#include "common.h"
#include "View.h"

namespace varitensor::impl {

struct Preparatory;

class LinkedOpIterator: public ExpressionIteratorBase {
public:
    LinkedOpIterator() = default;

    LinkedOpIterator(double modifier, Preparatory& preparatory, const std::vector<Operation>* signs, bool end=false);

    LinkedOpIterator(const LinkedOpIterator& other) = default;
    LinkedOpIterator& operator=(const LinkedOpIterator& other) = default;
    LinkedOpIterator(LinkedOpIterator&& other) noexcept = default;
    LinkedOpIterator& operator=(LinkedOpIterator&& other) = default;

    LinkedOpIterator& operator++();
    LinkedOpIterator operator++(int);
    double operator*() const;
    bool operator==(const LinkedOpIterator& other) const;

    [[nodiscard]] static bool is_metric();
    [[nodiscard]] static bool is_contiguous();
    [[nodiscard]] static double* data();

    void increment(int index_id) const;
    void reset(int index_id) const;
    [[nodiscard]] double deref() const;

private:
    double m_modifier{0};
    ExpressionIterators m_sub_iterators{};
    const std::vector<Operation>* m_signs{nullptr};  // * to allow default construction

    bool m_end{false};
};

class LinkedOp {
/**
 * Represents an operation between tensor expressions where repeated indices
 * are "linked". Supports addition and subtraction.
 */
public:
    template <ExpressionOperand_c OperandType1, ExpressionOperand_c OperandType2>
    LinkedOp(const OperandType1& first, const OperandType2& second, Operation sign) {
        call_add_function(first);
        call_add_function(second, sign);
    }

    void populate(Tensor& tensor, bool allocate = true);

    [[nodiscard]] bool is_scalar() const;
    [[nodiscard]] double get_scalar() const;

    using iterator = LinkedOpIterator;
    [[nodiscard]] iterator begin() const;
    [[nodiscard]] iterator end() const;
    [[nodiscard]] ExpressionIterator vbegin() const;

private:
    double m_modifier{0}; // used to collate double/scalar operands
    Expressions m_sub_expressions{};
    std::vector<Operation> m_signs;  // used to track the sign of each sub-expression

    template<ExpressionOperand_c OperandType>
    void call_add_function(const OperandType& operand, const Operation sign=ADD) {
        if constexpr (!std::is_same_v<OperandType, double>) {
            if (operand.is_scalar()) {
                // If we have a scalar Tensor or Expression, we can short-circuit by getting the value
                add_element(operand.get_scalar(), sign);
                return;
            }
        }
        add_element(operand, sign);
    }

    void add_element(double value, Operation sign);
    void add_element(const Tensor& tensor, Operation sign);
    void add_element(const LinkedOp& linked_op, Operation sign);
    void add_element(const ProductOp& product_op, Operation sign);
    void add_element(const View& view, Operation sign);
};

} // namespace varitensor::impl

template<varitensor::impl::ExpressionOperand_c T, varitensor::impl::ExpressionOperand_c U>
varitensor::impl::LinkedOp operator+(const T& first, const U& second) {
    return {first, second, varitensor::impl::ADD};
}
template<varitensor::impl::ExpressionOperand_c T, varitensor::impl::ExpressionOperand_c U>
varitensor::impl::LinkedOp operator-(const T& first, const U& second) {
    return {first, second, varitensor::impl::SUB};
}

#endif // VARITENSOR_LINKED_OP_H
