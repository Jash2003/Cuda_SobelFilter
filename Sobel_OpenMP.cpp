#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <ctime>
#include <float.h>
#include "mypgm.h"
#include <math.h>
// Sobel filter function for CPU
void sobel_cpu(float image1[1024][1024], float image3[1024][1024], int w, int h) {

  float dx, dy;

  #pragma omp parallel for collapse(2)  // Parallelize loops over i and j
  for (int i = 0; i < w - 2; i++) {
    for (int j = 0; j < h - 2; j++) {

      dy = (-1 * image1[i][j]) + (-2 * image1[i][j + 1]) + (-1 * image1[i][j + 2]) +
           (image1[i + 2][j]) + (2 * image1[i + 2][j + 1]) + (image1[i + 2][j + 2]);

      dx = (-1 * image1[i][j]) + (1 * image1[i][j + 2])
          + (-2 * image1[i + 1][j]) + (2 * image1[i + 1][j + 2])
          + (-1 * image1[i + 2][j]) + (1 * image1[i + 2][j + 2]);

      image3[i][j] = sqrt((dx * dx) + (dy * dy));
    }
  }
}

int main() {

  // Load Image (image image1 is a global variable for simplicity)
  load_image_data();

  int i, j;

  // No memory allocation or transfer needed for OpenMP

  // Initialize the clock so we can track CPU execution time
  std::clock_t start_cpu;
  double duration_cpu;
  start_cpu = std::clock();

  printf(" ----------- OpenMP -----------\n");

  printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ ~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
  printf("INITIALIZE image3 (IMAGE_OpenMP)\n");
  x_size3 = x_size1;
  y_size3 = y_size1;
  for (i = 0; i < y_size3; i++) {
    for (j = 0; j < x_size3; j++) {
      image3[i][j] = 0;
    }
  }

  printf("\nBEGINNING: Sobel filter using OpenMP ");

  sobel_cpu(image1, image3, x_size3, y_size3);

  // End CPU clock
  duration_cpu = (std::clock() - start_cpu) / (double)CLOCKS_PER_SEC;
  std::cout << "\nCOMPLETE: Sobel filter using OpenMP: " << duration_cpu << "s - SUCCESS!" << '\n';
  printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ ~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n\n");

  save_image_data_img3();

  return 0;
}
