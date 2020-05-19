#include "../run.h"
void kernel_case8(float (&B)[8][16], float (&A)[8][2][16]) {
  float temp[8][2][16] = {};
  for(int i = 0; i < 0 + 8; ++i){
    for(int j = 0; j < 0 + 2; ++j){
      for(int k = 0; k < 0 + 16; ++k){
        if ((((((0 <= i && i < 8) && 0 <= j) && j < 2) && 0 <= k) && k < 16)) {
          temp[i][j][k] =0;
        } else {
          A[i][j][k] =A[i][j][k];
        }
        if ((((0 <= i && i < 8) && 0 <= k) && k < 16)) {
          temp[i][j][k] =(temp[i][j][k] + B[i][k]);
        } else {
          A[i][j][k] =A[i][j][k];
        }
        if ((((((0 <= i && i < 8) && 0 <= j) && j < 2) && 0 <= k) && k < 16)) {
          A[i][j][k] =temp[i][j][k];
        } else {
          A[i][j][k] =A[i][j][k];
        }
      }
    }
  }
}
