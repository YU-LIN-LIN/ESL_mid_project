#ifndef SYSTEM_H_
#define SYSTEM_H_
#include <systemc>
using namespace sc_core;

#include "Testbench.h"
#ifndef NATIVE_SYSTEMC
#include "HeapSorter_wrap.h"
#else
#include "HeapSorter.h"
#endif

class System: public sc_module
{
public:
	SC_HAS_PROCESS( System );
	System( sc_module_name n, std::string input_seq, std::string output_seq );
	~System();
private:
  Testbench tb;
#ifndef NATIVE_SYSTEMC
	HeapSorter_wrapper heap_sorter;
#else
	HeapSorter heap_sorter;
#endif
	sc_clock clk;
	sc_signal<bool> rst;
#ifndef NATIVE_SYSTEMC
    // can change the bit size
	cynw_p2p< sc_dt::sc_uint<8> > in_seq;
	cynw_p2p< sc_dt::sc_uint<8> > result;
#else
	sc_fifo< sc_dt::sc_uint<8> > in_seq;
	sc_fifo< sc_dt::sc_uint<8> > result;
#endif

	std::string _output_seq;
};
#endif
