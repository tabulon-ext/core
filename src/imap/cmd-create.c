/* Copyright (C) 2002 Timo Sirainen */

#include "common.h"
#include "commands.h"

int cmd_create(struct client *client)
{
	const char *mailbox;
	int only_hiearchy;
	size_t len;

	/* <mailbox> */
	if (!client_read_string_args(client, 1, &mailbox))
		return FALSE;

	len = strlen(mailbox);
	if (mailbox[len-1] != client->storage->hierarchy_sep)
		only_hiearchy = FALSE;
	else {
		/* name ends with hierarchy separator - client is just
		   informing us that it wants to create a mailbox under
		   this name. */
                only_hiearchy = TRUE;
		mailbox = t_strndup(mailbox, len-1);
	}

	if (!client_verify_mailbox_name(client, mailbox, FALSE, TRUE))
		return TRUE;

	if (!client->storage->create_mailbox(client->storage, mailbox,
					     only_hiearchy)) {
		client_send_storage_error(client);
		return TRUE;
	}

	client_send_tagline(client, "OK Create completed.");
	return TRUE;
}
