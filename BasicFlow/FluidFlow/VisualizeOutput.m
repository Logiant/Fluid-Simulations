clear; close all; clc;
output = csvread('output.txt');
output(:, length(output)) = [];
contour(flipud(output), 60);

%% flat plate
hold on
plot([41 60], [60 41], '-k', 'LineWidth', 5);

%% plotting the line
c = 10; dx = 0.01;
t = 0.12;

x = 0:dx:c;

y = 5.*t.*c.*( (0.2969.*sqrt(x./c)) + (-0.1260.*(x./c)) + (-0.3516.*(x./c).*(x./c)) + (0.2843.*(x./c).*(x./c).*(x./c)) + (-0.1015.*(x./c).*(x./c).*(x./c).*(x./c)) );
plot(x, y, x, -y);
axis square
axis equal