function matlab_example_callback()
    import com.tinkerforge.IPConnection;
    import com.tinkerforge.BrickletRealTimeClockV2;

    HOST = 'localhost';
    PORT = 4223;
    UID = 'XYZ'; % Change XYZ to the UID of your Real-Time Clock Bricklet 2.0

    ipcon = IPConnection(); % Create IP connection
    rtc = handle(BrickletRealTimeClockV2(UID, ipcon), 'CallbackProperties'); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Register date and time callback to function cb_date_time
    set(rtc, 'DateTimeCallback', @(h, e) cb_date_time(e));

    % Set period for date and time callback to 5s (5000ms)
    rtc.setDateTimeCallbackConfiguration(5000);

    input('Press key to exit\n', 's');
    ipcon.disconnect();
end

% Callback function for date and time callback
function cb_date_time(e)
    fprintf('Year: %i\n', e.year);
    fprintf('Month: %i\n', e.month);
    fprintf('Day: %i\n', e.day);
    fprintf('Hour: %i\n', e.hour);
    fprintf('Minute: %i\n', e.minute);
    fprintf('Second: %i\n', e.second);
    fprintf('Centisecond: %i\n', e.centisecond);

    if e.weekday == com.tinkerforge.BrickletRealTimeClockV2.WEEKDAY_MONDAY
        fprintf('Weekday: Monday\n');
    elseif e.weekday == com.tinkerforge.BrickletRealTimeClockV2.WEEKDAY_TUESDAY
        fprintf('Weekday: Tuesday\n');
    elseif e.weekday == com.tinkerforge.BrickletRealTimeClockV2.WEEKDAY_WEDNESDAY
        fprintf('Weekday: Wednesday\n');
    elseif e.weekday == com.tinkerforge.BrickletRealTimeClockV2.WEEKDAY_THURSDAY
        fprintf('Weekday: Thursday\n');
    elseif e.weekday == com.tinkerforge.BrickletRealTimeClockV2.WEEKDAY_FRIDAY
        fprintf('Weekday: Friday\n');
    elseif e.weekday == com.tinkerforge.BrickletRealTimeClockV2.WEEKDAY_SATURDAY
        fprintf('Weekday: Saturday\n');
    elseif e.weekday == com.tinkerforge.BrickletRealTimeClockV2.WEEKDAY_SUNDAY
        fprintf('Weekday: Sunday\n');
    end

    fprintf('Timestamp: %i\n', e.timestamp);
    fprintf('\n');
end
