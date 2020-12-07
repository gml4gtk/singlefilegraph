# demo program to run sfg layout using python
# first run make swigpython to create the _sfg.so file

import sfg

# get the version number of sfg.c
version=sfg.sfg_version()

print("sfg.c version number is " + str(version));

# all routines return status <0 at error
# and the error codes are in sfg.h found

# first do init
status=sfg.sfg_init();

# a parser must first add all nodes then edges
# node and edge numbers must start with 1 and increase by 1

# add node 1,2 and 3 with size (20,10)
status=sfg.sfg_addnode(1,20,10);
status=sfg.sfg_addnode(2,20,10);
status=sfg.sfg_addnode(3,20,10);

# add edge 1 and 2 from node 1 to 2 and 1 to 3
# the last two numbers are optional size of edgelabel or 0 if no edgelabel
status=sfg.sfg_addedge(1,1,2,0,0);
status=sfg.sfg_addedge(2,1,3,0,0);

# run sugiyama barycenter layout
status=sfg.sfg_layout();

# get lowest and higest node number in use after layout */
nodemin=sfg.sfg_nodemin();
nodemax=sfg.sfg_nodemax();

print("node numbers are in range " + str(nodemin) + " ... " + str(nodemax));

# sfgdmo.c is using a callback routine in c to get the data
# this is manaul getting the node and edge data

for nn in range (nodemin, nodemax+1):
	xpos=sfg.sfg_nodexpos(nn);
	ypos=sfg.sfg_nodeypos(nn);
	print("node " + str(nn) + " is at position (" + str(xpos) +"," + str(ypos) + ")");

# get lowest and higest edge number in use after layout */
edgemin=sfg.sfg_edgemin();
edgemax=sfg.sfg_edgemax();

print("edge numbers are in range " + str(edgemin) + " ... " + str(edgemax));

for ne in range (edgemin, edgemax+1):
	fn=sfg.sfg_edgefrom(ne);
	tn=sfg.sfg_edgeto(ne);
	print("edge "+ str(ne) + " is between node " + str(fn) + " and node " + str(tn));

# sfgdemo.c has example how to create svg image date
# gml4gtk has in main.c example how to using cairo lib

# final do de-init and all memory is free()'d
status=sfg.sfg_deinit();
