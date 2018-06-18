var Tinkerforge = require('tinkerforge');

var HOST = 'localhost';
var PORT = 4223;
var UID = 'XYZ'; // Change XYZ to the UID of your Real-Time Clock Bricklet 2.0

var ipcon = new Tinkerforge.IPConnection(); // Create IP connection
var rtc = new Tinkerforge.BrickletRealTimeClockV2(UID, ipcon); // Create device object

ipcon.connect(HOST, PORT,
    function (error) {
        console.log('Error: ' + error);
    }
); // Connect to brickd
// Don't use device before ipcon is connected

ipcon.on(Tinkerforge.IPConnection.CALLBACK_CONNECTED,
    function (connectReason) {
        // Set period for date and time callback to 5s (5000ms)
        rtc.setDateTimeCallbackConfiguration(5000);
    }
);

// Register date and time callback
rtc.on(Tinkerforge.BrickletRealTimeClockV2.CALLBACK_DATE_TIME,
    // Callback function for date and time callback
    function (year, month, day, hour, minute, second, centisecond, weekday, timestamp) {
        console.log('Year: ' + year);
        console.log('Month: ' + month);
        console.log('Day: ' + day);
        console.log('Hour: ' + hour);
        console.log('Minute: ' + minute);
        console.log('Second: ' + second);
        console.log('Centisecond: ' + centisecond);
        console.log('Weekday: ' + weekday);
        console.log('Timestamp: ' + timestamp);
        console.log();
    }
);

console.log('Press key to exit');
process.stdin.on('data',
    function (data) {
        ipcon.disconnect();
        process.exit(0);
    }
);