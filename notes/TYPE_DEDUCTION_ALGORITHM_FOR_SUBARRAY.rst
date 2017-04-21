NOTE: With compile time numbers, we can improve this.

template <typename... SliceSpecifiers>
subarray_type<array_ref, SliceSpecifiers> subarray(SliceSpecifiers&& slice_specs) const noexcept;

Pre-conditions:
    pack_is_slice_specifiers<SliceSpecifiers...>::value == true
    sizeof...(SliceSpecifiers) == rank()

Iterate through SliceSpecifiers and our mapping object (e.g. for SliceSpecifier in SliceSpecifiers, Dim in mapping object), producing a new dimensions<> type:
    * If SliceSpecifier == all_tag and Dim == dyn, preserve the original dimension.
