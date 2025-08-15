#ifndef TEST_CONSTRUCTION_H
#define TEST_CONSTRUCTION_H

#include "TestSet.h"
#include "varitensor/Tensor.h"

// =================================================================================================
//                                                             variance qualified initializer-list |
// =================================================================================================

struct VqiList final: TestSet::Test {
    explicit VqiList() : Test("VQI Init-List") {}

    bool run_test() override {
        const Index i{LATIN}, j{LATIN};

        const Tensor tensor{
            {i, CONTRAVARIANT},
            {j, COVARIANT},
        };

        return true;
    }
};

struct VqiListName final: TestSet::Test {
    explicit VqiListName() : Test("VQI Init-List (Name)") {}

    bool run_test() override {
        const Index i{LATIN}, j{LATIN};

        const Tensor tensor{
            "test",
            {
                {i, CONTRAVARIANT},
                {j, COVARIANT},
            }
        };

        return tensor.name() == "test";
    }
};

struct VqiListInitVal final: TestSet::Test {
    explicit VqiListInitVal() : Test("VQI Init-List (Init. Value)") {}

    bool run_test() override {
        const Index i{LATIN}, j{LATIN};

        const Tensor tensor{
            {
                {i, CONTRAVARIANT},
                {j, COVARIANT},
            },
            1
        };

        return std::ranges::all_of(tensor, [](const auto& value) { return value == 1; });
    }
};

struct VqiListNameInitVal final: TestSet::Test {
    explicit VqiListNameInitVal() : Test("VQI Init-List (Name+Init. Value)") {}

    bool run_test() override {
        const Index i{LATIN}, j{LATIN};

        const Tensor tensor{
            "test",
            {
                {i, CONTRAVARIANT},
                {j, COVARIANT},
            },
            1
        };

        return
            tensor.name() == "test" &&
            std::ranges::all_of(tensor, [](const auto& value) { return value == 1; });
    }
};

// =================================================================================================
//                                                                       variance qualified vector |
// =================================================================================================

struct VqiVector final: TestSet::Test {
    explicit VqiVector() : Test("VQI Vector") {}

    bool run_test() override {
        const Index i{LATIN}, j{LATIN};
        const auto indices = std::vector<VarianceQualifiedIndex>{
            {i, CONTRAVARIANT},
            {j, COVARIANT},
        };
        const Tensor tensor{indices};

        return true;
    }
};

struct VqiVectorName final: TestSet::Test {
    explicit VqiVectorName() : Test("VQI Vector (Name)") {}

    bool run_test() override {
        const Index i{LATIN}, j{LATIN};
        const auto indices = std::vector<VarianceQualifiedIndex>{
            {i, CONTRAVARIANT},
            {j, COVARIANT},
        };
        const Tensor tensor{"test", indices};

        return tensor.name() == "test";
    }
};

struct VqiVectorInitVal final: TestSet::Test {
    explicit VqiVectorInitVal() : Test("VQI Vector (Init. Value)") {}

    bool run_test() override {
        const Index i{LATIN}, j{LATIN};
        const auto indices = std::vector<VarianceQualifiedIndex>{
            {i, CONTRAVARIANT},
            {j, COVARIANT},
        };
        const Tensor tensor{indices, 1};

        return std::ranges::all_of(tensor, [](const auto& value) { return value == 1; });
    }
};

struct VqiVectorNameInitVal final: TestSet::Test {
    explicit VqiVectorNameInitVal() : Test("VQI Vector (Name+Init. Value)") {}

    bool run_test() override {
        const Index i{LATIN}, j{LATIN};
        const auto indices = std::vector<VarianceQualifiedIndex>{
                {i, CONTRAVARIANT},
                {j, COVARIANT},
            };
        const Tensor tensor{"test", indices, 1};

        return
            tensor.name() == "test" &&
            std::ranges::all_of(tensor, [](const auto& value) { return value == 1; });
    }
};

// =================================================================================================
//                                                              unqualified index initializer-list |
// =================================================================================================

struct IndexList final: TestSet::Test {
    explicit IndexList() : Test("UQ Init-List") {}

    bool run_test() override {
        Index i{LATIN}, j{LATIN};
        const Tensor tensor{{i, j}};

        return true;
    }
};

struct IndexListName final: TestSet::Test {
    explicit IndexListName() : Test("UQ Init-List (Name)") {}

    bool run_test() override {
        Index i{LATIN}, j{LATIN};
        const Tensor tensor{"test", {i, j}};

        return tensor.name() == "test";
    }
};

struct IndexListInitVal final: TestSet::Test {
    explicit IndexListInitVal() : Test("UQ Init-List (Init. Value)") {}

    bool run_test() override {
        Index i{LATIN}, j{LATIN};
        const Tensor tensor{{i, j}, 1};

        return std::ranges::all_of(tensor, [](const auto& value) { return value == 1; });
    }
};

struct IndexListNameInitVal final: TestSet::Test {
    explicit IndexListNameInitVal() : Test("UQ Init-List (Name+Init. Value)") {}

    bool run_test() override {
        Index i{LATIN}, j{LATIN};
        const Tensor tensor{"test", {i, j}, 1};

        return
            tensor.name() == "test" &&
            std::ranges::all_of(tensor, [](const auto& value) { return value == 1; });
    }
};

// =================================================================================================
//                                                                        unqualified index vector |
// =================================================================================================

struct IndexVector final: TestSet::Test {
    explicit IndexVector() : Test("UQ Vector") {}

    bool run_test() override {
        const Index i{LATIN}, j{LATIN};
        const std::vector indices{i, j};

        const Tensor tensor{indices};

        return true;
    }
};

struct IndexVectorName final: TestSet::Test {
    explicit IndexVectorName() : Test("UQ Vector (Name)") {}

    bool run_test() override {
        const Index i{LATIN}, j{LATIN};
        const std::vector indices{i, j};

        const Tensor tensor{"test", indices};

        return tensor.name() == "test";
    }
};

struct IndexVectorInitVal final: TestSet::Test {
    explicit IndexVectorInitVal() : Test("UQ Vector (Init. Val)") {}

    bool run_test() override {
        const Index i{LATIN}, j{LATIN};
        const std::vector indices{i, j};

        const Tensor tensor{indices, 1};

        return std::ranges::all_of(tensor, [](const auto& value) { return value == 1; });
    }
};

struct IndexVectorNameInitVal final: TestSet::Test {
    explicit IndexVectorNameInitVal() : Test("UQ Vector (Name+Init. Val)") {}

    bool run_test() override {
        const Index i{LATIN}, j{LATIN};
        const std::vector indices{i, j};

        const Tensor tensor{"test", indices, 1};

        return
            tensor.name() == "test" &&
            std::ranges::all_of(tensor, [](const auto& value) { return value == 1; });
    }
};

// =================================================================================================
//                                                                                           other |
// =================================================================================================

struct Copy final: TestSet::Test {
    explicit Copy() : Test("Copying") {}

    bool run_test() override {
        const Tensor first(1);
        Tensor second(first); // NOLINT - clang tidy will correctly attempt to avoid the copy
        return first == second;
    }
};

struct CopyAssign final: TestSet::Test {
    explicit CopyAssign() : Test("Copy Assignment") {}

    bool run_test() override {
        const Tensor first(1);
        auto second = first; // NOLINT - clang tidy will correctly attempt to avoid the copy
        return first == second;
    }
};

struct Move final: TestSet::Test {
    explicit Move() : Test("Moving") {}

    bool run_test() override {
        Tensor first(1);
        const Tensor second(1);
        const Tensor third(std::move(first));
        return second == third;
    }
};

struct MoveAssign final: TestSet::Test {
    explicit MoveAssign() : Test("Move Assignment") {}

    bool run_test() override {
        Tensor first(1);
        auto second = std::move(first);
        return true;
    }
};

struct TestConstruction final: TestSet {
    explicit TestConstruction() : TestSet("Test Construction") {
        add_sub_test(std::make_unique<VqiList>());
        add_sub_test(std::make_unique<VqiListName>());
        add_sub_test(std::make_unique<VqiListInitVal>());
        add_sub_test(std::make_unique<VqiListNameInitVal>());

        add_sub_test(std::make_unique<VqiVector>());
        add_sub_test(std::make_unique<VqiVectorName>());
        add_sub_test(std::make_unique<VqiVectorInitVal>());
        add_sub_test(std::make_unique<VqiVectorNameInitVal>());

        add_sub_test(std::make_unique<IndexList>());
        add_sub_test(std::make_unique<IndexListName>());
        add_sub_test(std::make_unique<IndexListInitVal>());
        add_sub_test(std::make_unique<IndexListNameInitVal>());

        add_sub_test(std::make_unique<IndexVector>());
        add_sub_test(std::make_unique<IndexVectorName>());
        add_sub_test(std::make_unique<IndexVectorInitVal>());
        add_sub_test(std::make_unique<IndexVectorNameInitVal>());

        add_sub_test(std::make_unique<Copy>());
        add_sub_test(std::make_unique<CopyAssign>());
        add_sub_test(std::make_unique<Move>());
        add_sub_test(std::make_unique<MoveAssign>());
    }
};

#endif
