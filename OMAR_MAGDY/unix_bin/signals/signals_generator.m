pkg load signal

%% SIGNALS GENERATORS
% SINE WAVE
%TIME VECTOR
TIME_START = 0
TIME_END = 1;
samplingRate = 10000 %in sample/second
numSamples = samplingRate*(TIME_END - TIME_START) + 1;
phase_angle = 0;

Time = linspace(TIME_START, TIME_END, numSamples)';  
freq = 10;                  % Signal frequency in Hz
Amp = 220;                     % Signal amplitude
signal1 = Amp * sin(2 * pi * freq * Time + phase_angle);  % Generate the sinusoidal signal




data = [Time,signal1];
csvwrite('volt.csv', data);



%% SIGNALS GENERATORS
% SINE WAVE
%TIME VECTOR
TIME_START = 0
TIME_END = 0.05
samplingRate = 10000 %in sample/second
numSamples = samplingRate*(TIME_END - TIME_START) + 1
phase_angle = pi/4;


Time2 = linspace(TIME_START, TIME_END, numSamples) ;  
freq = 10;                  % Signal frequency in Hz
Amp = 1;                     % Signal amplitude
signal2 = Amp * sin(2 * pi * freq * Time2 + phase_angle);  % Generate the sinusoidal signal


%CONCATE TWO SIGNALS

TIME_START = 0.0501
TIME_END = 1
samplingRate = 10000 %in sample/second
numSamples = samplingRate*(TIME_END - TIME_START) + 1
phase_angle = pi/4;


Time3 = linspace(TIME_START, TIME_END, numSamples) ;  
freq = 10;                  % Signal frequency in Hz
Amp = 1;                     % Signal amplitude
signal3 = Amp * sin(2 * pi * freq * Time3 + phase_angle);  % Generate the sinusoidal signal


Time4 = [Time2 Time3];
signal4 = [signal2 signal3];

data = [Time4 ;signal4]';

csvwrite('current.csv', data);
