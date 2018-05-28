function octave_example_simple()
    more off;

    HOST = "localhost";
    PORT = 4223;
    UID = "XYZ"; % Change XYZ to the UID of your Real-Time Clock Bricklet 2.0

    ipcon = javaObject("com.tinkerforge.IPConnection"); % Create IP connection
    rtc = javaObject("com.tinkerforge.BrickletRealTimeClockV2", UID, ipcon); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Get current date and time
    dateTime = rtc.getDateTime();

    fprintf("Year: %d\n", dateTime.year);
    fprintf("Month: %d\n", dateTime.month);
    fprintf("Day: %d\n", dateTime.day);
    fprintf("Hour: %d\n", dateTime.hour);
    fprintf("Minute: %d\n", dateTime.minute);
    fprintf("Second: %d\n", dateTime.second);
    fprintf("Centisecond: %d\n", dateTime.centisecond);
    fprintf("Weekday: %d\n", dateTime.weekday);
    fprintf("Timestamp: %d ms\n", java2int(dateTime.timestamp));

    input("Press key to exit\n", "s");
    ipcon.disconnect();
end

function int = java2int(value)
    if compare_versions(version(), "3.8", "<=")
        int = value.intValue();
    else
        int = value;
    end
end
