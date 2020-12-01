// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "dictionary.h"
#include <string.h>
#include <limits.h>

// Represents number of buckets in a hash table
#define N 729

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1]; //is this + 1 for the /0 that is at the end?
    int misspell; //0 if correct 1 if misspelled
    struct node *next;
}
node;

// Represents a hash table
node* hashtable[N];
node* temp[N];
node* uniquewordarray[N];
node* temp2[N];
unsigned int wordcount = 0;

// Hashes word to a number between 0 and 25, inclusive, based on its first letter
unsigned int hash(const char *word)
{
    if ((word[0] < 65 || word[0] > 122 || (word[0] > 90 && word[0] < 97)))
    {
        return 702;
    }
    else if ((word[1] < 65 || word[1] > 122 || (word[1] > 90 && word[1] < 97)))
    {
        return ((tolower(word[0]) - 'a') + 703);
    }
    else if (strlen(word) >= 2)
    {
        return (tolower(word[0]) - 'a')*26 + (tolower(word[1]) - 'a'); //are we assuming that only the first letter can be capitalized? or are we just using the first letter for hashing, so that's why we do this?
    }
    else
    {
        return ((tolower(word[0]) - 'a') + 676);
    }
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Initialize hash table
    for (int i = 0; i < N; i++) //N is 676
    {
        //hashtable[i] = uniquewordarray[i] = NULL;
        hashtable[i] = malloc(sizeof(node));
        uniquewordarray[i] = malloc(sizeof(node));
        hashtable[i]->next = uniquewordarray[i]->next = NULL;
        temp[i] = hashtable[i];
        temp2[i] = uniquewordarray[i];
    } //check address?


    // Open dictionary
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        unload();
        return false;
    }

    // Buffer for a word
    char word[LENGTH + 1];


    //Preparing the linked list by making the head node and setting an address to the pointer
    //for (int i)

    // Insert words into hash table
    while (fscanf(file, "%s", word) != EOF) //should i shorten the size of the word?
    {
        int i = hash(word);
        strcpy(temp[i] -> word, word); //how is temp[i] -> word a pointer?
        temp[i] -> next = malloc(sizeof(node));
        temp[i] = temp[i] -> next;
        wordcount++;
    }

    //When we reach end of file, make all the last node* = NULL.
    for (int i = 0; i < N; i++)
    {
        temp[i] -> next = NULL; //new
    }

    // Close dictionary
    fclose(file);

    // Indicate success
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return wordcount; //we already initialized this to 0 before "load" so if not loaded we see 0!
}

int inarray(char *word)
{
    int tempu;
    int wordhashu = hash(word);
    while (temp2[wordhashu]->next != NULL)
    {
        if (strcmp(word, temp2[wordhashu]->word) == 0)
        {
            tempu = temp2[wordhashu]->misspell;
            temp2[wordhashu] = uniquewordarray[wordhashu];
            return (tempu);
        }

        temp2[wordhashu] = temp2[wordhashu]->next;
    }
    temp2[wordhashu] = uniquewordarray[wordhashu];
    return (-1);
}

void insertinarray(char *string, int misspell)
{
    int wordhashu = hash(string);
    while (temp2[wordhashu]->next != NULL)
        temp2[wordhashu] = temp2[wordhashu]->next;
    int i;
    for (i = 0; i < strlen(string); i++)
    {
        temp2[wordhashu]->word[i] = string[i];
    }
    temp2[wordhashu]->word[i] = '\0';
    temp2[wordhashu]->misspell = misspell;
    temp2[wordhashu]->next = malloc(sizeof(node));
    temp2[wordhashu]->next->next = NULL;
    temp2[wordhashu] = uniquewordarray[wordhashu];
}
// Returns true if word is in dictionary else false
bool check(const char* word) //does this count as the definition of word throughout the whole function?
{
    char aword[LENGTH + 1]; //why not do this after you know strlen?
     //have same memory address!
    int k = hash(word);
    int strlength = 0;
    //Counting the number of letters in each word
    for (int m = 0; word[m] != '\0'; m++)
    {
        strlength++;
    }
    //Insert word into aword
    for (int n = 0, y = strlength; n < y; n++)
    {
        aword[n] = word[n];
    }

    aword[strlength] = '\0';
    //Making each letter lowercase
    for (int n = 0, y = strlength; n < y; n++)
    {
        if(aword[n] >= 65 && aword[n] <= 90)
        {
            aword[n] = tolower(aword[n]);
        }
    }
    if (inarray(aword) == 0)
        return true;
    else if (inarray(aword) == 1)
        return false;
    else
    {
        for (node* temp1 = hashtable[k]; temp1 -> next != NULL; temp1 = temp1 -> next) //this bothers me, I am not really even looking at the last word!
        {

            char* dictionaryword = temp1 -> word; //the node has the word, not the pointer!
            if (strcmp(dictionaryword, aword) == 0)
            {
                insertinarray(aword,0);
                return true;
            }
        }
        insertinarray(aword,1);
        return false; //if it already hasn't returned true, then it returns false!
    }
}


// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    for (int l = 0; l < N; l++)
    {
        //node* temp2 = hashtable[l];
        temp[l] = hashtable[l];
        //node* temp3 = temp[l];
        while(hashtable[l]) //as to not affect the actual linkedlist we would be iterating on
        {
            temp[l] = temp[l] -> next;
            free(hashtable[l]);
            hashtable[l] = temp[l];
        }
        temp2[l] = uniquewordarray[l];
        while (uniquewordarray[l])
        {
            temp2[l] = temp2[l]->next;
            free(uniquewordarray[l]);
            uniquewordarray[l] = temp2[l];
        }
    }
    return true;
}
