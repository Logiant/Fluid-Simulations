output = csvread('output.txt');
output(:, length(output)) = [];
contour(flipud(output), 30);