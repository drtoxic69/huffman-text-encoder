#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TREE_HT 100
#define MAX_STRING_LENGTH 1000
#define MAX_CHARS 256
#define LEFT_CHILD 0
#define RIGHT_CHILD 1

struct MinHeapNode {
    char data;
    unsigned freq;
    struct MinHeapNode *left, *right;
};

struct MinHeap {
    unsigned size;
    unsigned capacity;
    struct MinHeapNode** array;
};

struct MinHeapNode* newNode(char data, unsigned freq) {
    struct MinHeapNode* temp = (struct MinHeapNode*)malloc(sizeof(struct MinHeapNode));
    temp->left = temp->right = NULL;
    temp->data = data;
    temp->freq = freq;
    return temp;
}

struct MinHeap* createMinHeap(unsigned capacity) {
    struct MinHeap* minHeap = (struct MinHeap*)malloc(sizeof(struct MinHeap));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array = (struct MinHeapNode**)malloc(minHeap->capacity * sizeof(struct MinHeapNode*));
    return minHeap;
}

void swapMinHeapNode(struct MinHeapNode** a, struct MinHeapNode** b) {
    struct MinHeapNode* t = *a;
    *a = *b;
    *b = t;
}

void minHeapify(struct MinHeap* minHeap, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < minHeap->size && minHeap->array[left]->freq < minHeap->array[smallest]->freq)
        smallest = left;

    if (right < minHeap->size && minHeap->array[right]->freq < minHeap->array[smallest]->freq)
        smallest = right;

    if (smallest != idx) {
        swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]);
        minHeapify(minHeap, smallest);
    }
}

int isSizeOne(struct MinHeap* minHeap) {
    return (minHeap->size == 1);
}

struct MinHeapNode* extractMin(struct MinHeap* minHeap) {
    struct MinHeapNode* temp = minHeap->array[0];
    minHeap->array[0] = minHeap->array[minHeap->size - 1];
    --minHeap->size;
    minHeapify(minHeap, 0);
    return temp;
}

void insertMinHeap(struct MinHeap* minHeap, struct MinHeapNode* minHeapNode) {
    ++minHeap->size;
    int i = minHeap->size - 1;
    while (i && minHeapNode->freq < minHeap->array[(i - 1) / 2]->freq) {
        minHeap->array[i] = minHeap->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    minHeap->array[i] = minHeapNode;
}

void buildMinHeap(struct MinHeap* minHeap) {
    int n = minHeap->size - 1;
    for (int i = (n - 1) / 2; i >= 0; --i)
        minHeapify(minHeap, i);
}

void printLine(char c, int length) {
    for (int i = 0; i < length; i++) {
        printf("%c", c);
    }
    printf("\n");
}

void printArr(int arr[], int n) {
    for (int i = 0; i < n; ++i) {
        printf("\033[1;36m%d\033[0m", arr[i]); // Cyan color for binary digits
    }
}

int isLeaf(struct MinHeapNode* root) {
    return !(root->left) && !(root->right);
}

#define BOX_WIDTH 70
#define LEFT_CHILD 0
#define RIGHT_CHILD 1

// Unicode box drawing characters
// ASCII box drawing characters
#define TOP_LEFT "+"
#define TOP_RIGHT "+"
#define BOTTOM_LEFT "+"
#define BOTTOM_RIGHT "+"
#define HORIZONTAL "-"
#define VERTICAL "|"
#define T_RIGHT "+"
#define T_LEFT "+"
#define VERTICAL_SPLIT "+"
#define HORIZONTAL_SPLIT "-"

// Function to print a horizontal line with ASCII characters
void printHorizontalLine(const char* left, const char* middle, const char* right, const char* fill) {
    printf("%s", left);
    for (int i = 0; i < BOX_WIDTH - 2; i++) {
        printf("%s", fill);
    }
    printf("%s\n", right);
}

// Function to display a cleaner ASCII box
void printCodes(struct MinHeapNode* root, int arr[], size_t arr_size, int top) {
    static int first_call = 1;
    
    if (!root) return;
    
    if (top >= arr_size) {
        fprintf(stderr, "Error: Buffer overflow detected.\n");
        return;
    }

    // Print header on first call
    if (first_call) {
        printf("\n============== HUFFMAN CODES TABLE =============\n\n");
        first_call = 0;
    }

    if (root->left) {
        arr[top] = LEFT_CHILD;
        printCodes(root->left, arr, arr_size, top + 1);
    }
    
    if (root->right) {
        arr[top] = RIGHT_CHILD;
        printCodes(root->right, arr, arr_size, top + 1);
    }
    
    if (isLeaf(root)) {
        // Print character info in a clean format
        printf("Character: '%c' |   Code:   ", root->data);
        
        // Print the Huffman code
        for (int i = 0; i < top; i++) {
            printf("%d", arr[i]);
        }
        
        // Print frequency
        printf(" | Frequency: %-2d\n", root->freq);
        
    }

    // Reset first_call flag when we're done
    if (root == NULL || (isLeaf(root) && !root->left && !root->right)) {
        first_call = 1;
    }
}

struct MinHeap* createAndBuildMinHeap(char data[], int freq[], int size) {
    struct MinHeap* minHeap = createMinHeap(size);
    for (int i = 0; i < size; ++i)
        minHeap->array[i] = newNode(data[i], freq[i]);
    minHeap->size = size;
    buildMinHeap(minHeap);
    return minHeap;
}

struct MinHeapNode* buildHuffmanTree(char data[], int freq[], int size) {
    struct MinHeapNode *left, *right, *top;
    struct MinHeap* minHeap = createAndBuildMinHeap(data, freq, size);

    while (!isSizeOne(minHeap)) {
        left = extractMin(minHeap);
        right = extractMin(minHeap);
        top = newNode('$', left->freq + right->freq);
        top->left = left;
        top->right = right;
        insertMinHeap(minHeap, top);
    }
    return extractMin(minHeap);
}

void HuffmanCodes(char data[], int freq[], int size) {
    struct MinHeapNode* root = buildHuffmanTree(data, freq, size);
    int arr[MAX_TREE_HT], top = 0;
    
    printf("\n================================================\n");
    printf("            HUFFMAN CODES FOR EACH CHARACTER            \n");
    printf("================================================\n");
    printCodes(root, arr, MAX_TREE_HT, top);
}

void getHuffmanCode(struct MinHeapNode* root, char c, int arr[], int* arrTop, int* found) {
    if (!root) return;

    if (isLeaf(root) && root->data == c) {
        *found = 1;
        return;
    }

    if (root->left) {
        arr[*arrTop] = LEFT_CHILD;
        (*arrTop)++;
        getHuffmanCode(root->left, c, arr, arrTop, found);
        if (*found) return;
        (*arrTop)--;
    }

    if (root->right) {
        arr[*arrTop] = RIGHT_CHILD;
        (*arrTop)++;
        getHuffmanCode(root->right, c, arr, arrTop, found);
        if (*found) return;
        (*arrTop)--;
    }
}

void encodeString(struct MinHeapNode* root, const char* input) {
    printf("\n================================================\n");
    printf("                  ENCODED MESSAGE                  \n");
    printf("================================================\n");
    printf("\nOriginal text: %s\n", input);
    printf("Encoded bits: ");
    
    for (int i = 0; input[i] != '\0'; i++) {
        int arr[MAX_TREE_HT], arrTop = 0, found = 0;
        getHuffmanCode(root, input[i], arr, &arrTop, &found);
        
        if (found) {
            for (int j = 0; j < arrTop; j++) {
                printf("%d", arr[j]);
            }
        }
    }
    printf("\n\n");
    printf("================================================\n");
}

void calculateFrequency(const char* str, char* data, int* freq, int* size) {
    int count[MAX_CHARS] = {0};
    int len = strlen(str);
    *size = 0;

    for(int i = 0; i < len; i++) {
        count[(unsigned char)str[i]]++;
    }

    for(int i = 0; i < MAX_CHARS; i++) {
        if(count[i] > 0) {
            data[*size] = (char)i;
            freq[*size] = count[i];
            (*size)++;
        }
    }
}

int main() {
    char input[MAX_STRING_LENGTH];
    char data[MAX_CHARS];
    int freq[MAX_CHARS];
    int size = 0;

    printf("\n================================================\n");
    printf("            HUFFMAN ENCODING PROGRAM               \n");
    printf("================================================\n\n");
    
    printf("Enter a string to encode: ");
    if (fgets(input, MAX_STRING_LENGTH, stdin) == NULL) {
        printf("Error reading input!\n");
        return 1;
    }
    input[strcspn(input, "\n")] = 0;

    if (strlen(input) == 0) {
        printf("Error: Empty input string!\n");
        return 1;
    }

    calculateFrequency(input, data, freq, &size);
    struct MinHeapNode* root = buildHuffmanTree(data, freq, size);

    HuffmanCodes(data, freq, size);
    encodeString(root, input);

    return 0;
}