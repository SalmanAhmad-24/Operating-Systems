#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 *   ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ 
 *  |   |   |   |   |                       |   |
 *  | 0 | 1 | 2 | 3 |     .....             |127|
 *  |___|___|___|___|_______________________|___|
 *  |   \    <-----  data blocks ------>
 *  |     \
 *  |       \
 *  |         \
 *  |           \
 *  |             \
 *  |               \
 *  |                 \
 *  |                   \
 *  |                     \
 *  |                       \
 *  |                         \
 *  |                           \
 *  |                             \
 *  |                               \
 *  |                                 \
 *  |                                   \
 *  |                                     \
 *  |                                       \
 *  |                                         \
 *  |                                           \
 *  |     <--- super block --->                   \
 *  |______________________________________________|
 *  |               |      |      |        |       |
 *  |        free   |      |      |        |       |
 *  |       block   |inode0|inode1|   .... |inode15|
 *  |        list   |      |      |        |       |
 *  |_______________|______|______|________|_______|
 *
 *
 */

#define FILENAME_MAXLEN 8
#define MAX_PATH_LEN 100
#define MAX_CMD_LEN 100
#define MAX_FILES 16
#define MAX_BLOCKS 128
#define BLOCK_SIZE 1024

typedef struct inode {
    int dir;           // boolean value. 1 if it's a directory.
    char name[FILENAME_MAXLEN];
    int size;          // actual file/directory size in bytes.
    int blockptrs[8];   // direct pointers to blocks containing file's content.
    int used;          // boolean value. 1 if the entry is in use.
    int rsvd;          // reserved for future use
} inode;

typedef struct dirent {
    char name[FILENAME_MAXLEN];
    int namelen;       // length of entry name
    int inode;         // this entry inode index
} dirent;

inode inodes[MAX_FILES];
dirent directories[MAX_FILES];
char disk[MAX_BLOCKS][BLOCK_SIZE];

int find_free_inode() {
    for (int i = 0; i < MAX_FILES; ++i) {
        if (!inodes[i].used) {
            return i;
        }
    }
    return -1;  // No free inode
}

int find_free_block() {
    for (int i = 0; i < MAX_BLOCKS; ++i) {
        if (disk[i][0] == '\0') {
            return i;
        }
    }
    return -1;  // No free block
}
void create_file(const char *filename, int size) {
    // Find the last '/' in the filename
    const char *lastSlash = strrchr(filename, '/');
    
    // Check if '/' is found and not at the beginning or end of the string
    if (lastSlash == NULL || lastSlash == filename || lastSlash[1] == '\0') {
        printf("Error: Invalid file path format.\n");
        return;
    }

    // Calculate the lengths of dirname and fname
    size_t dirnameLen = lastSlash - filename;
    size_t fnameLen = strlen(lastSlash + 1);

    // Allocate buffers for dirname and fname
    char dirname[MAX_PATH_LEN];
    char fname[FILENAME_MAXLEN];

    // Copy dirname and fname from the filename
    strncpy(dirname, filename, dirnameLen);
    dirname[dirnameLen] = '\0';

    strncpy(fname, lastSlash + 1, fnameLen);
    fname[fnameLen] = '\0';

    int dir_inode = -1;
    for (int i = 0; i < MAX_FILES; ++i) {
        if (inodes[i].used && inodes[i].dir && strcmp(inodes[i].name, dirname) == 0) {
            dir_inode = i;
            break;
        }
    }

    if (dir_inode == -1) {
        printf("Error: The directory %s in the given path does not exist.\n", dirname);
        return;
    }

    if (size > 8 * BLOCK_SIZE) {
        printf("Error: Not enough space on the disk.\n");
        return;
    }

    int file_inode = find_free_inode();
    int file_block = find_free_block();

    if (file_inode == -1 || file_block == -1) {
        printf("Error: Not enough space on the disk.\n");
        return;
    }

    inodes[file_inode].used = 1;
    inodes[file_inode].dir = 0;
    strcpy(inodes[file_inode].name, fname);
    inodes[file_inode].size = size;
    inodes[file_inode].blockptrs[0] = file_block;

    for (int i = 0; i < size; ++i) {
        disk[file_block][i % BLOCK_SIZE] = 'a' + (i % 26);
    }
    disk[file_block][size % BLOCK_SIZE] = '\0';

    printf("File %s created successfully.\n", filename);
}


// void create_file(const char *filename, int size) {
//     // Find the last '/' in the filename
//     const char *lastSlash = strrchr(filename, '/');
    
//     // Check if '/' is found and not at the beginning or end of the string
//     if (lastSlash == NULL || lastSlash == filename || lastSlash[1] == '\0') {
//         printf("Error: Invalid file path format.\n");
//         return;
//     }

//     // Calculate the lengths of dirname and fname
//     size_t dirnameLen = lastSlash - filename;
//     size_t fnameLen = strlen(lastSlash + 1);

//     // Allocate buffers for dirname and fname
//     char dirname[MAX_PATH_LEN];
//     char fname[FILENAME_MAXLEN];

//     // Copy dirname and fname from the filename
//     strncpy(dirname, filename, dirnameLen);
//     dirname[dirnameLen] = '\0';

//     strncpy(fname, lastSlash + 1, fnameLen);
//     fname[fnameLen] = '\0';

//     int dir_inode = -1;
//     for (int i = 0; i < MAX_FILES; ++i) {
//         if (inodes[i].used && inodes[i].dir && strcmp(inodes[i].name, dirname) == 0) {
//             dir_inode = i;
//             break;
//         }
//     }

//     if (dir_inode == -1) {
//         printf("Error: The directory %s in the given path does not exist.\n", dirname);
//         return;
//     }

//     if (size > 8 * BLOCK_SIZE) {
//         printf("Error: Not enough space on the disk.\n");
//         return;
//     }

//     int file_inode = find_free_inode();
//     int file_block = find_free_block();

//     if (file_inode == -1 || file_block == -1) {
//         printf("Error: Not enough space on the disk.\n");
//         return;
//     }

//     inodes[file_inode].used = 1;
//     inodes[file_inode].dir = 0;
//     strcpy(inodes[file_inode].name, fname);
//     inodes[file_inode].size = size;
//     inodes[file_inode].blockptrs[0] = file_block;

//     for (int i = 0; i < size; ++i) {
//         disk[file_block][i % BLOCK_SIZE] = 'a' + (i % 26);
//     }
//     disk[file_block][size % BLOCK_SIZE] = '\0';

//     printf("File %s created successfully.\n", filename);
// }

// void create_file(const char *filename, int size) {
//     for (int i = 0; i < MAX_FILES; ++i) {
//         if (inodes[i].used && strcmp(inodes[i].name, filename) == 0) {
//             printf("Error: The file already exists.\n");
//             return;
//         }
//     }

//     char dirname[MAX_PATH_LEN];
//     char fname[FILENAME_MAXLEN];
//     int result = sscanf(filename, "%[^/\n]/%s", dirname, fname);

//     int dir_inode = -1;
//     for (int i = 0; i < MAX_FILES; ++i) {
//         if (inodes[i].used && inodes[i].dir && strcmp(inodes[i].name, dirname) == 0) {
//             dir_inode = i;
//             break;
//         }
//     }

//     if (dir_inode == -1) {
//         printf("Debug: dirname = '%s', fname = '%s'\n", dirname, fname);

//         printf("Error: The directory %.*s in the given path does not exist.\n", (int)strlen(dirname), dirname);
//         return;
//     }

//     if (size > 8 * BLOCK_SIZE) {
//         printf("Error: Not enough space on the disk.\n");
//         return;
//     }

//     int file_inode = find_free_inode();
//     int file_block = find_free_block();

//     if (file_inode == -1 || file_block == -1) {
//         printf("Error: Not enough space on the disk.\n");
//         return;
//     }

//     inodes[file_inode].used = 1;
//     inodes[file_inode].dir = 0;
//     strcpy(inodes[file_inode].name, fname);
//     inodes[file_inode].size = size;
//     inodes[file_inode].blockptrs[0] = file_block;

//     for (int i = 0; i < size; ++i) {
//         disk[file_block][i % BLOCK_SIZE] = 'a' + (i % 26);
//     }
//     disk[file_block][size % BLOCK_SIZE] = '\0';

//     printf("File %s created successfully.\n", filename);
// }

void delete_file(const char *filename) {
    int file_inode = -1;
    for (int i = 0; i < MAX_FILES; ++i) {
        if (inodes[i].used && !inodes[i].dir && strcmp(inodes[i].name, filename) == 0) {
            file_inode = i;
            break;
        }
    }

    if (file_inode == -1) {
        printf("Error: The file does not exist.\n");
        return;
    }

    int file_block = inodes[file_inode].blockptrs[0];
    memset(disk[file_block], 0, BLOCK_SIZE);

    inodes[file_inode].used = 0;

    printf("File %s deleted successfully.\n", filename);
}

void copy_file(const char *srcname, const char *dstname) {
    int src_inode = -1;
    for (int i = 0; i < MAX_FILES; ++i) {
        if (inodes[i].used && !inodes[i].dir && strcmp(inodes[i].name, srcname) == 0) {
            src_inode = i;
            break;
        }
    }

    if (src_inode == -1) {
        printf("Error: The source file does not exist.\n");
        return;
    }

    char dst_dirname[MAX_PATH_LEN];
    char dst_fname[FILENAME_MAXLEN];
    int result = sscanf(dstname, "%[^/]/%s", dst_dirname, dst_fname);

    int dst_dir_inode = -1;
    for (int i = 0; i < MAX_FILES; ++i) {
        if (inodes[i].used && inodes[i].dir && strcmp(inodes[i].name, dst_dirname) == 0) {
            dst_dir_inode = i;
            break;
        }
    }

    if (dst_dir_inode == -1) {
        printf("Error: The destination directory %s does not exist.\n", dst_dirname);
        return;
    }

    if (inodes[dst_dir_inode].size + inodes[src_inode].size > 8 * BLOCK_SIZE) {
        printf("Error: Not enough space on the disk.\n");
        return;
    }

    int dst_inode = find_free_inode();
    int dst_block = find_free_block();

    if (dst_inode == -1 || dst_block == -1) {
        printf("Error: Not enough space on the disk.\n");
        return;
    }

    inodes[dst_inode].used = 1;
    inodes[dst_inode].dir = 0;
    strcpy(inodes[dst_inode].name, dst_fname);
    inodes[dst_inode].size = inodes[src_inode].size;
    inodes[dst_inode].blockptrs[0] = dst_block;

    int src_block = inodes[src_inode].blockptrs[0];
    memcpy(disk[dst_block], disk[src_block], inodes[src_inode].size);

    printf("File %s copied to %s successfully.\n", srcname, dstname);
}

void move_file(const char *srcname, const char *dstname) {
    copy_file(srcname, dstname);
    delete_file(srcname);

    printf("File %s moved to %s successfully.\n", srcname, dstname);
}

void create_directory(const char *dirname) {
    // Find the last '/' in the dirname
    const char *lastSlash = strrchr(dirname, '/');
    
    // Check if '/' is found and not at the beginning or end of the string
    if (lastSlash == NULL || lastSlash == dirname || lastSlash[1] == '\0') {
        printf("Error: Invalid directory path format.\n");
        return;
    }

    // Calculate the lengths of parent_dirname and new_dirname
    size_t parentDirLen = lastSlash - dirname;
    size_t newDirLen = strlen(lastSlash + 1);

    // Allocate buffers for parent_dirname and new_dirname
    char parent_dirname[MAX_PATH_LEN];
    char new_dirname[FILENAME_MAXLEN];

    // Copy parent_dirname and new_dirname from the dirname
    strncpy(parent_dirname, dirname, parentDirLen);
    parent_dirname[parentDirLen] = '\0';

    strncpy(new_dirname, lastSlash + 1, newDirLen);
    new_dirname[newDirLen] = '\0';

    for (int i = 0; i < MAX_FILES; ++i) {
        if (inodes[i].used && inodes[i].dir && strcmp(inodes[i].name, new_dirname) == 0) {
            printf("Error: The directory %s already exists.\n", new_dirname);
            return;
        }
    }

    int parent_dir_inode = -1;
    for (int i = 0; i < MAX_FILES; ++i) {
        if (inodes[i].used && inodes[i].dir && strcmp(inodes[i].name, parent_dirname) == 0) {
            parent_dir_inode = i;
            break;
        }
    }

    if (parent_dir_inode == -1) {
        printf("Error: The directory %s in the given path does not exist.\n", parent_dirname);
        return;
    }

    int dir_inode = find_free_inode();

    if (dir_inode == -1) {
        printf("Error: Not enough space on the disk.\n");
        return;
    }

    inodes[dir_inode].used = 1;
    inodes[dir_inode].dir = 1;
    strcpy(inodes[dir_inode].name, new_dirname);
    inodes[dir_inode].size = 0;

    printf("Directory %s created successfully.\n", dirname);
}


void remove_directory(const char *dirname) {
    int dir_inode = -1;
    for (int i = 0; i < MAX_FILES; ++i) {
        if (inodes[i].used && inodes[i].dir && strcmp(inodes[i].name, dirname) == 0) {
            dir_inode = i;
            break;
        }
    }

    if (dir_inode == -1) {
        printf("Error: The directory %s does not exist.\n", dirname);
        return;
    }

    for (int i = 0; i < MAX_FILES; ++i) {
        if (inodes[i].used && inodes[i].dir && strncmp(inodes[i].name, dirname, strlen(dirname)) == 0) {
            remove_directory(inodes[i].name);
        }
        if (inodes[i].used && !inodes[i].dir && strncmp(inodes[i].name, dirname, strlen(dirname)) == 0) {
            delete_file(inodes[i].name);
        }
    }

    for (int i = 0; i < MAX_BLOCKS; ++i) {
        memset(disk[i], 0, BLOCK_SIZE);
    }

    inodes[dir_inode].used = 0;

    printf("Directory %s removed successfully.\n", dirname);
}

void list_all() {
    for (int i = 0; i < MAX_FILES; ++i) {
        if (inodes[i].used) {
            printf("%s %d\n", inodes[i].name, inodes[i].size);
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    char command[MAX_CMD_LEN];
    char arg1[MAX_CMD_LEN];
    char arg2[MAX_CMD_LEN];

    while (fscanf(file, "%s", command) != EOF) {
        if (strcmp(command, "CR") == 0) {
            fscanf(file, "%s %s", arg1, arg2);
            create_file(arg1, *(int*)arg2);
        } else if (strcmp(command, "DL") == 0) {
            fscanf(file, "%s", arg1);
            delete_file(arg1);
        } else if (strcmp(command, "CP") == 0) {
            fscanf(file, "%s %s", arg1, arg2);
            copy_file(arg1, arg2);
        } else if (strcmp(command, "MV") == 0) {
            fscanf(file, "%s %s", arg1, arg2);
            move_file(arg1, arg2);
        } else if (strcmp(command, "CD") == 0) {
            fscanf(file, "%s", arg1);
            create_directory(arg1);
        } else if (strcmp(command, "DD") == 0) {
            fscanf(file, "%s", arg1);
            remove_directory(arg1);
        } else if (strcmp(command, "LL") == 0) {
            list_all();
        } else {
            printf("Unknown command: %s\n", command);
        }
    }

    fclose(file);

    return 0;
}
