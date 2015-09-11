/*
 * fcn_40state.h
 *
 * Code generation for function 'fcn_40state'
 *
 * C source code generated on: Mon Mar  3 11:45:45 2014
 *
 */

#ifndef __FCN_40STATE_H__
#define __FCN_40STATE_H__
/* Include files */
#include <math.h>
#include <stddef.h>
#include <stdlib.h>

#include "fcn_40state_types.h"

#define min(a,b) ((a < b) ? a : b)
#define max(a,b) ((a > b) ? a : b)

/* Function Declarations */
extern void fcn_40state(real_T t, const real_T y[76], real_T dy[76]);

enum state_indexes {
	INDEX_STATE_1 = 0,
	INDEX_STATE_2,
	INDEX_STATE_3,
	INDEX_STATE_4,
	INDEX_STATE_5,
	INDEX_STATE_6,
	INDEX_STATE_7,
	INDEX_STATE_8,
	INDEX_STATE_9,
	INDEX_STATE_10,
	INDEX_STATE_11,
	INDEX_STATE_12,
	INDEX_STATE_13,
	INDEX_STATE_14,
	INDEX_STATE_15,
	INDEX_STATE_16,
	INDEX_STATE_17,
	INDEX_STATE_18,
	INDEX_STATE_19,
	INDEX_STATE_20,
	INDEX_STATE_21,
	INDEX_STATE_22,
	INDEX_STATE_23,
	INDEX_STATE_24,
	INDEX_STATE_25,
	INDEX_STATE_26,
	INDEX_STATE_27,
	INDEX_STATE_28,
	INDEX_STATE_29,
	INDEX_STATE_30,
	INDEX_STATE_31,
	INDEX_STATE_32,
	INDEX_STATE_33,
	INDEX_STATE_34,
	INDEX_STATE_35,
	INDEX_STATE_36,
	INDEX_STATE_37,
	INDEX_STATE_38,
	INDEX_STATE_39,
	INDEX_STATE_40,
	INDEX_V,
	INDEX_mNa,
	INDEX_hNa,
	INDEX_jNa,
	INDEX_Nai,
	INDEX_Ki,
	INDEX_Cai,
	INDEX_CaSR,
	INDEX_LTRPNCa,
	INDEX_HTRPNCa,
	INDEX_xKs,
	INDEX_C0Kv43,
	INDEX_C1Kv43,
	INDEX_C2Kv43,
	INDEX_C3Kv43,
	INDEX_OKv43,
	INDEX_CI0Kv43,
	INDEX_CI1Kv43,
	INDEX_CI2Kv43,
	INDEX_CI3Kv43,
	INDEX_OIKv43,
	INDEX_C0Kv14,
	INDEX_C1Kv14,
	INDEX_C2Kv14,
	INDEX_C3Kv14,
	INDEX_OKv14,
	INDEX_CI0Kv14,
	INDEX_CI1Kv14,
	INDEX_CI2Kv14,
	INDEX_CI3Kv14,
	INDEX_OIKv14,
	INDEX_C1Herg,
	INDEX_C2Herg,
	INDEX_C3Herg,
	INDEX_OHerg,
	INDEX_IHerg,

	NUM_STATES
};

enum param_indexes {
	INDEX_Vclamp_flag = 0,
	INDEX_Period,
	INDEX_Shift,
	INDEX_Pulse_duration,
	INDEX_Pulse_amplitude,
	INDEX_Num_beats,
	INDEX_t_clamp_start,
	INDEX_t_clamp_end,
	INDEX_t_sim_end,
	INDEX_V_clamp,
	INDEX_V_hold,
	INDEX_DTOUT,
	INDEX_t_output_start,
	INDEX_NCaRU, //   Number of Ca Release Units

	INDEX_Faraday, //   Faraday's constant (C/mmol)
	INDEX_Temp, //   Temperature (K)
	INDEX_Rgas, //   Universal Gas constant (J/[mol K])
	INDEX_Csa, //   Membrane capacitance (pF)
	INDEX_VSS, //   Single dyad subspace volume (microL)
	INDEX_VSR, //   SR volume (microL)
	INDEX_Vmyo, //   Myoplasmic volume (microL)
	INDEX_PCa, //   LCC single channel permeability (cm^3/s)
	INDEX_JRyRmax, //   Ca flux rate through open RyR (1/ms)
	INDEX_r_xfer, //   Ca flux rate from SS to myoplasm (1/ms)
	INDEX_Cao, //   Extracellular [Ca] (mM)
	INDEX_Ko , //   Extracellular [K] (mM)
	INDEX_Nao, //   Extracellular [Na] (mM)
	INDEX_Clo, //   Extracellular [Cl] (mM)
	INDEX_Cli, //   Intracellular [Cl] (mM)

	INDEX_KdIto2, //   Dissociation constant of Cl channel (Ito2) for Ca (mM)
	INDEX_PCl, //   Cl single channel permeability (cm^3/s)

	INDEX_GKr, //   Maximal conductance of IKr (mS/microF)
	INDEX_GKs, //   Maximal conductance of IKs (mS/microF)
	INDEX_GK1, //   Maximal conductance of IK1 (mS/microF)
	INDEX_GKp, //   Maximal conductance of IKp (mS/microF)
	INDEX_GNa, //   Maximal conductance of INa (mS/microF)
	INDEX_kNaCa, //   Scale factor for Na/Ca exchanger (A/F)
	INDEX_KmNa, //   Na half saturation constant for Na/Ca exchanger (mM)
	INDEX_KmCa, //   Ca half saturation constant for Na/Ca exchanger (mM)
	INDEX_KmK1, //   K half saturation constant for IK1 (mM)
	INDEX_ksat, //   Na/Ca exchange saturation factor at negative potentials
	INDEX_eta, //   Controls voltage dependence of Na/Ca exchange
	INDEX_INaKmax, //   Scale factor for Na/K pump (A/F)
	INDEX_KmNai, //   Na half saturation constant for Na/K pump (mM)
	INDEX_KmKo, //   K half saturation constant for Na/K pump (mM)
	INDEX_IpCamax, //   Maximum sarcolemmal Ca pump current (A/F)
	INDEX_KmpCa, //   Ca half saturation constant for sarcolemmal Ca pump (mM)
	INDEX_GCab, //   Maximal conductance of ICab (mS/microF)
	INDEX_GNab, //   Maximal conductance of INab (mS/microF)

	INDEX_Kfb, //   Forward half-saturation constant for SR Ca ATPase (mM)
	INDEX_Krb, //   Reverse half-saturation constant for SR Ca ATPase (mM)
	INDEX_KSR, //   Scale factor for SR Ca ATPase
	INDEX_Nfb, //   Forward cooperativity constant for SR Ca ATPase
	INDEX_Nrb, //   Reverse cooperativity constant for SR Ca ATPase
	INDEX_vmaxf, //   SR Ca ATPase forward rate parameter (mM/ms)
	INDEX_vmaxr, //   SR Ca ATPase reverse rate parameter (mM/ms)

	INDEX_KvScale, //   Scale factor for Kv4.3 and Kv1.4 currents
	INDEX_Kv43Frac, //   Fraction of Ito1 (peak current at 40 mV) that is made up of Kv4.3
	INDEX_alphaa0Kv43, //   IKv43 rate parameter (1/ms)
	INDEX_aaKv43, //   IKv43 rate parameter (1/mV)
	INDEX_betaa0Kv43, //   IKv43 rate parameter (1/ms)
	INDEX_baKv43, //   IKv43 rate parameter (1/mV)
	INDEX_alphai0Kv43, //   IKv43 rate parameter (1/ms)
	INDEX_aiKv43, //   IKv43 rate parameter (1/mV)
	INDEX_betai0Kv43, //   IKv43 rate parameter (1/ms)
	INDEX_biKv43, //   IKv43 rate parameter (1/mV)
	INDEX_alphaa0Kv14, //   IKv14 rate parameter (1/ms)
	INDEX_aaKv14, //   IKv14 rate parameter (1/mV)
	INDEX_betaa0Kv14, //   IKv14 rate parameter (1/ms)
	INDEX_baKv14, //   IKv14 rate parameter (1/mV)
	INDEX_alphai0Kv14, //   IKv14 rate parameter (1/ms)
	INDEX_betai0Kv14, //   IKv14 rate parameter (1/ms)
	INDEX_f1Kv43, //   IKv43 rate parameter
	INDEX_f2Kv43, //   IKv43 rate parameter
	INDEX_f3Kv43, //   IKv43 rate parameter
	INDEX_f4Kv43 , //   IKv43 rate parameter
	INDEX_b1Kv43, //   IKv43 rate parameter
	INDEX_b2Kv43, //   IKv43 rate parameter
	INDEX_b3Kv43, //   IKv43 rate parameter
	INDEX_b4Kv43, //   IKv43 rate parameter
	INDEX_f1Kv14, //   IKv14 rate parameter
	INDEX_f2Kv14, //   IKv14 rate parameter
	INDEX_f3Kv14, //   IKv14 rate parameter
	INDEX_f4Kv14, //   IKv14 rate parameter
	INDEX_b1Kv14, //   IKv14 rate parameter
	INDEX_b2Kv14, //   IKv14 rate parameter
	INDEX_b3Kv14, //   IKv14 rate parameter
	INDEX_b4Kv14, //   IKv14 rate parameter

	INDEX_A0_HERG, //   IKr rate parameter (1/ms)
	INDEX_B0_HERG, //   IKr rate parameter (1/mV)
	INDEX_A1_HERG, //   IKr rate parameter (1/ms)
	INDEX_B1_HERG, //   IKr rate parameter (1/mV)
	INDEX_A2_HERG, //   IKr rate parameter (1/ms)
	INDEX_B2_HERG, //   IKr rate parameter (1/mV)
	INDEX_A3_HERG, //   IKr rate parameter (1/ms)
	INDEX_B3_HERG, //   IKr rate parameter (1/mV)
	INDEX_A4_HERG, //   IKr rate parameter (1/ms)
	INDEX_B4_HERG, //   IKr rate parameter (1/mV)
	INDEX_A5_HERG, //   IKr rate parameter (1/ms)
	INDEX_B5_HERG, //   IKr rate parameter (1/mV)
	INDEX_A6_HERG, //   IKr rate parameter (1/ms)
	INDEX_B6_HERG, //   IKr rate parameter (1/mV)
	INDEX_T_Const_HERG, //   IKr rate scale factor for temperature

	INDEX_CSQNtot, //   Total SR calsequestrin concentration (mM)
	INDEX_KmCSQN, //   Ca half saturation constant for calsequestrin (mM)
	INDEX_LTRPNtot, //   Total troponin low-affinity site concentration (mM)
	INDEX_HTRPNtot, //   Total troponin high-affinity site concentration (mM)
	INDEX_khtrpn_plus, //   Troponin Ca binding rate parameter (1/[mM ms])
	INDEX_khtrpn_minus, //   Troponin Ca binding rate parameter (1/ms)
	INDEX_kltrpn_plus, //   Troponin Ca binding rate parameter (1/[mM ms])
	INDEX_kltrpn_minus, //   Troponin Ca binding rate parameter (1/ms)
	INDEX_CMDNtot, //   Total myoplasmic calmodulin concentration (mM)
	INDEX_EGTAtot, //   Total myoplasmic EGTA concentration (mM)
	INDEX_KmCMDN, //   Ca half saturation constant for calmodulin (mM)
	INDEX_KmEGTA, //   Ca half saturation constant for EGTA (mM)

	INDEX_fL, //   L-type Ca channel rate parameter (1/ms)
	INDEX_gL, //   L-type Ca channel rate parameter (1/ms)
	INDEX_bL, //   L-type Ca channel rate parameter
	INDEX_aL, //   L-type Ca channel rate parameter
	INDEX_omega, //   L-type Ca channel rate parameter (1/ms)
	INDEX_gammacf, //   L-type Ca channel rate parameter (1/[mM ms])

	INDEX_k12, //   RyR channel rate parameter (1/ms)
	INDEX_k21, //   RyR channel rate parameter (1/ms)
	INDEX_k23, //   RyR channel rate parameter (1/ms)
	INDEX_k32, //   RyR channel rate parameter (1/ms)
	INDEX_k34, //   RyR channel rate parameter (1/ms)
	INDEX_k43, //   RyR channel rate parameter (1/ms)
	INDEX_k45, //   RyR channel rate parameter (1/ms)
	INDEX_k54, //   RyR channel rate parameter (1/ms)
	INDEX_k56, //   RyR channel rate parameter (1/ms)
	INDEX_k65, //   RyR channel rate parameter (1/ms)
	INDEX_k25, //   RyR channel rate parameter (1/ms)
	INDEX_k52, //   RyR channel rate parameter (1/ms)

	INDEX_I_STIM, //   Stimulus current

	NUM_PARAMS
};



#endif
/* End of code generation (fcn_40state.h) */
