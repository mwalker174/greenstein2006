%        -------------------------------------------------------------
% 
%          NOTICE OF COPYRIGHT AND OWNERSHIP OF SOFTWARE
% 
%          Copyright 2005, The Johns Hopkins University
%             School of Medicine and Whiting School of Engineering. 
%             All rights reserved.
% 			For research use only; commercial use prohibited.
% 			Distribution without permission of Joseph L. Greenstein or
% 			Raimond L. Winslow not permitted. 
%           (jgreenst@jhu.edu, rwinslow@jhu.edu)
% 
%          Name of Program: 40-State coupled LCC-RyR Model
%          Version: version 1.0
%          Date: September 2005
% 
%        -------------------------------------------------------------  

global Vclamp_flag Period Shift Pulse_duration Pulse_amplitude Nstates_CaRU
global Counter T_array ICaL_array JCaL_array JRyR_array CaSSavg_array 

Counter = 1;
Length_array = 100000;
options1 = odeset('MaxStep',0.1);       %   Set integrator options

T_array = zeros(Length_array,1);        %   Initialize large arrays
T_array(1) = 123;                       %   Large value here prevents Counter from incrementing on first call to fcn_40state
ICaL_array = zeros(Length_array,1);
JCaL_array = zeros(Length_array,1);
JRyR_array = zeros(Length_array,1);
CaSSavg_array = zeros(Length_array,1);
T = [];
Y = [];

Vclamp_flag = 0;                        %   = 0 for APs, = 1 for voltage clamp 
Period = 1000;                          %   Period between stimuli or voltage clamp (ms)
Shift=5;                                %   Delay between start of Period and stimulus (ms)
Pulse_duration=0.5;                     %   Duration of stimulus (ms)
Pulse_amplitude=-80;                    %   Amplitude of stimulus current (A/F)
Num_beats = 10;                         %   Simulation runs on interval [0, Num_beats*Period]

Nstates_CaRU = 40;                      %   Number of states in the coupled LCC-RyR model

                                        %   Set initial conditions of states
if Vclamp_flag
    IC_CaRU = zeros(1,40);
    IC_CaRU(1) = 1;
    IC_global_VC = [ -100.0100    0.0001    0.9995    0.9995   10.0000  134.5668    0.0001    0.7500    0.1033    0.9781 ...
                        0.0001    0.9683    0.0134    0.0001    0.0000    0.0000    0.0153    0.0027    0.0002    0.0000 ...
                        0.0000    0.8232    0.0522    0.0012    0.0000    0.0000    0.1192    0.0034    0.0007    0.0001 ...
                        0.0000    0.9998    0.0002    0.0000    0.0000    0.0000 ];
    IC_VC = [ IC_CaRU IC_global_VC];
    IC = IC_VC;
else
    IC_AP = 100* ...
     [0.00682939648962  0.00000003363556  0.00000000054922  0.00004610432538  0.00000009423947 ...
      0.00035998715326  0.00000000177342  0.00000000075367  0.00000242134125  0.00000000495121 ...
      0.00000000252252  0.00000000000001  0.00000000000000  0.00000000002125  0.00000000000004 ...
      0.00000000012458  0.00000000000000  0.00000000000000  0.00000000000091  0.00000000000000 ...
      0.00000021351848  0.00000000000603  0.00000001314943  0.00000016333934  0.00000000033371 ...
      0.00000000364257  0.00000000000002  0.00000000000001  0.00000000284231  0.00000000000581 ...
      0.00261492026499  0.00000001288289  0.00000000607437  0.00001796055777  0.00000003671797 ...
      0.00012775068729  0.00000000063057  0.00000000026748  0.00000086504155  0.00000000177104 ...
     -0.93319772769854  0.00000380504749  0.00997668179931  0.00998165969884  0.10447882220110 ...
      1.31625456905363  0.00000128015497  0.00891915411676  0.00114462781701  0.00980722138327 ...
      0.00000175551540  0.00957567652704  0.00021947168410  0.00000188633504  0.00000000720571 ...
      0.00000000001032  0.00015206520187  0.00004470341235  0.00000665651077  0.00000052176869 ...
      0.00000001229128  0.00749980626396  0.00088108760299  0.00003881711321  0.00000076012995 ...
      0.00000000560386  0.00146019656332  0.00007653042561  0.00002905030557  0.00001075016020 ...
      0.00000657420277  0.00999574286291  0.00000355495242  0.00000062513877  0.00000006753310 ...
      0.00000000951191  ];
    IC = IC_AP;
end

if Vclamp_flag              %   Uses Vclamp_flag to choose simulation(s) below
    AP_flag = 0;
    Gain_flag = 1;
else
    AP_flag = 1;
    Gain_flag = 0;
end    

        dy = fcn_40state(0,IC)
                            % Run APs.  Requires Vclamp_flag = 0
if AP_flag
    for i = 1:Num_beats
        i
        T_range = [(i-1)*Period (i-1)*Period+Shift+5];
        [Ta,Ya] = ode23t(@fcn_40state,T_range,IC,options1);
        T_range = [(i-1)*Period+Shift+5 i*Period];
        IC = Ya(end,:);
        [Tb,Yb] = ode23t(@fcn_40state,T_range,IC);
        T = [T; Ta; Tb];
        Y = [Y; Ya; Yb];
        IC = Yb(end,:);
    end
    figure; plot(T,Y(:,Nstates_CaRU+1))
end

                            % Generate EC coupling gain curve.  Requires Vclamp_flag = 1
if Gain_flag
    VC = [ -50.01:5:-5.01 0.01:5:60.01];
    clear JCaLp JRyRp
    rVC = round(VC);
    for i = 1:length(VC)
        Counter1 = Counter + 1;
        T_range = [0 5];
        IC = IC_VC;
        IC(Nstates_CaRU+1) = -100.01;
        [T1,Y1] = ode23t(@fcn_40state,T_range,IC,options1);
        T_range = [5 100];
        IC = Y1(end,:);
        IC(Nstates_CaRU+1) = VC(i);
        [T2,Y2] = ode23t(@fcn_40state,T_range,IC,options1);
        JCaLp(i) = max(JCaL_array(Counter1:Counter));
        JRyRp(i) = max(JRyR_array(Counter1:Counter));
        rVC(i)
    end
    ECGain1 = JRyRp./JCaLp;
    VolScale = 0.203e-12/25.84e-6*1000;  %  = VSS/Vmyo*1000
    figure;
    subplot(3,1,1); plot(rVC,JRyRp*VolScale,rVC,JCaLp*VolScale)
    subplot(3,1,2); plot(rVC,JRyRp/max(JRyRp),rVC,JCaLp/max(JCaLp))
    subplot(3,1,3); plot(rVC,ECGain1)
    set(gcf,'Position', [150   150   350   700])
end

T_array = T_array(1:Counter);           %   Truncate unused space from these storage arrays
ICaL_array = ICaL_array(1:Counter);
JCaL_array = JCaL_array(1:Counter);
JRyR_array = JRyR_array(1:Counter);
CaSSavg_array = CaSSavg_array(1:Counter);

