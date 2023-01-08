#include <memory.h>
#include <malloc.h>
#include <stdlib.h>
#include <ctype.h>

#define max(x, y)          ((x) > (y) ? (x) : (y))
#define min(x, y)          ((x) < (y) ? (x) : (y))
#define limit(n, from, to) max(min(n, to - 1), from)

#define F_WIDTH  17
#define F_HEIGHT 9

#define BUFSIZE 1024 * 1024 // 1MB

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


static void to_lower(char* dest, const char* src) {
	char* i = dest;
	const char* c = src;

	while(*c) {
		if(*c >= 'A' && *c <= 'Z')
			*i = *c - 'A' + 'a';
		else
			*i = *c;
		++i;
		++c;
	}
}


enum InputType {
	/** \brief 十六进制字符串 */
	HEX,
	/** \brief 字节流 */
	BYTES
};

enum InputType get_input_type(const char* in) {
	char buf[16] = {0};
	to_lower(buf, in);

	if(strcmp(buf, "hex") == 0)
		return HEX;
	else if(strcmp(buf, "bytes") == 0)
		return BYTES;
	else
		return -1;
}


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


static int to_dec(const char* const str, int base) {
	char buffer[BUFSIZE];
	int digit = 0;
	to_lower(buffer, str);

	for(const char* i = buffer; *i; i++)
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

		int bits = to_dec(block, 16);

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


/**
 * \brief 删除文件尾部连续的空行与空字符
 */
static void remove_end_whitespace(char* buffer) {
	char* offset = buffer;
	while(offset != buffer) {
		if(isspace(*(--offset)))
			*offset = '\0';
		else
			break;
	}
}


static int read_file(const char* file_path, char* buffer) {
	FILE* file = NULL;
	if(strcmp(file_path, "-") == 0)
		file = stdin;
	else
		file = fopen(file_path, "r");

	if(file != NULL) {
		char* offset = buffer;
		while((*offset++ = fgetc(file)) != EOF)
			/* nothing */;
		*(--offset) = '\0';
		fclose(file);
		remove_end_whitespace(buffer);
		return 0;
	} else {
		fputs(file_path, stderr);
		fputs(": no such file\n", stderr);
		return -1;
	}
}


int main(int argc, char* argv[]) {
	const char* hex_str =
	    "fc94b0c1e5b0987c5843997697ee9fb7";

	int is_help = 0;           // 打印帮助信息
	int is_version = 0;        // 打印版本信息
	int is_quiet = 0;          // quiet 模式
	int is_story = 0;          // 打印 Peter 的故事
	enum InputType type = HEX; // 输入的类型
	const char* file_path = NULL;

	int opt_hex = 0; // 是hash字符串

	for(int i = 1; i < argc; i++) {
		char* arg = argv[i];

		if(*arg != '-') {
			if(!opt_hex) {
				hex_str = arg;
				opt_hex = 1;
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
			int opt_is_value = 0;

			arg++;

			if(strcmp(arg, "version") == 0) {
				is_version = 1;
			} else if(strcmp(arg, "help") == 0) {
				is_help = 1;
			} else if(strcmp(arg, "story") == 0) {
				is_story = 1;
			} else if(strcmp(arg, "quiet") == 0) {
				is_quiet = 1;
			} else if(strcmp(arg, "in") == 0) {
				opt_is_value = 1;
				file_path = argv[++i];
			} else if(strcmp(arg, "type") == 0) {
				opt_is_value = 1;
				type = get_input_type(argv[++i]);
			} else {
				fputs("unrecognized flag `--", stderr);
				fputs(arg, stderr);
				fputs("'\n", stderr);
				return 1;
			}

			if(opt_is_value && i == argc) {
				// 需要值的选项是参数列表最后一个时
				// e.g. -q --in
				fputs("expected value after `", stderr);
				fputs(argv[i - 1], stderr);
				fputs("'\n", stderr);
				return 1;
			}

			continue;
		}

		for(; *arg != '\0'; ++arg) { // 短选项
			int opt_is_value = 0;

			switch(*arg) {
			case 'h':
				is_help = 1;
				break;
			case 'q':
				is_quiet = 1;
				break;
			case 'i':
				opt_is_value = 1;
				file_path = argv[++i];
				break;
			case 't':
				opt_is_value = 1;
				type = get_input_type(argv[++i]);
				break;
			default:
				fputs("unrecognized flag `-", stderr);
				fputs(arg, stderr);
				fputs("'\n", stderr);
				return 1;
			}

			if(opt_is_value
			   && (*(arg + 1) != '\0' || i == argc)) {
				// 需要值的短选项不是最后一个短选项时
				// e.g. -iq -
				// 或需要值的短选项是参数列表最后一个时
				// e.g. -q -i
				fputs("expected value after `-", stderr);
				fputc(*arg, stderr);
				fputs("'\n", stderr);
				return 1;
			}
		}
	}

	if(is_version) {
		fputs("bishop v" VERSION " by EiEddie\n", stdout);
		return 0;
	}
	if(is_help) {
		fputs(
		    "The hash fingerprint visualization algorithm, like OpenSSH.\n"
		    "Usage: bishop [OPTION] [hex]\n"
		    "\n"
		    "  -h, --help       print help info\n"
		    "      --version    print version info\n"
		    "  -q, --quiet      don't echo hex input\n"
		    "  -i, --in <file>  use file, if '-' use stdin\n"
		    "      --story      read the story of Bishop Peter\n",
		    stdout);
		return 0;
	}
	if(is_story) {
		fputs(STORY, stdout);
		return 0;
	}
	if(type == -1) {
		fputs("unknown input type\n", stderr);
		return 1;
	}

	char buffer[BUFSIZE];
	if(file_path != NULL) {
		if(!read_file(file_path, buffer)) {
			if(type == HEX)
				remove_end_whitespace(buffer);
			hex_str = buffer;
		} else {
			return 1;
		}
	}

	if(!is_quiet)
		printf("fingerprint of %s:\n", hex_str);

	return print_fingerprint(hex_str);
}
// TODO: 处理输入的字节流
// TODO: 修改文档: 版本, 帮助信息, README
