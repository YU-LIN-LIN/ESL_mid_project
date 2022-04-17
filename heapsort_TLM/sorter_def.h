#ifndef SORTER_DEF_H_
#define SORTER_DEF_H_

#define SEQ_LEN 9

// sobel mask parameters
// const int MASK_N = 1;
// const int MASK_X = 3;
// const int MASK_Y = 3;

// Sobel Filter inner transport addresses
// Used between blocking_transport() & do_filter()
const int SORTER_R_ADDR = 0x00000000;
const int SORTER_RESULT_ADDR = 0x00000004;
const int SORTER_CHECK_ADDR = 0x00000008;

union word {
  int sint;
  unsigned int uint;
  unsigned char uc[4];
  unsigned char ans[4];
};

/// gaussian mask
// const int mask[MASK_X][MASK_Y] = {{1, 2, 1}, {2, 4, 2}, {1, 2, 1}};

#endif