

clear variables;
close all;
pkg load signal


freq=[261.6,293.7,329.7,349.2,392,440,493.9,523.2,587.4,659.4,698.4,784,880,987.8,1046.4,1174.8,1318.8,1396.8,1568,1760,1975.6];
gamme=["DO";"RE";"MI";"FA";"SOL";"LA";"SI"];
[y,Fs] = audioread('sinedegrade_cligno.wav');

y=y(:,1)+y(:,2); %on réunit les canaux stereo en un seul canal mono
%Fs=12000
stop=1;
cpt=1;
mode=0;
p_haut=0;
p_bas=0;
while stop==1
  
    if (mode==2)&&(not(y(cpt)==0)) %on a trouvé la fin de la période basse
      p_bas=cpt-1;
      stop=0;
    endif
    
    if (mode==1)&&(y(cpt)==0)&&(((cpt-p_haut)/Fs)>=0.0039) %si on dépasse la freq de la note la plus basse, on est dans la période basse donc en mode 2
      mode=2;
    endif

    if (mode==0)&&(y(cpt)==0) %si il ya un point on démarre la vérification de la période basse
      mode=1;
      p_haut=cpt;
    endif
    
    if (mode==1)&&(not(y(cpt)=!0)) %en fait ce n'était pas la période basse on recommence
      mode=0;
    endif
    
    if cpt==length(y)%on s'arrete s'il n'y a pas de clignotement
      stop=0;
      p_haut=length(y);
    endif
    cpt+=1;
    
end
yp=y(1:p_haut);%on récupère la première partie haute

N = p_haut;            % samples
dF = Fs/N;                 % hertz per sample
f = -Fs/2:dF:Fs/2-dF + (dF/2)*mod(N,2);      % hertz
Y1_fft = fftshift(fft(yp))/N;

[c,I]=max(abs(Y1_fft));
f_sig=abs(f(I))

note='N/A';
for i=1:length(freq)-1
  moy=(freq(i)+freq(i+1))/2;
  if (f_sig>moy)&&(f_sig<=freq(i+1))
    note=gamme(rem(i-1,7)+2,:);
  end
  if (f_sig<=moy)&&(f_sig>=freq(i))
    note=gamme(rem(i-1,7)+1,:);
  end
end


p_bas=p_bas-p_haut
p_haut
note