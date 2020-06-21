#include "../run2.h"
void grad_case6(float (&C)[8][16][3][3], float (&dA)[2][8][5][5], float (&dB)[2][16][7][7]) {
  int temp1, temp2;
  for(int i = 0; i < 0 + 2; ++i){
    for(int j = 0; j < 0 + 16; ++j){
      for(int k = 0; k < 0 + 7; ++k){
        for(int l = 0; l < 0 + 7; ++l){
          dB[i][j][k][l] =0;
        }
      }
    }
  }
  for(int c = 0; c < 0 + 16; ++c){
    for(int k = 0; k < 0 + 8; ++k){
      for(int n = 0; n < 0 + 2; ++n){
        for(int p = 0; p < 0 + 5; ++p){
          for(int q = 0; q < 0 + 5; ++q){
            for(int r = -64; r < -64 + 128; ++r){
              for(int s = -64; s < -64 + 128; ++s){
                temp1 =(p + r);
                temp2 =(q + s);
                if ((((((((((((((0 <= c && c < 16) && 0 <= k) && k < 8) && 0 <= n) && n < 2) && 0 <= p) && p < 5) && 0 <= q) && q < 5) && 0 <= r) && r < 3) && 0 <= s) && s < 3)) {
                  dB[n][c][temp1][temp2] =(dB[n][c][temp1][temp2] + (dA[n][k][p][q] * (1 * C[k][c][r][s])));
                } else {
                  dA[n][k][p][q] =dA[n][k][p][q];
                }
              }
            }
          }
        }
      }
    }
  }
}
