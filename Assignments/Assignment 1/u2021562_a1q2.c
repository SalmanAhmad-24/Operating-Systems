#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s file1 file2 file3 ...\n", argv[0]);
        return 1;
    }

    // Open all the files
    FILE *files[argc - 1];
    for (int i = 1; i < argc; i++) {
        files[i - 1] = fopen(argv[i], "r");
        if (files[i - 1] == NULL) {
            perror("Error opening file");
            return 1;
        }
    }

    // Read and concatenate lines
    char line[1024];
    int linesPrinted = 0;

    while (1) {
        linesPrinted = 0;

        for (int i = 0; i < argc - 1; i++) {
            if (fgets(line, sizeof(line), files[i]) != NULL) {
                // Remove newline character
                char *newline = strchr(line, '\n');
                if (newline != NULL) {
                    *newline = '\0';
                }

                if (linesPrinted == 0) {
                    // Print the Nth line from each file on a single line
                    printf("%s", line);
                } else {
                    // Print subsequent lines from each file separated by a tab
                    printf("\t%s", line);
                }
                linesPrinted++;
            }
        }

        // If no lines were printed, we have reached the end of all files
        if (linesPrinted == 0) {
            break;
        }

        // Print a newline to separate the concatenated lines
        printf("\n");
    }

    // Close all files
    for (int i = 0; i < argc - 1; i++) {
        fclose(files[i]);
    }

    return 0;
}

