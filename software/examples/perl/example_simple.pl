#!/usr/bin/perl

use strict;
use Tinkerforge::IPConnection;
use Tinkerforge::BrickletRealTimeClockV2;

use constant HOST => 'localhost';
use constant PORT => 4223;
use constant UID => 'XYZ'; # Change XYZ to the UID of your Real-Time Clock Bricklet 2.0

my $ipcon = Tinkerforge::IPConnection->new(); # Create IP connection
my $rtc = Tinkerforge::BrickletRealTimeClockV2->new(&UID, $ipcon); # Create device object

$ipcon->connect(&HOST, &PORT); # Connect to brickd
# Don't use device before ipcon is connected

# Get current date and time
my ($year, $month, $day, $hour, $minute, $second, $centisecond, $weekday, $timestamp) = $rtc->get_date_time();

print "Year: $year\n";
print "Month: $month\n";
print "Day: $day\n";
print "Hour: $hour\n";
print "Minute: $minute\n";
print "Second: $second\n";
print "Centisecond: $centisecond\n";

if ($weekday == $rtc->WEEKDAY_MONDAY)
{
    print "Weekday: Monday\n";
}
elsif ($weekday == $rtc->WEEKDAY_TUESDAY)
{
    print "Weekday: Tuesday\n";
}
elsif ($weekday == $rtc->WEEKDAY_WEDNESDAY)
{
    print "Weekday: Wednesday\n";
}
elsif ($weekday == $rtc->WEEKDAY_THURSDAY)
{
    print "Weekday: Thursday\n";
}
elsif ($weekday == $rtc->WEEKDAY_FRIDAY)
{
    print "Weekday: Friday\n";
}
elsif ($weekday == $rtc->WEEKDAY_SATURDAY)
{
    print "Weekday: Saturday\n";
}
elsif ($weekday == $rtc->WEEKDAY_SUNDAY)
{
    print "Weekday: Sunday\n";
}

print "Timestamp: $timestamp ms\n";

print "Press key to exit\n";
<STDIN>;
$ipcon->disconnect();
