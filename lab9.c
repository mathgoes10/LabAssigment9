//Matheus Goes Lima
//Lab 9

#include <stdio.h>
#include <stdlib.h>

// RecordType
struct RecordType
{
    int     id;
    char    name;
    int     order; 
};

// HashType
struct HashNode
{
    struct RecordType record;
    struct HashNode* next;
};

struct HashType
{
    struct HashNode** array;
    int size; 
};

// Compute the hash function
int hash(int x, int size)
{
    return x % size;
}

// Initialize HashTable
struct HashType* createHashTable(int size)
{
    struct HashType* hashTable = (struct HashType*)malloc(sizeof(struct HashType));
    hashTable->size = size;
    hashTable->array = (struct HashNode**)malloc(size * sizeof(struct HashNode*));
    for (int i = 0; i < size; ++i)
        hashTable->array[i] = NULL;
    return hashTable;
}

// parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData)
{
    FILE* inFile = fopen(inputFileName, "r");
    int dataSz = 0;
    int i, n;
    char c;
    struct RecordType *pRecord;
    *ppData = NULL;

    if (inFile)
    {
        fscanf(inFile, "%d\n", &dataSz);
        *ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
        if (*ppData == NULL)
        {
            printf("Cannot allocate memory\n");
            exit(-1);
        }
        for (i = 0; i < dataSz; ++i)
        {
            pRecord = *ppData + i;
            fscanf(inFile, "%d ", &n);
            pRecord->id = n;
            fscanf(inFile, "%c ", &c);
            pRecord->name = c;
            fscanf(inFile, "%d ", &n);
            pRecord->order = n;
        }
        fclose(inFile);
    }
    return dataSz;
}

// prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
    printf("\nRecords:\n");
    for (int i = 0; i < dataSz; ++i)
    {
        printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
    }
    printf("\n");
}

// Display records in the hash structure
void displayRecordsInHash(struct HashType *hashTable)
{
    printf("\nRecords in Hash Table:\n");
    for (int i = 0; i < hashTable->size; ++i)
    {
        struct HashNode* currentNode = hashTable->array[i];
        if (currentNode != NULL)
        {
            printf("Index %d -> ", i);
            while (currentNode != NULL)
            {
                printf("%d %c %d -> ", currentNode->record.id, currentNode->record.name, currentNode->record.order);
                currentNode = currentNode->next;
            }
            printf("\n");
        }
    }
}


// Insert a record into HashTable
void insertRecord(struct HashType* hashTable, struct RecordType record)
{
    int index = hash(record.id, hashTable->size);
    struct HashNode* newNode = (struct HashNode*)malloc(sizeof(struct HashNode));
    newNode->record = record;
    newNode->next = hashTable->array[index];
    hashTable->array[index] = newNode;
}

int main(void)
{
    struct RecordType *pRecords;
    int recordSz = 0;
    recordSz = parseData("input.txt", &pRecords);
    printRecords(pRecords, recordSz);

    // Create HashTable
    int hashTableSize = 10; // Adjust the size as needed
    struct HashType* hashTable = createHashTable(hashTableSize);

    // Insert records into HashTable
    for (int i = 0; i < recordSz; ++i)
        insertRecord(hashTable, pRecords[i]);

    // Display records in HashTable
    displayRecordsInHash(hashTable);

    // Free allocated memory
    free(pRecords);

    // Free HashTable
    for (int i = 0; i < hashTableSize; ++i)
    {
        struct HashNode* currentNode = hashTable->array[i];
        while (currentNode != NULL)
        {
            struct HashNode* temp = currentNode;
            currentNode = currentNode->next;
            free(temp);
        }
    }
    free(hashTable->array);
    free(hashTable);

    return 0;
}