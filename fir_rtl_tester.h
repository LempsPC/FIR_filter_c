#include "systemc.h"
#include "fir_rtl_design_3.h"

int tester_main (int argc, char argv[])
{
    sc_clock tester_clk ("CLK", 20, SC_NS, 0.5, 5, SC_NS, false); // makes clock go

    sc_signal<bool> tester_sample;
    sc_signal<double>tester_data_in;
    sc_signal<double>tester_data_out;

    fir_rtl RTL_DESIGN ("RTL");
    RTL_DESIGN.CLK (tester_clk);
    RTL_DESIGN.sample (tester_sample);
    RTL_DESIGN.data_in (tester_data_in);
    RTL_DESIGN.data_out (tester_data_out);


    SC_THREAD(stimuli);
}

void tester_main::stimuli()
{
    while(1)
    {
        tester_sample.write(1);
        wait(20, SC_NS);
        tester_sample.write(0);
        wait(60, SC_NS);
    }
}