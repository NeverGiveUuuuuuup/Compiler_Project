#include "../run.h"
void kernel_case6(float (&B)[2][16][7][7], float (&C)[8][16][3][3], float (&A)[2][8][5][5]) {
  float temp[2][8][5][5] = {};
  for(int k = 0; k < 0 + 8; ++k){
    for(int n = 0; n < 0 + 2; ++n){
      for(int p = 0; p < 0 + 5; ++p){
        for(int q = 0; q < 0 + 5; ++q){
          if ((((((((0 <= n && n < 2) && 0 <= k) && k < 8) && 0 <= p) && p < 5) && 0 <= q) && q < 5)) {
            temp[n][k][p][q] =0;
          } else {
            A[n][k][p][q] =A[n][k][p][q];
          }
          if ((((((((0 <= k && k < 8) && 0 <= n) && n < 2) && 0 <= p) && p < 5) && 0 <= q) && q < 5)) {
            temp[n][k][p][q] =(temp[n][k][p][q] + A[n][k][p][q]);
          } else {
            A[n][k][p][q] =A[n][k][p][q];
          }
          for(int c = 0; c < 0 + 16; ++c){
            for(int r = -64; r < -64 + 128; ++r){
              for(int s = -64; s < -64 + 128; ++s){
                if ((((((((((((((0 <= c && c < 16) && 0 <= k) && k < 8) && 0 <= n) && n < 2) && 0 <= (p + r)) && (p + r) < 7) && 0 <= (q + s)) && (q + s) < 7) && 0 <= r) && r < 3) && 0 <= s) && s < 3)) {
                  temp[n][k][p][q] =(temp[n][k][p][q] + (B[n][c][(p + r)][(q + s)] * C[k][c][r][s]));
                } else {
                  A[n][k][p][q] =A[n][k][p][q];
                }
              }
            }
          }
          if ((((((((0 <= n && n < 2) && 0 <= k) && k < 8) && 0 <= p) && p < 5) && 0 <= q) && q < 5)) {
            A[n][k][p][q] =temp[n][k][p][q];
          } else {
            A[n][k][p][q] =A[n][k][p][q];
          }
        }
      }
    }
  }
}
