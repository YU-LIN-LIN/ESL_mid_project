#include <cmath>
#ifndef NATIVE_SYSTEMC
#include "stratus_hls.h"
#endif

#include "HeapSorter.h"
#include "sorter_def.h"
#include "directives.h"

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
    // Add a HLS_PIPELINE_LOOP directive that is enabled when II is set.
    #if defined (I)
        HLS_PIPELINE_LOOP(SOFT_STALL, II, "Loop" );
    #endif

	{
        #ifndef NATIVE_SYSTEMC
            HLS_DEFINE_PROTOCOL("main_reset");
            i_seq.reset();
            o_result.reset();
        #endif
            wait();
	}

    sc_dt::sc_uint<8> array[SEQ_LEN];
	
	while (true) {
		
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
            cout << "sorter received : " << array[i] << endl;
        }

        sc_dt::sc_uint<8> result;

        // simple sort
        sc_dt::sc_uint<8> min_addr, min_addr1;
        for (int i = 0; i < SEQ_LEN; i++) {
            OUT_LOOP;
            min_addr = i;
            min_addr1 = i;
            // for (int j = i; j < SEQ_LEN; j++) {
            //     IN_LOOP;
            //     HLS_CONSTRAIN_LATENCY(0, 2, "lat01");
            //     if (array[j] < array[min_addr]) {
            //         min_addr = j;
            //     }
            // }

            // swap(array[i], array[min_addr]);

            // unroll by 2
            for (int j = i; j < SEQ_LEN; j+=2) {
                IN_LOOP;
                HLS_CONSTRAIN_LATENCY(0, 4, "lat01");
                if (array[j] < array[min_addr]) {
                    min_addr = j;
                }
                if ((array[j + 1] < array[min_addr1]) && (j + 1 < SEQ_LEN)) {
                    min_addr1 = j + 1;
                }
            }
            if (array[min_addr] < array[min_addr1]) {
                swap(array[i], array[min_addr]);
            } else {
                swap(array[i], array[min_addr1]);
            }

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
	}
}
