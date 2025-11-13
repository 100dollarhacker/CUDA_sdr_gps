#include <vector>
#include "C_A__code_generator.h"
#include "baseband_generator.h"

int main() {
    vector<int> output1(1023), output2(1023);
    CA_generator ca;
    ca.get_gold_code_sequence(1, output1);
    ca.get_gold_code_sequence(2, output2);

    BasebandGenerator bg;
    auto baseband_signal1 = bg.resampleCaGoldCodeTOneMilisecondOfBaseband(output1);
    auto baseband_signal2 = bg.resampleCaGoldCodeTOneMilisecondOfBaseband(output2);

    auto cross = bg.crossCorrelation(baseband_signal1, baseband_signal2, 0);
}