// // gps_bitstream_parser.h
// #ifndef GPS_BITSTREAM_PARSER_H
// #define GPS_BITSTREAM_PARSER_H

// #include "baseband_generator.h"
// #include <cstdint>
// #include <vector>
// #include <stdexcept>

// class GpsBitstreamParser {
// private:
//     std::vector<uint8_t> bitstream;
//     size_t bitIndex;

//     uint32_t extractBits(size_t numBits, bool isSigned = false) {
//         if (bitIndex + numBits > bitstream.size() * 8) {
//             throw std::runtime_error("Bitstream overflow");
//         }

//         uint32_t value = 0;
//         for (size_t i = 0; i < numBits; ++i) {
//             size_t byteIdx = (bitIndex + i) / 8;
//             size_t bitIdx = 7 - ((bitIndex + i) % 8);
//             uint8_t bit = (bitstream[byteIdx] >> bitIdx) & 1;
//             value = (value << 1) | bit;
//         }

//         bitIndex += numBits;

//         // Handle two's complement for signed values
//         if (isSigned && (value & (1U << (numBits - 1)))) {
//             value |= (~0U << numBits);
//         }

//         return value;
//     }

// public:
//     GpsBitstreamParser(const std::vector<uint8_t>& data)
//         : bitstream(data), bitIndex(0) {}

//     GpsEphemeris parseEphemerisSubframe() {
//         GpsEphemeris eph;

//         // Subframe 1: Clock and health data
//         bitIndex = 0;
//         extractBits(8);  // Preamble
//         uint32_t sfId1 = extractBits(3);

//         if (sfId1 == 1) {
//             eph.week = extractBits(10);
//             extractBits(2);  // Reserved
//             uint32_t l2Code = extractBits(2);
//             extractBits(4);  // Reserved
//             extractBits(11); // IDOT
//             eph.iode = extractBits(8);
//             eph.toc = extractBits(16) * 16;

//             int af2 = extractBits(8, true);
//             int af1 = extractBits(11, true);
//             int af0 = extractBits(22, true);

//             eph.af2 = af2 * pow(2.0, -55);
//             eph.af1 = af1 * pow(2.0, -43);
//             eph.af0 = af0 * pow(2.0, -31);
//         }

//         // Subframe 2: Ephemeris (orbit)
//         bitIndex = 0;
//         extractBits(8);  // Preamble
//         uint32_t sfId2 = extractBits(3);

//         if (sfId2 == 2) {
//             eph.iode = extractBits(8);
//             int crs = extractBits(16, true);
//             int dn = extractBits(16, true);
//             int m0 = extractBits(32, true);
//             int cuc = extractBits(16, true);
//             uint32_t ecc = extractBits(32);
//             int cus = extractBits(16, true);
//             uint32_t sqrtA = extractBits(32);
//             eph.toe = extractBits(16) * 16;

//             eph.crs = crs * pow(2.0, -5);
//             eph.dn = dn * pow(2.0, -43) * M_PI;
//             eph.m0 = m0 * pow(2.0, -31) * M_PI;
//             eph.cuc = cuc * pow(2.0, -29);
//             eph.ecc = ecc * pow(2.0, -33);
//             eph.cus = cus * pow(2.0, -29);
//             eph.sqrtA = sqrtA * pow(2.0, -19);
//         }

//         // Subframe 3: Ephemeris (orbit continued)
//         bitIndex = 0;
//         extractBits(8);  // Preamble
//         uint32_t sfId3 = extractBits(3);

//         if (sfId3 == 3) {
//             int cic = extractBits(16, true);
//             int omega0 = extractBits(32, true);
//             int cis = extractBits(16, true);
//             int i0 = extractBits(32, true);
//             int crc = extractBits(16, true);
//             int w = extractBits(32, true);
//             int omegaDot = extractBits(24, true);
//             extractBits(8);  // Reserved
//             int iDot = extractBits(14, true);
//             extractBits(2);  // Reserved

//             eph.cic = cic * pow(2.0, -29);
//             eph.omega0 = omega0 * pow(2.0, -31) * M_PI;
//             eph.cis = cis * pow(2.0, -29);
//             eph.i0 = i0 * pow(2.0, -31) * M_PI;
//             eph.crc = crc * pow(2.0, -5);
//             eph.w = w * pow(2.0, -31) * M_PI;
//             eph.omegaDot = omegaDot * pow(2.0, -43) * M_PI;
//             eph.iDot = iDot * pow(2.0, -43) * M_PI;
//         }

//         eph.toc = eph.toe;
//         return eph;
//     }

//     void reset() {
//         bitIndex = 0;
//     }
// };

// #endif // GPS_BITSTREAM_PARSER_H
