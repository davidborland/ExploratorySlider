function m = exploratory_slider_data_02(p)

p = double(p(:,:,1));
s = size(p,1);
f = fspecial('gaussian', s/2, 1.0);
p = imfilter(p, f, 'symmetric', 'same');
p = (p - min(min(p))) / (max(max(p)) - min(min(p)));


f = fspecial('gaussian', s/2, 1.0);


n = 32;


m = zeros(s,s,n);


r = floor(rand(1) * (n - 3)) + 2


scale = 100;


for i = 1:n 
	if i == r
        m(:,:,i) = p + max(max(m(:,:,i-1)));
        f = fspecial('gaussian', s/2, rand(1) * 2);
        continue;   
    elseif i == r - 1
        mm = rand(s,s);
        mm = imfilter(mm, f, 'symmetric', 'same');
        mm = (mm - min(min(mm))) / (max(max(mm)) - min(min(mm)));
        m(:,:,i) = mm + max(max(m(:,:,i-1)));
        continue;
    elseif i == r + 1
        mm = rand(s,s);
        mm = imfilter(mm, f, 'symmetric', 'same');
        mm = (mm - min(min(mm))) / (max(max(mm)) - min(min(mm)));
        m(:,:,i) = mm + max(max(m(:,:,i-1))); 
        continue;                
    end
       
    mm = rand(s,s);
    mm = imfilter(mm, f, 'symmetric', 'same');
    mm = (mm - min(min(mm))) / (max(max(mm)) - min(min(mm)));
       
%    mm = mm * ((sin(i / 2) + 1) / 2) * scale;
    mm = mm * rand(1) * scale;

    
	if i > 1
        mm = mm + max(max(m(:,:,i-1)));
    end
    
    m(:,:,i) = mm;
end






%{
% VERY CLOSE


p = double(p(:,:,1));
s = size(p,1);
%p = -p;
f = fspecial('gaussian', s/2, 1.0);
p = imfilter(p, f, 'symmetric', 'same');
p = (p - min(min(p))) / (max(max(p)) - min(min(p)));


f = fspecial('gaussian', s/2, 1.0);


n = 32;


m = zeros(s,s,n);


r = floor(rand(1) * (n - 3)) + 2


scale = 1;


for i = 1:n
    if i == r
        m(:,:,i) = p + max(max(m(:,:,i-1)));
        continue;   
%{
    elseif i == r - 1
        m(:,:,i) = rand(s,s) + max(max(m(:,:,i-1)));
        continue;
    elseif i == r + 1
        m(:,:,i) = rand(s,s) + max(max(m(:,:,i-1)));        
        continue;      
%}  
    end
        
    
    mm = rand(s,s);
    mm = imfilter(mm, f, 'symmetric', 'same');
    mm = (mm - min(min(mm))) / (max(max(mm)) - min(min(mm))); 

%{
    if (abs(i - r) > 1)
        mm = mm * scale;
    end
%}
    mm = mm * scale * abs(r - i) ^ 5;
    
    
	if i > 1
        mm = mm + max(max(m(:,:,i-1)));
    end
    
    m(:,:,i) = mm;
end

%}




%{


p = double(p(:,:,1));
%p = -p;
p = (p - min(min(p))) / (max(max(p)) - min(min(p)));


s = size(p,1);


f = fspecial('gaussian', s/2, 1.0);


scale = 10000.0;

offset = 100.0;

mm = rand(s,s);
mm = imfilter(mm, f, 'symmetric', 'same');
mm = (mm - min(min(mm))) / (max(max(mm)) - min(min(mm))); 
mm = mm * -scale + offset;
m(:,:,1) = mm;

m(:,:,2) = zeros(s,s) + offset;


mm = p;
mm = imfilter(mm, f, 'symmetric', 'same');
mm = (mm - min(min(mm))) / (max(max(mm)) - min(min(mm)));
mm = mm + offset;
m(:,:,3) = mm;

m(:,:,4) = ones(s,s) + offset;

mm = rand(s,s);
mm = imfilter(mm, f, 'symmetric', 'same');
mm = (mm - min(min(mm))) / (max(max(mm)) - min(min(mm)));
mm = mm * scale + (max(max(m(:,:,4))));
m(:,:,5) = mm;


%}