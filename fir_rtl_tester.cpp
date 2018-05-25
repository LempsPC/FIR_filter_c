//#include "systemc.h"
#include "fir_rtl_design_3.h"



SC_MODULE(fir_tester)
{
    sc_signal<bool> sample, CLK;
    sc_signal<double> data_in;
    sc_signal<double> data_out;
    

    fir* fir_instance;
    
    void proc_clk(); // Using separate process for generating clock with PERIOD = 20 ns
    void sampler();
    void stimuli(); // Process for applying input values and select to the 4 to 1 MUX
    
    SC_CTOR(fir_tester):sample("sample"),CLK("CLK"),data_in("data_in"),data_out("data_out")
    {
        cout<<"fir filter constructor"<<endl;
        fir_instance=new fir("fir");
        fir_instance->CLK(CLK);
        fir_instance->sample(sample);
        fir_instance->data_in(data_in);
        fir_instance->data_out(data_out);
        
        SC_THREAD(proc_clk);
        SC_THREAD(sampler);
        SC_THREAD(stimuli);
    }
};

SC_MODULE_EXPORT(fir_tester);

void fir_tester::proc_clk()
{
    while (1)
    {
        CLK.write (false);
        wait (10,SC_NS);
        
        CLK.write (true);
        wait (10,SC_NS);
    }
}

void fir_tester::sampler()
{
    while(1)
    {
        sample.write(true);
        wait (20,SC_NS);
        sample.write(false);
        wait (60,SC_NS);
    }
}

void fir_tester::stimuli()
{
    while(1)
    {
        wait(240,SC_NS);
        data_in.write(1);
        wait(80,SC_NS);
        data_in.write(0);
    }
}