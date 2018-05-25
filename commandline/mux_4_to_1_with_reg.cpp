#include "mux_4_to_1_with_reg.h"

void mux_4_to_1_with_reg::process_mux()
{
    switch (select.read())
    {
        case 0: // "00"
            mux_out.write(in0.read());
            break;
            
        case 1: // "01"
            mux_out.write(in1.read());
            break;
            
        case 2: // "10"
            mux_out.write(in2.read());
            break;
            
        default: // "11"
            mux_out.write(in3.read());
            break;
    }
}

void mux_4_to_1_with_reg::process_output_reg()
{
    output.write(mux_out.read());
}
