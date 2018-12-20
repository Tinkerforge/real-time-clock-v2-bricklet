<?php

require_once('Tinkerforge/IPConnection.php');
require_once('Tinkerforge/BrickletRealTimeClockV2.php');

use Tinkerforge\IPConnection;
use Tinkerforge\BrickletRealTimeClockV2;

const HOST = 'localhost';
const PORT = 4223;
const UID = 'XYZ'; // Change XYZ to the UID of your Real-Time Clock Bricklet 2.0

// Callback function for date and time callback
function cb_dateTime($year, $month, $day, $hour, $minute, $second, $centisecond, $weekday,
                     $timestamp)
{
    echo "Year: $year\n";
    echo "Month: $month\n";
    echo "Day: $day\n";
    echo "Hour: $hour\n";
    echo "Minute: $minute\n";
    echo "Second: $second\n";
    echo "Centisecond: $centisecond\n";

    if ($weekday == BrickletRealTimeClockV2::WEEKDAY_MONDAY) {
        echo "Weekday: Monday\n";
    } elseif ($weekday == BrickletRealTimeClockV2::WEEKDAY_TUESDAY) {
        echo "Weekday: Tuesday\n";
    } elseif ($weekday == BrickletRealTimeClockV2::WEEKDAY_WEDNESDAY) {
        echo "Weekday: Wednesday\n";
    } elseif ($weekday == BrickletRealTimeClockV2::WEEKDAY_THURSDAY) {
        echo "Weekday: Thursday\n";
    } elseif ($weekday == BrickletRealTimeClockV2::WEEKDAY_FRIDAY) {
        echo "Weekday: Friday\n";
    } elseif ($weekday == BrickletRealTimeClockV2::WEEKDAY_SATURDAY) {
        echo "Weekday: Saturday\n";
    } elseif ($weekday == BrickletRealTimeClockV2::WEEKDAY_SUNDAY) {
        echo "Weekday: Sunday\n";
    }

    echo "Timestamp: $timestamp\n";
    echo "\n";
}

$ipcon = new IPConnection(); // Create IP connection
$rtc = new BrickletRealTimeClockV2(UID, $ipcon); // Create device object

$ipcon->connect(HOST, PORT); // Connect to brickd
// Don't use device before ipcon is connected

// Register date and time callback to function cb_dateTime
$rtc->registerCallback(BrickletRealTimeClockV2::CALLBACK_DATE_TIME, 'cb_dateTime');

// Set period for date and time callback to 5s (5000ms)
$rtc->setDateTimeCallbackConfiguration(5000);

echo "Press ctrl+c to exit\n";
$ipcon->dispatchCallbacks(-1); // Dispatch callbacks forever

?>
