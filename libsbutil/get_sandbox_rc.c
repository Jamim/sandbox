/*
 * get_sandbox_rc.c
 *
 * Util functions.
 *
 * Copyright 1999-2008 Gentoo Foundation
 * Licensed under the GPL-2
 *
 * Some parts might have Copyright:
 *   Copyright (C) 2002 Brad House <brad@mainstreetsoftworks.com>
 */

#include <errno.h>
#include <stdio.h>

#include "sbutil.h"

void get_sandbox_rc(char *path)
{
	save_errno();
	snprintf(path, SB_PATH_MAX, "%s/%s", SANDBOX_BASHRC_PATH, BASHRC_NAME);
	restore_errno();
}
