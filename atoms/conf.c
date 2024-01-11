#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 256

char* transformString(const char* oldString) {
    if (oldString == NULL || oldString[0] == '\0') {
        return NULL;
    }
    if (strncmp(oldString, "./", 2) == 0) {
        oldString += 2;
    }
    if (strncmp(oldString, "../", 3) == 0) {
        oldString += 3;
    }

    size_t length = strlen(oldString);
    char* newString = (char*)malloc((length + 1) * sizeof(char));
    if (newString == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i < length; i++) {
        if (oldString[i] == '/') {
            newString[i] = '_';
        } else {
            newString[i] = toupper(oldString[i]);
        }
    }

    oldString = newString;

    for (size_t i = 0; i < length; i++) {
        if (oldString[i] == '.') {
            newString[i] = '_';
        } else {
            newString[i] = toupper(oldString[i]);
        }
    }

    newString[length] = '\0';

    return newString;
}

void generateHeader(const char* inputFilePath, const char* outputFilePath) {
    FILE *inputFile, *outputFile;
    char line[MAX_LINE_LENGTH];

    inputFile = fopen(inputFilePath, "r");
    if (inputFile == NULL) {
        perror("Error opening input file");
        exit(EXIT_FAILURE);
    }

    outputFile = fopen(outputFilePath, "w");
    if (outputFile == NULL) {
        perror("Error opening output file");
        fclose(inputFile);
        exit(EXIT_FAILURE);
    }

    const char* header_name = transformString(outputFilePath);

    fprintf(outputFile, "#ifndef %s\n#define %s\n\n", header_name, header_name);

    while (fgets(line, MAX_LINE_LENGTH, inputFile) != NULL) {
        char *token = strtok(line, " =");
        char *left, *rigth;

        if (token != NULL) {
            left = strdup(token);
            token = strtok(NULL, " =");
        } else {
            printf("Error parsing input\n");
            exit(1);
        }

        if (token != NULL) {
            rigth = strdup(token);
        } else {
            printf("Error parsing input\n");
            free(left);
            exit(1);
        }

        fprintf(outputFile, "#define %s %s", left, rigth);
    }

    fprintf(outputFile, "\n#endif // %s\n", header_name);

    // Close files
    fclose(inputFile);
    fclose(outputFile);

    printf("Header file generated successfully.\n");
}

int main(const int argc, const char** argv) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input_file_path> <output_file_path>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    generateHeader(argv[1], argv[2]);

    return 0;
}
