program ExampleCallback;

{$ifdef MSWINDOWS}{$apptype CONSOLE}{$endif}
{$ifdef FPC}{$mode OBJFPC}{$H+}{$endif}

uses
  SysUtils, IPConnection, BrickletRealTimeClockV2;

type
  TExample = class
  private
    ipcon: TIPConnection;
    rtc: TBrickletRealTimeClockV2;
  public
    procedure DateTimeCB(sender: TBrickletRealTimeClockV2; const year: word;
                         const month: byte; const day: byte; const hour: byte;
                         const minute: byte; const second: byte; const centisecond: byte;
                         const weekday: byte; const timestamp: int64);
    procedure Execute;
  end;

const
  HOST = 'localhost';
  PORT = 4223;
  UID = 'XYZ'; { Change XYZ to the UID of your Real-Time Clock Bricklet 2.0 }

var
  e: TExample;

{ Callback procedure for date and time callback }
procedure TExample.DateTimeCB(sender: TBrickletRealTimeClockV2; const year: word;
                              const month: byte; const day: byte; const hour: byte;
                              const minute: byte; const second: byte;
                              const centisecond: byte; const weekday: byte;
                              const timestamp: int64);
begin
  WriteLn(Format('Year: %d', [year]));
  WriteLn(Format('Month: %d', [month]));
  WriteLn(Format('Day: %d', [day]));
  WriteLn(Format('Hour: %d', [hour]));
  WriteLn(Format('Minute: %d', [minute]));
  WriteLn(Format('Second: %d', [second]));
  WriteLn(Format('Centisecond: %d', [centisecond]));
  WriteLn(Format('Weekday: %d', [weekday]));
  WriteLn(Format('Timestamp: %d', [timestamp]));
  WriteLn('');
end;

procedure TExample.Execute;
begin
  { Create IP connection }
  ipcon := TIPConnection.Create;

  { Create device object }
  rtc := TBrickletRealTimeClockV2.Create(UID, ipcon);

  { Connect to brickd }
  ipcon.Connect(HOST, PORT);
  { Don't use device before ipcon is connected }

  { Register date and time callback to procedure DateTimeCB }
  rtc.OnDateTime := {$ifdef FPC}@{$endif}DateTimeCB;

  { Set period for date and time callback to 5s (5000ms) }
  rtc.SetDateTimeCallbackConfiguration(5000);

  WriteLn('Press key to exit');
  ReadLn;
  ipcon.Destroy; { Calls ipcon.Disconnect internally }
end;

begin
  e := TExample.Create;
  e.Execute;
  e.Destroy;
end.
