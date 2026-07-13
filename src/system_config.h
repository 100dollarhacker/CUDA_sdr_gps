#ifndef SYTEM_CONFIG_H
#define SYTEM_CONFIG_H


// const int SAMPLES_PER_SECOND = 10230000;//4092000; //SAMPLES_PER_MS00;
// // const int SAMPLES_PER_SECOND =    4092000; //SAMPLES_PER_MS00;

// const int SAMPLES_PER_MS = SAMPLES_PER_SECOND / 1000;
// const int BLOCK_SIZE = (SAMPLES_PER_MS/256)+1;
// const int SAMPLES_PER_CHIP = 10;
// const int SAMPLES_PER_CHIP_FRAC = 1;
// const int  OUTPUT_SIZE = 2*BLOCK_SIZE; //2*10230;//2*(BLOCK_SIZE/256);


const int GOLD_CODE_LENGTH = 1023;
const int SAMPLES_PER_SECOND = 25000000;//4092000; //SAMPLES_PER_MS00;

const int SAMPLES_PER_MS = SAMPLES_PER_SECOND / 1000;
const int BLOCK_SIZE = (SAMPLES_PER_MS/256)+1;
const int SAMPLES_PER_CHIP = 25;
const float SAMPLES_PER_CHIP_FRAC = 1.023;
const int  OUTPUT_SIZE = 2*BLOCK_SIZE;


const int CHIPS_PER_MS = SAMPLES_PER_SECOND/1000;



// const int CHIPS_PER_MS = 1023*10;

// const char* FILE_PATH_GPS_IQ_SAMPLE1 = "../test_data/wave_1565190Mhz_samp_1023KHz_sats_11_19.dat";
// const char* FILE_PATH_GPS_IQ_SAMPLE1 = "../test_data/wave_GPS_L1_samp_10.23MHZ_sats_11_12_20_21";
// const char* FILE_PATH_GPS_IQ_SAMPLE1 = "../test_data/wave_GPS_L1_samp_10.23MHZ_sats_11_12_20_21.dat";
// const char* FILE_PATH_GPS_IQ_SAMPLE1 = "../../test_data/wave_GPS_L1_samp_10.23MHZ_sats_11_12_20_21.dat2";
// const char* FILE_PATH_GPS_IQ_SAMPLE1 = "../test_data/wave_157542_sam_1023_sats_5_11_12.dat";
// const char* FILE_PATH_GPS_IQ_SAMPLE1 = "../test_data/wave_156542_sam_1023_sats_5_11_12.dat";
// const char* FILE_PATH_GPS_IQ_SAMPLE1 = "../../test_data/gps_sim_data_10p23MHZSampling.raw"; // Working
// const char* FILE_PATH_GPS_IQ_SAMPLE1 = "../../test_data/gps_sim_data_4p092MHzSampling.raw"; // Working
// const char* FILE_PATH_GPS_IQ_SAMPLE1 = "../../test_data/data.raw";
// const char* FILE_PATH_GPS_IQ_SAMPLE1 =  "../../test_data/data_p1_a1_l32_g40_b28_s10p23.raw"; BAD
// const char* FILE_PATH_GPS_IQ_SAMPLE1 =  "../../test_data/data_p1_a1_l40_g60_b28_s10p23.raw"; BAD
// const char* FILE_PATH_GPS_IQ_SAMPLE1 =  "../../test_data/data_p1_a1_l24_g30_b28_s10p23.raw";

// File downloaded from https://etsin.fairdata.fi/dataset/63f8b776-680b-4c98-ace7-d5e443f2b1c5/data
// const char* FILE_PATH_GPS_IQ_SAMPLE1 =  "../../test_data/skydel_L1_real_gps.raw"; // Working!
// const char*     FILE_PATH_GPS_IQ_SAMPLE1 = "../../test_data/data_clean_575MHz_Sine_10MHZSampling_Band5MHZ.raw";// No Signal how it possible ;)
// const char*     FILE_PATH_GPS_IQ_SAMPLE1 = "../../test_data/data_clean_949MHz_Sine_10MHZSampling_Band5MHZ.raw";// No Signal how it possible ;)
// const char*     FILE_PATH_GPS_IQ_SAMPLE1 = "../../test_data/data_clean_950MHzplus1KHz_Sine_10MHZSampling_Band5MHZ.raw";// No Signal how it possible ;)

// const char* FILE_PATH_GPS_IQ_SAMPLE1 =  "../../test_data/data_p1_a1_l24_g40_b28_s10p23.raw";
// const char* FILE_PATH_GPS_IQ_SAMPLE1 =  "../../test_data/data_p1_a1_l32_g40_s20MHz_L1.raw";
// const char* FILE_PATH_GPS_IQ_SAMPLE1 =  "../../test_data/data_p1_a1_l24_g40_s10p23_L1.raw";
// const char* FILE_PATH_GPS_IQ_SAMPLE1 =  "../../test_data/data_p1_a1_l40_g50_b28_s10p23.raw";
// const char* FILE_PATH_GPS_IQ_SAMPLE1 =  "../../test_data/data_p1_a1_l40_g60_b28_s10p23.raw";
// const char* FILE_PATH_GPS_IQ_SAMPLE1 = "../../test_data/gps_sim_data_20MHzSampling.raw"; // Working


#endif // SYTEM_CONFIG_H
