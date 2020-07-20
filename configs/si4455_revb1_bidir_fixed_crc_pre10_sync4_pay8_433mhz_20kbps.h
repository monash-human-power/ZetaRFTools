/*!
 * @brief Radio configuration data adapted from SiLabs's WDS software.
 *
 * Configuration for 433 MHz Zeta module.
 */

#pragma once

#include <stdint.h>

// INPUT DATA
/*
// Crys_freq(Hz): 30000000    Crys_tol(ppm): 30    IF_mode: 2    High_perf_Ch_Fil: 1    OSRtune: 0    Ch_Fil_Bw_AFC: 0    ANT_DIV: 0    PM_pattern: 0
// MOD_type: 2    Rsymb(sps): 20000    Fdev(Hz): 30000    RXBW(Hz): 114000    Manchester: 1    AFC_en: 1    Rsymb_error: 0.0    Chip-Version: 2
// RF Freq.(MHz): 433    API_TC: 28    fhst: 250000    inputBW: 0    BERT: 0    RAW_dout: 0    D_source: 0    Hi_pfm_div: 0
//
// # WB filter 2 (BW = 137.42 kHz);  NB-filter 2 (BW = 137.42 kHz)

//
// Modulation index: 3
*/

// USER DEFINED PARAMETERS
// Define your own parameters here

namespace ZetaRfConfigs {

struct Config433_FixedLength_CRC_Preamble10_Sync4_Payload8_20kbps
{
    static const uint8_t RadioConfigurationDataArray[];

    constexpr static uint8_t DefaultChannel = 0;
    constexpr static uint8_t PacketLength = 0x08;
    constexpr static bool VariableLengthPacketConfiguration = false;
};

}
