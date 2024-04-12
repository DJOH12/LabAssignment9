#include <stdio.h>
#include <stdlib.h>

// RecordType
struct RecordType
{
	int		id;
	char	name;
	int		order; 
};

// Fill out this structure
struct HashType
{
	struct RecordType *recordType;
	struct HashType *next;
};

// Compute the hash function
int hash(int x)
{
	if((x % 23) < 12 && (x % 23) % 2 == 0)
	return (x % 23) + 10;
	else
	return (x % 23);
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
		// Implement parse data block
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
	int i;
	printf("\nRecords:\n");
	for (i = 0; i < dataSz; ++i)
	{
		printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
	}
	printf("\n\n");
}

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType *pHashArray, int hashSz)
{
	int i;

	for (i=0;i<hashSz;++i)
	{
		// if index is occupied with any records, print all
		printf("index %d -> ", i);
        struct HashType *current = pHashArray[i].next;
        if (current == NULL) {
            printf("NULL\n");
        } else {
            while (current != NULL) {
                printf("%d, %c, %d -> ", current->recordType->id, current->recordType->name, current->recordType->order);
                current = current->next;
            }
            printf("NULL\n");
        }
	}
}

void insertRecord(struct HashType** hashTable, struct RecordType* newRecord) {
	int index = hash(newRecord->id);
	struct HashType *newNode = (struct HashType*)malloc(sizeof(struct HashType));
	newNode->recordType = newRecord;
	newNode->next = hashTable[index];
	hashTable[index] = newNode;
}

void freeHashTable(struct HashType **hashTable, int hashSize) {
    for (int i = 0; i < hashSize; ++i) {
        struct HashType *current = hashTable[i];
        while (current != NULL) {
            struct HashType *next = current->next;
            free(current);
            current = next;
        }
    }
    free(hashTable); 
}


int main(void)
{
	struct RecordType *pRecords;
	int recordSz = 0;

	recordSz = parseData("input_lab_9.txt", &pRecords);
	printRecords(pRecords, recordSz);
	// Your hash implementation

	int hashSize = 23;
	struct HashType **hashTable = (struct HashType**)calloc(hashSize, sizeof(struct HashType));

	for (int i = 0; i < recordSz; ++i) {
		insertRecord(hashTable, &pRecords[i]);
	}

	displayRecordsInHash(hashTable, hashSize);

	freeHashTable(hashTable, hashSize);
    free(pRecords);

	return 0;
}