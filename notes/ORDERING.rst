ijk
o[0, 1, 2]

rank index -> ordering
0, 0
1, 1
2, 2

ordering -> rank index (unsorted)
0, 0
1, 1
2, 2

ordering -> rank index (sorted)
0, 0
1, 1
2, 2



kji
o[2, 1, 0]

rank index -> ordering
0, 2
1, 1
2, 0

ordering -> rank index (unsorted)
2, 0
1, 1
0, 2

ordering -> rank index (sorted)
0, 2
1, 1
2, 0



ikj
o[0, 2, 1]

rank index -> ordering
0, 0
1, 2
2, 1

ordering -> rank index (unsorted)
0, 0
2, 1
1, 2

ordering -> rank index (sorted)
0, 0
1, 2
2, 1



jki
o[2, 0, 1]

rank index -> ordering
0, 2
1, 0
2, 1

ordering -> rank index (unsorted)
2, 0
0, 1
1, 2

ordering -> rank index (sorted)
0, 1
1, 2
2, 0



likj
o[1, 3, 2, 0]

rank index -> ordering
0, 1
1, 3
2, 2
3, 0

ordering -> rank index (unsorted)
1, 0
3, 1
2, 2
0, 3

ordering -> rank index (sorted)
0, 3
1, 0
2, 2
3, 1



lkji
o[3, 2, 1, 0]

rank index -> ordering
0, 3
1, 2
2, 1
3, 0

ordering -> rank index (unsorted)
3, 0
2, 1
1, 2
0, 3

ordering -> rank index (sorted)
0, 3
1, 2
2, 1
3, 0



Algorithm:
1.) Create a sequence of (ordering, rank index) key-value pairs from the ordering typelist. O(n)
2.) Sort the new key value sequence by key. O(sort(n))
3.) Create a new integer_sequence of sorted rank indices from the key-value sequence. O(n)
