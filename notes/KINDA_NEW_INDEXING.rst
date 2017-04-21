///////////////////////////////////////////////////////////////////////////////
// Layout Left/Column Major/Fortran
///////////////////////////////////////////////////////////////////////////////

d := dims
p := padding
s := stepping
i := index

stride[0] = s[0]
stride[n] = s[n] * (p[n-1] + d[n-1] * stride[n-1])
          = s[n] * p[n-1] + s[n] * d[n-1] * stride[n-1]

stride[0] =                                                                              s[0]
stride[1] =                                           s[1]    p[0] +             s[1]    s[0]d[0]
stride[2] =                s[2]    p[1] +     s[2]    s[1]d[1]p[0] +     s[2]    s[1]d[1]s[0]d[0]
stride[3] = s[3]p[2] + s[3]s[2]d[2]p[1] + s[3]s[2]d[2]s[1]d[1]p[0] + s[3]s[2]d[2]s[1]d[1]s[0]d[0]

stride[0 -> 3] =
    s[3] * (p[2] + d[2] * (                     // stride[3]
        s[2] * (p[1] + d[1] * (                 // stride[2]
            s[1] * (p[0] + d[1] * (             // stride[1]
                s[0]                            // stride[0]
            ))))))

stride[0 -> 3] =
    s[3] * p[2] + s[3] * d[2] * (               // stride[3]
        s[2] * p[1] + s[2] * d[1] * (           // stride[2]
            s[1] * p[0] + s[1] * d[1] * (       // stride[1]
                s[0]                            // stride[0]
            )))

s[3]s[2]d[2]s[1]d[1]s[0]d[0] + s[3]s[2]d[2]s[1]d[1]p[0] + s[3]s[2]d[2]p[1] + s[3]p[2]

index[0] = i[0] * stride[0]
index[n] = i[n] * stride[n] + index[n-1]

NEW index[0 -> 2]
    =                                                                  i[0]            s[0]
    +                        i[1]    s[1]    p[0]                    + i[1]    s[1]    s[0]d[0] 
    + i[2]s[2]        p[1] + i[2]s[2]s[1]d[1]p[0]                    + i[2]s[2]s[1]d[1]s[0]d[0]

OLD index[0 -> 2]
    =                                                                  i[0]            s[0]
    +                        i[1]    s[1]    p[0]                    + i[1]    s[1]    s[0]d[0] 
    + i[2]s[2]s[0]d[0]p[1] + i[2]s[2]s[1]d[1]p[0] + i[2]s[2]p[1]p[0] + i[2]s[2]s[1]d[1]s[0]d[0]

NEW index[0 -> 3]
    =                                                                      i[0]                    s[0]
    +                                       i[1]            s[1]    p[0] + i[1]            s[1]    s[0]d[0]
    +                i[2]    s[2]    p[1] + i[2]    s[2]    s[1]d[1]p[0] + i[2]    s[2]    s[1]d[1]s[0]d[0]
    + i[3]s[3]p[2] + i[3]s[3]s[2]d[2]p[1] + i[3]s[3]s[2]d[2]s[1]d[1]p[0] + i[3]s[3]s[2]d[2]s[1]d[1]s[0]d[0]

///////////////////////////////////////////////////////////////////////////////
// Order Agnostic
///////////////////////////////////////////////////////////////////////////////

d   := dims
p   := padding
s   := stepping
i   := index
rto := rank index to ordering (smallest stride dimension to largest stride dimension)
otr := ordering to rank index (smallest rank index to largest rank index)

stride[otr[0]] = s[otr[0]]
stride[n]      = s[n]      * (p[otr[rto[n]-1]] + d[otr[rto[n]-1]] * stride[otr[rto[n]-1]])
               = s[n]      * p[otr[rto[n]-1]] + s[n] * d[otr[rto[n]-1]] * stride[otr[rto[n]-1]]
start at n = otr[n]

index[otr[0]] = i[otr[0]] * stride[otr[0]]
index[n]      = i[n]      * stride[n] + index[otr[rto[n]-1]]
start at n = otr[n]

NEW stride[0 -> 2], rto[0, 1, 2], otr[0, 1, 2] (Layout Left/Column Major/Fortran)
stride[2] = s[2] * (p[1] + d[1] * s[1] * (p[0] + d[0] * s[0]))
          = s[2]p[1] + s[2]d[1]s[1]p[0] + s[2]d[1]s[1]d[0]s[0]
stride[1] = s[1] * (p[0] + d[0] * s[0])
          = s[1]p[0] + s[1]d[0]s[0]
stride[0] = s[0]

NEW index[0 -> 2], rto[0, 1, 2], otr[0, 1, 2] (Layout Left/Column Major/Fortran)
    = i[2]s[2]p[1] + i[2]s[2]d[1]s[1]p[0] + i[2]s[2]d[1]s[1]d[0]s[0]
    +                i[1]        s[1]p[0] + i[1]        s[1]d[0]s[0]
    +                                       i[0]                s[0]

NEW stride[0 -> 2], rto[2, 1, 0], otr[2, 1, 0] (Layout Right/Row Major/C++)
stride[0] = s[0] * (p[1] + d[1] * s[1] * (p[2] + d[2] * s[2]))
          = s[0]p[1] + s[0]d[1]s[1]p[2] + s[0]d[1]s[1]d[2]s[2]
stride[1] = s[1] * (p[2] + d[2] * s[2])
          = s[1]p[2] + s[1]d[2]s[2]
stride[2] = s[2]

NEW index[0 -> 2], rto[2, 1, 0], otr[2, 1, 0] (Layout Right/Row Major/C++)
    = i[0]s[0]p[1] + i[0]s[0]d[1]s[1]p[2] + i[0]s[0]d[1]s[1]d[2]s[2]
    +                i[1]        s[1]p[2] + i[1]        s[1]d[2]s[2]
    +                                       i[2]                s[2]

///////////////////////////////////////////////////////////////////////////////

NEW index[0 -> 2], rto[0, 1, 2], otr[0, 1, 2] (Layout Left/Column Major/Fortran)
    = i[2]s[2]p[1] + i[2]s[2]d[1]s[1]p[0] + i[2]s[2]d[1]s[1]d[0]s[0]
    +                i[1]        s[1]p[0] + i[1]        s[1]d[0]s[0]
    +                                       i[0]                s[0]

NEW index[0 -> 2], rto[0, 1, 2], otr[0, 1, 2] (Layout Left/Column Major/Fortran)
    = i[2]s[2]p[1] + i[2]s[2]d[1]s[1]p[0] + i[2]s[2]d[1]s[1]d[0]s[0]

    = i[2] s[2] (p[1]  + d[1]s[1](p[0] + d[0]s[0]))



    p[n1] = p[2]
    p[n0] = p[1] + d[1]s[1]p[0]


    p[n1] = p[2]
    p[n0] = p[1]/(d[1]s[1]) + p[0]

    s[n1] = s[2]d[1]s[1]


    p[n1] = p[2]
    p[n0] = p[1] + d[1]s[1]p[0]

    d[n0] = d[1]s[1]d[0]
   

    = i[2] s[2] (p[1]  + d[1]s[1](p[0] + d[0]s[0]))
 
    + i[n1]s[n1](p[n0] + d[n0]s[n0])


    + i[n1]        s[n1]p[n0] + i[n1]        s[n1]d[n0]s[n0]
    +                           i[n0]                  s[n0]

