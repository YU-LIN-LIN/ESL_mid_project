#include <cstdio>
#include <cstdlib>
#include <fstream>
using namespace std;

#include "Testbench.h"
#include "sorter_def.h"

// for latency calculation
#include <esc.h>                // for the latency logging functions
#include <queue>    
#include <iomanip>            
static std::queue<sc_time> time_queue;

// Testbench::Testbench(sc_module_name n) : sc_module(n), output_rgb_raw_data_offset(54) {
  Testbench::Testbench(sc_module_name n) : sc_module(n) {
  SC_THREAD(feed_seq);
  sensitive << i_clk.pos();
  dont_initialize();
  SC_THREAD(fetch_result);
  sensitive << i_clk.pos();
  dont_initialize();
}

Testbench::~Testbench() {
	cout << "Total run time = " << total_run_time << endl;
  cout << "Simulated time == " << sc_core::sc_time_stamp() << std::endl;
}

// read sequence
int Testbench::read_seq(string infile_name) {

  FILE *fp_s = NULL; // source file handler
  fp_s = fopen(infile_name.c_str(), "rb");
  if (fp_s == NULL) {
    printf("fopen %s error\n", infile_name.c_str());
    return -1;
  }

  source = (unsigned int *)malloc((size_t)(sizeof(unsigned int) * (SEQ_TOT + SEQ_LEN)));
  target = (unsigned int *)malloc((size_t)(sizeof(unsigned int) * (SEQ_TOT + SEQ_LEN)));

  // fread(source, sizeof(unsigned char),
  //       (size_t)(long)SEQ_LEN * sizeof(unsigned char), fp_s);

  cout << "Read : ";
  for (int i = 0;  i < SEQ_TOT; i++) {
    fscanf(fp_s, "%d", source + i);
    cout << *(source + i) << " ";
  } cout << endl;

  fclose(fp_s);
  return 0;
}

int Testbench::write_seq(string outfile_name) {

  FILE *fp_t = NULL;      // target file handler
  // unsigned int file_size; // file size

  fp_t = fopen(outfile_name.c_str(), "wb");
  if (fp_t == NULL) {
    printf("fopen %s error\n", outfile_name.c_str());
    return -1;
  }

  // fwrite(target, sizeof(unsigned char),
  //       (size_t)(long)(SEQ_LEN) * (sizeof(unsigned char)), fp_t);

  for (int i = 0; i < SEQ_TOT; i++) {
    fprintf(fp_t,  "%d\n", *(target + i));
  }

  return 0;
}

// feed sequence
void Testbench::feed_seq() {

  unsigned char element;
  unsigned int i;                 // loop index

#ifndef NATIVE_SYSTEMC
	o_seq.reset();
#endif
	o_rst.write(false);
	wait(5);
	o_rst.write(true);
	wait(1);
	total_start_time = sc_time_stamp();

  sc_dt::sc_uint<8> o_ele;

  if (SEQ_TOT <= 9) {
    for (i = 0; i < SEQ_LEN; i++) {
      if (SEQ_TOT <= i) {
        o_ele = 255;
      } else {
        o_ele = *(source + i);
      }
      cout << "o_ele = " << o_ele << endl;
      #ifndef NATIVE_SYSTEMC
        o_seq.put(o_ele);
      #else
        o_seq.write(o_ele);
      #endif
    }
  } else {
    for (int x = 0; x < 3 + ((SEQ_TOT - 10) / 9); x++) {
      int r1, r2;
      r1 = 0;
      while (r1 < SEQ_TOT) {
        for (int k = 0; k < SEQ_LEN; k++) {
          if (x == 0) {
            if (r1 + k >= SEQ_TOT) {
              o_ele = 255;
            } else {
              o_ele = *(source + r1 + k);
            }
          } else {
            if (r1 + k >= SEQ_TOT) {
              o_ele = 255;
            } else {
              o_ele = *(target + r1 + k);
            }
          }
          cout << "o_ele = " << o_ele << endl;
          #ifndef NATIVE_SYSTEMC
            o_seq.put(o_ele);
          #else
            o_seq.write(o_ele);
          #endif
        }
        r1 = r1 + SEQ_LEN;
      }
      r2 = 4;
      while (r2 + 5 < SEQ_TOT) {
        for (int k = 0; k < SEQ_LEN; k++) {
          if (r2 + k >= SEQ_TOT) {
            o_ele = 255;
          } else {
            o_ele = *(target + r2 + k);
          }
          cout << "o_ele = " << o_ele << endl;
          #ifndef NATIVE_SYSTEMC
            o_seq.put(o_ele);
          #else
            o_seq.write(o_ele);
          #endif
        }
        r2 = r2 + SEQ_LEN;
      }
    }
  }
}

void Testbench::fetch_result() {
  unsigned int i;       // loop index
  unsigned long total_latency = 0;
  int out_num = 0;
  sc_dt::sc_uint<8> result;
#ifndef NATIVE_SYSTEMC
	i_result.reset();
#endif
	wait(5);
	wait(1);

  if (SEQ_TOT <= 9) {
    for (i = 0; i < SEQ_LEN; i++) {
      #ifndef NATIVE_SYSTEMC
        result = i_result.get();
      #else
        result = i_result.read();
      #endif
      *(target + i) = result;
      out_num++;

      cout << "i = " << i << ", result in tb = " << result << endl;

      // Calculate latency
      time_queue.push( sc_time_stamp() );

      if (i > 0) {
      sc_time sent_time( time_queue.front() );
      time_queue.pop();
      
      unsigned long latency = clock_cycle( sc_time_stamp() - sent_time );
      total_latency += latency;
      // if (x + 256*y <= 1000) cout << setw(5) << x + 256*y << " : fetch time = " << sc_time_stamp() << ", Latency = " << latency <<  endl;
      //cout << "Latency for sample " << i << " is " <<  latency << endl;
      }
    }
  } else {
    for (int x = 0; x < 3 + ((SEQ_TOT - 10) / 9); x++) {
      int r1, r2;
      r1 = 0;
      while (r1 < SEQ_TOT) {
        for (int k = 0; k < SEQ_LEN; k++) {
          #ifndef NATIVE_SYSTEMC
            result = i_result.get();
          #else
            result = i_result.read();
          #endif
          *(target + r1 + k) = result;
          out_num++;

          cout << "index = " << r1 + k << ", result in tb = " << result << endl;

          // Calculate latency
          time_queue.push( sc_time_stamp() );

          if (r1 + k > 0) {
          sc_time sent_time( time_queue.front() );
          time_queue.pop();
          
          unsigned long latency = clock_cycle( sc_time_stamp() - sent_time );
          total_latency += latency;
          // if (x + 256*y <= 1000) cout << setw(5) << x + 256*y << " : fetch time = " << sc_time_stamp() << ", Latency = " << latency <<  endl;
          //cout << "Latency for sample " << i << " is " <<  latency << endl;
          }
        }
        r1 = r1 + SEQ_LEN;
      }
      r2 = 4;
      while (r2 + 5 < SEQ_TOT) {
        for (int k = 0; k < SEQ_LEN; k++) {
          #ifndef NATIVE_SYSTEMC
            result = i_result.get();
          #else
            result = i_result.read();
          #endif
          *(target + r2 + k) = result;
          out_num++;

          cout << "index = " << r2 + k << ", result in tb = " << result << endl;

          // Calculate latency
          time_queue.push( sc_time_stamp() );

          if (r2 + k > 0) {
          sc_time sent_time( time_queue.front() );
          time_queue.pop();
          
          unsigned long latency = clock_cycle( sc_time_stamp() - sent_time );
          total_latency += latency;
          // if (x + 256*y <= 1000) cout << setw(5) << x + 256*y << " : fetch time = " << sc_time_stamp() << ", Latency = " << latency <<  endl;
          //cout << "Latency for sample " << i << " is " <<  latency << endl;
          }
        }
        r2 = r2 + SEQ_LEN;
      }
    }
  }

	total_run_time = sc_time_stamp() - total_start_time;

  // log performance metrics
  unsigned long average_latency = total_latency / (out_num - 1);
  cout << "out_num = " << out_num << endl;
  cout << "Total latency = " <<  total_latency << "." << endl;
  // cout << "Average latency = " <<  average_latency << "." << endl;
  sc_stop();
}

// Convert a time in simulation time units to clock cycles
int Testbench::clock_cycle( sc_time time )
{
    sc_clock * i_clk_p = DCAST < sc_clock * >( i_clk.get_interface() );
    sc_time clock_period = i_clk_p->period(); // get period from the sc_clock object.
    return ( int )( time / clock_period );
}
