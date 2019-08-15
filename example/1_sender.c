#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "garlic.h"

void body()
{
	char buffer[MAXINPUT], hash[HASHSIZE + 1];
	int action = 0;

	// Get current action.
	printf("1 or 2... : ");
	scanf("%d", &action);

	memset(hash, 0x00, HASHSIZE + 1);

	// Exit.
	if (action == 2) {
		tgn_stop();
		return;
	}

	// If actions isn't a sending.
	if (action != 1) {
		return;
	}

	// Get message for sending (length - 10).
	printf("Write message: ");
	scanf("%10s", buffer);

	// Get address to send.
	printf("Write address: ");
	scanf("%32s", hash);

	// Send package.
	tgn_send_msg(hash, 10, (unsigned char *)buffer);
}

int main()
{
	// Init.
	the_garlic_network();
	// Cycle.
	while (tgn_is_working()) {
		body();
	}

	return 0;
}
