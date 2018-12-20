using System;
using Tinkerforge;

class Example
{
	private static string HOST = "localhost";
	private static int PORT = 4223;
	private static string UID = "XYZ"; // Change XYZ to the UID of your Real-Time Clock Bricklet 2.0

	static void Main()
	{
		IPConnection ipcon = new IPConnection(); // Create IP connection
		BrickletRealTimeClockV2 rtc =
		  new BrickletRealTimeClockV2(UID, ipcon); // Create device object

		ipcon.Connect(HOST, PORT); // Connect to brickd
		// Don't use device before ipcon is connected

		// Get current date and time
		int year; byte month, day, hour, minute, second, centisecond, weekday; long timestamp;
		rtc.GetDateTime(out year, out month, out day, out hour, out minute, out second,
		                out centisecond, out weekday, out timestamp);

		Console.WriteLine("Year: " + year);
		Console.WriteLine("Month: " + month);
		Console.WriteLine("Day: " + day);
		Console.WriteLine("Hour: " + hour);
		Console.WriteLine("Minute: " + minute);
		Console.WriteLine("Second: " + second);
		Console.WriteLine("Centisecond: " + centisecond);

		if(weekday == BrickletRealTimeClockV2.WEEKDAY_MONDAY)
		{
			Console.WriteLine("Weekday: Monday");
		}
		else if(weekday == BrickletRealTimeClockV2.WEEKDAY_TUESDAY)
		{
			Console.WriteLine("Weekday: Tuesday");
		}
		else if(weekday == BrickletRealTimeClockV2.WEEKDAY_WEDNESDAY)
		{
			Console.WriteLine("Weekday: Wednesday");
		}
		else if(weekday == BrickletRealTimeClockV2.WEEKDAY_THURSDAY)
		{
			Console.WriteLine("Weekday: Thursday");
		}
		else if(weekday == BrickletRealTimeClockV2.WEEKDAY_FRIDAY)
		{
			Console.WriteLine("Weekday: Friday");
		}
		else if(weekday == BrickletRealTimeClockV2.WEEKDAY_SATURDAY)
		{
			Console.WriteLine("Weekday: Saturday");
		}
		else if(weekday == BrickletRealTimeClockV2.WEEKDAY_SUNDAY)
		{
			Console.WriteLine("Weekday: Sunday");
		}

		Console.WriteLine("Timestamp: " + timestamp + " ms");

		Console.WriteLine("Press enter to exit");
		Console.ReadLine();
		ipcon.Disconnect();
	}
}
