#ifndef HEAP_SORTER_H_
#define HEAP_SORTER_H_
#include <systemc>
using namespace sc_core;

#ifndef NATIVE_SYSTEMC
#include <cynw_p2p.h>
#endif

#include "sorter_def.h"

class HeapSorter: public sc_module
{
public:
	sc_in_clk i_clk;
	sc_in < bool >  i_rst;
#ifndef NATIVE_SYSTEMC
	cynw_p2p< sc_dt::sc_uint<8> >::in i_seq;
	cynw_p2p< sc_dt::sc_uint<8> >::out o_result;
#else
	sc_fifo_in< sc_dt::sc_uint<8> > i_seq;
	sc_fifo_out< sc_dt::sc_uint<8> > o_result;
#endif

	SC_HAS_PROCESS( HeapSorter );
	HeapSorter( sc_module_name n );
	~HeapSorter();
private:
    void swap(sc_dt::sc_uint<8> &p1, sc_dt::sc_uint<8> &p2);
	void do_sorter();
    // int val[MASK_N];
    // unsigned char array[SEQ_LEN];
    // unsigned int count;
};
#endif
