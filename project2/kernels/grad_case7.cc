#include "../run2.h"
void grad_case7(float (&dB)[16][32], float (&dA)[32][16]) {
    for(int i = 0; i < 0 + 32; ++i){
    for(int j = 0; j < 0 + 16; ++j){
      dA[i][j] =0;
    }
  }
  for(int i = 0; i < 0 + 16; ++i){
    for(int j = 0; j < 0 + 32; ++j){
      if ((((0 <= i && i < 16) && 0 <= j) && j < 32)) {
        dA[j][i] =(dA[j][i] + (dB[i][j] * 1));
      } else {
        dB[i][j] =dB[i][j];
      }
    }
  }
}
