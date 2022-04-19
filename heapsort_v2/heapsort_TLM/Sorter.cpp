#include <cmath>
#include <iomanip>

#include "Sorter.h"

using namespace std;

Sorter::Sorter(sc_module_name n)
    : sc_module(n), t_skt("t_skt"), base_offset(0) {
  SC_THREAD(do_sorter);

  t_skt.register_b_transport(this, &Sorter::blocking_transport);
}

Sorter::~Sorter() = default;

void Sorter::swap(unsigned char &p1, unsigned char &p2) {
  unsigned char temp;
  temp = p1;
  p1 = p2; 
  p2 = temp;
}

void Sorter::do_sorter() {
  while (true) {
    int i, j;
    int result;

    for (i = 0; i < SEQ_LEN; i++) {
      array[i] = i_element.read();
    }

    // simple sort
    // for (i = 0; i < SEQ_LEN; i++) {
    //   min_addr = i;
    //   for (j = i; j < SEQ_LEN; j++) {
    //     if (array[j] < array[min_addr]) {
    //       min_addr = j;
    //     }
    //   }
    //   temp = array[i];
    //   array[i] = array[min_addr];
    //   array[min_addr] = temp;

    //   result = array[i];
    //   o_result.write(result);
    // }

    // heapsort
    for (i = SEQ_LEN-1; i >= 0; i--) {
      for (j = ((i + 1) / 2 - 1); j >= 0; j--) {
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
      o_result.write(result);
    }
  }
}

void Sorter::blocking_transport(tlm::tlm_generic_payload &payload,
                                     sc_core::sc_time &delay) {
  sc_dt::uint64 addr = payload.get_address();
  addr -= base_offset;
  unsigned char *mask_ptr = payload.get_byte_enable_ptr();
  unsigned char *data_ptr = payload.get_data_ptr();
  word buffer;
  switch (payload.get_command()) {
  case tlm::TLM_READ_COMMAND:
    switch (addr) {
    case SORTER_RESULT_ADDR:
      buffer.ans[0] = o_result.read();
      break;
    case SORTER_CHECK_ADDR:
      buffer.uint = o_result.num_available();
    break;
    default:
      std::cerr << "Error! Sorter::blocking_transport: address 0x"
                << std::setfill('0') << std::setw(8) << std::hex << addr
                << std::dec << " is not valid" << std::endl;
    }
    data_ptr[0] = buffer.uc[0];
    data_ptr[1] = buffer.uc[1];
    data_ptr[2] = buffer.uc[2];
    data_ptr[3] = buffer.uc[3];
    break;
  case tlm::TLM_WRITE_COMMAND:
    switch (addr) {
    case SORTER_R_ADDR:
      if (mask_ptr[0] == 0xff) {
        i_element.write(data_ptr[0]);
      }
      break;
    default:
      std::cerr << "Error! Sorter::blocking_transport: address 0x"
                << std::setfill('0') << std::setw(8) << std::hex << addr
                << std::dec << " is not valid" << std::endl;
    }
    break;
  case tlm::TLM_IGNORE_COMMAND:
    payload.set_response_status(tlm::TLM_GENERIC_ERROR_RESPONSE);
    return;
  default:
    payload.set_response_status(tlm::TLM_GENERIC_ERROR_RESPONSE);
    return;
  }
  payload.set_response_status(tlm::TLM_OK_RESPONSE); // Always OK
}