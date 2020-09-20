#ifndef __COMMON_H__
#define __COMMON_H__

extern int control_msg_level;

#define VERSION "1.0"

#define MSG(level, format, ...)						\
	do {								\
		if (level >= control_msg_level) {			\
			printf(format, ##__VA_ARGS__);			\
		}							\
	} while(0)

#endif
