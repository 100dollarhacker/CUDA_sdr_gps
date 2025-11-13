

#include "C_A__code_generator.h"


using namespace std;

int CA_generator::get_gold_code_sequence(int satelite_num, vector<int>& output) {

    for (int i = 0 ; i < 1023 ; i++)
    {
        // generate one bit of gold code
        int  next_bit = get_data_from_selector(satelite_num) ^ get_data_from_G1();
        // update G1
        G1_cycle();
        // update G2
        G2_cycle();

        output[i] = next_bit;
    }

    return 0;
}

void CA_generator::shift_left(vector<int>&  Gx )
{
    for (int i = Gx.size()-1; i  > 0 ; i--) {
        Gx[i] = Gx[i-1];
    }
}

void CA_generator::G1_cycle() {
    int next_cell_0_value = (G1[2]) ^ (G1[9]);
    shift_left(G1);
    G1[0] = next_cell_0_value;
}

void CA_generator::G2_cycle() {

    bool next_cell_0_value = (G2[1]) ^ (G2[2]) ^ (G2[5]) ^ (G2[7]) ^ (G2[8]) ^ (G2[9])  ;
    shift_left(G2);
    G2[0] = next_cell_0_value;
}

int CA_generator::get_data_from_selector(int satelite_num)
{
    return G2[get<0>(tap_array[satelite_num])]  ^ G2[get<1>(tap_array[satelite_num])] ;
}

int CA_generator::get_data_from_G1()
{
    return G1[9];
}



