#ifndef __COMMON_H__
#define __COMMON_H__

extern int control_msg_level;

#define VERSION "1.0"

#define min(a, b) ((a) < (b) ? (a) : (b))

#define BITS_PER_BYTE		8
#define BITS_PER_LONG		(BITS_PER_BYTE * sizeof(long))

#define __round_mask(x, y) ((__typeof__(x))((y)-1))
#define round_down(x, y) ((x) & ~__round_mask(x, y))
#define DIV_ROUND_UP(n,d) (((n) + (d) - 1) / (d))

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

#define ASSERT(exp)							\
	do {								\
		if (!(exp)) {						\
			printf("ASSERT [%s:%4d] (%s) failed\n",		\
				__FILE__, __LINE__, #exp);		\
			exit(-1);					\
		}							\
	} while(0)

#define MSG(level, format, ...)						\
	do {								\
		if (level >= control_msg_level) {			\
			printf("[MSG] " format, ##__VA_ARGS__);		\
		}							\
	} while(0)

#define MSG_NOHEAD(level, format, ...)						\
	do {								\
		if (level >= control_msg_level) {			\
			printf(format, ##__VA_ARGS__);			\
		}							\
	} while(0)

#define WARN(format, ...)						\
	do {								\
		printf("[WARN] [%s:%4d] " format "\n",			\
			__FILE__, __LINE__,  ##__VA_ARGS__);		\
	} while(0)							\

#define WARN_ON(condition, format, ...)					\
	do {								\
		if ((condition)) {					\
			WARN(##__VA_ARGS__);				\
		}							\
	} while(0)

#endif
