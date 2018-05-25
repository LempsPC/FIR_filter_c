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

#include "fir_rtl_design_3.h"
#include "sc_fixed.h"

#define DONT_CARE 0

using namespace sc_dt;

void fir::prc_next_state()
{
    switch (state.read())
    {
        case S0:
            if (sample.read() == true)
                next_state.write (S1);
            else
                next_state.write (S0);
            break;
            
        case S1:
            next_state.write (S2);
            break;
            
        case S2:
            next_state.write (S3);
            break;
            
        case S3:
            next_state.write (S0);
            break;
    }
}

void fir::prc_state_register()
{
    while (1)
    {
        wait();
        
        state.write (next_state.read());
    }
}

void fir::prc_inout_buf()
{
    while (1)
    {
        wait();

        if(state.read() == S0)
        {
            data_out.write (reg1.read());
            data_in_bf.write (data_in.read());
        }
    }
}

void fir::prc_data_reg()
{
    while (1)
    {
        wait();

        reg1.write (add1_out.read());
        reg2.write (add2_out.read());
        reg3.write (addsub3_out.read());
        reg4.write (sub4_out.read());
    }
}

void fir::prc_shift_reg()
{
    while (1)
    {
        wait();
        
        if (state.read() == S3)
        {
            del_8.write (del_7.read());
            del_7.write (del_6.read());
            del_6.write (del_5.read());
            del_5.write (del_4.read());
            del_4.write (del_3.read());
            del_3.write (del_2.read());
            del_2.write (del_1.read());
            del_1.write (data_in_bf.read());
        }
    }
}

void fir::prc_add1()
{
    sc_fixed<16,6> op1, op2;
    
    switch (state.read())
    {
        case S0: // "00"
            op1 = del_1.read();
            op2 = del_7.read();
            break;
            
        case S1:  // "01"
            op1 = data_in_bf.read();
            op2 = del_8.read();
            break;
            
        case S2:  // "10"
            op1 = asr3(reg1.read());
            op2 = reg2.read();
            break;
            
        case S3:  // "11"
            op1 = reg1.read();
            op2 = reg4.read();
            break;
    }
    
    add1_out.write (op1 + op2);
}

void fir::prc_add2()
{
    sc_fixed<16,6> op1, op2;
    
    switch (state.read())
    {
        case S0: // "00"
            op1 = del_2.read();
            op2 = del_6.read();
            break;
            
        case S1:  // "01"
            op1 = del_4.read();
            op2 = asr2(reg1.read());
            break;
            
        case S2:  // "10"
            op1 = DONT_CARE;
            op2 = DONT_CARE;
            break;
            
        case S3:  // "11"
            op1 = DONT_CARE;
            op2 = DONT_CARE;
            break;
    }
    
    add2_out.write (op1 + op2);
}

void fir::prc_addsub3()
{
    sc_fixed<16,6> op1, op2;
    
    switch (state.read())
    {
        case S0: // "00"
            op1 = del_3.read();
            op2 = del_5.read();
            break;
            
        case S1:  // "01"
            op1 = reg3.read();
            op2 = asr2(reg3.read());
            break;
            
        case S2:  // "10"
            op1 = DONT_CARE;
            op2 = DONT_CARE;
            break;
            
        case S3:  // "11"
            op1 = DONT_CARE;
            op2 = DONT_CARE;
            break;
    }
    
    addsub3_out.write (op1 + op2);
}


void fir::prc_sub4()
{
    sc_fixed<16,6> op1, op2;
    
    switch (state.read())
    {
        case S0: // "00"
            op1 = DONT_CARE;
            op2 = DONT_CARE;
            break;
            
        case S1:  // "01"
            op1 = reg2.read();
            op2 = asr2(reg2.read());
            break;
            
        case S2:  // "10"
            op1 = reg3.read();
            op2 = reg4.read();
            break;
            
        case S3:  // "11"
            op1 = DONT_CARE;
            op2 = DONT_CARE;
            break;
    }
    
    sub4_out.write (op1 - op2);
}





