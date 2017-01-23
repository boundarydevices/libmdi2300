/*
 * Copyright (C) 2017, Boundary Devices <info@boundarydevices.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <mdi2300.h>

#define DEFAULT_UART "/dev/ttymxc2"

int main(int argc, const char *argv[])
{
	int ret;
	char *device = DEFAULT_UART;
	char buffer[1024];

	if (argc > 1)
		device = (char *)argv[1];

	ret = mdi2300_init(device);
	if (ret < 0)
		goto end;

	ret = mdi2300_scan(device, buffer, sizeof(buffer));
	if (ret > 0) {
		/* Make sure the string is terminated */
		buffer[ret] = '\0';
		printf("Code: %s\n", buffer);
	}
end:
	mdi2300_close(device);
	return ret;
}
