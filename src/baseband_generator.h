#ifndef BASEBAND_GENERATOR_H
#define BASEBAND_GENERATOR_H

#include <vector>
#include <complex>
#include <algorithm>
#include <cmath>

class BasebandGenerator {
public:
    // Calculate cross-correlation between two gold codes
    static float crossCorrelation(
        const std::vector<std::complex<float>>& signal1,
        const std::vector<std::complex<float>>& signal2,
        int lag) {


        size_t n = signal1.size();

        // Cross-correlation: sum of products at each lag
        std::complex<float> sum = std::complex<float>(0.0f, 0.0f);
        for (size_t i = 0; i < n; ++i) {
            // printf("sum before:%f + %fi\n",sum.real(), sum.imag());
            sum += signal1[i] * signal2[(i + lag)%n];
        }

        return std::abs(sum);
    }

    // Convert gold codes to baseband (complex-valued signal)
    static std::vector<std::complex<float>> resampleCaGoldCodeTOneMilisecondOfBaseband(
        const std::vector<int>& goldCode) {

        std::vector<std::complex<float>> baseband(10230);
        size_t n = baseband.size();

        // Modulate with complex exponential carrier
        for (size_t i = 0; i < n; ++i) {
            float inPhase = goldCode[i/10] == 1 ? 1 : -1;

            // Apply carrier demodulation
            baseband[i] = std::complex<float>( inPhase  ,0  );
        }

        return baseband;
    }
};

#endif // BASEBAND_GENERATOR_H
