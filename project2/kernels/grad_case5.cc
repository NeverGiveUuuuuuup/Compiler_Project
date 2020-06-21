#include "../run2.h"
void grad_case5(float (&C)[32][32], float (&D)[4][32], float (&dA)[16][32], float (&dB)[16][32][4]) {
    for(int i = 0; i < 0 + 16; ++i){
    for(int j = 0; j < 0 + 32; ++j){
      for(int k = 0; k < 0 + 4; ++k){
        dB[i][j][k] =0;
      }
    }
  }
  for(int i = 0; i < 0 + 16; ++i){
    for(int j = 0; j < 0 + 32; ++j){
      for(int k = 0; k < 0 + 32; ++k){
        for(int l = 0; l < 0 + 4; ++l){
          if ((((((((0 <= i && i < 16) && 0 <= j) && j < 32) && 0 <= k) && k < 32) && 0 <= l) && l < 4)) {
            dB[i][k][l] =(dB[i][k][l] + (dA[i][j] * (1 * (C[k][j] * D[l][j]))));
          } else {
            dA[i][j] =dA[i][j];
          }
        }
      }
    }
  }
}
