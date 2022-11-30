#include <memory.h>
#include <malloc.h>

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
