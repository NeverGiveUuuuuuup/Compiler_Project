#include "../run2.h"
void grad_case8(float (&dB)[32], float (&dA)[2][16]) {
  int temp1, temp2;
  for(int i = 0; i < 0 + 2; ++i){
    for(int j = 0; j < 0 + 16; ++j){
      dA[i][j] =0;
    }
  }
  for(int i = 0; i < 0 + 32; ++i){
    temp1 =(i / 16);
    temp2 =((int)i % 16);
    if ((0 <= i && i < 32)) {
      dA[temp1][temp2] =(dA[temp1][temp2] + (dB[i] * 1));
    } else {
      dB[i] =dB[i];
    }
  }
}
