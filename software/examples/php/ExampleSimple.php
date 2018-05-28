<?php

require_once('Tinkerforge/IPConnection.php');
require_once('Tinkerforge/BrickletRealTimeClockV2.php');

use Tinkerforge\IPConnection;
use Tinkerforge\BrickletRealTimeClockV2;

const HOST = 'localhost';
const PORT = 4223;
const UID = 'XYZ'; // Change XYZ to the UID of your Real-Time Clock Bricklet 2.0

$ipcon = new IPConnection(); // Create IP connection
$rtc = new BrickletRealTimeClockV2(UID, $ipcon); // Create device object

$ipcon->connect(HOST, PORT); // Connect to brickd
// Don't use device before ipcon is connected

// Get current date and time
$date_time = $rtc->getDateTime();

echo "Year: " . $date_time['year'] . "\n";
echo "Month: " . $date_time['month'] . "\n";
echo "Day: " . $date_time['day'] . "\n";
echo "Hour: " . $date_time['hour'] . "\n";
echo "Minute: " . $date_time['minute'] . "\n";
echo "Second: " . $date_time['second'] . "\n";
echo "Centisecond: " . $date_time['centisecond'] . "\n";
echo "Weekday: " . $date_time['weekday'] . "\n";
echo "Timestamp: " . $date_time['timestamp'] . " ms\n";

echo "Press key to exit\n";
fgetc(fopen('php://stdin', 'r'));
$ipcon->disconnect();

?>
