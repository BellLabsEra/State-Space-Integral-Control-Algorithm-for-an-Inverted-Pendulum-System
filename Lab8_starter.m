% David Taylor, Georgia Tech, 7/15/2023

% do not modify the next line
clc, clear all, close all

% indicate duration of experiment
experiment_duration_in_seconds = 30;

%% begin serial port session

port = serialport("COM9",1250000);
flush(port);

%% run experiment using specified interval

% enter mode 1
write(port,1,"uint8");

% wait for experiment to complete
pause(experiment_duration_in_seconds+0.5);

% enter mode 0
write(port,0,"uint8");

% declare experimental data
N = 5000;
r = NaN(1,N);
u = NaN(1,N);
y1 = NaN(1,N);
y2 = NaN(1,N);

% collect experimental data
for i = 1:N
    r(i) = single(read(port,1,"single"));
    u(i) = single(read(port,1,"single"));
    y1(i) = single(read(port,1,"single"));
    y2(i) = single(read(port,1,"single"));
end

% define time grid
T = 6*0.001;
t = (0:N-1)*T;

% plot results
subplot(311)
plot(t, y1), xlabel('s'), ylabel('y1 (rad)')
subplot(312)
plot(t, y2), xlabel('s'), ylabel('y2 (rad)')
subplot(313)
plot(t, u), xlabel('s'), ylabel('u (V)')
%% end serial port session

delete(port)
clear port