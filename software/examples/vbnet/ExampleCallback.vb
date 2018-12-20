Imports System
Imports Tinkerforge

Module ExampleCallback
    Const HOST As String = "localhost"
    Const PORT As Integer = 4223
    Const UID As String = "XYZ" ' Change XYZ to the UID of your Real-Time Clock Bricklet 2.0

    ' Callback subroutine for date and time callback
    Sub DateTimeCB(ByVal sender As BrickletRealTimeClockV2, ByVal year As Integer, _
                   ByVal month As Byte, ByVal day As Byte, ByVal hour As Byte, _
                   ByVal minute As Byte, ByVal second As Byte, _
                   ByVal centisecond As Byte, ByVal weekday As Byte, _
                   ByVal timestamp As Long)
        Console.WriteLine("Year: " + year.ToString())
        Console.WriteLine("Month: " + month.ToString())
        Console.WriteLine("Day: " + day.ToString())
        Console.WriteLine("Hour: " + hour.ToString())
        Console.WriteLine("Minute: " + minute.ToString())
        Console.WriteLine("Second: " + second.ToString())
        Console.WriteLine("Centisecond: " + centisecond.ToString())

        If weekday = BrickletRealTimeClockV2.WEEKDAY_MONDAY Then
            Console.WriteLine("Weekday: Monday")
        Else If weekday = BrickletRealTimeClockV2.WEEKDAY_TUESDAY Then
            Console.WriteLine("Weekday: Tuesday")
        Else If weekday = BrickletRealTimeClockV2.WEEKDAY_WEDNESDAY Then
            Console.WriteLine("Weekday: Wednesday")
        Else If weekday = BrickletRealTimeClockV2.WEEKDAY_THURSDAY Then
            Console.WriteLine("Weekday: Thursday")
        Else If weekday = BrickletRealTimeClockV2.WEEKDAY_FRIDAY Then
            Console.WriteLine("Weekday: Friday")
        Else If weekday = BrickletRealTimeClockV2.WEEKDAY_SATURDAY Then
            Console.WriteLine("Weekday: Saturday")
        Else If weekday = BrickletRealTimeClockV2.WEEKDAY_SUNDAY Then
            Console.WriteLine("Weekday: Sunday")
        End If

        Console.WriteLine("Timestamp: " + timestamp.ToString())
        Console.WriteLine("")
    End Sub

    Sub Main()
        Dim ipcon As New IPConnection() ' Create IP connection
        Dim rtc As New BrickletRealTimeClockV2(UID, ipcon) ' Create device object

        ipcon.Connect(HOST, PORT) ' Connect to brickd
        ' Don't use device before ipcon is connected

        ' Register date and time callback to subroutine DateTimeCB
        AddHandler rtc.DateTimeCallback, AddressOf DateTimeCB

        ' Set period for date and time callback to 5s (5000ms)
        rtc.SetDateTimeCallbackConfiguration(5000)

        Console.WriteLine("Press key to exit")
        Console.ReadLine()
        ipcon.Disconnect()
    End Sub
End Module
