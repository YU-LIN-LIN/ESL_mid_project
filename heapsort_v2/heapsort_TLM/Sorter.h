#ifndef SORTER_H_
#define SORTER_H_
#include <systemc>
using namespace sc_core;

#include <tlm>
#include <tlm_utils/simple_target_socket.h>

#include "sorter_def.h"

class Sorter : public sc_module {
public:
  tlm_utils::simple_target_socket<Sorter> t_skt;

  sc_fifo<unsigned char> i_element;
  sc_fifo<unsigned char> o_result;

  SC_HAS_PROCESS(Sorter);
  Sorter(sc_module_name n);
  ~Sorter();

private:
  void do_sorter();
  void swap(unsigned char &p1, unsigned char &p2);

  unsigned char array[SEQ_LEN];

  unsigned int base_offset;
  void blocking_transport(tlm::tlm_generic_payload &payload,
                          sc_core::sc_time &delay);
};
#endif
