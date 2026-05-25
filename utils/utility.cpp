#include "utility.hpp"

void setColorByMatrix(float matrix[], float R, float G, float B) {
  matrix[3] = R;
  matrix[10] = G;
  matrix[17] = B;
}

void setSize(float matrix[], float size[]) {
  matrix[0] = size[0];
  matrix[1] = size[1];
  matrix[2] = size[2];
  matrix[6] = size[3];
  matrix[7] = size[4];
  matrix[8] = size[5];
  matrix[12] = size[6];
  matrix[13] = size[7];
  matrix[14] = size[8];
}
