

clear all;
clf;


M0=zeros(256,256);
M0(1:256,1:100)=0;
M0(1:256,100:200)=0.5;
M0(1:256,200:256)=0.9;
subplot(2,3,1)
imshow(M0,[]);
title('input image');

N=256;

R = randint(N,N,[0,255]);
M=M0;
M(R<=10/255)=0;
M(R>250/255)=1;

M=M+M0;

subplot(2,3,2)
imshow(M-M0,[]);
title('salt and pepper noise');


subplot(2,3,3)
imshow(M,[]);
title('noisy image');

nbins=256;
[h,t]=hist(M0,nbins);
h = h/sum(h);
subplot(2,3,4);
bar(t,h);
set(gca,'YTick',[]);
axis([-1 2 0 max(h)*1.01]);
title('input histogram');
grid on;

[h,t]=hist(M-M0,nbins);
h = h/sum(h);
subplot(2,3,5);
bar(t,h);
set(gca,'YTick',[]);
axis([-1 2 0 max(h)*1.01]);
title('noise histogram');
grid on;


[h,t]=hist(M,nbins);
h = h/sum(h);
subplot(2,3,6);
bar(t,h);
set(gca,'YTick',[]);
axis([-1 2 0 max(h)]);

title('output histogram');
grid on;



%
%subplot(3,3,3)
%imshow(M-M1,[]);
%title('rayleigh noise');
