///////////////////////////////////////////////////////////////////////////////
// Order Agnostic
///////////////////////////////////////////////////////////////////////////////

d   := dims
p   := padding 
s   := stepping
i   := index
n   := rank
rto := rank index to ordering (smallest stride dimension to largest stride dimension)
otr := ordering to rank index (smallest rank index to largest rank index)

stride[otr[0]] = s[otr[0]]
stride[i]      = p[otr[rto[i]-1]] + s[i] * d[otr[rto[i]-1]] * stride[otr[rto[i]-1]]
start at i = otr[n-1]

index[otr[0]] = i[otr[0]] * stride[otr[0]]
index[i]      = i[i]      * stride[i]      + index[otr[rto[i]-1]]
start at i = otr[n-1]



// Layout Left/Column Major/Fortran.
// n = 4.
// rto[0, 1, 2, 3], otr[0, 1, 2, 3]
stride[0] = s[0]
stride[1] = p[0] + s[1] * d[0] * stride[0]
stride[2] = p[1] + s[2] * d[1] * stride[1]
stride[3] = p[2] + s[3] * d[2] * stride[2]

stride[0] =                                                                   s[0]
stride[1] =                                             p[0] + s[1] * d[0] * (s[0])
stride[2] =                       p[1] + s[2] * d[1] * (p[0] + s[1] * d[0] * (s[0]))
stride[3] = p[2] + s[3] * d[2] * (p[1] + s[2] * d[1] * (p[0] + s[1] * d[0] * (s[0])))


// Layout Right/Row Major/C++.
// n = 4.
// rto[3, 2, 1, 0], otr[3, 2, 1, 0]
stride[3] = s[3]
stride[2] = p[3] + s[2] * d[3] * stride[3]
stride[1] = p[2] + s[1] * d[2] * stride[2]
stride[0] = p[1] + s[0] * d[1] * stride[1]

stride[3] =                                                                   s[3]
stride[2] =                                             p[3] + s[2] * d[3] * (s[3])
stride[1] =                       p[2] + s[1] * d[2] * (p[3] + s[2] * d[3] * (s[3]))
stride[0] = p[1] + s[0] * d[1] * (p[2] + s[1] * d[2] * (p[3] + s[2] * d[3] * (s[3])))



// Layout Left
0,0 - 1,0 - 2,0 
0,1 - 1,1 - 2,1
 N  -  N  -  N   // Padding

0 - 0,0     <-
1 - 0,1
N elements
2 - 1,0     <-
3 - 1,1
N elements
4 - 2,0
5 - 2,1 

stride[0] = s[0]
          = 1
stride[1] = p[0] + s[1] * d[0] * s[0]
          = N    + 1    * 2    * 1 
          = 2 + N

// Layout Right
0,0 - 0,1 - 0,2 - N // Padding
1,0 - 1,1 - 1,2 - N // Padding

0 - 0,0     <-
1 - 0,1
2 - 0,2
N elements
3 - 1,0     <-
4 - 1,1
5 - 1,2 

stride[1] = s[1]
          = 1
stride[0] = p[1] + s[0] * d[1] * s[1]
          = N    + 1    * 2    * 1
          = 2 + N
