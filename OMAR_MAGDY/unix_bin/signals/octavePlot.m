

filename = 'current.csv';
M = csvread(filename);
time = M(:, 1);  
value = M(:, 2); 


%deriv = M(:, 3);  
%deriv2 = M(:, 4);
%integ = M(:, 5);

figure;
plot(time, value, 'b-'); % Blue solid line
xlabel('Time');
ylabel('VALUE');
title('value vs. Time');
grid on;

figure;
plot(time, deriv, 'b-'); % Blue solid line
xlabel('Time');
ylabel('dV/dT');
title('dv/time');
grid on;

figure;
plot(time, deriv2, 'b-'); % Blue solid line
xlabel('Time');
ylabel('dV2/dT2');
title('dv2/time2');
grid on;

figure;
plot(time, integ, 'b-'); % Blue solid line
xlabel('Time');
ylabel('vdt');
title('vdt vs. Time');
grid on;

avg = mean(value)
rms = mean(value.*value)