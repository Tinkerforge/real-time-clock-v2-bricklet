import com.tinkerforge.IPConnection;
import com.tinkerforge.BrickletRealTimeClockV2;

public class ExampleCallback {
	private static final String HOST = "localhost";
	private static final int PORT = 4223;

	// Change XYZ to the UID of your Real-Time Clock Bricklet 2.0
	private static final String UID = "XYZ";

	// Note: To make the example code cleaner we do not handle exceptions. Exceptions
	//       you might normally want to catch are described in the documentation
	public static void main(String args[]) throws Exception {
		IPConnection ipcon = new IPConnection(); // Create IP connection
		BrickletRealTimeClockV2 rtc =
		  new BrickletRealTimeClockV2(UID, ipcon); // Create device object

		ipcon.connect(HOST, PORT); // Connect to brickd
		// Don't use device before ipcon is connected

		// Add date and time listener
		rtc.addDateTimeListener(new BrickletRealTimeClockV2.DateTimeListener() {
			public void dateTime(int year, int month, int day, int hour, int minute,
			                     int second, int centisecond, int weekday,
			                     long timestamp) {
				System.out.println("Year: " + year);
				System.out.println("Month: " + month);
				System.out.println("Day: " + day);
				System.out.println("Hour: " + hour);
				System.out.println("Minute: " + minute);
				System.out.println("Second: " + second);
				System.out.println("Centisecond: " + centisecond);

				if(weekday == BrickletRealTimeClockV2.WEEKDAY_MONDAY) {
					System.out.println("Weekday: Monday");
				} else if(weekday == BrickletRealTimeClockV2.WEEKDAY_TUESDAY) {
					System.out.println("Weekday: Tuesday");
				} else if(weekday == BrickletRealTimeClockV2.WEEKDAY_WEDNESDAY) {
					System.out.println("Weekday: Wednesday");
				} else if(weekday == BrickletRealTimeClockV2.WEEKDAY_THURSDAY) {
					System.out.println("Weekday: Thursday");
				} else if(weekday == BrickletRealTimeClockV2.WEEKDAY_FRIDAY) {
					System.out.println("Weekday: Friday");
				} else if(weekday == BrickletRealTimeClockV2.WEEKDAY_SATURDAY) {
					System.out.println("Weekday: Saturday");
				} else if(weekday == BrickletRealTimeClockV2.WEEKDAY_SUNDAY) {
					System.out.println("Weekday: Sunday");
				}

				System.out.println("Timestamp: " + timestamp);
				System.out.println("");
			}
		});

		// Set period for date and time callback to 5s (5000ms)
		rtc.setDateTimeCallbackConfiguration(5000);

		System.out.println("Press key to exit"); System.in.read();
		ipcon.disconnect();
	}
}
