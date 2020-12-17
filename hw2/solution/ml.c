#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "ml.h"


int main(int argc, char* argv[]) {
    // Check if enough arguments present
    if(argc < 3) {
        printf("Not enough arguments\n");
    }

    // Check if files exist
    char* trainFileName = argv[1];
    char* testFileName = argv[2];
    if(access(trainFileName, F_OK ) == -1 || access(testFileName, F_OK ) == -1) {
        printf("Error train/test file not found");
        return -1;
    }

    // Read Train Data File
    int numAttributes;
    int numExamples;
    FILE* fpTrain = fopen(trainFileName, "r");
    fscanf(fpTrain, "%d", &numAttributes);
    numAttributes++;
    fscanf(fpTrain,"%d", &numExamples);

    double **attributes = (double **)calloc(numExamples , sizeof(double *));
    for (int i=0; i<numExamples; i++)
        attributes[i] = (double *)calloc(numAttributes , sizeof(double ));

    double **prices = (double **)calloc(numExamples , sizeof(double *));
    for (int i=0; i<numExamples; i++)
        prices[i] = (double *)calloc(1, sizeof(double ));
    for (int i = 0; i < numExamples; i++) {
        attributes[i][0] = 1;
        for (int j = 1; j < numAttributes; j++) {
            fscanf(fpTrain,"%lf,", &attributes[i][j]);
        }
        fscanf(fpTrain,"%lf\n", &prices[i][0]);
    }

    fclose(fpTrain);

    // Compute Weights Vector
    double **attributesTranspose = transposeMatrix(attributes, numExamples, numAttributes);
    double **squareMatrix = multiplyMatrix(attributesTranspose, numAttributes, numExamples, attributes, numExamples,
                                           numAttributes);
    inverseMatrix(squareMatrix, numAttributes);
    double **tempMatrix = multiplyMatrix(squareMatrix, numAttributes, numAttributes, attributesTranspose, numAttributes,
                                         numExamples);
    double **weightsMatrix = multiplyMatrix(tempMatrix, numAttributes, numExamples, prices, numExamples, 1);

    // Read Test Data File
    int numTestExamples;
    FILE* fpTest = fopen(testFileName, "r");
    fscanf(fpTest, "%d", &numTestExamples);

    double **testExamples = (double **)calloc(numTestExamples , sizeof(double *));
    for (int i=0; i<numTestExamples; i++)
        testExamples[i] = (double *)calloc(numAttributes , sizeof(double ));

    for (int i = 0; i < numTestExamples; i++) {
        testExamples[i][0] = 1;
        for (int j = 1; j < numAttributes; j++) {
            fscanf(fpTest,"%lf,", &testExamples[i][j]);
        }
    }

    fclose(fpTest);

    // Print predicted house prices
    for (int i = 0; i < numTestExamples; ++i) {
        double sum = 0;
        for (int j = 0; j < numAttributes; ++j) {
            sum += testExamples[i][j] * weightsMatrix[j][0];
        }
        printf("%0.0lf\n", sum);
    }

    freeArray(attributes, numExamples);
    freeArray(prices, numExamples);
    freeArray(attributesTranspose, numAttributes);
    freeArray(squareMatrix, numAttributes);
    freeArray(tempMatrix, numAttributes);
    freeArray(weightsMatrix, numAttributes);
    freeArray(testExamples, numTestExamples);

    return 0;
}


double** multiplyMatrix(double** matrix1, int r1, int c1, double** matrix2, int r2, int c2) {
    double **mtx = (double **)calloc(r1 , sizeof(double *));
    for (int i=0; i<r1; i++)
        mtx[i] = (double *)calloc(c2 , sizeof(double));

    for (int i = 0; i < r1; ++i) {
        for (int j = 0; j < c2; ++j) {
            double sum = 0;
            for (int k = 0; k < c1; ++k) {
                sum += matrix1[i][k] * matrix2[k][j];
            }
            mtx[i][j] = sum;
        }
    }

    return mtx;
}


double** transposeMatrix(double** matrix, int r, int c) {
    double **transpose = (double **)calloc(c , sizeof(double *));
    for (int i=0; i<c; i++)
        transpose[i] = (double *)calloc(r , sizeof(double));

    for (int i = 0; i < r; ++i) {
        for (int j = 0; j < c; ++j) {
            transpose[j][i] = matrix[i][j];
        }
    }

    return transpose;
}


void inverseMatrix(double** matrix, int n) {
    double mtx[n][n*2];
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            mtx[i][j] = matrix[i][j];
        }
        for (int j = n; j < n*2; ++j) {
            if(j-n == i) {
                mtx[i][j] = 1;
            } else {
                mtx[i][j] = 0;
            }
        }
    }

    for (int i = 0; i < n; ++i) {
        double diagNum = mtx[i][i];
        // Get 1 in current pivot column
        for (int j = 0; j < n * 2; ++j) {
            mtx[i][j] = mtx[i][j] / diagNum;
        }

        // Fill in zeros below
        for (int j = i+1; j < n; ++j) {
            double scalar = mtx[j][i];
            for (int k = 0; k < n * 2; ++k) {
                mtx[j][k] = mtx[j][k] - (scalar*mtx[i][k]);
            }
        }
    }

    for (int i = n-1; i >= 0; i--) {
        // Fill in zeros above
        for (int j = i-1; j >= 0; j--) {
            double scalar = mtx[j][i];
            for (int k = 0; k < n * 2; ++k) {
                mtx[j][k] = mtx[j][k] - (scalar*mtx[i][k]);
            }
        }
    }

    // Copy inverse matrix into original matrix
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            matrix[i][j] = mtx[i][n+j];
        }
    }
}

void freeArray(double** array, int row) {
    for (int i = 0; i <  row; i++) {
        free(array[i]);
    }
    free(array);
}
