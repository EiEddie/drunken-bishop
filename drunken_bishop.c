#include <memory.h>
#include <malloc.h>
#include <stdlib.h>

#define max(x, y)          ((x) > (y) ? (x) : (y))
#define min(x, y)          ((x) < (y) ? (x) : (y))
#define limit(n, from, to) max(min(n, to - 1), from)

#define F_WIDTH  17
#define F_HEIGHT 9

#define VERSION "1.0.0"
#define STORY                                               \
	"Bishop Peter finds himself in the middle of an\n"      \
	"ambient atrium. There are walls on all four sides\n"   \
	"and apparently there is no exit. The floor is paved\n" \
	"with square tiles, strictly alternating between\n"     \
	"black and white. His head heavily aching—probably\n" \
	"from too much wine he had before—he starts\n"        \
	"wandering around randomly. Well, to be exact, he\n"    \
	"only makes diagonal steps—just like a bishop on a\n" \
	"chess board. When he hits a wall, he moves to the\n"   \
	"side, which takes him from the black tiles to the\n"   \
	"white tiles (or vice versa). And after each move,\n"   \
	"he places a coin on the floor, to remember that he\n"  \
	"has been there before. After 64 steps, just when no\n" \
	"coins are left, Peter suddenly wakes up. What a\n"     \
	"strange dream!\n"


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


static int __atoi(const char* const str, int base) {
	int digit = 0;
	for(const char* i = str; *i; i++)
		digit = digit * base + *i
		    - (*i >= 'a' ? 'a' - 10 : '0');
	return digit;
}


static int bishop_move(const char* const hex,
                       Point* const pnt,
                       Filed* const filed) {
	Point start_pnt = *pnt;
	const char* c = hex;
	char block[3];
	block[2] = '\0';

	while(*c) {
		block[0] = *c++;
		if(!*c) {
			fputs(
			    "length of given hex-string is not even.\n",
			    stderr);
			return -1;
		}
		block[1] = *c++;

		int bits = __atoi(block, 16);

		for(int i = 0; i < 4; i++) {
			move(pnt, bits & 0b11);
			bits >>= 2;
			filed->filed[get_pos(*pnt)] += 1;
		}
	}

	Point end_pnt = *pnt;
	filed->filed[get_pos(start_pnt)] = 15;
	filed->filed[get_pos(end_pnt)] = 16;

	return 0;
}


static int print_fingerprint(const char* const hex_str) {
	int err = 0;

	Filed filed;
	filed_init(&filed);

	Point bishop_pnt = {8, 4};
	if((err = bishop_move(hex_str, &bishop_pnt, &filed)))
		goto ERROR;
	filed_print(&filed);

ERROR:
	filed_free(&filed);
	return err;
}


int main(int argc, char* argv[]) {
	const char* hex_str =
	    "fc94b0c1e5b0987c5843997697ee9fb7";

	int opt_help = 0;
	int opt_version = 0;
	int opt_hex = 0;
	int opt_is_value = 0;
	int opt_quiet = 0;
	int opt_story = 0;

	for(int i = 1; i < argc; i++) {
		char* arg = argv[i];

		if(*arg != '-') {
			if(!opt_hex) { // 是hash字符串
				hex_str = arg;
				opt_hex = 1;
			} else if(opt_is_value) { // 是上个参数的值
				opt_is_value = 0;
				continue;
			} else {
				fputs("unknown param: `", stderr);
				fputs(arg, stderr);
				fputs("'\n", stderr);
				return 1;
			}
			continue;
		}
		arg++;

		if(*arg == '-') { // 长选项
			arg++;

			if(strcmp(arg, "version") == 0) {
				opt_version = 1;
			} else if(strcmp(arg, "help") == 0) {
				opt_help = 1;
			} else if(strcmp(arg, "story") == 0) {
				opt_story = 1;
			} else if(strcmp(arg, "quiet") == 0) {
				opt_quiet = 1;
			} else {
				fputs("unrecognized flag `--", stderr);
				fputs(arg, stderr);
				fputs("'\n", stderr);
				return 1;
			}
			continue;
		}

		for(; *arg != '\0'; ++arg) { // 短选项
			switch(*arg) {
			case 'h':
				opt_help = 1;
				break;
			case 'q':
				opt_quiet = 1;
				break;
			default:
				fputs("unrecognized flag `-", stderr);
				fputs(arg, stderr);
				fputs("'\n", stderr);
				return 1;
			}
		}
	}

	if(opt_version) {
		fputs("drunken_bishop v" VERSION " by EiEddie\n",
		      stdout);
		return 0;
	}
	if(opt_help) {
		fputs(
		    "The hash fingerprint visualization algorithm, like OpenSSH.\n"
		    "Usage: drunken_bishop [OPTION] [hex]\n"
		    "\n"
		    "  -h, --help     print help info\n"
		    "      --version  print version info\n"
		    "  -q, --q        don't echo hex input\n"
		    "      --story    read the story of Bishop Peter\n",
		    stdout);
		return 0;
	}
	if(opt_story) {
		fputs(STORY, stdout);
		return 0;
	}

	if(!opt_quiet)
		printf("fingerprint of %s:\n", hex_str);

	return print_fingerprint(hex_str);
}
