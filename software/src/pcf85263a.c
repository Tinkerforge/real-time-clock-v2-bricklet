/* real-time-clock-v2-bricklet
 * Copyright (C) 2018 Matthias Bolte <matthias@tinkerforge.com>
 *
 * pcf85263a.c: PCF85263A driver
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

#include "pcf85263a.h"

#include "configs/config.h"
#include "configs/config_pcf85263a.h"

#include "bricklib2/logging/logging.h"
#include "bricklib2/bootloader/bootloader.h"

// RTC Time and Date
#define PCF85263A_REG_RTC_TIME_100TH_SECOND                 0x00
#define PCF85263A_REG_RTC_TIME_OS_AND_SECOND                0x01
#define PCF85263A_REG_RTC_TIME_MINUTE                       0x02
#define PCF85263A_REG_RTC_TIME_HOUR                         0x03
#define PCF85263A_REG_RTC_DATE_DAY                          0x04
#define PCF85263A_REG_RTC_DATE_WEEKDAY                      0x05
#define PCF85263A_REG_RTC_DATE_MONTH                        0x06
#define PCF85263A_REG_RTC_DATE_YEAR                         0x07

// RTC Alarm 1 and 2
#define PCF85263A_REG_RTC_ALARM1_SECOND                     0x08
#define PCF85263A_REG_RTC_ALARM1_MINUTE                     0x09
#define PCF85263A_REG_RTC_ALARM1_HOUR                       0x0A
#define PCF85263A_REG_RTC_ALARM1_DAY                        0x0B
#define PCF85263A_REG_RTC_ALARM1_MONTH                      0x0C

#define PCF85263A_REG_RTC_ALARM2_MINUTE                     0x0D
#define PCF85263A_REG_RTC_ALARM2_HOUR                       0x0E
#define PCF85263A_REG_RTC_ALARM2_WEEKDAY                    0x0F

#define PCF85263A_REG_RTC_ALARM_ENABLE                      0x10

// RTC Timestamp 1, 2, and 3
#define PCF85263A_REG_RTC_TIMESTAMP1_SECOND                 0x11
#define PCF85263A_REG_RTC_TIMESTAMP1_MINUTE                 0x12
#define PCF85263A_REG_RTC_TIMESTAMP1_HOUR                   0x13
#define PCF85263A_REG_RTC_TIMESTAMP1_DAY                    0x14
#define PCF85263A_REG_RTC_TIMESTAMP1_MONTH                  0x15
#define PCF85263A_REG_RTC_TIMESTAMP1_YEAR                   0x16

#define PCF85263A_REG_RTC_TIMESTAMP2_SECOND                 0x17
#define PCF85263A_REG_RTC_TIMESTAMP2_MINUTE                 0x18
#define PCF85263A_REG_RTC_TIMESTAMP2_HOUR                   0x19
#define PCF85263A_REG_RTC_TIMESTAMP2_DAY                    0x1A
#define PCF85263A_REG_RTC_TIMESTAMP2_MONTH                  0x1B
#define PCF85263A_REG_RTC_TIMESTAMP2_YEAR                   0x1C

#define PCF85263A_REG_RTC_TIMESTAMP3_SECOND                 0x1D
#define PCF85263A_REG_RTC_TIMESTAMP3_MINUTE                 0x1E
#define PCF85263A_REG_RTC_TIMESTAMP3_HOUR                   0x1F
#define PCF85263A_REG_RTC_TIMESTAMP3_DAY                    0x20
#define PCF85263A_REG_RTC_TIMESTAMP3_MONTH                  0x21
#define PCF85263A_REG_RTC_TIMESTAMP3_YEAR                   0x22

#define PCF85263A_REG_RTC_TIMESTAMP_MODE                    0x23

// Stopwatch Time
#define PCF85263A_REG_STOPWATCH_TIME_100TH_SECOND           0x00
#define PCF85263A_REG_STOPWATCH_TIME_OS_AND_SECOND          0x01
#define PCF85263A_REG_STOPWATCH_TIME_MINUTE                 0x02
#define PCF85263A_REG_STOPWATCH_TIME_HOUR1                  0x03
#define PCF85263A_REG_STOPWATCH_TIME_HOUR2                  0x04
#define PCF85263A_REG_STOPWATCH_TIME_HOUR3                  0x05

// Stopwatch Alarm 1 and 2
#define PCF85263A_REG_STOPWATCH_ALARM1_SECOND               0x08
#define PCF85263A_REG_STOPWATCH_ALARM1_MINUTE               0x09
#define PCF85263A_REG_STOPWATCH_ALARM1_HOUR1                0x0A
#define PCF85263A_REG_STOPWATCH_ALARM1_HOUR2                0x0B
#define PCF85263A_REG_STOPWATCH_ALARM1_HOUR3                0x0C

#define PCF85263A_REG_STOPWATCH_ALARM2_MINUTE               0x0D
#define PCF85263A_REG_STOPWATCH_ALARM2_HOUR1                0x0E
#define PCF85263A_REG_STOPWATCH_ALARM2_HOUR2                0x0F

#define PCF85263A_REG_STOPWATCH_ALARM_ENABLE                0x10

// Stopwatch Timestamp 1, 2, and 3
#define PCF85263A_REG_STOPWATCH_TIMESTAMP1_SECOND           0x11
#define PCF85263A_REG_STOPWATCH_TIMESTAMP1_MINUTE           0x12
#define PCF85263A_REG_STOPWATCH_TIMESTAMP1_HOUR1            0x13
#define PCF85263A_REG_STOPWATCH_TIMESTAMP1_HOUR2            0x14
#define PCF85263A_REG_STOPWATCH_TIMESTAMP1_HOUR3            0x15

#define PCF85263A_REG_STOPWATCH_TIMESTAMP2_SECOND           0x17
#define PCF85263A_REG_STOPWATCH_TIMESTAMP2_MINUTE           0x18
#define PCF85263A_REG_STOPWATCH_TIMESTAMP2_HOUR1            0x19
#define PCF85263A_REG_STOPWATCH_TIMESTAMP2_HOUR2            0x1A
#define PCF85263A_REG_STOPWATCH_TIMESTAMP2_HOUR3            0x1B

#define PCF85263A_REG_STOPWATCH_TIMESTAMP3_SECOND           0x1D
#define PCF85263A_REG_STOPWATCH_TIMESTAMP3_MINUTE           0x1E
#define PCF85263A_REG_STOPWATCH_TIMESTAMP3_HOUR1            0x1F
#define PCF85263A_REG_STOPWATCH_TIMESTAMP3_HOUR2            0x20
#define PCF85263A_REG_STOPWATCH_TIMESTAMP3_HOUR3            0x21

#define PCF85263A_REG_STOPWATCH_TIMESTAMP_MODE              0x23

// Controls and Functions
#define PCF85263A_REG_OFFSET                                0x24
#define PCF85263A_REG_OSCILLATOR                            0x25
#define PCF85263A_REG_BATTERY_SWITCH                        0x26
#define PCF85263A_REG_PIN_IO                                0x27
#define PCF85263A_REG_FUNCTION                              0x28
#define PCF85263A_REG_INTA_ENABLE                           0x29
#define PCF85263A_REG_INTB_ENABLE                           0x2A
#define PCF85263A_REG_FLAGS                                 0x2B
#define PCF85263A_REG_RAM_BYTE                              0x2C
#define PCF85263A_REG_WATCHDOG                              0x2D
#define PCF85263A_REG_STOP_ENABLE                           0x2E
#define PCF85263A_REG_RESET                                 0x2F

#define PCF85263A_REG_RTC_ALARM_ENABLE_A1E_mask             0b00011111
#define PCF85263A_REG_RTC_ALARM_ENABLE_A1E_SECOND           0b00000001
#define PCF85263A_REG_RTC_ALARM_ENABLE_A1E_MINUTE           0b00000010
#define PCF85263A_REG_RTC_ALARM_ENABLE_A1E_HOUR             0b00000100
#define PCF85263A_REG_RTC_ALARM_ENABLE_A1E_DAY              0b00001000
#define PCF85263A_REG_RTC_ALARM_ENABLE_A1E_MONTH            0b00010000

#define PCF85263A_REG_RTC_ALARM_ENABLE_A2E_mask             0b11100000
#define PCF85263A_REG_RTC_ALARM_ENABLE_A2E_MINUTE           0b00100000
#define PCF85263A_REG_RTC_ALARM_ENABLE_A2E_HOUR             0b01000000
#define PCF85263A_REG_RTC_ALARM_ENABLE_A2E_WEEKDAY          0b10000000

#define PCF85263A_REG_STOPWATCH_ALARM_ENABLE_A1E_mask       0b00011111
#define PCF85263A_REG_STOPWATCH_ALARM_ENABLE_A1E_SECOND     0b00000001
#define PCF85263A_REG_STOPWATCH_ALARM_ENABLE_A1E_MINUTE     0b00000010
#define PCF85263A_REG_STOPWATCH_ALARM_ENABLE_A1E_HOUR1      0b00000100
#define PCF85263A_REG_STOPWATCH_ALARM_ENABLE_A1E_HOUR2      0b00001000
#define PCF85263A_REG_STOPWATCH_ALARM_ENABLE_A1E_HOUR3      0b00010000

#define PCF85263A_REG_STOPWATCH_ALARM_ENABLE_A2E_mask       0b11100000
#define PCF85263A_REG_STOPWATCH_ALARM_ENABLE_A2E_MINUTE     0b00100000
#define PCF85263A_REG_STOPWATCH_ALARM_ENABLE_A2E_HOUR1      0b01000000
#define PCF85263A_REG_STOPWATCH_ALARM_ENABLE_A2E_HOUR2      0b10000000

#define PCF85263A_REG_OSCILLATOR_OFFM_mask                  0b01000000
#define PCF85263A_REG_OSCILLATOR_OFFM_LOW_POWER             0b00000000
#define PCF85263A_REG_OSCILLATOR_OFFM_FAST_CORRECTION       0b01000000

#define PCF85263A_REG_OSCILLATOR_1224_mask                  0b00100000
#define PCF85263A_REG_OSCILLATOR_1224_24                    0b00000000
#define PCF85263A_REG_OSCILLATOR_1224_12                    0b00100000

#define PCF85263A_REG_OSCILLATOR_LOWJ_mask                  0b00010000
#define PCF85263A_REG_OSCILLATOR_LOWJ_DISABLED              0b00000000
#define PCF85263A_REG_OSCILLATOR_LOWJ_ENABLED               0b00010000

#define PCF85263A_REG_OSCILLATOR_OSCD_mask                  0b00001100
#define PCF85263A_REG_OSCILLATOR_OSCD_NORMAL                0b00000000
#define PCF85263A_REG_OSCILLATOR_OSCD_LOW                   0b00000100
#define PCF85263A_REG_OSCILLATOR_OSCD_HIGH                  0b00001000

#define PCF85263A_REG_OSCILLATOR_CL_mask                    0b00000011
#define PCF85263A_REG_OSCILLATOR_CL_7PF                     0b00000000
#define PCF85263A_REG_OSCILLATOR_CL_6PF                     0b00000001
#define PCF85263A_REG_OSCILLATOR_CL_12PF                    0b00000010

#define PCF85263A_REG_PIN_IO_CLKPM_mask                     0b10000000
#define PCF85263A_REG_PIN_IO_CLKPM_ENABLED                  0b00000000
#define PCF85263A_REG_PIN_IO_CLKPM_DISABLE                  0b10000000

#define PCF85263A_REG_PIN_IO_INTAPM_mask                    0b00000011
#define PCF85263A_REG_PIN_IO_INTAPM_CLK                     0b00000000
#define PCF85263A_REG_PIN_IO_INTAPM_BATTERY_MODE            0b00000001
#define PCF85263A_REG_PIN_IO_INTAPM_INTA                    0b00000010
#define PCF85263A_REG_PIN_IO_INTAPM_HI_Z                    0b00000011

#define PCF85263A_REG_FUNCTION_100TH_mask                   0b10000000
#define PCF85263A_REG_FUNCTION_100TH_DISABLED               0b00000000
#define PCF85263A_REG_FUNCTION_100TH_ENABLED                0b10000000

#define PCF85263A_REG_FUNCTION_RTCM_mask                    0b00010000
#define PCF85263A_REG_FUNCTION_RTCM_RTC                     0b00000000
#define PCF85263A_REG_FUNCTION_RTCM_STOPWATCH               0b00010000

#define PCF85263A_REG_FUNCTION_COF_mask                     0b00000111
#define PCF85263A_REG_FUNCTION_COF_32768HZ                  0b00000000
#define PCF85263A_REG_FUNCTION_COF_16384HZ                  0b00000001
#define PCF85263A_REG_FUNCTION_COF_8192HZ                   0b00000010
#define PCF85263A_REG_FUNCTION_COF_4096HZ                   0b00000011
#define PCF85263A_REG_FUNCTION_COF_2048HZ                   0b00000100
#define PCF85263A_REG_FUNCTION_COF_1024HZ                   0b00000101
#define PCF85263A_REG_FUNCTION_COF_1HZ                      0b00000110
#define PCF85263A_REG_FUNCTION_COF_STATIC_LOW               0b00000111

#define PCF85263A_REG_INTA_ENABLE_WATCHDOG                  0b00000001 // WDIEA
#define PCF85263A_REG_INTA_ENABLE_BATTERY_SWITCH            0b00000010 // BSIEA
#define PCF85263A_REG_INTA_ENABLE_TIMESTAMP                 0b00000100 // TSRIEA
#define PCF85263A_REG_INTA_ENABLE_ALARM2                    0b00001000 // A2IEA
#define PCF85263A_REG_INTA_ENABLE_ALARM1                    0b00010000 // A1IEA
#define PCF85263A_REG_INTA_ENABLE_OFFSET_CORRECTION         0b00100000 // OIEA
#define PCF85263A_REG_INTA_ENABLE_PERIODIC_INTERRUPT        0b01000000 // PIEA
#define PCF85263A_REG_INTA_ENABLE_LEVEL_MODE                0b10000000 // ILPA

#define PCF85263A_REG_FLAGS_TIMESTAMP1                      0b00000001 // TSR1F
#define PCF85263A_REG_FLAGS_TIMESTAMP2                      0b00000010 // TSR2F
#define PCF85263A_REG_FLAGS_TIMESTAMP3                      0b00000100 // TSR3F
#define PCF85263A_REG_FLAGS_BATTERY_SWITCH                  0b00001000 // BSF
#define PCF85263A_REG_FLAGS_WATCHDOG                        0b00010000 // WDF
#define PCF85263A_REG_FLAGS_ALARM1                          0b00100000 // A1F
#define PCF85263A_REG_FLAGS_ALARM2                          0b01000000 // A2F
#define PCF85263A_REG_FLAGS_PERIODIC_INTERRUPT              0b10000000 // PIF

#define PCF85263A_REG_WATCHDOG_MODE_mask                    0b10000000 // WDM
#define PCF85263A_REG_WATCHDOG_MODE_offset                  7
#define PCF85263A_REG_WATCHDOG_MODE_ONCE                    0b00000000
#define PCF85263A_REG_WATCHDOG_MODE_REPEAT                  0b10000000

#define PCF85263A_REG_WATCHDOG_COUNTER_mask                 0b01111100 // WDR
#define PCF85263A_REG_WATCHDOG_COUNTER_offset               2

#define PCF85263A_REG_WATCHDOG_STEP_mask                    0b00000011 // WDS
#define PCF85263A_REG_WATCHDOG_STEP_offset                  0
#define PCF85263A_REG_WATCHDOG_STEP_4000MS                  0b00000000
#define PCF85263A_REG_WATCHDOG_STEP_1000MS                  0b00000001
#define PCF85263A_REG_WATCHDOG_STEP_250MS                   0b00000010
#define PCF85263A_REG_WATCHDOG_STEP_62MS                    0b00000011

#define PCF85263A_REG_STOP_ENABLE_STOP_mask                 0b00000001
#define PCF85263A_REG_STOP_ENABLE_STOP_DISABLED             0b00000000
#define PCF85263A_REG_STOP_ENABLE_STOP_ENABLED              0b00000001

#define PCF85263A_REG_RESET_SOFTWARE                        0b00101100 // SR
#define PCF85263A_REG_RESET_CLEAR_PRESCALER                 0b10100100 // CPR
#define PCF85263A_REG_RESET_CLEAR_TIMESTAMP                 0b00100101 // CTS

PCF85263A pcf85263a;

static uint8_t bcd2bin(const uint8_t bcd) {
	return (bcd >> 4) * 10 + (bcd & 0b00001111);
}

static uint8_t bin2bcd(const uint8_t bin) {
	return ((bin / 10) << 4) + (bin % 10);
}

static const uint8_t days_in_this_month_table[12] = {
	31,
	28,
	31,
	30,
	31,
	30,
	31,
	31,
	30,
	31,
	30,
	31
};

static bool pcf85263a_add_seconds(PCF85263ADateTime *date_time, int32_t seconds) {
	date_time->second += seconds % 60;

	if (date_time->second > 59) {
		date_time->second -= 60;
		++date_time->minute;
	}

	int32_t minutes = seconds / 60;
	date_time->minute += minutes % 60;

	if (date_time->minute > 59) {
		date_time->minute -= 60;
		++date_time->hour;
	}

	int32_t hours = minutes / 60;
	date_time->hour += hours % 24;

	if (date_time->hour > 23) {
		date_time->hour -= 24;
		++date_time->day;
	}

	int32_t days = hours / 24;
	uint8_t days_in_this_month = days_in_this_month_table[date_time->month - 1];

	if (date_time->month == 2 && date_time->year % 4 == 0) {
		++days_in_this_month;
	}

	while (date_time->day + days > days_in_this_month) {
		days -= days_in_this_month - date_time->day + 1;

		date_time->day = 1;
		++date_time->month;

		if (date_time->month == 13) {
			date_time->month = 1;

			if (date_time->year == 2099) {
				return false;
			}

			++date_time->year;
		}

		days_in_this_month = days_in_this_month_table[date_time->month - 1];

		if (date_time->month == 2 && date_time->year % 4 == 0) {
			++days_in_this_month;
		}
	}

	date_time->day += days;

	return true;
}

static void pcf85263a_data_to_date_time(uint8_t data[8], PCF85263ADateTime *date_time) {
	data[2] &= 0b01111111; // remove EMON bit
	data[1] &= 0b01111111; // remove OS bit

	date_time->year        = 2000 + bcd2bin(data[7]);
	date_time->month       = bcd2bin(data[6]);
	date_time->day         = bcd2bin(data[4]);
	date_time->hour        = bcd2bin(data[3]);
	date_time->minute      = bcd2bin(data[2]);
	date_time->second      = bcd2bin(data[1]);
	date_time->centisecond = bcd2bin(data[0]);
	date_time->weekday     = bcd2bin(data[5]);

	// Convert from [Sun..Sat] == [0..6] to [Mon..Sun] == [1..7]
	if (date_time->weekday == 0) {
		date_time->weekday = 7;
	}
}

static void pcf85263a_write_calibration(void) {
	uint32_t page[EEPROM_PAGE_SIZE/sizeof(uint32_t)];

	page[PCF85263A_CALIBRATION_MAGIC_POS]  = PCF85263A_CALIBRATION_MAGIC;
	page[PCF85263A_CALIBRATION_OFFSET_POS] = pcf85263a.set_offset - INT8_MIN;

	bootloader_write_eeprom_page(PCF85263A_CALIBRATION_PAGE, page);
}

static void pcf85263a_read_calibration(void) {
	uint32_t page[EEPROM_PAGE_SIZE/sizeof(uint32_t)];
	bootloader_read_eeprom_page(PCF85263A_CALIBRATION_PAGE, page);

	// The magic number is not where it is supposed to be.
	// This is either our first startup or something went wrong.
	if (page[0] != PCF85263A_CALIBRATION_MAGIC) {
		pcf85263a.set_offset = 0;
		pcf85263a_write_calibration();
		return;
	}

	pcf85263a.set_offset = page[PCF85263A_CALIBRATION_OFFSET_POS] + INT8_MIN;
}

void  pcf85263a_init_state(void) {
	pcf85263a.state = PCF85263A_STATE_INIT_SET_OFFSET;

	pcf85263a.set_alarm_interval = -1;
	pcf85263a.get_alarm_interval = -1;
	pcf85263a.get_alarm_pending = true;

	i2c_fifo_init(&pcf85263a.i2c_fifo);
}

void pcf85263a_init(void) {
	memset(&pcf85263a, 0, sizeof(PCF85263A));

	pcf85263a_read_calibration();

	pcf85263a.i2c_fifo.baudrate         = PCF85263A_I2C_BAUDRATE;
	pcf85263a.i2c_fifo.address          = PCF85263A_I2C_ADDRESS;
	pcf85263a.i2c_fifo.i2c              = PCF85263A_I2C;

	pcf85263a.i2c_fifo.scl_port         = PCF85263A_SCL_PORT;
	pcf85263a.i2c_fifo.scl_pin          = PCF85263A_SCL_PIN;
	pcf85263a.i2c_fifo.scl_mode         = PCF85263A_SCL_PIN_MODE;
	pcf85263a.i2c_fifo.scl_input        = PCF85263A_SCL_INPUT;
	pcf85263a.i2c_fifo.scl_source       = PCF85263A_SCL_SOURCE;
	pcf85263a.i2c_fifo.scl_fifo_size    = PCF85263A_SCL_FIFO_SIZE;
	pcf85263a.i2c_fifo.scl_fifo_pointer = PCF85263A_SCL_FIFO_POINTER;

	pcf85263a.i2c_fifo.sda_port         = PCF85263A_SDA_PORT;
	pcf85263a.i2c_fifo.sda_pin          = PCF85263A_SDA_PIN;
	pcf85263a.i2c_fifo.sda_mode         = PCF85263A_SDA_PIN_MODE;
	pcf85263a.i2c_fifo.sda_input        = PCF85263A_SDA_INPUT;
	pcf85263a.i2c_fifo.sda_source       = PCF85263A_SDA_SOURCE;
	pcf85263a.i2c_fifo.sda_fifo_size    = PCF85263A_SDA_FIFO_SIZE;
	pcf85263a.i2c_fifo.sda_fifo_pointer = PCF85263A_SDA_FIFO_POINTER;

	pcf85263a_init_state();
}

void pcf85263a_tick(void) {
	I2CFifoState state = i2c_fifo_next_state(&pcf85263a.i2c_fifo);

	if (state & I2C_FIFO_STATE_ERROR) {
		loge("PCF85263A I2C error: %d\n\r", state);
		pcf85263a_init_state();
		return;
	}

	switch (state) {
		case I2C_FIFO_STATE_READ_REGISTER_READY: {
			uint8_t data[16];
			uint8_t length = i2c_fifo_read_fifo(&pcf85263a.i2c_fifo, data, 16);

			switch (pcf85263a.state) {
				case PCF85263A_STATE_GET_DATE_TIME: {
					if (length != 8) {
						loge("PCF85263A unexpected I2C read length: %d\n\r", length);
						pcf85263a_init();
						return;
					}

					pcf85263a_data_to_date_time(data, &pcf85263a.get_date_time);

					pcf85263a.get_date_time_valid = true;

					pcf85263a.state = PCF85263A_STATE_IDLE;

					break;
				}

				case PCF85263A_STATE_GET_ALARM: {
					if (length != 9) {
						loge("PCF85263A unexpected I2C read length: %d\n\r", length);
						pcf85263a_init();
						return;
					}

					if (pcf85263a.get_alarm_pending) {
						if ((data[8] & PCF85263A_REG_RTC_ALARM_ENABLE_A1E_MONTH) != 0) {
							pcf85263a.get_alarm.month = bcd2bin(data[4]);
						} else {
							pcf85263a.get_alarm.month = -1;
						}

						if ((data[8] & PCF85263A_REG_RTC_ALARM_ENABLE_A1E_DAY) != 0) {
							pcf85263a.get_alarm.day = bcd2bin(data[3]);
						} else {
							pcf85263a.get_alarm.day = -1;
						}

						if ((data[8] & PCF85263A_REG_RTC_ALARM_ENABLE_A1E_HOUR) != 0) {
							pcf85263a.get_alarm.hour = bcd2bin(data[2]);
						} else {
							pcf85263a.get_alarm.hour = -1;
						}

						if ((data[8] & PCF85263A_REG_RTC_ALARM_ENABLE_A1E_MINUTE) != 0) {
							pcf85263a.get_alarm.minute = bcd2bin(data[1]);
						} else {
							pcf85263a.get_alarm.minute = -1;
						}

						if ((data[8] & PCF85263A_REG_RTC_ALARM_ENABLE_A1E_SECOND) != 0) {
							pcf85263a.get_alarm.second = bcd2bin(data[0]);
						} else {
							pcf85263a.get_alarm.second = -1;
						}

						if ((data[8] & PCF85263A_REG_RTC_ALARM_ENABLE_A2E_WEEKDAY) != 0) {
							pcf85263a.get_alarm.weekday = bcd2bin(data[7]);

							// Convert from [Sun..Sat] == [0..6] to [Mon..Sun] == [1..7]
							if (pcf85263a.get_alarm.weekday == 0) {
								pcf85263a.get_alarm.weekday = 7;
							}
						} else {
							pcf85263a.get_alarm.weekday = -1;
						}

						pcf85263a.get_alarm_interval = -1;
						pcf85263a.get_alarm_pending = false;
						pcf85263a.get_alarm_valid = true;
					}

					pcf85263a.state = PCF85263A_STATE_IDLE;

					break;
				}

				case PCF85263A_STATE_CHECK_ALARM_GET_FLAGS: {
					if (length != 1) {
						loge("PCF85263A unexpected I2C read length: %d\n\r", length);
						pcf85263a_init();
						return;
					}

					pcf85263a.cached_flags = data[0];
					pcf85263a.state = PCF85263A_STATE_CHECK_ALARM_CLEAR_FLAGS;

					break;
				}

				case PCF85263A_STATE_CHECK_ALARM_GET_DATE_TIME: {
					if (length != 8) {
						loge("PCF85263A unexpected I2C read length: %d\n\r", length);
						pcf85263a_init();
						return;
					}

					pcf85263a_data_to_date_time(data, &pcf85263a.cached_date_time);

					pcf85263a.state = PCF85263A_STATE_CHECK_ALARM_TRIGGER;

					break;
				}

				default:
					break;
			}

			break;
		}

		case I2C_FIFO_STATE_WRITE_REGISTER_READY: {
			switch (pcf85263a.state) {
				case PCF85263A_STATE_INIT_SET_OFFSET:            pcf85263a.state = PCF85263A_STATE_INIT_OSCILLATOR;           break;
				case PCF85263A_STATE_INIT_OSCILLATOR:            pcf85263a.state = PCF85263A_STATE_INIT_FUNCTION;             break;
				case PCF85263A_STATE_INIT_FUNCTION:              pcf85263a.state = PCF85263A_STATE_INIT_PIN_IO;               break;
				case PCF85263A_STATE_INIT_PIN_IO:                pcf85263a.state = PCF85263A_STATE_INIT_INTA_ENABLE;          break;
				case PCF85263A_STATE_INIT_INTA_ENABLE:           pcf85263a.state = PCF85263A_STATE_IDLE;                      break;

				case PCF85263A_STATE_SET_DATE_TIME_STOP_CLOCK:   pcf85263a.state = PCF85263A_STATE_SET_DATE_TIME_SET_CLOCK;   break;
				case PCF85263A_STATE_SET_DATE_TIME_SET_CLOCK:    pcf85263a.state = PCF85263A_STATE_SET_DATE_TIME_START_CLOCK; break;
				case PCF85263A_STATE_SET_DATE_TIME_START_CLOCK:  pcf85263a.set_date_time_requested = false;
				                                                 pcf85263a.state = PCF85263A_STATE_IDLE;                      break;

				case PCF85263A_STATE_SET_ALARM_CLEAR_ENABLE:     pcf85263a.state = PCF85263A_STATE_SET_ALARM_CLEAR_INTERRUPT; break;
				case PCF85263A_STATE_SET_ALARM_CLEAR_INTERRUPT:  pcf85263a.state = PCF85263A_STATE_SET_ALARM_SET_ENABLE;      break;

				case PCF85263A_STATE_SET_ALARM_SET_ENABLE: {
					memcpy(&pcf85263a.get_alarm, &pcf85263a.set_alarm, sizeof(PCF85263AAlarm));

					pcf85263a.set_alarm_requested = false;
					pcf85263a.get_alarm_interval = pcf85263a.set_alarm_interval;
					pcf85263a.get_alarm_pending = false;
					pcf85263a.get_alarm_valid = true;

					pcf85263a.state = PCF85263A_STATE_IDLE;
					break;
				}

				case PCF85263A_STATE_CHECK_ALARM_CLEAR_FLAGS: {
					if (pcf85263a.get_alarm_valid) {
						pcf85263a.state = PCF85263A_STATE_CHECK_ALARM_GET_DATE_TIME;
					} else {
						pcf85263a.state = PCF85263A_STATE_IDLE;
					}

					break;
				}

				case PCF85263A_STATE_SET_OFFSET: {
					pcf85263a.set_offset_requested = false;
					pcf85263a.state = PCF85263A_STATE_IDLE;
					break;
				}

				default: {
					loge("PCF85263A unrecognized state: %d\n\r", pcf85263a.state);
					pcf85263a.state = PCF85263A_STATE_IDLE;
					break;
				}
			}

			break;
		}

		case I2C_FIFO_STATE_IDLE: {
			break; // Handled below
		}

		default: {
			// If we end up in a ready state that we don't handle, something went wrong
			if (state & I2C_FIFO_STATE_READY) {
				loge("PCF85263A unrecognized I2C ready state: %d\n\r", state);
				pcf85263a_init();
			}

			return;
		}
	}

	if (state == I2C_FIFO_STATE_IDLE || (state & I2C_FIFO_STATE_READY) != 0) {
		if ((pcf85263a.state & PCF85263A_STATE_INIT_mask) != 0) {
			// Initialize
			uint8_t reg;
			uint8_t data;

			switch (pcf85263a.state) {
				case PCF85263A_STATE_INIT_SET_OFFSET: {
					// Set offset from EEPROM
					reg = PCF85263A_REG_OFFSET;
					data = pcf85263a.set_offset;
					break;
				}

				case PCF85263A_STATE_INIT_OSCILLATOR: {
					// Select 12.5pF oscillator load capacitance
					reg = PCF85263A_REG_OSCILLATOR;
					data = PCF85263A_REG_OSCILLATOR_CL_12PF;
					break;
				}

				case PCF85263A_STATE_INIT_FUNCTION: {
					// Enable 100th second and set CLK pin frequency to 0Hz
					reg = PCF85263A_REG_FUNCTION;
					data = PCF85263A_REG_FUNCTION_100TH_ENABLED | PCF85263A_REG_FUNCTION_COF_STATIC_LOW;
					break;
				}

				case PCF85263A_STATE_INIT_PIN_IO: {
					// Disable CLK pin and enable nINTA output pin
					reg = PCF85263A_REG_PIN_IO;
					data = PCF85263A_REG_PIN_IO_CLKPM_DISABLE | PCF85263A_REG_PIN_IO_INTAPM_INTA;
					break;
				}

				case PCF85263A_STATE_INIT_INTA_ENABLE: {
					// Enable nINTA interrupts
					reg = PCF85263A_REG_INTA_ENABLE;
					data = PCF85263A_REG_INTA_ENABLE_ALARM1 | PCF85263A_REG_INTA_ENABLE_ALARM2 | PCF85263A_REG_INTA_ENABLE_LEVEL_MODE;
					break;
				}

				default: {
					loge("PCF85263A unrecognized init state: %d\n\r", pcf85263a.state);
					pcf85263a.state = PCF85263A_STATE_INIT_SET_OFFSET;
					return;
				}
			}

			i2c_fifo_write_register(&pcf85263a.i2c_fifo, reg, 1, &data, true);
		} else if (pcf85263a.state == PCF85263A_STATE_IDLE && XMC_GPIO_GetInput(PCF85263A_INT_PIN) == 0) {
			// Get flags to check for alarm
			pcf85263a.state = PCF85263A_STATE_CHECK_ALARM_GET_FLAGS;
			i2c_fifo_read_register(&pcf85263a.i2c_fifo, PCF85263A_REG_FLAGS, 1);
		} else if (pcf85263a.state == PCF85263A_STATE_CHECK_ALARM_CLEAR_FLAGS) {
			// Clear flags
			uint8_t data = 0;

			i2c_fifo_write_register(&pcf85263a.i2c_fifo, PCF85263A_REG_FLAGS, 1, &data, true);
		} else if (pcf85263a.state == PCF85263A_STATE_CHECK_ALARM_GET_DATE_TIME) {
			// Get date and time for alarm
			i2c_fifo_read_register(&pcf85263a.i2c_fifo, PCF85263A_REG_RTC_TIME_100TH_SECOND, 8);
		} else if (pcf85263a.state == PCF85263A_STATE_CHECK_ALARM_TRIGGER) {
			pcf85263a.state = PCF85263A_STATE_IDLE;

			if ((pcf85263a.cached_flags & (PCF85263A_REG_FLAGS_ALARM1 | PCF85263A_REG_FLAGS_ALARM2)) != 0) {
				uint8_t expected = 0;
				uint8_t found = 0;

				if (pcf85263a.get_alarm.second != -1 ||
				    pcf85263a.get_alarm.minute != -1 ||
				    pcf85263a.get_alarm.hour != -1 ||
				    pcf85263a.get_alarm.day != -1 ||
				    pcf85263a.get_alarm.month != -1) {
					++expected;

					if (pcf85263a.cached_flags & PCF85263A_REG_FLAGS_ALARM1) {
						++found;
					}
				}

				if (pcf85263a.get_alarm.weekday != -1) {
					++expected;

					if (pcf85263a.cached_flags & PCF85263A_REG_FLAGS_ALARM2) {
						++found;
					}
				}

				bool triggered = false;

				if (expected > 0) {
					if (expected == found) {
						triggered = true;
					} else if (expected == 2 && (pcf85263a.cached_flags & PCF85263A_REG_FLAGS_ALARM1)) {
						// Special case: weekday + other fields are enabled and the
						// other fields have triggered, but the weekday had already
						// triggered before or not at all. Manually check if the
						// weekday still matches to manually trigger the alarm.
						if (pcf85263a.get_alarm.weekday == pcf85263a.cached_date_time.weekday) {
							triggered = true;
						}
					}
				}

				if (triggered) {
					if (!pcf85263a.alarm_triggered) {
						memcpy(&pcf85263a.alarm_date_time, &pcf85263a.cached_date_time, sizeof(PCF85263ADateTime));
						pcf85263a.alarm_triggered = true;
					}

					if (pcf85263a.get_alarm_interval > 0 && !pcf85263a.set_alarm_requested) {
						PCF85263ADateTime date_time;

						memcpy(&date_time, &pcf85263a.cached_date_time, sizeof(PCF85263ADateTime));

						if (pcf85263a_add_seconds(&date_time, pcf85263a.get_alarm_interval)) {
							pcf85263a.set_alarm.month     = date_time.month;
							pcf85263a.set_alarm.day       = date_time.day;
							pcf85263a.set_alarm.hour      = date_time.hour;
							pcf85263a.set_alarm.minute    = date_time.minute;
							pcf85263a.set_alarm.second    = date_time.second;
							pcf85263a.set_alarm.weekday   = date_time.weekday;
							pcf85263a.set_alarm_interval  = pcf85263a.get_alarm_interval;
							pcf85263a.set_alarm_requested = true;
						}
					}
				}
			}
		} else if (pcf85263a.state == PCF85263A_STATE_IDLE && pcf85263a.set_date_time_requested) {
			// Stop clock and clear prescaler
			uint8_t data[2] = { PCF85263A_REG_STOP_ENABLE_STOP_ENABLED, PCF85263A_REG_RESET_CLEAR_PRESCALER };

			pcf85263a.state = PCF85263A_STATE_SET_DATE_TIME_STOP_CLOCK;
			i2c_fifo_write_register(&pcf85263a.i2c_fifo, PCF85263A_REG_STOP_ENABLE, 2, data, true);
		} else if (pcf85263a.state == PCF85263A_STATE_SET_DATE_TIME_SET_CLOCK) {
			// Set clock
			uint8_t data[8] = {
				bin2bcd(pcf85263a.set_date_time.centisecond),
				bin2bcd(pcf85263a.set_date_time.second),
				bin2bcd(pcf85263a.set_date_time.minute),
				bin2bcd(pcf85263a.set_date_time.hour),
				bin2bcd(pcf85263a.set_date_time.day),
				bin2bcd(pcf85263a.set_date_time.weekday % 7), // Convert from [Mon..Sun] == [1..7] to [Sun..Sat] == [0..6]
				bin2bcd(pcf85263a.set_date_time.month),
				bin2bcd(pcf85263a.set_date_time.year - 2000)
			};

			i2c_fifo_write_register(&pcf85263a.i2c_fifo, PCF85263A_REG_RTC_TIME_100TH_SECOND, 8, data, true);
		} else if (pcf85263a.state == PCF85263A_STATE_SET_DATE_TIME_START_CLOCK) {
			// Start clock
			uint8_t data = PCF85263A_REG_STOP_ENABLE_STOP_DISABLED;

			i2c_fifo_write_register(&pcf85263a.i2c_fifo, PCF85263A_REG_STOP_ENABLE, 1, &data, true);
		} else if (pcf85263a.state == PCF85263A_STATE_IDLE && pcf85263a.get_alarm_pending && !pcf85263a.get_alarm_valid) {
			// Get alarm
			pcf85263a.state = PCF85263A_STATE_GET_ALARM;
			i2c_fifo_read_register(&pcf85263a.i2c_fifo, PCF85263A_REG_RTC_ALARM1_SECOND, 9);
		} else if (pcf85263a.state == PCF85263A_STATE_IDLE && pcf85263a.set_alarm_requested && pcf85263a.get_date_time_valid) {
			// Clear alarm enable bits
			uint8_t data = 0;

			pcf85263a.state = PCF85263A_STATE_SET_ALARM_CLEAR_ENABLE;
			i2c_fifo_write_register(&pcf85263a.i2c_fifo, PCF85263A_REG_RTC_ALARM_ENABLE, 1, &data, true);
		} else if (pcf85263a.state == PCF85263A_STATE_SET_ALARM_CLEAR_INTERRUPT) {
			// Clear alarm interrupt flags
			uint8_t data = 0;

			i2c_fifo_write_register(&pcf85263a.i2c_fifo, PCF85263A_REG_FLAGS, 1, &data, true);
		} else if (pcf85263a.state == PCF85263A_STATE_SET_ALARM_SET_ENABLE) {
			// Set alarm and enable bits
			uint8_t data[9] = {0};

			if (pcf85263a.set_alarm.second != -1) {
				data[0] = bin2bcd(pcf85263a.set_alarm.second);
				data[8] |= PCF85263A_REG_RTC_ALARM_ENABLE_A1E_SECOND;
			}

			if (pcf85263a.set_alarm.minute != -1) {
				data[1] = bin2bcd(pcf85263a.set_alarm.minute);
				data[8] |= PCF85263A_REG_RTC_ALARM_ENABLE_A1E_MINUTE;
			}

			if (pcf85263a.set_alarm.hour != -1) {
				data[2] = bin2bcd(pcf85263a.set_alarm.hour);
				data[8] |= PCF85263A_REG_RTC_ALARM_ENABLE_A1E_HOUR;
			}

			if (pcf85263a.set_alarm.day != -1) {
				data[3] = bin2bcd(pcf85263a.set_alarm.day);
				data[8] |= PCF85263A_REG_RTC_ALARM_ENABLE_A1E_DAY;
			}

			if (pcf85263a.set_alarm.month != -1) {
				data[4] = bin2bcd(pcf85263a.set_alarm.month);
				data[8] |= PCF85263A_REG_RTC_ALARM_ENABLE_A1E_MONTH;
			}

			if (pcf85263a.set_alarm.weekday != -1) {
				data[7] = bin2bcd(pcf85263a.set_alarm.weekday % 7), // Convert from [Mon..Sun] == [1..7] to [Sun..Sat] == [0..6]
				data[8] |= PCF85263A_REG_RTC_ALARM_ENABLE_A2E_WEEKDAY;
			}

			if (data[8] == 0 && pcf85263a.set_alarm_interval > 0) {
				PCF85263ADateTime date_time;

				memcpy(&date_time, &pcf85263a.get_date_time, sizeof(PCF85263ADateTime));

				if (pcf85263a_add_seconds(&date_time, pcf85263a.set_alarm_interval)) {
					pcf85263a.set_alarm.second    = date_time.second;
					pcf85263a.set_alarm.minute    = date_time.minute;
					pcf85263a.set_alarm.hour      = date_time.hour;
					pcf85263a.set_alarm.day       = date_time.day;
					pcf85263a.set_alarm.month     = date_time.month;
					pcf85263a.set_alarm.weekday   = -1;
					pcf85263a.set_alarm_requested = true;

					data[0] = bin2bcd(date_time.second);
					data[1] = bin2bcd(date_time.minute);
					data[2] = bin2bcd(date_time.hour);
					data[3] = bin2bcd(date_time.day);
					data[4] = bin2bcd(date_time.month);

					data[8] = PCF85263A_REG_RTC_ALARM_ENABLE_A1E_MONTH |
					          PCF85263A_REG_RTC_ALARM_ENABLE_A1E_DAY |
					          PCF85263A_REG_RTC_ALARM_ENABLE_A1E_HOUR |
					          PCF85263A_REG_RTC_ALARM_ENABLE_A1E_MINUTE |
					          PCF85263A_REG_RTC_ALARM_ENABLE_A1E_SECOND;
				} else {
					data[8] = 0;
				}
			}

			i2c_fifo_write_register(&pcf85263a.i2c_fifo, PCF85263A_REG_RTC_ALARM1_SECOND, 9, data, true);
		} else if (pcf85263a.state == PCF85263A_STATE_IDLE && pcf85263a.set_offset_requested) {
			// Set offset
			uint8_t data = pcf85263a.set_offset;

			pcf85263a_write_calibration();

			pcf85263a.state = PCF85263A_STATE_SET_OFFSET;
			i2c_fifo_write_register(&pcf85263a.i2c_fifo, PCF85263A_REG_OFFSET, 1, &data, true);
		} else if (pcf85263a.state == PCF85263A_STATE_IDLE) {
			// Get date and time
			pcf85263a.state = PCF85263A_STATE_GET_DATE_TIME;
			i2c_fifo_read_register(&pcf85263a.i2c_fifo, PCF85263A_REG_RTC_TIME_100TH_SECOND, 8);
		} else {
			loge("PCF85263A unrecognized state: %d\n\r", pcf85263a.state);
			pcf85263a.state = PCF85263A_STATE_IDLE;
		}
	}
}
