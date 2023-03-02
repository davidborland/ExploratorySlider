function m = test_data()

s = 512;

v = 100;

m = zeros(s,s,s);

for i = 1:v
    m(i,:,:) = ones(s,s);
end
    