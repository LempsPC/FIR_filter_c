/*

---------------------------------------------------------------------

--  9-tab FIR filter --

--  Refined from the RTL model [fir_filter_syn(behave_rtl_2)]

--    by creating explicit state machine

---------------------------------------------------------------------

--  Data word - 16 bits: sign, 5 bits left & 10 bits right of point

--    "sddddd.ffffffffff"  ["d"~integer, "f"~fraction]

--    1.0 == "0000010000000000", 0.125 == "0000000010000000", etc.

---------------------------------------------------------------------

*/

#include "systemc.h"
#include "sc_fixed.h"

using namespace sc_dt;

enum state_type {S0, S1, S2, S3};

SC_MODULE(fir)
{
    sc_in<bool>      CLK;
    sc_in<bool>      sample;
    sc_in<double >   data_in;
    sc_out<double >  data_out;
    
    // State - type & signals
    
    sc_signal<state_type> state, next_state;

    // Signals
    
    sc_signal<sc_fixed<16,6> > del_1, del_2, del_3, del_4, del_5, del_6, del_7, del_8;
    sc_signal<sc_fixed<16,6> > data_in_bf, reg1, reg2, reg3, reg4;
    sc_signal<sc_fixed<16,6> > add1_out, add2_out, addsub3_out, sub4_out;
    
    // Shifters
    
    sc_fixed<16,6> asr3 (sc_fixed<16,6> inp)
    {
        return (inp >> 3);
    }
    
    sc_fixed<16,6> asr2 (sc_fixed<16,6> inp)
    {
        return (inp >> 2);
    }

    //    sc_fixed<16,6> coefs[9] = {0.125, 0.25, -0.75, 1.25, 1.0, 1.25, -0.75, 0.25, 0.125};
    
    // Moore FSM & registers
    void prc_next_state(); // Next state function of the state machine
    void prc_state_register(); //  State register
    void prc_inout_buf(); // Input/output buffers
    void prc_data_reg(); // Data registers
    void prc_shift_reg(); // Shift register
    
    void prc_add1(); // Adder #1 and its multiplexers
    void prc_add2(); // Adder #2 and its multiplexers
    void prc_addsub3(); // Adder #3 and its multiplexers
    void prc_sub4(); // Subtracter #1 and its multiplexers
    
    SC_CTOR(fir)
    {
        SC_METHOD (prc_next_state);
        sensitive << state << sample;

        SC_THREAD (prc_state_register);
        sensitive << CLK.pos();
        
        SC_THREAD (prc_inout_buf);
        sensitive << CLK.pos();

        SC_THREAD (prc_data_reg);
        sensitive << CLK.pos();
 
        SC_THREAD (prc_shift_reg);
        sensitive << CLK.pos();

        SC_METHOD (prc_add1);
        sensitive << del_1 << del_7 << data_in_bf << del_8 << reg1 << reg2 << reg4 << state;
        
        SC_METHOD (prc_add2);
        sensitive << del_2 << del_6 << del_4 << reg1 << state;
        
        SC_METHOD (prc_addsub3);
        sensitive << del_3 << del_5 << reg3 << state;
        
        SC_METHOD (prc_sub4);
        sensitive << reg2 << reg3 << reg4 << state;
    }
};
