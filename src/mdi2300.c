/*
 * Copyright (C) 2017, Boundary Devices <info@boundarydevices.com>
 */

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>

#include "mdi2300.h"

int mdi2300_serial_write(int fd, char *buffer, int length)
{
	int status;

	DBG_BUFFER("buffer", buffer, length);

	/* Write the buffer */
	status = write(fd, buffer, length);
	if (status < 0) {
		ERR("failed to write: %d", status);
	}

	return status;
}

int mdi2300_serial_read(int fd, char *buffer, int length)
{
	int status;

	/* Read the buffer */
	status = read(fd, buffer, length);
	if (status < 0) {
		ERR("failed to read: %d", status);
	}

	DBG_BUFFER("buffer", buffer, status);

	return status;
}

int mdi2300_serial_wait_ack(int fd)
{
	int status;
	char buffer;

	status = mdi2300_serial_read(fd, &buffer, sizeof(buffer));
	if (status <= 0)
		goto err;

	if (buffer != ACK_VALUE)
		ERR("failed to received ACK: %x", buffer);
err:
	return status;
}

int mdi2300_serial_send_cmd(int fd, char *cmd)
{
	int ret;

	if (!cmd)
		return -1;

	ret = mdi2300_serial_write(fd, cmd, strlen(cmd));
	/* Cancel transfer command is not acked */
	if ((ret == (int)strlen(cmd)) && (strcmp(cmd, CANCEL_TRANSFER)))
		ret = mdi2300_serial_wait_ack(fd);

	return ret;
}

int mdi2300_serial_param(int fd, int speed)
{
	int status;
	struct termios ti;

	/* Set the actual baud rate */
	status = tcgetattr(fd, &ti);
	if (status < 0) {
		ERR("tcgetattr failed: %d", status);
		goto err;
	}

	/* Raw setup */
	ti.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
	ti.c_oflag &= ~OPOST;
	ti.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN | CRTSCTS);

	/* Setup a 3s timeout */
	ti.c_cc[VMIN] = 0;
	ti.c_cc[VTIME] = 30;

	status = cfsetispeed(&ti, speed);
	if (status < 0) {
		ERR("can't set ispeed: %d", status);
		goto err;
	}

	status = cfsetospeed(&ti, speed);
	if (status < 0) {
		ERR("can't set ospeed: %d", status);
		goto err;
	}

	status = tcsetattr(fd, TCSAFLUSH, &ti);
	if (status < 0) {
		ERR("tcsetattr failed: %d", status);
		goto err;
	}

	/* Enable ACK for serial commands */
	status = mdi2300_serial_send_cmd(fd, ACK_ENABLE);
err:
	tcflush(fd, TCIOFLUSH);
	return status;
}

int mdi2300_init(char *device)
{
	int ret;
	int fd;

	/* Opening the device */
	fd = open(device, O_RDWR);
	if (fd < 0) {
		ERR("failed to open %s: %d", device, errno);
		return -1;
	}

	ret = mdi2300_serial_param(fd, B9600);

	close(fd);

	return ret;
}

int mdi2300_close(char *device)
{
	int ret;
	int fd;

	/* Opening the device */
	fd = open(device, O_RDWR);
	if (fd < 0) {
		ERR("failed to open %s: %d", device, errno);
		return -1;
	}

	ret = mdi2300_serial_send_cmd(fd, TRIGGER_STOP);
	if (ret < 0)
		goto end;

	ret = mdi2300_serial_send_cmd(fd, CANCEL_TRANSFER);
	if (ret < 0)
		goto end;
end:
	close(fd);
	return ret;
}

int mdi2300_scan(char *device, char *buffer, int length)
{
	int ret;
	int fd;
	int retries = 5;

	/* Opening the device */
	fd = open(device, O_RDWR);
	if (fd < 0) {
		ERR("failed to open %s: %d", device, errno);
		return -1;
	}

	ret = mdi2300_serial_send_cmd(fd, CODE_ALL);
	if (ret < 0)
		goto end;

	ret = mdi2300_serial_send_cmd(fd, READ_CONTINUOUS);
	if (ret < 0)
		goto end;

	while (retries--) {
		ret = mdi2300_serial_send_cmd(fd, TRIGGER_START);
		if (ret < 0)
			goto end;

		ret = mdi2300_serial_read(fd, buffer, length);
		if (ret > 0)
			break;
		else if (ret == 0)
			DBG("timeout!");
	}
end:
	close(fd);
	return ret;
}
