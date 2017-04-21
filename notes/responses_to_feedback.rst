===================================================================
Responses to Feedback on mdspan<> 
===================================================================

:Project: ISO JTC1/SC22/WG21: Programming Language C++
:Author: Bryce Lelbach 
:Contact: balelbach@lbl.gov

Responses to Feedback from Jacksonville, Oulu and C++Now
========================================================

What value should the implicit dimension constant ``std::dyn`` (e.g. the "magic value") have? -1? 0?
****************************************************************************************************

This has been discussed extensively, and the basic conclusion is that an
``inline constexpr std::ptrdiff_t`` with a value of 0 would work, but has
performance implications when you want to specify an explicit dimension with an
extent of 0 (which might arise during slicing or metaprogramming).
Since it would be impossible to distinguish a 0-length explicit dimension from
an implicit dimension, an extent of 0 would always be an implicit dimension
(with runtime storage) and compile-time reasoning in ``std::mdspan<>`` would
assume its value will not be known until runtime.

`Template argument type deduction <http://wg21.link/P0091r3>`_ is in the C++17
CD, and can be used to implement ``std::dyn`` as an ``enum class`` instead of a
``std::ptrdiff_t``.
That makes it possible to distinguish between ``std::dyn`` and an extent with
the same value as ``std::dyn``:

.. code-block:: c++

  enum class dynamic_extent_type {};

  inline constexpr dynamic_extent_type dyn { -1 };

  template < auto D > struct is_implicit_dimension
    : std::is_same_t< std::decay_t< decltype(D) > , dynamic_extent_type > {};

..

The other option, an ``inline constexpr std::ptrdiff_t`` with a value -1, does
not have any issue with 0-length implicit dimensions and is consistent with
``std::string`` and ``std::string_view`` (``std::string::npos``) and
``std::span`` (`P0122 <http://wg21.link/P0122>`_).
Consistency with ``std::extent<>``'s value for builtin array types
(``std::extent<T[], N>`` is 0, assuming T is not an array type) can be achieved
via specialization and metaprogramming. 

At Jacksonville and Kona, the consensus in LEWG was to use -1.
Our current proposal defines ``std::dyn`` as an ``inline constexpr`` instance
of an ``enum class`` with a value of -1.

Q: 
