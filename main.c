#include <stdio.h>
#include <unistd.h>

#define DEAD_CELL 46
#define LIVE_CELL 35

int pow2(int exp);
int get_nth_bit(int *decimal_number, int n);
int set_nth_bit(int *decimal_number, int n, int bit);
int get_neigh(int field[], int row, int column);
int spawn(int field[], int row, int column);
int die(int field[], int row, int column);
void print_field(int field[], int rows);
void iter_gen(int field[]);


int pow2(int exp) {
    if (exp == 0)
	return 1;
    if (exp < 0)
	return -1;
    if (exp == 1) {
	return 2;
    } else {
	return 2 * pow2(exp - 1);
    }
}


int get_nth_bit(int *decimal_number, int n) {
    int bit;
    bit = (*decimal_number & pow2(n)) == 0 ? 0 : 1;
    return bit;
}


int set_nth_bit(int *decimal_number, int n, int bit) {
    if (bit != 0 && bit != 1)
	return -1;
    if (bit == get_nth_bit(decimal_number, n))
	return -1;
    if (n < 0 || n > 31)
	return -1;

    if (bit == 0) {
	*decimal_number -= pow2(n);
    } else {
	*decimal_number += pow2(n);
    }
    return bit;
}


int get_cell(int field[], int row, int column) {
    return get_nth_bit(&field[row], 31 - column);
}


int spawn(int field[], int row, int column) {
    return set_nth_bit(&field[row], 31 - column, 1);
}


int die(int field[], int row, int column) {
    return set_nth_bit(&field[row], 31 - column, 0);
}


void print_field(int field[], int rows) {
    for (int i = 0; i < rows; i++) {
	for (int j = 31; j >= 0; j--) {
	    if (get_nth_bit(&field[i], j) == 0) {
		putchar(DEAD_CELL);
	    } else {
		putchar(LIVE_CELL);
	    }
	}
	putchar('\n');
    }
}


int get_neigh(int field[], int row, int column) {
    int neigh = 0;

    // UL corner: row=0 column=0
    if (row == 0 && column == 0) {
	neigh += get_cell(field, row, column + 1) + get_cell(field, row + 1, column) +
	    get_cell(field, row + 1, column + 1);

    // UR corner: row=0 column=31
    } else if (row == 0 && column == 31) {
	neigh += get_cell(field, row, column - 1) + get_cell(field, row + 1, column) +
	    get_cell(field, row + 1, column - 1);

    // BL corner: row=9 column=0
    } else if (row == 9 && column == 0) {
	neigh += get_cell(field, row, column + 1) + get_cell(field, row - 1, column) +
	    get_cell(field, row - 1, column + 1);

    // BR corner: row=9 column=31
    } else if (row == 9 && column == 31) {
	neigh += get_cell(field, row, column - 1) + get_cell(field, row - 1, column) +
	    get_cell(field, row - 1, column - 1);

    // US: row=0 column=1:30
    } else if (row == 0 && (column > 0 && column < 31)) {
	neigh += get_cell(field, row, column - 1) + get_cell(field, row, column + 1) +
	    get_cell(field, row + 1, column - 1) + get_cell(field, row + 1, column) +
	    get_cell(field, row + 1, column + 1);

    // LS: row=1:9 column=0
    } else if ((row > 0 && row < 10) && column == 0) {
	neigh += get_cell(field, row - 1, column) + get_cell(field, row, column + 1) +
	    get_cell(field, row + 1, column) + get_cell(field, row + 1, column + 1);

    // RS: row=1:9 column=31
    } else if ((row > 0 && row < 10) && column == 31) {
	neigh += get_cell(field, row - 1, column) + get_cell(field, row, column - 1) +
	    get_cell(field, row + 1, column - 1) + get_cell(field, row + 1, column);

    // BS: row=9 column=1:30
    } else if (row == 9 && (column > 0 && column < 31)) {
	neigh += get_cell(field, row, column - 1) + get_cell(field, row - 1, column - 1) +
	    get_cell(field, row - 1, column) + get_cell(field, row - 1, column + 1) +
	    get_cell(field, row, column + 1);

    } else {
    neigh += get_cell(field, row - 1, column - 1) + get_cell(field, row - 1, column) +
	get_cell(field, row - 1, column + 1) + get_cell(field, row, column - 1) +
	get_cell(field, row, column + 1) + get_cell(field, row + 1, column - 1) +
	get_cell(field, row + 1, column) + get_cell(field, row + 1, column + 1);
    }
    return neigh;
}


void iter_gen(int field[]) {
    int neigh_array[320] = {0};

    for (int i = 0; i < 10; i++) {
	for (int j = 31; j >= 0; j--) {
	    int neigh;
	    if (get_cell(field, i, j) == 1)
		neigh_array[32 * i + j] = 10;
	    neigh = get_neigh(field, i, j);
	    neigh_array[32 * i + j] += neigh;
	}
    }

    for (int i = 0; i < 320; i++) {
	int cell;
	cell = neigh_array[i];

	// dead cell
	if (cell < 10) {
	    if (cell == 3)
		spawn(field, i / 32, i % 32);
	} else
	    // live cell with fewer than 2 neighs
	    if (cell < 12) {
		die(field, i / 32, i % 32);
	} else
	    // live cell with 2 to 3 neighs
	    if ((cell > 11) && (cell < 14)) {
		;
	} else
	    // live cell with more than 3 neighs
	    if (cell > 13) {
		die(field, i / 32, i % 32);
	}
    }
}


void main() {
    int field[10] = {0};
    int *p = field;
    spawn(field, 2, 10);
    spawn(field, 3, 11);
    spawn(field, 4, 9);
    spawn(field, 4, 10);
    spawn(field, 4, 11);
    printf("\e[1;1H\e[2J");

    for (int i = 0; i < 28; i++) {
	print_field(field, 10);
	iter_gen(field);
	usleep(200000);
	printf("\e[1;1H\e[2J");
    }
}
