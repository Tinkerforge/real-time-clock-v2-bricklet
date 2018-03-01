/* real-time-clock-v2-bricklet
 * Copyright (C) 2018 Matthias Bolte <matthias@tinkerforge.com>
 *
 * communication.c: TFP protocol message handling
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include "communication.h"

#include "bricklib2/utility/communication_callback.h"
#include "bricklib2/protocols/tfp/tfp.h"

BootloaderHandleMessageResponse handle_message(const void *message, void *response) {
	switch(tfp_get_fid_from_message(message)) {
		case FID_SET_DATE_TIME: return set_date_time(message);
		case FID_GET_DATE_TIME: return get_date_time(message, response);
		case FID_SET_OFFSET: return set_offset(message);
		case FID_GET_OFFSET: return get_offset(message, response);
		case FID_SET_DATE_TIME_CALLBACK_CONFIGURATION: return set_date_time_callback_configuration(message);
		case FID_GET_DATE_TIME_CALLBACK_CONFIGURATION: return get_date_time_callback_configuration(message, response);
		case FID_SET_ALARM: return set_alarm(message);
		case FID_GET_ALARM: return get_alarm(message, response);
		default: return HANDLE_MESSAGE_RESPONSE_NOT_SUPPORTED;
	}
}


BootloaderHandleMessageResponse set_date_time(const SetDateTime *data) {

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_date_time(const GetDateTime *data, GetDateTime_Response *response) {
	response->header.length = sizeof(GetDateTime_Response);

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse set_offset(const SetOffset *data) {

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_offset(const GetOffset *data, GetOffset_Response *response) {
	response->header.length = sizeof(GetOffset_Response);

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse set_date_time_callback_configuration(const SetDateTimeCallbackConfiguration *data) {

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_date_time_callback_configuration(const GetDateTimeCallbackConfiguration *data, GetDateTimeCallbackConfiguration_Response *response) {
	response->header.length = sizeof(GetDateTimeCallbackConfiguration_Response);

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse set_alarm(const SetAlarm *data) {

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_alarm(const GetAlarm *data, GetAlarm_Response *response) {
	response->header.length = sizeof(GetAlarm_Response);

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}




bool handle_date_time_callback(void) {
	static bool is_buffered = false;
	static DateTime_Callback cb;

	if(!is_buffered) {
		tfp_make_default_header(&cb.header, bootloader_get_uid(), sizeof(DateTime_Callback), FID_CALLBACK_DATE_TIME);
		// TODO: Implement DateTime callback handling

		return false;
	}

	if(bootloader_spitfp_is_send_possible(&bootloader_status.st)) {
		bootloader_spitfp_send_ack_and_message(&bootloader_status, (uint8_t*)&cb, sizeof(DateTime_Callback));
		is_buffered = false;
		return true;
	} else {
		is_buffered = true;
	}

	return false;
}

bool handle_alarm_callback(void) {
	static bool is_buffered = false;
	static Alarm_Callback cb;

	if(!is_buffered) {
		tfp_make_default_header(&cb.header, bootloader_get_uid(), sizeof(Alarm_Callback), FID_CALLBACK_ALARM);
		// TODO: Implement Alarm callback handling

		return false;
	}

	if(bootloader_spitfp_is_send_possible(&bootloader_status.st)) {
		bootloader_spitfp_send_ack_and_message(&bootloader_status, (uint8_t*)&cb, sizeof(Alarm_Callback));
		is_buffered = false;
		return true;
	} else {
		is_buffered = true;
	}

	return false;
}

void communication_tick(void) {
	communication_callback_tick();
}

void communication_init(void) {
	communication_callback_init();
}
