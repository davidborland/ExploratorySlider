function m = exploratory_slider_data(p)

p = double(p(:,:,1));
p = (p - min(min(p))) / (max(max(p)) - min(min(p)));

f = 100;
p = (p + f) / (f + 1);

s = size(p,1);

f = fspecial('gaussian', s/2, 1.0);

m(:,:,1) = rand(s,s);

downsample = 8;

for i = 2:s/downsample
%    m(:,:,i) = rand(s,s) * i + max(max(m(:,:,i-1)));
%    m(:,:,i) = p * i + max(max(m(:,:,i-1)));

    mm = rand(s,s);
%    mm = mm .* (mm>0.99);
    mm = imfilter(mm, f, 'symmetric', 'same');
    mm = (mm - min(min(mm))) / (max(max(mm)) - min(min(mm)));

	m(:,:,i) = mm * i^1.5 + max(max(m(:,:,i-1)));
end


i = ceil(s/downsample * rand(1));
%m(:,:,i) = ones(s,s)*0.5 * i + max(max(m(:,:,i-1)));
m(:,:,i) = p * i^1.5 + max(max(m(:,:,i-1)));
%m(:,:,i) = (1.0 - mm) * i + max(max(m(:,:,i-1)));
%m(:,:,i) = p * 0.75 + m(:,:,i) * 0.25;

min(min(m(:,:,i)))


i = ceil(s/downsample * rand(1));
%m(:,:,i) = ones(s,s)*0.5 * i + max(max(m(:,:,i-1)));
%m(:,:,i) = p * i + max(max(m(:,:,i-1)));

%min(min(m(:,:,i)))



%{
p1 = double(p1(:,:,1));
p1 = (p1 - min(min(p1))) / (max(max(p1)) - min(min(p1)));

p2 = double(p2(:,:,1));
p2 = (p2 - min(min(p2))) / (max(max(p2)) - min(min(p2)));

s = size(p1,1);

m = repmat(p2, [1 1 s]);

for i = 2:s
    m(:,:,i) = m(:,:,i) * i;
end

for i = 2:s
    m(:,:,i) = m(:,:,i) + max(max(m(:,:,i-1)));
end

m(:,:,20) = m(:,:,20) .* logical(p1);
%}



%{

s1 = 64;
s2 = 64;


m = zeros(s1,s1,s1);

n = 6;

for q = 1:n;
    
cx = floor(rand(1) * (s1 - 1) + 1);
cy = floor(rand(1) * (s1 - 1) + 1);
cz = floor(rand(1) * (s1 - 1) + 1); 

v = rand(1) * 100;

r = rand(1) * s1 / 10 + 1;
    
for i = 1:s1;
    for j = 1:s1;
        for k = 1:s1
            x = cx - i;
            y = cy - j;
            z = cz - k;
            
            d = sqrt(x*x + y*y + z*z);
            
            if (d <= r) 
                m(i,j,k) = d / r * v;
            end
        end
    end
end

end

%m = imresize(m, [s2 s2 s2]);

%}



%{
p = double(p(:,:,1));
p = (p - min(min(p))) / (max(max(p)) - min(min(p)));


s = 512;

m = zeros(s,s,s);


sp = size(p,1);

f = fspecial('gaussian', sp / 2, 5.0);
p2 = imfilter(p, f, 'replicate', 'same'); 


h = 200;

m(:,:,h) = sa






s = size(p, 1);

for i = 1:s
    m(:,:,i) = ones(s,s) * abs(sin(i / 64));
end

for i = 2:s
    m(:,:,i) = m(:,:,i) + max(max(m(:,:,i-1)));
end

%}



%{
p = double(p(:,:,1));
p = (p - min(min(p))) / (max(max(p)) - min(min(p)));

s = size(p, 1);

f = fspecial('gaussian', s / 2, 1.0);

m = ones(s,s,s);
m(:,:,s/2) = p;


n = 3;

for i = 1:n
    h = floor(rand(1) * (s - 2) + 1)
    
    mi = ones(s,s,s);
    
    mi(:,:,h) = p;
    
    for j = h + 1:s
        mj = imfilter(mi(:,:,j-1), f, 'replicate', 'same'); 
        mj = (mj - min(min(mj))) / (max(max(mj)) - min(min(mj)));     
        mj = mj * j;
        
        mi(:,:,j) = mj;
    end
    
    for i = 2:s
        mi(:,:,i) = mi(:,:,i) + mi(:,:,i-1);
    end
    
    m = m + mi;
end
%}



%{
p = double(p(:,:,1));
p = (p - min(min(p))) / (max(max(p)) - min(min(p)));

s = size(p, 1);

m = rand(s,s);

for i = 2:s    
    m(:,:,i) = ones(s,s) * 1.1^i + max(max(m(:,:,i-1)));
end


i = 20;

m(:,:,i) = p * 1.05^i + max(max(m(:,:,i-1)));

i = i - 1;
m(:,:,i-1) = rand(s,s) * 1.1^i + max(max(m(:,:,i-1)));

i = i + 2;
m(:,:,i+1) = rand(s,s) * 1.1^i + max(max(m(:,:,i-1)));

%}





%{
s = 64;

m = zeros(s,s,s);

sigma = 5.0;

den = 2 * sigma * sigma;

c = [10 32 32];
for i = 1:s
    for j = 1:s
        for k = 1:s
            x = [i j k] - c;
            x = x .* x;
            v = exp(-(x(1) / den + x(2) / den + x(3) / den));
            
            m(i,j,k) = m(i,j,k) + v^4;
        end
    end
end

   c = [30 32 32];
for i = 1:s
    for j = 1:s
        for k = 1:s
            x = [i j k] - c;
            x = x .* x;
            v = exp(-(x(1) / den + x(2) / den + x(3) / den));
            
            m(i,j,k) = m(i,j,k) + v;
        end
    end
end         

c = [50.01 32 32];
for i = 1:s
    for j = 1:s
        for k = 1:s
            x = [i j k] - c;
            x = x .* x;
            v = exp(-(x(1) / den + x(2) / den + x(3) / den));
            
            m(i,j,k) = m(i,j,k) + v^4;
        end
    end
end
%}

%{

% Good interesting volume

s = 64;

m = zeros(s, s, s);

c = (s - 1) / 2;

sigma = 10.0;
den = 2 * sigma * sigma;

n = 20;

for q = 1:n
    
cx = rand(1) * (s-1) + 1;
cy = rand(1) * (s-1) + 1;
cz = rand(1) * (s-1) + 1;

p = rand(1) * 9 + 1;

for i = 1:s
    for j = 1:s
        for k = 1:s
            x = i - cx;
            y = j - cy;
            z = k - cz;
            
            v = exp(-(x * x / den + y * y / den + z * z / den));
            
            v = v^p;
            
            m(i,j,k) = m(i,j,k) + v;
        end
    end
end

end

for q = 1:n
    
cx = rand(1) * (s-1) + 1;
cy = rand(1) * (s-1) + 1;
cz = rand(1) * (s-1) + 1;

p = rand(1) * 9 + 1;

for i = 1:s
    for j = 1:s
        for k = 1:s
            x = i - cx;
            y = j - cy;
            z = k - cz;
            
            v = exp(-(x * x / den + y * y / den + z * z / den));
            
            v = v^p;
            
            m(i,j,k) = m(i,j,k) - v;
        end
    end
end

end
         
%}


%{
p = (p - min(min(p))) / (max(max(p)) - min(min(p)));

f = fspecial('gaussian', 64, 10.0);

m = imfilter(p, f, 'replicate', 'same');

m = (m - min(min(m))) / (max(max(m)) - min(min(m)));

%}


%{

p = (p - min(min(p))) / (max(max(p)) - min(min(p)));

s = 512;
m = zeros(s, s);

w = 64;

p = imresize(p, [w w]);

n = 8;

for q = 1:n;

cx = floor(rand(1) * (s - w - 1) + w/2) + 1;
cy = floor(rand(1) * (s - w - 1) + w/2) + 1;

v = rand(1) * 100;
v = v * v;

for i = 1:w
    for j = 1:w
        x = cx - i + w/2;
        y = cy - j + w/2;

        m(x,y) = p(i,j) + v;
    end
end

end
%}


%{
s = 256;
m = zeros(s, s);

n = 8;

w = 8;

g = fspecial('gaussian', s, w);


for q = 1:n
    
m1 = zeros(s, s);
m2 = zeros(s, s);
    
cx = rand(1) * (s - w * 4) + w * 2;
cy = rand(1) * (s - w * 4) + w * 2;

cx = floor(cx);
cy = floor(cy);

v = rand(1) * 0.9 + 0.1;
v = v*v*v;
    
for i = cx - w:cx + w 
    for j = cy - w:cy + w            
        m1(i, j) = 1;
    end
end

m2(cx, cy) = 1;
m2 = imfilter(m2, g, 'replicate', 'same');
m2 = (m2 - min(min(m2))) / (max(max(m2)) - min(min(m2)));
m2 = m2 * v;

indeces = m1 > 0;
m2(indeces) = max(m2(indeces));

m = m + m2;

end 
    

m = repmat(m, [1 1 s]);
%}

%{

s = 256;

c = (s - 1) / 2;

m = zeros(s, s, s);

n = 4;

w = 8;

for i = 1:s
    for j = 1:s
        for k = 1:s
            x = c - i;
            y = c - j;
            z = c - k;
            
            m(i,j,k) = sqrt(x * x + y * y + z * z);
        end
    end
end


for q = 1:n
    
cx = floor(rand(1) * (s - 2*w - 1) + w + 1);
cy = floor(rand(1) * (s - 2*w - 1) + w + 1);
cz = floor(rand(1) * (s - 2*w - 1) + w + 1);
    
for i = cx-w:cx+w
    for j = cy-w:cy+w
        for k = cz-w:cz+w
            m(i,j,k) = m(i,j,k) * 0.999;
        end
    end
end

end


m = (m - min(min(min(m)))) / (max(max(max(m))) / min(min(min(m))));


%}

%{

s = 256;
m = zeros(s, s, s);

n = 2;

sigma2 = rand(1) * 10 + 10;
sigma2 = sigma2 * sigma2;

for q = 1:n;
    
cx = rand(1) * (s - 1) + 1;
cy = rand(1) * (s - 1) + 1;
cz = rand(1) * (s - 1) + 1;

v = 1;

for i = 1:s
    for j = 1:s
        for k = 1:s
            x = cx - i;
            y = cy - j;
            z = cz - k;
            
            m(i,j,k) = m(i,j,k) + exp(-v * (x * x / (2 * sigma2) + y * y / (2 * sigma2) + z * z / (2 * sigma2)));
            
            m(i,j,k) = m(i,j,k) - exp(-v * (x * x / (2 * sigma2 * 2) + y * y / (2 * sigma2 * 2) + z * z / (2 * sigma2 * 2)));
        end
    end
end

end


%}



%{
s = 512;
m = zeros(s, s);

n = 4;

w = 8;

g = fspecial('gaussian', s, w * 2);


for q = 1:n
    
m1 = zeros(s, s);
m2 = zeros(s, s);
    
cx = rand(1) * (s - w * 4) + w * 2;
cy = rand(1) * (s - w * 4) + w * 2;

cx = floor(cx);
cy = floor(cy);

v = rand(1) * 0.9 + 0.1;
    
for i = cx - w:cx + w 
    for j = cy - w:cy + w            
        m1(i, j) = 1;
    end
end

m2(cx, cy) = 1;
m2 = imfilter(m2, g, 'replicate', 'same');
m2 = (m2 - min(min(m2))) / (max(max(m2)) - min(min(m2)));
m2 = m2 * v;

indeces = m1 > 0;
m2(indeces) = mean(m2(indeces));

m = m + m2;

end    

%}


%{

s = 256;
m = zeros(s, s, s);

n = 4;

for q = 1:n;
    
cx = rand(1) * (s - 1) + 1;
cy = rand(1) * (s - 1) + 1;
cz = rand(1) * (s - 1) + 1;

sigma2 = rand(1) * 10 + 10;
sigma2 = sigma2 * sigma2;

for i = 1:s
    for j = 1:s
        for k = 1:s
            x = cx - i;
            y = cy - j;
            z = cz - k;
            
            m(i, j, k) = m(i, j, k) + exp(-(x * x / (2 * sigma2) + y * y / (2 * sigma2) + z * z / (2 * sigma2)));
        end
    end
end

end

%}

%{

s = 128;
m = zeros(s, s, s);

n = 8;

for q = 1:n;
    
cx = floor(rand(1) * (s - 1) + 1);
cy = floor(rand(1) * (s - 1) + 1);
cz = floor(rand(1) * (s - 1) + 1);

r = floor(rand(1) * s / 2);

for i = 1:s
    for j = 1:s
        for k = 1:s
            x = cx - i;
            y = cy - j;
            z = cz - k;
            
            d = sqrt(x * x + y * y + z * z);
            
            if (d > r) 
                m(i,j,k) = max(m(i,j,k), 1 / d);
            else
                m(i,j,k) = d;
            end            
        end
    end
end

end

%}

%{

s = 1024;
m = zeros(s, s);

for i = 1:100
    m2 = zeros(s, s);
    
    x = floor(rand(1) * (s - 1) + 1);
    y = floor(rand(1) * (s - 1) + 1);
    
    m2(x, y) = 1;
    
    
    hs = floor(rand(1) * 500 + 100);
    sigma = rand(1) * 100 + 1;
    
    h = fspecial('gaussian', hs, sigma);
    
    m2 = imfilter(m2, h, 'replicate', 'same');
    
    m = m + m2;
end

m = (m - min(min(m))) / (max(max(m)) - min(min(m)));

%}

%{
s = 1024
c = s / 2.0

m = zeros(s, s);

for i = 1:s
    for j = 1:s
        x = c - i;
        y = c - j;
            
        d = sqrt(x * x + y * y);
            
        m(i, j) = d / c;
    end
end

m = -m + max(max(m));

m = (m - min(min(m))) / (max(max(m)) - min(min(m)));
%}



%{
s = 8;
c = s / 2;

m = zeros(s, s, s);

for i = 1:s
    for j = 1:s
        for k = 1:s
            x = c - i;
            y = c - j;
            z = c - k;
            
            d = sqrt(x * x + y * y + z * z);
            
            m(i, j, k) = d / c;
        end
    end
end
%}