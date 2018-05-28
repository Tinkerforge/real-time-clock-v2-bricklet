function matlab_example_simple()
    import com.tinkerforge.IPConnection;
    import com.tinkerforge.BrickletRealTimeClockV2;

    HOST = 'localhost';
    PORT = 4223;
    UID = 'XYZ'; % Change XYZ to the UID of your Real-Time Clock Bricklet 2.0

    ipcon = IPConnection(); % Create IP connection
    rtc = handle(BrickletRealTimeClockV2(UID, ipcon), 'CallbackProperties'); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Get current date and time
    dateTime = rtc.getDateTime();

    fprintf('Year: %i\n', dateTime.year);
    fprintf('Month: %i\n', dateTime.month);
    fprintf('Day: %i\n', dateTime.day);
    fprintf('Hour: %i\n', dateTime.hour);
    fprintf('Minute: %i\n', dateTime.minute);
    fprintf('Second: %i\n', dateTime.second);
    fprintf('Centisecond: %i\n', dateTime.centisecond);
    fprintf('Weekday: %i\n', dateTime.weekday);
    fprintf('Timestamp: %i ms\n', dateTime.timestamp);

    input('Press key to exit\n', 's');
    ipcon.disconnect();
end
