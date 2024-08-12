/*
 * File: project.cpp
 * Project: Parcel Management System with Hash Tables and Binary Search Trees
 * Programmer: Kridhay Makwana-8981833, Ayushpreet-8982295
 * First version: 08/10/2024
 * Description: This program manages parcel data using hash tables and binary search trees,
 *              allowing users to perform various searches and display operations.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>



 // Disable specific compiler warnings
#pragma warning(disable : 4996)


// Define the hash table size
#define hashtableSize 127



// Struct definition for the parcel data
struct Parcel
{
    char* destination; // Destination country name
    int weight;        // Weight of the parcel in grams
    float valuation;   // Valuation of the parcel in dollars

};


// Struct definition for the binary search tree node
struct TreeNode
{
    Parcel parcel;           // Parcel data
    struct TreeNode* left;   // Pointer to the left child
    struct TreeNode* right;  // Pointer to the right child

};


// Struct definition for the hash table
struct HashTable
{
    TreeNode* buckets[hashtableSize]; // Array of pointers to the root nodes of BSTs

};


// Function prototypes
HashTable * initializeHashTable();
void loadParcelsFromFile(HashTable* hashTable, const char* filename);
void showParcelsForCountry(HashTable* hashTable, char* country);
void showTotalLoadAndValuation(HashTable* hashTable, char* country);
void displayParcelInfo(Parcel* parcel);
Parcel* findParcelByWeight(TreeNode* rootNode, int weight);
void releaseHashTableMemory(HashTable* hashTable);
void findMinAndMaxParcel(TreeNode* rootNode, Parcel** minParcel, Parcel** maxParcel);
void findLightestAndHeaviestParcel(TreeNode* rootNode, Parcel** lightestParcel, Parcel** heaviestParcel);
void calculateTotal(TreeNode* currentNode, int* totalWeight, float* totalValuation);
unsigned long hashFunction(char* str);
TreeNode* createNewTreeNode(Parcel parcel);
TreeNode* insertTreeNode(TreeNode* rootNode, Parcel parcel);
void addParcelToHashTable(HashTable* hashTable, Parcel parcel);
void traverseInOrder(TreeNode* rootNode);
void releaseTreeMemory(TreeNode* rootNode);

// Allocates memory for the hash table and initializes all bucket pointers to NULL */
HashTable* initializeHashTable()
{

    HashTable* hashTable = (HashTable*)malloc(sizeof(HashTable));

    if (!hashTable){

        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    
    }
    for (int i = 0; i < hashtableSize; i++) {

        hashTable->buckets[i] = NULL;
    
    }
    
    return hashTable;

}



/* Loads parcel data from a file and inserts it into the hash table */
void loadParcelsFromFile(HashTable* hashTable, const char* filename)
{

    FILE* file = fopen(filename, "r");
    if (!file)
    {
        fprintf(stderr, "Failed to open file %s\n", filename);

        exit(1);
    }

    char destination[100];
    int weight;
    float valuation;

    while (fscanf(file, "%[^,], %d, %f\n", destination, &weight, &valuation) == 3)
    {
        Parcel parcel;

        parcel.destination = strdup(destination);
        parcel.weight = weight;

        parcel.valuation = valuation;
        addParcelToHashTable(hashTable, parcel);
    }
    fclose(file);
}


// Displays all parcels for a given country
void showParcelsForCountry(HashTable* hashTable, char* country)
{
    unsigned long index = hashFunction(country);
    TreeNode* rootNode = hashTable->buckets[index];

    if (rootNode == NULL)
    {
        printf("No parcels found for %s.\n", country);
        return;
    }
    printf("Parcels for %s:\n", country);
    traverseInOrder(rootNode);
}

// Displays the total load and valuation for a given country
void showTotalLoadAndValuation(HashTable* hashTable, char* country)
{
    unsigned long index = hashFunction(country);
    TreeNode* rootNode = hashTable->buckets[index];

    if (rootNode == NULL)
    {
        printf("No parcels found for %s.\n", country);
        return;
    }
    int totalWeight = 0;
    float totalValuation = 0.0;

    calculateTotal(rootNode, &totalWeight, &totalValuation);

    printf("Total load for %s: %d grams\n", country, totalWeight);
    printf("Total valuation for %s: $%.2f\n", country, totalValuation);

}

// Displays the details of a parcel
void displayParcelInfo(Parcel* parcel)
{
    if (parcel)
    {
        printf("Destination: %s, Weight: %d grams, Valuation: $%.2f\n", parcel->destination, parcel->weight, parcel->valuation);
    }
    else
    {
        printf("Parcel not found.\n");
    }
}


// Searches for a parcel by weight in a binary search tree
Parcel* findParcelByWeight(TreeNode * rootNode, int weight)
{
    if (rootNode == NULL) return NULL;
    if (rootNode->parcel.weight == weight) return &(rootNode->parcel);

    if (weight < rootNode->parcel.weight) return findParcelByWeight(rootNode->left, weight);

    return findParcelByWeight(rootNode->right, weight);

}

// Frees the memory allocated for a hash table
void releaseHashTableMemory(HashTable* hashTable)
{
    for (int i = 0; i < hashtableSize; i++)
    {
        releaseTreeMemory(hashTable->buckets[i]);
    }
    free(hashTable);
}

// Finds the minimum and maximum valuation parcels in a binary search tree
void findMinAndMaxParcel(TreeNode* rootNode, Parcel** minParcel, Parcel** maxParcel)
{
    if (rootNode == NULL) return;

    if (*minParcel == NULL || rootNode->parcel.valuation < (*minParcel)->valuation)
    {
        *minParcel = &(rootNode->parcel);
    }
    if (*maxParcel == NULL || rootNode->parcel.valuation > (*maxParcel)->valuation)
    {
        *maxParcel = &(rootNode->parcel);
    }

    findMinAndMaxParcel(rootNode->left, minParcel, maxParcel);
    findMinAndMaxParcel(rootNode->right, minParcel, maxParcel);

}

// Finds the lightest and heaviest parcels in a binary search tree
void findLightestAndHeaviestParcel(TreeNode* rootNode, Parcel** lightestParcel, Parcel** heaviestParcel)
{
    if (rootNode == NULL) return;

    if (*lightestParcel == NULL || rootNode->parcel.weight < (*lightestParcel)->weight)
    {
        *lightestParcel = &(rootNode->parcel);
    }
    if (*heaviestParcel == NULL || rootNode->parcel.weight > (*heaviestParcel)->weight)
    {
        *heaviestParcel = &(rootNode->parcel);
    }

    findLightestAndHeaviestParcel(rootNode->left, lightestParcel, heaviestParcel);
    findLightestAndHeaviestParcel(rootNode->right, lightestParcel, heaviestParcel);
}

// Helper function to calculate total weight and valuation in a binary search tree
void calculateTotal(TreeNode* currentNode, int* totalWeight, float* totalValuation)
{
    if (currentNode != NULL)
    {
        *totalWeight += currentNode->parcel.weight;
        *totalValuation += currentNode->parcel.valuation;

        calculateTotal(currentNode->left, totalWeight, totalValuation);
        calculateTotal(currentNode->right, totalWeight, totalValuation);
    }
}

// Hash function: Generates a hash value for a given string using the djb2 algorithm
unsigned long hashFunction(char* str)
{
    unsigned long hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; // hash * 33 + c

    return hash % hashtableSize;
}

// Creates and initializes a new tree node with the given parcel data
TreeNode* createNewTreeNode(Parcel parcel)
{
    TreeNode* newNode = (TreeNode*)malloc(sizeof(TreeNode));

    if (!newNode)
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    newNode->parcel = parcel;
    newNode->left = NULL;
    newNode->right = NULL;

    return newNode;

}

// Inserts a new node with the given parcel data into a binary search tree
TreeNode* insertTreeNode(TreeNode* rootNode, Parcel parcel)
{
    if (rootNode == NULL)
    {
        return createNewTreeNode(parcel);
    }
    if (parcel.weight < rootNode->parcel.weight)
    {
        rootNode->left = insertTreeNode(rootNode->left, parcel);
    }
    else
    {
        rootNode->right = insertTreeNode(rootNode->right, parcel);
    }
    return rootNode;
}

// Inserts a parcel into the hash table
void addParcelToHashTable(HashTable* hashTable, Parcel parcel)
{
    unsigned long index = hashFunction(parcel.destination);
    hashTable->buckets[index] = insertTreeNode(hashTable->buckets[index], parcel);
}

// Performs an in-order traversal of a binary search tree and prints parcel details
void traverseInOrder(TreeNode* rootNode)
{
    if (rootNode != NULL)
    {
        traverseInOrder(rootNode->left);
        printf("Destination: %s, Weight: %d grams, Valuation: $%.2f\n", rootNode->parcel.destination, rootNode->parcel.weight, rootNode->parcel.valuation);
        traverseInOrder(rootNode->right);
    }
}

// Frees the memory allocated for a binary search tree
void releaseTreeMemory(TreeNode* rootNode)
{
    if (rootNode != NULL)
    {
        releaseTreeMemory(rootNode->left);
        releaseTreeMemory(rootNode->right);

        free(rootNode->parcel.destination);
        free(rootNode);
    }
}

// Main function: handles user interaction
int main()
{
    HashTable* parcelHashTable = initializeHashTable();

    loadParcelsFromFile(parcelHashTable, "couries.txt");

    int choice;
    char inputCountry[100];
    int inputWeight;

    while (1)
    {
       printf("\nMenu List:\n");
       printf("1. Enter country name and display all parcels\n");
        printf("2. Enter country and weight pair\n");
       printf("3. Display total parcel load and valuation for the country\n");
        printf("4. Display cheapest and most expensive parcels for the country\n");
       printf("5. Display lightest and heaviest parcels for the country\n");
       printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        if (choice == 1)
        {
            printf("Enter country name: ");
            scanf("%99s", inputCountry);

            showParcelsForCountry(parcelHashTable, inputCountry);
        }

        else if (choice == 2)
        {
            printf("Enter country name: ");
            scanf("%99s", inputCountry);

            printf("Enter weight: ");

            scanf("%d", &inputWeight);
            displayParcelInfo(findParcelByWeight(parcelHashTable->buckets[hashFunction(inputCountry)], inputWeight));
        }
        else if (choice == 3)
        {
            printf("Enter country name: ");
            scanf("%99s", inputCountry);

            showTotalLoadAndValuation(parcelHashTable, inputCountry);

        }
        else if (choice == 4)
        {
            printf("Enter country name: ");
            scanf("%99s", inputCountry);
            Parcel* minParcel = NULL;

            Parcel* maxParcel = NULL;
            findMinAndMaxParcel(parcelHashTable->buckets[hashFunction(inputCountry)], &minParcel, &maxParcel);
            printf("Cheapest parcel for %s:\n", inputCountry);
            displayParcelInfo(minParcel);

            printf("Most expensive parcel for %s:\n", inputCountry);
            displayParcelInfo(maxParcel);

        }

        else if (choice == 5)
        {
            printf("Enter country name: ");

            scanf("%99s", inputCountry);
            Parcel* lightestParcel = NULL;
            Parcel* heaviestParcel = NULL;

            findLightestAndHeaviestParcel(parcelHashTable->buckets[hashFunction(inputCountry)], &lightestParcel, &heaviestParcel);
            printf("Lightest parcel for %s:\n", inputCountry);
            displayParcelInfo(lightestParcel);

            printf("Heaviest parcel for %s:\n", inputCountry);
            displayParcelInfo(heaviestParcel);
        }

        else if (choice == 6)
        {
            printf("\nExiting program..... \n\nThank you for choosing Kridhay Makwana's and Ayushpreet's Program\n");

            releaseHashTableMemory(parcelHashTable);

            break;
        }
        else
        {
            printf("Invalid choice. Please try again.\n");
        }
    }


    return 0;
}