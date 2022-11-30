#include <memory.h>
#include <malloc.h>

#define max(x, y)          ((x) > (y) ? (x) : (y))
#define min(x, y)          ((x) < (y) ? (x) : (y))
#define limit(n, from, to) max(min(n, to - 1), from)

#define F_WIDTH  17
#define F_HEIGHT 9


typedef struct {
	int x, y;
} Point;


static int get_pos(const Point pnt) {
	return pnt.x + pnt.y * F_WIDTH;
}


typedef struct {
	size_t w, h;
	int* filed;
} Filed;

static void filed_print(const Filed* filed) {
	putc('+', stdout);
	for(int i = 0; i < filed->w; i++)
		putc('-', stdout);
	fputs("+\n", stdout);

	for(int y = 0; y < filed->h; y++) {
		putc('|', stdout);
		for(int x = 0; x < filed->w; x++) {
			Point pnt = {x, y};
			putc(" .o+=*BOX@%&#/^SE"[filed->filed[get_pos(
			         pnt)]],
			     stdout);
		}
		fputs("|\n", stdout);
	}

	putc('+', stdout);
	for(int i = 0; i < filed->w; i++)
		putc('-', stdout);
	fputs("+\n", stdout);
}

static void filed_init(Filed* filed) {
	filed->w = F_WIDTH;
	filed->h = F_HEIGHT;
	filed->filed =
	    (int*)calloc(filed->w * filed->h, sizeof(int));
}

static void filed_free(Filed* filed) {
	free(filed->filed);
	filed->filed = NULL;
}


/**
 * \param dir direction
 *
 * 0b00 UP_LEFT    \n
 * 0b01 UP_RIGHT   \n
 * 0b10 DOWN_LEFT  \n
 * 0b11 DOWN_RIGHT \n
 */
static void move(Point* pnt, int dir) {
	pnt->x += ((dir & 0b01) << 1) - 1;
	pnt->x = limit(pnt->x, 0, F_WIDTH);

	pnt->y += (dir & 0b10) - 1;
	pnt->y = limit(pnt->y, 0, F_HEIGHT);
}


static int atoi(const char* const str, int base) {
	int digit = 0;
	for(const char* i = str; *i; i++)
		digit = digit * base + *i
		    - (*i >= 'a' ? 'a' - 10 : '0');
	return digit;
}


static void bishop_move(const char* const hex,
                        Point* const pnt,
                        Filed* const filed) {
	Point start_pnt = *pnt;
	const char* c = hex;
	char block[3];
	block[2] = '\0';

	while(*c) {
		block[0] = *c++;
		block[1] = *c++;

		int bits = atoi(block, 16);

		for(int i = 0; i < 4; i++) {
			move(pnt, bits & 0b11);
			bits >>= 2;
			filed->filed[get_pos(*pnt)] += 1;
		}
	}

	Point end_pnt = *pnt;
	filed->filed[get_pos(start_pnt)] = 15;
	filed->filed[get_pos(end_pnt)] = 16;
}
