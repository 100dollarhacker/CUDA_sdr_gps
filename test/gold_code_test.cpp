#include <string>
#include <thread>


#include "gtest/gtest.h" // Include the Google Test header
#include "C_A__code_generator.h"
#include "baseband_generator.h"
#include "GPS_IQ_reader.h"
#include "fourier_transform_signal.h"
#include "get_bits_from_gps_input.h"
#include "cuda_lib.cuh"


const int CHIPS_PER_MS = 25000;


// const int CHIPS_PER_MS = div.products  .product-element-top> a[href]1023*10;

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
const char* FILE_PATH_GPS_IQ_SAMPLE1 =  "../../test_data/skydel_L1_real_gps.raw"; // Working!
// const char* FILE_PATH_GPS_IQ_SAMPLE1 =  "../../test_data/gps_sim_data_25MBPS.raw"; // ???

// const char*     FILE_PATH_GPS_IQ_SAMPLE1 = "../../test_data/data_clean_575MHz_Sine_10MHZSampling_Band5MHZ.raw";// No Signal how it possible ;)
// const char*     FILE_PATH_GPS_IQ_SAMPLE1 = "../../test_data/data_clean_949MHz_Sine_10MHZSampling_Band5MHZ.raw";// No Signal how it possible ;)
// const char*     FILE_PATH_GPS_IQ_SAMPLE1 = "../../test_data/data_clean_950MHzplus1KHz_Sine_10MHZSampling_Band5MHZ.raw";// No Signal how it possible ;)

// const char* FILE_PATH_GPS_IQ_SAMPLE1 =  "../../test_data/data_p1_a1_l24_g40_b28_s10p23.raw";
// const char* FILE_PATH_GPS_IQ_SAMPLE1 =  "../../test_data/data_p1_a1_l32_g40_s20MHz_L1.raw";
// const char* FILE_PATH_GPS_IQ_SAMPLE1 =  "../../test_data/data_p1_a1_l24_g40_s10p23_L1.raw";
// const char* FILE_PATH_GPS_IQ_SAMPLE1 =  "../../test_data/data_p1_a1_l40_g50_b28_s10p23.raw";
// const char* FILE_PATH_GPS_IQ_SAMPLE1 =  "../../test_data/data_p1_a1_l40_g60_b28_s10p23.raw";
// const char* FILE_PATH_GPS_IQ_SAMPLE1 = "../../test_data/gps_sim_data_20MHzSampling.raw"; // Working

int first_ten_bits_binary_to_int(vector<int> input)
{
    int n = 0;
    for (int i = 0; i < 10; i++)
    {
        n *= 2;
        n += input[i];
    }
    return n;
}

// This data is taken from IS-GPS-200H Table 3-Ia
vector<string> gold_codes_initials = {"01440", "01620", "01710", "01744", "01133", "01455", "01131", "01454", "01626", "01504", "01642", "01750", "01764", "01772", "01775", "01776", "01156", "01467", "01633", "01715", "01746", "01763", "01063", "01706", "01743", "01761", "01770", "01774", "01127", "01453", "01625", "01712"};

TEST(GoldCodeTest, CheckAllSatelites)
{

    vector<int> output(1023);
    for (int i = 0; i < 32; i++)
    {
        CA_generator ca;
        ca.get_gold_code_sequence(i, output);
        // printf("Sat #%d Seq:%o\n",i, first_ten_bits_binary_to_int(output));
        int gold_code_initials_decimal_value = strtol(gold_codes_initials[i].c_str(), NULL, 8);

        ASSERT_EQ(first_ten_bits_binary_to_int(output), gold_code_initials_decimal_value);
    }
}

TEST(GoldCodeTest, autocorelation)
{

    vector<int> output1(1023), output2(1023);
    CA_generator ca;
    ca.get_gold_code_sequence(1, output1);
    ca.get_gold_code_sequence(1, output2);

    BasebandGenerator bg;
    auto baseband_signal1 = bg.resampleCaGoldCodeTOneMilisecondOfBaseband(output1);
    auto baseband_signal2 = bg.resampleCaGoldCodeTOneMilisecondOfBaseband(output2);

    auto cross = (int)bg.crossCorrelation(baseband_signal1, baseband_signal2, 0);

    ASSERT_EQ(CHIPS_PER_MS, cross);
}

TEST(GoldCodeTest, crosscorelation)
{

    vector<int> output1(1023), output2(1023);
    CA_generator ca;
    ca.get_gold_code_sequence(1, output1);
    ca.get_gold_code_sequence(14, output2);

    BasebandGenerator bg;
    auto baseband_signal1 = bg.resampleCaGoldCodeTOneMilisecondOfBaseband(output1);
    auto baseband_signal2 = bg.resampleCaGoldCodeTOneMilisecondOfBaseband(output2);

    auto cross = (int)bg.crossCorrelation(baseband_signal1, baseband_signal2, 0);

    ASSERT_LT(cross, 650);
}

TEST(GoldCodeTest, autocrosscorelation_with_lag)
{

    vector<int> output1(1023), output2(1023);
    CA_generator ca;
    ca.get_gold_code_sequence(1, output1);
    ca.get_gold_code_sequence(1, output2);
    int lag = 500;

    BasebandGenerator bg;
    auto baseband_signal1 = bg.resampleCaGoldCodeTOneMilisecondOfBaseband(output1);
    auto baseband_signal2 = bg.resampleCaGoldCodeTOneMilisecondOfBaseband(output2);

    auto cross = (int)bg.crossCorrelation(baseband_signal1, baseband_signal2, lag);

    ASSERT_LT(cross, 650);
}

void run_one_sateliate_in_thread(int i, std::vector<std::complex<float>> iq_samples)
{
    int max_cross = 0;
    vector<int> output1(1023);
    CA_generator ca;
    ca.get_gold_code_sequence(i, output1);

    printf("Processing Sat #%d\n", i);
    BasebandGenerator bg;
    int max_lag = 0;
    float max_freq = 0;
    for (float freq = -5000; freq <= 5000; freq += 500) {
        auto baseband_signal1 = bg.resampleCaGoldCodeTOneMilisecondOfBaseband(output1, freq );//- 10e6 );

        for (int lag = 0 ; lag < CHIPS_PER_MS ; lag+=3) {
            auto cross = (int)bg.crossCorrelation(iq_samples, baseband_signal1, lag);
            // printf("%d, ", cross);
            if (cross > max_cross) {
                max_cross = cross;
                max_freq = freq;
                max_lag = lag;
                // printf("Sat #%d Lag:%d Cross:%d freq:%f\n", i, lag, cross, freq);
                // Max for now: Sat #0 Lag:350 Cross:832 freq:-300.000000
            }
        }

        // printf("\n");
    }
    // printf("MAX: Sat #%d Lag:%d Cross:%d freq:%f\n", i, max_lag, max_cross, max_freq);

}
#if 0

// Disabled test - takes too long to run
TEST(GoldCodeTest, autocorelate_wit_real_samples) {

    GPS_IQ_reader reader;
    reader.open(FILE_PATH_GPS_IQ_SAMPLE1);
    reader.seekSample(0x4000);

    std::vector<std::complex<float>> iq_samples;
    reader.readSamples(CHIPS_PER_MS, iq_samples); // Read 10 ms of IQ samples

    std::vector<std::thread> threads;

    // for (int i = 4; i < 13 ; i++)
    for (int i = 0; i < 16 ; i++)
    // int i = 0;
    {
        threads.emplace_back(run_one_sateliate_in_thread, i, iq_samples);
    }

    for (auto& t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }
    // ASSERT_GT(cross, 8000);
}

TEST(GoldCodeTest, autocorelate_wit_real_samples_fft_version)
{

    GPS_IQ_reader reader;
    // reader.open("../test_data/gps_iq_sample2.raw");
    reader.open(FILE_PATH_GPS_IQ_SAMPLE1);

    std::vector<std::complex<float>> iq_samples;
    reader.readSamples(CHIPS_PER_MS, iq_samples); // Read 10 ms of IQ samples

    // for (int i = 4; i < 32; i++)
    //Sat #11 Lag:5032 Cross:5335 freq:-500.000000
    //Sat #4 Lag:587 Cross:5122 freq:-500.000000


    int i =11;
    {
        vector<int> output1(1023);
        CA_generator ca;
        ca.get_gold_code_sequence(i, output1);

        // printf("Processing Sat #%d\n", i);
        BasebandGenerator bg;
        int max_cross = 0;
        int max_lag = 0;
        float max_freq = 0;

        float freq = -500;
        //for (float freq = -5000; freq <= 5000; freq += 500)
        {
            auto baseband_signal1 = bg.resampleCaGoldCodeTOneMilisecondOfBaseband(output1, freq);
            auto baseband_signal1_fft = ft::fft_from_complex(baseband_signal1);
            baseband_signal1_fft = ft::conj(baseband_signal1_fft);

            auto iq_samples_fft = ft::fft_from_complex(iq_samples);

            vector<complex<float>> cross = bg.crossCorrelation(iq_samples_fft, baseband_signal1_fft);


            // ft::fft_inplace(cross, true);
            // auto cross_ifft = cross;
            auto cross_ifft = ft::ifft_to_real(cross);
            // printf("cross_if ft size:%zu\n", cross_ifft.size());

            for (size_t lag = 0; lag < cross_ifft.size(); lag++)
            {
                int cross_value = std::abs(cross_ifft[lag]);
                if (cross_value > max_cross)
                {
                    max_cross = cross_value;
                    max_lag = lag;
                    max_freq = freq;

                    printf("Sat #%d Lag:%zu Cross:%d freq:%f\n", i, lag, cross_value, freq);
                    // Max for now: Sat #0 Lag:350 Cross:832 freq:-300.000000
                }

            }

        }
        printf("Sat #%d Lag:%d Cross:%d freq:%f\n", i, max_lag, max_cross, max_freq);



    }
    // ASSERT_GT(cross, 8000);
}
#endif

// TEST(GoldCodeTest, get_navigation_bits_From_IQ_samples)
// {

//     GPS_IQ_reader reader;

//     reader.open(FILE_PATH_GPS_IQ_SAMPLE1);

//     std::vector<std::complex<float>> iq_samples;
//     reader.readSamples(CHIPS_PER_MS, iq_samples); // Read 10 ms of IQ samples

//     // for (int i = 0; i < 32; i++)
//     int i = 29;
//     {
//         vector<int> output1(1023);
//         CA_generator ca;
//         ca.get_gold_code_sequence(i, output1);

//         printf("Processing Sat #%d\n", i);
//         BasebandGenerator bg;
//         int max_cross = 0;
//         int max_lag = 0;
//         float max_freq = 0;

//         for (float freq = -1500; freq <= 1500; freq += 50)
//         {
//             auto baseband_signal1 = bg.resampleCaGoldCodeTOneMilisecondOfBaseband(output1, freq);
//             auto baseband_signal1_fft = ft::fft_from_complex(baseband_signal1);
//             baseband_signal1_fft = ft::conj(baseband_signal1_fft);

//             auto iq_samples_fft = ft::fft_from_complex(iq_samples);

//             vector<complex<float>> cross = bg.crossCorrelation(iq_samples_fft, baseband_signal1_fft);

//             // auto cross_ifft = ft::ifft_to_real(cross);
//             ft::fft_inplace(cross, true);
//             auto cross_ifft = cross;
//             // printf("cross_if ft size:%zu\n", cross_ifft.size());

//             for (size_t lag = 0; lag < cross_ifft.size(); lag++)
//             {
//                 int cross_value = std::abs(cross_ifft[lag]);
//                 if (cross_value > max_cross)
//                 {
//                     max_cross = cross_value;
//                     max_lag = lag;
//                     max_freq = freq;

//                     // printf("Sat #%d Lag:%zu Cross:%d freq:%f\n", i, lag, cross_value, freq);
//                     // Max for now: Sat #0 Lag:350 Cross:832 freq:-300.000000
//                 }

//             }

//         }
//         printf("Sat #%d Lag:%d Cross:%d freq:%f\n", i, max_lag, max_cross, max_freq);

//         // max_freq = 300;

//         GPS_IQ_reader reader2;
//         reader2.open(FILE_PATH_GPS_IQ_SAMPLE1);
//         reader2.seekSample(max_lag);

//         std::vector<std::complex<float>> iq_samples;
//         reader2.readSamples(500*CHIPS_PER_MS, iq_samples); // Read 40 ms of IQ samples

//         auto baseband_signal1 = bg.resampleInputSignalToBaseband(iq_samples, max_freq);


//         auto gps_data = getGPSData(baseband_signal1, output1, CHIPS_PER_MS);

//         // First 10 bits should be: 1,0,1,0,1,0,1,0,1,0
//         // std::vector<int> expected_bits = {1, 0, 1, 0, 1, 0, 1, 0, 1, 0};

//         // ASSERT_EQ(gps_data.nav_bits.size(), 40);
//         // for (size_t i = 0; i < expected_bits.size(); i++)
//         // {
//         //     ASSERT_EQ(gps_data.nav_bits[i], expected_bits[i]);
//         // }
//         // ASSERT_GT(gps_data.snr, 5.0f);



//     }
//     // ASSERT_GT(cross, 8000);
// }


TEST(CudaTest, ResampleGoldCodeToBaseband) {
    vector<int> gold_code(1023);
    CA_generator ca;
    ca.get_gold_code_sequence(1, gold_code);


     auto start = std::chrono::system_clock::now();

    // 2. Perform some operations or wait (for demonstration)
    BasebandGenerator bg;
    auto baseband_signal_cpu = bg.resampleCaGoldCodeTOneMilisecondOfBaseband(gold_code, 1000.0f);


    // 3. Get the ending time point
    auto end = std::chrono::system_clock::now();

    // 4. Calculate the difference (results in a duration object)
    std::chrono::system_clock::duration diff = end - start;

    // For floating-point seconds:
    std::chrono::duration<double> seconds_double = end - start;
    // std::cout << "Time difference in seconds (double): " << seconds_double.count() << std::endl;
    std::cout << "Time difference in seconds (double): " << seconds_double.count() << std::endl;


    auto baseband_signal_gpu = resampleCaGoldCodeTOneMilisecondOfBasebandCUDA(gold_code, 1000.0f);

    ASSERT_EQ(baseband_signal_cpu.size(), baseband_signal_gpu.size());

    for (size_t i = 0; i < baseband_signal_cpu.size(); i++) {
        EXPECT_NEAR(baseband_signal_cpu[i].real(), baseband_signal_gpu[i].real(), 1e-5);
        EXPECT_NEAR(baseband_signal_cpu[i].imag(), baseband_signal_gpu[i].imag(), 1e-5);
    }
}

TEST(CudaGoldCodeTest, autocorelation)
{

    vector<int> output1(1023), output2(1023);
    CA_generator ca;
    ca.get_gold_code_sequence(1, output1);
    ca.get_gold_code_sequence(1, output2);

    BasebandGenerator bg;
    auto baseband_signal1 = resampleCaGoldCodeTOneMilisecondOfBasebandCUDA(output1, 0);
    auto baseband_signal2 = resampleCaGoldCodeTOneMilisecondOfBasebandCUDA(output2, 0);


     for (size_t i = 0; i < 10; i++) { //baseband_signal1.size(); i++) {
        EXPECT_NEAR(baseband_signal2[i].real(), baseband_signal1[i].real(), 1e-5);
        EXPECT_NEAR(baseband_signal2[i].imag(), baseband_signal1[i].imag(), 1e-5);
        // std::cout << "Baseband1[" << i << "]: " << baseband_signal1[i] << " Baseband2[" << i << "]: " << baseband_signal2[i] << std::endl;
    }
    auto cross = (int)bg.crossCorrelation(baseband_signal1, baseband_signal2, 0);

    ASSERT_LE(10220, cross);
}

TEST(CudaGoldCodeTest, autocorelation_with_frequency_shift)
{

    vector<int> output1(1023), output2(1023);
    CA_generator ca;
    ca.get_gold_code_sequence(1, output1);
    ca.get_gold_code_sequence(1, output2);

    BasebandGenerator bg;
    auto baseband_signal1 = resampleCaGoldCodeTOneMilisecondOfBasebandCUDA(output1, 250);
    auto baseband_signal2 = resampleCaGoldCodeTOneMilisecondOfBasebandCUDA(output2, 250);


     for (size_t i = 0; i < 10; i++) { //baseband_signal1.size(); i++) {
        EXPECT_NEAR(baseband_signal2[i].real(), baseband_signal1[i].real(), 1e-5);
        EXPECT_NEAR(baseband_signal2[i].imag(), baseband_signal1[i].imag(), 1e-5);
        // std::cout << "Baseband1[" << i << "]: " << baseband_signal1[i] << " Baseband2[" << i << "]: " << baseband_signal2[i] << std::endl;
    }
    auto cross = (int)bg.crossCorrelation(baseband_signal1, baseband_signal2, 0);

    ASSERT_LE(10220/2, cross);
}



TEST(CudaGoldCodeTest, autocorelation_with_freq_in_CUDA)
{

    vector<int> output1(1023), output2(1023);
    CA_generator ca;
    ca.get_gold_code_sequence(1, output1);
    ca.get_gold_code_sequence(1, output2);

    BasebandGenerator bg;
    auto baseband_signal1 = resampleCaGoldCodeTOneMilisecondOfBasebandCUDA(output1, 250);
    auto baseband_signal2 = resampleCaGoldCodeTOneMilisecondOfBasebandCUDA(output2, 250);


     for (size_t i = 0; i < 10; i++) { //baseband_signal1.size(); i++) {
        EXPECT_NEAR(baseband_signal2[i].real(), baseband_signal1[i].real(), 1e-5);
        EXPECT_NEAR(baseband_signal2[i].imag(), baseband_signal1[i].imag(), 1e-5);
        // std::cout << "Baseband1[" << i << "]: " << baseband_signal1[i] << " Baseband2[" << i << "]: " << baseband_signal2[i] << std::endl;
    }
    auto cross = (int)bg.crossCorrelation(baseband_signal1, baseband_signal2, 0);
    auto cross_CUDA = (int)crossCorrelationCUDA(baseband_signal1, baseband_signal2, 0);

    EXPECT_NEAR(cross_CUDA, cross, 30);
}

TEST(CudaGoldCodeTest, autocorelation_with_freq_in_CUDA_and_lag)
{

    vector<int> output1(1023), output2(1023);
    CA_generator ca;
    ca.get_gold_code_sequence(1, output1);
    ca.get_gold_code_sequence(1, output2);

    BasebandGenerator bg;
    auto baseband_signal1 = resampleCaGoldCodeTOneMilisecondOfBasebandCUDA(output1, 250);
    auto baseband_signal2 = resampleCaGoldCodeTOneMilisecondOfBasebandCUDA(output2, 250);


     for (size_t i = 0; i < 10; i++) { //baseband_signal1.size(); i++) {
        EXPECT_NEAR(baseband_signal2[i].real(), baseband_signal1[i].real(), 1e-5);
        EXPECT_NEAR(baseband_signal2[i].imag(), baseband_signal1[i].imag(), 1e-5);
        // std::cout << "Baseband1[" << i << "]: " << baseband_signal1[i] << " Baseband2[" << i << "]: " << baseband_signal2[i] << std::endl;
    }
    auto cross = (int)bg.crossCorrelation(baseband_signal1, baseband_signal2, 10);
    auto cross_CUDA = (int)crossCorrelationCUDA(baseband_signal1, baseband_signal2, 10);

    EXPECT_NEAR(cross_CUDA, cross, 30);
}

// TEST(CudaGoldCodeTest, one_resample_and_correlate_in_CUDA)
// {

//     GPS_IQ_reader reader;
//     reader.open(FILE_PATH_GPS_IQ_SAMPLE1);
//     reader.seekSample(0x4000);

//     std::vector<std::complex<float>> iq_samples;
//     reader.readSamples(CHIPS_PER_MS, iq_samples); // Read 10 ms of IQ samples
//     int max_cross = 0;
//     vector<int> output1(1023);
//     CA_generator ca;
//     int i = 1; // test for satelite 1
//     ca.get_gold_code_sequence(i, output1);

//     BasebandGenerator bg;
//     vector<std::complex<float>> local_gold_code = bg.resampleCaGoldCodeTOneMilisecondOfBaseband(output1, 0);

//     printf("Processing Sat #%d\n", i);
//     int max_lag = 0;
//     float max_freq = 0;
//     float freq = 1000;
// // for (float freq = -5000; freq <= 5000; freq += 500) {
//     auto baseband_signal1 = bg.resampleInputSignalToBaseband(iq_samples, freq );//- 10e6 );

//     int lag = 0;
//     // for (int lag = 0 ; lag < CHIPS_PER_MS ; lag+=3) {
//     auto cross = (int)bg.crossCorrelation(local_gold_code, baseband_signal1, lag);

//     // Now make it run all in CUDA
//     auto cross_CUDA = (int)crossCorrelationCUDA(local_gold_code, baseband_signal1, lag);


//     EXPECT_EQ(cross_CUDA, cross);

// }

// TEST(CudaGoldCodeTest, resample_and_correlate_in_one_kernel_CUDA)
// {

//     GPS_IQ_reader reader;
//     reader.open(FILE_PATH_GPS_IQ_SAMPLE1);
//     reader.seekSample(0x4000);

//     std::vector<std::complex<float>> iq_samples;
//     reader.readSamples(CHIPS_PER_MS, iq_samples); // Read 10 ms of IQ samples
//     int max_cross = 0;
//     vector<int> goldCode(1023);
//     CA_generator ca;
//     int i = 1; // test for satelite 1
//     ca.get_gold_code_sequence(i, goldCode);
//     float freqShiftHz = 0;//1000;
//     int lag = 0;



//     BasebandGenerator bg;
//     vector<std::complex<float>> local_gold_code = bg.resampleCaGoldCodeTOneMilisecondOfBaseband(goldCode, 0);

//     printf("Processing Sat #%d\n", i);

//     auto baseband_signal1 = bg.resampleInputSignalToBaseband(iq_samples,    freqShiftHz );//- 10e6 );

//     auto cross = (int)bg.crossCorrelation(local_gold_code, baseband_signal1, lag);

//     auto cross_cuda = freq_shift_correlateCUDA(goldCode, freqShiftHz , iq_samples,  lag) ;
//     EXPECT_EQ(cross_cuda, cross);

// }


TEST(CudaGoldCodeTest, findOneSateCUDA)
{

    GPS_IQ_reader reader;
    reader.open(FILE_PATH_GPS_IQ_SAMPLE1);
    reader.seekSample(45000);

    std::vector<std::complex<float>> iq_samples;
    reader.readSamples(CHIPS_PER_MS, iq_samples); // Read 10 ms of IQ samples
    int max_cross = 0;
    vector<int> goldCode(1023);
    CA_generator ca;

    printf("Running on file :%s\n", FILE_PATH_GPS_IQ_SAMPLE1);

    // for (int i  = 0; i < 32; i++)
    // for (int i = 10; i < 14; i++)
    int i =12;
    {

        // int i = 4; // test for satelite 1
        ca.get_gold_code_sequence(i, goldCode);
        float freqShiftHz = 0;//1000;
        int lag = 0;

        printf("Processing Sat #%d\n", i);
        // for (freqShiftHz = -5000; freqShiftHz <= 5000; freqShiftHz += 500) {

        //     for (lag = 0 ; lag < CHIPS_PER_MS ; lag+=3) {
            for (int samples = 0 ; samples < 100; samples++) {
                reader.readSamples(CHIPS_PER_MS, iq_samples); // Read 10 ms of IQ samples

                // skip 100 ms
                for (int y = 0 ; y < 100 ; y++)
                {
                    iq_samples.clear();
                    reader.readSamples(CHIPS_PER_MS, iq_samples);
                }

                 // go over the entire signal and add average to remove HackRF DC spike
                static float avgI = 0.0f;
                static float avgQ = 0.0f;

                for (size_t idx = 0; idx < iq_samples.size(); idx++) {
                    avgI += iq_samples[idx].real();
                    avgQ += iq_samples[idx].imag();
                }

                avgI /= iq_samples.size();
                avgQ /= iq_samples.size();

                for (size_t idx = 0; idx < iq_samples.size(); idx++) {
                    iq_samples[idx] -= std::complex<float>(avgI, avgQ);
                }

                auto cross_cuda_complex = freq_shift_correlateCUDA(goldCode, freqShiftHz , iq_samples,  lag) ;
                iq_samples.clear();

                // auto cross_cuda = (int)abs(cross_cuda_complex);
                // if (cross_cuda > max_cross) {
                //     max_cross = cross_cuda;
                //     printf( "Sat #%d freqShiftHz:%f Lag:%d Cross:%d\n", i, freqShiftHz, lag, cross_cuda);
                // }
            }
        // }
    }
}


/**
 * Checks the parity of a received 30-bit GPS word.
 *
 * @param received_word The 30-bit word received from the satellite (Bits 1-30 aligned as MSB-to-LSB).
 * @param prev_d29      Bit 29 from the previous GPS word (0 or 1).
 * @param prev_d30      Bit 30 from the previous GPS word (0 or 1).
 * @param out_data      Pointer to store the corrected, recovered 24-bit raw data.
 * @return true if the parity check passes, false otherwise.
 */
bool check_gps_parity(uint32_t received_word, uint8_t prev_d29, uint8_t prev_d30, uint32_t *out_data) {
    // Extract transmitted bits from the 30-bit word (received_word contains bits 30 down to 1)
    // Shift maps D1 -> bit 29 down to D30 -> bit 0
    uint32_t D[31];
    for (int i = 1; i <= 30; i++) {
        D[i] = (received_word >> (30 - i+2  )) & 1;
    }

    printf("Received Word: 0x%08X  prev_d29: %d prev_d30: %d\n", received_word, prev_d29, prev_d30);
    printf("Extracted Bits: ");
    for (int i = 1; i <= 30; i++) {
        if ((i-1)%10 == 0)
            printf("|");
        printf("%d", D[i]);
    }
    printf("\n");

    // Step 1: Recover raw data bits (d1 to d24) by checking previous word's D30 bit
    uint32_t d[25];
    for (int i = 1; i <= 24; i++) {

        // d[i] ^= 1;
        if (prev_d30 == 1) {
            d[i] = D[i] ^ 1; // Invert if previous D30 was 1
        } else {
            d[i] = D[i];     // Keep unchanged if previous D30 was 0
        }
    }

    // Step 2: Compute parity bits (D25 to D30) based on IS-GPS-200 matrix equations
    uint32_t W25 = prev_d29 ^ d[1] ^ d[2] ^ d[3] ^ d[5] ^ d[6] ^ d[10] ^ d[11] ^ d[12] ^ d[13] ^ d[14] ^ d[17] ^ d[18] ^ d[20] ^ d[23];
    uint32_t W26 = prev_d30 ^ d[2] ^ d[3] ^ d[4] ^ d[6] ^ d[7] ^ d[11] ^ d[12] ^ d[13] ^ d[14] ^ d[15] ^ d[18] ^ d[19] ^ d[21] ^ d[24];
    uint32_t W27 = prev_d29 ^ d[1] ^ d[3] ^ d[4] ^ d[5] ^ d[7] ^ d[8] ^ d[12] ^ d[13] ^ d[14] ^ d[15] ^ d[16] ^ d[19] ^ d[20] ^ d[22];
    uint32_t W28 = prev_d30 ^ d[2] ^ d[4] ^ d[5] ^ d[6] ^ d[8] ^ d[9] ^ d[13] ^ d[14] ^ d[15] ^ d[16] ^ d[17] ^ d[20] ^ d[21] ^ d[23];
    uint32_t W29 = prev_d30 ^ d[1] ^ d[3] ^ d[5] ^ d[6] ^ d[7] ^ d[9] ^ d[10] ^ d[14] ^ d[15] ^ d[16] ^ d[17] ^ d[18] ^ d[21] ^ d[22] ^ d[24];
    uint32_t W30 = prev_d29 ^ d[3] ^ d[5] ^ d[6] ^ d[8] ^ d[9] ^ d[11] ^ d[13] ^ d[15] ^ d[19] ^ d[22] ^ d[23] ^ d[24];


    printf("Calculated Parity: W25=%d, W26=%d, W27=%d, W28=%d, W29=%d, W30=%d\n", W25, W26, W27, W28, W29, W30);


    //exit(123);
    // Step 3: Compare calculated parity against received parity bits
    if (W25 == D[25] && W26 == D[26] && W27 == D[27] &&
        W28 == D[28] && W29 == D[29] && W30 == D[30]) {

        // Pass: Pack recovered data array back into a compact uint32_t container
        uint32_t data = 0;
        for (int i = 1; i <= 24; i++) {
            data = (data << 1) | d[i];
        }
        *out_data = data;
        printf("Parity check succesful \n");
        return true;
    }

    printf("Oh, no! Parity check failed \n");
    return false; // Parity failure
}

uint32_t reverse_bits(uint32_t num) {
    uint32_t result = 0;
    for (int i = 0; i < 32; i++) {
        result <<= 1;          // Shift the result to make room for the next bit
        result |= (num & 1);   // Grab the last bit of num and add it to result
        num >>= 1;             // Shift num to process the next bit
    }
    return result;
}

bool check_gps_32bit_parity(uint32_t data) {

   uint32_t recovered_data = 0;

   printf("Checking 32-bit data: 0x%08X\n", data);

    if (check_gps_parity(reverse_bits(data),
                (data & 0x40000000) == 0 ? 0 :1,
                (data & 0x80000000) == 0 ? 0 :1 , &recovered_data)) {
        printf("Parity Check: PASSED\n");
        printf("Recovered Data: 0x%06X\n", recovered_data);
        return true;
    } else {
        printf("Parity Check: FAILED\n");
        return false;
    }
}

std::vector<std::complex<float>> run_fft_and_print_freq(std::vector<std::complex<float>> subinput)
{

    // std::vector<std::complex<float>> subinput;
    auto fft_res = ft::fft_from_complex(subinput);

    float max_abs = 0.0f;
    int max_idx = 0;
    for(int i =1 ; i < fft_res.size(); i++) {
        if (max_abs < abs(fft_res[i])) {
            max_abs = abs(fft_res[i]);
            max_idx = i;
        }
    }

    printf("Max FFT index: %d  Max FFT abs: %.3f\n", max_idx, max_abs/1000000);

    for(int i= 0 ; i < fft_res.size(); i++) {
        printf("(%d)ABS:%.3f i=%.3f  j=%.3f\n", i,abs(fft_res[i]) /1000000,fft_res[i].real()/1000, fft_res[i].imag()/1000);
    }

    subinput.clear();
    printf("After clearing subinput:\n");
    for(int i= 0 ; i < fft_res.size(); i++) {
        fft_res[i] = 0;
        printf("(%d)ABS:%.3f i=%.3f  j=%.3f\n", i,abs(fft_res[i]) /1000000,fft_res[i].real()/1000, fft_res[i].imag()/1000);
    }


    fft_res[max_idx/2] = max_abs;
    ft::ifft_inplace(fft_res);

    printf("After IFFT:\n");
    for(int i =0 ; i < fft_res.size(); i++) {
        printf("(%d)ABS:%.3f i=%.3f  j=%.3f\n", i,abs(fft_res[i]) /1000000,fft_res[i].real()/1000, fft_res[i].imag()/1000);
    }

    return fft_res;

}


TEST(CudaGoldCodeTest, findOneSateCUDALimited5)
{

    GPS_IQ_reader reader;
    reader.open(FILE_PATH_GPS_IQ_SAMPLE1);
    reader.seekSample(45000);
    int data_bits[300];

    std::vector<std::complex<float>> iq_samples;
    reader.readSamples(CHIPS_PER_MS, iq_samples); // Read 10 ms of IQ samples
    int max_cross = 0;
    vector<int> goldCode(1023);
    CA_generator ca;

        // Example test case: A real GPS Telemetry (TLM) Word sample
    uint32_t test_word = 0x0000000C; // 30 bits: [MSB] 10001 00101 01011 01000 01010 11010 [LSB]
    uint8_t p_d29 = 0;
    uint8_t p_d30 = 0;
    uint32_t recovered_data = 0;

/**
 *
 *  if ((preamble & 0b11111111) == 0b00101110 || (preamble & 0b11111111) == 0b11010001) {
                        printf("\nFound reversed data: %X  preamble at  bit %d  sample %d \n",preamble, bit, samples);

                        if (check_gps_parity(reverse_bits(preamble),
                                 preamble & 0x40000000 == 0 ? 0 :1,
                                 preamble & 0x80000000 == 0 ? 0 :1 , &recovered_data)) {
                        printf("Parity Check: PASSED\n");
                        printf("Recovered Data: 0x%06X\n", recovered_data);
                    } else {
                        printf("Parity Check: FAILED\n");
                    }
                    }

 */


    // check_gps_32bit_parity(0x0000000C);
    check_gps_32bit_parity(0x00000003);
    // check_gps_32bit_parity(0x00000000);
    // if (check_gps_parity(test_word, p_d29, p_d30, &recovered_data)) {
    //     printf("Parity Check: PASSED\n");
    //     printf("Recovered Data: 0x%06X\n", recovered_data);
    // } else {
    //     printf("Parity Check: FAILED\n");
    // }

    // exit(0);
    printf("Running on file :%s\n", FILE_PATH_GPS_IQ_SAMPLE1);

    //for (int i = 0; i < 32; i++)
    // for (int i = 10; i < 14; i++)
    int i = 24;//24;//23;//28;//24;
    {

        // int i = 4; // test for satelite 1
        ca.get_gold_code_sequence(i, goldCode);
        float freqShiftHz = -2500;//-2500;//1500;//1000;//-2500;
        int lag = 21036;//21036;//10305;//8109;//21036;

        printf("Processing Sat #%d\n", i);
        // for (freqShiftHz = -5000; freqShiftHz <= 5000; freqShiftHz += 500) {
        const int SAMPLES_DATA_SIZE = 256;
        vector<std::complex<float>> samples_data = vector<std::complex<float>>(SAMPLES_DATA_SIZE);
        int lagShift = 0;
        int freqShift = 0;
        int prevIsign = 1, lastIsign = 1 ;
        int prevRsign = 1, lastRsign = 1;
        int bit = 0;
        int wasSignChange = 0;
        int bitCount = 0;
        uint32_t preamble = 0 ;
        //     for (lag = 0 ; lag < CHIPS_PER_MS ; lag+=3) {
            for (int samples = 0 ; samples < 60000; samples++) {
                if (reader.readSamples(CHIPS_PER_MS, iq_samples) == 0) // Read 10 ms of IQ samples
                    break;

                 // go over the entire signal and add average to remove HackRF DC spike
                static float avgI = 0.0f;
                static float avgQ = 0.0f;

                for (size_t idx = 0; idx < iq_samples.size(); idx++) {
                    avgI += iq_samples[idx].real();
                    avgQ += iq_samples[idx].imag();
                }

                avgI /= iq_samples.size();
                avgQ /= iq_samples.size();

                for (size_t idx = 0; idx < iq_samples.size(); idx++) {
                    iq_samples[idx] -= std::complex<float>(avgI, avgQ);
                }

                gpsTrackingData trackingData = freq_shift_correlateCUDALimited5(goldCode, freqShiftHz , iq_samples,  lag, samples) ;
                iq_samples.clear();

                if (trackingData.lag < lag)
                {
                    lagShift--;
                }
                else if (trackingData.lag > lag)
                {
                    lagShift++;
                }
const int LOG_LOG = 1;

                // Track the lag, update them when not in sync.
                if (samples % 20 == 0) {
                    // printf("lagshift:%d\n", lagShift);
                    if (lagShift < -5) {
                        lag -= 3;
                        if (LOG_LOG)printf(">>l<<");
                    }
                    if (lagShift > 5) {
                        lag += 3;
                        if (LOG_LOG) printf(">>L<<");
                    }


                    lagShift = 0;
                }

                samples_data[samples % SAMPLES_DATA_SIZE] = trackingData.maxCrossCorrelation;
                if (samples % 20 == 0 ) {

                    bitCount++;

                    if (samples % SAMPLES_DATA_SIZE == 0 && samples > 0) {
                        auto fft_res = run_fft_and_print_freq(samples_data);

                        for(int i= 0 ; i < fft_res.size(); i++) {
                            std::complex<float> val = conj(fft_res[i]) *  samples_data[i];
                            printf("VAL:(%d)ABS:%.3f i=%.3f  j=%.3f\n", i,abs(val) /10000000,val.real()/10000, val.imag()/10000);
                        }

                    }

                    if (bitCount % 300 == 0) {
                        printf("|||");
                    } else if (bitCount %30 == 0) {
                        printf("|");
                    }

                    if (wasSignChange) {
                        bit ^= 1;
                    }

                    data_bits[bitCount % 300] = bit;


                    // printf("%d", bit);
                    if (LOG_LOG) printf("%d(%d)", bit, bitCount);


                    preamble = ((preamble << 1) | (bit & 0x1) ) ;//& 0xC0000000;// 0b11111111; ;

                    if ((preamble & 0b11111111) == 0b00101110 || (preamble & 0b11111111) == 0b11010001) {

                        printf("\nFound preamble at sample %d\n ----", samples);
                        for (int i = 0; i < 300; i++) {
                            printf("%d", data_bits[(bitCount-i)%300]);
                            if (i  % 30 == 29) {
                                printf("-");
                            }
                        }
                        printf("---\n");

                    }
                    if ((preamble & 0b11111111) == 0b00101110 || (preamble & 0b11111111) == 0b11010001) {
                        printf("\nFound reversed data: %X  preamble at  bitCount %d  sample %d \n",preamble, bitCount, samples);

                        if (check_gps_32bit_parity(preamble) || check_gps_32bit_parity(~preamble)) {
                            printf("Parity Check: PASSED\n");
                        // printf("Recovered Data: 0x%06X\n", recovered_data);
                        } else {
                            printf("Parity Check: FAILED\n");
                        }
                    }

                    wasSignChange = 0;
                }

                // if (samples % 2 == 0 )
                {
                    int signChange =   prevRsign * trackingData.maxCrossCorrelation.real() < 0 &&  prevIsign * trackingData.maxCrossCorrelation.imag() < 0 ? 1 : 0;
                    if (signChange) {
                        wasSignChange  = 1;
                    }
                    if (LOG_LOG) printf("Sat #%d signChange:%d freqShiftHz:%f Lag:%d Cross:%d R:%f I:%f Rsign:%d Isign:%d\n", i, signChange, trackingData.freqShiftHz, trackingData.lag,
                        (int)std::abs(trackingData.maxCrossCorrelation), trackingData.maxCrossCorrelation.real(), trackingData.maxCrossCorrelation.imag(),
                          prevRsign * trackingData.maxCrossCorrelation.real() > 0 ? 1 : -1, prevIsign * trackingData.maxCrossCorrelation.imag() > 0 ? 1 : -1
                        );


                    prevRsign = lastRsign;
                    prevIsign = lastIsign;

                    lastRsign = trackingData.maxCrossCorrelation.real() > 0 ? 1 : -1;
                    lastIsign = trackingData.maxCrossCorrelation.imag() > 0 ? 1 : -1;



                }

                // auto cross_cuda = (int)abs(cross_cuda_complex);
                // if (cross_cuda > max_cross) {
                //     max_cross = cross_cuda;
                //     printf( "Sat #%d freqShiftHz:%f Lag:%d Cross:%d\n", i, freqShiftHz, lag, cross_cuda);
                // }
            }
        // }
    }
}





TEST(CudaGoldCodeTest, findOneSate20miliApartCUDA)
{

    GPS_IQ_reader reader;
    reader.open(FILE_PATH_GPS_IQ_SAMPLE1);
    reader.seekSample(45000);

    std::vector<std::complex<float>> iq_samples;
    reader.readSamples(CHIPS_PER_MS, iq_samples); // Read 10 ms of IQ samples
    int max_cross = 0;
    vector<int> goldCode(1023);
    CA_generator ca;

    printf("Running on file :%s\n", FILE_PATH_GPS_IQ_SAMPLE1);

    // for (int i = 0; i < 32; i++)
    int i = 24;
    // for (int i = 10; i < 14; i++)
    // int i =13;
    while(1){

        // int i = 4; // test for satelite 1
        ca.get_gold_code_sequence(i, goldCode);
        float freqShiftHz = 0;//1000;
        int lag = 0;

        printf("Processing Sat #%d\n", i);
        // for (freqShiftHz = -5000; freqShiftHz <= 5000; freqShiftHz += 500) {

        //     for (lag = 0 ; lag < CHIPS_PER_MS ; lag+=3) {
            for (int samples = 0 ; samples < 1; samples++) {
                reader.readSamples(CHIPS_PER_MS, iq_samples); // Read 1 ms of IQ samples


                 // go over the entire signal and add average to remove HackRF DC spike
                static float avgI = 0.0f;
                static float avgQ = 0.0f;

                for (size_t idx = 0; idx < iq_samples.size(); idx++) {
                    avgI += iq_samples[idx].real();
                    avgQ += iq_samples[idx].imag();
                }

                avgI /= iq_samples.size();
                avgQ /= iq_samples.size();

                for (size_t idx = 0; idx < iq_samples.size(); idx++) {
                    iq_samples[idx] -= std::complex<float>(avgI, avgQ);
                }

                auto cross_cuda_complex = freq_shift_correlateCUDA(goldCode, freqShiftHz , iq_samples,  lag) ;

                 for (int j = 0; j < 19; j++) {
                    reader.readSamples(CHIPS_PER_MS, iq_samples); // Read 10 ms of IQ samples
                }

                iq_samples.clear();

                // auto cross_cuda = (int)abs(cross_cuda_complex);
                // if (cross_cuda > max_cross) {
                //     max_cross = cross_cuda;
                //     printf( "Sat #%d freqShiftHz:%f Lag:%d Cross:%d\n", i, freqShiftHz, lag, cross_cuda);
                // }
            }
        // }
    }
}




TEST(CudaGoldCodeTest, runOneSateMultipleChipsLimitedSearchCUDA)
{

    GPS_IQ_reader reader;
    reader.open(FILE_PATH_GPS_IQ_SAMPLE1);
    reader.seekSample(0x4000);

    std::vector<std::complex<float>> iq_samples;
    int max_cross = 0;
    vector<int> goldCode(1023);
    CA_generator ca;

    int satellite_id =28;//19;//10;//=4;
    ca.get_gold_code_sequence(satellite_id, goldCode);
    float freqShiftHz =2500.0;// 750;//1250;//-2500.000000;// -2750;
    int lag = 21036;// 8109;//16230;//3387;//5184;

    iq_samples.clear();

    printf("Processing Sat #%d\n", satellite_id);
    for (int i = 0;  i < 2000 ; i++) {
        // reader.seekSample(0x4000 + i*CHIPS_PER_MS);
        auto samples_out_num = reader.readSamples(CHIPS_PER_MS, iq_samples); // Read 1 ms of IQ samples

        // for (int i = 0; i < CHIPS_PER_MS; i++) {
        //     float gc = goldCode[(i/10+123)%1023] == 1 ? 1.0f : -1.0f;
        //     // float gc = goldCode[(i/10)%1023] == 1 ? 1.0f : -1.0f;
        //     iq_samples.push_back(std::complex<float>(gc* std::cos(2.0f * 3.14159265f * freqShiftHz * i / 10.23e6f),
        //                                              gc* std::sin(2.0f * 3.14159265f * freqShiftHz * i / 10.23e6f)));
        //                                             // goldCode[(i/10+123)%1023] * std::sin(2.0f * 3.14159265f * freqShiftHz * i / 10.23e6f)));
        // }

        auto cross_cuda = freq_shift_correlateLimitedSearchCUDA(goldCode, freqShiftHz , iq_samples,  lag, i) ;
        iq_samples.clear();
        // printf("Sample %d: Cross:(%f, %f) ", i, cross_cuda.real(), cross_cuda.imag());
    }

}


// Main function to run all tests
int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv); // Initialize Google Test
    return RUN_ALL_TESTS();                 // Run all defined tests
}
