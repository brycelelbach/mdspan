David, Neil, Jeffrey,

Prologue: It seems clear that the name 'array_ref' isn't suitable. I'm going to
rename it to 'mdspan' (multi-dimensional span, naming style is similar to numpy
and some other languages) for this email. Also, apologies for the length of
this mail.

I think I've got an elegant solution to the outstanding questions about how much
complexity mdspan should expose to the user.

Thus far, we've been talking about the mdspan interface in terms of class
templates. E.g.

    // Form #1: This form of mdspan<> is preferred by those who are not
    // concerned with array layout (e.g. row-major vs column major) or other
    // array "properties".

    mdspan<T, 32, dyn, 64>
    // 32 x dyn x 64 array, native/c-style/right/row-major layout



    // Form #2: This form of mdspan<> are preferred by those who are concerned
    // with array layout and other array "properties" like memory space or
    // atomicity.

    mdspan<T, dimensions<32, dyn, 64> >
    // 32 x dyn x 64 array, native/c-style/right/row-major layout

    mdspan<T, dimensions<32, dyn, 64>, layout_left>
    // 32 x dyn x 64 array, matlab-style/left/column-major layout

    mdspan<T, dimensions<32, dyn, 64>, layout_left, my_namespace::gpu>
    // 32 x dyn x 64 array, matlab-style/left/column-major layout, lives in GPU memory

We haven't really considered trying to support both syntaxes, because we've
made the assumption that Form #1 needs a non-type parameter pack and Form #2
needs a type parameter pack:

    // Form #1
    template <typename T, std::ptrdiff_t... Dims>
    struct mdspan;

    // Form #2
    template <typename T, typename Dims, typename... Properties>
    struct mdspan;

There is another problem with both forms of the class template interface:

    mdspan<T, dyn, Y> a(ptr, X);              // 1st dimension is X, but we wrote Y first!
    mdspan<T, dimensions<dyn, Y> > a(ptr, X); // Same problem. 

    using my_3d_mdspan = mdspan<T, X, dyn, Z>;
    // ... many lines of code ...
    my_3d_mdspan b(ptr, Y); // It may be unclear to the user why they are specifying
                            // only one dimension of a 3D array - and why that
                            // dimension is the second one!

Finally, there is the matter of the 'dyn' parameter, which we have discussed
multiple times in LEWG:

    constexpr std::size_t dyn = /* What number do we pick? */; 

(I should note that this problem can be solved using C++17 template <auto...>
and making dyn a struct dynamic_extent_tag)

I think we can solve all of these issues. My solution, inspired by Louis
Dionne's suggestion at C++Now, makes use of "compile time numbers" and C++17
template deduction for class templates:

    template <auto... I>
    struct dimensions; // Essentially an array<std::ptrdiff_t, Rank> which 
                       // doesn't have any memory overhead for static
                       // (compile-time) extents. 

    template <auto... Props>
    using __properties; // Sequence of non-type template parameters.

    template <typename T, typename Dims, typename Properties>
    struct __mdspan_impl; 

    template <typename T, typename... Things>
    constexpr auto __build_mdspan_impl(Things... things) noexcept;  
    template <typename T, typename... Things>
    constexpr auto __build_mdspan_impl(T* ptr, Things... things) noexcept;  
    // This algorithm goes through the list of Things parameter pack and builds
    // a dimensions<> object and a properties<> object. Then it returns
    // an __mdspan_impl<T, dimensions<>, properties<> > constructed from the
    // inputs.
    // - If a Thing is an integral_constant<>, it is a static dimension.
    // - If a Thing is an integral type, it is a specified dynamic dimension.
    // - If a Thing is a dynamic_extent_tag, it is an unspecified dynamic
    //   dimension (e.g. the dynamic dimension is default initialized).
    // - If a Thing is anything else, it is a property.
    // - If any Things that would be classified as dimensions are encountered
    //   after the first property, this function should static_assert.

    template <typename T, typename... Things>
    struct mdspan
      : decltype(__build_mdspan_impl(add_pointer_t<T>(), Things()...)
    {
        explicit constexpr mdspan(Things... things) noexcept
          : __mdspan_impl(__build_mdspan_impl(things...)
            // If you find it odd to invoke __md_span_impl's move ctor, you
            // could instead write __build_mdspan_impl() as two functions, one
            // returning the dimensions<> object and one returning the
            // properties<> object.
        {}

        explicit constexpr mdspan(T* ptr, Things... things) noexcept
          : __mdspan_impl(__build_mdspan_impl(ptr, things...)
        {}
    };



    // Constructing/defining mdspan<>s is easy
    auto a = mdspan<T>(p, 32_c, 128, 64_c);
    auto b = mdspan<T, layout_left>(p, 32_c, 128, 64_c);

    return mdspan<T>(p, 32_c, 128, 64_c);
    return mdspan<T, layout_left>(p, 32_c, 128, 64_c);

    // Declaring them is also easy! Note that in these
    mdspan<T, 32c, dyn, 64_c> a;
    void f(mdspan<T, 32_c, dyn, 64_c> a);

Epilogue: I have not given any consideration to the suggested mdspan<T[][][]>
interface which requires language changes; the issue 

