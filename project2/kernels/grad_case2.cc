#include "../run2.h"
void grad_case2(float (&A)[4][16], float (&dB)[4][16], float (&dA)[4][16]) {
    for(int i = 0; i < 0 + 4; ++i){
    for(int j = 0; j < 0 + 16; ++j){
      dA[i][j] =0;
    }
  }
  for(int i = 0; i < 0 + 4; ++i){
    for(int j = 0; j < 0 + 16; ++j){
      if ((((0 <= i && i < 4) && 0 <= j) && j < 16)) {
        dA[i][j] =(dA[i][j] + (dB[i][j] * ((1 * A[i][j]) + 0)));
      } else {
        dB[i][j] =dB[i][j];
      }
      if ((((0 <= i && i < 4) && 0 <= j) && j < 16)) {
        dA[i][j] =(dA[i][j] + (dB[i][j] * ((A[i][j] * 1) + 0)));
      } else {
        dB[i][j] =dB[i][j];
      }
    }
  }
}
