/* This file is part of staffAuth.
 *
 * staffAuth is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see
 *
 *  http://www.gnu.org/licenses/
 *
 * Copyright (C)
 *  2015 Alexander Haase IT Services <support@alexhaase.de>
 */

#include <mauth.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include "config.h"


/** \brief Search for mauth server in local domain and copy address into \p
 *  domain.
 *
 * \details This function will lookup the hosts FQDNs and extracts the domain
 *  of them. A host named MAUTH_HOSTNAME will be searched in each domain and
 *  their higher level domains.
 *
 *
 * \param domain Buffer to store domain of mauth server in.
 * \param size Size of \p domain.
 *
 * \return On success the number of written bytes into \p domain will be
 *  returned. If no server was found or an error occured zero will be returned.
 *
 * \note If this function fails, the value of domain is unpredictable and must
 *  not be used.
 */
size_t
mauth_find_server(char *buffer, size_t size)
{
	/* This solution was adapted from http://stackoverflow.com/a/505546.
	 * First we will get the hostname and then the FQDNs associated with this
	 * hostname. With the FQDNs we can search for the mauth server in the
	 * domains of this host. */
	if (gethostname(buffer, size) < 0)
		return 0;

	/* Search for FQDNs of hostname. */
	struct addrinfo hints = {0}, *gai_addr;
	hints.ai_flags = AI_CANONNAME; // we need the FQDN
	if (getaddrinfo(buffer, NULL, &hints, &gai_addr) != 0)
		return 0;

	/* Iterate through FQDNs of host. */
	bool found = false;
	for (struct addrinfo *p = gai_addr; p != NULL && !found; p = p->ai_next) {
		if (p->ai_canonname == NULL)
			continue;

		/* Iterate through all levels of FQDN domain. Thus the hostname of this
		 * host is not used, the part before the first dot in domain can be
		 * ignored (while instead of do while). */
		char *domain = p->ai_canonname;
		while ((domain = strchr(domain, '.')) != NULL && !found) {
			/* strchr will return the position of '.', but we need the first
			 * character of the domain without the leading dot. */
			domain++;

			/* Build mauth FQDN with MAUTH_HOSTNAME and domain concatenated and
			 * separated by a dot. */
			int len = snprintf(buffer, size, "%s.%s", MAUTH_HOSTNAME, domain);
			if (len < 0 || len >= size)
				break;

			/* Lookup domain stored in buffer. If domain exists, we have found
			 * our mauth server. */
			struct addrinfo *dom_lookup;
			if (getaddrinfo(buffer, NULL, NULL, &dom_lookup) == 0)
				found = true;
			freeaddrinfo(dom_lookup);
		}
	}
	freeaddrinfo(gai_addr);

	if (found)
		return strlen(buffer);

	return 0;
}