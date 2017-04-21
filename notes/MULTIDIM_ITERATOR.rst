// Define the performance model for regular layout mappings.

// Ideas:
// * Mask away bits? E.g. just increment everything, and then mask? Doesn't work.

// Layout Left/Column Major/Fortran
int idx = i0                        
        + i1*d0
        + i2*d1*d0
        + i3*d2*d1*d0;

///////////////////////////////////////////////////////////////////////////////

idx = i[0] + i[1]*d[0] + i[2]*d[1]*d[0];

K   | U
---------
d   | i
idx |

for (int k = 2; k >= 0; --k)
  i[k] = idx / stride<k>();
  idx = idx % stride<k>();

///////////////////////////////////////////////////////////////////////////////

for (int i3 = 0; i3 < d3; ++i3)
{
    auto* p3 = &bp[i3*d2*d1*d0];
 
    for (int i2 = 0; i2 < d2; ++i2)
    {
        auto* p2 = &p3[i2*d1*d0];

        for (int i1 = 0; i1 < d1; ++i1)
        {
            auto* p1 = &p2[i1*d0];

            for (int i0 = 0; i0 < d0; ++i0)
            {
                auto* p0 = &p1[i0];
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////

for (int i3 = 0; i3 < d3; ++i3) {
    auto* p3 = &bp[i3*d2*d1*d0];

    for (int i2 = 0; i2 < d2; ++i2) {
        auto* p2 = &p3[i2*d1*d0];

        for (int i1 = 0; i1 < d1; ++i1) {
            auto* p1 = &p2[i1*d0];

            for (int i0 = 0; i0 < d0; ++i0) {
                auto* p0 = &p1[i0];
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////

for (int i3 = 0; i3 < d3; ++i3) {
    auto* p3 = bp + i3*d2*d1*d0;
 
    for (int i2 = 0; i2 < d2; ++i2) {
        auto* p2 = p3 + i2*d1*d0;

        for (int i1 = 0; i1 < d1; ++i1) {
            auto* p1 = p2 + i1*d0;

            for (int i0 = 0; i0 < d0; ++i0) {
                auto* p0 = p1 + i0;
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////

for (int i3 = 0; i3 < d[3]; ++i3) {
    auto* p3 = bp + i3*d[2]*d[1]*d[0];
 
    for (int i2 = 0; i2 < d[2]; ++i2) {
        auto* p2 = p3 + i2*d[1]*d[0];
        
        for (int i1 = 0; i1 < d[1]; ++i1) {
            auto* p1 = p2 + i1*d[0];

            for (int i0 = 0; i0 < d[0]; ++i0) {
                auto* p0 = p1 + i0;

                f(*p0, i0, i1, i2, i3);
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////

for (int i[n] = 0; i[n] < d[n]; ++i[n]) {
  auto* p[n] = bp + i[n]*d[n-1]*d[n-2]*...*d[1]*d[0];
                      // |--------stride<n>()--------|

  for (int i[n-1] = 0; i[n-1] < d[n-1]; ++i[n-1]) {
    auto* p[n-1] = p[n  ] + i[n-1]*d[n-2]*d[n-3]*...*d[1]*d[0];
                                // |-------stride<n-1>()-------|

    // ...

      for (int i[1] = 0; i[1] < d[1]; ++i[1]) {
        auto* p[1] = p[2] + i[1]*d[0];
                              // |stride<1>()|

        for (int i[0] = 0; i[0] < d[0]; ++i[0]) {
          auto* p[0] = p[1] + i[0];

          f(*p[0], i);
        }
      }
    // ...
  }
}

///////////////////////////////////////////////////////////////////////////////

for (int i[n] = 0; i[n] < d[n]; ++i[n]) {
  auto* p[n] = bp + i[n]*stride<n>();

  for (int i[n-1] = 0; i[n-1] < d[n-1]; ++i[n-1]) {
    auto* p[n-1] = p[n  ] + i[n-1]*stride<n-1>();

    // ...

      for (int i[1] = 0; i[1] < d[1]; ++i[1]) {
        auto* p[1] = p[2] + i[1]*stride<1>();

        for (int i[0] = 0; i[0] < d[0]; ++i[0]) {
          auto* p[0] = p[1] + i[0]*stride<0>();

          f(*p[0], i);
        }
      }
    // ...
  }
}

///////////////////////////////////////////////////////////////////////////////

for (int i[n  ] = 0; i[n  ] < d[n  ]; ++i[n  ]) {
  int idx[n  ] =            i[n  ]*stride[n  ];

  for (int i[n-1] = 0; i[n-1] < d[n-1]; ++i[n-1]) {
    int idx[n-1] = idx[n  ] + i[n-1]*stride<n-1>();

    // ...

      for (int i[1  ] = 0; i[1  ] < d[1  ]; ++i[1  ]) {
        int idx[1  ] = idx[2  ] + i[1  ]*stride[1  ];

        for (int i[0  ] = 0; i[0  ] < d[0  ]; ++i[0  ]) {
          int idx[0  ] = idx[1  ] + i[0  ]*stride[0  ];

          f(bp[idx[0]], i);
        }
      }
    // ...
  }
}

///////////////////////////////////////////////////////////////////////////////

for (int i[n  ] = 0; i[n  ] < d[n  ]; ++i[n  ]) {
  int idx[n  ] =            i[n  ]*stride[n  ];

  for (int i[n-1] = 0; i[n-1] < d[n-1]; ++i[n-1]) {
    int idx[n-1] = idx[n  ] + i[n-1]*stride<n-1>();

    // ...

      for (int i[1  ] = 0; i[1  ] < d[1  ]; ++i[1  ]) {
        int idx[1  ] = idx[2  ] + i[1  ]*stride[1  ];

        for (int i[0  ] = 0; i[0  ] < d[0  ]; ++i[0  ]) {
          int idx[0  ] = idx[1  ] + i[0  ]*stride[0  ];

          f(bp[idx[0]], i);
        }
      }
    // ...
  }
}

///////////////////////////////////////////////////////////////////////////////

for ( init-statement condition ; expression ) statement

// is equivalent to

{
  init-statement
  while ( condition ) {
    statement
    expression;
  }
}


while ( condition ) statement

// is equivalent to 

label:
{
  if ( condition ) {
    statement
    goto label;
  }
}


// Thus

for ( init-statement condition ; expression ) statement

// is equivalent to

init-statement
label:
{
  if ( condition ) {
    statement
    expression ;
    goto label;
  }
}

///////////////////////////////////////////////////////////////////////////////

for (int i[n  ] = 0; i[n  ] < d[n  ]; ++i[n  ]) {
  int idx[n  ] =            i[n  ]*stride[n  ];

  // ...
}

init-statment :=
  int i[n  ] = 0;
condition :=
  i[n  ] < d[n  ]
expression :=
  ++i[n  ]
statement :=
  int idx[n  ] =            i[n  ]*stride[n  ];

  // ...

int i[n  ] = 0;
label[n  ]:
{
  if ( i[n  ] < d[n  ] ) {
    int idx[n  ] =            i[n  ]*stride[n  ];

    // ...

    ++i[n  ] ;
    goto label[n  ];
  }
}

///////////////////////////////////////////////////////////////////////////////

int i[n  ] = 0;
label[n  ]:
{
  if ( i[n  ] < d[n  ] ) {
    int idx[n  ] =            i[n  ]*stride[n  ];

    // ...

    ++i[n  ] ;
    goto label[n  ];
  }
}

///////////////////////////////////////////////////////////////////////////////

int i[n  ] = 0;
label[n  ]:
{
  if ( i[n  ] < d[n  ] ) {
    int idx[n  ] =            i[n  ]*stride[n  ];

    int i[n-1] = 0;
    label[n-1]:
    {
      if ( i[n-1] < d[n-1] ) {
        int idx[n-1] = idx[n  ] + i[n-1]*stride<n-1>();

        // ...

          int i[1] = 0;
          label[1]:
          {
            if ( i[1] < d[1] ) {
              int idx[1  ] = idx[2  ] + i[1  ]*stride[1  ];

              int i[0] = 0;
              label[0]:
              {
                if ( i[0] < d[0] ) {
                  int idx[0  ] = idx[1  ] + i[0  ]*stride[0  ];

                  f(bp[idx[0]], i);

                  ++i[0] ;
                  goto label[0];
                }
              }

              ++i[1] ;
              goto label[1];
            }
          }

        // ... 

        ++i[n-1] ;
        goto label[n-1];
      }
    }

    ++i[n  ] ;
    goto label[n  ];
  }
}

///////////////////////////////////////////////////////////////////////////////

for (int i[n] = 0; i[n] < d[n]; ++i[n]) {
  int idx[n] = i[n]*stride<n>();

  // ...

    for (int i[k] = 0; i[k] < d[k]; ++i[k]) {
      int idx[k] = idx[k+1] + i[k]*stride<k>();

      // ...

        for (int i[1] = 0; i[1] < d[1]; ++i[1]) {
          int idx[1] = idx[2] + i[1]*stride<1>();

          for (int i[0] = 0; i[0] < d[0]; ++i[0]) {
            int idx[0] = idx[1] + i[0]*stride<0>();

            f(bp[idx[0]], i);
          } // 0
        } // 1
      // ...
    } // k
  // ...
} // n

///////////////////////////////////////////////////////////////////////////////

if ( condition ) stateent

// is equivalent to 

bool r = condition ;
branch r, label;
{
  statement ;
}
label:

///////////////////////////////////////////////////////////////////////////////

int i[n] = 0;
label_for[n]:
{
  bool r = i[n] < d[n];                               // 2 LOADS, 1 COMPARE
  branch r, label_if[n];                              // 1 CONDITIONAL BRANCH
  {
    int idx[n] = i[n]*stride<n>();                      // 2 LOADS, 1 MULTIPLY, 1 STORE

    // ...

    ++i[n];                                           // 1 LOAD, 1 ADD, 1 STORE 
    goto label_for[n];                                // 1 UNCONDITIONAL BRANCH
  }
  label_if[n]:
}

///////////////////////////////////////////////////////////////////////////////

int i[n] = 0;
label_for[n]:
{
  bool r = i[n] < d[n];                               // 2 LOADS, 1 COMPARE
  branch r, label_if[n];                              // 1 CONDITIONAL BRANCH
  {
    int idx[n] = i[n]*stride<n>();                      // 2 LOADS, PM(stride<n>()), 1 MULTIPLY, 1 STORE

    // ...

      int i[k] = 0;
      label_for[k]:
      {
        bool r = i[k] < d[k];                         // 2 LOADS, 1 COMPARE
        branch r, label_if[k];                        // 1 CONDITIONAL BRANCH
        {
          int idx[k] = idx[k+1] + i[k]*stride<k>();     // 3 LOADS, PM(stride<k>()), 1 MULTIPLY, 1 ADD, 1 STORE

          // ...

            int i[0] = 0;
            label_for[0]:
            {
              bool r = i[0] < d[0];                   // 2 LOADS, 1 COMPARE
              branch r, label_if[0];                  // 1 CONDITIONAL BRANCH
              {
                int idx[0] = idx[1] + i[0]*stride<0>(); // 3 LOADS, PM(stride<0>()), 1 MULTIPLY, 1 ADD, 1 STORE

                auto& e = bp[idx[0]];                 // 1 LOAD, 1 INDIRECT LOAD

                f(e, i);                              // PM(f(e, i))

                ++i[0];                               // 1 LOAD, 1 ADD, 1 STORE 
                goto label_for[0];                    // 1 UNCONDITIONAL BRANCH
              } 
              label_if[0]:
            }
          // ... 
          ++i[k];                                     // 1 LOAD, 1 ADD, 1 STORE 
          goto label_for[k];                          // 1 UNCONDITIONAL BRANCH
        } 
        label_if[k]:
      }
    // ... 
    ++i[n];                                           // 1 LOAD, 1 ADD, 1 STORE 
    goto label_for[n];                                // 1 UNCONDITIONAL BRANCH
  }
  label_if[n]:
}

///////////////////////////////////////////////////////////////////////////////

  int i[n] = 0;
label_for[n]:
  bool r = i[n] < d[n];                   // 2 LOADS, 1 COMPARE
  branch r, label_if[n];                  // 1 CONDITIONAL BRANCH
  int idx[n] = i[n]*stride<n>();            // 2 LOADS, 1 MULTIPLY, 1 STORE
  // ...
  ++i[n];                                 // 1 LOAD, 1 ADD, 1 STORE 
  goto label_for[n];                      // 1 UNCONDITIONAL BRANCH
label_if[n]:

///////////////////////////////////////////////////////////////////////////////

  int i[n] = 0;
label_for[n]:
  bool r = i[n] < d[n];                   // 2 LOADS, 1 COMPARE
  branch r, label_if[n];                  // 1 CONDITIONAL BRANCH
  int idx[n] = i[n]*stride<n>();            // 2 LOADS, PM(stride<n>()), 1 MULTIPLY, 1 STORE
  // ...
  int i[k] = 0;
label_for[k]:
  bool r = i[k] < d[k];                   // 2 LOADS, 1 COMPARE
  branch r, label_if[k];                  // 1 CONDITIONAL BRANCH
  int idx[k] = idx[k+1] + i[k]*stride<k>(); // 3 LOADS, PM(stride<k>()), 1 MULTIPLY, 1 ADD, 1 STORE
  // ...
  int i[0] = 0;
label_for[0]:
  bool r = i[0] < d[0];                   // 2 LOADS, 1 COMPARE
  branch r, label_if[0];                  // 1 CONDITIONAL BRANCH
  int idx[0] = idx[1] + i[0]*stride<0>();   // 3 LOADS, PM(stride<0>()), 1 MULTIPLY, 1 ADD, 1 STORE
  auto& e = bp[idx[0]];                   // 1 LOAD, 1 INDIRECT LOAD
  f(e, i);                                // PM(f(e, i))
  ++i[0];                                 // 1 LOAD, 1 ADD, 1 STORE 
  goto label_for[0];                      // 1 UNCONDITIONAL BRANCH
label_if[0]:
  // ... 
  ++i[k];                                 // 1 LOAD, 1 ADD, 1 STORE 
  goto label_for[k];                      // 1 UNCONDITIONAL BRANCH
label_if[k]:
  // ... 
  ++i[n];                                 // 1 LOAD, 1 ADD, 1 STORE 
  goto label_for[n];                      // 1 UNCONDITIONAL BRANCH
label_if[n]:

///////////////////////////////////////////////////////////////////////////////

  int i[n] = 0;
label_for[n]:
  bool r = i[n] < d[n];                     // 2 LOADS, 1 COMPARE
  branch r, label_if[n];                    // 1 CONDITIONAL BRANCH
  int idx[n] = i[n]*stride<n>();            // 2 LOADS, PM(stride<k>()), 1 MULTIPLY, 1 STORE
  // ...
  int i[k] = 0;
label_for[k]:
  bool r = i[k] < d[k];                     // 2 LOADS, 1 COMPARE
  branch r, label_if[k];                    // 1 CONDITIONAL BRANCH
  int idx[k] = idx[k+1] + i[k]*stride<k>(); // 3 LOADS, PM(stride<k>()), 1 MULTIPLY, 1 ADD, 1 STORE
  // ...
  int i[0] = 0;
label_for[0]:
  bool r = i[0] < d[0];                     // 2 LOADS, 1 COMPARE
  branch r, label_if[0];                    // 1 CONDITIONAL BRANCH
  int idx[0] = idx[1] + i[0]*stride<0>();   // 3 LOADS, PM(stride<0>()), 1 MULTIPLY, 1 ADD, 1 STORE
  auto& e = bp[idx[0]];                     // 1 LOAD, 1 INDIRECT LOAD
 kf(e, i);                                  // PM(f(e, i))
  ++i[0];                                   // 1 LOAD, 1 ADD, 1 STORE 
  goto label_for[0];                        // 1 UNCONDITIONAL BRANCH
label_if[0]:
  // ... 
  ++i[k];                                   // 1 LOAD, 1 ADD, 1 STORE 
  goto label_for[k];                        // 1 UNCONDITIONAL BRANCH
label_if[k]:
  // ... 
  ++i[n];                                   // 1 LOAD, 1 ADD, 1 STORE 
  goto label_for[n];                        // 1 UNCONDITIONAL BRANCH
label_if[n]:

///////////////////////////////////////////////////////////////////////////////

// Super conservative model

n:  int i[n] = 0;
n:label_for[n]:
n:  bool r = i[n] < d[n];                     // 2 LOADS (i[n], d[n]), 1 COMPARE
n:  branch r, label_if[n];                    // 1 CONDITIONAL BRANCH
n:  int str[n] = stride<n>();                 // 1 CALL TO stride<n>()
n:  int idx[n] = i[n]*str[n];                 // 1 LOAD (i[n]), MULTIPLY, 1 STORE (idx[n])
    // ...
k:  int i[k] = 0;
k:label_for[k]:
k:  bool r = i[k] < d[k];                     // 2 LOADS (i[k], d[k]), 1 COMPARE
k:  branch r, label_if[k];                    // 1 CONDITIONAL BRANCH
k:  int str[k] = stride<k>();                 // 1 CALL TO stride<k>()
k:  int tmp[k] = i[k]*str[k];                 // 1 LOAD (i[k]), 1 MULTIPLY
k:  int idx[k] = idx[k+1] + tmp[k];           // 1 LOAD (idx[k+1]), 1 ADD, 1 STORE (idx[k])
    // ...
0:  int i[0] = 0;
0:label_for[0]:
0:  bool r = i[0] < d[0];                     // 2 LOADS (i[0], d[0]), 1 COMPARE
0:  branch r, label_if[0];                    // 1 CONDITIONAL BRANCH
0:  int str[0] = stride<0>();                 // 1 CALL TO stride<0>()
0:  int tmp[0] = i[0]*str[0];                 // 1 LOAD (i[0]), 1 MULTIPLY
0:  int idx[0] = idx[1] + tmp[0];             // 1 LOAD (idx[0]), 1 ADD, 1 STORE (idx[0])
0:  auto& e = bp[idx[0]];                     // 1 LOAD (idx[0]), 1 INDIRECT LOAD (bp[idx[0]])
0:  f(e, i);                                  // 1 CALL TO f(e, i)
0:  ++i[0];                                   // 1 LOAD (i[0], 1 ADD, 1 STORE (i[0]) 
0:  goto label_for[0];                        // 1 UNCONDITIONAL BRANCH
0:label_if[0]:
    // ... 
k:  ++i[k];                                   // 1 LOAD (i[k], 1 ADD, 1 STORE (i[k])
k:  goto label_for[k];                        // 1 UNCONDITIONAL BRANCH
k:label_if[k]:
    // ... 
n:  ++i[n];                                   // 1 LOAD (i[n], 1 ADD, 1 STORE (i[n])
n:  goto label_for[n];                        // 1 UNCONDITIONAL BRANCH
n:label_if[n]:

///////////////////////////////////////////////////////////////////////////////

// Super conservative model
// - In the statement blocks, i[k]s are assumed not to spill 

n  :  int i[n] = 0;
n  :label_for[n]:
n  :  bool r = i[n] < d[n];                     // 1 LOAD (d[n]), 1 COMPARE
n  :  branch r, label_if[n];                    // 1 CONDITIONAL BRANCH
n  :  int str[n] = stride<n>();                 // 1 CALL TO stride<n>()
n  :  int idx[n] = i[n]*str[n];                 // 1 MULTIPLY, 1 STORE (idx[n])
      // ...
k  :  int i[k] = 0;
k  :label_for[k]:
k  :  bool r = i[k] < d[k];                     // 1 LOAD (d[k]), 1 COMPARE
k  :  branch r, label_if[k];                    // 1 CONDITIONAL BRANCH
k  :  int str[k] = stride<k>();                 // 1 CALL TO stride<k>()
k  :  int tmp[k] = i[k]*str[k];                 // 1 MULTIPLY
k  :  int idx[k] = idx[k+1] + tmp[k];           // 1 LOAD (idx[k+1]), 1 ADD, 1 STORE (idx[k])
k-1:  int i[k-1] = 0;
k-1:label_for[k-1]:
k-1:  bool r = i[k-1] < d[k-1];                 // 1 LOAD (d[k-1]), 1 COMPARE
k-1:  branch r, label_if[k-1];                  // 1 CONDITIONAL BRANCH
k-1:  int str[k-1] = stride<k-1>();             // 1 CALL TO stride<k-1>()
k-1:  int tmp[k-1] = i[k-1]*str[k-1];           // 1 MULTIPLY
k-1:  int idx[k-1] = idx[k] + tmp[k-1];         // 1 LOAD (idx[k]), 1 ADD, 1 STORE (idx[k-1])
      // ...
0  :  int i[0] = 0;
0  :label_for[0]:
0  :  bool r = i[0] < d[0];                     // 1 LOAD (d[0]), 1 COMPARE
0  :  branch r, label_if[0];                    // 1 CONDITIONAL BRANCH
0  :  int str[0] = stride<0>();                 // 1 CALL TO stride<0>()
0  :  int tmp[0] = i[0]*str[0];                 // 1 MULTIPLY
0  :  int idx[0] = idx[1] + tmp[0];             // 1 LOAD (idx[0]), 1 ADD, 1 STORE (idx[0])
0  :  auto& e = bp[idx[0]];                     // 1 LOAD (idx[0]), 1 INDIRECT LOAD (bp[idx[0]])
0  :  f(e, i);                                  // 1 CALL TO f(e, i)
0  :  ++i[0];                                   // 1 LOAD (i[0], 1 ADD, 1 STORE (i[0]) 
0  :  goto label_for[0];                        // 1 UNCONDITIONAL BRANCH
0  :label_if[0]:
      // ... 
k-1:  ++i[k-1];                                 // 1 LOAD (i[k-1], 1 ADD, 1 STORE (i[k-1])
k-1:  goto label_for[k-1];                      // 1 UNCONDITIONAL BRANCH
k-1:label_if[k-1]:
k  :  ++i[k];                                   // 1 LOAD (i[k], 1 ADD, 1 STORE (i[k])
k  :  goto label_for[k];                        // 1 UNCONDITIONAL BRANCH
k  :label_if[k]:
    // ... 
n  :  ++i[n];                                   // 1 LOAD (i[n], 1 ADD, 1 STORE (i[n])
n  :  goto label_for[n];                        // 1 UNCONDITIONAL BRANCH
n  :label_if[n]:

