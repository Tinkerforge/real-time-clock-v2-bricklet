#!/bin/sh
# Connects to localhost:4223 by default, use --host and --port to change this

uid=XYZ # Change XYZ to the UID of your Real-Time Clock Bricklet 2.0

# Handle incoming date and time callbacks
tinkerforge dispatch real-time-clock-v2-bricklet $uid date-time &

# Set period for date and time callback to 5s (5000ms)
tinkerforge call real-time-clock-v2-bricklet $uid set-date-time-callback-configuration 5000

echo "Press key to exit"; read dummy

kill -- -$$ # Stop callback dispatch in background
