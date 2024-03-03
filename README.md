
## 8-Puzzle Solver

This program is designed to solve the classic 8-puzzle game.

### Puzzle State

- `dim`: Represents the dimension of the puzzle grid (default is 3x3).
- `table`: Represents the puzzle grid as a 1D array.
- `zero_pos`: Represents the position of the empty tile (zero).
- `solved`: Flag to indicate whether the puzzle is solved.
- `shuffle_iterations`: Number of random shuffles performed initially.

### Tools

- `sgn`: Returns the sign of a number.
- `geti`, `getj`: Get row and column indices from a linear index.
- `getij`: Get row and column indices from a linear index (pointer version).
- `getk`: Get linear index from row and column indices.
- `getelem_tableref`, `getelem`: Get element from the table by linear index.
- `setelem_tableref`, `setelem`: Set element in the table by linear index.
- `swapmem`: Swap values of two integers.
- `swapel_tableref`, `swapel`: Swap elements in the table by linear indices.
- `randomrange`: Generate a random number within a specified range.
- `calc_inversion_forel`, `calc_inversions`: Calculate inversions in the puzzle grid.
- `calc_manhattan_and_hamming`: Calculate the Manhattan and Hamming distances of the puzzle grid.
- `create_table_copy`: Create a copy of the puzzle grid.

### Functions

- `fill_table_random`: Fill the puzzle grid with numbers in random order.
- `print_game`: Print the current state of the puzzle grid.
- `calc_del_position`: Calculate the movement direction of the empty tile.
- `move_exists`: Check if a move is possible.
- `change_hole_pos_ref`, `change_hole_pos`: Change the position of the empty tile.
- `click_elem`, `click_elem_k`: Perform a move by clicking a tile.
- `verify_win`: Verify if the puzzle is solved.
- `solve_bruteforce`: Solve the puzzle using a brute-force approach.
- `init_puzzle`: Initialize the puzzle grid and solve it.

### How to Use

1. Initialize the puzzle using `init_puzzle()`.
2. The program will print the initial state of the puzzle and then solve it.
3. The solution will be printed along with the number of iterations required.

### Limitations

- It may not find the optimal solution in terms of the minimum number of moves.

Compile and run the program to see the solver in action!

References:

https://deniz.co/8-puzzle-solver/
