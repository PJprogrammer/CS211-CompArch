#pragma once

double** multiplyMatrix(double** matrix1, int r1, int c1, double** matrix2, int r2, int c2);
double** transposeMatrix(double** matrix, int r, int c);
void inverseMatrix(double** matrix, int n);
void freeArray(double** array, int row);