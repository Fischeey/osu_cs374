#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h> // for EXIT_SUCCESS and EXIT_FAILURE
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <time.h> 

/*
* Function: processMovieFile
* Opens a file, reads and prints each line
* filePath: path to the file
*
* This function shows sample code that opens a file, then in a loop reads and
prints each line in that file.
* You can use this code however you want in your Prog Assignment 2.
*/

struct movie
{
    char *title;
    int year;
    char **languageArr;
    double rating;
    int arrcount;
    struct movie *next;
};
struct movie *createMovie(char *title, int year, char *inplanguageArr, double rating)
{
    struct movie *currMovie = malloc(sizeof(struct movie));
    currMovie->title = calloc(strlen(title) + 1, sizeof(char));
    strcpy(currMovie->title, title);

    currMovie->year = year;

    char *start = strchr(inplanguageArr, '[');
    char *end = strchr(inplanguageArr, ']');

    int element_count = 0;
    // Move past the '[' and replace ']' with a null terminator to isolate the inner content
    char *inner_content = start + 1;
    *end = '\0'; // inner_content is now "asd;afd;bd"

    // Initialize your struct's array pointer to NULL so realloc works correctly
    currMovie->languageArr = NULL;

    // Tokenize using strtok_r
    char *saveptr;
    const char *delim = ";";

    char *token = strtok_r(inner_content, delim, &saveptr);

    while (token != NULL)
    {
        // 1. Grow the pointer array dynamically to hold the new token + room for a trailing NULL
        char **temp = realloc(currMovie->languageArr, (element_count + 2) * sizeof(char *));
        if (temp == NULL)
        {
            // Handle allocation failure (out of memory)
            break;
        }
        currMovie->languageArr = temp;

        // 2. Deep-copy the extracted token into the array
        currMovie->languageArr[element_count] = strdup(token);

        element_count++;

        // 3. Keep the array NULL-terminated on every iteration
        currMovie->languageArr[element_count] = NULL;

        // Get the next token
        token = strtok_r(NULL, delim, &saveptr);
    }
    currMovie->arrcount = element_count;

    currMovie->rating = rating;

    currMovie->next = NULL;
    return currMovie;
}
void printMovieList(struct movie *list)
{
    while (list != NULL)
    {
        // 1. Print the starting details
        printf("%s %d ", list->title, list->year);

        // 2. Loop through the char ** array until reaching the trailing NULL pointer
        // This automatically stops exactly when the languages end, preventing the seg fault!
        for (int i = 0; list->languageArr != NULL && list->languageArr[i] != NULL; i++)
        {
            printf("%s ", list->languageArr[i]);
        }

        // 3. Print the trailing details
        printf("%.1f\n", list->rating); // Configured to 1 decimal place or use %f

        list = list->next;
    }
}

/**
 * Free the memory for one struct
 */
void freeMovie(struct movie *curr)
{
    free(curr->title);
    free(curr->languageArr);
    free(curr);
}

/*
 * Free the memory allocated for elements in the list
 */
void freeMovieList(struct movie *list)
{
    struct movie *curr;
    while (list != NULL)
    {
        curr = list;
        list = list->next;
        freeMovie(curr);
    }
}

void processFile(char fileName[257], int size)
{
    int file_descriptor = open(fileName, 'r');

    if (file_descriptor == -1)
    {
        printf("open() failed on \"%s\"\n", fileName);
        perror("Error");
        exit(1);
    }
    int rand_num = (rand() % (100000));
    char numarr[8];
    char pathname[24] = "./fischej5.movies.";
    snprintf(numarr, sizeof(numarr), "%d", rand_num);
    strcat(pathname, numarr);
    printf("Created directory with name %s\n", pathname);
    strcat(pathname, "/");
    int resDir = mkdir(pathname, 0750);
    FILE *movieFile = fopen(fileName, "r");
    char *currLine = NULL;
    size_t len = 0;
    // Open the specified file for reading only
    // Read the file line by line

    struct movie *head = NULL;
    struct movie *tail = NULL;
    struct movie *newMovie = NULL;
    char *saveptr;
    int count = 0;

    getline(&currLine, &len, movieFile);
    while (getline(&currLine, &len, movieFile) != -1)
    {

        const char *delim = ",";
        char *token = strtok_r(currLine, delim, &saveptr);
        char *title = strdup(token);
        token = strtok_r(NULL, delim, &saveptr);
        int year = atoi(token);

        token = strtok_r(NULL, delim, &saveptr);
        char *languageArr = token;
        token = strtok_r(NULL, delim, &saveptr);

        double rating = strtod(token, NULL);
        newMovie = createMovie(title, year, languageArr, rating);

        // Is this the first element in the linked list?
        if (head == NULL)
        {
            // This is the first element in the linked link
            // Set the head and the tail to this element
            head = newMovie;
            tail = newMovie;
        }
        else
        {
            // This is not the first element.
            // Add this element to the list and advance the tail
            tail->next = newMovie;
            tail = newMovie;
        }

        count++;
    }



    int found = 0;
    struct movie *curPos = head;
    while (curPos != NULL)
    {
        char tempPathName[513] = "";
        strcpy(tempPathName, pathname);
        char yearStr[8] = "";
        snprintf(yearStr, sizeof(yearStr), "%d", curPos->year);
        strcat(tempPathName, yearStr);
        strcat(tempPathName, ".txt");
        int yearFile = open(tempPathName, O_WRONLY | O_CREAT | O_APPEND, 0640);

        
        char tempTitle[128] = "";
        strcat(tempTitle, curPos->title);
        strcat(tempTitle, "\n");
        ssize_t bytes_written = write(yearFile, tempTitle, strlen(tempTitle));
        if (bytes_written < 0) {
            perror("Error writing to file");
            close(yearFile);
            return;
        }
        close(yearFile);
        curPos = curPos->next;
    }

    
    close(file_descriptor);
}

/**
















 *
 */
int main(int argc, char **argv)
{
    srand(time(NULL));
    DIR *dir = opendir(".");

    struct dirent *entry;
    struct stat dirStat;

    int exit = 0;
    while (exit == 0)
    {
        printf("1. Select file to process\n"
               "2. Exit the program\n"
               "Enter a choice 1 or 2:\n");

        int respO;
        scanf("%d", &respO);

        if (respO == 1)
        {
            int exitI = 0;
            while (exitI == 0)
            {
                printf("Which file you want to process?\n"
                       "Enter 1 to pick the largest file\n"
                       "Enter 2 to pick the smallest file\n"
                       "Enter 3 to specify the name of a file\n"
                       "Enter a choice from 1 to 3:\n");

                int respI;
                scanf("%d", &respI);

                if (respI == 1)
                {
                    char largestFileName[257];
                    int largestFileSize = 0;
                    while ((entry = readdir(dir)) != NULL)
                    {
                        if (strncmp(entry->d_name, "movies_", 7) == 0)
                        {
                            stat(entry->d_name, &dirStat);
                            if (dirStat.st_size > largestFileSize)
                            {
                                largestFileSize = dirStat.st_size;

                                strcpy(largestFileName, entry->d_name);
                            }
                        }
                    }
                    if (largestFileSize == 0)
                    {
                        printf("error123\n");
                    }
                    else
                    {
                        printf("Now processing the chosen file named %s\n", largestFileName);
                        processFile(largestFileName, largestFileSize);
                    }
                    exitI = 1;
                }
                else if (respI == 2)
                {
                    char smallestFileName[257];
                    int smallestFileSize = 10000000;
                    while ((entry = readdir(dir)) != NULL)
                    {
                        if (strncmp(entry->d_name, "movies_", 7) == 0)
                        {
                            stat(entry->d_name, &dirStat);
                            if (dirStat.st_size < smallestFileSize && dirStat.st_size > 0)
                            {
                                smallestFileSize = dirStat.st_size;
                                strcpy(smallestFileName, entry->d_name);
                            }
                        }
                    }
                    if (smallestFileSize == 10000000)
                    {
                        printf("error123\n");
                    }
                    else
                    {
                        printf("Now processing the chosen file named %s\n", smallestFileName);
                        processFile(smallestFileName, smallestFileSize);
                    }
                    exitI = 1;
                }
                else if (respI == 3)
                {
                    char fileName[257];
                    int size = 0;
                    printf("Enter the complete file name:\n");
                    scanf("%s", fileName);

                    while ((entry = readdir(dir)) != NULL)
                    {
                        stat(entry->d_name, &dirStat);
                        if (strcmp(entry->d_name, fileName) == 0)
                        {
                            size = dirStat.st_size;
                        }
                    }
                    if (size != 0)
                    {
                        printf("Now processing the chosen file named %s\n", fileName);
                        processFile(fileName, size);
                        exitI = 1;
                    }
                    else
                    {
                        printf("erro5324\n");
                    }
                }
                else
                {
                    printf("You entered an incorrect choice. Try again.");
                }
            }
        }
        else if (respO == 2)
        {
            exit = 1;
        }
        else
        {
            printf("You entered an incorrect choice. Try again.");
        }
    }

    return EXIT_SUCCESS;
}
