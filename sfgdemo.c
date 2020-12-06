
/*
 *  Copyright t lefering
 *  parts are (C) Universitaet Passau 1986-1991
 *  parts are Copyright (C) 1998-2021 Free Software Foundation, Inc.
 *  parts are Copyright (C) Felix von Leitner from dietlibc
 *
 *  https://notabug.org/mooigraph/sfgraph
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

/* demo program how to use the sfg.c Single File Graph layout file
 * compile with gcc sfgdemo.c sfg.c -o sfgdemo
 * usage:
 * ./sfgdemo
 * ./sfgdemo input.txt output.ps
 */

#include <stdio.h>
#include <string.h>

#include "sfg.h"

static char *namein = NULL;
static char *nameout = NULL;
static FILE *fin = NULL;
static FILE *fos = NULL;

static char lbuf[1024];

static void readfile(void);
static void head(void);
static void tail(void);

/* this gets the node data after layout */
static int getnodedata(int num, int level, int pos, void *data, int xpos, int ypos, int tx, int ty, int nselfedges, int type,
		       int indegree, int outdegree, int ly0, int ly1);

/* this gets the node data after layout */
static int getnodedataps(int num, int level, int pos, void *data, int xpos, int ypos, int tx, int ty, int nselfedges, int type,
			 int indegree, int outdegree, int ly0, int ly1);

/* this gets the edge data after layout */
static int getedgedata(int num, int from, int to, void *data, int type, int rev);

/* this gets the edge data after layout */
static int getedgedataps(int num, int from, int to, void *data, int type, int rev);

int main(int argc, char *argv[])
{
	int status = 0;
	int nn = 0;
	int ne = 0;

	if (argc == 1) {
		namein = "sfgdemo.txt";
		nameout = "sfgdemo.svg";
	} else if (argc == 3) {
		namein = argv[1];
		nameout = argv[2];
	} else {
		printf("Usage: ./sfgdemo input.txt output.svg or ./sfgdemo\n");
		return (0);
	}

	if (argc == 1) {
		printf("demo of sfg Single File Graph layout library version %d\n", sfg_version());

		/* init to start */
		status = sfg_init();
		if (status < 0) {	/* error */
		}

		/* add few nodes starting at 1, no user data, small texlabel area */
		for (nn = 1; nn < 4; nn++) {
			status = sfg_addnode(nn, 25, 20, NULL);
			if (status < 0) {	/* error */
				break;
			}
		}

		/* add few edges with number starting at 1 */
		ne++;
		status = sfg_addedge(ne, /* from */ 1, /* to */ 2, 0, 0, NULL);
		if (status < 0) {	/* error */
		}

		ne++;
		status = sfg_addedge(ne, /* from */ 1, /* to */ 3, 10, 10, NULL);
		if (status < 0) {	/* error */
		}

		/* set min (x,y) spacing of nodes */
		status = sfg_xspacing(5);
		if (status < 0) {	/* error */
		}

		status = sfg_yspacing(15);
		if (status < 0) {	/* error */
		}

		/* run layout */
		status = sfg_layout();
		if (status < 0) {	/* error */
		}

		printf("drawing has size (%d,%d) with %d levels\n", sfg_maxx(), sfg_maxy(), sfg_nlevels());

		/* get node data */
		status = sfg_node_foreach(getnodedata);
		if (status < 0) {	/* error */
		}

		/* get edge data */
		status = sfg_edge_foreach(getedgedata);
		if (status < 0) {	/* error */
		}

		/* de-init to finish */
		status = sfg_deinit();
		if (status < 0) {	/* error */
		}
	}

	/* now run it on file */
	fin = fopen(namein, "r");
	fos = fopen(nameout, "w");

	if (fin == NULL) {
		printf("cannot open file %s for reading\n", namein);
		return (0);
	}
	if (fos == NULL) {
		printf("cannot open file %s for writing\n", nameout);
		return (0);
	}

	fflush(stdout);

	/* re-init and read input file, then generate postscript */

	/* init to start */
	status = sfg_init();
	if (status < 0) {	/* error */
	}

	readfile();

	/* run layout */
	status = sfg_layout();
	if (status < 0) {	/* error */
	}

	printf("drawing has size (%d,%d) with %d levels and %d nodes with %d edges\n", sfg_maxx(), sfg_maxy(), sfg_nlevels(),
	       sfg_nnodes(), sfg_nedges());

	head();

	/* get node data */
	status = sfg_node_foreach(getnodedataps);
	if (status < 0) {	/* error */
	}

	/* get edge data */
	status = sfg_edge_foreach(getedgedataps);
	if (status < 0) {	/* error */
	}

	tail();

	/* de-init to finish */
	status = sfg_deinit();
	if (status < 0) {	/* error */
	}

	return (0);
}

/* this gets the node data after layout */
static int getnodedata(int num, int level, int pos, void *data, int xpos, int ypos, int tx, int ty, int nselfedges, int type,
		       int indegree, int outdegree, int ly0, int ly1)
{
	/* return(1); to stop the callback */
	if (data) {
	}
	if (xpos) {
	}
	if (ypos) {
	}
	if (tx) {
	}
	if (ty) {
	}
	if (nselfedges) {
	}
	if (type) {
	}
	if (indegree) {
	}
	if (outdegree) {
	}
	if (ly0) {
	}
	if (ly1) {
	}
	printf("node %d relative pos (%d,%d) in/outdegree %d:%d final pos (%d,%d)\n", num, pos, level, indegree, outdegree, xpos,
	       ypos);
	/* return 0 to continue callbacks */
	return (0);
}

/* this gets the edge data after layout */
static int getedgedata(int num, int from, int to, void *data, int type, int rev)
{
	/* return(1); to stop the callback */
	if (data) {
	}
	if (type) {
	}
	printf("edge %d from node %d to %d reversed=%d\n", num, from, to, rev);
	/* return 0 to continue callbacks */
	return (0);
}

static void readfile(void)
{
	int ch = 0;
	int i = 0;
	int n = 0;
	int fn = 0;
	int tn = 0;
	int status = 0;
	int ne = 0;
	for (;;) {
		/* fill buffer */
		memset(lbuf, 0, 1024);
		i = 0;
		for (;;) {
			ch = fgetc(fin);
			if (ch == EOF) {
				break;
			}
			if (ch == '\n') {
				break;
			}
			lbuf[i] = ch;
			if (i > 1023) {
				break;
			}
			i++;
		}
		/* check data in buffer */
		if (strlen(lbuf)) {
			if (lbuf[0] == '#') {
				/* comment line */ } else {
				n = sscanf(lbuf, "%d %d", &fn, &tn);
				if (n == 1) {
					/* node */
					printf("parsed node number %d\n", fn);
					status = sfg_addnode(fn, 25, 20, NULL);
					if (status < 0) {	/* error */
						break;
					}
				} else if (n == 2) {
					/* edge */
					printf("parsed edge from node %d to node %d\n", fn, tn);
					ne++;
					status = sfg_addedge(ne, /* from */ fn, /* to */ tn, 0, 0, NULL);
					if (status < 0) {	/* error */
					}
				} else {
					/* skip line */
				}
			}
		}
		if (ch == EOF) {
			break;
		}
	}
	/* testing add few single nodes */
	if (0) {
		status = sfg_addnode(100, 25, 20, NULL);
		if (status < 0) {	/* error */
		}
		status = sfg_addnode(101, 25, 40, NULL);
		if (status < 0) {	/* error */
		}
		status = sfg_addnode(102, 25, 30, NULL);
		if (status < 0) {	/* error */
		}

	}
	return;
}

/* this gets the node data after layout, see sfg.h */
static int getnodedataps(int num, int level, int pos, void *data, int xpos, int ypos, int tx, int ty, int nselfedges, int type,
			 int indegree, int outdegree, int ly0, int ly1)
{

	/* return(1); to stop the callback */

	if (num) {
	}
	if (level) {
	}
	if (pos) {
	}
	if (data) {
	}
	if (xpos) {
	}
	if (ypos) {
	}
	if (nselfedges) {
		/* self edges at the node must be drawn here */
	}
	if (type) {
	}
	if (indegree) {
	}
	if (outdegree) {
	}
	if (ly0) {
	}
	if (ly1) {
	}
	fprintf(fos,
		"  <rect id=\"node_%d\" height=\"%d\" width=\"%d\" y=\"%d\" x=\"%d\" stroke-linecap=\"null\" stroke-linejoin=\"null\" stroke-dasharray=\"null\" stroke=\"#000000\" fill=\"#7fff00\"/>\n",
		num, ty, tx, ypos, xpos);

	if (tx && ty) {
		fprintf(fos, "  <text x=\"%d\" y=\"%d\" class=\"small\">%d</text>\n", xpos, ypos + ty, num);
	}

	/* return 0 to continue callbacks */
	return (0);
}

/* this gets the edge data after layout, see sfg.h */
static int getedgedataps(int num, int from, int to, void *data, int type, int rev)
{
	int fx = 0;
	int fy = 0;
	int tx = 0;
	int ty = 0;
	int typef = 0;
	int typet = 0;
	int szxf = 0;
	int szyf = 0;
	int szxt = 0;
	int szyt = 0;
	int ly = 0;
	int ryf = 0;
	int ryt = 0;
	char *scolor = "";

	/* todo this should also guide the dummy nodes to get a better drawing */

	if (num) {
	}

	if (rev) {
		/* draw arrow at from-node */
		scolor = "#0f0fff";
	} else {
		/* draw arrow at to-node */
		scolor = "#000000";
	}

	/* return(1); to stop the callback */
	if (data) {
		/* optional edge data */
	}

	/* get type of nodes */
	typef = sfg_nodetype(from);
	typet = sfg_nodetype(to);

	/* get (x,y) pos of nodes */
	fx = sfg_nodexpos(from);
	fy = sfg_nodeypos(from);
	tx = sfg_nodexpos(to);
	ty = sfg_nodeypos(to);

	/* get sizes of nodes */
	szxf = sfg_nodexsize(from);
	szyf = sfg_nodeysize(from);
	szxt = sfg_nodexsize(to);
	szyt = sfg_nodeysize(to);

	/* relative y positions of from/to node */
	ryf = sfg_noderelypos(from);
	ryt = sfg_noderelypos(to);

	if (ryf) {
	}
	if (ryt) {
	}

	/* type of edge */
	if (type == 3) {

		/* this horizontal edge must be drawn different */
		if (fy + szyf > ty + szyt) {
			ly = fy + szyf;
		} else {
			ly = ty + szyt;
		}
		ly = ly + 4;
		fx = fx + (szxf / 2);
		fy = fy + szyf;
		tx = tx + (szxt / 2);
		ty = ty + szyt;

		fprintf(fos,
			"  <line id=\"edge_%d\" y2=\"%d\" x2=\"%d\" y1=\"%d\" x1=\"%d\" stroke-linecap=\"null\" stroke-linejoin=\"null\" stroke-dasharray=\"null\" stroke-width=\"2\" stroke=\"%s\" fill=\"none\"/>\n",
			num, ly, fx, fy, fx, scolor);

		fprintf(fos,
			"  <line id=\"edge_%d\" y2=\"%d\" x2=\"%d\" y1=\"%d\" x1=\"%d\" stroke-linecap=\"null\" stroke-linejoin=\"null\" stroke-dasharray=\"null\" stroke-width=\"2\" stroke=\"%s\" fill=\"none\"/>\n",
			num, ly, tx, ly, fx, scolor);

		fprintf(fos,
			"  <line id=\"edge_%d\" y2=\"%d\" x2=\"%d\" y1=\"%d\" x1=\"%d\" stroke-linecap=\"null\" stroke-linejoin=\"null\" stroke-dasharray=\"null\" stroke-width=\"2\" stroke=\"%s\" fill=\"none\"/>\n",
			num, ty, tx, ly, tx, scolor);

	} else {

		/* if from node is regular, tune the edge conn. point */
		if (typef == 1) {
			fx = fx + (szxf / 2);
			fy = fy + szyf;
		}

		/* if to node is regular, tune the edge conn. point */
		if (typet == 1) {
			tx = tx + (szxt / 2);
			if (szyt) {
			}
		}

		fprintf(fos,
			"  <line id=\"edge_%d\" y2=\"%d\" x2=\"%d\" y1=\"%d\" x1=\"%d\" stroke-linecap=\"null\" stroke-linejoin=\"null\" stroke-dasharray=\"null\" stroke-width=\"2\" stroke=\"%s\" fill=\"none\"/>\n",
			num, ty, tx, fy, fx, scolor);
	}

	/* return 0 to continue callbacks */
	return (0);
}

/* header */
static void head(void)
{

	/* make image +5 y pixels larger to allow for horizontal edge */
	fprintf(fos,
		"<svg width=\"%d\" height=\"%d\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:svg=\"http://www.w3.org/2000/svg\">\n",
		sfg_maxx() + 1, sfg_maxy() + 5);
	fprintf(fos, "%s\n", " <g>");
	fprintf(fos, "%s\n", "  <title>sfgdemo</title>");
	fprintf(fos, "%s\n", "  <style>");
	fprintf(fos, "%s\n", "    .small { font: italic 15px sans-serif; }");
	fprintf(fos, "%s\n", "  </style>");

	return;
}

/* footer */
static void tail(void)
{
	fprintf(fos, "%s\n", " </g>");
	fprintf(fos, "%s\n", "</svg>");

	return;
}

/* end */
