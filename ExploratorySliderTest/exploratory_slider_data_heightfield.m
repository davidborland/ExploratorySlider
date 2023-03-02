function m = exploratory_slider_data_heightfield()

s = 512;

m = zeros(s,s);

n = 10;

x = floor(rand(10) * (s*0.9)) + 1;
y = floor(rand(10) * (s*0.9)) + 1;

for i = 1:n
    m(x(i),y(i)) = rand(1) ^ 10;
end

g = fspecial('gaussian', 32, 5.0);

m = imfilter(m, g, 'symmetric', 'same');

m = mat2gray(m) * 100;

