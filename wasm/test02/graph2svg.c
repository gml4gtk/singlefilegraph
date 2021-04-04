
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
#include <sfg.h>

struct inedges {
	int fn;
	int tn;
	struct inedges *next;
};

/* no graph data output svg */
char *nodata = "<svg width=\"120\" height=\"20\">"
    "    <rect width=\"100%\" height=\"100%\" fill=\"#eceedc\" />"
    "    <text x=\"5\" y=\"15\" font-family=\"Verdana\" font-size=\"15\""
    "          fill=\"white\" stroke=\"black\" stroke-width=\"1\">" "No graph data" "    </text>" "  </svg>";

/* syntax error output svg */
char *syntaxerror1 = "<svg width=\"300\" height=\"20\">"
    "    <rect width=\"100%\" height=\"100%\" fill=\"#eceedc\" />"
    "    <text x=\"5\" y=\"15\" font-family=\"Verdana\" font-size=\"15\""
    "          fill=\"white\" stroke=\"black\" stroke-width=\"1\">" "Syntax error at line ";

char *syntaxerror2 = "    </text>" "  </svg>";

/* sfg error output svg */
char *sfgstatus1 = "<svg width=\"300\" height=\"20\">"
    "    <rect width=\"100%\" height=\"100%\" fill=\"#eceedc\" />"
    "    <text x=\"5\" y=\"15\" font-family=\"Verdana\" font-size=\"15\""
    "          fill=\"white\" stroke=\"black\" stroke-width=\"1\">" "Status from sfg.c is ";

char *sfgstatus2 = "    </text>" "  </svg>";

/* no. char in input */
unsigned long ninchars = 0;

/* */
int nrealloc = 0;

/* input pointer */
int ip = 0;
char ibuf[1024];
char lbuf[256];

/* current input line */
int curline = 0;
int curie = 0;

/* output buffer */
int stepobuf = 500 * 1024;
int szobuf = 0;
char *obuf = NULL;
int uobuf = 0;
char *pobuf = NULL;

struct inedges *ie = NULL;
struct inedges *ieend = NULL;

/* forward decl. */
static int parse(void);
static void setparseerror(void);
static void setnodata(void);
static void setsfgstatus(int status);
static void init_obuf(void);
static void realloc_obuf(int len);
static void str2obuf(char *s);
static int nextline(void);
static int tosfg(void);
static int fromsfg(int xs, int ys);
static void ohead(int xs, int ys);
static void otail(void);
static int getnodedataps(int num, int level, int pos, int xpos, int ypos, int tx, int ty, int nselfedges, int type,
			 int indegree, int outdegree, int ly0, int ly1);
static int getedgedataps(int num, int from, int to, int type, int rev);

int graph2svg(char *str, char *output)
{
	int i = 0;
	int status = 0;
	struct inedges *pie = NULL;
	/* first input checks */
	if (str == (char *)0) {
		/* shouldnothappen */
		strcpy(output, nodata);
		return (99);
	}
	/* how long is input string */
	ninchars = strlen(str);
	/* minimal graph is "0 1" edge */
	if (ninchars < 3) {
		/* shouldnothappen */
		strcpy(output, nodata);
		return (99);
	}
	/* copy str to static buffer before first calloc()
	 * todo the memory in str gets damaged by calloc()
	 */
	strcpy(ibuf, str);
	/* set start of input */
	init_obuf();
	ip = 0;
	curline = 0;
	status = parse();

	if (status || 0) {
		setparseerror();
		strcpy(output, obuf);
		return (strlen(obuf));
	}

	pie = ie;
	curie = 0;
	while (pie) {
		curie++;
		pie = pie->next;
	}

	if (curie == 0) {
		setnodata();
		strcpy(output, obuf);
		return (strlen(obuf));
	}

	status = sfg_init();

	if (status || 0) {
		setsfgstatus(status);
		strcpy(output, obuf);
		return (strlen(obuf));
	}

	status = tosfg();

	if (status || 0) {
		setsfgstatus(status);
		strcpy(output, obuf);
		return (strlen(obuf));
	}

	status = sfg_layout();

	if (status || 0) {
		setsfgstatus(status);
		strcpy(output, obuf);
		return (strlen(obuf));
	}

	status = fromsfg(sfg_maxx(), sfg_maxy());

	if (status || 0) {
		setsfgstatus(status);
		strcpy(output, obuf);
		return (strlen(obuf));
	}

	status = sfg_deinit();

	if (status || 0) {
		setsfgstatus(status);
		strcpy(output, obuf);
		return (strlen(obuf));
	}

	if (strlen(obuf) == 0) {
		strcpy(obuf, "oke");
	}

	/* copy the data in svgdata() below 
	 * memcpy(output, obuf,strlen(obuf));
	 */

	/* return no. of chars in output svg data */
	return (strlen(obuf));
}

int svgdata(char *output, int len)
{
	int ret = strlen(obuf);
	memcpy(output, obuf, len);
	return (ret);
}

static void setparseerror(void)
{
	memset(obuf, 0, szobuf);
	str2obuf(syntaxerror1);
	str2obuf(intoa(curline));
	str2obuf(syntaxerror2);
	return;
}

static void setnodata(void)
{
	memset(obuf, 0, szobuf);
	str2obuf(nodata);
	return;
}

static void setsfgstatus(int status)
{
	memset(obuf, 0, szobuf);
	str2obuf(sfgstatus1);
	str2obuf(intoa(status));
	str2obuf(sfgstatus2);
	return;
}

/* parse input pointed at ip, return 0 if oke */
static int parse(void)
{
	int ll = 0;
	char lnum[16];
	char rnum[16];
	char *p = NULL;
	int len = 0;
	int fnn = 0;
	int tnn = 0;
	int ac = 0;
	struct inedges *newie = NULL;
	for (;;) {
		ll = nextline();
		if (ll == 0) {
			break;
		}
		curline++;
		/* check if line with only spaces */
		p = lbuf;
		ac = 0;
		while ((*p)) {
			if ((*p) != ' ') {
				ac++;
				break;
			}
			p++;
		}
		if (ac == 0) {
			/* only spaces */
			lbuf[0] = 0;
		}
		if (lbuf[0] == '#') {
			/* comment line to skip */
		} else if (lbuf[0] == 0) {
			/* empty line with only '\n' */
		} else {
			/* minimum edge entry is 3 chars as in "2 3" */
			if (strlen(lbuf) < 3) {
				return (1);
			}
			p = lbuf;
			memset(lnum, 0, 16);
			memset(rnum, 0, 16);
			/* skip leading spaces */
			while ((*p) != 0) {
				if ((*p) != ' ') {
					break;
				}
				p++;
			}
			/* copy only max 8 number chars for node number from-node */
			len = 0;
			while ((*p) != 0) {
				/* stop at seperator char */
				if ((*p) == ' ') {
					break;
				}
				if (((*p) >= '0') && ((*p) <= '9')) {
					lnum[len] = (*p);
					len++;
					if (len > 8) {
						return (1);
					}
				} else {
					return (1);
				}
				p++;
			}
			/* at separator char now */
			/* skip leading spaces */
			while ((*p) != 0) {
				if ((*p) != ' ') {
					break;
				}
				p++;
			}
			/* copy only max 8 number chars for node number to-node */
			len = 0;
			while ((*p) != 0) {
				/* stop at seperator char */
				if ((*p) == ' ') {
					break;
				}
				if (((*p) >= '0') && ((*p) <= '9')) {
					rnum[len] = (*p);
					len++;
					if (len > 8) {
						return (1);
					}
				} else {
					return (1);
				}
				p++;
			}
			/* any more chars on line after two node numbers are silently skipped */
			if (strlen(lnum) < 1) {
				return (1);
			}
			if (strlen(rnum) < 1) {
				return (1);
			}
			fnn = atoi(lnum);
			tnn = atoi(rnum);
 if(fnn==0 || tnn==0){
 /* edges with a node number 0 are skipped. */
}else			if (fnn == tnn) {
				/* skip currently self-edges as in "4 4" */
			} else {
				newie = calloc(1, sizeof(struct inedges));
				newie->fn = fnn;
				newie->tn = tnn;
				if (ie == NULL) {
					ie = newie;
					ieend = newie;
				} else {
					ieend->next = newie;
					ieend = newie;
				}
			}
		}
	}
	/* status parsed oke. */
	return (0);
}

static int nextline(void)
{
	int len = 0;
	char c = 0;
	memset(lbuf, 0, 256);
	len = 0;
	for (;;) {
		c = ibuf[ip];
		if (c == 0) {
			break;
		}
		if (c == '\n') {
			ip++;
			len++;
			break;
		}
		/* only copy first 255 chars of one line */
		if (len < (256 - 1)) {
			lbuf[len] = c;
			len++;
		}
		ip++;
	}

	return (len);
}

static void init_obuf(void)
{
	if (obuf) {
		free(obuf);
		obuf = NULL;
	}
	obuf = calloc(1, stepobuf);
	szobuf = stepobuf;
	uobuf = 0;
	pobuf = obuf;
	return;
}

static void realloc_obuf(int len)
{
	obuf = realloc(obuf, szobuf + stepobuf + len + 1);
	/* todo:
	   char *old = NULL;
	   int oldsize = szobuf;
	   old = calloc(1, szobuf);
	   memcpy(old, obuf, szobuf);
	   free(obuf);
	   obuf = calloc(1, szobuf + stepobuf + len + 1);
	   memcpy(obuf, old, oldsize);
	   free(old);
	   szobuf += stepobuf + len + 1;
	 */
	nrealloc++;
	return;
}

void str2obuf(char *s)
{
	int len = 0;
	char *ptr = 0;
	if (s == NULL) {
		return;
	}
	if (*s == 0) {
		return;
	}
	len = strlen(s);
	if ((uobuf + len) > szobuf) {
		realloc_obuf(len);
	}
	if (uobuf == 0) {
		strcpy(obuf, s);
	} else {
		strcat(obuf, s);
	}
	uobuf += len;
	return;
}

static int tosfg(void)
{
	int status = 0;
	int en = 0;
	struct inedges *pie = NULL;
	/* first add all nodes */
	pie = ie;
	while (pie) {
		status = sfg_addnode(pie->fn, /* tx */ 25, /* ty */ 30);
		status = sfg_addnode(pie->tn, /* tx */ 25, /* ty */ 30);
		pie = pie->next;
	}

	en = 0;

	pie = ie;
	while (pie) {
		en++;
		status = sfg_addedge( /* number */ en, /* from */ pie->fn, /* to */ pie->tn, /* tx */ 0, /* ty */ 0);
		pie = pie->next;
	}

	return (0);
}

/* this gets the node data after layout, see sfg.h */
static int getnodedataps(int num, int level, int pos, int xpos, int ypos, int tx, int ty, int nselfedges, int type,
			 int indegree, int outdegree, int ly0, int ly1)
{

	/* return(1); to stop the callback */

	if (num) {
	}
	if (level) {
	}
	if (pos) {
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

	/*
	 * "  <rect id=\"node_%d\" height=\"%d\" width=\"%d\" y=\"%d\" x=\"%d\" stroke-linecap=\"null\" stroke-linejoin=\"null\" stroke-dasharray=\"null\" stroke=\"#000000\" fill=\"#7fff00\"/>\n",
	 *             num, ty, tx, ypos, xpos);
	 */

	str2obuf("  <rect id=\"node_");
	str2obuf(intoa(num));
	str2obuf("\" height=\"");
	str2obuf(intoa(ty));
	str2obuf("\" width=\"");
	str2obuf(intoa(tx));
	str2obuf("\" y=\"");
	str2obuf(intoa(ypos));
	str2obuf("\" x=\"");
	str2obuf(intoa(xpos));
	str2obuf("\" fill=\"#7fff00\"/>\n");

	if (tx && ty) {
		/*
		 * "  <text x=\"%d\" y=\"%d\" class=\"small\">%d</text>\n", xpos, ypos + ty, num);
		 */

		str2obuf("  <text x=\"");
		str2obuf(intoa(xpos));
		str2obuf("\" y=\"");
		str2obuf(intoa(ypos + ty));
		str2obuf("\" class=\"small\">");
		str2obuf(intoa(num));
		str2obuf("</text>\n");

	}

	/* return 0 to continue callbacks */
	return (0);
}

/* this gets the edge data after layout, see sfg.h */
static int getedgedataps(int num, int from, int to, int type, int rev)
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

		/*
		 * "  <line id=\"edge_%d\" y2=\"%d\" x2=\"%d\" y1=\"%d\" x1=\"%d\" stroke-linecap=\"null\" stroke-linejoin=\"null\" stroke-dasharray=\"null\" stroke-width=\"2\" stroke=\"%s\" fill=\"none\"/>\n",
		 * num, ly, fx, fy, fx, scolor);
		 */
		str2obuf("  <line id=\"edge_");
		str2obuf(intoa(num));
		str2obuf("\" y2=\"");
		str2obuf(intoa(ly));
		str2obuf("\" x2=\"");
		str2obuf(intoa(fx));
		str2obuf("\" y1=\"");
		str2obuf(intoa(fy));
		str2obuf("\" x1=\"");
		str2obuf(intoa(fx));
		str2obuf("\" stroke-width=\"2\" stroke=\"");
		str2obuf(scolor);
		str2obuf("\" fill=\"none\"/>\n");

		/*
		 * "  <line id=\"edge_%d\" y2=\"%d\" x2=\"%d\" y1=\"%d\" x1=\"%d\" stroke-linecap=\"null\" stroke-linejoin=\"null\" stroke-dasharray=\"null\" stroke-width=\"2\" stroke=\"%s\" fill=\"none\"/>\n",
		 * num, ly, tx, ly, fx, scolor);
		 */
		str2obuf("  <line id=\"edge_");
		str2obuf(intoa(num));
		str2obuf("\" y2=\"");
		str2obuf(intoa(ly));
		str2obuf("\" x2=\"");
		str2obuf(intoa(tx));
		str2obuf("\" y1=\"");
		str2obuf(intoa(ly));
		str2obuf("\" x1=\"");
		str2obuf(intoa(fx));
		str2obuf("\" stroke-width=\"2\" stroke=\"");
		str2obuf(scolor);
		str2obuf("\" fill=\"none\"/>\n");

		/*
		 * "  <line id=\"edge_%d\" y2=\"%d\" x2=\"%d\" y1=\"%d\" x1=\"%d\" stroke-linecap=\"null\" stroke-linejoin=\"null\" stroke-dasharray=\"null\" stroke-width=\"2\" stroke=\"%s\" fill=\"none\"/>\n",
		 * num, ty, tx, ly, tx, scolor);
		 */
		str2obuf("  <line id=\"edge_");
		str2obuf(intoa(num));
		str2obuf("\" y2=\"");
		str2obuf(intoa(ty));
		str2obuf("\" x2=\"");
		str2obuf(intoa(tx));
		str2obuf("\" y1=\"");
		str2obuf(intoa(ly));
		str2obuf("\" x1=\"");
		str2obuf(intoa(tx));
		str2obuf("\" stroke-width=\"2\" stroke=\"");
		str2obuf(scolor);
		str2obuf("\" fill=\"none\"/>\n");

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
		/*
		 * "  <line id=\"edge_%d\" y2=\"%d\" x2=\"%d\" y1=\"%d\" x1=\"%d\" stroke-linecap=\"null\" stroke-linejoin=\"null\" stroke-dasharray=\"null\" stroke-width=\"2\" stroke=\"%s\" fill=\"none\"/>\n",
		 * num, ty, tx, fy, fx, scolor);
		 */
		str2obuf("  <line id=\"edge_");
		str2obuf(intoa(num));
		str2obuf("\" y2=\"");
		str2obuf(intoa(ty));
		str2obuf("\" x2=\"");
		str2obuf(intoa(tx));
		str2obuf("\" y1=\"");
		str2obuf(intoa(fy));
		str2obuf("\" x1=\"");
		str2obuf(intoa(fx));
		str2obuf("\" stroke-width=\"2\" stroke=\"");
		str2obuf(scolor);
		str2obuf("\" fill=\"none\"/>\n");

	}

	/* return 0 to continue callbacks */
	return (0);
}

static int fromsfg(int xs, int ys)
{
	int status = 0;

	memset(obuf, 0, szobuf);
	ohead(xs, ys);

	/* get node data */
	status = sfg_node_foreach(getnodedataps);
	if (status < 0) {	/* error */
	}

	/* get edge data */
	status = sfg_edge_foreach(getedgedataps);
	if (status < 0) {	/* error */
	}

	otail();

	return (0);
}

/* header */
static void ohead(int xsize, int ysize)
{

	str2obuf("<svg width=\"");
	str2obuf(intoa(xsize));
	str2obuf("\" height=\"");
	str2obuf(intoa(ysize));
	str2obuf("\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:svg=\"http://www.w3.org/2000/svg\">\n");
	str2obuf(" <g>\n");
	str2obuf("  <title>sfgdemo</title>\n");
	str2obuf("  <style>\n");
	str2obuf("    .small { font: italic 15px sans-serif; }\n");
	str2obuf("  </style>\n");
	str2obuf("    <rect width=\"100%\" height=\"100%\" fill=\"#eceedc\" />");

	return;
}

/* footer */
static void otail(void)
{

	str2obuf(" </g>\n");
	str2obuf("</svg>\n");

	return;
}

/* end */
