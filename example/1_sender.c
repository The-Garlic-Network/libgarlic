#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "garlic.h"

void body()
{
	char *buffer, hash[HASHSIZE + 1];
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

	// Get address to send.
	printf("Write address: ");
	scanf("%s", hash);

	// Text for sending.
	buffer = "Hey dude!";

	// Send package.
	tgn_send_msg(hash, strlen(buffer), (unsigned char *)buffer);
}

int main()
{
	char *hash;
	// Init.
	the_garlic_network();
	// Getting own hash.
	hash = tgn_myhash();
	printf("My hash is %s\n", hash);
	// Cycle.
	while (tgn_is_working()) {
		body();
	}

	return 0;
}