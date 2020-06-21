#include "../run2.h"
void grad_case1(float (&B)[4][16], float (&dC)[4][16], float (&dA)[4][16]) {
    for(int i = 0; i < 0 + 4; ++i){
    for(int j = 0; j < 0 + 16; ++j){
      dA[i][j] =0;
    }
  }
  for(int i = 0; i < 0 + 4; ++i){
    for(int j = 0; j < 0 + 16; ++j){
      if ((((0 <= i && i < 4) && 0 <= j) && j < 16)) {
        dA[i][j] =(dA[i][j] + (dC[i][j] * ((1 * B[i][j]) + 0)));
      } else {
        dC[i][j] =dC[i][j];
      }
    }
  }
}
