#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MY_ID 469420
#define WORD_LEN 40  //size of wordStat.name even with the null operator
#define STATS_CAPACITY 8 //starting sizze of the stats array
#define SOURCE_AUTHOR "Satoshi Nakamoto"
#define SOURCE_TITLE "Bitcoin: A Peer-to-Peer Electronic Cash System"


//sample paragraph. I have included the full pdf in the repo
//I had to put each line in quotes and add a space at the end of each like. For some reason, the words will join without the space
const char *paragraph = "A purely peer-to-peer version of electronic cash would allow online "
"payments to be sent directly from one party to another without going through a "
"financial institution. Digital signatures provide part of the solution, but the main "
"benefits are lost if a trusted third party is still required to prevent double-spending. "
"We propose a solution to the double-spending problem using a peer-to-peer network. "
"The network timestamps transactions by hashing them into an ongoing chain of "
"hash-based proof-of-work, forming a record that cannot be changed without redoing "
"the proof-of-work. The longest chain not only serves as proof of the sequence of "
"events witnessed, but proof that it came from the largest pool of CPU power. As "
"long as a majority of CPU power is controlled by nodes that are not cooperating to "
"attack the network, they'll generate the longest chain and outpace attackers. The "
"network itself requires minimal structure. Messages are broadcast on a best effort "
"basis, and nodes can leave and rejoin the network at will, accepting the longest "
"proof-of-work chain as proof of what happened while they were gone.";

typedef struct {
    char name[WORD_LEN];
    int count;
}wordStat;

//structure of the analyzer
typedef struct {
    wordStat *stats;
    char *text;
    int text_len;
    int distinct;
    int stats_capacity;
    int total_words;
    char source_title[100];
}Analyzer;


//my function prototypes
void count_words(Analyzer *);
void longest_word(Analyzer *);
void most_frequent(Analyzer *);
void display_word_statistics(Analyzer *);
void search_word(Analyzer *);
void generate_text_summary(Analyzer *);
int load_text(Analyzer *);
int analyze_text(Analyzer *);
int find_word(Analyzer *, const char *);
void custom_analysis(Analyzer *);
int cmpr_name(const void *x, const void *y);
int cmpr_count(const void *x, const void *y);
void sort_word_statistics(Analyzer *a);

//this function loads the paragraph into the buffer
int load_text(Analyzer *a)
{
    int length = strlen(paragraph);
    a->text = malloc(length + 1); //added 1 for the null operator \0
    if (a->text == NULL)
    {
        printf("Failed to allocate memory!\n");
        return (0);
    }
    strcpy(a->text, paragraph);
    a->text_len = length;
    strcpy(a->source_title, SOURCE_TITLE);

    return (1);     
}


//main engine. this function looks through the paragraph, extract every word and update the stats
int analyze_text(Analyzer *a)
{
    char *p = a->text; // this is the pointer that walks through the paragraph
    a->total_words = 0;
    

    a->stats = malloc(STATS_CAPACITY * sizeof(wordStat));
    if (a->stats == NULL)
    {
        printf("Failed to allocate memory\n");
        return(0);
    }
    a->stats_capacity = STATS_CAPACITY;
    a->distinct = 0;
    a->total_words = 0;

    while (*p != '\0') 
    {
        //skip every character that is not a letter or the \0
        while (*p != '\0' && !isalpha((unsigned char)*p)) 
        {
            p++;
        }
        if (*p == '\0')
            break;
        char word[WORD_LEN];
        int len = 0;
        //copy the characters. apostrophe can be accepted if another letter follows it
        while (isalpha(*p) || (*p == '\'' && isalpha(*(p + 1))))
        {
            if (len < WORD_LEN - 1)
            {
                word[len] = (char)tolower((unsigned char)*p);
                len++;
            }
            p++;
        }
        word[len] = '\0';
        
        int found = 0;
        for (int i =0; i < a->distinct; i++)
        {
            if (strcmp(a->stats[i].name, word) == 0)
            {
                a->stats[i].count++;
                found = 1;
                break;
            }
        }
        if (!found)
        {
            if (a->distinct == a->stats_capacity) //then that means the capacity is full.
            {
                int new_size = a->stats_capacity * 2; // double the existing one
                wordStat *new_mem = realloc(a->stats, new_size * sizeof(wordStat));
                if (new_mem == NULL)
                {
                    printf("Failed to allocate memory\n");
                    return (0);
                }
                a->stats = new_mem;
                a->stats_capacity = new_size;
            }
            strcpy(a->stats[a->distinct].name, word);
            a->stats[a->distinct].count = 1;
            a->distinct++;
        }
        //count every word, whether new or repeat
        a->total_words+= 1;
    }
    return (1);
}

int main()
{
    printf("===============================================================\n");
    printf("Student ID: %d\n", MY_ID);
    printf("Project: Dynamic Text Mining and Linguistic Pattern Analyzer\n");
    printf("Source: %s  Author: %s\n", SOURCE_TITLE, SOURCE_AUTHOR); 
    printf("Custom Analysis: Lexical Diversity Score\n");
    printf("===============================================================\n");
    printf("\n");

    Analyzer a = {0};  //set every field to 0
    if (!load_text(&a))
        return (1);
    if (!analyze_text(&a))
    {
        free(a.text);
        free(a.stats);
        return (1);
    }

    // printf("Length: %d\n", a.text_len);
    // printf("\n");
    // printf("%s\n", a.text);
    //print_words(&a);

    //function pointer array
    void (*funxs[8])(Analyzer *) = {count_words, longest_word, most_frequent, 
        display_word_statistics, sort_word_statistics, search_word, generate_text_summary, custom_analysis};

    while (1)
    {
        printf("\n1.Count words\n2.Longest word\n3.Most frequent word\n4.Display word statistics\n"
            "5.Sort word statistics\n6.Search for a word\n7.Text summary\n8.Custom analysis\n9.Exit\n");
        char line[16];
        int choice = 0;

        printf("Enter your choice: ");
        if (fgets(line, sizeof(line), stdin) == NULL)
            break;
        if (sscanf(line, "%d", &choice) != 1 || choice < 1 || choice > 9)
        {
            printf("Enter a number between 1 and 9\n");
            continue;
        }
        if (choice == 9)
            break;

        //choice -1 matches the menu number to the index
        funxs[choice - 1](&a);
    }

    free(a.text);
    free(a.stats);
    return (0);
}


//total words colllected, including repeated ones
void count_words(Analyzer *a)
{
    if (a->distinct == 0)
    {
        printf("No words have been processed\n");
        return;
    }
    printf("Total words: %d\n", a->total_words);
    printf("\n");
}

//this function finds the longest word by comparing lengths of all words
void longest_word(Analyzer *a)
{
    if (a->distinct == 0)
    {
        printf("No words have been processed\n");
        return;
    }
    wordStat *longest = a->stats;
    wordStat *w;
    wordStat *end = a->stats + a->distinct;

    for (w = a->stats; w < end; w++)
    {
        if (strlen(w->name) > strlen(longest->name))
        {
            longest = w;
        }
    }
    printf("The longest word is %s which has %zu letters\n", longest->name, strlen(longest->name));
    printf("\n");
}


//word with the highest count
void most_frequent(Analyzer *a)
{
    if (a->distinct == 0)
    {
        printf("No words have been processed\n");
        return;
    }
    wordStat *frequent = a->stats;
    wordStat *w;
    wordStat *end = a->stats + a->distinct;

    for (w = a->stats; w < end; w++)
    {
        if (w->count > frequent->count)
            frequent = w;
    }
    printf("most frequent word is %s, which appeared %d times\n", frequent->name, frequent->count);
    printf("\n");
}


//print every distinct word with its count
void display_word_statistics(Analyzer *a)
{
    if (a->distinct == 0)
    {
        printf("No words have been processed\n");
        return;
    }

    wordStat *w;
    wordStat *end = a->stats + a->distinct;

    printf("%-20s %5s\n", "Word", "Count");
    printf("--------------------------\n");

    for (w = a->stats; w < end; w++)
    {
        printf("%-20s %5d\n", w->name, w->count);
    }

    printf("--------------------------\n");
    printf("Distinct words: %d, total words: %d\n", a->distinct, a->total_words);
    printf("\n");
}

// return the index of word in the stats array, or return -1 if not found
int find_word(Analyzer *a, const char *word)
{
    for (int i = 0; i < a->distinct; i++)
    {
        if (strcmp(a->stats[i].name, word) == 0)
            return (i);
    }
    return (-1);
}

// search with user input but convert to lower case first
void search_word(Analyzer *a)
{
    if (a->distinct == 0)
    {
        printf("No words have been processed\n");
        return;
    }

    char input[WORD_LEN];
    printf("Enter a word to search: ");
    if (fgets(input, WORD_LEN, stdin) == NULL)
        return;
    input[strcspn(input, "\n")] = '\0';  //remove new line

    char *item = input;
    while (*item != '\0')
    {
        *item = (char)tolower((unsigned char)*item);
        item++;
    }

    int index = find_word(a, input);
    if (index == -1)
    {
        printf(" There's no %s in the text\n", input);
        return;
    }
    printf("The word %s appears %d times\n", a->stats[index].name, a->stats[index].count);
    printf("\n");
}

//Print all the required stats, then the memory inspection.
void generate_text_summary(Analyzer *a)
{
    if (a->distinct == 0)
    {
        printf("No words have been processed\n");
        return;
    }

    int letters = 0;
    wordStat *w;
    wordStat *end = a->stats + a->distinct;

    for (w = a->stats; w < end; w++)
    {
        letters += (int)strlen(w->name) * w->count;
    }

    printf("\n============ Text Summary  ==========\n");
    count_words(a);
    printf("Distinct words: %d\n", a->distinct);
    printf("Average word length: %.2f letters\n", (float)letters / a->total_words);
    longest_word(a);
    most_frequent(a);
    custom_analysis(a);
    printf("======================================\n");
    printf("\n");

    //%p prints an address. (void *) is used for casting here.
    printf("Memory Inspection\n");
    printf("--------------------------\n");
    printf("Text buffer address: %p\n", (void *)a->text);
    printf("Word stats array: %p\n", (void *)a->stats);
    printf("Selected word-stat entry:  %p\n", (void *)&a->stats[a->distinct - 1]);
    printf("\n");
}

/*
 * Lexical Diversity Score
 * Purpose: measures how varied the vocabulary of the text is.
 * Formula: distinct words / total words
 * Interpretation: close to 1 means almost every word is different,
 *                 close to 0 means the same words are repeated a lot.
 *                 0.7 and above: varied, 0.4 to 0.7: moderate, below 0.4: repetitive.
 */

 //Lexical Diversity Score
 //this function measures how varied the vocabulary of the text is
 //the algorithm is :  distinct words / total words
 //if the result is close to 1, it means almost every word is different
 // if the result is close to 0 means the same words are repeated a lot
 //i0.7 and above: varied,   0.4 to 0.7 is moderate, below 0.4 is repetitive
void custom_analysis(Analyzer *a)  
{
    if (a->total_words == 0)
    {
        printf("No words have been processed\n");
        return;
    }

    //Cast to float first, to avoid integer division problems
    float score = (float)a->distinct / a->total_words;

    printf("%d distinct out of %d total words\n");
    printf("Score: %.2f \n", score);

    if (score >= 0.7)
        printf("Results: varied\n");
    else if (score >= 0.4)
        printf("Results: moderate\n");
    else
        printf("Results: repetitive\n");

    printf("\n");
}


//functions for qsort
int cmpr_name(const void *x, const void *y)
{
    const wordStat *first = x;
    const wordStat *second = y;
    return strcmp(first->name, second->name);
}

int cmpr_count(const void *x, const void *y)
{
    const wordStat *first = x;
    const wordStat *second = y;
    if (first->count != second->count)
        return second->count - first->count;
    return strcmp(first->name, second->name);
}

//sort stats array alphabetically or by frequesncy
void sort_word_statistics(Analyzer *a)
{
    if (a->distinct == 0)
    {
        printf("No words have been processed\n");
        return;
    }

    char line[16];
    int choice = 0;

    printf("Sort by 1.Alphabetic, 2.Frequency: ");
    if (fgets(line, sizeof(line), stdin) == NULL)
        return;
    sscanf(line, "%d", &choice);

    if (choice == 1)
    {
        qsort(a->stats, a->distinct, sizeof(wordStat), cmpr_name);
    }
    else if (choice == 2)
    {
        qsort(a->stats, a->distinct, sizeof(wordStat), cmpr_count);
    }
    else
    {
        printf("Wrong chioce\n");
        return;
    }
    display_word_statistics(a);
}
