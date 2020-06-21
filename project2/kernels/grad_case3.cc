#include "../run2.h"
void grad_case3(float (&B)[16][16], float (&dC)[4][16], float (&dA)[4][16]) {
    for(int i = 0; i < 0 + 4; ++i){
    for(int j = 0; j < 0 + 16; ++j){
      dA[i][j] =0;
    }
  }
  for(int i = 0; i < 0 + 4; ++i){
    for(int j = 0; j < 0 + 16; ++j){
      for(int k = 0; k < 0 + 16; ++k){
        if ((((((0 <= i && i < 4) && 0 <= j) && j < 16) && 0 <= k) && k < 16)) {
          dA[i][k] =(dA[i][k] + (dC[i][j] * (1 * B[k][j])));
        } else {
          dC[i][j] =dC[i][j];
        }
      }
    }
  }
}
