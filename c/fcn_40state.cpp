/*
 * fcn_40state.c
 *
 * Code generation for function 'fcn_40state'
 *
 * C source code generated on: Mon Mar  3 11:45:45 2014
 *
 */

/* Include files */
#include "fcn_40state.h"
#include <nvector/nvector_serial.h>  /* serial N_Vector types, fct. and macros */
#include <sundials/sundials_types.h> /* definition of realtype */
#include <stdio.h>

//f1(realtype t, N_Vector y, N_Vector ydot, void *user_data)
/* Function Definitions */
int fcn_40state(real_T t, N_Vector nvec_y, N_Vector nvec_dy, void *user_data) {
	real_T* y = N_VGetArrayPointer(nvec_y);
	real_T* dy = N_VGetArrayPointer(nvec_dy);
	real_T* params = (real_T*) user_data;

	/*
	 //        -------------------------------------------------------------
	 //
	 //          NOTICE OF COPYRIGHT AND OWNERSHIP OF SOFTWARE
	 //
	 //          Copyright 2005, The Johns Hopkins University
	 //             School of Medicine and Whiting School of Engineering.
	 //             All rights reserved.
	 // 			For research use only; commercial use prohibited.
	 // 			Distribution without permission of Joseph L. Greenstein or
	 // 			Raimond L. Winslow not permitted.
	 //           (jgreenst@jhu.edu, rwinslow@jhu.edu)
	 //
	 //          Name of Program: 40-State coupled LCC-RyR Model
	 //          Version: version 1.0
	 //          Date: September 2005
	 //
	 //        -------------------------------------------------------------
	 */

	real_T Vclamp_flag = params[INDEX_Vclamp_flag];
	real_T Period = params[INDEX_Period];
	real_T Shift = params[INDEX_Shift];
	real_T Pulse_duration = params[INDEX_Pulse_duration];
	real_T Pulse_amplitude = params[INDEX_Pulse_amplitude];

	real_T Faraday = params[INDEX_Faraday]; //   Faraday's constant (C/mmol)
	real_T Temp = params[INDEX_Temp]; //   Temperature (K)
	real_T Rgas = params[INDEX_Rgas]; //   Universal Gas constant (J/[mol K])
	real_T Csa = params[INDEX_Csa]; //   Membrane capacitance (pF)
	real_T VSS = params[INDEX_VSS]; //   Single dyad subspace volume (microL)
	real_T VSR = params[INDEX_VSR]; //   SR volume (microL)
	real_T Vmyo = params[INDEX_Vmyo]; //   Myoplasmic volume (microL)
	real_T PCa = params[INDEX_PCa]; //   LCC single channel permeability (cm^3/s)
	real_T JRyRmax = params[INDEX_JRyRmax]; //   Ca flux rate through open RyR (1/ms)
	real_T r_xfer = params[INDEX_r_xfer]; //   Ca flux rate from SS to myoplasm (1/ms)
	real_T Cao = params[INDEX_Cao]; //   Extracellular [Ca] (mM)
	real_T Ko = params[INDEX_Ko ]; //   Extracellular [K] (mM)
	real_T Nao = params[INDEX_Nao]; //   Extracellular [Na] (mM)
	real_T Clo = params[INDEX_Clo]; //   Extracellular [Cl] (mM)
	real_T Cli = params[INDEX_Cli]; //   Intracellular [Cl] (mM)

	real_T KdIto2 = params[INDEX_KdIto2]; //   Dissociation constant of Cl channel (Ito2) for Ca (mM)
	real_T PCl = params[INDEX_PCl]; //   Cl single channel permeability (cm^3/s)

	real_T GKr = params[INDEX_GKr]; //   Maximal conductance of IKr (mS/microF)
	real_T GKs = params[INDEX_GKs]; //   Maximal conductance of IKs (mS/microF)
	real_T GK1 = params[INDEX_GK1]; //   Maximal conductance of IK1 (mS/microF)
	real_T GKp = params[INDEX_GKp]; //   Maximal conductance of IKp (mS/microF)
	real_T GNa = params[INDEX_GNa]; //   Maximal conductance of INa (mS/microF)
	real_T kNaCa = params[INDEX_kNaCa]; //   Scale factor for Na/Ca exchanger (A/F)
	real_T KmNa = params[INDEX_KmNa]; //   Na half saturation constant for Na/Ca exchanger (mM)
	real_T KmCa = params[INDEX_KmCa]; //   Ca half saturation constant for Na/Ca exchanger (mM)
	real_T KmK1 = params[INDEX_KmK1]; //   K half saturation constant for IK1 (mM)
	real_T ksat = params[INDEX_ksat]; //   Na/Ca exchange saturation factor at negative potentials
	real_T eta = params[INDEX_eta]; //   Controls voltage dependence of Na/Ca exchange
	real_T INaKmax = params[INDEX_INaKmax]; //   Scale factor for Na/K pump (A/F)
	real_T KmNai = params[INDEX_KmNai]; //   Na half saturation constant for Na/K pump (mM)
	real_T KmKo = params[INDEX_KmKo]; //   K half saturation constant for Na/K pump (mM)
	real_T IpCamax = params[INDEX_IpCamax]; //   Maximum sarcolemmal Ca pump current (A/F)
	real_T KmpCa = params[INDEX_KmpCa]; //   Ca half saturation constant for sarcolemmal Ca pump (mM)
	real_T GCab = params[INDEX_GCab]; //   Maximal conductance of ICab (mS/microF)
	real_T GNab = params[INDEX_GNab]; //   Maximal conductance of INab (mS/microF)

	real_T Kfb = params[INDEX_Kfb]; //   Forward half-saturation constant for SR Ca ATPase (mM)
	real_T Krb = params[INDEX_Krb]; //   Reverse half-saturation constant for SR Ca ATPase (mM)
	real_T KSR = params[INDEX_KSR]; //   Scale factor for SR Ca ATPase
	real_T Nfb = params[INDEX_Nfb]; //   Forward cooperativity constant for SR Ca ATPase
	real_T Nrb = params[INDEX_Nrb]; //   Reverse cooperativity constant for SR Ca ATPase
	real_T vmaxf = params[INDEX_vmaxf]; //   SR Ca ATPase forward rate parameter (mM/ms)
	real_T vmaxr = params[INDEX_vmaxr]; //   SR Ca ATPase reverse rate parameter (mM/ms)

	real_T KvScale = params[INDEX_KvScale]; //   Scale factor for Kv4.3 and Kv1.4 currents
	real_T Kv43Frac = params[INDEX_Kv43Frac]; //   Fraction of Ito1 (peak current at 40 mV) that is made up of Kv4.3
	real_T alphaa0Kv43 = params[INDEX_alphaa0Kv43]; //   IKv43 rate parameter (1/ms)
	real_T aaKv43 = params[INDEX_aaKv43]; //   IKv43 rate parameter (1/mV)
	real_T betaa0Kv43 = params[INDEX_betaa0Kv43]; //   IKv43 rate parameter (1/ms)
	real_T baKv43 = params[INDEX_baKv43]; //   IKv43 rate parameter (1/mV)
	real_T alphai0Kv43 = params[INDEX_alphai0Kv43]; //   IKv43 rate parameter (1/ms)
	real_T aiKv43 = params[INDEX_aiKv43]; //   IKv43 rate parameter (1/mV)
	real_T betai0Kv43 = params[INDEX_betai0Kv43]; //   IKv43 rate parameter (1/ms)
	real_T biKv43 = params[INDEX_biKv43]; //   IKv43 rate parameter (1/mV)
	real_T alphaa0Kv14 = params[INDEX_alphaa0Kv14]; //   IKv14 rate parameter (1/ms)
	real_T aaKv14 = params[INDEX_aaKv14]; //   IKv14 rate parameter (1/mV)
	real_T betaa0Kv14 = params[INDEX_betaa0Kv14]; //   IKv14 rate parameter (1/ms)
	real_T baKv14 = params[INDEX_baKv14]; //   IKv14 rate parameter (1/mV)
	real_T alphai0Kv14 = params[INDEX_alphai0Kv14]; //   IKv14 rate parameter (1/ms)
	real_T betai0Kv14 = params[INDEX_betai0Kv14]; //   IKv14 rate parameter (1/ms)
	real_T f1Kv43 = params[INDEX_f1Kv43]; //   IKv43 rate parameter
	real_T f2Kv43 = params[INDEX_f2Kv43]; //   IKv43 rate parameter
	real_T f3Kv43 = params[INDEX_f3Kv43]; //   IKv43 rate parameter
	real_T f4Kv43 = params[INDEX_f4Kv43 ]; //   IKv43 rate parameter
	real_T b1Kv43 = params[INDEX_b1Kv43]; //   IKv43 rate parameter
	real_T b2Kv43 = params[INDEX_b2Kv43]; //   IKv43 rate parameter
	real_T b3Kv43 = params[INDEX_b3Kv43]; //   IKv43 rate parameter
	real_T b4Kv43 = params[INDEX_b4Kv43]; //   IKv43 rate parameter
	real_T f1Kv14 = params[INDEX_f1Kv14]; //   IKv14 rate parameter
	real_T f2Kv14 = params[INDEX_f2Kv14]; //   IKv14 rate parameter
	real_T f3Kv14 = params[INDEX_f3Kv14]; //   IKv14 rate parameter
	real_T f4Kv14 = params[INDEX_f4Kv14]; //   IKv14 rate parameter
	real_T b1Kv14 = params[INDEX_b1Kv14]; //   IKv14 rate parameter
	real_T b2Kv14 = params[INDEX_b2Kv14]; //   IKv14 rate parameter
	real_T b3Kv14 = params[INDEX_b3Kv14]; //   IKv14 rate parameter
	real_T b4Kv14 = params[INDEX_b4Kv14]; //   IKv14 rate parameter

	real_T A0_HERG = params[INDEX_A0_HERG]; //   IKr rate parameter (1/ms)
	real_T B0_HERG = params[INDEX_B0_HERG]; //   IKr rate parameter (1/mV)
	real_T A1_HERG = params[INDEX_A1_HERG]; //   IKr rate parameter (1/ms)
	real_T B1_HERG = params[INDEX_B1_HERG]; //   IKr rate parameter (1/mV)
	real_T A2_HERG = params[INDEX_A2_HERG]; //   IKr rate parameter (1/ms)
	real_T B2_HERG = params[INDEX_B2_HERG]; //   IKr rate parameter (1/mV)
	real_T A3_HERG = params[INDEX_A3_HERG]; //   IKr rate parameter (1/ms)
	real_T B3_HERG = params[INDEX_B3_HERG]; //   IKr rate parameter (1/mV)
	real_T A4_HERG = params[INDEX_A4_HERG]; //   IKr rate parameter (1/ms)
	real_T B4_HERG = params[INDEX_B4_HERG]; //   IKr rate parameter (1/mV)
	real_T A5_HERG = params[INDEX_A5_HERG]; //   IKr rate parameter (1/ms)
	real_T B5_HERG = params[INDEX_B5_HERG]; //   IKr rate parameter (1/mV)
	real_T A6_HERG = params[INDEX_A6_HERG]; //   IKr rate parameter (1/ms)
	real_T B6_HERG = params[INDEX_B6_HERG]; //   IKr rate parameter (1/mV)
	real_T T_Const_HERG = params[INDEX_T_Const_HERG]; //   IKr rate scale factor for temperature

	real_T CSQNtot = params[INDEX_CSQNtot]; //   Total SR calsequestrin concentration (mM)
	real_T KmCSQN = params[INDEX_KmCSQN]; //   Ca half saturation constant for calsequestrin (mM)
	real_T LTRPNtot = params[INDEX_LTRPNtot]; //   Total troponin low-affinity site concentration (mM)
	real_T HTRPNtot = params[INDEX_HTRPNtot]; //   Total troponin high-affinity site concentration (mM)
	real_T khtrpn_plus = params[INDEX_khtrpn_plus]; //   Troponin Ca binding rate parameter (1/[mM ms])
	real_T khtrpn_minus = params[INDEX_khtrpn_minus]; //   Troponin Ca binding rate parameter (1/ms)
	real_T kltrpn_plus = params[INDEX_kltrpn_plus]; //   Troponin Ca binding rate parameter (1/[mM ms])
	real_T kltrpn_minus = params[INDEX_kltrpn_minus]; //   Troponin Ca binding rate parameter (1/ms)
	real_T CMDNtot = params[INDEX_CMDNtot]; //   Total myoplasmic calmodulin concentration (mM)
	real_T EGTAtot = params[INDEX_EGTAtot]; //   Total myoplasmic EGTA concentration (mM)
	real_T KmCMDN = params[INDEX_KmCMDN]; //   Ca half saturation constant for calmodulin (mM)
	real_T KmEGTA = params[INDEX_KmEGTA]; //   Ca half saturation constant for EGTA (mM)

	real_T fL = params[INDEX_fL]; //   L-type Ca channel rate parameter (1/ms)
	real_T gL = params[INDEX_gL]; //   L-type Ca channel rate parameter (1/ms)
	real_T bL = params[INDEX_bL]; //   L-type Ca channel rate parameter
	real_T aL = params[INDEX_aL]; //   L-type Ca channel rate parameter
	real_T omega = params[INDEX_omega]; //   L-type Ca channel rate parameter (1/ms)
	real_T gammacf = params[INDEX_gammacf]; //   L-type Ca channel rate parameter (1/[mM ms])

	real_T k12 = params[INDEX_k12]; //   RyR channel rate parameter (1/ms)
	real_T k21 = params[INDEX_k21]; //   RyR channel rate parameter (1/ms)
	real_T k23 = params[INDEX_k23]; //   RyR channel rate parameter (1/ms)
	real_T k32 = params[INDEX_k32]; //   RyR channel rate parameter (1/ms)
	real_T k34 = params[INDEX_k34]; //   RyR channel rate parameter (1/ms)
	real_T k43 = params[INDEX_k43]; //   RyR channel rate parameter (1/ms)
	real_T k45 = params[INDEX_k45]; //   RyR channel rate parameter (1/ms)
	real_T k54 = params[INDEX_k54]; //   RyR channel rate parameter (1/ms)
	real_T k56 = params[INDEX_k56]; //   RyR channel rate parameter (1/ms)
	real_T k65 = params[INDEX_k65]; //   RyR channel rate parameter (1/ms)
	real_T k25 = params[INDEX_k25]; //   RyR channel rate parameter (1/ms)
	real_T k52 = params[INDEX_k52]; //   RyR channel rate parameter (1/ms)

	int NCaRU = (int)params[INDEX_NCaRU]; //   Number of Ca Release Units
	real_T RT_over_F = Rgas * Temp / Faraday;
	real_T GKv43 = Kv43Frac * KvScale * 0.1; //   Maximal conductance of IKv43 (mS/microF)
	real_T PKv14 = (1 - Kv43Frac) * KvScale * 4.792933e-7; //   Maximal conductance of IKv14 (mS/microF)
	real_T C2H_to_C3H = T_Const_HERG * 0.02608362043337; //   IKr transition rate (1/ms)
	real_T C3H_to_C2H = T_Const_HERG * 0.14832978132145; //   IKr transition rate (1/ms)


	real_T V = y[INDEX_V]; //   Assign states to local variables
	real_T mNa = y[INDEX_mNa];
	real_T hNa = y[INDEX_hNa];
	real_T jNa = y[INDEX_jNa];
	real_T Nai = y[INDEX_Nai];
	real_T Ki = y[INDEX_Ki];
	real_T Cai = y[INDEX_Cai];
	real_T CaSR = y[INDEX_CaSR];
	real_T LTRPNCa = y[INDEX_LTRPNCa];
	real_T HTRPNCa = y[INDEX_HTRPNCa];
	real_T xKs = y[INDEX_xKs];
	real_T C0Kv43 = y[INDEX_C0Kv43];
	real_T C1Kv43 = y[INDEX_C1Kv43];
	real_T C2Kv43 = y[INDEX_C2Kv43];
	real_T C3Kv43 = y[INDEX_C3Kv43];
	real_T OKv43 = y[INDEX_OKv43];
	real_T CI0Kv43 = y[INDEX_CI0Kv43];
	real_T CI1Kv43 = y[INDEX_CI1Kv43];
	real_T CI2Kv43 = y[INDEX_CI2Kv43];
	real_T CI3Kv43 = y[INDEX_CI3Kv43];
	real_T OIKv43 = y[INDEX_OIKv43];
	real_T C0Kv14 = y[INDEX_C0Kv14];
	real_T C1Kv14 = y[INDEX_C1Kv14];
	real_T C2Kv14 = y[INDEX_C2Kv14];
	real_T C3Kv14 = y[INDEX_C3Kv14];
	real_T OKv14 = y[INDEX_OKv14];
	real_T CI0Kv14 = y[INDEX_CI0Kv14];
	real_T CI1Kv14 = y[INDEX_CI1Kv14];
	real_T CI2Kv14 = y[INDEX_CI2Kv14];
	real_T CI3Kv14 = y[INDEX_CI3Kv14];
	real_T OIKv14 = y[INDEX_OIKv14];
	real_T C1Herg = y[INDEX_C1Herg];
	real_T C2Herg = y[INDEX_C2Herg];
	real_T C3Herg = y[INDEX_C3Herg];
	real_T OHerg = y[INDEX_OHerg];
	real_T IHerg = y[INDEX_IHerg];

	real_T VF_over_RT = V / RT_over_F;
	real_T VFsq_over_RT = (1000 * Faraday) * VF_over_RT;

	/*real_T time_on_Is1 = floor((t - Shift) / Period) * Period; //   Establish stimulus current
	real_T time_off_Is1 = time_on_Is1 + Pulse_duration;
	real_T Istim = 0;
	if ((t - Shift) >= time_on_Is1 & (t - Shift) < time_off_Is1) {
		Istim = Istim + Pulse_amplitude;
	}*/
	real_T Istim = params[INDEX_I_STIM]; //   Stimulus current

	real_T alpha = 0.835399 * exp(0.0269241 * (V - 35)); //   L-type Ca channel rate parameter (1/ms)
	real_T beta = 0.0331584 * exp(-0.0934594 * (V - 35)); //   L-type Ca channel rate parameter (1/ms)

	real_T yCainf = 0.4 / (1 + exp((V + 12.5) / 5)) + 0.6; //   L-type Ca channel steady-state inactivation
	real_T tauyCa = 340 / (1 + exp((V + 30) / 12)) + 60; //   L-type Ca channel inactivation time constant (ms)
	real_T kby = yCainf / tauyCa; //   L-type Ca channel rate parameter (1/ms)
	real_T kfy = (1 - yCainf) / tauyCa; //   L-type Ca channel rate parameter (1/ms)

	real_T CaSS_1 = Cai; //   Ca concentration for each open-closed configuaration
	real_T CaSS_2 =
			(PCa * 2 * VF_over_RT / VSS * 0.341 * Cao
					/ (exp(2 * VF_over_RT) - 1) + r_xfer * Cai)
					/ (PCa * 2 * VF_over_RT / VSS / (1 - exp(-2 * VF_over_RT))
							+ r_xfer);
	real_T CaSS_3 = (JRyRmax * CaSR + r_xfer * Cai) / (JRyRmax + r_xfer);
	real_T CaSS_4 = (PCa * 2 * VF_over_RT / VSS * 0.341 * Cao
			/ (exp(2 * VF_over_RT) - 1) + JRyRmax * CaSR + r_xfer * Cai)
			/ (PCa * 2 * VF_over_RT / VSS / (1 - exp(-2 * VF_over_RT)) + JRyRmax
					+ r_xfer);

	real_T LCC_1_to_2 = alpha; //   Set L-type Ca channel rates (1/ms)
	real_T LCC_2_to_1 = beta;
	real_T LCC_2_to_3 = fL;
	real_T LCC_3_to_2 = gL;
	real_T LCC_1_to_4_CaSS_1 = gammacf * CaSS_1;
	real_T LCC_1_to_4_CaSS_2 = gammacf * CaSS_2;
	real_T LCC_1_to_4_CaSS_3 = gammacf * CaSS_3;
	real_T LCC_1_to_4_CaSS_4 = gammacf * CaSS_4;
	real_T LCC_4_to_1 = omega;
	real_T LCC_4_to_5 = aL * alpha;
	real_T LCC_5_to_4 = beta / bL;
	real_T LCC_2_to_5_CaSS_1 = aL * gammacf * CaSS_1;
	real_T LCC_2_to_5_CaSS_2 = aL * gammacf * CaSS_2;
	real_T LCC_2_to_5_CaSS_3 = aL * gammacf * CaSS_3;
	real_T LCC_2_to_5_CaSS_4 = aL * gammacf * CaSS_4;
	real_T LCC_5_to_2 = omega / bL;

	real_T LCC_6_to_7 = alpha;
	real_T LCC_7_to_6 = beta;
	real_T LCC_7_to_8 = fL;
	real_T LCC_8_to_7 = gL;
	real_T LCC_6_to_9_CaSS_1 = gammacf * CaSS_1;
	real_T LCC_6_to_9_CaSS_2 = gammacf * CaSS_2;
	real_T LCC_6_to_9_CaSS_3 = gammacf * CaSS_3;
	real_T LCC_6_to_9_CaSS_4 = gammacf * CaSS_4;
	real_T LCC_9_to_6 = omega;
	real_T LCC_9_to_10 = aL * alpha;
	real_T LCC_10_to_9 = beta / bL;
	real_T LCC_7_to_10_CaSS_1 = aL * gammacf * CaSS_1;
	real_T LCC_7_to_10_CaSS_2 = aL * gammacf * CaSS_2;
	real_T LCC_7_to_10_CaSS_3 = aL * gammacf * CaSS_3;
	real_T LCC_7_to_10_CaSS_4 = aL * gammacf * CaSS_4;
	real_T LCC_10_to_7 = omega / bL;

	real_T LCC_1_to_6 = kfy;
	real_T LCC_2_to_7 = kfy;
	real_T LCC_3_to_8 = kfy;
	real_T LCC_4_to_9 = kfy;
	real_T LCC_5_to_10 = kfy;
	real_T LCC_6_to_1 = kby;
	real_T LCC_7_to_2 = kby;
	real_T LCC_8_to_3 = kby;
	real_T LCC_9_to_4 = kby;
	real_T LCC_10_to_5 = kby;

	real_T CaSSmod_1 = min(CaSS_1, 0.05); // 50 microM max on RyR rates except for k12
	real_T CaSSmod_2 = min(CaSS_2, 0.05);
	real_T CaSSmod_3 = min(CaSS_3, 0.05);
	real_T CaSSmod_4 = min(CaSS_4, 0.05);

	real_T RyR_1_to_2_CaSS_1 = k12 * CaSS_1 * CaSS_1; //   Set RyR channel rates (1/ms)
	real_T RyR_1_to_2_CaSS_2 = k12 * CaSS_2 * CaSS_2;
	real_T RyR_1_to_2_CaSS_3 = k12 * CaSS_3 * CaSS_3;
	real_T RyR_1_to_2_CaSS_4 = k12 * CaSS_4 * CaSS_4;
	real_T RyR_2_to_1 = k21;
	real_T RyR_2_to_3_CaSS_1 = k23 * CaSSmod_1 * CaSSmod_1;
	real_T RyR_2_to_3_CaSS_2 = k23 * CaSSmod_2 * CaSSmod_2;
	real_T RyR_2_to_3_CaSS_3 = k23 * CaSSmod_3 * CaSSmod_3;
	real_T RyR_2_to_3_CaSS_4 = k23 * CaSSmod_4 * CaSSmod_4;
	real_T RyR_3_to_2_CaSS_1 = k32 * k43 / (k34 * CaSSmod_1 * CaSSmod_1 + k43);
	real_T RyR_3_to_2_CaSS_2 = k32 * k43 / (k34 * CaSSmod_2 * CaSSmod_2 + k43);
	real_T RyR_3_to_2_CaSS_3 = k32 * k43 / (k34 * CaSSmod_3 * CaSSmod_3 + k43);
	real_T RyR_3_to_2_CaSS_4 = k32 * k43 / (k34 * CaSSmod_4 * CaSSmod_4 + k43);
	real_T RyR_2_to_4_CaSS_1 = k25 * CaSSmod_1 * CaSSmod_1;
	real_T RyR_2_to_4_CaSS_2 = k25 * CaSSmod_2 * CaSSmod_2;
	real_T RyR_2_to_4_CaSS_3 = k25 * CaSSmod_3 * CaSSmod_3;
	real_T RyR_2_to_4_CaSS_4 = k25 * CaSSmod_4 * CaSSmod_4;
	real_T RyR_4_to_2_CaSS_1 = k52 * k65 / (k56 * CaSSmod_1 * CaSSmod_1 + k65);
	real_T RyR_4_to_2_CaSS_2 = k52 * k65 / (k56 * CaSSmod_2 * CaSSmod_2 + k65);
	real_T RyR_4_to_2_CaSS_3 = k52 * k65 / (k56 * CaSSmod_3 * CaSSmod_3 + k65);
	real_T RyR_4_to_2_CaSS_4 = k52 * k65 / (k56 * CaSSmod_4 * CaSSmod_4 + k65);
	real_T RyR_3_to_4_CaSS_1 = k45 * k34 * CaSSmod_1 * CaSSmod_1
			/ (k34 * CaSSmod_1 * CaSSmod_1 + k43);
	real_T RyR_3_to_4_CaSS_2 = k45 * k34 * CaSSmod_2 * CaSSmod_2
			/ (k34 * CaSSmod_2 * CaSSmod_2 + k43);
	real_T RyR_3_to_4_CaSS_3 = k45 * k34 * CaSSmod_3 * CaSSmod_3
			/ (k34 * CaSSmod_3 * CaSSmod_3 + k43);
	real_T RyR_3_to_4_CaSS_4 = k45 * k34 * CaSSmod_4 * CaSSmod_4
			/ (k34 * CaSSmod_4 * CaSSmod_4 + k43);
	real_T RyR_4_to_3_CaSS_1 = k65 * k54 * CaSSmod_1 * CaSSmod_1
			/ (k56 * CaSSmod_1 * CaSSmod_1 + k65);
	real_T RyR_4_to_3_CaSS_2 = k65 * k54 * CaSSmod_2 * CaSSmod_2
			/ (k56 * CaSSmod_2 * CaSSmod_2 + k65);
	real_T RyR_4_to_3_CaSS_3 = k65 * k54 * CaSSmod_3 * CaSSmod_3
			/ (k56 * CaSSmod_3 * CaSSmod_3 + k65);
	real_T RyR_4_to_3_CaSS_4 = k65 * k54 * CaSSmod_4 * CaSSmod_4
			/ (k56 * CaSSmod_4 * CaSSmod_4 + k65);

	dy[0] = -LCC_1_to_2 * y[0] + LCC_2_to_1 * y[1] - LCC_1_to_4_CaSS_1 * y[0]
			+ LCC_4_to_1 * y[3] - LCC_1_to_6 * y[0] + LCC_6_to_1 * y[5]
			- RyR_1_to_2_CaSS_1 * y[0] + RyR_2_to_1 * y[10];
	dy[1] = +LCC_1_to_2 * y[0] - LCC_2_to_1 * y[1] - LCC_2_to_3 * y[1]
			+ LCC_3_to_2 * y[2] - LCC_2_to_5_CaSS_1 * y[1] + LCC_5_to_2 * y[4]
			- LCC_2_to_7 * y[1] + LCC_7_to_2 * y[6] - RyR_1_to_2_CaSS_1 * y[1]
			+ RyR_2_to_1 * y[11];
	dy[2] = +LCC_2_to_3 * y[1] - LCC_3_to_2 * y[2] - LCC_3_to_8 * y[2]
			+ LCC_8_to_3 * y[7] - RyR_1_to_2_CaSS_2 * y[2] + RyR_2_to_1 * y[12];
	dy[3] = +LCC_1_to_4_CaSS_1 * y[0] - LCC_4_to_1 * y[3] - LCC_4_to_5 * y[3]
			+ LCC_5_to_4 * y[4] - LCC_4_to_9 * y[3] + LCC_9_to_4 * y[8]
			- RyR_1_to_2_CaSS_1 * y[3] + RyR_2_to_1 * y[13];
	dy[4] = +LCC_2_to_5_CaSS_1 * y[1] - LCC_5_to_2 * y[4] + LCC_4_to_5 * y[3]
			- LCC_5_to_4 * y[4] - LCC_5_to_10 * y[4] + LCC_10_to_5 * y[9]
			- RyR_1_to_2_CaSS_1 * y[4] + RyR_2_to_1 * y[14];
	dy[5] = +LCC_1_to_6 * y[0] - LCC_6_to_1 * y[5] - LCC_6_to_7 * y[5]
			+ LCC_7_to_6 * y[6] - LCC_6_to_9_CaSS_1 * y[5] + LCC_9_to_6 * y[8]
			- RyR_1_to_2_CaSS_1 * y[5] + RyR_2_to_1 * y[15];
	dy[6] = +LCC_2_to_7 * y[1] - LCC_7_to_2 * y[6] + LCC_6_to_7 * y[5]
			- LCC_7_to_6 * y[6] - LCC_7_to_8 * y[6] + LCC_8_to_7 * y[7]
			- LCC_7_to_10_CaSS_1 * y[6] + LCC_10_to_7 * y[9]
			- RyR_1_to_2_CaSS_1 * y[6] + RyR_2_to_1 * y[16];
	dy[7] = +LCC_3_to_8 * y[2] - LCC_8_to_3 * y[7] + LCC_7_to_8 * y[6]
			- LCC_8_to_7 * y[7] - RyR_1_to_2_CaSS_1 * y[7] + RyR_2_to_1 * y[17];
	dy[8] = +LCC_4_to_9 * y[3] - LCC_9_to_4 * y[8] + LCC_6_to_9_CaSS_1 * y[5]
			- LCC_9_to_6 * y[8] - LCC_9_to_10 * y[8] + LCC_10_to_9 * y[9]
			- RyR_1_to_2_CaSS_1 * y[8] + RyR_2_to_1 * y[18];
	dy[9] = +LCC_5_to_10 * y[4] - LCC_10_to_5 * y[9] + LCC_7_to_10_CaSS_1 * y[6]
			- LCC_10_to_7 * y[9] + LCC_9_to_10 * y[8] - LCC_10_to_9 * y[9]
			- RyR_1_to_2_CaSS_1 * y[9] + RyR_2_to_1 * y[19];
	dy[10] = +RyR_1_to_2_CaSS_1 * y[0] - RyR_2_to_1 * y[10] - LCC_1_to_2 * y[10]
			+ LCC_2_to_1 * y[11] - LCC_1_to_4_CaSS_1 * y[10]
			+ LCC_4_to_1 * y[13] - LCC_1_to_6 * y[10] + LCC_6_to_1 * y[15]
			- RyR_2_to_3_CaSS_1 * y[10] + RyR_3_to_2_CaSS_3 * y[20]
			- RyR_2_to_4_CaSS_1 * y[10] + RyR_4_to_2_CaSS_1 * y[30];
	dy[11] = +RyR_1_to_2_CaSS_1 * y[1] - RyR_2_to_1 * y[11] + LCC_1_to_2 * y[10]
			- LCC_2_to_1 * y[11] - LCC_2_to_3 * y[11] + LCC_3_to_2 * y[12]
			- LCC_2_to_5_CaSS_1 * y[11] + LCC_5_to_2 * y[14]
			- LCC_2_to_7 * y[11] + LCC_7_to_2 * y[16]
			- RyR_2_to_3_CaSS_1 * y[11] + RyR_3_to_2_CaSS_3 * y[21]
			- RyR_2_to_4_CaSS_1 * y[11] + RyR_4_to_2_CaSS_1 * y[31];
	dy[12] = +RyR_1_to_2_CaSS_2 * y[2] - RyR_2_to_1 * y[12] + LCC_2_to_3 * y[11]
			- LCC_3_to_2 * y[12] - LCC_3_to_8 * y[12] + LCC_8_to_3 * y[17]
			- RyR_2_to_3_CaSS_2 * y[12] + RyR_3_to_2_CaSS_4 * y[22]
			- RyR_2_to_4_CaSS_2 * y[12] + RyR_4_to_2_CaSS_2 * y[32];
	dy[13] = +RyR_1_to_2_CaSS_1 * y[3] - RyR_2_to_1 * y[13]
			+ LCC_1_to_4_CaSS_1 * y[10] - LCC_4_to_1 * y[13]
			- LCC_4_to_5 * y[13] + LCC_5_to_4 * y[14] - LCC_4_to_9 * y[13]
			+ LCC_9_to_4 * y[18] - RyR_2_to_3_CaSS_1 * y[13]
			+ RyR_3_to_2_CaSS_3 * y[23] - RyR_2_to_4_CaSS_1 * y[13]
			+ RyR_4_to_2_CaSS_1 * y[33];
	dy[14] = +RyR_1_to_2_CaSS_1 * y[4] - RyR_2_to_1 * y[14]
			+ LCC_2_to_5_CaSS_1 * y[11] - LCC_5_to_2 * y[14]
			+ LCC_4_to_5 * y[13] - LCC_5_to_4 * y[14] - LCC_5_to_10 * y[14]
			+ LCC_10_to_5 * y[19] - RyR_2_to_3_CaSS_1 * y[14]
			+ RyR_3_to_2_CaSS_3 * y[24] - RyR_2_to_4_CaSS_1 * y[14]
			+ RyR_4_to_2_CaSS_1 * y[34];
	dy[15] = +RyR_1_to_2_CaSS_1 * y[5] - RyR_2_to_1 * y[15] + LCC_1_to_6 * y[10]
			- LCC_6_to_1 * y[15] - LCC_6_to_7 * y[15] + LCC_7_to_6 * y[16]
			- LCC_6_to_9_CaSS_1 * y[15] + LCC_9_to_6 * y[18]
			- RyR_2_to_3_CaSS_1 * y[15] + RyR_3_to_2_CaSS_3 * y[25]
			- RyR_2_to_4_CaSS_1 * y[15] + RyR_4_to_2_CaSS_1 * y[35];
	dy[16] = +RyR_1_to_2_CaSS_1 * y[6] - RyR_2_to_1 * y[16] + LCC_2_to_7 * y[11]
			- LCC_7_to_2 * y[16] + LCC_6_to_7 * y[15] - LCC_7_to_6 * y[16]
			- LCC_7_to_8 * y[16] + LCC_8_to_7 * y[17]
			- LCC_7_to_10_CaSS_1 * y[16] + LCC_10_to_7 * y[19]
			- RyR_2_to_3_CaSS_1 * y[16] + RyR_3_to_2_CaSS_3 * y[26]
			- RyR_2_to_4_CaSS_1 * y[16] + RyR_4_to_2_CaSS_1 * y[36];
	dy[17] = +RyR_1_to_2_CaSS_1 * y[7] - RyR_2_to_1 * y[17] + LCC_3_to_8 * y[12]
			- LCC_8_to_3 * y[17] + LCC_7_to_8 * y[16] - LCC_8_to_7 * y[17]
			- RyR_2_to_3_CaSS_1 * y[17] + RyR_3_to_2_CaSS_3 * y[27]
			- RyR_2_to_4_CaSS_1 * y[17] + RyR_4_to_2_CaSS_1 * y[37];
	dy[18] = +RyR_1_to_2_CaSS_1 * y[8] - RyR_2_to_1 * y[18] + LCC_4_to_9 * y[13]
			- LCC_9_to_4 * y[18] + LCC_6_to_9_CaSS_1 * y[15]
			- LCC_9_to_6 * y[18] - LCC_9_to_10 * y[18] + LCC_10_to_9 * y[19]
			- RyR_2_to_3_CaSS_1 * y[18] + RyR_3_to_2_CaSS_3 * y[28]
			- RyR_2_to_4_CaSS_1 * y[18] + RyR_4_to_2_CaSS_1 * y[38];
	dy[19] = +RyR_1_to_2_CaSS_1 * y[9] - RyR_2_to_1 * y[19]
			+ LCC_5_to_10 * y[14] - LCC_10_to_5 * y[19]
			+ LCC_7_to_10_CaSS_1 * y[16] - LCC_10_to_7 * y[19]
			+ LCC_9_to_10 * y[18] - LCC_10_to_9 * y[19]
			- RyR_2_to_3_CaSS_1 * y[19] + RyR_3_to_2_CaSS_3 * y[29]
			- RyR_2_to_4_CaSS_1 * y[19] + RyR_4_to_2_CaSS_1 * y[39];
	dy[20] = +RyR_2_to_3_CaSS_1 * y[10] - RyR_3_to_2_CaSS_3 * y[20]
			- LCC_1_to_2 * y[20] + LCC_2_to_1 * y[21]
			- LCC_1_to_4_CaSS_3 * y[20] + LCC_4_to_1 * y[23]
			- LCC_1_to_6 * y[20] + LCC_6_to_1 * y[25]
			- RyR_3_to_4_CaSS_3 * y[20] + RyR_4_to_3_CaSS_1 * y[30];
	dy[21] = +RyR_2_to_3_CaSS_1 * y[11] - RyR_3_to_2_CaSS_3 * y[21]
			+ LCC_1_to_2 * y[20] - LCC_2_to_1 * y[21] - LCC_2_to_3 * y[21]
			+ LCC_3_to_2 * y[22] - LCC_2_to_5_CaSS_3 * y[21]
			+ LCC_5_to_2 * y[24] - LCC_2_to_7 * y[21] + LCC_7_to_2 * y[26]
			- RyR_3_to_4_CaSS_3 * y[21] + RyR_4_to_3_CaSS_1 * y[31];
	dy[22] = +RyR_2_to_3_CaSS_2 * y[12] - RyR_3_to_2_CaSS_4 * y[22]
			+ LCC_2_to_3 * y[21] - LCC_3_to_2 * y[22] - LCC_3_to_8 * y[22]
			+ LCC_8_to_3 * y[27] - RyR_3_to_4_CaSS_4 * y[22]
			+ RyR_4_to_3_CaSS_2 * y[32];
	dy[23] = +RyR_2_to_3_CaSS_1 * y[13] - RyR_3_to_2_CaSS_3 * y[23]
			+ LCC_1_to_4_CaSS_3 * y[20] - LCC_4_to_1 * y[23]
			- LCC_4_to_5 * y[23] + LCC_5_to_4 * y[24] - LCC_4_to_9 * y[23]
			+ LCC_9_to_4 * y[28] - RyR_3_to_4_CaSS_3 * y[23]
			+ RyR_4_to_3_CaSS_1 * y[33];
	dy[24] = +RyR_2_to_3_CaSS_1 * y[14] - RyR_3_to_2_CaSS_3 * y[24]
			+ LCC_2_to_5_CaSS_3 * y[21] - LCC_5_to_2 * y[24]
			+ LCC_4_to_5 * y[23] - LCC_5_to_4 * y[24] - LCC_5_to_10 * y[24]
			+ LCC_10_to_5 * y[29] - RyR_3_to_4_CaSS_3 * y[24]
			+ RyR_4_to_3_CaSS_1 * y[34];
	dy[25] = +RyR_2_to_3_CaSS_1 * y[15] - RyR_3_to_2_CaSS_3 * y[25]
			+ LCC_1_to_6 * y[20] - LCC_6_to_1 * y[25] - LCC_6_to_7 * y[25]
			+ LCC_7_to_6 * y[26] - LCC_6_to_9_CaSS_3 * y[25]
			+ LCC_9_to_6 * y[28] - RyR_3_to_4_CaSS_3 * y[25]
			+ RyR_4_to_3_CaSS_1 * y[35];
	dy[26] = +RyR_2_to_3_CaSS_1 * y[16] - RyR_3_to_2_CaSS_3 * y[26]
			+ LCC_2_to_7 * y[21] - LCC_7_to_2 * y[26] + LCC_6_to_7 * y[25]
			- LCC_7_to_6 * y[26] - LCC_7_to_8 * y[26] + LCC_8_to_7 * y[27]
			- LCC_7_to_10_CaSS_3 * y[26] + LCC_10_to_7 * y[29]
			- RyR_3_to_4_CaSS_3 * y[26] + RyR_4_to_3_CaSS_1 * y[36];
	dy[27] = +RyR_2_to_3_CaSS_1 * y[17] - RyR_3_to_2_CaSS_3 * y[27]
			+ LCC_3_to_8 * y[22] - LCC_8_to_3 * y[27] + LCC_7_to_8 * y[26]
			- LCC_8_to_7 * y[27] - RyR_3_to_4_CaSS_3 * y[27]
			+ RyR_4_to_3_CaSS_1 * y[37];
	dy[28] = +RyR_2_to_3_CaSS_1 * y[18] - RyR_3_to_2_CaSS_3 * y[28]
			+ LCC_4_to_9 * y[23] - LCC_9_to_4 * y[28]
			+ LCC_6_to_9_CaSS_3 * y[25] - LCC_9_to_6 * y[28]
			- LCC_9_to_10 * y[28] + LCC_10_to_9 * y[29]
			- RyR_3_to_4_CaSS_3 * y[28] + RyR_4_to_3_CaSS_1 * y[38];
	dy[29] = +RyR_2_to_3_CaSS_1 * y[19] - RyR_3_to_2_CaSS_3 * y[29]
			+ LCC_5_to_10 * y[24] - LCC_10_to_5 * y[29]
			+ LCC_7_to_10_CaSS_3 * y[26] - LCC_10_to_7 * y[29]
			+ LCC_9_to_10 * y[28] - LCC_10_to_9 * y[29]
			- RyR_3_to_4_CaSS_3 * y[29] + RyR_4_to_3_CaSS_1 * y[39];
	dy[30] = +RyR_2_to_4_CaSS_1 * y[10] - RyR_4_to_2_CaSS_1 * y[30]
			+ RyR_3_to_4_CaSS_3 * y[20] - RyR_4_to_3_CaSS_1 * y[30]
			- LCC_1_to_2 * y[30] + LCC_2_to_1 * y[31]
			- LCC_1_to_4_CaSS_1 * y[30] + LCC_4_to_1 * y[33]
			- LCC_1_to_6 * y[30] + LCC_6_to_1 * y[35];
	dy[31] = +RyR_2_to_4_CaSS_1 * y[11] - RyR_4_to_2_CaSS_1 * y[31]
			+ RyR_3_to_4_CaSS_3 * y[21] - RyR_4_to_3_CaSS_1 * y[31]
			+ LCC_1_to_2 * y[30] - LCC_2_to_1 * y[31] - LCC_2_to_3 * y[31]
			+ LCC_3_to_2 * y[32] - LCC_2_to_5_CaSS_1 * y[31]
			+ LCC_5_to_2 * y[34] - LCC_2_to_7 * y[31] + LCC_7_to_2 * y[36];
	dy[32] = +RyR_2_to_4_CaSS_2 * y[12] - RyR_4_to_2_CaSS_2 * y[32]
			+ RyR_3_to_4_CaSS_4 * y[22] - RyR_4_to_3_CaSS_2 * y[32]
			+ LCC_2_to_3 * y[31] - LCC_3_to_2 * y[32] - LCC_3_to_8 * y[32]
			+ LCC_8_to_3 * y[37];
	dy[33] = +RyR_2_to_4_CaSS_1 * y[13] - RyR_4_to_2_CaSS_1 * y[33]
			+ RyR_3_to_4_CaSS_3 * y[23] - RyR_4_to_3_CaSS_1 * y[33]
			+ LCC_1_to_4_CaSS_1 * y[30] - LCC_4_to_1 * y[33]
			- LCC_4_to_5 * y[33] + LCC_5_to_4 * y[34] - LCC_4_to_9 * y[33]
			+ LCC_9_to_4 * y[38];
	dy[34] = +RyR_2_to_4_CaSS_1 * y[14] - RyR_4_to_2_CaSS_1 * y[34]
			+ RyR_3_to_4_CaSS_3 * y[24] - RyR_4_to_3_CaSS_1 * y[34]
			+ LCC_2_to_5_CaSS_1 * y[31] - LCC_5_to_2 * y[34]
			+ LCC_4_to_5 * y[33] - LCC_5_to_4 * y[34] - LCC_5_to_10 * y[34]
			+ LCC_10_to_5 * y[39];
	dy[35] = +RyR_2_to_4_CaSS_1 * y[15] - RyR_4_to_2_CaSS_1 * y[35]
			+ RyR_3_to_4_CaSS_3 * y[25] - RyR_4_to_3_CaSS_1 * y[35]
			+ LCC_1_to_6 * y[30] - LCC_6_to_1 * y[35] - LCC_6_to_7 * y[35]
			+ LCC_7_to_6 * y[36] - LCC_6_to_9_CaSS_1 * y[35]
			+ LCC_9_to_6 * y[38];
	dy[36] = +RyR_2_to_4_CaSS_1 * y[16] - RyR_4_to_2_CaSS_1 * y[36]
			+ RyR_3_to_4_CaSS_3 * y[26] - RyR_4_to_3_CaSS_1 * y[36]
			+ LCC_2_to_7 * y[31] - LCC_7_to_2 * y[36] + LCC_6_to_7 * y[35]
			- LCC_7_to_6 * y[36] - LCC_7_to_8 * y[36] + LCC_8_to_7 * y[37]
			- LCC_7_to_10_CaSS_1 * y[36] + LCC_10_to_7 * y[39];
	dy[37] = +RyR_2_to_4_CaSS_1 * y[17] - RyR_4_to_2_CaSS_1 * y[37]
			+ RyR_3_to_4_CaSS_3 * y[27] - RyR_4_to_3_CaSS_1 * y[37]
			+ LCC_3_to_8 * y[32] - LCC_8_to_3 * y[37] + LCC_7_to_8 * y[36]
			- LCC_8_to_7 * y[37];
	dy[38] = +RyR_2_to_4_CaSS_1 * y[18] - RyR_4_to_2_CaSS_1 * y[38]
			+ RyR_3_to_4_CaSS_3 * y[28] - RyR_4_to_3_CaSS_1 * y[38]
			+ LCC_4_to_9 * y[33] - LCC_9_to_4 * y[38]
			+ LCC_6_to_9_CaSS_1 * y[35] - LCC_9_to_6 * y[38]
			- LCC_9_to_10 * y[38] + LCC_10_to_9 * y[39];
	dy[39] = +RyR_2_to_4_CaSS_1 * y[19] - RyR_4_to_2_CaSS_1 * y[39]
			+ RyR_3_to_4_CaSS_3 * y[29] - RyR_4_to_3_CaSS_1 * y[39]
			+ LCC_5_to_10 * y[34] - LCC_10_to_5 * y[39]
			+ LCC_7_to_10_CaSS_1 * y[36] - LCC_10_to_7 * y[39]
			+ LCC_9_to_10 * y[38] - LCC_10_to_9 * y[39];

	real_T PCaSS_2 = y[2] + y[12] + y[32]; //   Evaluate occupancy probability for each open-closed configuartion
	real_T PCaSS_3 = y[20] + y[21] + y[23] + y[24] + y[25] + y[26] + y[27]
			+ y[28] + y[29];
	real_T PCaSS_4 = y[22];
	real_T PCaSS_1 = 1 - PCaSS_2 - PCaSS_3 - PCaSS_4;

	//   Evaluate fluxes/currents across LCC-RyR boundary
	real_T ICaL = NCaRU / Csa * PCa * 4 * VFsq_over_RT
			/ (exp(2 * VF_over_RT) - 1)
			* (PCaSS_2 * (CaSS_2 * exp(2 * VF_over_RT) - 0.341 * Cao)
					+ PCaSS_4 * (CaSS_4 * exp(2 * VF_over_RT) - 0.341 * Cao));
	real_T JCaL = -ICaL * Csa / (2 * Faraday * 1000 * VSS);
	real_T Ito2 = NCaRU / Csa * PCl * VFsq_over_RT
			* (Cli * exp(-VF_over_RT) - Clo) / (exp(-VF_over_RT) - 1)
			* (PCaSS_4 / (1 + KdIto2 / CaSS_4) + PCaSS_3 / (1 + KdIto2 / CaSS_3)
					+ PCaSS_2 / (1 + KdIto2 / CaSS_2)
					+ PCaSS_1 / (1 + KdIto2 / CaSS_1));
	real_T JRyR = NCaRU * JRyRmax
			* (PCaSS_4 * (CaSR - CaSS_4) + PCaSS_3 * (CaSR - CaSS_3));
	real_T Jss2b = NCaRU * r_xfer
			* (PCaSS_4 * (CaSS_4 - Cai) + PCaSS_3 * (CaSS_3 - Cai)
					+ PCaSS_2 * (CaSS_2 - Cai) + PCaSS_1 * (CaSS_1 - Cai));
	real_T CaSSavg = PCaSS_1 * CaSS_1 + PCaSS_2 * CaSS_2 + PCaSS_3 * CaSS_3
			+ PCaSS_4 * CaSS_4; // Average dyadic Ca concentration

			//   Evaluate currents and state derivatives of global model
	real_T ENa = RT_over_F * log(Nao / Nai);
	real_T EK = RT_over_F * log(Ko / Ki);
	real_T EKs = RT_over_F * log((Ko + 0.01833 * Nao) / (Ki + 0.01833 * Nai));
	real_T ECa = 0.5 * RT_over_F * log(Cao / max(1e-10,Cai));

	real_T INa = GNa * (mNa * mNa * mNa) * hNa * jNa * (V - ENa);

	real_T fKo = sqrt(Ko / 4);
	real_T IKr = GKr * fKo * OHerg * (V - EK);

	real_T IKs = GKs * (xKs * xKs) * (V - EKs);

	real_T IKv43 = GKv43 * OKv43 * (V - EK);

	real_T a1 = Ki * exp(VF_over_RT) - Ko;
	real_T a2 = exp(VF_over_RT) - 1;
	real_T IKv14_K = PKv14 * OKv14 * VFsq_over_RT * (a1 / a2);
	a1 = Nai * exp(VF_over_RT) - Nao;
	real_T IKv14_Na = 0.02 * PKv14 * OKv14 * VFsq_over_RT * (a1 / a2);
	real_T IKv14 = IKv14_K + IKv14_Na;
	real_T Ito1 = IKv43 + IKv14;

	real_T K1_inf = 1 / (0.94 + exp(1.76 / RT_over_F * (V - EK)));
	real_T IK1 = GK1 * Ko / (Ko + KmK1) * K1_inf * (V - EK);

	real_T INab = GNab * (V - ENa);

	real_T KpV = 1 / (1 + exp((7.488 - V) / 5.98));
	real_T IKp = GKp * KpV * (V - EK);

	real_T sigma = (exp(Nao / 67.3) - 1) / 7;
	a1 = 1 + 0.1245 * exp(-0.1 * VF_over_RT);
	a2 = 0.0365 * sigma * exp(-VF_over_RT);
	real_T fNaK = 1 / (a1 + a2);
	a1 = Ko / (Ko + KmKo);
	a2 = 1 + (KmNai / Nai) * (KmNai / Nai);
	real_T INaK = INaKmax * fNaK * (a1 / a2);

	a1 = exp(eta * VF_over_RT) * Nai * Nai * Nai * Cao;
	a2 = exp((eta - 1) * VF_over_RT) * Nao * Nao * Nao * Cai;
	real_T a3 = 1 + ksat * exp((eta - 1) * VF_over_RT);
	real_T a4 = KmCa + Cao;
	real_T a5 = 5000 / (KmNa * KmNa * KmNa + Nao * Nao * Nao);
	real_T INaCa = kNaCa * a5 * (a1 - a2) / (a4 * a3);

	real_T ICab = GCab * (V - ECa);

	real_T IpCa = IpCamax * Cai / (KmpCa + Cai);

	real_T alpha_m, alpha_h, beta_h, alpha_j, beta_j;
	if (fabs(V + 47.13) <= 1e-3)
		alpha_m = 0.32 / (0.1 - 0.005 * (V + 47.13));
	else
		alpha_m = 0.32 * (V + 47.13) / (1 - exp(-0.1 * (V + 47.13)));

	real_T beta_m = 0.08 * exp(-V / 11);
	if (V < -40) {
		alpha_h = 0.135 * exp((80 + V) / -6.8);
		beta_h = 3.56 * exp(0.079 * V) + 310000 * exp(0.35 * V);
		a1 = -127140 * exp(0.2444 * V);
		a2 = 3.474e-5 * exp(-0.04391 * V);
		a3 = 1 + exp(0.311 * (V + 79.23));
		alpha_j = (a1 - a2) * (V + 37.78) / a3;
		a2 = 1 + exp(-0.1378 * (V + 40.14));
		beta_j = 0.1212 * exp(-0.01052 * V) / a2;
	} else {
		alpha_h = 0;
		beta_h = 1 / (0.13 * (1 + exp((V + 10.66) / -11.1)));
		alpha_j = 0;
		a1 = 1 + exp(-0.1 * (V + 32));
		beta_j = 0.3 * exp(-2.535e-7 * V) / a1;
	}
	real_T dmNa = alpha_m * (1 - mNa) - beta_m * mNa;
	real_T dhNa = alpha_h * (1 - hNa) - beta_h * hNa;
	real_T djNa = alpha_j * (1 - jNa) - beta_j * jNa;

	real_T C1H_to_C2H = T_Const_HERG * A0_HERG * exp(B0_HERG * V);
	real_T C2H_to_C1H = T_Const_HERG * A1_HERG * exp(B1_HERG * V);
	real_T C3H_to_OH = T_Const_HERG * A2_HERG * exp(B2_HERG * V);
	real_T OH_to_C3H = T_Const_HERG * A3_HERG * exp(B3_HERG * V);
	real_T OH_to_IH = T_Const_HERG * A4_HERG * exp(B4_HERG * V);
	real_T IH_to_OH = T_Const_HERG * A5_HERG * exp(B5_HERG * V);
	real_T C3H_to_IH = T_Const_HERG * A6_HERG * exp(B6_HERG * V);
	real_T IH_to_C3H = (OH_to_C3H * IH_to_OH * C3H_to_IH)
			/ (C3H_to_OH * OH_to_IH);
	real_T dC1Herg = C2H_to_C1H * C2Herg - C1H_to_C2H * C1Herg;
	a1 = C1H_to_C2H * C1Herg + C3H_to_C2H * C3Herg;
	a2 = (C2H_to_C1H + C2H_to_C3H) * C2Herg;
	real_T dC2Herg = a1 - a2;
	a1 = C2H_to_C3H * C2Herg + OH_to_C3H * OHerg + IH_to_C3H * IHerg;
	a2 = (C3H_to_IH + C3H_to_OH + C3H_to_C2H) * C3Herg;
	real_T dC3Herg = a1 - a2;
	a1 = C3H_to_OH * C3Herg + IH_to_OH * IHerg;
	a2 = (OH_to_C3H + OH_to_IH) * OHerg;
	real_T dOHerg = a1 - a2;
	a1 = C3H_to_IH * C3Herg + OH_to_IH * OHerg;
	a2 = (IH_to_C3H + IH_to_OH) * IHerg;
	real_T dIHerg = a1 - a2;

	real_T xKs_inf = 1 / (1 + exp(-(V - 24.7) / 13.6));
	a1 = 7.19e-5 * (V - 10) / (1 - exp(-0.148 * (V - 10)));
	a2 = 1.31e-4 * (V - 10) / (exp(0.0687 * (V - 10)) - 1);
	real_T tau_xKs = 1 / (a1 + a2);
	real_T dxKs = (xKs_inf - xKs) / tau_xKs;

	real_T fb = pow(Cai / Kfb, Nfb);
	real_T rb = pow(CaSR / Krb, Nrb);
	real_T Jup = KSR * (vmaxf * fb - vmaxr * rb) / (1 + fb + rb);

	real_T dLTRPNCa = kltrpn_plus * Cai * (1 - LTRPNCa)
			- kltrpn_minus * LTRPNCa;
	real_T dHTRPNCa = khtrpn_plus * Cai * (1 - HTRPNCa)
			- khtrpn_minus * HTRPNCa;
	real_T Jtrpn = LTRPNtot * dLTRPNCa + HTRPNtot * dHTRPNCa;

	real_T beta_i = 1
			/ (1 + CMDNtot * KmCMDN / pow(Cai + KmCMDN, 2)
					+ EGTAtot * KmEGTA / pow(Cai + KmEGTA, 2));
	real_T beta_SR = 1 / (1 + CSQNtot * KmCSQN / pow(CaSR + KmCSQN, 2));

	a1 = Csa / (Vmyo * Faraday * 1000);
	real_T dNai = -(INa + INab + 3 * (INaCa + INaK) + IKv14_Na) * a1;

	a3 = IKr + IKs + IK1 + IKp;
	real_T dKi = -(a3 - 2 * INaK + Ito1) * a1;

	a3 = ICab - 2 * INaCa + IpCa;
	real_T dCai = beta_i * (Jss2b * VSS / Vmyo - Jup - Jtrpn - a3 * 0.5 * a1);

	real_T dCaSR = beta_SR * (Jup * Vmyo / VSR - JRyR * VSS / VSR);

	a1 = INa + ICaL + IKr + IKs;
	a2 = IK1 + IKp + INaCa + INaK + Ito1 + Ito2;
	a3 = IpCa + ICab + INab;
	real_T Itot = a1 + a2 + a3 + Istim;
	real_T dV = -Itot;

	real_T alpha_act43 = alphaa0Kv43 * exp(aaKv43 * V);
	real_T beta_act43 = betaa0Kv43 * exp(-baKv43 * V);
	real_T alpha_inact43 = alphai0Kv43 * exp(-aiKv43 * V);
	real_T beta_inact43 = betai0Kv43 * exp(biKv43 * V);

	real_T C0Kv43_to_C1Kv43 = 4 * alpha_act43;
	real_T C1Kv43_to_C2Kv43 = 3 * alpha_act43;
	real_T C2Kv43_to_C3Kv43 = 2 * alpha_act43;
	real_T C3Kv43_to_OKv43 = alpha_act43;

	real_T CI0Kv43_to_CI1Kv43 = 4 * b1Kv43 * alpha_act43;
	real_T CI1Kv43_to_CI2Kv43 = 3 * b2Kv43 * alpha_act43 / b1Kv43;
	real_T CI2Kv43_to_CI3Kv43 = 2 * b3Kv43 * alpha_act43 / b2Kv43;
	real_T CI3Kv43_to_OIKv43 = b4Kv43 * alpha_act43 / b3Kv43;

	real_T C1Kv43_to_C0Kv43 = beta_act43;
	real_T C2Kv43_to_C1Kv43 = 2 * beta_act43;
	real_T C3Kv43_to_C2Kv43 = 3 * beta_act43;
	real_T OKv43_to_C3Kv43 = 4 * beta_act43;

	real_T CI1Kv43_to_CI0Kv43 = beta_act43 / f1Kv43;
	real_T CI2Kv43_to_CI1Kv43 = 2 * f1Kv43 * beta_act43 / f2Kv43;
	real_T CI3Kv43_to_CI2Kv43 = 3 * f2Kv43 * beta_act43 / f3Kv43;
	real_T OIKv43_to_CI3Kv43 = 4 * f3Kv43 * beta_act43 / f4Kv43;

	real_T C0Kv43_to_CI0Kv43 = beta_inact43;
	real_T C1Kv43_to_CI1Kv43 = f1Kv43 * beta_inact43;
	real_T C2Kv43_to_CI2Kv43 = f2Kv43 * beta_inact43;
	real_T C3Kv43_to_CI3Kv43 = f3Kv43 * beta_inact43;
	real_T OKv43_to_OIKv43 = f4Kv43 * beta_inact43;

	real_T CI0Kv43_to_C0Kv43 = alpha_inact43;
	real_T CI1Kv43_to_C1Kv43 = alpha_inact43 / b1Kv43;
	real_T CI2Kv43_to_C2Kv43 = alpha_inact43 / b2Kv43;
	real_T CI3Kv43_to_C3Kv43 = alpha_inact43 / b3Kv43;
	real_T OIKv43_to_OKv43 = alpha_inact43 / b4Kv43;

	a1 = (C0Kv43_to_C1Kv43 + C0Kv43_to_CI0Kv43) * C0Kv43;
	a2 = C1Kv43_to_C0Kv43 * C1Kv43 + CI0Kv43_to_C0Kv43 * CI0Kv43;
	real_T dC0Kv43 = a2 - a1;

	a1 = (C1Kv43_to_C2Kv43 + C1Kv43_to_C0Kv43 + C1Kv43_to_CI1Kv43) * C1Kv43;
	a2 = C2Kv43_to_C1Kv43 * C2Kv43 + CI1Kv43_to_C1Kv43 * CI1Kv43
			+ C0Kv43_to_C1Kv43 * C0Kv43;
	real_T dC1Kv43 = a2 - a1;

	a1 = (C2Kv43_to_C3Kv43 + C2Kv43_to_C1Kv43 + C2Kv43_to_CI2Kv43) * C2Kv43;
	a2 = C3Kv43_to_C2Kv43 * C3Kv43 + CI2Kv43_to_C2Kv43 * CI2Kv43
			+ C1Kv43_to_C2Kv43 * C1Kv43;
	real_T dC2Kv43 = a2 - a1;

	a1 = (C3Kv43_to_OKv43 + C3Kv43_to_C2Kv43 + C3Kv43_to_CI3Kv43) * C3Kv43;
	a2 = OKv43_to_C3Kv43 * OKv43 + CI3Kv43_to_C3Kv43 * CI3Kv43
			+ C2Kv43_to_C3Kv43 * C2Kv43;
	real_T dC3Kv43 = a2 - a1;

	a1 = (OKv43_to_C3Kv43 + OKv43_to_OIKv43) * OKv43;
	a2 = C3Kv43_to_OKv43 * C3Kv43 + OIKv43_to_OKv43 * OIKv43;
	real_T dOKv43 = a2 - a1;

	a1 = (CI0Kv43_to_C0Kv43 + CI0Kv43_to_CI1Kv43) * CI0Kv43;
	a2 = C0Kv43_to_CI0Kv43 * C0Kv43 + CI1Kv43_to_CI0Kv43 * CI1Kv43;
	real_T dCI0Kv43 = a2 - a1;

	a1 = (CI1Kv43_to_CI2Kv43 + CI1Kv43_to_C1Kv43 + CI1Kv43_to_CI0Kv43)
			* CI1Kv43;
	a2 = CI2Kv43_to_CI1Kv43 * CI2Kv43 + C1Kv43_to_CI1Kv43 * C1Kv43
			+ CI0Kv43_to_CI1Kv43 * CI0Kv43;
	real_T dCI1Kv43 = a2 - a1;

	a1 = (CI2Kv43_to_CI3Kv43 + CI2Kv43_to_C2Kv43 + CI2Kv43_to_CI1Kv43)
			* CI2Kv43;
	a2 = CI3Kv43_to_CI2Kv43 * CI3Kv43 + C2Kv43_to_CI2Kv43 * C2Kv43
			+ CI1Kv43_to_CI2Kv43 * CI1Kv43;
	real_T dCI2Kv43 = a2 - a1;

	a1 = (CI3Kv43_to_OIKv43 + CI3Kv43_to_C3Kv43 + CI3Kv43_to_CI2Kv43) * CI3Kv43;
	a2 = OIKv43_to_CI3Kv43 * OIKv43 + C3Kv43_to_CI3Kv43 * C3Kv43
			+ CI2Kv43_to_CI3Kv43 * CI2Kv43;
	real_T dCI3Kv43 = a2 - a1;

	a1 = (OIKv43_to_OKv43 + OIKv43_to_CI3Kv43) * OIKv43;
	a2 = OKv43_to_OIKv43 * OKv43 + CI3Kv43_to_OIKv43 * CI3Kv43;
	real_T dOIKv43 = a2 - a1;

	real_T alpha_act14 = alphaa0Kv14 * exp(aaKv14 * V);
	real_T beta_act14 = betaa0Kv14 * exp(-baKv14 * V);
	real_T alpha_inact14 = alphai0Kv14;
	real_T beta_inact14 = betai0Kv14;

	real_T C0Kv14_to_C1Kv14 = 4 * alpha_act14;
	real_T C1Kv14_to_C2Kv14 = 3 * alpha_act14;
	real_T C2Kv14_to_C3Kv14 = 2 * alpha_act14;
	real_T C3Kv14_to_OKv14 = alpha_act14;

	real_T CI0Kv14_to_CI1Kv14 = 4 * b1Kv14 * alpha_act14;
	real_T CI1Kv14_to_CI2Kv14 = 3 * b2Kv14 * alpha_act14 / b1Kv14;
	real_T CI2Kv14_to_CI3Kv14 = 2 * b3Kv14 * alpha_act14 / b2Kv14;
	real_T CI3Kv14_to_OIKv14 = b4Kv14 * alpha_act14 / b3Kv14;

	real_T C1Kv14_to_C0Kv14 = beta_act14;
	real_T C2Kv14_to_C1Kv14 = 2 * beta_act14;
	real_T C3Kv14_to_C2Kv14 = 3 * beta_act14;
	real_T OKv14_to_C3Kv14 = 4 * beta_act14;

	real_T CI1Kv14_to_CI0Kv14 = beta_act14 / f1Kv14;
	real_T CI2Kv14_to_CI1Kv14 = 2 * f1Kv14 * beta_act14 / f2Kv14;
	real_T CI3Kv14_to_CI2Kv14 = 3 * f2Kv14 * beta_act14 / f3Kv14;
	real_T OIKv14_to_CI3Kv14 = 4 * f3Kv14 * beta_act14 / f4Kv14;

	real_T C0Kv14_to_CI0Kv14 = beta_inact14;
	real_T C1Kv14_to_CI1Kv14 = f1Kv14 * beta_inact14;
	real_T C2Kv14_to_CI2Kv14 = f2Kv14 * beta_inact14;
	real_T C3Kv14_to_CI3Kv14 = f3Kv14 * beta_inact14;
	real_T OKv14_to_OIKv14 = f4Kv14 * beta_inact14;

	real_T CI0Kv14_to_C0Kv14 = alpha_inact14;
	real_T CI1Kv14_to_C1Kv14 = alpha_inact14 / b1Kv14;
	real_T CI2Kv14_to_C2Kv14 = alpha_inact14 / b2Kv14;
	real_T CI3Kv14_to_C3Kv14 = alpha_inact14 / b3Kv14;
	real_T OIKv14_to_OKv14 = alpha_inact14 / b4Kv14;

	a1 = (C0Kv14_to_C1Kv14 + C0Kv14_to_CI0Kv14) * C0Kv14;
	a2 = C1Kv14_to_C0Kv14 * C1Kv14 + CI0Kv14_to_C0Kv14 * CI0Kv14;
	real_T dC0Kv14 = a2 - a1;

	a1 = (C1Kv14_to_C2Kv14 + C1Kv14_to_C0Kv14 + C1Kv14_to_CI1Kv14) * C1Kv14;
	a2 = C2Kv14_to_C1Kv14 * C2Kv14 + CI1Kv14_to_C1Kv14 * CI1Kv14
			+ C0Kv14_to_C1Kv14 * C0Kv14;
	real_T dC1Kv14 = a2 - a1;

	a1 = (C2Kv14_to_C3Kv14 + C2Kv14_to_C1Kv14 + C2Kv14_to_CI2Kv14) * C2Kv14;
	a2 = C3Kv14_to_C2Kv14 * C3Kv14 + CI2Kv14_to_C2Kv14 * CI2Kv14
			+ C1Kv14_to_C2Kv14 * C1Kv14;
	real_T dC2Kv14 = a2 - a1;

	a1 = (C3Kv14_to_OKv14 + C3Kv14_to_C2Kv14 + C3Kv14_to_CI3Kv14) * C3Kv14;
	a2 = OKv14_to_C3Kv14 * OKv14 + CI3Kv14_to_C3Kv14 * CI3Kv14
			+ C2Kv14_to_C3Kv14 * C2Kv14;
	real_T dC3Kv14 = a2 - a1;

	a1 = (OKv14_to_C3Kv14 + OKv14_to_OIKv14) * OKv14;
	a2 = C3Kv14_to_OKv14 * C3Kv14 + OIKv14_to_OKv14 * OIKv14;
	real_T dOKv14 = a2 - a1;

	a1 = (CI0Kv14_to_C0Kv14 + CI0Kv14_to_CI1Kv14) * CI0Kv14;
	a2 = C0Kv14_to_CI0Kv14 * C0Kv14 + CI1Kv14_to_CI0Kv14 * CI1Kv14;
	real_T dCI0Kv14 = a2 - a1;

	a1 = (CI1Kv14_to_CI2Kv14 + CI1Kv14_to_C1Kv14 + CI1Kv14_to_CI0Kv14)
			* CI1Kv14;
	a2 = CI2Kv14_to_CI1Kv14 * CI2Kv14 + C1Kv14_to_CI1Kv14 * C1Kv14
			+ CI0Kv14_to_CI1Kv14 * CI0Kv14;
	real_T dCI1Kv14 = a2 - a1;

	a1 = (CI2Kv14_to_CI3Kv14 + CI2Kv14_to_C2Kv14 + CI2Kv14_to_CI1Kv14)
			* CI2Kv14;
	a2 = CI3Kv14_to_CI2Kv14 * CI3Kv14 + C2Kv14_to_CI2Kv14 * C2Kv14
			+ CI1Kv14_to_CI2Kv14 * CI1Kv14;
	real_T dCI2Kv14 = a2 - a1;

	a1 = (CI3Kv14_to_OIKv14 + CI3Kv14_to_C3Kv14 + CI3Kv14_to_CI2Kv14) * CI3Kv14;
	a2 = OIKv14_to_CI3Kv14 * OIKv14 + C3Kv14_to_CI3Kv14 * C3Kv14
			+ CI2Kv14_to_CI3Kv14 * CI2Kv14;
	real_T dCI3Kv14 = a2 - a1;

	a1 = (OIKv14_to_OKv14 + OIKv14_to_CI3Kv14) * OIKv14;
	a2 = OKv14_to_OIKv14 * OKv14 + CI3Kv14_to_OIKv14 * CI3Kv14;
	real_T dOIKv14 = a2 - a1;

	//   Assign global state derviative to derivative array
	if (Vclamp_flag)
		dy[INDEX_V] = 0;
	else
		dy[INDEX_V] = dV;

	dy[INDEX_mNa] = dmNa;
	dy[INDEX_hNa] = dhNa;
	dy[INDEX_jNa] = djNa;
	dy[INDEX_Nai] = dNai;
	dy[INDEX_Ki] = dKi;
	dy[INDEX_Cai] = dCai;
	dy[INDEX_CaSR] = dCaSR;
	dy[INDEX_LTRPNCa] = dLTRPNCa;
	dy[INDEX_HTRPNCa] = dHTRPNCa;
	dy[INDEX_xKs] = dxKs;
	dy[INDEX_C0Kv43] = dC0Kv43;
	dy[INDEX_C1Kv43] = dC1Kv43;
	dy[INDEX_C2Kv43] = dC2Kv43;
	dy[INDEX_C3Kv43] = dC3Kv43;
	dy[INDEX_OKv43] = dOKv43;
	dy[INDEX_CI0Kv43] = dCI0Kv43;
	dy[INDEX_CI1Kv43] = dCI1Kv43;
	dy[INDEX_CI2Kv43] = dCI2Kv43;
	dy[INDEX_CI3Kv43] = dCI3Kv43;
	dy[INDEX_OIKv43] = dOIKv43;
	dy[INDEX_C0Kv14] = dC0Kv14;
	dy[INDEX_C1Kv14] = dC1Kv14;
	dy[INDEX_C2Kv14] = dC2Kv14;
	dy[INDEX_C3Kv14] = dC3Kv14;
	dy[INDEX_OKv14] = dOKv14;
	dy[INDEX_CI0Kv14] = dCI0Kv14;
	dy[INDEX_CI1Kv14] = dCI1Kv14;
	dy[INDEX_CI2Kv14] = dCI2Kv14;
	dy[INDEX_CI3Kv14] = dCI3Kv14;
	dy[INDEX_OIKv14] = dOIKv14;
	dy[INDEX_C1Herg] = dC1Herg;
	dy[INDEX_C2Herg] = dC2Herg;
	dy[INDEX_C3Herg] = dC3Herg;
	dy[INDEX_OHerg] = dOHerg;
	dy[INDEX_IHerg] = dIHerg;

	//Store currents/fluxes for output
	params[NUM_PARAMS+0] = ICaL;
	params[NUM_PARAMS+1] = JCaL;
	params[NUM_PARAMS+2] = JRyR;
	params[NUM_PARAMS+3] = CaSSavg;

	return 0;
}

/* End of code generation (fcn_40state.c) */
