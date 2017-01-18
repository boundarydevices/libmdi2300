/*
 * Copyright (C) 2017, Boundary Devices <info@boundarydevices.com>
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
end:
	return ret;
}
