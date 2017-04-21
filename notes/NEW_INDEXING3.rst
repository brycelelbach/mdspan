///////////////////////////////////////////////////////////////////////////////
// Order Agnostic
///////////////////////////////////////////////////////////////////////////////

d   := dims
p   := padding (should have rank + 1 elements not rank elements)
s   := stepping
i   := index
rto := rank index to ordering (smallest stride dimension to largest stride dimension)
otr := ordering to rank index (smallest rank index to largest rank index)

stride[otr[0]] = p[rto[otr[0]+1]] + s[otr[0]]
stride[n]      = p[n]      + s[n]      * d[otr[rto[n]-1]] * stride[otr[rto[n]-1]]
start at n = otr[n]

index[otr[0]] = i[rto[otr[0]+1]] * stride[otr[0]]
index[n]      = i[n]      * stride[n]      + index[otr[rto[n]-1]]
start at n = otr[n]

// Left
// p[0] is the unit stride padding. 
stride[0] =                                                 p[1] + s[0]
stride[1] =                                 p[2] + s[1]d[0](p[1] + s[0])
stride[2] =                 p[3] + s[2]d[1](p[2] + s[1]d[0](p[1] + s[0]))
stride[3] = p[4] + s[3]d[2](p[3] + s[2]d[1](p[2] + s[1]d[0](p[1] + s[0])))

// Right
// p[4] is the unit stride padding.
stride[3] =                                                 p[3] + s[3]
stride[2] =                                 p[2] + s[2]d[3](p[3] + s[3])
stride[1] =                 p[1] + s[1]d[2](p[2] + s[2]d[3](p[3] + s[3]))
stride[0] = p[0] + s[0]d[1](p[1] + s[1]d[2](p[2] + s[2]d[3](p[3] + s[3])))

Approaches:
* Try and make this work.
* Go back to r padding dims, add a new padding parameter.
* Make p[-1] valid.
* Don't care about this type of padding.
    * I think this is the thing to do. We're not the one allocating the array,
      we're just a view into it. If the user wants to pad at the end, they
      should take care of that themselves.
    * This has the added benefit of simplifying the new indexing code and not
      breaking the current unit tests quite so badly.
    * Downsides?
        * Feels like a cop-out.
        * Adding this later might require another reformulation of indexing.

Let's go with scrapping this for now. This leaves us with the below formulation.

I still question the general usefulness of p[otr[0]]. You should probably just
rebase the pointer instead of using p[otr[0]].

///////////////////////////////////////////////////////////////////////////////
// Order Agnostic
///////////////////////////////////////////////////////////////////////////////

d   := dims
p   := padding 
s   := stepping
i   := index
rto := rank index to ordering (smallest stride dimension to largest stride dimension)
otr := ordering to rank index (smallest rank index to largest rank index)

stride[otr[0]] = p[otr[0]] + s[otr[0]]
stride[n]      = p[n]      + s[n]      * d[otr[rto[n]-1]] * stride[otr[rto[n]-1]]
start at n = otr[n]

index[otr[0]] = i[otr[0]] * stride[otr[0]]
index[n]      = i[n]      * stride[n]      + index[otr[rto[n]-1]]
start at n = otr[n]


