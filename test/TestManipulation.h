#ifndef TEST_MANIPULATION_H
#define TEST_MANIPULATION_H

#include <memory>

#include "TestSet.h"
#include "varitensor/Tensor.h"

struct SetName final : TestSet::Test {
    explicit SetName() : Test("Set Name") {}
    bool run_test() override {
        Tensor test_tensor(1);
        test_tensor.set_name("test");
        return test_tensor.name() == "test";
    }
};

struct Transposition3x3 final : TestSet::Test {
    explicit Transposition3x3() : Test("Index Transposition (3x3)") {}
    bool run_test() override
    {
        // given
        const Index i{LATIN}, j{LATIN};
        Tensor tensor{i, j};
        std::ranges::iota(tensor, 0);

        // when
        tensor.transpose(i, j);

        // then
        const bool values_correct =
            tensor[0, 0] == 0 && tensor[1, 0] == 3 && tensor[2, 0] == 6 &&
            tensor[0, 1] == 1 && tensor[1, 1] == 4 && tensor[2, 1] == 7 &&
            tensor[0, 2] == 2 && tensor[1, 2] == 5 && tensor[2, 2] == 8;

        return values_correct && tensor.index_position(i) == 1 && tensor.index_position(j) == 0;
    }
};

struct Transposition3x3x3 final : TestSet::Test {
    explicit Transposition3x3x3() : Test("Index Transposition (3x3x3)") {}
    bool run_test() override
    {
        const Index i{LATIN}, j{LATIN}, k{LATIN};
        Tensor tensor{i, j, k};

        tensor.transpose(i, k);

        return tensor.index_position(i) == 2 && tensor.index_position(k) == 0;
    }
};

struct Relabelling final : TestSet::Test {
    explicit Relabelling() : Test("Index Relabelling") {}
    bool run_test() override {
        const Index i{LATIN},j{LATIN};
        Tensor test_tensor{i};

        test_tensor.relabel(i, j);

        return test_tensor.has_index(j) && !test_tensor.has_index(i);
    }
};

struct SetVariance final : TestSet::Test {
    explicit SetVariance() : Test("Set Variance") {}
    bool run_test() override {
        const Index i{LATIN};
        Tensor test_tensor1{{i, COVARIANT}};
        Tensor test_tensor2{{i, CONTRAVARIANT}};

        test_tensor1.set_variance(i, CONTRAVARIANT);
        test_tensor2.set_variance(i, COVARIANT);

        return test_tensor1.variance(i) == CONTRAVARIANT && test_tensor2.variance(i) == COVARIANT;
    }
};

struct Raising final : TestSet::Test {
    explicit Raising() : Test("Index Raising") {}
    bool run_test() override {
        const Index i{LATIN};
        Tensor test_tensor{{i, COVARIANT}};

        test_tensor.raise(i);
        return test_tensor.variance(i) == CONTRAVARIANT;
    }
};

struct Lowering final : TestSet::Test {
    explicit Lowering() : Test("Index Lowering") {}
    bool run_test() override {
        const Index i{LATIN};
        Tensor test_tensor{{i, CONTRAVARIANT}};

        test_tensor.lower(i);
        return test_tensor.variance(i) == COVARIANT;
    }
};

struct TestManipulation final : TestSet {
    explicit TestManipulation() : TestSet("Test Manipulation Functions") {
        add_sub_test(std::make_unique<SetName>());
        add_sub_test(std::make_unique<Transposition3x3>());
        add_sub_test(std::make_unique<Transposition3x3x3>());
        add_sub_test(std::make_unique<Relabelling>());
        add_sub_test(std::make_unique<SetVariance>());
        add_sub_test(std::make_unique<Raising>());
        add_sub_test(std::make_unique<Lowering>());
    }
};

#endif
