cmake_minimum_required(VERSION 3.25)
project(varitensor VERSION 1.1.0 DESCRIPTION "Ergonomic tensors with variance.")

set(CMAKE_CXX_STANDARD 23)

add_executable(varitensor_tests
    test/run_tests.cxx
    src/Tensor.cxx
    src/pretty_print.cxx
    src/impl/ExpressionIteratorBase.cxx
    src/impl/Index.cxx
    src/impl/LinkedOp.cxx
    src/impl/pre_defined.cxx
    src/impl/ProductOp.cxx
    src/impl/View.cxx
    src/impl/bulk.cxx
    src/impl/Preparatory.cxx
)

target_include_directories(varitensor_tests
    PRIVATE
        test
        include
)
