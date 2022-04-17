#include <cassert>
#include <cstdio>
#include <cstdlib>
using namespace std;

#include "Testbench.h"

Testbench::Testbench(sc_module_name n)
    : sc_module(n), initiator("initiator") {
  SC_THREAD(do_sort);
}

Testbench::~Testbench() {}

int Testbench::read_seq(string infile_name) {
  FILE *fp_s = NULL; // source file handler
  fp_s = fopen(infile_name.c_str(), "rb");
  if (fp_s == NULL) {
    printf("fopen %s error\n", infile_name.c_str());
    return -1;
  }

  source = (unsigned char *)malloc((size_t)(sizeof(unsigned char) * SEQ_LEN));
  target = (unsigned char *)malloc((size_t)(sizeof(unsigned char) * SEQ_LEN));

  fread(source, sizeof(unsigned char),
        (size_t)(long)SEQ_LEN * sizeof(unsigned char), fp_s);

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

  fwrite(target, sizeof(unsigned char),
        (size_t)(long)(SEQ_LEN) * (sizeof(unsigned char)), fp_t);

  return 0;
}

void Testbench::do_sort() {

  word data;
  unsigned char mask[4];
  // unsigned char result;

  int i;

  for (i = 0; i < SEQ_LEN; i++) {
    data.uc[0] = *(source + i);
    mask[0] = 0xff;
    mask[1] = 0;
    mask[2] = 0;
    mask[3] = 0;

    cout << "output in tb = " << data.uc[0] << endl;

    initiator.write_to_socket(SORT_MM_BASE + SORTER_R_ADDR, mask,
                              data.uc, 4);
    wait(20 * CLOCK_PERIOD, SC_NS);
  }

  for (i = 0; i < SEQ_LEN; i++) {
    bool done=false;
    int output_num=0;
    while(!done){
      initiator.read_from_socket(SORT_MM_BASE + SORTER_CHECK_ADDR, mask, data.uc, 4);
      output_num = data.sint;
      if(output_num>0) done=true;
    }
    wait(10 * CLOCK_PERIOD, SC_NS);

    initiator.read_from_socket(SORT_MM_BASE + SORTER_RESULT_ADDR, mask, data.uc, 4);
    *(target + i) = data.uc[0];

    cout << "Sorting result read from sorter in tb = " << data.uc[0] << endl;
  }

  sc_stop();
}
