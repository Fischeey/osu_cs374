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
*
* all the code in this movie structure setup is from assignment 2, i know that some of it is probably unnessessary but its not breaking anything to leave it so im leaving it
*
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
void freeMovie(struct movie *m) {
    if (m == NULL) return;



    // i had to google this, i could not figure out for the life of me what the issue was that was causing me a memory leak 
    // its late and ive been working on this for a while I just dont care ngl
    if (m->languageArr != NULL) {
        for (int i = 0; i < m->arrcount; i++) {
            if (m->languageArr[i] != NULL) {
                free(m->languageArr[i]); // <--- Releases the createMovie strdup allocations
                m->languageArr[i] = NULL;
            }
        }
        free(m->languageArr);
        m->languageArr = NULL;
    }

    free(m);
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




















/*

this is partly new and partly old, mostly the while loop in the middle is from assignmnet 2
*/


void processFile(char fileName[257], int size)
{
    int rand_num = (rand() % 100000);

    char pathname[512];
    snprintf(pathname, sizeof(pathname), "./fischej5.movies.%d", rand_num);
    printf("Created directory with name %s\n\n", pathname);

    int resDir = mkdir(pathname, 0750);
    if (resDir == -1)
    {
        perror("Error creating directory");
        // Handle error or exit if necessary
    }

    // Append the trailing slash for the subsequent file operations
    strcat(pathname, "/");

    FILE *movieFile = fopen(fileName, "r");
    if (movieFile == NULL)
    {
        perror("Error opening source file");
        return;
    }

    char *currLine = NULL;
    size_t len = 0;

    struct movie *head = NULL;
    struct movie *tail = NULL;
    struct movie *newMovie = NULL;
    char *saveptr;
    int count = 0;

    // Skip header line
    getline(&currLine, &len, movieFile);

    while (getline(&currLine, &len, movieFile) != -1)
    {
        const char *delim = ",";
        char *token = strtok_r(currLine, delim, &saveptr);
        if (!token)
            continue;
        char *title = strdup(token);

        token = strtok_r(NULL, delim, &saveptr);
        int year = token ? atoi(token) : 0;

        token = strtok_r(NULL, delim, &saveptr);
        char *languageArr = token;

        token = strtok_r(NULL, delim, &saveptr);
        double rating = token ? strtod(token, NULL) : 0.0;

        newMovie = createMovie(title, year, languageArr, rating);

        if (head == NULL)
        {
            head = newMovie;
            tail = newMovie;
        }
        else
        {
            tail->next = newMovie;
            tail = newMovie;
        }
        count++;
        free(title);
        
    }

    struct movie *curPos = head;
    while (curPos != NULL)
    {

        char tempPathName[640] = "";
        snprintf(tempPathName, sizeof(tempPathName), "%s%d.txt", pathname, curPos->year);

        // Windows open call format
        int yearFile = open(tempPathName, O_WRONLY | O_CREAT | O_APPEND, 0640);
        if (yearFile < 0)
        {
            perror("Error opening year file");
            curPos = curPos->next;
            continue;
        }

        char tempTitle[256] = "";
        snprintf(tempTitle, sizeof(tempTitle), "%s\n", curPos->title);

        ssize_t bytes_written = write(yearFile, tempTitle, strlen(tempTitle));
        if (bytes_written < 0)
        {
            perror("Error writing to file");
        }
        close(yearFile);
        curPos = curPos->next;
    }

    free(currLine);
    freeMovieList(head);
    fclose(movieFile);
}













/**
this is all new content i believe, its the same structure as the loops i used in the last two assignments, definitely a little messy but whatever
 */
int main(int argc, char **argv)
{

    DIR *dir = opendir(".");
    int exit = 0;
    while (exit == 0)
    {
        srand(time(NULL));

        struct dirent *entry;
        struct stat dirStat;

        printf("1. Select file to process\n"
               "2. Exit the program\n\n"
               "Enter a choice 1 or 2: ");

        int respO;
        scanf("%d", &respO);
        printf("\n");
        if (respO == 1)
        {
            closedir(dir);
            dir = opendir(".");
            int exitI = 0;
            while (exitI == 0)
            {
                printf("Which file you want to process?\n"
                       "Enter 1 to pick the largest file\n"
                       "Enter 2 to pick the smallest file\n"
                       "Enter 3 to specify the name of a file\n\n"
                       "Enter a choice from 1 to 3: ");

                int respI;
                scanf("%d", &respI);
                printf("\n");
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
                        printf("no largest file found\n");
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
                    closedir(dir);
                    dir = opendir(".");
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
                        printf("no smallest file found\n");
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
                    closedir(dir);
                    dir = opendir(".");
                    char fileName[257];
                    int size = 0;
                    printf("Enter the complete file name: ");
                    scanf("%s", fileName);
                    printf("\n");
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
                        printf("The file %s was not found. Try again\n", fileName);
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

    closedir(dir);
    return EXIT_SUCCESS;
}
