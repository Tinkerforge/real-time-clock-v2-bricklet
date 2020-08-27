#include "bindings/hal_common.h"
#include "bindings/bricklet_real_time_clock_v2.h"

#define UID "XYZ" // Change XYZ to the UID of your Real-Time Clock Bricklet 2.0

void check(int rc, const char* msg);

void example_setup(TF_HalContext *hal);
void example_loop(TF_HalContext *hal);


static TF_RealTimeClockV2 rtc;

void example_setup(TF_HalContext *hal) {
	// Create device object
	check(tf_real_time_clock_v2_create(&rtc, UID, hal), "create device object");

	// Get current date and time
	uint16_t year; uint8_t month, day, hour, minute, second, centisecond, weekday;
	int64_t timestamp;
	check(tf_real_time_clock_v2_get_date_time(&rtc, &year, &month, &day, &hour, &minute,
	                                          &second, &centisecond, &weekday,
	                                          &timestamp), "get date and time");

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

	tf_hal_printf("Timestamp: %lld ms\n", timestamp);
}

void example_loop(TF_HalContext *hal) {
	// Poll for callbacks
	tf_hal_callback_tick(hal, 0);
}
