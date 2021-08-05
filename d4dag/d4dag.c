
/*
 *  Copyright t lefering
 *  Copyright Simon Speich 2013
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  These are the four essential freedoms with GNU GPL software:
 *  1: freedom to run the program, for any purpose
 *  2: freedom to study how the program works, and change it to make it do what you wish
 *  3: freedom to redistribute copies to help your Free Software friends
 *  4: freedom to distribute copies of your modified versions to your Free Software friends
 *   ,           ,
 *  /             \
 * ((__-^^-,-^^-__))
 * `-_---'  `---_-'
 *  `--|o`   'o|--'
 *      \  `  /
 *       ): :(
 *       :o_o:
 *        "-"
 *
 * SPDX-License-Identifier: GPL-3.0+
 * License-Filename: LICENSE
 */

/**
 * all routines have prefix d4d_
 * all static routines and vars have prefix d4d___
 */

#undef D4DAG__BEGIN_DECLS
#undef D4DAG__END_DECLS
#ifdef __cplusplus
#define D4DAG__BEGIN_DECLS extern "C" {
#define D4DAG__END_DECLS }
#else
#define D4DAG__BEGIN_DECLS	/* empty */
#define D4DAG__END_DECLS	/* empty */
#endif

D4DAG__BEGIN_DECLS
/**
 * set here version number as int
 */
#define D4DAG_VERSION 10
/* */
#include <stdio.h>
#include <stdlib.h>
/* defs */
#include "d4dag.h"
/* mallocer/freeer */
typedef void *(*malloc_fn)(size_t n);
typedef void (*free_fn)(void *ptr);
/* toplevel control struct */
struct d4d__maing {
	/* wrappers for malloc/free */
	malloc_fn d4d__malloc;
	free_fn d4d__free;
};

/* main control */
static struct d4d__maing *d4d__main = (struct d4d__maing *)0;

/* forward decl. */
static void d4d__memzero(void *ptr, size_t n);
/**
 * returns version number as int
 */
int d4d_version(void)
{
	return (D4DAG_VERSION);
}

/**
 *
 * returns:
 *  0 if oke
 * -1 if missing malloc/free pointer
 * -2 if malloc failed
 * For embedded devices this source has no linkage to libraries
 * which means here pointer to malloc/free must be supplied.
 */
int d4d_init(void *(*mallocer)(size_t n), void(*freeer)(void *ptr))
{
	if(mallocer == NULL) {
		return(-1);
	}
	if (freeer == NULL) {
		return (-1);
	}
	d4d__main = (struct d4d__maing *)(*mallocer) (sizeof(struct d4d__maing));
	if (d4d__main == (struct d4d__maing *)0) {
		return (-2);
	}
	/* set pointers to malloc/free in toplevel control */
	d4d__memzero(d4d__main, sizeof(struct d4d__maing));
	d4d__main->d4d__malloc = mallocer;
	d4d__main->d4d__free = freeer;
	return (0);
}

/**
 *
 */
int d4d_deinit(void)
{
	/* check if active */
	if (d4d__main == (struct d4d__maing *)0) {
		return (0);
	}
	/* free structs */
	/* free control */
	d4d__main->d4d__free(d4d__main);
	d4d__main = (struct d4d__maing *)0;
	return (0);
}

/* like memset */
static void d4d__memzero(void *ptr, size_t n)
{
	unsigned char *p = (unsigned char *)0;
	p = (unsigned char *)ptr;
	while (n) {
		*p = 0;
		p++;
		n--;
	}
	return;
}

D4DAG__END_DECLS
/* end. */
