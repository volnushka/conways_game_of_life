#include <stdio.h>
#include <unistd.h>

#define X_AXIS 20
#define Y_AXIS 30
#define DEAD_CELL 46
#define LIVE_CELL 35

int pow2(int exp);
int get_cell(char field[][X_AXIS], int x, int y);
int set_cell(char field[][X_AXIS], int x, int y, int bit);
int get_neigh(char field[][X_AXIS], int x, int y);
int spawn(char field[][X_AXIS], int x, int y);
int die(char field[][X_AXIS], int x, int y);
void print_field(char field[][X_AXIS]);
void iter_gen(char field[][X_AXIS]);

struct Index {
    int i;
    int j;
};

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

struct Index map(int x, int y) {
    struct Index index;
    
    index.i = y;
    index.j = x / 8;

    return index;
}

int get_cell(char field[][X_AXIS], int x, int y) {
    char conj;
    struct Index index;

    index = map(x, y);
    conj = field[index.i][index.j] & pow2(7 - x % 8);

    if (conj != 0)
	return 1;
    return 0;
}

int set_cell(char field[][X_AXIS], int x, int y, int bit) {
    struct Index index;

    index = map(x, y);
    
    if (get_cell(field, x, y) == bit)
	return -1;

    if (bit == 0) {
	field[index.i][index.j] -= pow2(7 - x % 8);
    } else {
	field[index.i][index.j] += pow2(7 - x % 8);
    }

    return bit;
}


int spawn(char field[][X_AXIS], int x, int y) {
    return set_cell(field, x, y, 1);
}


int die(char field[][X_AXIS], int x, int y) {
    return set_cell(field, x, y, 0);
}


void print_field(char field[][X_AXIS]) {
    for (int i = 0; i < Y_AXIS; i++) {
	for (int j = 0; j < X_AXIS * 8; j++) {
	    if (get_cell(field, j, i) == 0) {
		putchar(DEAD_CELL);
	    } else {
		putchar(LIVE_CELL);
	    }
	}
	putchar('\n');
    }
}

int get_neigh(char field[][X_AXIS], int x, int y) {
    int neigh = 0;

    // UL corner: x=0 y=0
    if (x == 0 && y == 0) {
	neigh += get_cell(field, x, y + 1) + get_cell(field, x + 1, y) +
	    get_cell(field, x + 1, y + 1);

    // BL corner: x=0 y=Y_AXIS-1
    } else if (x == 0 && y == Y_AXIS - 1) {
	neigh += get_cell(field, x, y - 1) + get_cell(field, x + 1, y) +
	    get_cell(field, x + 1, y - 1);

    // UR corner: x=X_AXIS-1 y=0
    } else if (x == X_AXIS * 8 - 1 && y == 0) {
	neigh += get_cell(field, x, y + 1) + get_cell(field, x - 1, y) +
	    get_cell(field, x - 1, y + 1);

    // BR corner: x=X_AXIS-1 y=Y_AXIS-1
    } else if (x == X_AXIS * 8 - 1 && y == Y_AXIS - 1) {
	neigh += get_cell(field, x, y - 1) + get_cell(field, x - 1, y) +
	    get_cell(field, x - 1, y - 1);

    // US: x=1:X_AXIS-2 y=0
    } else if (y == 0 && (x > 0) && (x < X_AXIS * 8 - 1)) {
	neigh += get_cell(field, x - 1, y) + get_cell(field, x + 1, y) +
	    get_cell(field, x - 1, y + 1) + get_cell(field, x, y + 1) +
	    get_cell(field, x + 1, y + 1);

    // LS: x=0 y=1:Y_AXIS-2
    } else if ((y > 0 && y < Y_AXIS - 1) && x == 0) {
	neigh += get_cell(field, x, y - 1) + get_cell(field, x, y + 1) +
	    get_cell(field, x + 1, y - 1) + get_cell(field, x + 1, y) +
	    get_cell(field, x + 1, y + 1);

    // RS: x=X_AXIS-1 y=1:Y_AXIS-2
    } else if ((y > 0) && (y < Y_AXIS - 1) && (x == X_AXIS * 8 - 1)) {
	neigh += get_cell(field, x - 1, y) + get_cell(field, x, y - 1) +
	    get_cell(field, x - 1, y - 1) + get_cell(field, x - 1, y + 1) +
	    get_cell(field, x, y + 1);

    // BS: x=1:X_AXIS-2 y=Y_AXIS-1
    } else if (y == (Y_AXIS - 1) && (x > 0) && (x < X_AXIS * 8 - 1)) {
	neigh += get_cell(field, x - 1, y) + get_cell(field, x - 1, y - 1) +
	    get_cell(field, x, y - 1) + get_cell(field, x + 1, y - 1) +
	    get_cell(field, x + 1, y);

    // any inner cell: x=1:X_AXIS-2 y=1:Y_AXIS-2
    } else {
    neigh += get_cell(field, x - 1, y - 1) + get_cell(field, x - 1, y) +
	get_cell(field, x - 1, y + 1) + get_cell(field, x, y - 1) +
	get_cell(field, x, y + 1) + get_cell(field, x + 1, y - 1) +
	get_cell(field, x + 1, y) + get_cell(field, x + 1, y + 1);
    }
    return neigh;
}

void iter_gen(char field[][X_AXIS]) {
    char neigh_array[Y_AXIS * X_AXIS * 8] = {0};

    for (int i = 0; i < Y_AXIS; i++) {
	for (int j = 0; j < X_AXIS * 8; j++) {
	    int neigh = 0;
	    if (get_cell(field, j, i) == 1)
		neigh_array[i * 8 * X_AXIS + j] = 10;
	    neigh = get_neigh(field, j, i);
	    neigh_array[i * 8 * X_AXIS + j] += neigh;
	}
    }

    for (int i = 0; i < Y_AXIS * X_AXIS * 8; i++) {
	char cell;
	cell = neigh_array[i];

	// dead cell
	if (cell < 10) {
	    if (cell == 3) {
		spawn(field, i % (X_AXIS * 8), i / (X_AXIS * 8));
	    }
	} else
	    // live cell with fewer than 2 neighs
	    if (cell < 12) {
		die(field, i % (X_AXIS * 8), i / (X_AXIS * 8));
	} else
	    // live cell with more than 3 neighs
	    if (cell > 13) {
		die(field, i % (X_AXIS * 8), i / (X_AXIS * 8));
	}
	    // live cells with 2 to 3 neighs (11 < cell < 14) remain
    }
}


void main() {
    char field[Y_AXIS][X_AXIS] = {{0}, {0}};
    spawn(field, 10, 1);
    spawn(field, 11, 2);
    spawn(field, 9, 3);
    spawn(field, 10, 3);
    spawn(field, 11, 3);
    
    spawn(field, 15, 6);
    spawn(field, 16, 7);
    spawn(field, 14, 8);
    spawn(field, 15, 8);
    spawn(field, 16, 8);

    spawn(field, 40, 10);
    spawn(field, 40, 11);
    spawn(field, 39, 11);
    spawn(field, 40, 12);
    spawn(field, 41, 12);

    spawn(field, 90, 20);
    spawn(field, 92, 20);
    spawn(field, 92, 19);
    spawn(field, 94, 18);
    spawn(field, 94, 17);
    spawn(field, 94, 16);
    spawn(field, 96, 17);
    spawn(field, 96, 16);
    spawn(field, 96, 15);
    spawn(field, 97, 16);

    print_field(field);
    sleep(1);
    for (int i = 0; i < 1000; i++) {
	printf("\e[1;1H\e[2J");
	iter_gen(field);
	print_field(field);
	usleep(100000);
    }
}
