clear; close all; clc;
output = csvread('output.txt');
output(:, length(output)) = [];

s = size(output); height = s(1); width = s(2);

contour(-width/2:width/2-1, -height/2:height/2-1, flipud(output), 30);
axis equal

%% flat plate
hold on
plot([41 60], [60 41], '-k', 'LineWidth', 5);

%% plotting the line
c = 300; dx = 0.01;
t = 0.12;

x = 0:dx:c;

y = 5.*t.*c.*( (0.2969.*sqrt(x./c)) + (-0.1260.*(x./c)) + (-0.3516.*(x./c).*(x./c)) + (0.2843.*(x./c).*(x./c).*(x./c)) + (-0.1015.*(x./c).*(x./c).*(x./c).*(x./c)) );
hold on
plot(x-c/2, y-1, 'k', x-c/2, -y-1, 'k');
axis square
axis equal