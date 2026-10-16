#include "Vcounter.h"
#include "verilated.h"
#include "verilated_vcd_c.h"
#include "vbuddy.cpp"

int main(int argc, char **argv, char **env){
    int i;
    int clk;

    Verilated::commandArgs(argc,argv);
    //init top verilog instance
    Vcounter* top=new Vcounter;
    //init trace dump
    Verilated::traceEverOn(true);
    VerilatedVcdC* tfp=new VerilatedVcdC;
    top->trace (tfp,99);
    tfp->open ("counter.vcd");
    
    //innit Vbuddy
    if (vbdOpen()!=1) return(-1);
    vbdHeader("Lab 1: counter");
    //initialize simulation inputs 
    top->clk=1;
    top->rst=1;
    top->en=0;

    //run simulation for many clock cycles
    for(i=0;i<300;i++){
        
        //dump variables into VCD file and toggle clock
        for(clk=0;clk<2;clk++){
            tfp->dump (2*i+clk);  //unit is in ps!!!!
            top->clk=!top->clk;
            top->eval ();
        }


         //++++ send count value to vbuddy
        vbdHex(4,(int(top->count)>>16) & 0xF);
        vbdHex(3,(int(top->count)>>8) & 0xF);
        vbdHex(2,(int(top->count)>>4) & 0xF);
        vbdHex(1,int(top->count) & 0xF);
        vbdCycle(i+1);
         //-------- end of vbuddy output selection

        /* //plots the graph
        vbdPlot(int(top->count),0,30);
        vbdPlot(int(top->count), 30, 255);
        vbdPlot(int(top->count), 255, 900);
        vbdCycle(i+1);
        // ----- Vbuddy graph output 
*/

        top->rst=(i<2)|(i==15);
            
    
        top->en=vbdFlag();

        if (vbdFlag()==0){
            top->en=1;
        }
        else{
            top->en=-1;
        }



        if(Verilated::gotFinish()) exit(0);



    }

    vbdClose();
    tfp->close();
    exit(0);
}
