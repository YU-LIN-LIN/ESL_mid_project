#ifndef TESTBENCH_H_
#define TESTBENCH_H_

#include <string>
using namespace std;

#include <systemc>
using namespace sc_core;

#include "Initiator.h"
#include "MemoryMap.h"
#include "sorter_def.h"

const int WHITE = 255;
const int BLACK = 0;
const int THRESHOLD = 90;

class Testbench : public sc_module {
public:
  Initiator initiator;

  SC_HAS_PROCESS(Testbench);

  Testbench(sc_module_name n);
  ~Testbench();

  int read_seq(string infile_name);
  int write_seq(string outfile_name);

  // unsigned int get_width() { return width; }

  // unsigned int get_height() { return height; }

  // unsigned int get_width_bytes() { return width_bytes; }

  unsigned int get_bytes_per_pixel() { return bytes_per_pixel; }

  unsigned char *get_source_image() { return source; }
  unsigned char *get_target_image() { return target; }

private:
  unsigned int input_rgb_raw_data_offset;
  // const unsigned int output_rgb_raw_data_offset;
  // int width;
  // int height;
  // unsigned int width_bytes;
  unsigned char bits_per_pixel;
  unsigned short bytes_per_pixel;
  unsigned char *source;
  unsigned char *target;

  void do_sort();
};
#endif
