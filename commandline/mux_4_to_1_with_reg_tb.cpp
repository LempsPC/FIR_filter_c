#include "mux_4_to_1_with_reg.h"

SC_MODULE (driver)
{
    sc_out <sc_uint<8> > d_in0, d_in1, d_in2, d_in3;
    sc_out <sc_uint<2> > d_select;

    void prc_driver() // Process for applying inputs and select to MUX
    {
        while (1)
        {
            d_in0 = 0;
            d_in1 = 0;
            d_in2 = 0;
            d_in3 = 0;
            d_select = 0;
            
            wait(20, SC_NS);
            
            d_in0 = 2;
            d_in1 = 4;
            d_in2 = 5;
            d_in3 = 8;
            d_select = 0;
            
            wait(20, SC_NS);

            d_in0 = 2;
            d_in1 = 4;
            d_in2 = 5;
            d_in3 = 8;
            d_select = 1;

            wait(20, SC_NS);
            
            d_in0 = 2;
            d_in1 = 4;
            d_in2 = 5;
            d_in3 = 8;
            d_select = 2;
            
            wait(1, SC_NS);

            d_in0 = 2;
            d_in1 = 4;
            d_in2 = 5;
            d_in3 = 8;
            d_select = 3;
            
            wait(20, SC_NS);
            
            sc_stop();

        }
    }
    
    SC_CTOR(driver) {
        SC_THREAD(prc_driver);
    }
};

int sc_main (int argc, char * argv[])
{
    sc_clock t_clk("t_clk", 20,SC_NS, 0.5, 0,SC_NS, false); // Instead of generating clock using a seperate process, we have used SystemC's clock notation.
    sc_signal <sc_uint<8> > t_in0, t_in1, t_in2, t_in3;
    sc_signal <sc_uint<2> > t_select;
    sc_signal <sc_uint<8> > mux_out;

    // DRIVER
    driver DRIVER ("DRIVER");
    DRIVER.d_in0    (t_in0);
    DRIVER.d_in1    (t_in1);
    DRIVER.d_in2    (t_in2);
    DRIVER.d_in3    (t_in3);
    DRIVER.d_select (t_select);
    
    // MAIN MODULE
    mux_4_to_1_with_reg MUX ("MUX_4_to_1");
    MUX.clk  (t_clk);
    MUX.in0  (t_in0);
    MUX.in1  (t_in1);
    MUX.in2  (t_in2);
    MUX.in3  (t_in3);
    MUX.select  (t_select);
    MUX.output (mux_out);
    
    // Dumping signals into .vcd file (for viewing using GTKWave)
    sc_trace_file *trace = sc_create_vcd_trace_file ("MUX_4_to_1_with_reg");
    
    sc_trace (trace, t_clk                              , "clk");
    sc_trace (trace, t_in0                              , "in0");
    sc_trace (trace, t_in1                              , "in1");
    sc_trace (trace, t_in2                              , "in2");
    sc_trace (trace, t_in3                              , "in3");
    sc_trace (trace, t_select                           , "select");
    sc_trace (trace, mux_out                              , "out");
			
    sc_start();
    
    sc_close_vcd_trace_file(trace);

    return 0;
}

