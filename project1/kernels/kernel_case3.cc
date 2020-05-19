#include "../run.h"
void kernel_case3(int (&B)[16][32], int (&C)[16][32], int (&A)[16][32]) {
  int temp[16][32] = {};
  for(int i = 0; i < 0 + 16; ++i){
    for(int j = 0; j < 0 + 32; ++j){
      if ((((0 <= i && i < 16) && 0 <= j) && j < 32)) {
        temp[i][j] =0;
      } else {
        A[i][j] =A[i][j];
      }
      if ((((0 <= i && i < 16) && 0 <= j) && j < 32)) {
        temp[i][j] =(temp[i][j] + B[i][j]);
      } else {
        A[i][j] =A[i][j];
      }
      if ((((0 <= i && i < 16) && 0 <= j) && j < 32)) {
        temp[i][j] =(temp[i][j] + C[i][j]);
      } else {
        A[i][j] =A[i][j];
      }
      if ((((0 <= i && i < 16) && 0 <= j) && j < 32)) {
        A[i][j] =temp[i][j];
      } else {
        A[i][j] =A[i][j];
      }
    }
  }
}
