#include <stdio.h>
#include <stdlib.h> // for EXIT_SUCCESS and EXIT_FAILURE
#include <string.h>
/*
* Function: processMovieFile
* Opens a file, reads and prints each line
* filePath: path to the file
*
* This function shows sample code that opens a file, then in a loop reads and
prints each line in that file.
* You can use this code however you want in your Prog Assignment 2.
*/
struct movie{
    char* title;
    int year;
    char **languageArr;
    double rating;
    int arrcount;
    struct movie* next;
};
struct movie* createMovie(char* title, int year, char* inplanguageArr, double rating){
    struct movie* currMovie = malloc(sizeof(struct movie));
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

  while (token != NULL) {
      // 1. Grow the pointer array dynamically to hold the new token + room for a trailing NULL
      char **temp = realloc(currMovie->languageArr, (element_count + 2) * sizeof(char *));
      if (temp == NULL) {
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
void printMovieList(struct movie* list){
  while(list != NULL){
      // 1. Print the starting details
      printf("%s %d ", list->title, list->year);

      // 2. Loop through the char ** array until reaching the trailing NULL pointer
      // This automatically stops exactly when the languages end, preventing the seg fault!
      for (int i = 0; list->languageArr != NULL && list->languageArr[i] != NULL; i++) {
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
void freeMovie(struct movie* curr){
  free(curr->title);
  free(curr->languageArr);
  free(curr);
}

/*
* Free the memory allocated for elements in the list
*/
void freeMovieList(struct movie* list){
  struct movie* curr;
  while(list != NULL){
    curr = list;
    list = list->next;
    freeMovie(curr);
  }
}



/**
 *
 */
int main(int argc, char **argv)
{
    
    if (argc < 2)
    {
        printf("You must provide the name of the file to process\n");
        printf("Example usage: ./movies movies.csv\n");
        return EXIT_FAILURE;
    }
    



    char *currLine = NULL;
    size_t len = 0;
    // Open the specified file for reading only
    FILE *movieFile = fopen(argv[1], "r");
    // Read the file line by line

    struct movie* head = NULL;
    struct movie* tail = NULL;
    struct movie* newMovie = NULL;
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
        if(head == NULL){
        // This is the first element in the linked link
        // Set the head and the tail to this element
        head = newMovie;
        tail = newMovie;
        } else{
        // This is not the first element. 
        // Add this element to the list and advance the tail
        tail->next = newMovie;
        tail = newMovie;
        }
        
        count++;
    }


    printf("Processed file %s and parsed data for %d movies\n", argv[1], count);




    


  

    int exit = 0;
    while (exit==0)
    {
        printf("1. Show movies released in the specified year\n"
            "2. Show highest rated movie for each year\n"
            "3. Show the title and year of release of all movies in a specific language\n"
            "4. Exit from the program\n\n");

            printf("Enter a choice from 1 to 4: ");
            int respO;
            scanf("%d", &respO);


            if (respO == 1)
            {
                printf("Enter the year for which you want to see movies: \n");
                int respyear;
                scanf("%d", &respyear);
                int found = 0;
                struct movie* curPos = head;
                while (curPos != NULL)
                {
                    if (curPos->year == respyear)
                    {
                        found = 1;
                        printf("%s \n", curPos->title);
                    }
                    curPos = curPos->next;
                }
                if (found == 0)
                {
                    printf("No data about movies released in the year %d \n", respyear);
                }
                
                printf("\n");
            } else if (respO == 2)
            {
                
                struct movie* curPos = head;

                while (curPos != NULL)
                {
                    struct movie* incPos = head;
                    int most = 1;

                    while (incPos != NULL)
                    {
                        if (curPos->year == incPos->year&&incPos->rating>curPos->rating)
                        {
                            most = 0;
                        }
                        
                        incPos = incPos->next;
                    }

                    if (most==1)
                    {
                        printf("%d %.1f %s\n", curPos->year, curPos->rating, curPos->title);
                    }
                    
                    
                    curPos = curPos->next;
                }

                printf("\n");
            }else if (respO == 3)
            {
                printf("Enter the language for which you want to see movies: \n");
                char lang[29];
                scanf("%29s", lang); 
                struct movie* curPos = head;
                while (curPos != NULL)
                {


                    for (int i = 0; i < curPos->arrcount; i++) {
                        if (strcmp(curPos->languageArr[i],lang)==0)
                        {
                            printf("%d %s\n", curPos->year, curPos->title);
                        }
                    }
                    curPos = curPos->next;
                }
                printf("\n");
            }else if (respO == 4)
            {
                exit = 1;
            } else {
                printf("You entered an incorrect choice. Try again.\n");
            }
            
            
            
            


    }
    







        // Free the memory allocated by getline for currLine
    free(currLine);
    freeMovieList(head);
    
    // Close the file
    fclose(movieFile);
    //printf("\nProcessed file %s with %d lines(not including title line)\n", argv[1], count);
    return EXIT_SUCCESS;
}
























