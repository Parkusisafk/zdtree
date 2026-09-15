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

Row 0: X + feature_index + 1~3^0

Row 1: X + feature_TC1idx + feature_index*3 + 1~3^1

Row 2: X + feature_TC1idx + feature_TC1idx*3 + (feature_index*3)*3 + 1~3^2

Row 3: X + feature_TC1idx + feature_TC1idx*3 + feature_TC1idx*3*3 + (feature_index*3*3)*3 + 1~3^3

Row N = X + feature_TC1idx * (1/2) * (3^N - 1) + feature_index*3^N + 1~3^N

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

Row 3 rotate rotato + potatorow 2 mod random + rotatorow 2 mod random rounds

Row 4 rotate rotato + potatorow 3 mod random + rotatorow 3 mod random rounds

Row k rotate rotato + potatorow k-1 mod random + rotatorow k-1 mod random rounds


You can track individual data points with its specific features on this data structure, and tracking of order of insertion and rank of each feature for each set of data is O(n)

Also its somewhat cryptographically secure i hope since its always rotating potatoes, maybe.

What am i doing with my life


<img width="511" height="520" alt="image" src="https://github.com/user-attachments/assets/003289ce-2fe5-4999-ba09-3c2bb6705045" />
<img width="475" height="501" alt="image" src="https://github.com/user-attachments/assets/21b9c089-a4bd-408c-b8d7-2e0e9045ad33" />

visual aid
