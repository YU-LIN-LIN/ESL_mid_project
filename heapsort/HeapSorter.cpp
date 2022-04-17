#include <cmath>
#ifndef NATIVE_SYSTEMC
#include "stratus_hls.h"
#endif

#include "HeapSorter.h"
#include "sorter_def.h"

HeapSorter::HeapSorter( sc_module_name n ): sc_module( n )
{
	SC_THREAD( do_sorter );
	sensitive << i_clk.pos();
	dont_initialize();
	reset_signal_is(i_rst, false);

#ifndef NATIVE_SYSTEMC
	i_seq.clk_rst(i_clk, i_rst);
  o_result.clk_rst(i_clk, i_rst);
#endif
}

HeapSorter::~HeapSorter() {}

void HeapSorter::swap(sc_dt::sc_uint<8> &p1, sc_dt::sc_uint<8> &p2) {
    sc_dt::sc_uint<8> temp;
    temp = p1;
    p1 = p2;
    p2 = temp;
}

void HeapSorter::do_sorter() {
	{
        #ifndef NATIVE_SYSTEMC
            HLS_DEFINE_PROTOCOL("main_reset");
            i_seq.reset();
            o_result.reset();
        #endif
            wait();
	}
	
	while (true) {
		// Add a HLS_PIPELINE_LOOP directive that is enabled when II is set.
		#if defined (I)
			HLS_PIPELINE_LOOP(SOFT_STALL, II, "Loop" );
		#endif

        sc_dt::sc_uint<8> array[SEQ_LEN];

        for (int i = 0; i < SEQ_LEN; i++) {
            #ifndef NATIVE_SYSTEMC
            {
                HLS_DEFINE_PROTOCOL("input");
                array[i] = i_seq.get();
                wait();
            }
            #else
                    array[i] = i_seq.read();
            #endif
        }

        sc_dt::sc_uint<8> result;
        
        for (int i = SEQ_LEN-1; i >= 0; i--) {
            for (int j = ((i + 1) / 2 - 1); j >= 0; j--) {
                HLS_CONSTRAIN_LATENCY(0, 6, "lat01");
                if (j == 0) {
                    if (i >= 1 && array[1] < array[2] && array[1] < array[0]) {
                        swap(array[1], array[0]);
                    } else if (i >= 2 && array[2] < array[1] && array[2] < array[0]) {
                        swap(array[2], array[0]);
                    }
                } else {
                    if (i >= (2*j+1) && array[2*j+1] < array[2*j+2] && array[2*j+1] < array[j]) {
                        swap(array[2*j+1], array[j]);
                    } else if (i >= (2*j+2) && array[2*j+2] < array[2*j+1] && array[2*j+2] < array[j]) {
                        swap(array[2*j+2], array[j]);
                    }
                }
            }
            swap(array[0], array[i]);
            result = array[i];
            #ifndef NATIVE_SYSTEMC
                {
                    HLS_DEFINE_PROTOCOL("output");
                    o_result.put(result);
                    wait();
                }
            #else
                o_result.write(result);
            #endif
        }

        // for (int i = 0; i < SEQ_LEN; i++) {
        //     result = array[i];
        //     #ifndef NATIVE_SYSTEMC
        //         {
        //             HLS_DEFINE_PROTOCOL("output");
        //             o_result.put(result);
        //             wait();
        //         }
        //     #else
        //         o_result.write(result);
        //     #endif
        // }
	}
}
