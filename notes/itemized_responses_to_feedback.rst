===================================================================
Responses to Feedback on array_ref<> 
===================================================================

:Project: ISO JTC1/SC22/WG21: Programming Language C++
:Author: Bryce Lelbach 
:Contact: balelbach@lbl.gov

Responses to Comments on P0009 Raised in Jacksonville
=====================================================

**(Axel/Eric N): Why not use npos as the "magic value" for implicit dimensions instead of 0?**

I originally felt that this was an interesting idea and supported it, however I now
believe this is the wrong decision:

* ``npos`` is a static const member of ``std::string``. This make it quite verbose to use ``npos`` (e.g. ``std::string::npos``) and potentially confusing to users (*"Why are we using this string thing with arrays?"*).
* Additionally, using a non-zero magic value may be undesirable in the future. If zero-length arrays, and thus zero-length types, were added to the standard (which Sean Parent proposed at C++Now 2016), then we would be able to use an abbreviated syntax for specifying array dimensions (``array_ref<T[0][N][0]>``).

**(Axel/Bryce): Is 0 an acceptable "magic value", despite the overlap with compiletime-specified dimensions of size 0? If 0 is used as a magic value, then a dimension of size 0 cannot be expressed as a compiletime-specified dimension - it will always be runtime-specified. There is only one place where we know this may have performance implications. For a layout which supports padding, ``dimensions<0, 0, ...>`` would be a natural way to express no padding (e.g. the default option). However, because 0 is a magic value, the indexing logic will not see the padding as compile-time 0s (although an optimizing compiler may be able to figure this out). Additionally, it may be confusing if you are trying to express padding for just one dimension. In theory, if you just wanted (runtime-specified) padding for the 2nd dimension of a 3D array, you'd want to write ``dimensions<0, dynamic, 0> d(N)``. Today, you'd have to write ``dimensions<0, 0, 0> d(0, N, 0)``.**

I came up with two potential ideas about how to fix this.

My first idea was to express no padding with a special ``no_padding`` tag
instead of ``dimensions<0, 0, ...>``.  However, this does not fix the confusing
syntax for specifying runtime-specified padding for one dimension and no
padding for other dimensions. 

My second idea is to change how padding is specified. Instead of asking for a
particular amount of padding, we could specify padding as a divisor for a
modulo operation (e.g. "pad-to"). A good implementation could pre-compute the
actual padding. Clearly, this would require no storage for a
compiletime-specified pad-to. For runtime-specified pad-to, the implementation
can pre-compute and store the actual padding and discard the pad-to value,
requiring no additional no storage. However, a naive implementation might
perform integer modulos in indexing operations for runtime-specified pad-to
(this is a QoI issue though).

Finally, if `template argument type deduction <http://open-std.org/jtc1/sc22/wg21/docs/papers/2015/n4469.html>`_
is ever added to the IS, we could use a tag type as the magic value instead of
an integral value.

**(Tony): Are there use cases for reference not be add_reference_t<>?**

Yes. ``array_ref<>`` might be referring to memory in another memory space
(an accelerator, for example), and ``reference`` could be a proxy type.

**(Axel): You should not take integral arguments by template parameter. If this
helps with vectorization, then add a note to implementors**

This defect is mostly fixed. For the functions that take a number of parameters
equal to the rank, we still use variadic templates (which I think is fine).

**(Axel): What is the correct size_type/rank_type? std::size_t? Don't let this
be implementation defined, bad implementations could make it short.**

We agree this is an issue. Setting aside issues with std::size_t/unsigned types
in loops (which are out of scope), we'd like to fix this. I'd like some guidance
from LEWG. Should we explicity specify a type, or specify the requirements of
the type and let implementors choose (e.g. *"A type which is large enough to..."*).

**(Eric N): Pointer-based constructor should be explicit.**

Fixed.

**(Axel and Eric N): The destructor should be trivial.**

Fixed.

**(Tony): Can we SFINAE out stride() with is_regular (or static_assert)?**

We can. This should have been fixed, but we lost the wording for stride() in
the latest version of the paper. We'll fix this in the next draft.

**(Axel, Johnathon C): Remove maximum rank**

Done.

**(Tony, Axel): Don't explicity specialize is_assignable, instead SFINAE. (Eric N): Design some custom predicate, array_ref_is_assignable. Use magic "doesn't participate in overload resolution" language".**

Unsure of the status of this, Carter?

**(Axel): Add an array_ref<> conversion that "strips" out a dimension.**

Unsure of the status of this, and it's unclear what was intended from the
minutes. Carter indicated this is only possible if we SFINAE on all
dimension being compiletime. I'm not sure if we want this.

**(Neil, Tony, Titus): Having bounds checking be a part of the type may be a problem. Should we have .at()? (Titus): No.**

We need to decide on a direction here. Do we have it as part of the type, or do
we have .at()?

Response to Comments on P0009 Raised at C++Now
==============================================

**(David S): This is too complex for my users. I'd like a name that makes it clear this is for advanced users.**

I'm onboard with this. We could also potentially define a simpler type that
doesn't expose layout or properties to users using ``array_ref``.

Other Issues
============

**Review of layout_concept**

**Unify extent/dimension terminology**

**Extensibility**

**Owning type built with array_ref<> and vector<>**


