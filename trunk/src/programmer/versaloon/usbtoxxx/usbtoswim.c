/***************************************************************************
 *   Copyright (C) 2009 by Simon Qian <SimonQian@SimonQian.com>            *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include <string.h>

#include "app_cfg.h"
#include "app_type.h"
#include "app_err.h"
#include "app_log.h"
#include "prog_interface.h"

#include "../versaloon.h"
#include "../versaloon_internal.h"
#include "usbtoxxx.h"
#include "usbtoxxx_internal.h"

uint8_t usbtoswim_num_of_interface = 0;

RESULT usbtoswim_init(void)
{
	return usbtoxxx_init_command(USB_TO_SWIM, &usbtoswim_num_of_interface);
}

RESULT usbtoswim_fini(void)
{
	return usbtoxxx_fini_command(USB_TO_SWIM);
}

RESULT usbtoswim_set_param(uint8_t interface_index, uint8_t mHz, 
							uint8_t cnt0, uint8_t cnt1)
{
	uint8_t buff[3];
	
#if PARAM_CHECK
	if (interface_index > 7)
	{
		LOG_BUG(_GETTEXT("invalid inteface_index %d.\n"), interface_index);
		return ERROR_FAIL;
	}
#endif
	
	buff[0] = mHz;
	buff[1] = cnt0;
	buff[2] = cnt1;
	
	return usbtoxxx_conf_command(USB_TO_SWIM, interface_index, buff, 3);
}

RESULT usbtoswim_out(uint8_t interface_index, uint8_t data, uint8_t bitlen)
{
	uint8_t buff[2];
	
#if PARAM_CHECK
	if (interface_index > 7)
	{
		LOG_BUG(_GETTEXT("invalid inteface_index %d.\n"), interface_index);
		return ERROR_FAIL;
	}
#endif
	
	if (bitlen > 8)
	{
		LOG_BUG(_GETTEXT("max length of a single data is 8 bit\n"));
		return ERROR_FAIL;
	}
	
	buff[0] = bitlen;
	buff[1] = data;
	
	return usbtoxxx_out_command(USB_TO_SWIM, interface_index, buff, 2, 1);
}

RESULT usbtoswim_in(uint8_t interface_index, uint8_t *data, uint8_t bytelen)
{
#if PARAM_CHECK
	if (interface_index > 7)
	{
		LOG_BUG(_GETTEXT("invalid inteface_index %d.\n"), interface_index);
		return ERROR_FAIL;
	}
#endif
	
	versaloon_cmd_buf[0] = bytelen;
	memset(&versaloon_cmd_buf[1], 0, bytelen);
	
	return usbtoxxx_in_command(USB_TO_SWIM, interface_index, 
				versaloon_cmd_buf, 1 + bytelen, bytelen, data, 0, bytelen, 0);
}

