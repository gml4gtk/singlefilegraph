
/*
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
 * SPDX-License-Identifier: GPL-3.0+
 * License-Filename: LICENSE
 */

/* convert els file to sfgdemo file, ./els2sfg input.els output.txt */

#include <stdio.h>
#include <string.h>
#include <libgen.h>

char *ifn = NULL;
char *ofn = NULL;

FILE *fi = NULL;
FILE *fo = NULL;

int els_lineno = 1;
char els_buf[1024];

int i = 0;
int n = 0;
int ncount = 0;
int ecount = 0;
int dcount = 0;

int fnnumber = 0;
int tnnumber = 0;


int
main (int argc, char *argv[])
{

  if (argc != 3)
    {
      printf ("missing file names:\nusage: ./els2sfg input.els output.txt\n");
      return (1);
    }

  ifn = argv[1];
  ofn = argv[2];

  fi = fopen (ifn, "r");
  if (fi == NULL)
    {
      printf ("cannot open file %s for reading\n", ifn);
      return (1);
    }

  fo = fopen (ofn, "w");
  if (ofn == NULL)
    {
      printf ("cannot open file %s for writing\n", ofn);
      return (1);
    }

  els_lineno = 1;
  memset (els_buf, 0, 1024);


  /* get first line with 3 numbers */
  if (fgets (els_buf, 1024, fi) == NULL)
    {
      printf ("error reading first line file %s\n", ifn);
      return (1);
    }

  /* read the first line with numbers, node-count, edge-count, deleted-edge count */
  n = sscanf (els_buf, "%d %d %d", &ncount, &ecount, &dcount);

  if ((n != 3) || (n == EOF))
    {
      printf ("no 3 number found on first line in file %s\n", ifn);
      return (1);
    }

  fprintf (fo, "# this file has %d nodes, %d edges\n", ncount, ecount);

  for (i = 0; i < ncount; i++)
    {
      fprintf (fo, "%d\n", i + 1);
    }

  /* read the edge lines */
  for (i = 0; i < ecount; i++)
    {

      els_lineno = els_lineno + 1;
      memset (els_buf, 0, 1024);

      /* read edge info line */
      if (fgets (els_buf, 1024, fi) == NULL)
	{
	  printf ("error reading line %d in file %s\n", els_lineno, ifn);
	  return (1);
	}

      /* read edge data */
      n = sscanf (els_buf, "%d %d", &fnnumber, &tnnumber);

      if ((n != 2) || (n == EOF))
	{
	  printf ("no 2 node number on line %d in file %s\n", els_lineno,
		  ifn);
	  return (1);
	}

      fprintf (fo, "%d %d\n", fnnumber + 1, tnnumber + 1);

      /* to the next edge */
    }

  (void) fclose (fi);
  (void) fclose (fo);

  return (0);
}

/* end */
