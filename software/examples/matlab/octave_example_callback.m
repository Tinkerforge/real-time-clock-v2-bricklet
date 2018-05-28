function octave_example_callback()
    more off;

    HOST = "localhost";
    PORT = 4223;
    UID = "XYZ"; % Change XYZ to the UID of your Real-Time Clock Bricklet 2.0

    ipcon = javaObject("com.tinkerforge.IPConnection"); % Create IP connection
    rtc = javaObject("com.tinkerforge.BrickletRealTimeClockV2", UID, ipcon); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Register date and time callback to function cb_date_time
    rtc.addDateTimeCallback(@cb_date_time);

    % Set period for date and time callback to 5s (5000ms)
    rtc.setDateTimeCallbackConfiguration(5000);

    input("Press key to exit\n", "s");
    ipcon.disconnect();
end

% Callback function for date and time callback
function cb_date_time(e)
    fprintf("Year: %d\n", e.year);
    fprintf("Month: %d\n", e.month);
    fprintf("Day: %d\n", e.day);
    fprintf("Hour: %d\n", e.hour);
    fprintf("Minute: %d\n", e.minute);
    fprintf("Second: %d\n", e.second);
    fprintf("Centisecond: %d\n", e.centisecond);
    fprintf("Weekday: %d\n", e.weekday);
    fprintf("Timestamp: %d\n", java2int(e.timestamp));
    fprintf("\n");
end

function int = java2int(value)
    if compare_versions(version(), "3.8", "<=")
        int = value.intValue();
    else
        int = value;
    end
end
