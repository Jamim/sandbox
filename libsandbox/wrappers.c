/*
 * wrappers.c
 *
 * Function wrapping functions.
 *
 * Copyright 1999-2008 Gentoo Foundation
 * Licensed under the GPL-2
 *
 *  Partly Copyright (C) 1998-9 Pancrazio `Ezio' de Mauro <p@demauro.net>,
 *  as some of the InstallWatch code was used.
 */

#include "headers.h"
#include "sbutil.h"
#include "libsandbox.h"
#include "wrappers.h"

#if !defined(BROKEN_RTLD_NEXT) && defined(HAVE_RTLD_NEXT)
# define USE_RTLD_NEXT
#endif

/* Macro to check if a wrapper is defined, if not
 * then try to resolve it again. */
#define check_dlsym(_name, _symname, _symver) \
{ \
	int old_errno = errno; \
	if (NULL == _name) \
		_name = get_dlsym(_symname, _symver); \
	errno = old_errno; \
}

static void *libc_handle = NULL;

extern char sandbox_lib[SB_PATH_MAX];
extern bool sandbox_on;

/* Need to include the function wrappers here, as they are needed below */
#include "symbols.h"


void *get_dlsym(const char *symname, const char *symver)
{
	void *symaddr = NULL;

#if defined(USE_RTLD_NEXT)
	libc_handle = RTLD_NEXT;
#endif

	/* Checking for -1UL is significant on hardened!
	 * USE_RTLD_NEXT returns it as a sign of being unusable.
	 * However using !x or NULL checks does NOT pick it up!
	 */
#define INVALID_LIBC_HANDLE(x) (!x || NULL == x || (void *)-1UL == x)
	if (INVALID_LIBC_HANDLE(libc_handle)) {
		libc_handle = dlopen(LIBC_VERSION, RTLD_LAZY);
		if (INVALID_LIBC_HANDLE(libc_handle)) {
			fprintf(stderr, "libsandbox:  Can't dlopen libc: %s\n",
				dlerror());
			exit(EXIT_FAILURE);
		}
	}
#undef INVALID_LIBC_HANDLE

	if (NULL == symver)
		symaddr = dlsym(libc_handle, symname);
	else
		symaddr = dlvsym(libc_handle, symname, symver);
	if (!symaddr) {
		fprintf(stderr, "libsandbox:  Can't resolve %s: %s\n",
			symname, dlerror());
		exit(EXIT_FAILURE);
	}

	return symaddr;
}

int libsb_open(const char *pathname, int flags, ...)
{
	va_list ap;
	int mode = 0;
	int result = -1;

	if (flags & O_CREAT) {
		va_start(ap, flags);
		mode = va_arg(ap, int);
		va_end(ap);
	}

	check_dlsym(true_open_DEFAULT, symname_open_DEFAULT,
		    symver_open_DEFAULT);
	if (flags & O_CREAT)
		result = true_open_DEFAULT(pathname, flags, mode);
	else
		result = true_open_DEFAULT(pathname, flags);

	return result;
}

char *libsb_getcwd(char *buf, size_t size)
{
	check_dlsym(true_getcwd_DEFAULT, symname_getcwd_DEFAULT,
		    symver_getcwd_DEFAULT);

	return true_getcwd_DEFAULT(buf, size);
}
