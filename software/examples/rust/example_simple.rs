use std::{error::Error, io};

use tinkerforge::{ipconnection::IpConnection, real_time_clock_v2_bricklet::*};

const HOST: &str = "127.0.0.1";
const PORT: u16 = 4223;
const UID: &str = "XYZ"; // Change XYZ to the UID of your Real-Time Clock Bricklet 2.0

fn main() -> Result<(), Box<dyn Error>> {
    let ipcon = IpConnection::new(); // Create IP connection
    let real_time_clock_v2_bricklet = RealTimeClockV2Bricklet::new(UID, &ipcon); // Create device object

    ipcon.connect(HOST, PORT).recv()??; // Connect to brickd
                                        // Don't use device before ipcon is connected

    // Get current date and time
    let get_date_time_result = real_time_clock_v2_bricklet.get_date_time().recv()?;

    println!("Year: {}", get_date_time_result.year);
    println!("Month: {}", get_date_time_result.month);
    println!("Day: {}", get_date_time_result.day);
    println!("Hour: {}", get_date_time_result.hour);
    println!("Minute: {}", get_date_time_result.minute);
    println!("Second: {}", get_date_time_result.second);
    println!("Centisecond: {}", get_date_time_result.centisecond);
    println!("Weekday: {}", get_date_time_result.weekday);
    println!("Timestamp: {}{}", get_date_time_result.timestamp, " ms");

    println!("Press enter to exit.");
    let mut _input = String::new();
    io::stdin().read_line(&mut _input)?;
    ipcon.disconnect();
    Ok(())
}
