#include "../run.h"
void kernel_case10(float (&B)[10][10], float (&A)[8][8]) {
  float temp[8][8] = {};
  for(int i = 0; i < 0 + 8; ++i){
    for(int j = 0; j < 0 + 8; ++j){
      if ((((0 <= i && i < 8) && 0 <= j) && j < 8)) {
        temp[i][j] =0;
      } else {
        A[i][j] =A[i][j];
      }
      if ((((((((0 <= i && i < 10) && 0 <= (i + 1)) && (i + 1) < 10) && 0 <= (i + 2)) && (i + 2) < 10) && 0 <= j) && j < 10)) {
        temp[i][j] =(temp[i][j] + ((B[i][j] + (B[(i + 1)][j] + B[(i + 2)][j])) / 3));
      } else {
        A[i][j] =A[i][j];
      }
      if ((((0 <= i && i < 8) && 0 <= j) && j < 8)) {
        A[i][j] =temp[i][j];
      } else {
        A[i][j] =A[i][j];
      }
    }
  }
}
