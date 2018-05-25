#include "systemc.h"
#include "mux_4_to_1_with_reg.h"

SC_MODULE(t_mux)
{
    sc_signal<bool> clk;
    sc_signal<sc_uint<8> > t_in0, t_in1, t_in2, t_in3;
    sc_signal<sc_uint<2> > t_select;
    sc_signal<sc_uint<8> > t_output;
    
    mux_4_to_1_with_reg* mux_instance;
    
    void proc_clk(); // Using separate process for generating clock with PERIOD = 20 ns
    void stimulus(); // Process for applying input values and select to the 4 to 1 MUX
    
    SC_CTOR(t_mux):clk("clk"),t_in0("t_in0"),t_in1("t_in1"),t_in2("t_in2"), t_in3("t_in3"), t_select("t_select"),
    t_output("t_output"){
        cout<<"t_mux constructor"<<endl;
        mux_instance=new mux_4_to_1_with_reg("mux_4_to_1_with_reg");
        mux_instance->clk(clk);
        mux_instance->in0(t_in0);
        mux_instance->in1(t_in1);
        mux_instance->in2(t_in2);
        mux_instance->in3(t_in3);
        mux_instance->select(t_select);
        mux_instance->output(t_output);
        
        SC_THREAD(proc_clk);
        SC_THREAD(stimulus);
    }
};

SC_MODULE_EXPORT(t_mux);

void t_mux::proc_clk()
{
    while (1)
    {
        clk.write (false);
        wait (10,SC_NS);
        
        clk.write (true);
        wait (10,SC_NS);
    }
}

void t_mux::stimulus(){
    
    while (1)
    {
        t_in0.write(15);
        t_in1.write(98);
        t_in2.write(3);
        t_in3.write(26);
        
        
        t_select.write(0);
        wait(20,SC_NS);
        
        
        t_select.write(1);
        wait(20,SC_NS);
        
        
        t_select.write(2);
        wait(20,SC_NS);
        
        
        t_select.write(3);
        wait(20,SC_NS);
        
        wait();
        
    }
    
}

