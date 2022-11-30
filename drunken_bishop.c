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

static void filed_add(int pos, Filed* filed) {
	filed->filed[pos] += 1;
}

static void filed_init(Filed* filed) {
	filed->w = F_WIDTH;
	filed->h = F_HEIGHT;
	filed->filed = (int*)calloc(filed->w * filed->h,
	                            sizeof(int));

	Point begin_pnt = {filed->w / 2, filed->h / 2};
	filed_add(get_pos(begin_pnt), filed);
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

	pnt->y += ((dir & 0b10) << 1) - 1;
	pnt->y = limit(pnt->y, 0, F_HEIGHT);
}
