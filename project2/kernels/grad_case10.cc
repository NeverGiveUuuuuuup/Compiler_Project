#include "../run2.h"
void grad_case10(float (&dA)[8][8], float (&dB)[10][8]) {
  int temp1, temp2;
  for(int i = 0; i < 0 + 10; ++i){
    for(int j = 0; j < 0 + 8; ++j){
      dB[i][j] =0;
    }
  }
  for(int i = 0; i < 0 + 8; ++i){
    for(int j = 0; j < 0 + 8; ++j){
      if ((((0 <= i && i < 8) && 0 <= j) && j < 8)) {
        dB[i][j] =(dB[i][j] + (dA[i][j] * ((0 + (0 + 1)) / (float)3)));
      } else {
        dA[i][j] =dA[i][j];
      }
      temp1 =(i + 1);
      if ((((0 <= i && i < 8) && 0 <= j) && j < 8)) {
        dB[temp1][j] =(dB[temp1][j] + (dA[i][j] * ((0 + (1 + 0)) / (float)3)));
      } else {
        dA[i][j] =dA[i][j];
      }
      temp2 =(i + 2);
      if ((((0 <= i && i < 8) && 0 <= j) && j < 8)) {
        dB[temp2][j] =(dB[temp2][j] + (dA[i][j] * ((1 + (0 + 0)) / (float)3)));
      } else {
        dA[i][j] =dA[i][j];
      }
    }
  }
}
