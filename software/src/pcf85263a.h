/* real-time-clock-v2-bricklet
 * Copyright (C) 2018 Matthias Bolte <matthias@tinkerforge.com>
 *
 * pcf85263a.h: PCF85263A driver
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

#ifndef PCF85263A_H
#define PCF85263A_H

#include <stdint.h>

#include "bricklib2/hal/i2c_fifo/i2c_fifo.h"

#define PCF85263A_STATE_INIT_mask          0x0100
#define PCF85263A_STATE_SET_DATE_TIME_mask 0x0200
#define PCF85263A_STATE_GET_DATE_TIME_mask 0x0400
#define PCF85263A_STATE_SET_ALARM_mask     0x0800
#define PCF85263A_STATE_GET_ALARM_mask     0x1000
#define PCF85263A_STATE_CHECK_ALARM_mask   0x2000
#define PCF85263A_STATE_SET_OFFSET_mask    0x4000

typedef enum {
	PCF85263A_STATE_IDLE = 0,

	PCF85263A_STATE_INIT_SET_OFFSET           = PCF85263A_STATE_INIT_mask          | 0x0001,
	PCF85263A_STATE_INIT_OSCILLATOR           = PCF85263A_STATE_INIT_mask          | 0x0002,
	PCF85263A_STATE_INIT_FUNCTION             = PCF85263A_STATE_INIT_mask          | 0x0004,
	PCF85263A_STATE_INIT_PIN_IO               = PCF85263A_STATE_INIT_mask          | 0x0008,
	PCF85263A_STATE_INIT_INTA_ENABLE          = PCF85263A_STATE_INIT_mask          | 0x0010,

	PCF85263A_STATE_SET_DATE_TIME_STOP_CLOCK  = PCF85263A_STATE_SET_DATE_TIME_mask | 0x0001,
	PCF85263A_STATE_SET_DATE_TIME_SET_CLOCK   = PCF85263A_STATE_SET_DATE_TIME_mask | 0x0002,
	PCF85263A_STATE_SET_DATE_TIME_START_CLOCK = PCF85263A_STATE_SET_DATE_TIME_mask | 0x0004,

	PCF85263A_STATE_GET_DATE_TIME             = PCF85263A_STATE_GET_DATE_TIME_mask | 0x0001,

	PCF85263A_STATE_SET_ALARM_GET_DATE_TIME   = PCF85263A_STATE_SET_ALARM_mask     | 0x0001,
	PCF85263A_STATE_SET_ALARM_CLEAR_ENABLE    = PCF85263A_STATE_SET_ALARM_mask     | 0x0002,
	PCF85263A_STATE_SET_ALARM_CLEAR_INTERRUPT = PCF85263A_STATE_SET_ALARM_mask     | 0x0004,
	PCF85263A_STATE_SET_ALARM_SET_ENABLE      = PCF85263A_STATE_SET_ALARM_mask     | 0x0008,

	PCF85263A_STATE_GET_ALARM                 = PCF85263A_STATE_GET_ALARM_mask     | 0x0001,

	PCF85263A_STATE_CHECK_ALARM_GET_FLAGS     = PCF85263A_STATE_CHECK_ALARM_mask   | 0x0001,
	PCF85263A_STATE_CHECK_ALARM_CLEAR_FLAGS   = PCF85263A_STATE_CHECK_ALARM_mask   | 0x0002,
	PCF85263A_STATE_CHECK_ALARM_GET_DATE_TIME = PCF85263A_STATE_CHECK_ALARM_mask   | 0x0004,
	PCF85263A_STATE_CHECK_ALARM_TRIGGER       = PCF85263A_STATE_CHECK_ALARM_mask   | 0x0008,

	PCF85263A_STATE_SET_OFFSET                = PCF85263A_STATE_SET_OFFSET_mask    | 0x0001,
} PCF85263AState;

#define PCF85263A_CALIBRATION_PAGE       1
#define PCF85263A_CALIBRATION_MAGIC_POS  0
#define PCF85263A_CALIBRATION_OFFSET_POS 1

#define PCF85263A_CALIBRATION_MAGIC      0x12345678

typedef struct {
	uint16_t year;
	uint8_t month;
	uint8_t day;
	uint8_t hour;
	uint8_t minute;
	uint8_t second;
	uint8_t centisecond;
	uint8_t weekday;
} PCF85263ADateTime;

typedef struct {
	int8_t month;
	int8_t day;
	int8_t hour;
	int8_t minute;
	int8_t second;
	int8_t weekday;
} PCF85263AAlarm;

typedef struct {
	PCF85263AState state;

	PCF85263ADateTime set_date_time;
	bool set_date_time_requested;

	PCF85263ADateTime get_date_time;
	bool get_date_time_valid;

	PCF85263AAlarm set_alarm;
	int32_t set_alarm_interval; // in seconds
	bool set_alarm_requested;
	PCF85263ADateTime cached_set_alarm_date_time;

	PCF85263AAlarm get_alarm; // always holds the current alarm config, if valid
	int32_t get_alarm_interval; // in seconds
	bool get_alarm_pending;
	bool get_alarm_valid;

	uint8_t cached_flags;
	PCF85263ADateTime cached_date_time; // read after alarm was detected

	PCF85263ADateTime alarm_date_time;
	bool alarm_triggered;

	int8_t set_offset;
	bool set_offset_requested;

	I2CFifo i2c_fifo;
} PCF85263A;

void pcf85263a_init(void);
void pcf85263a_tick(void);

#endif
