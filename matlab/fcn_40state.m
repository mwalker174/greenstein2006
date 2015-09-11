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


function dy = fcn_40state(t,y)

global Vclamp_flag Period Shift Pulse_duration Pulse_amplitude Nstates_CaRU
global Counter T_array ICaL_array JCaL_array JRyR_array CaSSavg_array 

NCaRU = 50000;                 %   Number of Ca Release Units
Faraday = 96.5;                 %   Faraday's constant (C/mmol)
Temp = 310;                     %   Temperature (K)
Rgas = 8.314;                   %   Universal Gas constant (J/[mol K])
RT_over_F = Rgas*Temp/Faraday;
Csa = 153.4e-6;                 %   Membrane capacitance (pF)
VSS = 0.203e-12;                %   Single dyad subspace volume (microL)
VSR = 1.3913e-6;                %   SR volume (microL)
Vmyo = 25.84e-6;                %   Myoplasmic volume (microL)
PCa = 9.13e-13;                 %   LCC single channel permeability (cm^3/s)
JRyRmax = 5 * 3.92;             %   Ca flux rate through open RyR (1/ms)
r_xfer = 220;                   %   Ca flux rate from SS to myoplasm (1/ms)
Cao = 2;                        %   Extracellular [Ca] (mM)
Ko = 4;                         %   Extracellular [K] (mM)
Nao = 138;                      %   Extracellular [Na] (mM)
Clo = 150;                      %   Extracellular [Cl] (mM)
Cli = 20;                       %   Intracellular [Cl] (mM)

KdIto2 = 0.1502;                %   Dissociation constant of Cl channel (Ito2) for Ca (mM)
PCl = 2.65e-15;                 %   Cl single channel permeability (cm^3/s)

GKr = 0.029;                    %   Maximal conductance of IKr (mS/microF)
GKs = 0.035;                    %   Maximal conductance of IKs (mS/microF)
GK1 = 2.73;                     %   Maximal conductance of IK1 (mS/microF)
GKp = 0.003;                    %   Maximal conductance of IKp (mS/microF)
GNa = 12.8;                     %   Maximal conductance of INa (mS/microF)
kNaCa = 0.27;                   %   Scale factor for Na/Ca exchanger (A/F)
KmNa = 87.5;                    %   Na half saturation constant for Na/Ca exchanger (mM)
KmCa = 1.38;                    %   Ca half saturation constant for Na/Ca exchanger (mM)
KmK1 = 13.0;                    %   K half saturation constant for IK1 (mM)
ksat = 0.2;                     %   Na/Ca exchange saturation factor at negative potentials
eta = 0.35;                     %   Controls voltage dependence of Na/Ca exchange
INaKmax = 1.7325;               %   Scale factor for Na/K pump (A/F)
KmNai = 20;                     %   Na half saturation constant for Na/K pump (mM)
KmKo = 1.5;                     %   K half saturation constant for Na/K pump (mM)
IpCamax = 0.03;                 %   Maximum sarcolemmal Ca pump current (A/F)
KmpCa = 0.0005;                 %   Ca half saturation constant for sarcolemmal Ca pump (mM)
GCab = 0;                       %   Maximal conductance of ICab (mS/microF)
GNab = 0.00001;                 %   Maximal conductance of INab (mS/microF)

Kfb = 0.26e-3;                  %   Forward half-saturation constant for SR Ca ATPase (mM)
Krb = 1.8;                      %   Reverse half-saturation constant for SR Ca ATPase (mM)
KSR = 1;                        %   Scale factor for SR Ca ATPase
Nfb = 0.75;                     %   Forward cooperativity constant for SR Ca ATPase
Nrb = 0.75;                     %   Reverse cooperativity constant for SR Ca ATPase
vmaxf = 2.0961e-4;              %   SR Ca ATPase forward rate parameter (mM/ms)
vmaxr = 2.0961e-4;              %   SR Ca ATPase reverse rate parameter (mM/ms)

KvScale = 1.5876;               %   Scale factor for Kv4.3 and Kv1.4 currents
Kv43Frac = 0.77;                %   Fraction of Ito1 (peak current at 40 mV) that is made up of Kv4.3 
GKv43 = Kv43Frac*KvScale*0.1;   %   Maximal conductance of IKv43 (mS/microF)
PKv14 = (1-Kv43Frac)*KvScale*4.792933e-7;       %   Maximal conductance of IKv14 (mS/microF)
alphaa0Kv43 = 0.543708;         %   IKv43 rate parameter (1/ms)
aaKv43 = 0.028983;              %   IKv43 rate parameter (1/mV)
betaa0Kv43 = 0.080185;          %   IKv43 rate parameter (1/ms)
baKv43 = 0.0468437;             %   IKv43 rate parameter (1/mV)
alphai0Kv43 = 0.0498424;        %   IKv43 rate parameter (1/ms)
aiKv43 = 0.000373016;           %   IKv43 rate parameter (1/mV)
betai0Kv43 = 0.000819482;       %   IKv43 rate parameter (1/ms)
biKv43 = 0.00000005374;         %   IKv43 rate parameter (1/mV)
alphaa0Kv14 = 6*0.31551;        %   IKv14 rate parameter (1/ms)
aaKv14 = 0.00695;               %   IKv14 rate parameter (1/mV)
betaa0Kv14 = 6*0.001966;        %   IKv14 rate parameter (1/ms)
baKv14 = 0.08527;               %   IKv14 rate parameter (1/mV)
alphai0Kv14 = 6*0.0004938;      %   IKv14 rate parameter (1/ms)
betai0Kv14 = 6*0.0000176;       %   IKv14 rate parameter (1/ms)
f1Kv43 = 1.8936;                %   IKv43 rate parameter
f2Kv43 = 14.224647456;          %   IKv43 rate parameter
f3Kv43 = 158.574378389;         %   IKv43 rate parameter
f4Kv43 = 142.936645351;         %   IKv43 rate parameter
b1Kv43 = 6.77348;               %   IKv43 rate parameter
b2Kv43 = 15.6212705152;         %   IKv43 rate parameter
b3Kv43 = 28.7532603313;         %   IKv43 rate parameter
b4Kv43 = 524.576206679;         %   IKv43 rate parameter
f1Kv14 = 0.20005;               %   IKv14 rate parameter
f2Kv14 = 0.320280;              %   IKv14 rate parameter
f3Kv14 = 13.50909223;           %   IKv14 rate parameter
f4Kv14 = 1151.7651385;          %   IKv14 rate parameter
b1Kv14 = 2.2300;                %   IKv14 rate parameter
b2Kv14 = 12.000299;             %   IKv14 rate parameter
b3Kv14 = 5.3701338025;          %   IKv14 rate parameter
b4Kv14 = 5.2396395511;          %   IKv14 rate parameter

A0_HERG = 0.017147641733086;    %   IKr rate parameter (1/ms)
B0_HERG = 0.03304608038835;     %   IKr rate parameter (1/mV)
A1_HERG = 0.03969328381141;     %   IKr rate parameter (1/ms)
B1_HERG = -0.04306054163980;    %   IKr rate parameter (1/mV)
A2_HERG = 0.02057448605977;     %   IKr rate parameter (1/ms)
B2_HERG = 0.02617412715118;     %   IKr rate parameter (1/mV)
A3_HERG = 0.00134366604423;     %   IKr rate parameter (1/ms)
B3_HERG = -0.02691385498399;    %   IKr rate parameter (1/mV)
A4_HERG = 0.10666316491288;     %   IKr rate parameter (1/ms)
B4_HERG = 0.00568908859717;     %   IKr rate parameter (1/mV)
A5_HERG = 0.00646393910049;     %   IKr rate parameter (1/ms)
B5_HERG = -0.04536642959543;    %   IKr rate parameter (1/mV)
A6_HERG = 0.00008039374403;     %   IKr rate parameter (1/ms)
B6_HERG = 0.00000069808924;     %   IKr rate parameter (1/mV)
T_Const_HERG = 5.320000001;     %   IKr rate scale factor for temperature
C2H_to_C3H = T_Const_HERG*0.02608362043337;     %   IKr transition rate (1/ms)
C3H_to_C2H = T_Const_HERG*0.14832978132145;     %   IKr transition rate (1/ms)


CSQNtot = 2.7;                  %   Total SR calsequestrin concentration (mM)
KmCSQN = 0.63;                  %   Ca half saturation constant for calsequestrin (mM)
LTRPNtot = 0.07;                %   Total troponin low-affinity site concentration (mM)
HTRPNtot = 0.14;                %   Total troponin high-affinity site concentration (mM)
khtrpn_plus = 20;               %   Troponin Ca binding rate parameter (1/[mM ms])
khtrpn_minus = 0.066e-3;        %   Troponin Ca binding rate parameter (1/ms)
kltrpn_plus = 40;               %   Troponin Ca binding rate parameter (1/[mM ms])
kltrpn_minus = 0.04;            %   Troponin Ca binding rate parameter (1/ms)
CMDNtot = 0.05;                 %   Total myoplasmic calmodulin concentration (mM)
EGTAtot = 0;                    %   Total myoplasmic EGTA concentration (mM)
KmCMDN = 2.38e-3;               %   Ca half saturation constant for calmodulin (mM)
KmEGTA = 1.5e-4;                %   Ca half saturation constant for EGTA (mM)

V = y(Nstates_CaRU+1);          %   Assign states to local variables
mNa = y(Nstates_CaRU+2);
hNa = y(Nstates_CaRU+3);
jNa = y(Nstates_CaRU+4);
Nai = y(Nstates_CaRU+5);
Ki = y(Nstates_CaRU+6);
Cai = y(Nstates_CaRU+7);
CaSR = y(Nstates_CaRU+8);
LTRPNCa = y(Nstates_CaRU+9);
HTRPNCa = y(Nstates_CaRU+10);
xKs = y(Nstates_CaRU+11);
C0Kv43 = y(Nstates_CaRU+12);
C1Kv43 = y(Nstates_CaRU+13);
C2Kv43 = y(Nstates_CaRU+14);
C3Kv43 = y(Nstates_CaRU+15);
OKv43 = y(Nstates_CaRU+16);
CI0Kv43 = y(Nstates_CaRU+17);
CI1Kv43 = y(Nstates_CaRU+18);
CI2Kv43 = y(Nstates_CaRU+19);
CI3Kv43 = y(Nstates_CaRU+20);
OIKv43 = y(Nstates_CaRU+21);
C0Kv14 = y(Nstates_CaRU+22);
C1Kv14 = y(Nstates_CaRU+23);
C2Kv14 = y(Nstates_CaRU+24);
C3Kv14 = y(Nstates_CaRU+25);
OKv14 = y(Nstates_CaRU+26);
CI0Kv14 = y(Nstates_CaRU+27);
CI1Kv14 = y(Nstates_CaRU+28);
CI2Kv14 = y(Nstates_CaRU+29);
CI3Kv14 = y(Nstates_CaRU+30);
OIKv14 = y(Nstates_CaRU+31);
C1Herg = y(Nstates_CaRU+32);
C2Herg = y(Nstates_CaRU+33);
C3Herg = y(Nstates_CaRU+34);
OHerg = y(Nstates_CaRU+35);
IHerg = y(Nstates_CaRU+36);

VF_over_RT=V/RT_over_F;
VFsq_over_RT=(1000*Faraday)*VF_over_RT;

time_on_Is1 = floor((t-Shift)/Period)*Period;       %   Establish stimulus current
time_off_Is1 = time_on_Is1+Pulse_duration;
Istim = 0;
if ((t-Shift) >= time_on_Is1 & (t-Shift) <= time_off_Is1)
    Istim = Istim + Pulse_amplitude;
end

fL = 0.85;                      %   L-type Ca channel rate parameter (1/ms)
gL = 2;                         %   L-type Ca channel rate parameter (1/ms)
bL = 32.1948;                   %   L-type Ca channel rate parameter
aL = 12.8878;                   %   L-type Ca channel rate parameter
omega = 0.0269659;              %   L-type Ca channel rate parameter (1/ms)
gammacf = 1.39;                 %   L-type Ca channel rate parameter (1/[mM ms])

alpha = 0.835399*exp(0.0269241*(V-35));     %   L-type Ca channel rate parameter (1/ms)
beta =  0.0331584*exp(-0.0934594*(V-35));   %   L-type Ca channel rate parameter (1/ms)

yCainf = 0.4/(1+exp((V+12.5)/5)) + 0.6;     %   L-type Ca channel steady-state inactivation
tauyCa = 340/(1+exp((V+30)/12)) + 60;       %   L-type Ca channel inactivation time constant (ms)
kby = yCainf/tauyCa;            %   L-type Ca channel rate parameter (1/ms)
kfy = (1 - yCainf)/tauyCa;      %   L-type Ca channel rate parameter (1/ms)

k12 = 5 * 877.5;                %   RyR channel rate parameter (1/ms)
k21 = 5 * 250;                  %   RyR channel rate parameter (1/ms)
k23 = 2.358e8;                  %   RyR channel rate parameter (1/ms)
k32 = 9.6;                      %   RyR channel rate parameter (1/ms)
k34 = 1.415e6;                  %   RyR channel rate parameter (1/ms)
k43 = 13.65;                    %   RyR channel rate parameter (1/ms)
k45 = 0.07;                     %   RyR channel rate parameter (1/ms)
k54 = 93.385;                   %   RyR channel rate parameter (1/ms)
k56 = 1.887e7;                  %   RyR channel rate parameter (1/ms)
k65 = 30;                       %   RyR channel rate parameter (1/ms)
k25 = 2.358e6;                  %   RyR channel rate parameter (1/ms)
k52 = 0.001235;                 %   RyR channel rate parameter (1/ms)

CaSS_1 = Cai;                   %   Ca concentration for each open-closed configuaration
CaSS_2 = (PCa*2*VF_over_RT/VSS*0.341*Cao/(exp(2*VF_over_RT)-1) + r_xfer*Cai) ...
          / (PCa*2*VF_over_RT/VSS/(1-exp(-2*VF_over_RT)) + r_xfer);
CaSS_3 = (JRyRmax*CaSR + r_xfer*Cai)/(JRyRmax + r_xfer);
CaSS_4 = (PCa*2*VF_over_RT/VSS*0.341*Cao/(exp(2*VF_over_RT)-1) + JRyRmax*CaSR + r_xfer*Cai) ...
          / (PCa*2*VF_over_RT/VSS/(1-exp(-2*VF_over_RT)) + JRyRmax + r_xfer);

LCC_1_to_2 = alpha;             %   Set L-type Ca channel rates (1/ms)
LCC_2_to_1 = beta;
LCC_2_to_3 = fL;
LCC_3_to_2 = gL;
LCC_1_to_4_CaSS_1 = gammacf*CaSS_1;
LCC_1_to_4_CaSS_2 = gammacf*CaSS_2;
LCC_1_to_4_CaSS_3 = gammacf*CaSS_3;
LCC_1_to_4_CaSS_4 = gammacf*CaSS_4;
LCC_4_to_1 = omega;
LCC_4_to_5 = aL*alpha;
LCC_5_to_4 = beta/bL;
LCC_2_to_5_CaSS_1 = aL*gammacf*CaSS_1;
LCC_2_to_5_CaSS_2 = aL*gammacf*CaSS_2;
LCC_2_to_5_CaSS_3 = aL*gammacf*CaSS_3;
LCC_2_to_5_CaSS_4 = aL*gammacf*CaSS_4;
LCC_5_to_2 = omega/bL;

LCC_6_to_7 = alpha;
LCC_7_to_6 = beta;
LCC_7_to_8 = fL;
LCC_8_to_7 = gL;
LCC_6_to_9_CaSS_1 = gammacf*CaSS_1;
LCC_6_to_9_CaSS_2 = gammacf*CaSS_2;
LCC_6_to_9_CaSS_3 = gammacf*CaSS_3;
LCC_6_to_9_CaSS_4 = gammacf*CaSS_4;
LCC_9_to_6 = omega;
LCC_9_to_10 = aL*alpha;
LCC_10_to_9 = beta/bL;
LCC_7_to_10_CaSS_1 = aL*gammacf*CaSS_1;
LCC_7_to_10_CaSS_2 = aL*gammacf*CaSS_2;
LCC_7_to_10_CaSS_3 = aL*gammacf*CaSS_3;
LCC_7_to_10_CaSS_4 = aL*gammacf*CaSS_4;
LCC_10_to_7 = omega/bL;

LCC_1_to_6 = kfy;
LCC_2_to_7 = kfy;
LCC_3_to_8 = kfy;
LCC_4_to_9 = kfy;
LCC_5_to_10 = kfy;
LCC_6_to_1 = kby;
LCC_7_to_2 = kby;
LCC_8_to_3 = kby;
LCC_9_to_4 = kby;
LCC_10_to_5 = kby;

CaSSmod_1 = min(CaSS_1, 0.05); % 50 microM max on RyR rates except for k12
CaSSmod_2 = min(CaSS_2, 0.05);
CaSSmod_3 = min(CaSS_3, 0.05);
CaSSmod_4 = min(CaSS_4, 0.05);

RyR_1_to_2_CaSS_1 = k12*CaSS_1^2;       %   Set RyR channel rates (1/ms)
RyR_1_to_2_CaSS_2 = k12*CaSS_2^2;
RyR_1_to_2_CaSS_3 = k12*CaSS_3^2;
RyR_1_to_2_CaSS_4 = k12*CaSS_4^2;
RyR_2_to_1 = k21;
RyR_2_to_3_CaSS_1 = k23*CaSSmod_1^2;
RyR_2_to_3_CaSS_2 = k23*CaSSmod_2^2;
RyR_2_to_3_CaSS_3 = k23*CaSSmod_3^2;
RyR_2_to_3_CaSS_4 = k23*CaSSmod_4^2;
RyR_3_to_2_CaSS_1 = k32*k43/(k34*CaSSmod_1^2 + k43);
RyR_3_to_2_CaSS_2 = k32*k43/(k34*CaSSmod_2^2 + k43);
RyR_3_to_2_CaSS_3 = k32*k43/(k34*CaSSmod_3^2 + k43);
RyR_3_to_2_CaSS_4 = k32*k43/(k34*CaSSmod_4^2 + k43);
RyR_2_to_4_CaSS_1 = k25*CaSSmod_1^2;
RyR_2_to_4_CaSS_2 = k25*CaSSmod_2^2;
RyR_2_to_4_CaSS_3 = k25*CaSSmod_3^2;
RyR_2_to_4_CaSS_4 = k25*CaSSmod_4^2;
RyR_4_to_2_CaSS_1 = k52*k65/(k56*CaSSmod_1^2 + k65);
RyR_4_to_2_CaSS_2 = k52*k65/(k56*CaSSmod_2^2 + k65);
RyR_4_to_2_CaSS_3 = k52*k65/(k56*CaSSmod_3^2 + k65);
RyR_4_to_2_CaSS_4 = k52*k65/(k56*CaSSmod_4^2 + k65);
RyR_3_to_4_CaSS_1 = k45*k34*CaSSmod_1^2/(k34*CaSSmod_1^2 + k43);
RyR_3_to_4_CaSS_2 = k45*k34*CaSSmod_2^2/(k34*CaSSmod_2^2 + k43);
RyR_3_to_4_CaSS_3 = k45*k34*CaSSmod_3^2/(k34*CaSSmod_3^2 + k43);
RyR_3_to_4_CaSS_4 = k45*k34*CaSSmod_4^2/(k34*CaSSmod_4^2 + k43);
RyR_4_to_3_CaSS_1 = k65*k54*CaSSmod_1^2/(k56*CaSSmod_1^2 + k65);
RyR_4_to_3_CaSS_2 = k65*k54*CaSSmod_2^2/(k56*CaSSmod_2^2 + k65);
RyR_4_to_3_CaSS_3 = k65*k54*CaSSmod_3^2/(k56*CaSSmod_3^2 + k65);
RyR_4_to_3_CaSS_4 = k65*k54*CaSSmod_4^2/(k56*CaSSmod_4^2 + k65);


dy = zeros(length(y),1);    %   Evaluate state derivatives for coupled LCC-RyR model

dy( 1) = -LCC_1_to_2*y(1) +LCC_2_to_1*y(2) -LCC_1_to_4_CaSS_1*y(1) +LCC_4_to_1*y(4)  ...
         -LCC_1_to_6*y(1) +LCC_6_to_1*y(6) -RyR_1_to_2_CaSS_1*y(1) +RyR_2_to_1*y(11);
dy( 2) = +LCC_1_to_2*y(1) -LCC_2_to_1*y(2) -LCC_2_to_3*y(2) +LCC_3_to_2*y(3) ...
         -LCC_2_to_5_CaSS_1*y(2) +LCC_5_to_2*y(5) -LCC_2_to_7*y(2) +LCC_7_to_2*y(7) ...
         -RyR_1_to_2_CaSS_1*y(2) +RyR_2_to_1*y(12);
dy( 3) = +LCC_2_to_3*y(2) -LCC_3_to_2*y(3) -LCC_3_to_8*y(3) +LCC_8_to_3*y(8) ...
         -RyR_1_to_2_CaSS_2*y(3) +RyR_2_to_1*y(13);
dy( 4) = +LCC_1_to_4_CaSS_1*y(1) -LCC_4_to_1*y(4) -LCC_4_to_5*y(4) +LCC_5_to_4*y(5) ...
         -LCC_4_to_9*y(4) +LCC_9_to_4*y(9) -RyR_1_to_2_CaSS_1*y(4) +RyR_2_to_1*y(14);
dy( 5) = +LCC_2_to_5_CaSS_1*y(2) -LCC_5_to_2*y(5) +LCC_4_to_5*y(4) -LCC_5_to_4*y(5) ...
         -LCC_5_to_10*y(5) +LCC_10_to_5*y(10) -RyR_1_to_2_CaSS_1*y(5) +RyR_2_to_1*y(15);
dy( 6) = +LCC_1_to_6*y(1) -LCC_6_to_1*y(6) -LCC_6_to_7*y(6) +LCC_7_to_6*y(7) ...
         -LCC_6_to_9_CaSS_1*y(6) +LCC_9_to_6*y(9) -RyR_1_to_2_CaSS_1*y(6) +RyR_2_to_1*y(16);
dy( 7) = +LCC_2_to_7*y(2) -LCC_7_to_2*y(7) +LCC_6_to_7*y(6) -LCC_7_to_6*y(7) ...
         -LCC_7_to_8*y(7) +LCC_8_to_7*y(8) -LCC_7_to_10_CaSS_1*y(7) +LCC_10_to_7*y(10) ...
         -RyR_1_to_2_CaSS_1*y(7) +RyR_2_to_1*y(17);
dy( 8) = +LCC_3_to_8*y(3) -LCC_8_to_3*y(8) +LCC_7_to_8*y(7) -LCC_8_to_7*y(8) ...
         -RyR_1_to_2_CaSS_1*y(8) +RyR_2_to_1*y(18);
dy( 9) = +LCC_4_to_9*y(4) -LCC_9_to_4*y(9) +LCC_6_to_9_CaSS_1*y(6) -LCC_9_to_6*y(9) ...
         -LCC_9_to_10*y(9) +LCC_10_to_9*y(10) -RyR_1_to_2_CaSS_1*y(9) +RyR_2_to_1*y(19);
dy(10) = +LCC_5_to_10*y(5) -LCC_10_to_5*y(10) +LCC_7_to_10_CaSS_1*y(7) -LCC_10_to_7*y(10) ...
         +LCC_9_to_10*y(9) -LCC_10_to_9*y(10) -RyR_1_to_2_CaSS_1*y(10) +RyR_2_to_1*y(20);
dy(11) = +RyR_1_to_2_CaSS_1*y(1) -RyR_2_to_1*y(11) -LCC_1_to_2*y(11) +LCC_2_to_1*y(12) ...
         -LCC_1_to_4_CaSS_1*y(11) +LCC_4_to_1*y(14) -LCC_1_to_6*y(11) +LCC_6_to_1*y(16) ...
         -RyR_2_to_3_CaSS_1*y(11) +RyR_3_to_2_CaSS_3*y(21) -RyR_2_to_4_CaSS_1*y(11) +RyR_4_to_2_CaSS_1*y(31);
dy(12) = +RyR_1_to_2_CaSS_1*y(2) -RyR_2_to_1*y(12) +LCC_1_to_2*y(11) -LCC_2_to_1*y(12) ...
         -LCC_2_to_3*y(12) +LCC_3_to_2*y(13) -LCC_2_to_5_CaSS_1*y(12) +LCC_5_to_2*y(15) ...
         -LCC_2_to_7*y(12) +LCC_7_to_2*y(17) -RyR_2_to_3_CaSS_1*y(12) +RyR_3_to_2_CaSS_3*y(22) ...
         -RyR_2_to_4_CaSS_1*y(12) +RyR_4_to_2_CaSS_1*y(32);
dy(13) = +RyR_1_to_2_CaSS_2*y(3) -RyR_2_to_1*y(13) +LCC_2_to_3*y(12) -LCC_3_to_2*y(13) ...
         -LCC_3_to_8*y(13) +LCC_8_to_3*y(18) -RyR_2_to_3_CaSS_2*y(13) +RyR_3_to_2_CaSS_4*y(23) ...
         -RyR_2_to_4_CaSS_2*y(13) +RyR_4_to_2_CaSS_2*y(33);
dy(14) = +RyR_1_to_2_CaSS_1*y(4) -RyR_2_to_1*y(14) +LCC_1_to_4_CaSS_1*y(11) -LCC_4_to_1*y(14) ...
         -LCC_4_to_5*y(14) +LCC_5_to_4*y(15) -LCC_4_to_9*y(14) +LCC_9_to_4*y(19) ...
         -RyR_2_to_3_CaSS_1*y(14) +RyR_3_to_2_CaSS_3*y(24) -RyR_2_to_4_CaSS_1*y(14) +RyR_4_to_2_CaSS_1*y(34);
dy(15) = +RyR_1_to_2_CaSS_1*y(5) -RyR_2_to_1*y(15) +LCC_2_to_5_CaSS_1*y(12) -LCC_5_to_2*y(15) ...
         +LCC_4_to_5*y(14) -LCC_5_to_4*y(15) -LCC_5_to_10*y(15) +LCC_10_to_5*y(20) ...
         -RyR_2_to_3_CaSS_1*y(15) +RyR_3_to_2_CaSS_3*y(25) -RyR_2_to_4_CaSS_1*y(15) +RyR_4_to_2_CaSS_1*y(35);
dy(16) = +RyR_1_to_2_CaSS_1*y(6) -RyR_2_to_1*y(16) +LCC_1_to_6*y(11) -LCC_6_to_1*y(16) ...
         -LCC_6_to_7*y(16) +LCC_7_to_6*y(17) -LCC_6_to_9_CaSS_1*y(16) +LCC_9_to_6*y(19) ...
         -RyR_2_to_3_CaSS_1*y(16) +RyR_3_to_2_CaSS_3*y(26) -RyR_2_to_4_CaSS_1*y(16) +RyR_4_to_2_CaSS_1*y(36);
dy(17) = +RyR_1_to_2_CaSS_1*y(7) -RyR_2_to_1*y(17) +LCC_2_to_7*y(12) -LCC_7_to_2*y(17) ...
         +LCC_6_to_7*y(16) -LCC_7_to_6*y(17) -LCC_7_to_8*y(17) +LCC_8_to_7*y(18) ...
         -LCC_7_to_10_CaSS_1*y(17) +LCC_10_to_7*y(20) -RyR_2_to_3_CaSS_1*y(17) +RyR_3_to_2_CaSS_3*y(27) ...
         -RyR_2_to_4_CaSS_1*y(17) +RyR_4_to_2_CaSS_1*y(37);
dy(18) = +RyR_1_to_2_CaSS_1*y(8) -RyR_2_to_1*y(18) +LCC_3_to_8*y(13) -LCC_8_to_3*y(18) ...
         +LCC_7_to_8*y(17) -LCC_8_to_7*y(18) -RyR_2_to_3_CaSS_1*y(18) +RyR_3_to_2_CaSS_3*y(28) ...
         -RyR_2_to_4_CaSS_1*y(18) +RyR_4_to_2_CaSS_1*y(38);
dy(19) = +RyR_1_to_2_CaSS_1*y(9) -RyR_2_to_1*y(19) +LCC_4_to_9*y(14) -LCC_9_to_4*y(19) ...
         +LCC_6_to_9_CaSS_1*y(16) -LCC_9_to_6*y(19) -LCC_9_to_10*y(19) +LCC_10_to_9*y(20) ...
         -RyR_2_to_3_CaSS_1*y(19) +RyR_3_to_2_CaSS_3*y(29) -RyR_2_to_4_CaSS_1*y(19) +RyR_4_to_2_CaSS_1*y(39);
dy(20) = +RyR_1_to_2_CaSS_1*y(10) -RyR_2_to_1*y(20) +LCC_5_to_10*y(15) -LCC_10_to_5*y(20) ...
         +LCC_7_to_10_CaSS_1*y(17) -LCC_10_to_7*y(20) +LCC_9_to_10*y(19) -LCC_10_to_9*y(20) ...
         -RyR_2_to_3_CaSS_1*y(20) +RyR_3_to_2_CaSS_3*y(30) -RyR_2_to_4_CaSS_1*y(20) +RyR_4_to_2_CaSS_1*y(40);
dy(21) = +RyR_2_to_3_CaSS_1*y(11) -RyR_3_to_2_CaSS_3*y(21) -LCC_1_to_2*y(21) +LCC_2_to_1*y(22) ...
         -LCC_1_to_4_CaSS_3*y(21) +LCC_4_to_1*y(24) -LCC_1_to_6*y(21) +LCC_6_to_1*y(26) ...
         -RyR_3_to_4_CaSS_3*y(21) +RyR_4_to_3_CaSS_1*y(31);
dy(22) = +RyR_2_to_3_CaSS_1*y(12) -RyR_3_to_2_CaSS_3*y(22) +LCC_1_to_2*y(21) -LCC_2_to_1*y(22) ...
         -LCC_2_to_3*y(22) +LCC_3_to_2*y(23) -LCC_2_to_5_CaSS_3*y(22) +LCC_5_to_2*y(25) ...
         -LCC_2_to_7*y(22) +LCC_7_to_2*y(27) -RyR_3_to_4_CaSS_3*y(22) +RyR_4_to_3_CaSS_1*y(32);
dy(23) = +RyR_2_to_3_CaSS_2*y(13) -RyR_3_to_2_CaSS_4*y(23) +LCC_2_to_3*y(22) -LCC_3_to_2*y(23) ...
         -LCC_3_to_8*y(23) +LCC_8_to_3*y(28) -RyR_3_to_4_CaSS_4*y(23) +RyR_4_to_3_CaSS_2*y(33);
dy(24) = +RyR_2_to_3_CaSS_1*y(14) -RyR_3_to_2_CaSS_3*y(24) +LCC_1_to_4_CaSS_3*y(21) -LCC_4_to_1*y(24) ...
         -LCC_4_to_5*y(24) +LCC_5_to_4*y(25) -LCC_4_to_9*y(24) +LCC_9_to_4*y(29) ...
         -RyR_3_to_4_CaSS_3*y(24) +RyR_4_to_3_CaSS_1*y(34);
dy(25) = +RyR_2_to_3_CaSS_1*y(15) -RyR_3_to_2_CaSS_3*y(25) +LCC_2_to_5_CaSS_3*y(22) -LCC_5_to_2*y(25) ...
         +LCC_4_to_5*y(24) -LCC_5_to_4*y(25) -LCC_5_to_10*y(25) +LCC_10_to_5*y(30) ...
         -RyR_3_to_4_CaSS_3*y(25) +RyR_4_to_3_CaSS_1*y(35);
dy(26) = +RyR_2_to_3_CaSS_1*y(16) -RyR_3_to_2_CaSS_3*y(26) +LCC_1_to_6*y(21) -LCC_6_to_1*y(26) ...
         -LCC_6_to_7*y(26) +LCC_7_to_6*y(27) -LCC_6_to_9_CaSS_3*y(26) +LCC_9_to_6*y(29) ...
         -RyR_3_to_4_CaSS_3*y(26) +RyR_4_to_3_CaSS_1*y(36);
dy(27) = +RyR_2_to_3_CaSS_1*y(17) -RyR_3_to_2_CaSS_3*y(27) +LCC_2_to_7*y(22) -LCC_7_to_2*y(27) ...
         +LCC_6_to_7*y(26) -LCC_7_to_6*y(27) -LCC_7_to_8*y(27) +LCC_8_to_7*y(28) ...
         -LCC_7_to_10_CaSS_3*y(27) +LCC_10_to_7*y(30) -RyR_3_to_4_CaSS_3*y(27) +RyR_4_to_3_CaSS_1*y(37);
dy(28) = +RyR_2_to_3_CaSS_1*y(18) -RyR_3_to_2_CaSS_3*y(28) +LCC_3_to_8*y(23) -LCC_8_to_3*y(28) ...
         +LCC_7_to_8*y(27) -LCC_8_to_7*y(28) -RyR_3_to_4_CaSS_3*y(28) +RyR_4_to_3_CaSS_1*y(38);
dy(29) = +RyR_2_to_3_CaSS_1*y(19) -RyR_3_to_2_CaSS_3*y(29) +LCC_4_to_9*y(24) -LCC_9_to_4*y(29) ...
         +LCC_6_to_9_CaSS_3*y(26) -LCC_9_to_6*y(29) -LCC_9_to_10*y(29) +LCC_10_to_9*y(30) ...
         -RyR_3_to_4_CaSS_3*y(29) +RyR_4_to_3_CaSS_1*y(39);
dy(30) = +RyR_2_to_3_CaSS_1*y(20) -RyR_3_to_2_CaSS_3*y(30) +LCC_5_to_10*y(25) -LCC_10_to_5*y(30) ...
         +LCC_7_to_10_CaSS_3*y(27) -LCC_10_to_7*y(30) +LCC_9_to_10*y(29) -LCC_10_to_9*y(30) ...
         -RyR_3_to_4_CaSS_3*y(30) +RyR_4_to_3_CaSS_1*y(40);
dy(31) = +RyR_2_to_4_CaSS_1*y(11) -RyR_4_to_2_CaSS_1*y(31) +RyR_3_to_4_CaSS_3*y(21) -RyR_4_to_3_CaSS_1*y(31) ...
         -LCC_1_to_2*y(31) +LCC_2_to_1*y(32) -LCC_1_to_4_CaSS_1*y(31) +LCC_4_to_1*y(34) ...
         -LCC_1_to_6*y(31) +LCC_6_to_1*y(36);
dy(32) = +RyR_2_to_4_CaSS_1*y(12) -RyR_4_to_2_CaSS_1*y(32) +RyR_3_to_4_CaSS_3*y(22) -RyR_4_to_3_CaSS_1*y(32) ...
         +LCC_1_to_2*y(31) -LCC_2_to_1*y(32) -LCC_2_to_3*y(32) +LCC_3_to_2*y(33) ...
         -LCC_2_to_5_CaSS_1*y(32) +LCC_5_to_2*y(35) -LCC_2_to_7*y(32) +LCC_7_to_2*y(37);
dy(33) = +RyR_2_to_4_CaSS_2*y(13) -RyR_4_to_2_CaSS_2*y(33) +RyR_3_to_4_CaSS_4*y(23) -RyR_4_to_3_CaSS_2*y(33) ...
         +LCC_2_to_3*y(32) -LCC_3_to_2*y(33) -LCC_3_to_8*y(33) +LCC_8_to_3*y(38);
dy(34) = +RyR_2_to_4_CaSS_1*y(14) -RyR_4_to_2_CaSS_1*y(34) +RyR_3_to_4_CaSS_3*y(24) -RyR_4_to_3_CaSS_1*y(34) ...
         +LCC_1_to_4_CaSS_1*y(31) -LCC_4_to_1*y(34) -LCC_4_to_5*y(34) +LCC_5_to_4*y(35) ...
         -LCC_4_to_9*y(34) +LCC_9_to_4*y(39);
dy(35) = +RyR_2_to_4_CaSS_1*y(15) -RyR_4_to_2_CaSS_1*y(35) +RyR_3_to_4_CaSS_3*y(25) -RyR_4_to_3_CaSS_1*y(35) ...
         +LCC_2_to_5_CaSS_1*y(32) -LCC_5_to_2*y(35) +LCC_4_to_5*y(34) -LCC_5_to_4*y(35) ...
         -LCC_5_to_10*y(35) +LCC_10_to_5*y(40);
dy(36) = +RyR_2_to_4_CaSS_1*y(16) -RyR_4_to_2_CaSS_1*y(36) +RyR_3_to_4_CaSS_3*y(26) -RyR_4_to_3_CaSS_1*y(36) ...
         +LCC_1_to_6*y(31) -LCC_6_to_1*y(36) -LCC_6_to_7*y(36) +LCC_7_to_6*y(37) ...
         -LCC_6_to_9_CaSS_1*y(36) +LCC_9_to_6*y(39);
dy(37) = +RyR_2_to_4_CaSS_1*y(17) -RyR_4_to_2_CaSS_1*y(37) +RyR_3_to_4_CaSS_3*y(27) -RyR_4_to_3_CaSS_1*y(37) ...
         +LCC_2_to_7*y(32) -LCC_7_to_2*y(37) +LCC_6_to_7*y(36) -LCC_7_to_6*y(37) ...
         -LCC_7_to_8*y(37) +LCC_8_to_7*y(38) -LCC_7_to_10_CaSS_1*y(37) +LCC_10_to_7*y(40);
dy(38) = +RyR_2_to_4_CaSS_1*y(18) -RyR_4_to_2_CaSS_1*y(38) +RyR_3_to_4_CaSS_3*y(28) -RyR_4_to_3_CaSS_1*y(38) ...
         +LCC_3_to_8*y(33) -LCC_8_to_3*y(38) +LCC_7_to_8*y(37) -LCC_8_to_7*y(38);
dy(39) = +RyR_2_to_4_CaSS_1*y(19) -RyR_4_to_2_CaSS_1*y(39) +RyR_3_to_4_CaSS_3*y(29) -RyR_4_to_3_CaSS_1*y(39) ...
         +LCC_4_to_9*y(34) -LCC_9_to_4*y(39) +LCC_6_to_9_CaSS_1*y(36) -LCC_9_to_6*y(39) ...
         -LCC_9_to_10*y(39) +LCC_10_to_9*y(40);
dy(40) = +RyR_2_to_4_CaSS_1*y(20) -RyR_4_to_2_CaSS_1*y(40) +RyR_3_to_4_CaSS_3*y(30) -RyR_4_to_3_CaSS_1*y(40) ...
         +LCC_5_to_10*y(35) -LCC_10_to_5*y(40) +LCC_7_to_10_CaSS_1*y(37) -LCC_10_to_7*y(40) ...
         +LCC_9_to_10*y(39) -LCC_10_to_9*y(40);


PCaSS_2 = y(3)+y(13)+y(33);         %   Evaluate occupancy probability for each open-closed configuartion
PCaSS_3 = y(21)+y(22)+y(24)+y(25)+y(26)+y(27)+y(28)+y(29)+y(30);
PCaSS_4 = y(23);
PCaSS_1 = 1 - PCaSS_2 - PCaSS_3 - PCaSS_4;

                                    %   Evaluate fluxes/currents across LCC-RyR boundary
ICaL = NCaRU/Csa*PCa*4*VFsq_over_RT/(exp(2*VF_over_RT)-1) ...
    * ( PCaSS_2*(CaSS_2*exp(2*VF_over_RT)-0.341*Cao) + PCaSS_4*(CaSS_4*exp(2*VF_over_RT)-0.341*Cao) );
JCaL = -ICaL*Csa/(2*Faraday*1000*VSS);
Ito2 = NCaRU/Csa*PCl*VFsq_over_RT*(Cli*exp(-VF_over_RT)-Clo)/(exp(-VF_over_RT) - 1) ...
        * ( PCaSS_4/(1 + KdIto2/CaSS_4) + PCaSS_3/(1 + KdIto2/CaSS_3) ...
            + PCaSS_2/(1 + KdIto2/CaSS_2) + PCaSS_1/(1 + KdIto2/CaSS_1) );
JRyR = NCaRU*JRyRmax * ( PCaSS_4*(CaSR - CaSS_4) + PCaSS_3*(CaSR - CaSS_3) );
Jss2b = NCaRU*r_xfer * ( PCaSS_4*(CaSS_4 - Cai) + PCaSS_3*(CaSS_3 - Cai) ...
                            + PCaSS_2*(CaSS_2 - Cai) + PCaSS_1*(CaSS_1 - Cai) );
CaSSavg = PCaSS_1*CaSS_1 + PCaSS_2*CaSS_2 + PCaSS_3*CaSS_3 + PCaSS_4*CaSS_4;        % Average dyadic Ca concentration


                                    %   Evaluate currents and state derivatives of global model 
ENa = RT_over_F*log(Nao/Nai);
EK =  RT_over_F*log(Ko/Ki);
EKs = RT_over_F*log((Ko+0.01833*Nao)/(Ki+0.01833*Nai));
ECa = 0.5*RT_over_F*log(Cao/max(1e-10,Cai));

INa = GNa*(mNa^3)*hNa*jNa*(V-ENa);

fKo = (Ko/4)^0.5;
IKr = GKr*fKo*OHerg*(V-EK);

IKs = GKs*(xKs^2)*(V-EKs);

IKv43 = GKv43*OKv43*(V-EK);

a1 =  Ki*exp(VF_over_RT)-Ko;
a2 =  exp(VF_over_RT)-1;
IKv14_K = PKv14*OKv14*VFsq_over_RT*(a1/a2);
a1 =  Nai*exp(VF_over_RT)-Nao;
IKv14_Na = 0.02*PKv14*OKv14*VFsq_over_RT*(a1/a2);
IKv14 = IKv14_K + IKv14_Na;
Ito1 = IKv43 + IKv14;

K1_inf = 1/(0.94+exp(1.76/RT_over_F*(V-EK)));
IK1 = GK1*Ko/(Ko+KmK1)*K1_inf*(V-EK);

INab = GNab*(V-ENa);

KpV = 1/(1+exp((7.488-V)/5.98));
IKp = GKp*KpV*(V-EK);				
      
sigma = (exp(Nao/67.3)-1)/7;
a1 = 1+0.1245*exp(-0.1*VF_over_RT);
a2 = 0.0365*sigma*exp(-VF_over_RT);
fNaK = 1/(a1+a2);
a1 = Ko/(Ko+KmKo);
a2 = 1+(KmNai/Nai)^2;
INaK = INaKmax*fNaK*(a1/a2);

a1 = exp(eta*VF_over_RT)*Nai^3*Cao;
a2 = exp((eta-1)*VF_over_RT)*Nao^3*Cai;
a3 = 1+ksat*exp((eta-1)*VF_over_RT);
a4 = KmCa+Cao;
a5 = 5000/(KmNa^3+Nao^3);
INaCa = kNaCa*a5*(a1-a2)/(a4*a3);

ICab = GCab*(V-ECa);

IpCa = IpCamax*Cai/(KmpCa+Cai);

if (abs(V+47.13) <= 1e-3)
    alpha_m = 0.32/(0.1 - 0.005*(V+47.13)); 
else
	alpha_m = 0.32*(V+47.13)/(1-exp(-0.1*(V+47.13)));
end
beta_m = 0.08*exp(-V/11);
if(V < -40)
    alpha_h = 0.135*exp((80+V)/-6.8);
	beta_h = 3.56*exp(0.079*V)+310000*exp(0.35*V);
	a1 = -127140*exp(0.2444*V);
	a2 = 3.474e-5*exp(-0.04391*V);
	a3 = 1+exp(0.311*(V+79.23));
	alpha_j = (a1-a2)*(V+37.78)/a3;
	a2 = 1+exp(-0.1378*(V+40.14));
	beta_j = 0.1212*exp(-0.01052*V)/a2;
else
	alpha_h = 0;
	beta_h = 1/(0.13*(1+exp((V+10.66)/-11.1)));
	alpha_j = 0;
	a1 = 1+exp(-0.1*(V+32));
	beta_j = 0.3*exp(-2.535e-7*V)/a1;
end
dmNa = alpha_m*(1-mNa)-beta_m*mNa;	
dhNa = alpha_h*(1-hNa)-beta_h*hNa;	
djNa = alpha_j*(1-jNa)-beta_j*jNa;

C1H_to_C2H = T_Const_HERG*A0_HERG*exp(B0_HERG*V);
C2H_to_C1H = T_Const_HERG*A1_HERG*exp(B1_HERG*V);
C3H_to_OH =  T_Const_HERG*A2_HERG*exp(B2_HERG*V);
OH_to_C3H =  T_Const_HERG*A3_HERG*exp(B3_HERG*V);
OH_to_IH =   T_Const_HERG*A4_HERG*exp(B4_HERG*V);
IH_to_OH =   T_Const_HERG*A5_HERG*exp(B5_HERG*V);
C3H_to_IH =  T_Const_HERG*A6_HERG*exp(B6_HERG*V);
IH_to_C3H =  (OH_to_C3H*IH_to_OH*C3H_to_IH)/(C3H_to_OH*OH_to_IH);
dC1Herg = C2H_to_C1H * C2Herg - C1H_to_C2H * C1Herg;
a1 = C1H_to_C2H * C1Herg + C3H_to_C2H * C3Herg;
a2 = (C2H_to_C1H + C2H_to_C3H) * C2Herg;
dC2Herg = a1-a2;
a1 = C2H_to_C3H*C2Herg + OH_to_C3H*OHerg + IH_to_C3H*IHerg;
a2 = (C3H_to_IH + C3H_to_OH + C3H_to_C2H) * C3Herg;
dC3Herg = a1-a2;
a1 = C3H_to_OH * C3Herg + IH_to_OH * IHerg;
a2 = (OH_to_C3H + OH_to_IH) * OHerg;
dOHerg = a1-a2;
a1 = C3H_to_IH * C3Herg + OH_to_IH * OHerg;
a2 = (IH_to_C3H + IH_to_OH) * IHerg;
dIHerg = a1-a2;

xKs_inf = 1/(1+exp(-(V-24.7)/13.6));
a1 = 7.19e-5*(V-10)/(1-exp(-0.148*(V-10)));
a2 = 1.31e-4*(V-10)/(exp(0.0687*(V-10))-1);
tau_xKs = 1/(a1+a2);
dxKs = (xKs_inf-xKs)/tau_xKs;
	
fb = (Cai/Kfb)^Nfb;
rb = (CaSR/Krb)^Nrb;
Jup = KSR*(vmaxf*fb - vmaxr*rb)/(1 + fb + rb);
 
dLTRPNCa = kltrpn_plus*Cai*(1 - LTRPNCa) - kltrpn_minus * LTRPNCa;
dHTRPNCa = khtrpn_plus*Cai*(1 - HTRPNCa) - khtrpn_minus * HTRPNCa;
Jtrpn = LTRPNtot*dLTRPNCa+HTRPNtot*dHTRPNCa;

beta_i = 1/(1+CMDNtot*KmCMDN/(Cai+KmCMDN)^2+EGTAtot*KmEGTA/(Cai+KmEGTA)^2);
beta_SR = 1/(1+CSQNtot*KmCSQN/(CaSR+KmCSQN)^2);

a1 = Csa/(Vmyo*Faraday*1000);
dNai = -( INa+INab+3*(INaCa+INaK)+ IKv14_Na)*a1;

a3 = IKr+IKs+IK1+IKp;
dKi = -( a3-2*INaK+Ito1 )*a1;

a3 = ICab-2*INaCa+IpCa;
dCai = beta_i*(Jss2b*VSS/Vmyo-Jup-Jtrpn - a3*0.5*a1);

dCaSR = beta_SR*(Jup*Vmyo/VSR - JRyR*VSS/VSR);

a1 = INa+ICaL+IKr+IKs;
a2 = IK1+IKp+INaCa+INaK+Ito1+Ito2;
a3 = IpCa+ICab+INab;
Itot = a1+a2+a3+Istim;
dV = -Itot;

alpha_act43 = alphaa0Kv43*exp(aaKv43*V);
beta_act43  = betaa0Kv43*exp(-baKv43*V);
alpha_inact43 = alphai0Kv43*exp(-aiKv43*V);
beta_inact43  = betai0Kv43*exp(biKv43*V);

C0Kv43_to_C1Kv43 = 4*alpha_act43;
C1Kv43_to_C2Kv43 = 3*alpha_act43;
C2Kv43_to_C3Kv43 = 2*alpha_act43;
C3Kv43_to_OKv43  =   alpha_act43;

CI0Kv43_to_CI1Kv43 = 4*b1Kv43*alpha_act43;
CI1Kv43_to_CI2Kv43 = 3*b2Kv43*alpha_act43/b1Kv43;
CI2Kv43_to_CI3Kv43 = 2*b3Kv43*alpha_act43/b2Kv43;
CI3Kv43_to_OIKv43  =   b4Kv43*alpha_act43/b3Kv43;

C1Kv43_to_C0Kv43 =   beta_act43;
C2Kv43_to_C1Kv43 = 2*beta_act43;
C3Kv43_to_C2Kv43 = 3*beta_act43;
OKv43_to_C3Kv43  = 4*beta_act43;

CI1Kv43_to_CI0Kv43 =          beta_act43/f1Kv43;
CI2Kv43_to_CI1Kv43 = 2*f1Kv43*beta_act43/f2Kv43;
CI3Kv43_to_CI2Kv43 = 3*f2Kv43*beta_act43/f3Kv43;
OIKv43_to_CI3Kv43  = 4*f3Kv43*beta_act43/f4Kv43;

C0Kv43_to_CI0Kv43 = beta_inact43;
C1Kv43_to_CI1Kv43 = f1Kv43*beta_inact43;
C2Kv43_to_CI2Kv43 = f2Kv43*beta_inact43;
C3Kv43_to_CI3Kv43 = f3Kv43*beta_inact43;
OKv43_to_OIKv43   = f4Kv43*beta_inact43;

CI0Kv43_to_C0Kv43 = alpha_inact43;
CI1Kv43_to_C1Kv43 = alpha_inact43/b1Kv43;
CI2Kv43_to_C2Kv43 = alpha_inact43/b2Kv43;
CI3Kv43_to_C3Kv43 = alpha_inact43/b3Kv43;
OIKv43_to_OKv43   = alpha_inact43/b4Kv43;

a1 = (C0Kv43_to_C1Kv43+C0Kv43_to_CI0Kv43)*C0Kv43;
a2 = C1Kv43_to_C0Kv43*C1Kv43 + CI0Kv43_to_C0Kv43*CI0Kv43;
dC0Kv43 = a2 - a1;

a1 = (C1Kv43_to_C2Kv43+C1Kv43_to_C0Kv43+C1Kv43_to_CI1Kv43)*C1Kv43;
a2 = C2Kv43_to_C1Kv43*C2Kv43 + CI1Kv43_to_C1Kv43*CI1Kv43 + C0Kv43_to_C1Kv43*C0Kv43;
dC1Kv43 = a2 - a1;

a1 = (C2Kv43_to_C3Kv43+C2Kv43_to_C1Kv43+C2Kv43_to_CI2Kv43)*C2Kv43;
a2 = C3Kv43_to_C2Kv43*C3Kv43 + CI2Kv43_to_C2Kv43*CI2Kv43 + C1Kv43_to_C2Kv43*C1Kv43;
dC2Kv43 = a2 - a1;

a1 = (C3Kv43_to_OKv43+C3Kv43_to_C2Kv43+C3Kv43_to_CI3Kv43)*C3Kv43;
a2 = OKv43_to_C3Kv43*OKv43 + CI3Kv43_to_C3Kv43*CI3Kv43 + C2Kv43_to_C3Kv43*C2Kv43;
dC3Kv43 = a2 - a1;

a1 = (OKv43_to_C3Kv43+OKv43_to_OIKv43)*OKv43;
a2 = C3Kv43_to_OKv43*C3Kv43 + OIKv43_to_OKv43*OIKv43;
dOKv43 = a2 - a1;

a1 = (CI0Kv43_to_C0Kv43+CI0Kv43_to_CI1Kv43)*CI0Kv43;
a2 = C0Kv43_to_CI0Kv43*C0Kv43 + CI1Kv43_to_CI0Kv43*CI1Kv43;
dCI0Kv43 = a2 - a1;

a1 = (CI1Kv43_to_CI2Kv43+CI1Kv43_to_C1Kv43+CI1Kv43_to_CI0Kv43)*CI1Kv43;
a2 = CI2Kv43_to_CI1Kv43*CI2Kv43 + C1Kv43_to_CI1Kv43*C1Kv43 + CI0Kv43_to_CI1Kv43*CI0Kv43;
dCI1Kv43 = a2 - a1;

a1 = (CI2Kv43_to_CI3Kv43+CI2Kv43_to_C2Kv43+CI2Kv43_to_CI1Kv43)*CI2Kv43;
a2 = CI3Kv43_to_CI2Kv43*CI3Kv43 + C2Kv43_to_CI2Kv43*C2Kv43 + CI1Kv43_to_CI2Kv43*CI1Kv43;
dCI2Kv43 = a2 - a1;

a1 = (CI3Kv43_to_OIKv43+CI3Kv43_to_C3Kv43+CI3Kv43_to_CI2Kv43)*CI3Kv43;
a2 = OIKv43_to_CI3Kv43*OIKv43 + C3Kv43_to_CI3Kv43*C3Kv43 + CI2Kv43_to_CI3Kv43*CI2Kv43;
dCI3Kv43 = a2 - a1;

a1 = (OIKv43_to_OKv43+OIKv43_to_CI3Kv43)*OIKv43;
a2 = OKv43_to_OIKv43*OKv43 + CI3Kv43_to_OIKv43*CI3Kv43;
dOIKv43 = a2 - a1;


alpha_act14 = alphaa0Kv14*exp(aaKv14*V);
beta_act14  = betaa0Kv14*exp(-baKv14*V);
alpha_inact14 = alphai0Kv14;
beta_inact14  = betai0Kv14;

C0Kv14_to_C1Kv14 = 4*alpha_act14;
C1Kv14_to_C2Kv14 = 3*alpha_act14;
C2Kv14_to_C3Kv14 = 2*alpha_act14;
C3Kv14_to_OKv14  =   alpha_act14;

CI0Kv14_to_CI1Kv14 = 4*b1Kv14*alpha_act14;
CI1Kv14_to_CI2Kv14 = 3*b2Kv14*alpha_act14/b1Kv14;
CI2Kv14_to_CI3Kv14 = 2*b3Kv14*alpha_act14/b2Kv14;
CI3Kv14_to_OIKv14  =   b4Kv14*alpha_act14/b3Kv14;

C1Kv14_to_C0Kv14 =   beta_act14;
C2Kv14_to_C1Kv14 = 2*beta_act14;
C3Kv14_to_C2Kv14 = 3*beta_act14;
OKv14_to_C3Kv14  = 4*beta_act14;

CI1Kv14_to_CI0Kv14 =          beta_act14/f1Kv14;
CI2Kv14_to_CI1Kv14 = 2*f1Kv14*beta_act14/f2Kv14;
CI3Kv14_to_CI2Kv14 = 3*f2Kv14*beta_act14/f3Kv14;
OIKv14_to_CI3Kv14  = 4*f3Kv14*beta_act14/f4Kv14;

C0Kv14_to_CI0Kv14 = beta_inact14;
C1Kv14_to_CI1Kv14 = f1Kv14*beta_inact14;
C2Kv14_to_CI2Kv14 = f2Kv14*beta_inact14;
C3Kv14_to_CI3Kv14 = f3Kv14*beta_inact14;
OKv14_to_OIKv14   = f4Kv14*beta_inact14;

CI0Kv14_to_C0Kv14 = alpha_inact14;
CI1Kv14_to_C1Kv14 = alpha_inact14/b1Kv14;
CI2Kv14_to_C2Kv14 = alpha_inact14/b2Kv14;
CI3Kv14_to_C3Kv14 = alpha_inact14/b3Kv14;
OIKv14_to_OKv14   = alpha_inact14/b4Kv14;

a1 = (C0Kv14_to_C1Kv14+C0Kv14_to_CI0Kv14)*C0Kv14;
a2 = C1Kv14_to_C0Kv14*C1Kv14 + CI0Kv14_to_C0Kv14*CI0Kv14;
dC0Kv14 = a2 - a1;

a1 = (C1Kv14_to_C2Kv14+C1Kv14_to_C0Kv14+C1Kv14_to_CI1Kv14)*C1Kv14;
a2 = C2Kv14_to_C1Kv14*C2Kv14 + CI1Kv14_to_C1Kv14*CI1Kv14 + C0Kv14_to_C1Kv14*C0Kv14;
dC1Kv14 = a2 - a1;

a1 = (C2Kv14_to_C3Kv14+C2Kv14_to_C1Kv14+C2Kv14_to_CI2Kv14)*C2Kv14;
a2 = C3Kv14_to_C2Kv14*C3Kv14 + CI2Kv14_to_C2Kv14*CI2Kv14 + C1Kv14_to_C2Kv14*C1Kv14;
dC2Kv14 = a2 - a1;

a1 = (C3Kv14_to_OKv14+C3Kv14_to_C2Kv14+C3Kv14_to_CI3Kv14)*C3Kv14;
a2 = OKv14_to_C3Kv14*OKv14 + CI3Kv14_to_C3Kv14*CI3Kv14 + C2Kv14_to_C3Kv14*C2Kv14;
dC3Kv14 = a2 - a1;

a1 = (OKv14_to_C3Kv14+OKv14_to_OIKv14)*OKv14;
a2 = C3Kv14_to_OKv14*C3Kv14 + OIKv14_to_OKv14*OIKv14;
dOKv14 = a2 - a1;

a1 = (CI0Kv14_to_C0Kv14+CI0Kv14_to_CI1Kv14)*CI0Kv14;
a2 = C0Kv14_to_CI0Kv14*C0Kv14 + CI1Kv14_to_CI0Kv14*CI1Kv14;
dCI0Kv14 = a2 - a1;

a1 = (CI1Kv14_to_CI2Kv14+CI1Kv14_to_C1Kv14+CI1Kv14_to_CI0Kv14)*CI1Kv14;
a2 = CI2Kv14_to_CI1Kv14*CI2Kv14 + C1Kv14_to_CI1Kv14*C1Kv14 + CI0Kv14_to_CI1Kv14*CI0Kv14;
dCI1Kv14 = a2 - a1;

a1 = (CI2Kv14_to_CI3Kv14+CI2Kv14_to_C2Kv14+CI2Kv14_to_CI1Kv14)*CI2Kv14;
a2 = CI3Kv14_to_CI2Kv14*CI3Kv14 + C2Kv14_to_CI2Kv14*C2Kv14 + CI1Kv14_to_CI2Kv14*CI1Kv14;
dCI2Kv14 = a2 - a1;

a1 = (CI3Kv14_to_OIKv14+CI3Kv14_to_C3Kv14+CI3Kv14_to_CI2Kv14)*CI3Kv14;
a2 = OIKv14_to_CI3Kv14*OIKv14 + C3Kv14_to_CI3Kv14*C3Kv14 + CI2Kv14_to_CI3Kv14*CI2Kv14;
dCI3Kv14 = a2 - a1;

a1 = (OIKv14_to_OKv14+OIKv14_to_CI3Kv14)*OIKv14;
a2 = OKv14_to_OIKv14*OKv14 + CI3Kv14_to_OIKv14*CI3Kv14;
dOIKv14 = a2 - a1;

                                    %   Assign global state derviative to derivative array
if Vclamp_flag
    dy(Nstates_CaRU+1) = 0;
else    
    dy(Nstates_CaRU+1) = dV;
end
dy(Nstates_CaRU+2) = dmNa;
dy(Nstates_CaRU+3) = dhNa;
dy(Nstates_CaRU+4) = djNa;
dy(Nstates_CaRU+5) = dNai;
dy(Nstates_CaRU+6) = dKi;
dy(Nstates_CaRU+7) = dCai;
dy(Nstates_CaRU+8) = dCaSR;
dy(Nstates_CaRU+9) = dLTRPNCa;
dy(Nstates_CaRU+10) = dHTRPNCa;
dy(Nstates_CaRU+11) = dxKs;
dy(Nstates_CaRU+12) = dC0Kv43;
dy(Nstates_CaRU+13) = dC1Kv43;
dy(Nstates_CaRU+14) = dC2Kv43;
dy(Nstates_CaRU+15) = dC3Kv43;
dy(Nstates_CaRU+16) = dOKv43;
dy(Nstates_CaRU+17) = dCI0Kv43;
dy(Nstates_CaRU+18) = dCI1Kv43;
dy(Nstates_CaRU+19) = dCI2Kv43;
dy(Nstates_CaRU+20) = dCI3Kv43;
dy(Nstates_CaRU+21) = dOIKv43;
dy(Nstates_CaRU+22) = dC0Kv14;
dy(Nstates_CaRU+23) = dC1Kv14;
dy(Nstates_CaRU+24) = dC2Kv14;
dy(Nstates_CaRU+25) = dC3Kv14;
dy(Nstates_CaRU+26) = dOKv14;
dy(Nstates_CaRU+27) = dCI0Kv14;
dy(Nstates_CaRU+28) = dCI1Kv14;
dy(Nstates_CaRU+29) = dCI2Kv14;
dy(Nstates_CaRU+30) = dCI3Kv14;
dy(Nstates_CaRU+31) = dOIKv14;
dy(Nstates_CaRU+32) = dC1Herg;
dy(Nstates_CaRU+33) = dC2Herg;
dy(Nstates_CaRU+34) = dC3Herg;
dy(Nstates_CaRU+35) = dOHerg;
dy(Nstates_CaRU+36) = dIHerg;

if t > T_array(Counter)                %   Roughly eliminates data from rejected time steps
    Counter = Counter + 1;  
end
T_array(Counter) = t;                  %   Store currents/fluxes for output
ICaL_array(Counter) = ICaL;
JCaL_array(Counter) = JCaL;
JRyR_array(Counter) = JRyR;
CaSSavg_array(Counter) = CaSSavg;

