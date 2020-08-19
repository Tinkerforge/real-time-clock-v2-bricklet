#define __STDC_FORMAT_MACROS // for PRId64/PRIu64 in C++


#include <inttypes.h>

#include "bindings/hal_common.h"
#include "bindings/bricklet_real_time_clock_v2.h"

#define UID "XYZ" // Change XYZ to the UID of your Real-Time Clock Bricklet 2.0

void check(int rc, const char* msg);



// Callback function for date and time callback
void date_time_handler(TF_RealTimeClockV2 *device, uint16_t year, uint8_t month,
                       uint8_t day, uint8_t hour, uint8_t minute, uint8_t second,
                       uint8_t centisecond, uint8_t weekday, int64_t timestamp,
                       void *user_data) {
	(void)device; (void)user_data; // avoid unused parameter warning

	tf_hal_printf("Year: %u\n", year);
	tf_hal_printf("Month: %u\n", month);
	tf_hal_printf("Day: %u\n", day);
	tf_hal_printf("Hour: %u\n", hour);
	tf_hal_printf("Minute: %u\n", minute);
	tf_hal_printf("Second: %u\n", second);
	tf_hal_printf("Centisecond: %u\n", centisecond);

	if(weekday == TF_REAL_TIME_CLOCK_V2_WEEKDAY_MONDAY) {
		tf_hal_printf("Weekday: Monday\n");
	} else if(weekday == TF_REAL_TIME_CLOCK_V2_WEEKDAY_TUESDAY) {
		tf_hal_printf("Weekday: Tuesday\n");
	} else if(weekday == TF_REAL_TIME_CLOCK_V2_WEEKDAY_WEDNESDAY) {
		tf_hal_printf("Weekday: Wednesday\n");
	} else if(weekday == TF_REAL_TIME_CLOCK_V2_WEEKDAY_THURSDAY) {
		tf_hal_printf("Weekday: Thursday\n");
	} else if(weekday == TF_REAL_TIME_CLOCK_V2_WEEKDAY_FRIDAY) {
		tf_hal_printf("Weekday: Friday\n");
	} else if(weekday == TF_REAL_TIME_CLOCK_V2_WEEKDAY_SATURDAY) {
		tf_hal_printf("Weekday: Saturday\n");
	} else if(weekday == TF_REAL_TIME_CLOCK_V2_WEEKDAY_SUNDAY) {
		tf_hal_printf("Weekday: Sunday\n");
	}

	tf_hal_printf("Timestamp: %" PRId64 "\n", timestamp);
	tf_hal_printf("\n");
}


TF_RealTimeClockV2 rtc;

void example_setup(TF_HalContext *hal) {
	// Create device object
	check(tf_real_time_clock_v2_create(&rtc, UID, hal), "create device object");


	// Register date and time callback to function date_time_handler
	tf_real_time_clock_v2_register_date_time_callback(&rtc,
	                                                 date_time_handler,
	                                                 NULL);

	// Set period for date and time callback to 5s (5000ms)
	tf_real_time_clock_v2_set_date_time_callback_configuration(&rtc, 5000);

}

void example_loop(TF_HalContext *hal) {
	// Poll for callbacks
	tf_hal_callback_tick(hal, 0);
}
