#include <stdio.h>

#define TABLE_SIZE 15

// RecordType
struct RecordType
{
	int		id;
	char	name;
	int		order; 
};

// Linked List of Records is required to implement separate chaining feature
struct RecLL{
	struct RecordType record;
	struct RecLL * next;
};

// hash table consists of array of record linked lists
struct HashType
{
	struct RecLL * arr[TABLE_SIZE];
};

// Compute the hash function
int hash(int x)
{
	return x % TABLE_SIZE;
}

// Insert record into hash table
struct HashType * insertRecord(struct HashType * hash_table, struct RecordType record){
	int hash_key = hash(record.id);
	struct RecLL * rec = (struct RecLL *)malloc(sizeof(struct RecLL));
	rec->record.id = record.id;
	rec->record.name = record.name;
	rec->record.order = record.order;
	rec->next = NULL;
	if(hash_table->arr[hash_key] == NULL){
		hash_table->arr[hash_key] = rec;
	}
	else{
		struct RecLL * tmp = hash_table->arr[hash_key];
		while(tmp->next != NULL)
			tmp = tmp->next;
		tmp->next = rec;
	}
	return hash_table;
}

struct HashType * freeTable(struct HashType * hash_table){
	if(hash_table == NULL)
		return NULL;
	for(int i = 0; i < TABLE_SIZE; i++){
		struct RecLL * tmp = hash_table->arr[i];
		while(tmp != NULL){
			struct RecLL * tmp2 = tmp;
			tmp = tmp->next;
			free(tmp2);
		}
	}
	return NULL;
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
		struct RecLL * tmp = pHashArray->arr[i];
		printf("index %d", i);
		while(tmp != NULL){
			printf(" -> ");
			printf("%d, %c, %d", tmp->record.id, tmp->record.name, tmp->record.order);
			tmp = tmp->next;
		}
		printf("\n");
	}
}

int main(void)
{
	struct RecordType *pRecords;
	int recordSz = 0;

	recordSz = parseData("input.txt", &pRecords);
	printRecords(pRecords, recordSz);
	// Your hash implementation
	struct HashType * hash_table = (struct HashType *)malloc(sizeof(struct HashType));
	for(int j = 0; j < TABLE_SIZE; j++)
		hash_table->arr[j] = NULL;

	for(int i = 0; i < recordSz; i++){
		hash_table = insertRecord(hash_table, pRecords[i]);
	}
	displayRecordsInHash(hash_table, TABLE_SIZE);
	hash_table = freeTable(hash_table);
}
