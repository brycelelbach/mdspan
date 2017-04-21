dimensions<A, B, C, D, E>

slice(all, 1, 2, all, all)

template <auto... Dims, typename... SliceSpecs>
auto slice(dimensions<Dims...> d, SliceSpecs&&... ss);

template <std::size_t NDimsHead, auto... DimsTail, typename SliceSpecHead, typename... SliceSpecTail>
auto slice(dimensions<DimsHead, DimsTail...> d, SliceSpecHead&& ssh, SliceSpecTail&&... sst);

We may need to iterate dimensions in "order".

Generate a compile-time list of indices in order?

template <
    typename Dimensions
  , typename SliceSpecs // tuple
  , std::size_t N
  , typename enable
    >
struct slicer
{
    auto operator()(
        Dimensions d
      , SliceSpecs ss
        ) const noexcept
    {
        using T = std::tuple_element_t<N, SliceSpecs>;
        static_assert(is_slice_specifier_v<T>);

        if constexpr (is_integral_range_slice_specifier_v<T>)
        {
             
        }
        else // Integral specifier, remove the dimension
        {
            // Compute new base pointer
        }
    }
};

a00 a01 a02
a10 a11 a12

Column:         Row:
a00             a00
a10             a01
a01             a02
a11             a10
a02             a11
a12             a12
2*i + j         i + 3*j


idx = i + nx*j + nx*ny*k

ij_idx = bp + i + nx*j
ik_idx = bp + i + nx*ny*k
jk_idx = bp + nx*j + nx*ny*k
 
