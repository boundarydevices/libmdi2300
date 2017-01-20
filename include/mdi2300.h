/*
 * Copyright (C) 2017, Boundary Devices <info@boundarydevices.com>
 */

#ifndef _MDI2300_H
#define _MDI2300_H

int mdi2300_init(char *device);
int mdi2300_close(char *device);
int mdi2300_scan(char *device, char *buffer, int length);

#endif
