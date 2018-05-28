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

# Get current date and time as [year, month, day, hour, minute, second, centisecond,
#                               weekday, timestamp]
date_time = rtc.get_date_time

puts "Year: #{date_time[0]}"
puts "Month: #{date_time[1]}"
puts "Day: #{date_time[2]}"
puts "Hour: #{date_time[3]}"
puts "Minute: #{date_time[4]}"
puts "Second: #{date_time[5]}"
puts "Centisecond: #{date_time[6]}"
puts "Weekday: #{date_time[7]}"
puts "Timestamp: #{date_time[8]} ms"

puts 'Press key to exit'
$stdin.gets
ipcon.disconnect
