

pkg load signal

figure;

%%subplot(1,3,1);

filename = '../Load3.csv';
M = csvread(filename);
time1 = M(:, 1);  
value1 = M(:, 2); 

time2 = M(:, 1);
value2 = M(: , 3);

plot(time1, value2, 'b-'); % Blue solid line
xlabel('Time');
ylabel('VALUE');
title('value vs. Time');
grid on;


printf('voltage\n');
printf('RMS : ');
rms(value1)
printf('AVG : ');
mean(value1)

%%subplot(1,3,2);
hold on;
#filename = 'current_output_filtered.csv';
#M = csvread(filename);
time = M(:, 1);  
value = M(:, 3); 
plot(time, value, 'r-'); % Blue solid line
xlabel('Time');
ylabel('VALUE');
title('value vs. Time');
grid on;

printf('current\n');
printf('RMS : ');
rms(value2)
printf('AVG : ');
mean(value2)
hold on;

%%subplot(1,3,3);



print("power_plot");