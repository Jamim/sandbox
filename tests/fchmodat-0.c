#define FUNC fchmodat
#define SFUNC "fchmodat"
#define FUNC_STR "%i, \"%s\", %o, %x"
#define FUNC_IMP dirfd, file, mode, flags
#define ARG_CNT 4
#define ARG_USE "<dirfd> <file> <mode> <flags>"

#define process_args() \
	s = argv[i++]; \
	int dirfd = atoi(s); \
	\
	s = argv[i++]; \
	char *file = s; \
	\
	s = argv[i++]; \
	mode_t mode; \
	sscanf(s, "%i", &mode); \
	\
	s = argv[i++]; \
	int flags = 0; \
	sscanf(s, "%i", &flags);

#include "test-skel-0.c"