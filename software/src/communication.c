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
#include "bricklib2/utility/util_definitions.h"
#include "bricklib2/hal/system_timer/system_timer.h"
#include "bricklib2/protocols/tfp/tfp.h"
#include "bricklib2/logging/logging.h"

#include "pcf85263a.h"

extern PCF85263A pcf85263a;

static uint32_t date_time_callback_period = 0;

static const uint16_t days_before_this_month_table[12] = {
	0,
	31,
	31 + 28,
	31 + 28 + 31,
	31 + 28 + 31 + 30,
	31 + 28 + 31 + 30 + 31,
	31 + 28 + 31 + 30 + 31 + 30,
	31 + 28 + 31 + 30 + 31 + 30 + 31,
	31 + 28 + 31 + 30 + 31 + 30 + 31 + 31,
	31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30,
	31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31,
	31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30
};

int64_t calculate_timestamp(PCF85263ADateTime *date_time) {
	uint8_t year = date_time->year - 2000;
	int64_t days_before_this_year = year * 365 + (MAX((int8_t)year - 1, 0)) / 4 + (year > 0 ? 1 : 0);
	int64_t days_before_this_month = days_before_this_month_table[date_time->month - 1] + (date_time->month > 2 && year % 4 == 0 ? 1 : 0);

	return (((((days_before_this_year + days_before_this_month + date_time->day - 1) * 24 + date_time->hour) * 60 + date_time->minute) * 60) + date_time->second) * 1000 + date_time->centisecond * 10;
}

BootloaderHandleMessageResponse handle_message(const void *message, void *response) {
	switch(tfp_get_fid_from_message(message)) {
		case FID_SET_DATE_TIME: return set_date_time(message);
		case FID_GET_DATE_TIME: return get_date_time(message, response);
		case FID_GET_TIMESTAMP: return get_timestamp(message, response);
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
	if (data->year < 2000 || data->year > 2099 ||
	    data->month < 1 || data->month > 12 ||
	    data->day < 1 || data->day > 31 ||
	    data->hour > 23 ||
	    data->minute > 59 ||
	    data->second > 59 ||
	    data->centisecond > 99 ||
	    data->weekday < 1 || data->weekday > 7) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	pcf85263a.set_date_time.year        = data->year;
	pcf85263a.set_date_time.month       = data->month;
	pcf85263a.set_date_time.day         = data->day;
	pcf85263a.set_date_time.hour        = data->hour;
	pcf85263a.set_date_time.minute      = data->minute;
	pcf85263a.set_date_time.second      = data->second;
	pcf85263a.set_date_time.centisecond = data->centisecond;
	pcf85263a.set_date_time.weekday     = data->weekday;
	pcf85263a.set_date_time_requested   = true;

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_date_time(const GetDateTime *data, GetDateTime_Response *response) {
	if (!pcf85263a.get_date_time_valid) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	response->header.length = sizeof(GetDateTime_Response);
	response->year          = pcf85263a.get_date_time.year;
	response->month         = pcf85263a.get_date_time.month;
	response->day           = pcf85263a.get_date_time.day;
	response->hour          = pcf85263a.get_date_time.hour;
	response->minute        = pcf85263a.get_date_time.minute;
	response->second        = pcf85263a.get_date_time.second;
	response->centisecond   = pcf85263a.get_date_time.centisecond;
	response->weekday       = pcf85263a.get_date_time.weekday;
	response->timestamp     = calculate_timestamp(&pcf85263a.get_date_time);

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse get_timestamp(const GetTimestamp *data, GetTimestamp_Response *response) {
	if (!pcf85263a.get_date_time_valid) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	response->header.length = sizeof(GetTimestamp_Response);
	response->timestamp     = calculate_timestamp(&pcf85263a.get_date_time);

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse set_offset(const SetOffset *data) {
	pcf85263a.set_offset = data->offset;
	pcf85263a.set_offset_requested = true;

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_offset(const GetOffset *data, GetOffset_Response *response) {
	response->header.length = sizeof(GetOffset_Response);
	response->offset        = pcf85263a.set_offset;

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse set_date_time_callback_configuration(const SetDateTimeCallbackConfiguration *data) {
	date_time_callback_period = data->period;

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_date_time_callback_configuration(const GetDateTimeCallbackConfiguration *data, GetDateTimeCallbackConfiguration_Response *response) {
	response->header.length = sizeof(GetDateTimeCallbackConfiguration_Response);
	response->period        = date_time_callback_period;

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse set_alarm(const SetAlarm *data) {
	if ((data->month != -1 && (data->month < 1 || data->month > 12)) ||
	    (data->day != -1 && (data->day < 1 || data->day > 31)) ||
	    (data->hour != -1 && (data->hour < 0 || data->hour > 23)) ||
	    (data->minute != -1 && (data->minute < 0 || data->minute > 59)) ||
	    (data->second != -1 && (data->second < 0 || data->second > 59)) ||
	    (data->weekday != -1 && (data->weekday < 1 || data->weekday > 7)) ||
	    (data->interval != -1 && data->interval == 0)) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	pcf85263a.set_alarm.month     = data->month;
	pcf85263a.set_alarm.day       = data->day;
	pcf85263a.set_alarm.hour      = data->hour;
	pcf85263a.set_alarm.minute    = data->minute;
	pcf85263a.set_alarm.second    = data->second;
	pcf85263a.set_alarm.weekday   = data->weekday;
	pcf85263a.set_alarm_interval  = data->interval;
	pcf85263a.set_alarm_requested = true;

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_alarm(const GetAlarm *data, GetAlarm_Response *response) {
	if (pcf85263a.set_alarm_requested) {
		response->header.length = sizeof(GetAlarm_Response);
		response->month         = pcf85263a.set_alarm.month;
		response->day           = pcf85263a.set_alarm.day;
		response->hour          = pcf85263a.set_alarm.hour;
		response->minute        = pcf85263a.set_alarm.minute;
		response->second        = pcf85263a.set_alarm.second;
		response->weekday       = pcf85263a.set_alarm.weekday;
		response->interval      = pcf85263a.set_alarm_interval;

		return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
	} else if (pcf85263a.get_alarm_valid) {
		response->header.length = sizeof(GetAlarm_Response);
		response->month         = pcf85263a.get_alarm.month;
		response->day           = pcf85263a.get_alarm.day;
		response->hour          = pcf85263a.get_alarm.hour;
		response->minute        = pcf85263a.get_alarm.minute;
		response->second        = pcf85263a.get_alarm.second;
		response->weekday       = pcf85263a.get_alarm.weekday;
		response->interval      = pcf85263a.get_alarm_interval;

		return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
	} else {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}
}

bool handle_date_time_callback(void) {
	static bool is_buffered = false;
	static DateTime_Callback cb;
	static uint32_t last_time = 0;

	if (!is_buffered) {
		tfp_make_default_header(&cb.header, bootloader_get_uid(), sizeof(DateTime_Callback), FID_CALLBACK_DATE_TIME);

		if (date_time_callback_period == 0 ||
		    !system_timer_is_time_elapsed_ms(last_time, date_time_callback_period)) {
			return false;
		}

		cb.year        = pcf85263a.get_date_time.year;
		cb.month       = pcf85263a.get_date_time.month;
		cb.day         = pcf85263a.get_date_time.day;
		cb.hour        = pcf85263a.get_date_time.hour;
		cb.minute      = pcf85263a.get_date_time.minute;
		cb.second      = pcf85263a.get_date_time.second;
		cb.centisecond = pcf85263a.get_date_time.centisecond;
		cb.weekday     = pcf85263a.get_date_time.weekday;
		cb.timestamp   = calculate_timestamp(&pcf85263a.get_date_time);

		last_time = system_timer_get_ms();
	}

	if (bootloader_spitfp_is_send_possible(&bootloader_status.st)) {
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

	if (!is_buffered) {
		if (!pcf85263a.alarm_triggered) {
			return false;
		}

		tfp_make_default_header(&cb.header, bootloader_get_uid(), sizeof(Alarm_Callback), FID_CALLBACK_ALARM);

		cb.year        = pcf85263a.alarm_date_time.year;
		cb.month       = pcf85263a.alarm_date_time.month;
		cb.day         = pcf85263a.alarm_date_time.day;
		cb.hour        = pcf85263a.alarm_date_time.hour;
		cb.minute      = pcf85263a.alarm_date_time.minute;
		cb.second      = pcf85263a.alarm_date_time.second;
		cb.centisecond = pcf85263a.alarm_date_time.centisecond;
		cb.weekday     = pcf85263a.alarm_date_time.weekday;
		cb.timestamp   = calculate_timestamp(&pcf85263a.alarm_date_time);

		pcf85263a.alarm_triggered = false;
	}

	if (bootloader_spitfp_is_send_possible(&bootloader_status.st)) {
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
