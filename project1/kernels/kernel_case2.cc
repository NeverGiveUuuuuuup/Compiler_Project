#include "../run.h"
void kernel_case2(float (&A)[16][8]) {
  float temp[16][8] = {};
  for(int i = 0; i < 0 + 16; ++i){
    for(int j = 0; j < 0 + 8; ++j){
      if ((((0 <= i && i < 16) && 0 <= j) && j < 8)) {
        temp[i][j] =0;
      } else {
        A[i][j] =A[i][j];
      }
      if ((((0 <= i && i < 16) && 0 <= j) && j < 8)) {
        temp[i][j] =(temp[i][j] + A[i][j]);
      } else {
        A[i][j] =A[i][j];
      }
      if ((((0 <= i && i < 16) && 0 <= j) && j < 8)) {
        temp[i][j] =(temp[i][j] + 2);
      } else {
        A[i][j] =A[i][j];
      }
      if ((((0 <= i && i < 16) && 0 <= j) && j < 8)) {
        A[i][j] =temp[i][j];
      } else {
        A[i][j] =A[i][j];
      }
    }
  }
}
