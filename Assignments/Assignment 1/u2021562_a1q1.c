#include <stdio.h>
#include <stdlib.h>

void freeArray(int **a, int m);

// Function to free a dynamically allocated 2D integer array
void freeArray(int **a, int m) {
    int i;
    for (i = 0; i < m; i++) {
        free(a[i]);
    }
    free(a);
}

int main(int argc, char *argv[]) {
    // Checks if all the required arguments are passed
    if (argc < 3) {
        printf("Insufficient arguments!");
        exit(-1);
    }

    // Opening the input files and assigning them to file pointers
    FILE *infile1 = fopen(argv[1], "r");
    FILE *infile2 = fopen(argv[2], "r");

    if (infile1 == NULL) {
        printf("Error: Failed to open input file: %s\n", argv[1]);
        exit(-2);
    }

    if (infile2 == NULL) {
        printf("Error: Failed to open input file: %s\n", argv[2]);
        fclose(infile1);
        exit(-2);
    }

    int nrows1, ncols1;
    int nrows2, ncols2;

    // Read the number of rows and columns from the input files
    fscanf(infile1, "%d %d", &nrows1, &ncols1);
    fscanf(infile2, "%d %d", &nrows2, &ncols2);

    if (nrows1 != ncols2) {
        printf("Sorry could not multiply. The dimensions are not suitable for multiplication.\n");
        fclose(infile1);
        fclose(infile2);
        exit(-4);
    }

    // Allocate memory for the first matrix (mat1)
    int **mat1 = (int **)malloc(nrows1 * sizeof(int *));
    if (mat1 == NULL) {
        printf("Failed to allocate memory for the matrix\n");
        fclose(infile1);
        fclose(infile2);
        exit(-3);
    }

    for (int i = 0; i < nrows1; i++) {
        mat1[i] = (int *)malloc(ncols1 * sizeof(int));
        if (mat1[i] == NULL) {
            printf("Failed to allocate memory for the row\n");
            freeArray(mat1, nrows1);
            fclose(infile1);
            fclose(infile2);
            exit(-3);
        }
    }

    // Read data for the first matrix (mat1) from the input file
    for (int i = 0; i < nrows1; i++) {
        for (int j = 0; j < ncols1; j++) {
            fscanf(infile1, "%d ", &mat1[i][j]);
        }
    }

    // Allocate memory for the second matrix (mat2)
    int **mat2 = (int **)malloc(nrows2 * sizeof(int *));
    if (mat2 == NULL) {
        printf("Failed to allocate memory for the matrix\n");
        fclose(infile1);
        fclose(infile2);
        freeArray(mat1, nrows1);
        exit(-3);
    }

    for (int i = 0; i < nrows2; i++) {
        mat2[i] = (int *)malloc(ncols2 * sizeof(int));
        if (mat2[i] == NULL) {
            printf("Failed to allocate memory for the row\n");
            fclose(infile1);
            fclose(infile2);
            freeArray(mat1, nrows1);
            freeArray(mat2, nrows2);
            exit(-3);
        }
    }

    // Read data for the second matrix (mat2) from the input file
    for (int i = 0; i < nrows2; i++) {
        for (int j = 0; j < ncols2; j++) {
            fscanf(infile2, "%d ", &mat2[i][j]);
        }
    }

    // Allocate memory for the output matrix (output)
    int **output = (int **)malloc(nrows1 * sizeof(int *));
    if (output == NULL) {
        printf("Failed to allocate memory for the matrix\n");
        fclose(infile1);
        fclose(infile2);
        freeArray(mat1, nrows1);
        freeArray(mat2, nrows2);
        exit(-3);
    }

    for (int i = 0; i < nrows1; i++) {
        output[i] = (int *)malloc(ncols2 * sizeof(int));
        if (output[i] == NULL) {
            printf("Failed to allocate memory for the row\n");
            fclose(infile1);
            fclose(infile2);
            freeArray(mat1, nrows1);
            freeArray(mat2, nrows2);
            freeArray(output, nrows1);
            exit(-3);
        }
    }

    // Perform matrix multiplication
    for (int i = 0; i < nrows1; i++) {
        for (int j = 0; j < ncols2; j++) {
            int sum = 0;
            for (int k = 0; k < nrows2; k++) {
                sum = sum + mat1[i][k] * mat2[k][j];
            }
            output[i][j] = sum;
        }
    }

    // Check if an output file was passed as an argument
    if (argc > 3) {
        FILE *outfile = fopen(argv[3], "w");
        if (outfile == NULL) {
            printf("Error: Failed to open output file: %s\n", argv[3]);
            fclose(infile1);
            fclose(infile2);
            freeArray(mat1, nrows1);
            freeArray(mat2, nrows2);
            freeArray(output, nrows1);
            exit(-2);
        }

        // Write the output matrix to the output file
        for (int i = 0; i < nrows1; i++) {
            for (int j = 0; j < ncols2; j++) {
                fprintf(outfile, "%d ", output[i][j]);
            }
            fprintf(outfile, "\n");
        }

        fclose(outfile);
    } else {
        // Print the output matrix to the console
        printf("The output matrix is:\n");
        for (int i = 0; i < nrows1; i++) {
            for (int j = 0; j < ncols2; j++) {
                printf("%d ", output[i][j]);
            }
            printf("\n");
        }
    }

    // Close input and output files, and free memory
    fclose(infile1);
    fclose(infile2);
    freeArray(mat1, nrows1);
    freeArray(mat2, nrows2);
    freeArray(output, nrows1);

    return 0;
}
