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