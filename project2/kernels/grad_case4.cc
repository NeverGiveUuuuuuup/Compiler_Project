#include "../run2.h"
void grad_case4(float (&B)[16][32], float (&C)[32][32], float (&dA)[16][32], float (&dB)[16][32], float (&dC)[32][32]) {
    for(int i = 0; i < 0 + 16; ++i){
    for(int j = 0; j < 0 + 32; ++j){
      dB[i][j] =0;
    }
  }
  for(int i = 0; i < 0 + 32; ++i){
    for(int j = 0; j < 0 + 32; ++j){
      dC[i][j] =0;
    }
  }
  for(int i = 0; i < 0 + 16; ++i){
    for(int j = 0; j < 0 + 32; ++j){
      for(int k = 0; k < 0 + 32; ++k){
        if ((((((0 <= i && i < 16) && 0 <= j) && j < 32) && 0 <= k) && k < 32)) {
          dB[i][k] =(dB[i][k] + (dA[i][j] * (1 * C[k][j])));
        } else {
          dA[i][j] =dA[i][j];
        }
        if ((((((0 <= i && i < 16) && 0 <= j) && j < 32) && 0 <= k) && k < 32)) {
          dC[k][j] =(dC[k][j] + (dA[i][j] * (B[i][k] * 1)));
        } else {
          dA[i][j] =dA[i][j];
        }
      }
    }
  }
}
