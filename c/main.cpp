#include<string.h>
#include<stdio.h>
#include<iostream>

using namespace std;

#include "fcn_40state.h"

#include <cvodes/cvodes.h> /* main integrator header file */
#include <cvodes/cvodes_dense.h>   /* use CVDENSE linear solver */
#include <cvodes/cvodes_band.h>/* use CVBAND linear solver */
#include <cvodes/cvodes_diag.h>/* use CVDIAG linear solver */
#include <nvector/nvector_serial.h>  /* serial N_Vector types, fct. and macros */
#include <sundials/sundials_types.h> /* definition of realtype */
#include <sundials/sundials_math.h>  /* contains the macros ABS, SQR, and EXP*/

/* Shared Problem Constants */

#define ATOL RCONST(1.0e-6)
#define RTOL RCONST(1.0e-6)
#define MAX_STEP_SIZE RCONST(0.1)
#define MAX_NCF 100

/* Private Helper Functions */

void checkopen(FILE* f, char filename[]);
void readparam_double(char filename[], char paramname[], double* param);
void readparam_int(char filename[], char paramname[], int* param);
static void PrintFinalStats(void *cvode_mem, int miter, realtype ero);

extern int fcn_40state(real_T t, N_Vector y, N_Vector dy, void *user_data);

/* Private function to check function return values */

static int check_flag(void *flagvalue, char *funcname, int opt);


int main(int argc, char* argv[]) {

	/* Create output file strings */
	char states_path[1024];
	char fluxes_path[1024];
	sprintf(states_path, "%s", argv[1]);
	sprintf(fluxes_path, "%s", argv[2]);

	/* Opening of Datafiles */
	FILE* fstates = fopen(states_path, "w");
	FILE* ffluxes = fopen(fluxes_path, "w");

	/* Throw error if a file could not be opened */
	checkopen(fstates, states_path);
	checkopen(ffluxes, fluxes_path);

	//Data passed in/out of ODE function - last 4 entries for ICaL, JCaL, JRyR, CaSSavg
	real_T user_data[NUM_PARAMS+4];

	//Initial conditions vector
	realtype y0[NUM_STATES];

	//Read initial conditions
	readparam_double(argv[3], "STATE_1", &y0[INDEX_STATE_1]);
	readparam_double(argv[3], "STATE_2", &y0[INDEX_STATE_2]);
	readparam_double(argv[3], "STATE_3", &y0[INDEX_STATE_3]);
	readparam_double(argv[3], "STATE_4", &y0[INDEX_STATE_4]);
	readparam_double(argv[3], "STATE_5", &y0[INDEX_STATE_5]);
	readparam_double(argv[3], "STATE_6", &y0[INDEX_STATE_6]);
	readparam_double(argv[3], "STATE_7", &y0[INDEX_STATE_7]);
	readparam_double(argv[3], "STATE_8", &y0[INDEX_STATE_8]);
	readparam_double(argv[3], "STATE_9", &y0[INDEX_STATE_9]);
	readparam_double(argv[3], "STATE_10", &y0[INDEX_STATE_10]);
	readparam_double(argv[3], "STATE_11", &y0[INDEX_STATE_11]);
	readparam_double(argv[3], "STATE_12", &y0[INDEX_STATE_12]);
	readparam_double(argv[3], "STATE_13", &y0[INDEX_STATE_13]);
	readparam_double(argv[3], "STATE_14", &y0[INDEX_STATE_14]);
	readparam_double(argv[3], "STATE_15", &y0[INDEX_STATE_15]);
	readparam_double(argv[3], "STATE_16", &y0[INDEX_STATE_16]);
	readparam_double(argv[3], "STATE_17", &y0[INDEX_STATE_17]);
	readparam_double(argv[3], "STATE_18", &y0[INDEX_STATE_18]);
	readparam_double(argv[3], "STATE_19", &y0[INDEX_STATE_19]);
	readparam_double(argv[3], "STATE_20", &y0[INDEX_STATE_20]);
	readparam_double(argv[3], "STATE_21", &y0[INDEX_STATE_21]);
	readparam_double(argv[3], "STATE_22", &y0[INDEX_STATE_22]);
	readparam_double(argv[3], "STATE_23", &y0[INDEX_STATE_23]);
	readparam_double(argv[3], "STATE_24", &y0[INDEX_STATE_24]);
	readparam_double(argv[3], "STATE_25", &y0[INDEX_STATE_25]);
	readparam_double(argv[3], "STATE_26", &y0[INDEX_STATE_26]);
	readparam_double(argv[3], "STATE_27", &y0[INDEX_STATE_27]);
	readparam_double(argv[3], "STATE_28", &y0[INDEX_STATE_28]);
	readparam_double(argv[3], "STATE_29", &y0[INDEX_STATE_29]);
	readparam_double(argv[3], "STATE_30", &y0[INDEX_STATE_30]);
	readparam_double(argv[3], "STATE_31", &y0[INDEX_STATE_31]);
	readparam_double(argv[3], "STATE_32", &y0[INDEX_STATE_32]);
	readparam_double(argv[3], "STATE_33", &y0[INDEX_STATE_33]);
	readparam_double(argv[3], "STATE_34", &y0[INDEX_STATE_34]);
	readparam_double(argv[3], "STATE_35", &y0[INDEX_STATE_35]);
	readparam_double(argv[3], "STATE_36", &y0[INDEX_STATE_36]);
	readparam_double(argv[3], "STATE_37", &y0[INDEX_STATE_37]);
	readparam_double(argv[3], "STATE_38", &y0[INDEX_STATE_38]);
	readparam_double(argv[3], "STATE_39", &y0[INDEX_STATE_39]);
	readparam_double(argv[3], "STATE_40", &y0[INDEX_STATE_40]);
	readparam_double(argv[3], "V", &y0[INDEX_V]);
	readparam_double(argv[3], "mNa", &y0[INDEX_mNa]);
	readparam_double(argv[3], "hNa", &y0[INDEX_hNa]);
	readparam_double(argv[3], "jNa", &y0[INDEX_jNa]);
	readparam_double(argv[3], "Nai", &y0[INDEX_Nai]);
	readparam_double(argv[3], "Ki", &y0[INDEX_Ki]);
	readparam_double(argv[3], "Cai", &y0[INDEX_Cai]);
	readparam_double(argv[3], "CaSR", &y0[INDEX_CaSR]);
	readparam_double(argv[3], "LTRPNCa", &y0[INDEX_LTRPNCa]);
	readparam_double(argv[3], "HTRPNCa", &y0[INDEX_HTRPNCa]);
	readparam_double(argv[3], "xKs", &y0[INDEX_xKs]);
	readparam_double(argv[3], "C0Kv43", &y0[INDEX_C0Kv43]);
	readparam_double(argv[3], "C1Kv43", &y0[INDEX_C1Kv43]);
	readparam_double(argv[3], "C2Kv43", &y0[INDEX_C2Kv43]);
	readparam_double(argv[3], "C3Kv43", &y0[INDEX_C3Kv43]);
	readparam_double(argv[3], "OKv43", &y0[INDEX_OKv43]);
	readparam_double(argv[3], "CI0Kv43", &y0[INDEX_CI0Kv43]);
	readparam_double(argv[3], "CI1Kv43", &y0[INDEX_CI1Kv43]);
	readparam_double(argv[3], "CI2Kv43", &y0[INDEX_CI2Kv43]);
	readparam_double(argv[3], "CI3Kv43", &y0[INDEX_CI3Kv43]);
	readparam_double(argv[3], "OIKv43", &y0[INDEX_OIKv43]);
	readparam_double(argv[3], "C0Kv14", &y0[INDEX_C0Kv14]);
	readparam_double(argv[3], "C1Kv14", &y0[INDEX_C1Kv14]);
	readparam_double(argv[3], "C2Kv14", &y0[INDEX_C2Kv14]);
	readparam_double(argv[3], "C3Kv14", &y0[INDEX_C3Kv14]);
	readparam_double(argv[3], "OKv14", &y0[INDEX_OKv14]);
	readparam_double(argv[3], "CI0Kv14", &y0[INDEX_CI0Kv14]);
	readparam_double(argv[3], "CI1Kv14", &y0[INDEX_CI1Kv14]);
	readparam_double(argv[3], "CI2Kv14", &y0[INDEX_CI2Kv14]);
	readparam_double(argv[3], "CI3Kv14", &y0[INDEX_CI3Kv14]);
	readparam_double(argv[3], "OIKv14", &y0[INDEX_OIKv14]);
	readparam_double(argv[3], "C1Herg", &y0[INDEX_C1Herg]);
	readparam_double(argv[3], "C2Herg", &y0[INDEX_C2Herg]);
	readparam_double(argv[3], "C3Herg", &y0[INDEX_C3Herg]);
	readparam_double(argv[3], "OHerg", &y0[INDEX_OHerg]);
	readparam_double(argv[3], "IHerg", &y0[INDEX_IHerg]);

	//Read parameters
	readparam_double(argv[3], "Vclamp_flag", &user_data[INDEX_Vclamp_flag]);
	readparam_double(argv[3], "Period", &user_data[INDEX_Period]);
	readparam_double(argv[3], "Shift", &user_data[INDEX_Shift]);
	readparam_double(argv[3], "Pulse_duration", &user_data[INDEX_Pulse_duration]);
	readparam_double(argv[3], "Pulse_amplitude", &user_data[INDEX_Pulse_amplitude]);
	readparam_double(argv[3], "Num_beats", &user_data[INDEX_Num_beats]);
	readparam_double(argv[3], "t_clamp_start", &user_data[INDEX_t_clamp_start]);
	readparam_double(argv[3], "t_clamp_end", &user_data[INDEX_t_clamp_end]);
	readparam_double(argv[3], "t_sim_end", &user_data[INDEX_t_sim_end]);
	readparam_double(argv[3], "V_clamp", &user_data[INDEX_V_clamp]);
	readparam_double(argv[3], "V_hold", &user_data[INDEX_V_hold]);
	readparam_double(argv[3], "DTOUT", &user_data[INDEX_DTOUT]);
	readparam_double(argv[3], "t_output_start", &user_data[INDEX_t_output_start]);
	readparam_double(argv[3], "NCaRU", &user_data[INDEX_NCaRU]);

	readparam_double(argv[3], "Faraday", &user_data[INDEX_Faraday]); //   Faraday's constant (C/mmol)
	readparam_double(argv[3], "Temp", &user_data[INDEX_Temp]); //   Temperature (K)
	readparam_double(argv[3], "Rgas", &user_data[INDEX_Rgas]); //   Universal Gas constant (J/[mol K])
	readparam_double(argv[3], "Csa", &user_data[INDEX_Csa]); //   Membrane capacitance (pF)
	readparam_double(argv[3], "VSS", &user_data[INDEX_VSS]); //   Single dyad subspace volume (microL)
	readparam_double(argv[3], "VSR", &user_data[INDEX_VSR]); //   SR volume (microL)
	readparam_double(argv[3], "Vmyo", &user_data[INDEX_Vmyo]); //   Myoplasmic volume (microL)
	readparam_double(argv[3], "PCa", &user_data[INDEX_PCa]); //   LCC single channel permeability (cm^3/s)
	readparam_double(argv[3], "JRyRmax", &user_data[INDEX_JRyRmax]); //   Ca flux rate through open RyR (1/ms)
	readparam_double(argv[3], "r_xfer", &user_data[INDEX_r_xfer]); //   Ca flux rate from SS to myoplasm (1/ms)
	readparam_double(argv[3], "Cao", &user_data[INDEX_Cao]); //   Extracellular [Ca] (mM)
	readparam_double(argv[3], "Ko", &user_data[INDEX_Ko ]); //   Extracellular [K] (mM)
	readparam_double(argv[3], "Nao", &user_data[INDEX_Nao]); //   Extracellular [Na] (mM)
	readparam_double(argv[3], "Clo", &user_data[INDEX_Clo]); //   Extracellular [Cl] (mM)
	readparam_double(argv[3], "Cli", &user_data[INDEX_Cli]); //   Intracellular [Cl] (mM)

	readparam_double(argv[3], "KdIto2", &user_data[INDEX_KdIto2]); //   Dissociation constant of Cl channel (Ito2) for Ca (mM)
	readparam_double(argv[3], "PCl", &user_data[INDEX_PCl]); //   Cl single channel permeability (cm^3/s)

	readparam_double(argv[3], "GKr", &user_data[INDEX_GKr]); //   Maximal conductance of IKr (mS/microF)
	readparam_double(argv[3], "GKs", &user_data[INDEX_GKs]); //   Maximal conductance of IKs (mS/microF)
	readparam_double(argv[3], "GK1", &user_data[INDEX_GK1]); //   Maximal conductance of IK1 (mS/microF)
	readparam_double(argv[3], "GKp", &user_data[INDEX_GKp]); //   Maximal conductance of IKp (mS/microF)
	readparam_double(argv[3], "GNa", &user_data[INDEX_GNa]); //   Maximal conductance of INa (mS/microF)
	readparam_double(argv[3], "kNaCa", &user_data[INDEX_kNaCa]); //   Scale factor for Na/Ca exchanger (A/F)
	readparam_double(argv[3], "KmNa", &user_data[INDEX_KmNa]); //   Na half saturation constant for Na/Ca exchanger (mM)
	readparam_double(argv[3], "KmCa", &user_data[INDEX_KmCa]); //   Ca half saturation constant for Na/Ca exchanger (mM)
	readparam_double(argv[3], "KmK1", &user_data[INDEX_KmK1]); //   K half saturation constant for IK1 (mM)
	readparam_double(argv[3], "ksat", &user_data[INDEX_ksat]); //   Na/Ca exchange saturation factor at negative potentials
	readparam_double(argv[3], "eta", &user_data[INDEX_eta]); //   Controls voltage dependence of Na/Ca exchange
	readparam_double(argv[3], "INaKmax", &user_data[INDEX_INaKmax]); //   Scale factor for Na/K pump (A/F)
	readparam_double(argv[3], "KmNai", &user_data[INDEX_KmNai]); //   Na half saturation constant for Na/K pump (mM)
	readparam_double(argv[3], "KmKo", &user_data[INDEX_KmKo]); //   K half saturation constant for Na/K pump (mM)
	readparam_double(argv[3], "IpCamax", &user_data[INDEX_IpCamax]); //   Maximum sarcolemmal Ca pump current (A/F)
	readparam_double(argv[3], "KmpCa", &user_data[INDEX_KmpCa]); //   Ca half saturation constant for sarcolemmal Ca pump (mM)
	readparam_double(argv[3], "GCab", &user_data[INDEX_GCab]); //   Maximal conductance of ICab (mS/microF)
	readparam_double(argv[3], "GNab", &user_data[INDEX_GNab]); //   Maximal conductance of INab (mS/microF)

	readparam_double(argv[3], "Kfb", &user_data[INDEX_Kfb]); //   Forward half-saturation constant for SR Ca ATPase (mM)
	readparam_double(argv[3], "Krb", &user_data[INDEX_Krb]); //   Reverse half-saturation constant for SR Ca ATPase (mM)
	readparam_double(argv[3], "KSR", &user_data[INDEX_KSR]); //   Scale factor for SR Ca ATPase
	readparam_double(argv[3], "Nfb", &user_data[INDEX_Nfb]); //   Forward cooperativity constant for SR Ca ATPase
	readparam_double(argv[3], "Nrb", &user_data[INDEX_Nrb]); //   Reverse cooperativity constant for SR Ca ATPase
	readparam_double(argv[3], "vmaxf", &user_data[INDEX_vmaxf]); //   SR Ca ATPase forward rate parameter (mM/ms)
	readparam_double(argv[3], "vmaxr", &user_data[INDEX_vmaxr]); //   SR Ca ATPase reverse rate parameter (mM/ms)

	readparam_double(argv[3], "KvScale", &user_data[INDEX_KvScale]); //   Scale factor for Kv4.3 and Kv1.4 currents
	readparam_double(argv[3], "Kv43Frac", &user_data[INDEX_Kv43Frac]); //   Fraction of Ito1 (peak current at 40 mV) that is made up of Kv4.3
	readparam_double(argv[3], "alphaa0Kv43", &user_data[INDEX_alphaa0Kv43]); //   IKv43 rate parameter (1/ms)
	readparam_double(argv[3], "aaKv43", &user_data[INDEX_aaKv43]); //   IKv43 rate parameter (1/mV)
	readparam_double(argv[3], "betaa0Kv43", &user_data[INDEX_betaa0Kv43]); //   IKv43 rate parameter (1/ms)
	readparam_double(argv[3], "baKv43", &user_data[INDEX_baKv43]); //   IKv43 rate parameter (1/mV)
	readparam_double(argv[3], "alphai0Kv43", &user_data[INDEX_alphai0Kv43]); //   IKv43 rate parameter (1/ms)
	readparam_double(argv[3], "aiKv43", &user_data[INDEX_aiKv43]); //   IKv43 rate parameter (1/mV)
	readparam_double(argv[3], "betai0Kv43", &user_data[INDEX_betai0Kv43]); //   IKv43 rate parameter (1/ms)
	readparam_double(argv[3], "biKv43", &user_data[INDEX_biKv43]); //   IKv43 rate parameter (1/mV)
	readparam_double(argv[3], "alphaa0Kv14", &user_data[INDEX_alphaa0Kv14]); //   IKv14 rate parameter (1/ms)
	readparam_double(argv[3], "aaKv14", &user_data[INDEX_aaKv14]); //   IKv14 rate parameter (1/mV)
	readparam_double(argv[3], "betaa0Kv14", &user_data[INDEX_betaa0Kv14]); //   IKv14 rate parameter (1/ms)
	readparam_double(argv[3], "baKv14", &user_data[INDEX_baKv14]); //   IKv14 rate parameter (1/mV)
	readparam_double(argv[3], "alphai0Kv14", &user_data[INDEX_alphai0Kv14]); //   IKv14 rate parameter (1/ms)
	readparam_double(argv[3], "betai0Kv14", &user_data[INDEX_betai0Kv14]); //   IKv14 rate parameter (1/ms)
	readparam_double(argv[3], "f1Kv43", &user_data[INDEX_f1Kv43]); //   IKv43 rate parameter
	readparam_double(argv[3], "f2Kv43", &user_data[INDEX_f2Kv43]); //   IKv43 rate parameter
	readparam_double(argv[3], "f3Kv43", &user_data[INDEX_f3Kv43]); //   IKv43 rate parameter
	readparam_double(argv[3], "f4Kv43", &user_data[INDEX_f4Kv43 ]); //   IKv43 rate parameter
	readparam_double(argv[3], "b1Kv43", &user_data[INDEX_b1Kv43]); //   IKv43 rate parameter
	readparam_double(argv[3], "b2Kv43", &user_data[INDEX_b2Kv43]); //   IKv43 rate parameter
	readparam_double(argv[3], "b3Kv43", &user_data[INDEX_b3Kv43]); //   IKv43 rate parameter
	readparam_double(argv[3], "b4Kv43", &user_data[INDEX_b4Kv43]); //   IKv43 rate parameter
	readparam_double(argv[3], "f1Kv14", &user_data[INDEX_f1Kv14]); //   IKv14 rate parameter
	readparam_double(argv[3], "f2Kv14", &user_data[INDEX_f2Kv14]); //   IKv14 rate parameter
	readparam_double(argv[3], "f3Kv14", &user_data[INDEX_f3Kv14]); //   IKv14 rate parameter
	readparam_double(argv[3], "f4Kv14", &user_data[INDEX_f4Kv14]); //   IKv14 rate parameter
	readparam_double(argv[3], "b1Kv14", &user_data[INDEX_b1Kv14]); //   IKv14 rate parameter
	readparam_double(argv[3], "b2Kv14", &user_data[INDEX_b2Kv14]); //   IKv14 rate parameter
	readparam_double(argv[3], "b3Kv14", &user_data[INDEX_b3Kv14]); //   IKv14 rate parameter
	readparam_double(argv[3], "b4Kv14", &user_data[INDEX_b4Kv14]); //   IKv14 rate parameter

	readparam_double(argv[3], "A0_HERG", &user_data[INDEX_A0_HERG]); //   IKr rate parameter (1/ms)
	readparam_double(argv[3], "B0_HERG", &user_data[INDEX_B0_HERG]); //   IKr rate parameter (1/mV)
	readparam_double(argv[3], "A1_HERG", &user_data[INDEX_A1_HERG]); //   IKr rate parameter (1/ms)
	readparam_double(argv[3], "B1_HERG", &user_data[INDEX_B1_HERG]); //   IKr rate parameter (1/mV)
	readparam_double(argv[3], "A2_HERG", &user_data[INDEX_A2_HERG]); //   IKr rate parameter (1/ms)
	readparam_double(argv[3], "B2_HERG", &user_data[INDEX_B2_HERG]); //   IKr rate parameter (1/mV)
	readparam_double(argv[3], "A3_HERG", &user_data[INDEX_A3_HERG]); //   IKr rate parameter (1/ms)
	readparam_double(argv[3], "B3_HERG", &user_data[INDEX_B3_HERG]); //   IKr rate parameter (1/mV)
	readparam_double(argv[3], "A4_HERG", &user_data[INDEX_A4_HERG]); //   IKr rate parameter (1/ms)
	readparam_double(argv[3], "B4_HERG", &user_data[INDEX_B4_HERG]); //   IKr rate parameter (1/mV)
	readparam_double(argv[3], "A5_HERG", &user_data[INDEX_A5_HERG]); //   IKr rate parameter (1/ms)
	readparam_double(argv[3], "B5_HERG", &user_data[INDEX_B5_HERG]); //   IKr rate parameter (1/mV)
	readparam_double(argv[3], "A6_HERG", &user_data[INDEX_A6_HERG]); //   IKr rate parameter (1/ms)
	readparam_double(argv[3], "B6_HERG", &user_data[INDEX_B6_HERG]); //   IKr rate parameter (1/mV)
	readparam_double(argv[3], "T_Const_HERG", &user_data[INDEX_T_Const_HERG]); //   IKr rate scale factor for temperature

	readparam_double(argv[3], "CSQNtot", &user_data[INDEX_CSQNtot]); //   Total SR calsequestrin concentration (mM)
	readparam_double(argv[3], "KmCSQN", &user_data[INDEX_KmCSQN]); //   Ca half saturation constant for calsequestrin (mM)
	readparam_double(argv[3], "LTRPNtot", &user_data[INDEX_LTRPNtot]); //   Total troponin low-affinity site concentration (mM)
	readparam_double(argv[3], "HTRPNtot", &user_data[INDEX_HTRPNtot]); //   Total troponin high-affinity site concentration (mM)
	readparam_double(argv[3], "khtrpn_plus", &user_data[INDEX_khtrpn_plus]); //   Troponin Ca binding rate parameter (1/[mM ms])
	readparam_double(argv[3], "khtrpn_minus", &user_data[INDEX_khtrpn_minus]); //   Troponin Ca binding rate parameter (1/ms)
	readparam_double(argv[3], "kltrpn_plus", &user_data[INDEX_kltrpn_plus]); //   Troponin Ca binding rate parameter (1/[mM ms])
	readparam_double(argv[3], "kltrpn_minus", &user_data[INDEX_kltrpn_minus]); //   Troponin Ca binding rate parameter (1/ms)
	readparam_double(argv[3], "CMDNtot", &user_data[INDEX_CMDNtot]); //   Total myoplasmic calmodulin concentration (mM)
	readparam_double(argv[3], "EGTAtot", &user_data[INDEX_EGTAtot]); //   Total myoplasmic EGTA concentration (mM)
	readparam_double(argv[3], "KmCMDN", &user_data[INDEX_KmCMDN]); //   Ca half saturation constant for calmodulin (mM)
	readparam_double(argv[3], "KmEGTA", &user_data[INDEX_KmEGTA]); //   Ca half saturation constant for EGTA (mM)

	readparam_double(argv[3], "fL", &user_data[INDEX_fL]); //   L-type Ca channel rate parameter (1/ms)
	readparam_double(argv[3], "gL", &user_data[INDEX_gL]); //   L-type Ca channel rate parameter (1/ms)
	readparam_double(argv[3], "bL", &user_data[INDEX_bL]); //   L-type Ca channel rate parameter
	readparam_double(argv[3], "aL", &user_data[INDEX_aL]); //   L-type Ca channel rate parameter
	readparam_double(argv[3], "omega", &user_data[INDEX_omega]); //   L-type Ca channel rate parameter (1/ms)
	readparam_double(argv[3], "gammacf", &user_data[INDEX_gammacf]); //   L-type Ca channel rate parameter (1/[mM ms])

	readparam_double(argv[3], "k12", &user_data[INDEX_k12]); //   RyR channel rate parameter (1/ms)
	readparam_double(argv[3], "k21", &user_data[INDEX_k21]); //   RyR channel rate parameter (1/ms)
	readparam_double(argv[3], "k23", &user_data[INDEX_k23]); //   RyR channel rate parameter (1/ms)
	readparam_double(argv[3], "k32", &user_data[INDEX_k32]); //   RyR channel rate parameter (1/ms)
	readparam_double(argv[3], "k34", &user_data[INDEX_k34]); //   RyR channel rate parameter (1/ms)
	readparam_double(argv[3], "k43", &user_data[INDEX_k43]); //   RyR channel rate parameter (1/ms)
	readparam_double(argv[3], "k45", &user_data[INDEX_k45]); //   RyR channel rate parameter (1/ms)
	readparam_double(argv[3], "k54", &user_data[INDEX_k54]); //   RyR channel rate parameter (1/ms)
	readparam_double(argv[3], "k56", &user_data[INDEX_k56]); //   RyR channel rate parameter (1/ms)
	readparam_double(argv[3], "k65", &user_data[INDEX_k65]); //   RyR channel rate parameter (1/ms)
	readparam_double(argv[3], "k25", &user_data[INDEX_k25]); //   RyR channel rate parameter (1/ms)
	readparam_double(argv[3], "k52", &user_data[INDEX_k52]); //   RyR channel rate parameter (1/ms)

	int Vclamp_flag = (int)user_data[INDEX_Vclamp_flag];
	int Num_beats = (int)user_data[INDEX_Num_beats];
	user_data[INDEX_I_STIM] = 0;
	real_T T0 = 0;

	real_T final_time = (Vclamp_flag == 0) ? Num_beats * user_data[INDEX_Period] : user_data[INDEX_t_sim_end]; //Simulation end time (ms) (set according to protocol)

	realtype reltol = RTOL, abstol = ATOL, t, ero;
	int flag, temp_flag, nerr = 0;
	N_Vector y;
	N_Vector dy_dummy = N_VNew_Serial(NUM_STATES);
	void *cvode_mem;
	int qu;
	realtype hu;

	y = NULL;
	cvode_mem = NULL;

	if (Vclamp_flag) {
		printf("Holding membrane potential at %g mV\n",user_data[INDEX_V_hold]);
		y0[INDEX_V] = user_data[INDEX_V_hold];
	}

	char bRestartCVODE = 0;
	realtype t_now = 0;
	char bPulseOn;
	realtype t_disc;
	if (Vclamp_flag == 0) { //Pulse protocol
		if (user_data[INDEX_Shift] > 0) {
			bPulseOn = 1;
			t_disc = user_data[INDEX_Shift];
		} else {
			bPulseOn = 0;
			t_disc = user_data[INDEX_Pulse_duration];
			user_data[INDEX_I_STIM] = user_data[INDEX_Pulse_amplitude];
		}
	} else { //V-clamp protocol
		if (user_data[INDEX_t_clamp_start] > 0) {
			bPulseOn = 1;
			t_disc = user_data[INDEX_t_clamp_start];
		} else {
			bPulseOn = 0;
			t_disc = user_data[INDEX_t_clamp_end];
			y0[INDEX_V] = user_data[INDEX_V_clamp];
		}
	}

	y = N_VMake_Serial(NUM_STATES, y0);
	if (check_flag((void *) y, "N_VNew_Serial", 0))
		return (1);

	cvode_mem = CVodeCreate(CV_BDF, CV_NEWTON);
	if (check_flag((void *) cvode_mem, "CVodeCreate", 0))
		return (1);

	ero = 0;
	flag = CVodeInit(cvode_mem, fcn_40state, T0, y);
	if (check_flag(&flag, "CVodeInit", 1))
		return (1);
	flag = CVodeSStolerances(cvode_mem, reltol, abstol);
	if (check_flag(&flag, "CVodeSStolerances", 1))
		return (1);
	flag = CVodeSetMaxStep(cvode_mem, MAX_STEP_SIZE);
	if (check_flag(&flag, "CVodeSetMaxStep", 1))
		return (1);
	flag = CVodeSetMaxConvFails(cvode_mem, MAX_NCF);
	if (check_flag(&flag, "CVodeSetMaxConvFails", 1))
		return (1);
	flag = CVodeSetStopTime(cvode_mem, t_disc);
	if (check_flag(&flag, "CVodeSetStopTime", 1))
		return (1);

	//flag = CVDiag(cvode_mem);
	//check_flag(&flag, "CVDiag", 1);
	flag = CVDense(cvode_mem, NUM_STATES);
	check_flag(&flag, "CVDense", 1);

	char bClamp_Set = 0;

	flag = CVodeSetUserData(cvode_mem, user_data);
	if (check_flag(&flag, "CVodeSetUserData", 1))
		return (1);
	realtype t_next;

	//Output states
	if (user_data[INDEX_t_output_start] <= 0) {
		fprintf(fstates, "%f\t", 0.0);
		for (int i = 0; i < NUM_STATES; i++) {
			fprintf(fstates, "%g\t", NV_Ith_S(y,i));
		}
		fprintf(fstates, "\n");

		//Output fluxes
		fcn_40state(0, y, dy_dummy, user_data);
		double ICaL = user_data[NUM_PARAMS+0];
		double JCaL = user_data[NUM_PARAMS+1];
		double JRyR = user_data[NUM_PARAMS+2];
		double CaSSavg = user_data[NUM_PARAMS+3];
		fprintf(ffluxes, "%f\t%g\t%g\t%g\t%g\n", 0.0, ICaL, JCaL, JRyR, CaSSavg);
	}

	while (t_now < final_time) {

		//printf("t_now = %g, t_disc = %g, final_time = %g\n",t_now,t_disc,final_time);

		t_next = t_now + user_data[INDEX_DTOUT];

		/*if (t_now == t_disc) {
			//printf("\tResetting CVode...\n");
			flag = CVodeReInit(cvode_mem, t_now, y);
			if (check_flag(&flag, "CVodeReInit", 1))
				return (1);
			bRestartCVODE = 0;
		}*/

		if (Vclamp_flag == 0) {
			if (t_now >= t_disc) {
				if (bPulseOn) {
					t_disc += user_data[INDEX_Pulse_duration];
					user_data[INDEX_I_STIM] = user_data[INDEX_Pulse_amplitude];
				} else {
					t_disc += user_data[INDEX_Period] - user_data[INDEX_Pulse_duration];
					user_data[INDEX_I_STIM] = 0;
				}
				//printf("\tSet t_disc = %g\n",t_disc);
				bPulseOn = !bPulseOn;
				flag = CVodeSetStopTime(cvode_mem, t_disc);
				if (check_flag(&flag, "CVodeSetStopTime", 1))
					return (1);
				flag = CVodeReInit(cvode_mem, t_now, y);
				if (check_flag(&flag, "CVodeReInit", 1))
					return (1);
			}
		} else {
			if (t_now >= t_disc) {
				if (bPulseOn) {
					t_disc  = user_data[INDEX_t_clamp_end];
					NV_Ith_S(y,40) = user_data[INDEX_V_clamp];
					printf("Set V_clamp\n");
				} else {
					t_disc = final_time;
					NV_Ith_S(y,40) = user_data[INDEX_V_hold];
				}
				//printf("\tSet t_disc = %g\n",t_disc);
				bPulseOn = !bPulseOn;
				flag = CVodeSetStopTime(cvode_mem, t_disc);
				if (check_flag(&flag, "CVodeSetStopTime", 1))
					return (1);
				flag = CVodeReInit(cvode_mem, t_now, y);
				if (check_flag(&flag, "CVodeReInit", 1))
					return (1);
			}
		}
		//printf("%g\t%g\n",t_now,t_next);
		/*if (Vclamp_flag == 0) { //Pacing protocol

			real_T time_on_Is1 =  user_data[INDEX_Shift] + (floor((t_now) / user_data[INDEX_Period]) * user_data[INDEX_Period]); //   Establish stimulus current
			real_T time_off_Is1 = time_on_Is1 + user_data[INDEX_Pulse_duration];
			//printf("\t[%g\t%g]\n",time_on_Is1,time_off_Is1);

			if (t_next >= time_on_Is1 && t_now < time_on_Is1) {
				t_next = time_on_Is1;
				//printf("\tChanged t_next to %g\n",t_next);
				bRestartCVODE = 1;
			} else if (t_next >= time_off_Is1 && t_now < time_off_Is1) {
				t_next = time_off_Is1;
				bRestartCVODE = 1;
				//printf("\tChanged t_next to %g\n",t_next);
			}

		} else { //Voltage clamp protocol

			if (t_next >= user_data[INDEX_t_clamp_start] && t_now < user_data[INDEX_t_clamp_start]) {
				t_next = user_data[INDEX_t_clamp_start];
				bRestartCVODE = 1;
				//printf("\tChanged t_next to %g\n",t_next);
			} else if (t_next >= user_data[INDEX_t_clamp_end] && t_now < user_data[INDEX_t_clamp_end]) {
				t_next = user_data[INDEX_t_clamp_start];
				bRestartCVODE = 1;
				//printf("\tChanged t_next to %g\n",t_next);
			} else if (!bClamp_Set && t_next <= user_data[INDEX_t_clamp_end] && t_now >= user_data[INDEX_t_clamp_start]) {
				NV_Ith_S(y,40) = user_data[INDEX_V_clamp];
				bClamp_Set = 1;
				flag = CVodeReInit(cvode_mem, t_next, y);
				if (check_flag(&flag, "CVodeReInit", 1))
					return (1);
			}

		}*/

		//printf("%g\t%g\n", tout, NV_Ith_S(y,40));

		flag = CVode(cvode_mem, t_next, y, &t, CV_NORMAL);
		t_next = t;
		//printf("\ttret = %g\n",t);
		if (check_flag(&flag, "CVode", 1))
			return (1);
		temp_flag = CVodeGetLastOrder(cvode_mem, &qu);
		if (check_flag(&temp_flag, "CVodeGetLastOrder", 1))
			++nerr;
		temp_flag = CVodeGetLastStep(cvode_mem, &hu);
		if (check_flag(&temp_flag, "CVodeGetLastStep", 1))
			++nerr;

		if (t_next >= user_data[INDEX_t_output_start]) {
			//Output states
			fprintf(fstates, "%f\t", t_next);
			for (int i = 0; i < NUM_STATES; i++) {
				fprintf(fstates, "%g", NV_Ith_S(y,i));
				if (i < NUM_STATES-1){
					fprintf(fstates, "\t");
				}
			}
			fprintf(fstates, "\n");

			//Output fluxes
			fcn_40state(t_next, y, dy_dummy, user_data);
			double ICaL = user_data[NUM_PARAMS+0];
			double JCaL = user_data[NUM_PARAMS+1];
			double JRyR = user_data[NUM_PARAMS+2];
			double CaSSavg = user_data[NUM_PARAMS+3];
			fprintf(ffluxes, "%f\t%g\t%g\t%g\t%g\n", t_next, ICaL, JCaL, JRyR, CaSSavg);
		}

		t_now = t_next;

	}

	PrintFinalStats(cvode_mem, 0, ero);

	CVodeFree(&cvode_mem);
	N_VDestroy_Serial(y);
	fclose(fstates);
	fclose(ffluxes);

}

static void PrintFinalStats(void *cvode_mem, int miter, realtype ero) {
	long int lenrw, leniw, nst, nfe, nsetups, nni, ncfn, netf;
	int flag;

	flag = CVodeGetWorkSpace(cvode_mem, &lenrw, &leniw);
	check_flag(&flag, "CVodeGetWorkSpace", 1);
	flag = CVodeGetNumSteps(cvode_mem, &nst);
	check_flag(&flag, "CVodeGetNumSteps", 1);
	flag = CVodeGetNumRhsEvals(cvode_mem, &nfe);
	check_flag(&flag, "CVodeGetNumRhsEvals", 1);
	flag = CVodeGetNumLinSolvSetups(cvode_mem, &nsetups);
	check_flag(&flag, "CVodeGetNumLinSolvSetups", 1);
	flag = CVodeGetNumErrTestFails(cvode_mem, &netf);
	check_flag(&flag, "CVodeGetNumErrTestFails", 1);
	flag = CVodeGetNumNonlinSolvIters(cvode_mem, &nni);
	check_flag(&flag, "CVodeGetNumNonlinSolvIters", 1);
	flag = CVodeGetNumNonlinSolvConvFails(cvode_mem, &ncfn);
	check_flag(&flag, "CVodeGetNumNonlinSolvConvFails", 1);

	printf("\n Final statistics for this run:\n\n");
	printf(" CVode real workspace length              = %4ld \n", lenrw);
	printf(" CVode integer workspace length           = %4ld \n", leniw);
	printf(" Number of steps                          = %4ld \n", nst);
	printf(" Number of f-s                            = %4ld \n", nfe);
	printf(" Number of setups                         = %4ld \n", nsetups);
	printf(" Number of nonlinear iterations           = %4ld \n", nni);
	printf(" Number of nonlinear convergence failures = %4ld \n", ncfn);
	printf(" Number of error test failures            = %4ld \n\n", netf);

#if defined(SUNDIALS_EXTENDED_PRECISION)
	printf(" Error overrun = %.3Lf \n", ero);
#else
	printf(" Error overrun = %.3f \n", ero);
#endif
}

/* Check function return value...
 opt == 0 means SUNDIALS function allocates memory so check if
 returned NULL pointer
 opt == 1 means SUNDIALS function returns a flag so check if
 flag >= 0
 opt == 2 means function allocates memory so check if returned
 NULL pointer */

static int check_flag(void *flagvalue, char *funcname, int opt) {
	int *errflag;

	/* Check if SUNDIALS function returned NULL pointer - no memory allocated */
	if (opt == 0 && flagvalue == NULL) {
		fprintf(stderr,
				"\nSUNDIALS_ERROR: %s() failed - returned NULL pointer\n\n",
				funcname);
		return (1);
	}

	/* Check if flag < 0 */
	else if (opt == 1) {
		errflag = (int *) flagvalue;
		if (*errflag < 0) {
			fprintf(stderr, "\nSUNDIALS_ERROR: %s() failed with flag = %d\n\n",
					funcname, *errflag);
			return (1);
		}
	}

	/* Check if function returned NULL pointer - no memory allocated */
	else if (opt == 2 && flagvalue == NULL) {
		fprintf(stderr,
				"\nMEMORY_ERROR: %s() failed - returned NULL pointer\n\n",
				funcname);
		return (1);
	}

	return (0);
}

void checkopen(FILE* f, char filename[]) {
	if (f == NULL) {
		fprintf(stderr, "Error: could not open file: %s\n", filename);
		exit(1);
	}
}

void readparam_double(char filename[], char paramname[], double* param) {
	//Open parameters file
	FILE* f = fopen(filename, "r");

	/* Throw error if file could not be opened */
	checkopen(f, filename);

	char line[255];
	char* tok;
	while (fgets(line, 255, f) != NULL) {
		tok = strtok(line, "=");
		if (!strcmp(tok, paramname)) {
			*param = atof(strtok(NULL, "="));
			printf("Set parameter %s = %f\n", paramname, *param);
			fclose(f);
			return;
		}
	}

	fclose(f);
	fprintf(stderr, "Error: could not read parameter: %s\n", paramname);
	exit(1);
}

void readparam_int(char filename[], char paramname[], int* param) {
	//Open parameters file
	FILE* f = fopen(filename, "r");

	/* Throw error if file could not be opened */
	checkopen(f, filename);

	char line[255];
	char* tok;
	while (fgets(line, 255, f) != NULL) {
		tok = strtok(line, "=");
		if (!strcmp(tok, paramname)) {
			*param = atoi(strtok(NULL, "="));
			printf("Set parameter %s = %d\n", paramname, *param);
			fclose(f);
			return;
		}
	}

	fclose(f);
	fprintf(stderr, "Error: could not read parameter: %s\n", paramname);
	exit(1);
}

