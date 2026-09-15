# zdtree
successor of li chao tree




Tree is 3d
[0] stores number of items
[1 + (n-1)*feature_TC1idx + feature_index] stores rank of feature for the nth member
(feature index is 0 indexed, n is 1 indexed, TC1idx means total count 1-indexed)
Last element of which is [n*feature_TC1idx]

//[n*feature + 1] to [n*feature + feature_TC1idx] stores the same base rotation index

Assuming current no rotation

Let X = n*feature_TC1idx

First feature is time, followed by rank, followed by others

Ternary tree structure:
Nth feature has index
Row 0: X + feature_index + 1~30
Row 1: X + feature_TC1idx + feature_index*3 + 1~31
Row 2: X + feature_TC1idx + feature_TC1idx*3 + (feature_index*3)*3 + 1~32
Row 3: X + feature_TC1idx + feature_TC1idx*3 + feature_TC1idx*3*3 + (feature_index*3*3)*3 + 1~33
Row N = X + feature_TC1idx * (1/2) * (3N - 1) + feature_index*3N + 1~3N
By GP formula
Where 3N >= n
So the last row can store all the stuff

For each triplet apart from last row:
First one stores “max of its 3 numbers”
Second one stores “middle of its 3 numbers”
Last one stores “min of its 3 numbers”
To form an asymmetric ternary rotating tree structure

Rotation:
Row 0 would not rotate
Row 1 rotate rotato + potato rounds
Row 2 rotate rotato + potatorotato + rotatopotato rounds
Row 3 rotate rotato + potatorow 2 mod 7 + rotatorow 2 mod 5 rounds
Row 4 rotate rotato + potatorow 3 mod 7 + rotatorow 3 mod 5 rounds
Row k rotate rotato + potatorow k-1 mod 7 + rotatorow k-1 mod 5 rounds




What am i doing with my life



