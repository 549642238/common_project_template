#ifndef __COMMON_H__
#define __COMMON_H__

extern int control_msg_level;

#define VERSION "1.0"

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
			printf(format, ##__VA_ARGS__);			\
		}							\
	} while(0)

#endif
