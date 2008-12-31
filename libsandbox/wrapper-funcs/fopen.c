/*
 * fopen() wrapper.
 *
 * Copyright 1999-2008 Gentoo Foundation
 * Licensed under the GPL-2
 */

#define WRAPPER_ARGS_PROTO const char *pathname, const char *mode
#define WRAPPER_ARGS pathname, mode
#define WRAPPER_SAFE() FUNCTION_SANDBOX_SAFE_OPEN_CHAR(pathname, mode)
#define WRAPPER_RET_TYPE FILE *
#define WRAPPER_RET_DEFAULT NULL

#ifndef SB_FOPEN_PRE_CHECK
#define SB_FOPEN_PRE_CHECK
static inline bool sb_fopen_pre_check(WRAPPER_ARGS_PROTO)
{
	save_errno();
	if ((NULL != mode) && (mode[0] == 'r')) {
		/* If we're trying to read, fail normally if file does not stat */
		struct stat st;
		if (-1 == stat(pathname, &st))
			return false;
	}
	restore_errno();
	return true;
}
#endif
#define WRAPPER_PRE_CHECKS() if (!sb_fopen_pre_check(WRAPPER_ARGS)) return result;

#include "__wrapper_simple.c"
