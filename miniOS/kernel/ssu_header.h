#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 파일 노드를 정의합니다.
typedef struct FileNode {
    char fileName[256];
    struct FileNode* next;
} FileNode;

// 디렉토리 구조체를 정의합니다.
typedef struct Directory {
    char dirName[256];
    FileNode* files;
    struct Directory* parent;
    struct Directory* next;
    struct Directory* subDirs;
} Directory;

// 루트 디렉토리와 현재 디렉토리를 정의합니다.
Directory* rootDir = NULL;
Directory* currentDir = NULL;
Directory* clipboardDir = NULL;

// 파일 노드를 생성합니다.
FileNode* createFileNode(const char* fileName) {
    FileNode* newNode = (FileNode*)malloc(sizeof(FileNode));
    if (newNode == NULL) {
        perror("Failed to create file node");
        exit(EXIT_FAILURE);
    }
    strncpy(newNode->fileName, fileName, 255);
    newNode->fileName[255] = '\0';
    newNode->next = NULL;
    return newNode;
}

// 디렉토리를 생성합니다.
Directory* createDirectory(const char* dirName, Directory* parent) {
    Directory* newDir = (Directory*)malloc(sizeof(Directory));
    if (newDir == NULL) {
        perror("Failed to create directory");
        exit(EXIT_FAILURE);
    }
    strncpy(newDir->dirName, dirName, 255);
    newDir->dirName[255] = '\0';
    newDir->files = NULL;
    newDir->parent = parent;
    newDir->next = NULL;
    newDir->subDirs = NULL;
    return newDir;
}

// 새로운 서브 디렉토리를 현재 디렉토리에 추가합니다.
void addSubDirectory(const char* dirName) {
    Directory* newDir = createDirectory(dirName, currentDir);
    newDir->next = currentDir->subDirs;
    currentDir->subDirs = newDir;
    currentDir = newDir;
    printf("New directory '%s' created and moved into it.\n", dirName);
}

// 현재 디렉토리를 상위 디렉토리로 이동합니다.
void moveToParentDirectory() {
    if (currentDir->parent != NULL) {
        currentDir = currentDir->parent;
        printf("Moved to parent directory '%s'.\n", currentDir->dirName);
    } else {
        printf("Already in the root directory.\n");
    }
}

// 현재 디렉토리의 서브 디렉토리로 이동합니다.
void moveToDirectory(const char* dirName) {
    Directory* subDir = currentDir->subDirs;
    while (subDir != NULL) {
        if (strcmp(subDir->dirName, dirName) == 0) {
            currentDir = subDir;
            printf("Moved to directory '%s'.\n", dirName);
            return;
        }
        subDir = subDir->next;
    }
    printf("Directory '%s' not found in current directory.\n", dirName);
}

// 새로운 파일을 생성합니다.
void newfile(const char* fileName) {
    FileNode* newFile = createFileNode(fileName);
    newFile->next = currentDir->files;
    currentDir->files = newFile;
    printf("File '%s' created successfully.\n", fileName);
}

// 클립보드 디렉토리를 비웁니다.
void clearClipboard() {
    FileNode* current = clipboardDir->files;
    while (current != NULL) {
        FileNode* temp = current;
        current = current->next;
        free(temp);
    }
    clipboardDir->files = NULL;
}

// 파일을 현재 디렉토리에서 clipboard 디렉토리로 복사합니다.
void copy(const char* fileName) {
    if (currentDir == clipboardDir) {
        printf("Cannot copy files from the clipboard directory.\n");
        return;
    }

    FileNode* current = currentDir->files;

    while (current != NULL && strcmp(current->fileName, fileName) != 0) {
        current = current->next;
    }

    if (current == NULL) {
        printf("File '%s' not found in current directory.\n", fileName);
        return;
    }

    // 클립보드 디렉토리를 비웁니다.
    clearClipboard();

    // 파일을 복사하여 clipboard 디렉토리로 이동합니다.
    FileNode* copiedFile = createFileNode(fileName);
    copiedFile->next = clipboardDir->files;
    clipboardDir->files = copiedFile;
    printf("File '%s' copied to clipboard.\n", fileName);
}

// 파일을 제거합니다.
void removeFile(const char* fileName) {
    FileNode* prev = NULL;
    FileNode* current = currentDir->files;

    while (current != NULL && strcmp(current->fileName, fileName) != 0) {
        prev = current;
        current = current->next;
    }

    if (current == NULL) {
        printf("File '%s' not found in current directory.\n", fileName);
        return;
    }

    if (prev == NULL) {
        currentDir->files = current->next;
    } else {
        prev->next = current->next;
    }

    free(current);
    printf("File '%s' removed from current directory.\n", fileName);
}

// 파일을 현재 디렉토리로 붙여넣습니다.
void paste() {
    if (clipboardDir->files == NULL) {
        printf("Clipboard is empty.\n");
        return;
    }

    FileNode* fileToPaste = clipboardDir->files;

    // 파일을 현재 디렉토리로 이동합니다.
    FileNode* pastedFile = createFileNode(fileToPaste->fileName);
    pastedFile->next = currentDir->files;
    currentDir->files = pastedFile;

    // 클립보드를 비웁니다.
    clearClipboard();

    printf("File '%s' pasted to current directory.\n", pastedFile->fileName);
}

// 디렉토리 구조를 시각적으로 출력합니다.
void printDirectoryStructure(Directory* dir) {
    printf("now - %s\n", dir->dirName);
    FileNode* currentFile = dir->files;
    while (currentFile != NULL) {
        printf("|--- %s\n", currentFile->fileName);
        currentFile = currentFile->next;
    }

    Directory* currentSubDir = dir->subDirs;
    while (currentSubDir != NULL) {
        printf("|--- <DIR> %s\n", currentSubDir->dirName);
        currentSubDir = currentSubDir->next;
    }
}
