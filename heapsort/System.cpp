#include "System.h"
System::System( sc_module_name n, string input_seq, string output_seq ): sc_module( n ), 
	tb("tb"), heap_sorter("heap_sorter"), clk("clk", CLOCK_PERIOD, SC_NS), rst("rst"), _output_seq(output_seq)
{
	tb.i_clk(clk);
	tb.o_rst(rst);
	heap_sorter.i_clk(clk);
	heap_sorter.i_rst(rst);
	tb.o_seq(in_seq);
	tb.i_result(result);
	heap_sorter.i_seq(in_seq);
	heap_sorter.o_result(result);

  tb.read_seq(input_seq);
}

System::~System() {
  tb.write_seq(_output_seq);
}
