
#include <stdio.h>
#include <stdlib.h>

#include "d4dag.h"

int main(int argc, char *argv[])
{
	int status = 0;
	if (argc) {
	}
	if (argv) {
	}
	status = d4d_init(malloc, free);
	printf("%d\n", status);
	status = d4d_deinit();
	return (0);
}
