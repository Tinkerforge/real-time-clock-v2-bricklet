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

    if dateTime.weekday == BrickletRealTimeClockV2.WEEKDAY_MONDAY
        fprintf('Weekday: Monday\n');
    elseif dateTime.weekday == BrickletRealTimeClockV2.WEEKDAY_TUESDAY
        fprintf('Weekday: Tuesday\n');
    elseif dateTime.weekday == BrickletRealTimeClockV2.WEEKDAY_WEDNESDAY
        fprintf('Weekday: Wednesday\n');
    elseif dateTime.weekday == BrickletRealTimeClockV2.WEEKDAY_THURSDAY
        fprintf('Weekday: Thursday\n');
    elseif dateTime.weekday == BrickletRealTimeClockV2.WEEKDAY_FRIDAY
        fprintf('Weekday: Friday\n');
    elseif dateTime.weekday == BrickletRealTimeClockV2.WEEKDAY_SATURDAY
        fprintf('Weekday: Saturday\n');
    elseif dateTime.weekday == BrickletRealTimeClockV2.WEEKDAY_SUNDAY
        fprintf('Weekday: Sunday\n');
    end

    fprintf('Timestamp: %i ms\n', dateTime.timestamp);

    input('Press key to exit\n', 's');
    ipcon.disconnect();
end
