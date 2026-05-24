#include "utility.hpp"

void setColorByMatrix(float matrix[], float R, float G, float B) {
  matrix[3] = R;
  matrix[10] = G;
  matrix[17] = B;
}
