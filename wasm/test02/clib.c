
/*
 *  Copyright t lefering
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
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned long strlen(const char *str)
{
	const char *p = (char *)0;
	unsigned long i = 0;
	if (str == (char *)0) {
		return (0);
	}
	p = str;
	while (*p) {
		i++;
		p++;
	}
	return (i);
}

char *strcpy(char *s1, const char *s2)
{
	char *res = s1;
	while ((*s1++ = *s2++)) {;
	}
	return (res);
}

char *strcat(char *s, const char *t)
{
	char *dest = s;
	s += strlen(s);
	for (;;) {
		if (!(*s = *t)) {
			break;
		}
		++s;
		++t;
	}
	return dest;
}

void *memset(void *dst, int s, size_t count)
{
	char *a = dst;
	count++;		/* this actually creates smaller code than using count-- */
	while (--count) {
		*a++ = s;
	}
	return dst;
}

void *memcpy(void *dst, const void *src, size_t n)
{
	void *res = dst;
	unsigned char *c1 = NULL;
	unsigned char *c2 = NULL;
	c1 = (unsigned char *)dst;
	c2 = (unsigned char *)src;
	while (n--) {
		*c1++ = *c2++;
	}
	return (res);
}

 /* reverse:  reverse string s in place */
static void reverse(char s[])
{
	int i, j;
	char c;

	for (i = 0, j = strlen(s) - 1; i < j; i++, j--) {
		c = s[i];
		s[i] = s[j];
		s[j] = c;
	}
	return;
}

static char bs[32];

char *intoa(int n)
{
	int i, sign;
	memset(bs, 0, 32);
	if ((sign = n) < 0)	/* record sign */
		n = -n;		/* make n positive */
	i = 0;
	do {			/* generate digits in reverse order */
		bs[i++] = n % 10 + '0';	/* get next digit */
	} while ((n /= 10) > 0);	/* delete it */
	if (sign < 0)
		bs[i++] = '-';
	bs[i] = '\0';
	reverse(bs);
	return (bs);
}

int atoi(const char *s)
{
	int v = 0;
	int sign = 1;
	int ret = 0;
	switch (*s) {
	case '-':
		sign = -1;	/* fall through */
	case '+':
		++s;
	}
	while ((unsigned int)((*s) - '0') < 10) {
		v = (v * 10) + ((*s) - '0');
		++s;
	}
	if (sign < 0) {
		ret = -v;
	} else {
		ret = v;
	}
	return (ret);
}
