# Rules
1. Live cells with fewer than 2 neighbours die.
2. Live cells with 2 to 3 neighbours live onto the next generation.
3. Live cells with more than 3 neighbours die.
4. Dead cells with 3 neighbours become live cells.

The field is made of an array of `char` values, thus for now the number of columns is always divisible by 8.
The number of rows and columns are set by Y_AXIS (1 step = 1 row) and X_AXIS (1 step = 8 columns).
The animation is done by clearing a terminal via `printf("\e[1;1H\e[2J")`.
