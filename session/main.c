#include <stdio.h>
#include <stdlib.h>
#include "context.h"


int main(int argc, char** argv)
{
	struct context* context = NULL;

	context = context_create();
	wl_display_run(context->display);

	return EXIT_SUCCESS;
}