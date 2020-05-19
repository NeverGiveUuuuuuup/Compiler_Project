#include "../run.h"
void kernel_example(float (&B)[32][16], float (&C)[32][16], float (&A)[32][16]) {
  float temp[32][16] = {};
  for(int i = 0; i < 0 + 32; ++i){
    for(int j = 0; j < 0 + 16; ++j){
      if ((((0 <= i && i < 32) && 0 <= j) && j < 16)) {
        temp[i][j] =0;
      } else {
        A[i][j] =A[i][j];
      }
      if ((((0 <= i && i < 32) && 0 <= j) && j < 16)) {
        temp[i][j] =(temp[i][j] + (C[i][j] * B[i][j]));
      } else {
        A[i][j] =A[i][j];
      }
      if ((((0 <= i && i < 32) && 0 <= j) && j < 16)) {
        A[i][j] =temp[i][j];
      } else {
        A[i][j] =A[i][j];
      }
    }
  }
}
