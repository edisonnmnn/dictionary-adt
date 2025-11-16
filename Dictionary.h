// name: Edison Kuo
// cruzID: edkuo
// assignment: pa5

// Dictionary.h
// Header file for Dictionary ADT

#pragma once

#include <stdio.h>
#include <stdbool.h>

// Exported types -------------------------------------------------------------

typedef const char* keyType; // Points to const data
typedef int valType;

// Dictionary reference type
typedef struct DictionaryObj* Dictionary;

// Constructors-Destructors ---------------------------------------------------

// newDictionary()
// Constructs a new empty Dictionary.
Dictionary newDictionary(void);

// freeDictionary()
// Frees heap memory associated with *pD, and sets *pD to NULL.
void freeDictionary(Dictionary* pD);

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