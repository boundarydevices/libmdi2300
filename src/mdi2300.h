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

#ifndef _MDI2300_H
#define _MDI2300_H

/* #define DEBUG */

/* Commands from MDI-2300 Serial Interface document */

#define ACK_ENABLE	"\x1bWC\r"
#define ACK_DISABLE	"\x1bWD\r"
#define ACK_VALUE	0x06
#define NACK_VALUE	0x15

#define TRIGGER_START	"\x1bZ\r"
#define TRIGGER_STOP	"\x1bY\r"
#define TRIGGER_ENABLE	"\x1bS8\r"
#define TRIGGER_DISABLE	"\x1bS7\r"

#define BUZZER_GOOD	"\x1b""B\r"
#define BUZZER_ERROR	"\x1b""E\r"

#define LED_GOOD	"\x1bL\r"

#define SAVE_SETTINGS	"\x1bZ2\r"

#define AIMING_ON	"\x1b""D3D\r"
#define AIMING_OFF	"\x1b""D3E\r"

#define ILLUM_ON	"\x1b""D39\r"
#define ILLUM_OFF	"\x1b""D3A\r"

#define READ_SINGLE	"\x1bS0\r"
#define READ_MULTIPLE	"\x1bS1\r"
#define READ_CONTINUOUS	"\x1bS2\r"

#define CODE_ALL	"\x1b""A0\r"

#define CANCEL_TRANSFER	"\x18"

/* Convenient printing macros */

#define ERR(fmt, arg...)  fprintf(stderr, "[%s]: " fmt "\n" , __func__ , ## arg)

#ifndef DEBUG
#define DBG(fmt, arg...)	{}
#define DBG_BUFFER(fmt, arg...)	{}
#else
#define DBG(fmt, arg...)  printf("[%s]: " fmt "\n" , __func__ , ## arg)
#define DBG_BUFFER(fmt, buf, len) \
	{ \
		int IDEBUG; \
		printf("[%s]: " fmt " [len:%d]", __func__, len); \
		for(IDEBUG = 0; IDEBUG < len; IDEBUG++) { \
			if (!(IDEBUG%16)) \
				printf("\n"); \
			printf("0x%02x ", buf[IDEBUG]); \
		} \
		printf("\n"); \
	}
#endif

#endif
