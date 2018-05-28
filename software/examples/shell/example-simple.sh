#!/bin/sh
# Connects to localhost:4223 by default, use --host and --port to change this

uid=XYZ # Change XYZ to the UID of your Real-Time Clock Bricklet 2.0

# Get current date and time
tinkerforge call real-time-clock-v2-bricklet $uid get-date-time
