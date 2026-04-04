## Contributing

Contributions in all forms are welcome!
- For bug reports, feature requests or questions: please open an issue, making it clear which
of the aforementioned it relates to
- If you have a patch: create a branch and open a pull request

If you plan to work on a patch, it is highly recommended that you first open an issue so that
your proposal can be discussed. This helps prevent obvious pitfalls and ensures that any work
done fits with the goals of the project.

Keep all discussions civil and respectful!

## Getting Started

Once you've cloned the repo, load the CMakeLists.txt into your IDE, and you should be able to
run the various test targets:

 - varitensor_tests
 - varitensor_tests_avx
 - varitensor_tests_validation_off
 - varitensor_test_avx_validation_off

## Code Style

Follow the style of the already-existing code. General guidelines and exceptions are given
below...

For braces, use K&R style with no cuddled else:

```c++
if (...) {
    // ...
}
else {
    // ...
}
```

Exception — constructors with initialiser lists should be formatted as follows:

```c++
ClassName::Classname(int param1, int param2):
    m_variable1{param1},
    m_variable2{param2}
{
    // ...   
}
```

Single line control statements are OK as long as they do one simple thing and are actually on a
single line:

```c++
if (a > b) return a; // good

if (a > b)
    return a; // bad
```

Use varitensor::impl::deny() to deny things that don't make mathematical sense. This
function can be toggled off at compile time for increased performance.

## Naming Conventions

 - Follow the conventions of the already-existing code — it's fairly standard
 - Prefix members with "m_" and statics with "s_". Suffix concepts with "_c".
 - Prefix header guards with VARITENSOR_
 - The public interface should be under the namespace "varitensor". All other entities should
be under the namespace "varitensor::impl".

## Overview of architecture

Programmatically, a tensor is essentially just a multidimensional array. Internally they
consist of a size, a double* to a block of heap data, and a vector of dimensions that
describes the data's shape. In Vari-Tensor, each dimension also has "variance"
(UPPER/CONTRAVARIANT or LOWER/COVARIANT) which if you're not familiar with the maths can
be thought of analogous to a unit of measure for that dimension.

Throughout the library and the rest of this document, a certain amount of familiarity with
tensors and their accompanying jargon is assumed.

---

Unless basic information (eg. the number of indices) is being retrieved or modified,
Vari-Tensor uses three objects to capture tensor expressions:

- View — used to express unary operations that amount to viewing, slicing or contracting a
  tensor
- LinkedOp — used to express binary operations (currently just + and -) where repeated
  indices should be "linked" together
- ProductOp — used to express binary operations (currently just * and /) where repeated
  indices should be summed over with the Einstein summation convention

The ProductOp and LinkedOp classes are effectively lists of pointers to other tensor
expressions. The View class is effectively a pointer to a tensor. When a user writes a
tensor expression, the two operation classes form the branches of an expression tree,
with View being the leaves.

```
T = A * (B - C)  ->   ProductOp
                            |
                      View  ⊥  LinkedOp
                        |            |
                        A       View ⊥ View
                                 |      |
                                 B      C
```

Each expression class has an iterator. To find the values of an expression, the library
iterates over the top-level object with begin()/end(). Each sub-expression understands how
to iterate over its own sub-expressions down to the View objects which understand how to
iterate over their target tensors.

The lynch-pin of the whole operation is the increment_positions() function, which is used to
perform all incrementation of iterators, whether over their position to advance the iterator,
or over repeated indices to find the iterator's value at the current point.

---

A const version of ViewIterator is necessary because the elements of a View that is
contracted do not yet exist. Non-const View::operator*() returns double&, as it is a true
view on the elements of an already existing tensor. But contraction creates a tensor with
entirely new elements, so there are no values to return double& on. Therefore, a const
version returning only double is required. This is achieved by passing ViewIterator a
boolean template parameter that is concealed from the user via type deduction.

---

Another point of subtlety in the ViewIterator concerns the const-ness of deref(), increment()
and reset(). The deref() function is conceptually const, so users should be able to rely on
this, but, to calculate its value, it has to iterate through an entire cycle of the
m_repeated_positions vector. This is fine because the positions always start at 0 and return
to 0 by the end of the function. However, since we pass the call through
increment_positions(), there must be const versions of increment()/reset() for it to call,
despite the fact that these operations are individually NOT const.

To complicate this further, both increment() and reset() make sense as part of the public
interface, but in this case should NOT be const as this could give users spurious and
annoying linter warnings. These issues are currently resolved as follows:

 - Several members are declared mutable so that they can be modified by deref() and
by increment()/reset() when called under deref()
 - increment()/reset() both have a private overload that is declared const but isn't really
as it modifies the state – use these with caution!
 - increment()/reset() have public overloads that are (correctly) not declared const but only
call the (spuriously) const private versions; we therefore have to swallow the clang tidy
warning with NOLINT so that users don't have to

This is convoluted, but it achieves the correct public interface. ProductIterator also has this
problem to a lesser extent, but since it's currently under namespace "impl", we resolve it by
simply declaring m_repeated_positions as mutable.

---

As of the last major upgrade, Vari-Tensor now has two different ways of resolving expressions
that can be thought of as two different "modes". The general fallback (also used when
the user doesn't want to resolve the whole expression at once) is the original iterator mode.

However, if we are resolving the whole expression (e.g. for an assignment) and it falls within
a set of operations which can be efficiently optimised, a new "bulk" mode is used. This mode
is far faster than the iterators as it deals directly with the memory and can employ CPU
vectorisation, but algorithms only exist for common cases.

These two systems are individually very understandable, but the boundary where they meet can
be complex. The populate() functions govern which algorithms are used to resolve different
cases. They accomplish this by creating a "Preparatory" object that optimistically prepares
to execute a bulk algorithm but can seamlessly switch to having prepared to use the iterators
at any point.
