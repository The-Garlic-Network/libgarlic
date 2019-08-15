#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "garlic.h"
/*
 *	Callback function.
 */
void fn_callback(char *hash, unsigned char *buff,
	size_t len)
{
	char msg[MAXINPUT];
	// Copy bytes to string buffer.
	memcpy(msg, buff, len);
	buff[len] = 0x00;

	printf("New package from %s:\n%s\n\n", hash, msg);
}

int main()
{
	// Init.
	the_garlic_network();
	// Set callback func.
	tgn_callback(fn_callback);
	// Cycle.
	while (tgn_is_working());

	return 0;
}
