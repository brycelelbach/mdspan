///////////////////////////////////////////////////////////////////////////////
// Layout Left/Column Major/Fortran
///////////////////////////////////////////////////////////////////////////////

d := dims
p := padding (should have r+1 elements not r elements)
s := stepping
i := index

stride[0] = p[0] + s[0]
stride[n] = p[n] + s[n] * d[n-1] * stride[n-1]

stride[0] =                                                 p[0] + s[0]
stride[1] =                                 p[1] + s[1]d[0](p[0] + s[0])
stride[2] =                 p[2] + s[2]d[1](p[1] + s[1]d[0](p[0] + s[0]))
stride[3] = p[3] + s[3]d[2](p[2] + s[2]d[1](p[1] + s[1]d[0](p[0] + s[0])))

stride[0] =                                                                      p[0] +                         s[0]
stride[1] =                                       p[1] +                 s[1]d[0]p[0] +                 s[1]d[0]s[0]
stride[2] =                p[2] +         s[2]d[1]p[1] +         s[2]d[1]s[1]d[0]p[0] +         s[2]d[1]s[1]d[0]s[0]
stride[3] = p[3] + s[3]d[2]p[2] + s[3]d[2]s[2]d[1]p[1] + s[3]d[2]s[2]d[1]s[1]d[0]p[0] + s[3]d[2]s[2]d[1]s[1]d[0]s[0]

index[0] = i[0] * stride[0]
index[n] = i[n] * stride[n] + index[n-1]

index[3]

    = i[0]                                                (p[0] + s[0])
    + i[1]                                (p[1] + s[1]d[0](p[0] + s[0]))
    + i[2]                (p[2] + s[2]d[1](p[1] + s[1]d[0](p[0] + s[0])))
    + i[3](p[3] + s[3]d[2](p[2] + s[2]d[1](p[1] + s[1]d[0](p[0] + s[0]))))


index[3]

    =                                                          i[0]                        p[0] + i[0]                        s[0]
    +                               i[1]                p[1] + i[1]                s[1]d[0]p[0] + i[1]                s[1]d[0]s[0]
    +            i[2]        p[2] + i[2]        s[2]d[1]p[1] + i[2]        s[2]d[1]s[1]d[0]p[0] + i[2]        s[2]d[1]s[1]d[0]s[0]
    + i[3]p[3] + i[3]s[3]d[2]p[2] + i[3]s[3]d[2]s[2]d[1]p[1] + i[3]s[3]d[2]s[2]d[1]s[1]d[0]p[0] + i[3]s[3]d[2]s[2]d[1]s[1]d[0]s[0]


///////////////////////////////////////////////////////////////////////////////
// Order Agnostic
///////////////////////////////////////////////////////////////////////////////

d   := dims
p   := padding (should have rank + 1 elements not rank elements)
s   := stepping
i   := index
r   := rank of dims, stepping and index
rto := rank index to ordering (smallest stride dimension to largest stride dimension)
otr := ordering to rank index (smallest rank index to largest rank index)

stride[otr[0]] = p[otr[0]] + s[otr[0]]
stride[n]      = p[n]      + s[n]      * d[otr[rto[n]-1]] * stride[otr[rto[n]-1]]
start at n = otr[r-1], e.g. greatest stride]

index[otr[0]] = i[otr[0]] * stride[otr[0]]
index[n]      = i[n]      * stride[n]      + index[otr[rto[n]-1]]
start at n = otr[r-1], e.g. greatest stride


NEW stride[3], rto[0, 1, 2, 3], otr[0, 1, 2, 3] (Layout Left/Column Major/Fortran)

stride[0] = p[otr[0]] + s[otr[0]]
          = p[0]      + s[0]
stride[1] = p[1]      + s[1]     d[otr[rto[1]-1]]stride[otr[rto[1]-1]]
          = p[1]      + s[1]     d[otr[1-1]]     stride[otr[1-1]]
          = p[1]      + s[1]     d[otr[0]]       stride[otr[0]]
          = p[1]      + s[1]     d[0]            stride[0]
stride[2] = p[2]      + s[2]     d[otr[rto[2]-1]]stride[otr[rto[2]-1]]
          = p[2]      + s[2]     d[otr[2-1]]     stride[otr[2-1]]
          = p[2]      + s[2]     d[otr[1]]       stride[otr[1]]
          = p[2]      + s[2]     d[1]            stride[1]
stride[3] = p[3]      + s[3]     d[otr[rto[3]-1]]stride[otr[rto[3]-1]]
          = p[3]      + s[3]     d[otr[3-1]]     stride[otr[3-1]]
          = p[3]      + s[3]     d[otr[2]]       stride[otr[2]]
          = p[3]      + s[3]     d[2]            stride[2]

stride[0] =                                                 p[0] + s[0]
stride[1] =                                 p[1] + s[1]d[0](p[0] + s[0])
stride[2] =                 p[2] + s[2]d[1](p[1] + s[1]d[0](p[0] + s[0]))
stride[3] = p[3] + s[3]d[2](p[2] + s[2]d[1](p[1] + s[1]d[0](p[0] + s[0])))

stride[0] =                                                                      p[0] +                         s[0]
stride[1] =                                       p[1] +                 s[1]d[0]p[0] +                 s[1]d[0]s[0]
stride[2] =                p[2] +         s[2]d[1]p[1] +         s[2]d[1]s[1]d[0]p[0] +         s[2]d[1]s[1]d[0]s[0]
stride[3] = p[3] + s[3]d[2]p[2] + s[3]d[2]s[2]d[1]p[1] + s[3]d[2]s[2]d[1]s[1]d[0]p[0] + s[3]d[2]s[2]d[1]s[1]d[0]s[0]


NEW index[3], rto[0, 1, 2, 3], otr[0, 1, 2, 3] (Layout Left/Column Major/Fortran)

    = i[otr[0]]stride[otr[0]] 
    + i[1]     stride[1]
    + i[2]     stride[2]
    + i[3]     stride[3] 

    = i[0]                                                (p[0] + s[0])
    + i[1]                                (p[1] + s[1]d[0](p[0] + s[0]))
    + i[2]                (p[2] + s[2]d[1](p[1] + s[1]d[0](p[0] + s[0])))
    + i[3](p[3] + s[3]d[2](p[2] + s[2]d[1](p[1] + s[1]d[0](p[0] + s[0]))))

    = i[0]                                                                     (p[0] +                         s[0])
    + i[1]                                      (p[1] +                 s[1]d[0]p[0] +                 s[1]d[0]s[0])
    + i[2]               (p[2] +         s[2]d[1]p[1] +         s[2]d[1]s[1]d[0]p[0] +         s[2]d[1]s[1]d[0]s[0])
    + i[3](p[3] + s[3]d[2]p[2] + s[3]d[2]s[2]d[1]p[1] + s[3]d[2]s[2]d[1]s[1]d[0]p[0] + s[3]d[2]s[2]d[1]s[1]d[0]s[0])

    =                                                          i[0]                        p[0] + i[0]                        s[0]
    +                               i[1]                p[1] + i[1]                s[1]d[0]p[0] + i[1]                s[1]d[0]s[0]
    +            i[2]        p[2] + i[2]        s[2]d[1]p[1] + i[2]        s[2]d[1]s[1]d[0]p[0] + i[2]        s[2]d[1]s[1]d[0]s[0]
    + i[3]p[3] + i[3]s[3]d[2]p[2] + i[3]s[3]d[2]s[2]d[1]p[1] + i[3]s[3]d[2]s[2]d[1]s[1]d[0]p[0] + i[3]s[3]d[2]s[2]d[1]s[1]d[0]s[0]


NEW stride[3], rto[3, 2, 1, 0], otr[3, 2, 1, 0] (Layout Right/Row Major/C++)

stride[3] = p[otr[0]] + s[otr[0]]
          = p[3]      + s[3]
stride[2] = p[2]      + s[2]     d[otr[rto[2]-1]]stride[otr[rto[2]-1]]
          = p[2]      + s[2]     d[otr[1-1]]     stride[otr[1-1]]
          = p[2]      + s[2]     d[otr[0]]       stride[otr[0]]
          = p[2]      + s[2]     d[3]            stride[3]
stride[1] = p[1]      + s[1]     d[otr[rto[1]-1]]stride[otr[rto[1]-1]]
          = p[1]      + s[1]     d[otr[2-1]]     stride[otr[2-1]]
          = p[1]      + s[1]     d[otr[1]]       stride[otr[1]]
          = p[1]      + s[1]     d[2]            stride[2]
stride[0] = p[0]      + s[0]     d[otr[rto[0]-1]]stride[otr[rto[0]-1]]
          = p[0]      + s[0]     d[otr[3-1]]     stride[otr[3-1]]
          = p[0]      + s[0]     d[otr[2]]       stride[otr[2]]
          = p[0]      + s[0]     d[1]            stride[1]

stride[3] =                                                 p[3] + s[3]
stride[2] =                                 p[2] + s[2]d[3](p[3] + s[3])
stride[1] =                 p[1] + s[1]d[2](p[2] + s[2]d[3](p[3] + s[3]))
stride[0] = p[0] + s[0]d[1](p[1] + s[1]d[2](p[2] + s[2]d[3](p[3] + s[3])))

stride[3] =                                                                      p[3] +                         s[3]
stride[2] =                                       p[2] +                 s[2]d[3]p[3] +                 s[2]d[3]s[3]
stride[1] =                p[1] +         s[1]d[2]p[2] +         s[1]d[2]s[2]d[3]p[3] +         s[1]d[2]s[2]d[3]s[3]
stride[0] = p[0] + s[0]d[1]p[1] + s[0]d[1]s[1]d[2]p[2] + s[0]d[1]s[1]d[2]s[2]d[3]p[3] + s[0]d[1]s[1]d[2]s[2]d[3]s[3]


NEW index[3], rto[3, 2, 1, 0], otr[3, 2, 1, 0] (Layout Right/Row Major/C++)

    = i[otr[0]]stride[otr[0]] 
    + i[2]     stride[2]
    + i[1]     stride[1]
    + i[0]     stride[0] 

    = i[3]                                                (p[3] + s[3])
    + i[2]                                (p[2] + s[2]d[3](p[3] + s[3]))
    + i[1]                (p[1] + s[1]d[2](p[2] + s[2]d[3](p[3] + s[3])))
    + i[0](p[0] + s[0]d[1](p[1] + s[1]d[2](p[2] + s[2]d[3](p[3] + s[3]))))

    = i[3]                                                                     (p[3] +                         s[3])
    + i[2]                                      (p[2] +                 s[2]d[3]p[3] +                 s[2]d[3]s[3])
    + i[1]               (p[1] +         s[1]d[2]p[2] +         s[1]d[2]s[2]d[3]p[3] +         s[1]d[2]s[2]d[3]s[3])
    + i[0](p[0] + s[0]d[1]p[1] + s[0]d[1]s[1]d[2]p[2] + s[0]d[1]s[1]d[2]s[2]d[3]p[3] + s[0]d[1]s[1]d[2]s[2]d[3]s[3])

    =                                                          i[3]                        p[3] + i[3]                        s[3]
    +                               i[2]                p[2] + i[2]                s[2]d[3]p[3] + i[2]                s[2]d[3]s[3]
    +            i[1]        p[1] + i[1]        s[1]d[2]p[2] + i[1]        s[1]d[2]s[2]d[3]p[3] + i[1]        s[1]d[2]s[2]d[3]s[3]
    + i[0]p[0] + i[0]s[0]d[1]p[1] + i[0]s[0]d[1]s[1]d[2]p[2] + i[0]s[0]d[1]s[1]d[2]s[2]d[3]p[3] + i[0]s[0]d[1]s[1]d[2]s[2]d[3]s[3]

///////////////////////////////////////////////////////////////////////////////
// Slicing 
///////////////////////////////////////////////////////////////////////////////

NEW index[2], rto[0, 1, 2], otr[0, 1, 2] (Layout Left/Column Major/Fortran)

    =                             i[0]                  p[0] + i[0]                  s[0]
    + i[1](               p[1]) + i[1](        s[1])d[0]p[0] + i[1](        s[1])d[0]s[0]
    + i[2](p[2] + s[2]d[1]p[1]) + i[2](s[2]d[1]s[1])d[0]p[0] + i[2](s[2]d[1]s[1])d[0]s[0]


NEW index[3], rto[0, 1, 2, 3], otr[0, 1, 2, 3] (Layout Left/Column Major/Fortran)

    =                                                          i[0]                          p[0] + i[0]                          s[0]
    +                             i[1]                  p[1] + i[1]                  s[1]d[0]p[0] + i[1]                  s[1]d[0]s[0]
    + i[2](               p[2]) + i[2](        s[2])d[1]p[1] + i[2](        s[2])d[1]s[1]d[0]p[0] + i[2](        s[2])d[1]s[1]d[0]s[0]
    + i[3](p[3] + s[3]d[2]p[2]) + i[3](s[3]d[2]s[2])d[1]p[1] + i[3](s[3]d[2]s[2])d[1]s[1]d[0]p[0] + i[3](s[3]d[2]s[2])d[1]s[1]d[0]s[0]


NEW index[2], rto[2, 1, 0], otr[2, 1, 0] (Layout Right/Row Major/C++)

    =                             i[2]                  p[2] + i[2]                  s[2]
    + i[1](               p[1]) + i[1](        s[1])d[2]p[2] + i[1](        s[1])d[2]s[2]
    + i[0](p[0] + s[0]d[1]p[1]) + i[0](s[0]d[1]s[1])d[2]p[2] + i[0](s[0]d[1]s[1])d[2]s[2]


NEW index[3], rto[3, 2, 1, 0], otr[3, 2, 1, 0] (Layout Right/Row Major/C++)

    =                                                          i[3]                          p[3] + i[3]                          s[3]
    +                             i[2]                  p[2] + i[2]                  s[2]d[3]p[3] + i[2]                  s[2]d[3]s[3]
    + i[1](               p[1]) + i[1](        s[1])d[2]p[2] + i[1](        s[1])d[2]s[2]d[3]p[3] + i[1](        s[1])d[2]s[2]d[3]s[3]
    + i[0](p[0] + s[0]d[1]p[1]) + i[0](s[0]d[1]s[1])d[2]p[2] + i[0](s[0]d[1]s[1])d[2]s[2]d[3]p[3] + i[0](s[0]d[1]s[1])d[2]s[2]d[3]s[3]


slice[k]

if otr[rto[k]+1] > k
    for rto[m] < rto[k]
    d'[m] = d[m]
    p'[m] = p[m]
    s'[m] = s[m]
    i'[m] = i[m]

    for rto[m] == rto[k] 
    d'[m] = d[otr[rto[m]+1]]
    p'[m] = p[otr[rto[m]+1]] + s[otr[rto[m]+1]]d[m]p[m]
    s'[m] = s[otr[rto[m]+1]]d[m]s[m]
    i'[m] = i[otr[rto[m]+1]]

    for rto[m] > rto[k]
    d'[m] = d[otr[rto[m]+1]]
    p'[m] = p[otr[rto[m]+1]]
    s'[m] = s[otr[rto[m]+1]]
    i'[m] = i[otr[rto[m]+1]]
else // otr[rto[k]+1] < k
    for rto[m] < (rto[k]+1)
    d'[m] = d[otr[rto[m]+1]]
    p'[m] = p[otr[rto[m]+1]]
    s'[m] = s[otr[rto[m]+1]]
    i'[m] = i[otr[rto[m]+1]]

    for rto[m] == (rto[k]+1)
    d'[m] = d[m]
    p'[m] = p[m] + s[m]d[otr[rto[m]+1]]p[otr[rto[m]+1]]
    s'[m] = s[m]d[otr[rto[m]+1]]s[otr[rto[m]+1]]
    i'[m] = i[m]

    for rto[m] > (rto[k]+1)
    d'[m] = d[m]
    p'[m] = p[m]
    s'[m] = s[m]
    i'[m] = i[m]

slice[1], rto[0, 1, 2, 3], otr[0, 1, 2, 3] (Layout Left/Column Major/Fortran)

rto[m] < rto[1]: 0
rto[m] > rto[1]: 2, 3

d'[0] = d[0]
p'[0] = p[0]
s'[0] = s[0]
i'[0] = i[0]
d'[1] = d[2]
p'[1] = p[2] + s[2]d[1]p[1]
s'[1] = s[2]d[1]s[1]
i'[1] = i[2]
d'[2] = d[3]
p'[2] = p[3]
s'[2] = s[3]
i'[2] = i[3]
p'[3] = p[4]

    =                                                     i'[0]                           p'[0] + i'[0]                           s'[0]
    +              i'[1]          p'[1]                 + i'[1]          s'[1]       d'[0]p'[0] + i'[1]          s'[1]       d'[0]s'[0]
    + i'[2]p'[2] + i'[2]s'[2]d'[1]p'[1]                 + i'[2]s'[2]d'[1]s'[1]       d'[0]p'[0] + i'[2]s'[2]d'[1]s'[1]       d'[0]s'[0]

    =                                                     i [0]                           p [0] + i [0]                           s [0]
    +              i [2]          (p[2] + s[2]d[1]p[1]) + i [2]          s[2]d[1]s[1]d [0]p [0] + i [2]          s[2]d[1]s[1]d [0]s [0]
    + i [3]p [3] + i [3]s [3]d [2](p[2] + s[2]d[1]p[1]) + i [3]s [3]d [2]s[2]d[1]s[1]d [0]p [0] + i [3]s [3]d [2]s[2]d[1]s[1]d [0]s [0]

    =                                                          i[0]                        p[0] + i[0]                        s[0]
    +            i[2]        p[2] + i[2]        s[2]d[1]p[1] + i[2]        s[2]d[1]s[1]d[0]p[0] + i[2]        s[2]d[1]s[1]d[0]s[0]
    + i[3]p[3] + i[3]s[3]d[2]p[2] + i[3]s[3]d[2]s[2]d[1]p[1] + i[3]s[3]d[2]s[2]d[1]s[1]d[0]p[0] + i[3]s[3]d[2]s[2]d[1]s[1]d[0]s[0]

REFERENCE (FOR COMPARISON):

    =                                                          i[0]                        p[0] + i[0]                        s[0]
    +            i[2]        p[2] + i[2]        s[2]d[1]p[1] + i[2]        s[2]d[1]s[1]d[0]p[0] + i[2]        s[2]d[1]s[1]d[0]s[0]
    + i[3]p[3] + i[3]s[3]d[2]p[2] + i[3]s[3]d[2]s[2]d[1]p[1] + i[3]s[3]d[2]s[2]d[1]s[1]d[0]p[0] + i[3]s[3]d[2]s[2]d[1]s[1]d[0]s[0]


slice[1], rto[3, 2, 1, 0], otr[3, 2, 1, 0] (Layout Right/Row Major/C++)

rto[m] < (rto[1]+1): 1, 2, 3 
rto[m] > (rto[1]+1):

d'[0] = d[0]
p'[0] = p[0] + s[0]d[1]p[1]
s'[0] = s[0]d[1]s[1]
i'[0] = i[0]
d'[1] = d[2]
p'[1] = p[2]
s'[1] = s[2]
i'[1] = i[2]
d'[2] = d[3]
p'[2] = p[3]
s'[2] = s[3]
i'[2] = i[3]
p'[3] = p[4]


slice[2], rto[3, 2, 1, 0], otr[3, 2, 1, 0] (Layout Right/Row Major/C++)

rto[m] < (rto[2]+1): 2, 3 
rto[m] > (rto[2]+1): 0

d'[0] = d[0]
p'[0] = p[0]
s'[0] = s[0]
i'[0] = i[0]
d'[1] = d[1]
p'[1] = p[1] + s[1]d[2]p[2]
s'[1] = s[1]d[2]s[2]
i'[1] = i[1]
d'[2] = d[3]
p'[2] = p[3]
s'[2] = s[3]
i'[2] = i[3]
p'[3] = p[4]

    =                                                     i'[2]                           p'[2] + i'[2]                           s'[2]
    +              i'[1]          p'[1]                 + i'[1]          s'[1]       d'[2]p'[2] + i'[1]          s'[1]       d'[2]s'[2]
    + i'[0]p'[0] + i'[0]s'[0]d'[1]p'[1]                 + i'[0]s'[0]d'[1]s'[1]       d'[2]p'[2] + i'[0]s'[0]d'[1]s'[1]       d'[2]s'[2]

    =                                                     i [3]                           p [3] + i [3]                           s [3]
    +              i [1]          (p[1] + s[1]d[2]p[2]) + i [1]          s[1]d[2]s[2]d [3]p [3] + i [1]          s[1]d[2]s[2]d [3]s [3]
    + i [0]p [0] + i [0]s [0]d [1](p[1] + s[1]d[2]p[2]) + i [0]s [0]d [1]s[1]d[2]s[2]d [3]p [3] + i [0]s [0]d [1]s[1]d[2]s[2]d [3]s [3]

    =                                                          i[3]                        p[3] + i[3]                        s[3]
    +            i[1]        p[1] + i[1]        s[1]d[2]p[2] + i[1]        s[1]d[2]s[2]d[3]p[3] + i[1]        s[1]d[2]s[2]d[3]s[3]
    + i[0]p[0] + i[0]s[0]d[1]p[1] + i[0]s[0]d[1]s[1]d[2]p[2] + i[0]s[0]d[1]s[1]d[2]s[2]d[3]p[3] + i[0]s[0]d[1]s[1]d[2]s[2]d[3]s[3]

REFERENCE (FOR COMPARISON):

    =                                                          i[3]                        p[3] + i[3]                        s[3]
    +            i[1]        p[1] + i[1]        s[1]d[2]p[2] + i[1]        s[1]d[2]s[2]d[3]p[3] + i[1]        s[1]d[2]s[2]d[3]s[3]
    + i[0]p[0] + i[0]s[0]d[1]p[1] + i[0]s[0]d[1]s[1]d[2]p[2] + i[0]s[0]d[1]s[1]d[2]s[2]d[3]p[3] + i[0]s[0]d[1]s[1]d[2]s[2]d[3]s[3]


