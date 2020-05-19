#include "../run.h"
void kernel_case4(float (&B)[16][32], float (&C)[32][32], float (&A)[16][32]) {
  float temp[16][32] = {};
  for(int i = 0; i < 0 + 16; ++i){
    for(int j = 0; j < 0 + 32; ++j){
      if ((((0 <= i && i < 16) && 0 <= j) && j < 32)) {
        temp[i][j] =0;
      } else {
        A[i][j] =A[i][j];
      }
      if ((((0 <= i && i < 16) && 0 <= j) && j < 32)) {
        temp[i][j] =(temp[i][j] + A[i][j]);
      } else {
        A[i][j] =A[i][j];
      }
      for(int k = 0; k < 0 + 32; ++k){
        if ((((((0 <= i && i < 16) && 0 <= j) && j < 32) && 0 <= k) && k < 32)) {
          temp[i][j] =(temp[i][j] + (B[i][k] * C[k][j]));
        } else {
          A[i][j] =A[i][j];
        }
      }
      if ((((0 <= i && i < 16) && 0 <= j) && j < 32)) {
        A[i][j] =temp[i][j];
      } else {
        A[i][j] =A[i][j];
      }
    }
  }
}
