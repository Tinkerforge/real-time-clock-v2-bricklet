use std::{error::Error, io};

use tinkerforge::{ip_connection::IpConnection, real_time_clock_v2_bricklet::*};

const HOST: &str = "localhost";
const PORT: u16 = 4223;
const UID: &str = "XYZ"; // Change XYZ to the UID of your Real-Time Clock Bricklet 2.0.

fn main() -> Result<(), Box<dyn Error>> {
    let ipcon = IpConnection::new(); // Create IP connection.
    let rtc = RealTimeClockV2Bricklet::new(UID, &ipcon); // Create device object.

    ipcon.connect((HOST, PORT)).recv()??; // Connect to brickd.
                                          // Don't use device before ipcon is connected.

    // Get current date and time.
    let date_time = rtc.get_date_time().recv()?;

    println!("Year: {}", date_time.year);
    println!("Month: {}", date_time.month);
    println!("Day: {}", date_time.day);
    println!("Hour: {}", date_time.hour);
    println!("Minute: {}", date_time.minute);
    println!("Second: {}", date_time.second);
    println!("Centisecond: {}", date_time.centisecond);

    if date_time.weekday == REAL_TIME_CLOCK_V2_BRICKLET_WEEKDAY_MONDAY {
        println!("Weekday: Monday");
    } else if date_time.weekday == REAL_TIME_CLOCK_V2_BRICKLET_WEEKDAY_TUESDAY {
        println!("Weekday: Tuesday");
    } else if date_time.weekday == REAL_TIME_CLOCK_V2_BRICKLET_WEEKDAY_WEDNESDAY {
        println!("Weekday: Wednesday");
    } else if date_time.weekday == REAL_TIME_CLOCK_V2_BRICKLET_WEEKDAY_THURSDAY {
        println!("Weekday: Thursday");
    } else if date_time.weekday == REAL_TIME_CLOCK_V2_BRICKLET_WEEKDAY_FRIDAY {
        println!("Weekday: Friday");
    } else if date_time.weekday == REAL_TIME_CLOCK_V2_BRICKLET_WEEKDAY_SATURDAY {
        println!("Weekday: Saturday");
    } else if date_time.weekday == REAL_TIME_CLOCK_V2_BRICKLET_WEEKDAY_SUNDAY {
        println!("Weekday: Sunday");
    }

    println!("Timestamp: {} ms", date_time.timestamp);

    println!("Press enter to exit.");
    let mut _input = String::new();
    io::stdin().read_line(&mut _input)?;
    ipcon.disconnect();
    Ok(())
}
