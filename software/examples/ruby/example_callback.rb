#!/usr/bin/env ruby
# -*- ruby encoding: utf-8 -*-

require 'tinkerforge/ip_connection'
require 'tinkerforge/bricklet_real_time_clock_v2'

include Tinkerforge

HOST = 'localhost'
PORT = 4223
UID = 'XYZ' # Change XYZ to the UID of your Real-Time Clock Bricklet 2.0

ipcon = IPConnection.new # Create IP connection
rtc = BrickletRealTimeClockV2.new UID, ipcon # Create device object

ipcon.connect HOST, PORT # Connect to brickd
# Don't use device before ipcon is connected

# Register date and time callback
rtc.register_callback(BrickletRealTimeClockV2::CALLBACK_DATE_TIME) do |year, month, day,
                                                                       hour, minute,
                                                                       second,
                                                                       centisecond,
                                                                       weekday, timestamp|
  puts "Year: #{year}"
  puts "Month: #{month}"
  puts "Day: #{day}"
  puts "Hour: #{hour}"
  puts "Minute: #{minute}"
  puts "Second: #{second}"
  puts "Centisecond: #{centisecond}"

  if weekday == BrickletRealTimeClockV2::WEEKDAY_MONDAY
    puts "Weekday: Monday"
  elsif weekday == BrickletRealTimeClockV2::WEEKDAY_TUESDAY
    puts "Weekday: Tuesday"
  elsif weekday == BrickletRealTimeClockV2::WEEKDAY_WEDNESDAY
    puts "Weekday: Wednesday"
  elsif weekday == BrickletRealTimeClockV2::WEEKDAY_THURSDAY
    puts "Weekday: Thursday"
  elsif weekday == BrickletRealTimeClockV2::WEEKDAY_FRIDAY
    puts "Weekday: Friday"
  elsif weekday == BrickletRealTimeClockV2::WEEKDAY_SATURDAY
    puts "Weekday: Saturday"
  elsif weekday == BrickletRealTimeClockV2::WEEKDAY_SUNDAY
    puts "Weekday: Sunday"
  end

  puts "Timestamp: #{timestamp}"
  puts ''
end

# Set period for date and time callback to 5s (5000ms)
rtc.set_date_time_callback_configuration 5000

puts 'Press key to exit'
$stdin.gets
ipcon.disconnect
