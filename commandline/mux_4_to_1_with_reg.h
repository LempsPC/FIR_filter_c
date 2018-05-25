#include "/cad/sysC/2.2/include/systemc.h"

SC_MODULE (mux_4_to_1_with_reg)
{
    sc_in_clk clk;
    sc_in <sc_uint<8> > in0, in1, in2, in3;
    sc_in <sc_uint<2> > select;
    
    sc_out <sc_uint<8> > output; // Register output
    
    sc_signal <sc_uint<8> > mux_out;
    
    void process_mux();
    void process_output_reg();
    
    SC_CTOR (mux_4_to_1_with_reg)
    {
        SC_METHOD (process_mux);
        sensitive << in0 << in1 << in2 << in3 << select;
        
        SC_METHOD (process_output_reg);
        sensitive << clk.pos();
    }
    
    
};
