/* real-time-clock-v2-bricklet
 * Copyright (C) 2018 Matthias Bolte <matthias@tinkerforge.com>
 *
 * communication.h: TFP protocol message handling
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

#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <stdint.h>
#include <stdbool.h>

#include "bricklib2/protocols/tfp/tfp.h"
#include "bricklib2/bootloader/bootloader.h"

// Default functions
BootloaderHandleMessageResponse handle_message(const void *data, void *response);
void communication_tick(void);
void communication_init(void);

// Constants
#define REAL_TIME_CLOCK_V2_WEEKDAY_MONDAY 1
#define REAL_TIME_CLOCK_V2_WEEKDAY_TUESDAY 2
#define REAL_TIME_CLOCK_V2_WEEKDAY_WEDNESDAY 3
#define REAL_TIME_CLOCK_V2_WEEKDAY_THURSDAY 4
#define REAL_TIME_CLOCK_V2_WEEKDAY_FRIDAY 5
#define REAL_TIME_CLOCK_V2_WEEKDAY_SATURDAY 6
#define REAL_TIME_CLOCK_V2_WEEKDAY_SUNDAY 7

#define REAL_TIME_CLOCK_V2_ALARM_MATCH_DISABLED -1

#define REAL_TIME_CLOCK_V2_ALARM_INTERVAL_DISABLED -1

#define REAL_TIME_CLOCK_V2_BOOTLOADER_MODE_BOOTLOADER 0
#define REAL_TIME_CLOCK_V2_BOOTLOADER_MODE_FIRMWARE 1
#define REAL_TIME_CLOCK_V2_BOOTLOADER_MODE_BOOTLOADER_WAIT_FOR_REBOOT 2
#define REAL_TIME_CLOCK_V2_BOOTLOADER_MODE_FIRMWARE_WAIT_FOR_REBOOT 3
#define REAL_TIME_CLOCK_V2_BOOTLOADER_MODE_FIRMWARE_WAIT_FOR_ERASE_AND_REBOOT 4

#define REAL_TIME_CLOCK_V2_BOOTLOADER_STATUS_OK 0
#define REAL_TIME_CLOCK_V2_BOOTLOADER_STATUS_INVALID_MODE 1
#define REAL_TIME_CLOCK_V2_BOOTLOADER_STATUS_NO_CHANGE 2
#define REAL_TIME_CLOCK_V2_BOOTLOADER_STATUS_ENTRY_FUNCTION_NOT_PRESENT 3
#define REAL_TIME_CLOCK_V2_BOOTLOADER_STATUS_DEVICE_IDENTIFIER_INCORRECT 4
#define REAL_TIME_CLOCK_V2_BOOTLOADER_STATUS_CRC_MISMATCH 5

#define REAL_TIME_CLOCK_V2_STATUS_LED_CONFIG_OFF 0
#define REAL_TIME_CLOCK_V2_STATUS_LED_CONFIG_ON 1
#define REAL_TIME_CLOCK_V2_STATUS_LED_CONFIG_SHOW_HEARTBEAT 2
#define REAL_TIME_CLOCK_V2_STATUS_LED_CONFIG_SHOW_STATUS 3

// Function and callback IDs and structs
#define FID_SET_DATE_TIME 1
#define FID_GET_DATE_TIME 2
#define FID_GET_TIMESTAMP 3
#define FID_SET_OFFSET 4
#define FID_GET_OFFSET 5
#define FID_SET_DATE_TIME_CALLBACK_CONFIGURATION 6
#define FID_GET_DATE_TIME_CALLBACK_CONFIGURATION 7
#define FID_SET_ALARM 8
#define FID_GET_ALARM 9

#define FID_CALLBACK_DATE_TIME 10
#define FID_CALLBACK_ALARM 11

typedef struct {
	TFPMessageHeader header;
	uint16_t year;
	uint8_t month;
	uint8_t day;
	uint8_t hour;
	uint8_t minute;
	uint8_t second;
	uint8_t centisecond;
	uint8_t weekday;
} __attribute__((__packed__)) SetDateTime;

typedef struct {
	TFPMessageHeader header;
} __attribute__((__packed__)) GetDateTime;

typedef struct {
	TFPMessageHeader header;
	uint16_t year;
	uint8_t month;
	uint8_t day;
	uint8_t hour;
	uint8_t minute;
	uint8_t second;
	uint8_t centisecond;
	uint8_t weekday;
	int64_t timestamp;
} __attribute__((__packed__)) GetDateTime_Response;

typedef struct {
	TFPMessageHeader header;
} __attribute__((__packed__)) GetTimestamp;

typedef struct {
	TFPMessageHeader header;
	int64_t timestamp;
} __attribute__((__packed__)) GetTimestamp_Response;

typedef struct {
	TFPMessageHeader header;
	int8_t offset;
} __attribute__((__packed__)) SetOffset;

typedef struct {
	TFPMessageHeader header;
} __attribute__((__packed__)) GetOffset;

typedef struct {
	TFPMessageHeader header;
	int8_t offset;
} __attribute__((__packed__)) GetOffset_Response;

typedef struct {
	TFPMessageHeader header;
	uint32_t period;
} __attribute__((__packed__)) SetDateTimeCallbackConfiguration;

typedef struct {
	TFPMessageHeader header;
} __attribute__((__packed__)) GetDateTimeCallbackConfiguration;

typedef struct {
	TFPMessageHeader header;
	uint32_t period;
} __attribute__((__packed__)) GetDateTimeCallbackConfiguration_Response;

typedef struct {
	TFPMessageHeader header;
	int8_t month;
	int8_t day;
	int8_t hour;
	int8_t minute;
	int8_t second;
	int8_t weekday;
	int32_t interval;
} __attribute__((__packed__)) SetAlarm;

typedef struct {
	TFPMessageHeader header;
} __attribute__((__packed__)) GetAlarm;

typedef struct {
	TFPMessageHeader header;
	int8_t month;
	int8_t day;
	int8_t hour;
	int8_t minute;
	int8_t second;
	int8_t weekday;
	int32_t interval;
} __attribute__((__packed__)) GetAlarm_Response;

typedef struct {
	TFPMessageHeader header;
	uint16_t year;
	uint8_t month;
	uint8_t day;
	uint8_t hour;
	uint8_t minute;
	uint8_t second;
	uint8_t centisecond;
	uint8_t weekday;
	int64_t timestamp;
} __attribute__((__packed__)) DateTime_Callback;

typedef struct {
	TFPMessageHeader header;
	uint16_t year;
	uint8_t month;
	uint8_t day;
	uint8_t hour;
	uint8_t minute;
	uint8_t second;
	uint8_t centisecond;
	uint8_t weekday;
	int64_t timestamp;
} __attribute__((__packed__)) Alarm_Callback;


// Function prototypes
BootloaderHandleMessageResponse set_date_time(const SetDateTime *data);
BootloaderHandleMessageResponse get_date_time(const GetDateTime *data, GetDateTime_Response *response);
BootloaderHandleMessageResponse get_timestamp(const GetTimestamp *data, GetTimestamp_Response *response);
BootloaderHandleMessageResponse set_offset(const SetOffset *data);
BootloaderHandleMessageResponse get_offset(const GetOffset *data, GetOffset_Response *response);
BootloaderHandleMessageResponse set_date_time_callback_configuration(const SetDateTimeCallbackConfiguration *data);
BootloaderHandleMessageResponse get_date_time_callback_configuration(const GetDateTimeCallbackConfiguration *data, GetDateTimeCallbackConfiguration_Response *response);
BootloaderHandleMessageResponse set_alarm(const SetAlarm *data);
BootloaderHandleMessageResponse get_alarm(const GetAlarm *data, GetAlarm_Response *response);

// Callbacks
bool handle_date_time_callback(void);
bool handle_alarm_callback(void);

#define COMMUNICATION_CALLBACK_TICK_WAIT_MS 1
#define COMMUNICATION_CALLBACK_HANDLER_NUM 2
#define COMMUNICATION_CALLBACK_LIST_INIT \
	handle_date_time_callback, \
	handle_alarm_callback, \


#endif
