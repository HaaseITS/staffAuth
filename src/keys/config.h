/* This file is part of staffAuth.
 *
 * staffAuth is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see
 *
 *  http://www.gnu.org/licenses/
 *
 *
 * Copyright (C)
 *  2015-2016 Alexander Haase IT Services <support@alexhaase.de>
 */

#ifndef STAFFAUTH_KEYS_CONFIG_H
#define STAFFAUTH_KEYS_CONFIG_H


typedef struct keys_config
{
	const char *login;
	char *server;
} keys_config;


void staffauth_keys_parse_args(int argc, char **argv, keys_config *config);
void staffauth_keys_parse_conffile(keys_config *config);


#endif
