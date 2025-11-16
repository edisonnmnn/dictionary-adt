// name: Edison Kuo
// cruzID: edkuo
// assignment: pa5

// Dictionary.c
// Implementation of Dictionary ADT

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <math.h>
#include "Dictionary.h"

// private structs, functions, and constants -----------------------------------

typedef uint64_t codeType;

// Structs
typedef struct Element {
    keyType key;
    valType val;
    codeType code;
 } Element;

typedef struct DictionaryObj {
    int64_t* tbl; // table
    size_t tblSize; // table size
    double tblLoadFactor; // table load factor
    Element* data; // data
    uint64_t numPairs; // number of pairs
    uint64_t numDelElem; // number of deleted elements
    size_t dataSize; // data size
    uint64_t dataNext; // data next index
    double dataDensity; // data density
} DictionaryObj;

// Constants for table array (sparse, indices to data array)
const size_t TableInitialSize = 8;
const double TableLoadFactorThreshold = 0.67;
const int TableExpandFactor1 = 4;
const int TableExpandFactor2 = 2;
const int TableSizeThreshold = 50000;
const int TableEmpty = 0;
const int TableDeleted = -1;

// Constants for data array (dense, dictionary data)
const size_t DataInitialSize = 1;
const double DataExpandFactor = 1.5;
const double DataDensityThreshold = 0.8;
const keyType DataEmpty = NULL;
const char dummy[] = {255, 0}; // array is const
const keyType DataDeleted = dummy; // pointer is const


// Helper Functions

// hash()
// Returns the hash code for key k.
codeType hash(keyType k) {
    codeType result = 0x2096DB9D4D43C94E;
    // binary: 0010000010010110110110111001110101001101010000111100100101001110
    codeType nextChar = *k;             // get first char and convert to codeType
    while(nextChar) {                   // while not at end of k
        result ^= nextChar;                 // result = result (exor) nextChar
        result = (result<<5)|(result>>59);  // left rotate result by 5 bits
        nextChar = *(++k);                  // get next char and convert to codeType
    }
    return result;
}

// probe()
// Returns the ith term in the probe sequence for code.
size_t probe(codeType code, size_t tbl_size, size_t i) {
    codeType h1 = code & (tbl_size-1); // code % tbl_size
    codeType h2 = 2*(code & (tbl_size/2 - 1)) + 1; // 2*(code % tbl_size/2) + 1
    return (h1 + i*h2) & (tbl_size-1); // (h1 + i*h2) % tbl_size
}

// findSlot()
// Steps through the probe sequence for code=hash(k). If the Dictionary contains
// key k, returns the slot in D->table containing the index of key k in D->data.
// Otherwise, returns the first slot in D->table that contains either the special
// value TableEmpty or the special value TableDeleted.
uint64_t findSlot(Dictionary D, keyType k, codeType code);

// insertIndex()
// Steps through the probe sequence for code and inserts idx into the first
// slot in array T (of length m) at which either the special value TableEmpty
// or the special value TableDeleted are found.
void insertIndex(int64_t* T, size_t m, size_t idx, codeType code);

// expandTable()
// Expands length of array D->table. If tableSize<TableSizeThreshold, increases
// size by TableExpandFactor1, otherwise increases size by TableExpandFactor2.
// Re-inserts indices of D->data into D->table.
void expandTable(Dictionary D);

// expandData()
// Expands length of array D->data, leaving contents unchanged.
void expandData(Dictionary D);

// compactData()
// Removes the deleted elements from array D->data, making its entries contiguious,
// and sets D->dataDensity equal to 1.
void compactData(Dictionary D);

// Constructors-Destructors ---------------------------------------------------

// newDictionary()
// Constructs a new empty Dictionary.
Dictionary newDictionary(void) {
    Dictionary D; 
    D = malloc(sizeof(DictionaryObj));
    assert(D != NULL);

    D->tbl = calloc(TableInitialSize, sizeof(int64_t));
    assert(D->tbl != NULL);

    D->data = calloc(DataInitialSize, sizeof(Element));
    assert(D->data != NULL);

    D->data[0].key = DataEmpty;
    D->data[0].val = 0;
    D->data[0].code = 0;

    D->tblSize = TableInitialSize;
    D->tblLoadFactor = TableEmpty;
    D->numPairs = TableEmpty;
    D->numDelElem = TableEmpty;
    D->dataSize = DataInitialSize;
    D->dataNext = 1;
    D->dataDensity = 1.0;

    return D;

}

// freeDictionary()
// Frees heap memory associated with *pD, and sets *pD to NULL.
void freeDictionary(Dictionary* pD) {
    if (pD != NULL && *pD != NULL) {
        Dictionary D = *pD;
        free(D->tbl);
        free(D->data);
        free(*pD);
        *pD = NULL;
    }
}

// Access functions -----------------------------------------------------------
// size()
// Returns the number of key-value pairs in D.
int size(Dictionary D);

// contains()
// Returns true if D contains a pair with key k, returns false otherwise.
bool contains(Dictionary D, keyType k);

// getValue()
// Returns the value associated with key k.
// pre: contains(D, k)
valType getValue(Dictionary D, keyType k);

// Manipulation procedures ----------------------------------------------------

// clear()
// Sets D to the empty state.
void clear(Dictionary D);

// setValue()
// If D contains a pair with key==k, overwrites the associated value with v,
// otherwise inserts the new pair (k, v) into D.
void setValue(Dictionary D, keyType k, valType v);

// removeKey()
// Deletes the pair with key==k from D.
// pre: contains(D, k)
void removeKey(Dictionary D, keyType k);

// Other operations -----------------------------------------------------------

// copy()
// Returns a new Dictionary containing the same key-value pairs as D.
Dictionary copy(Dictionary D);

// equals()
// Returns true if A and B contain the same key-value pairs, and returns false
// otherwise.
bool equals(Dictionary A, Dictionary B);

// printDictionary()
// Prints a string representation of Dictionary D to the FILE pointer out. Each
// key-value pair is printed on its own line in the form "key : value". Pairs
// will appear in the order in which they were inserted into the Dictionary.
void printDictionary(FILE* out, Dictionary D);

// printDiagnostic()
// Prints a string representation of the internal state of Dictionary D to FILE
// out. First D->data is printed with accompanying parameters, then D->table
// is printed with its accompanying parameters.
void printDiagnostic(FILE* out, Dictionary D);