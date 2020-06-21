#include "../run2.h"
void grad_case9(float (&dB)[4][6], float (&dA)[4]) {
    for(int i = 0; i < 0 + 4; ++i){
    dA[i] =0;
  }
  for(int i = 0; i < 0 + 4; ++i){
    for(int j = 0; j < 0 + 6; ++j){
      if ((((0 <= i && i < 4) && 0 <= j) && j < 6)) {
        dA[i] =(dA[i] + (dB[i][j] * 1));
      } else {
        dB[i][j] =dB[i][j];
      }
    }
  }
}
