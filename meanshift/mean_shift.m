
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
mu = [0 0];
Sigma = [.25 .3; .3 1];
x1 = -3:.2:3; x2 = -3:.2:3;
[X1,X2] = meshgrid(x1,x2);
F = mvnpdf([X1(:) X2(:)],mu,Sigma);
F = reshape(F,length(x2),length(x1));
%surf(x1,x2,F);
%hold on;

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%




%caxis([min(F(:))-.5*range(F(:)),max(F(:))]);
%axis([-3 3 -3 3 0 .4])
%xlabel('x1'); ylabel('x2'); 
%zlabel('Probability Density');
%figure
X = [mvnrnd(mu,Sigma,200)];
%scatter(X(:,1),X(:,2),10,'ko')
%axis([-3 3 -3 3])



p=gkde2(X);

p1=gkde2x(X,1);
p2=gkde2x(X,2);

vector=[-1,-2];
[a,b]=min(abs(p.x(1,:)-vector(1)));
[c,d]=min(abs(p.y(:,1)-vector(2)));
value=p.pdf(b,d);
%vector=[-1,-2];
%kde_pdf(vector,p)

%%% Thus given a non parametric estimate of the PDF %%%%%%
%%% find the value at -1,2 using interpolation %%%
%
figure(1)
surf(p.x,p.y,p.pdf);	
hold on;
caxis([min(F(:))-.5*range(F(:)),max(F(:))]);
axis([-3 3 -3 3 0 .4])
xlabel('x1'); ylabel('x2'); 
zlabel('Probability Density Estimate');

[dx,dy]=gradient(p.pdf);
filename = '/media/software/Dropbox/Dropbox/repository/im/documents/x/a3.gif';

%writerObj = VideoWriter(filename);
%open(writerObj);
%aviobj = avifile('example.avi','compression','None');

n=1;
while 1==1
value = interp2(p.x,p.y,p.pdf,vector(1),vector(2),'spline') ;    
plot3(vector(1),vector(2),value,'b*','MarkerSize',3,'LineWidth',3);
frame = getframe(1);
im = frame2im(frame);
[A,map] = rgb2ind(im,256); 
if n == 1;
		imwrite(A,map,filename,'gif','LoopCount',Inf,'DelayTime',1);
	else
		imwrite(A,map,filename,'gif','WriteMode','append','DelayTime',1);
	end
%writeVideo(writerObj,frame);

n=n+1;
%dx2=interp2(p.x,p.y,dx,vector(1),vector(2),'spline') ;    
%dy2=interp2(p.x,p.y,dy,vector(1),vector(2),'spline') ;    
dx1=interp2(p1.x,p1.y,p1.pdf,vector(1),vector(2),'spline');
dy1=interp2(p2.x,p2.y,p2.pdf,vector(1),vector(2),'spline');


%dx1=dx1./(value+eps);
%dy1=dy1./(value+eps);
dx1=2*p1.h(1)*p1.h(1)*dx1;
dy1=2*p2.h(2)*p2.h(2)*dy1;

nvector(1)=vector(1)+dx1;
nvector(2)=vector(2)+dy1;
e=nvector-vector;
e=sqrt(e*e');
if e < 0.001
    break;
end
vector=nvector;
e
end

%close(writerObj);

%quiver(p.x,p.y,dx,dy)

%p1=gkde2mean(X);
%quiver(p.x,p.y,p1.mpdf(:,:,1),p1.mpdf(:,:,2))
%figure

% x=ones(1,4);
% y=[-2:0.2:-3];
% plot3(-1,-2,(value),'r','MarkerSize',10);
% 
% caxis([min(F(:))-.5*range(F(:)),max(F(:))]);
% axis([-3 3 -3 3 0 .4])
% 
% x=[-1,-2];
% value=mvnpdf(x,mu,Sigma)
% x=ones(1,4);
% y=[-2:0.2:-3];
% plot3(-1,-2,(value),'r','MarkerSize',10);
% 

