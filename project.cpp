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