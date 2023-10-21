//Deleted the MIT Stuff to focus on the code at hand.
//Assignment 2: Multithreading
//Name: Mary-Rose Tracy
//ID#:1001852753
//I'm gonna copy & paste my last back up from my program. I wanted it to be faster, but what I ended up doing was messing the numbers up somehow:
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <getopt.h>
#include <stdint.h>
//My include
#include <pthread.h>
#include <time.h> //don't ask me why i deleted this, then it somehow worked. codespace it tripping
//other orginal includes
#include "utility.h"
#include "star.h"
#include "float.h"
//orignal Defines
#define NUM_STARS 30000 
#define MAX_LINE 1024
#define DELIMITER " \t\n"
//msc structs (original)
//have a feeling we gonna be using this thorughout the array
struct Star star_array[ NUM_STARS ];
uint8_t (*distance_calculated)[NUM_STARS];

uint64_t UNOC =0; //check UNOC= UNumberOf Count
pthread_mutex_t Mutex;
int NOThread = 0; //?? 1 NOThread = the number of thread(s)
int IDXOC = 0; //The index of Count
//original doubles
double  min  = FLT_MAX; //Check
double  max  = FLT_MIN; //Check
//my doubles
//from another function let's make it global because it's in several other funct.
double mean = 0; //?? not 0.0 altho that is the same

//don't mess with
void showHelp()
{
  printf("Use: findAngular [options]\n");
  printf("Where options are:\n");
  printf("-t          Number of threads to use\n");
  printf("-h          Show this help\n");
}
void determineAverageAngularDistance(struct Star arr[], int Start, int End )
{
    //double mean = 0; from original, but we made it into a 

    //uint32_t i, j; just put it in the for loop

    //uint64_t UNOC = 0; made UNOC as global
    for (uint32_t i = Start; i < End; i++)//Lrt's just intiialixe the uint_t i & j to make it shorter
    {
      for (uint32_t j = 0; j < NUM_STARS; j++)
      {
        if( i!=j && distance_calculated[i][j] == 0 )
        {
          //From original
          double distance = calculateAngularDistance(arr[i].RightAscension, arr[i].Declination, arr[j].RightAscension, arr[j].Declination ) ;
          //I just noticed that there are THREE j's in the orignal. nope. it's suppose to be 2 i & j's
          distance_calculated[i][j] = 1; //from orignal 
          distance_calculated[j][i] = 1; //from orignal
          //no need for count++;
          pthread_mutex_lock(&Mutex); //I noticed that when I was looking for pthreads that this whole function up in here
          {
            UNOC++; //is looking like a lock and unlock. so we didnt even need to create a lock/ unlock method. we just 
            //do a pthread lock and unlock
            //From Oringal
            if(min>distance )
            {
              min=distance;
            }
            if(max<distance )
            {
              max=distance;
            }
            mean=mean+(distance-mean)/UNOC;//just changed count -> UNOC our global
          }
            pthread_mutex_unlock(&Mutex); //no not &
        }
      }
    }
    //return mean;
    //we aint gonna do separately so boom no return mean
}
///////////////////THE PRBLEMS WITH MAIN//////////////////
//for some reason the thread above aint doing what its suppsoe to do. So Imma dereference it in this here function for the DAAD
//the computer has to know the dereferenced method before the main
void *ThreadFuncOMain(void *arg)
{
  int Start = 0;
  int IVal = NUM_STARS/NOThread;
  int End = 0;

  //copted lines 66-81 in above method then figured out I need to do one of these only smaller form
  pthread_mutex_lock(&Mutex);
  {
    Start=IVal*IDXOC++;  //IDXOC++; we don't need to do that, but we can  sythesize
    End=(IVal*IDXOC);
  }
  pthread_mutex_unlock(&Mutex);

  //initialed DAAD in the end instead in the int main
  determineAverageAngularDistance(star_array,Start,End);

}
int main( int argc, char * argv[] )
{
  //from original
  FILE *fp; //original
  uint32_t star_count = 0; //original

  uint32_t n; //original

  distance_calculated = malloc(sizeof(uint8_t[NUM_STARS][NUM_STARS])); //original
  //this method = original
  if( distance_calculated == NULL )
  {
    uint64_t num_stars = NUM_STARS;
    uint64_t size = num_stars * num_stars * sizeof(uint8_t);
    printf("Could not allocate %ld bytes\n", size);
    exit( EXIT_FAILURE );
  }

  int i, j; //original
  for (i = 0; i < NUM_STARS; i++) //function original
  {
    for (j = 0; j < NUM_STARS; j++)
    {
      distance_calculated[i][j] = 0;
    }
  }

  //printf("%d", argc);
  for( n = 1; n < argc; n++ )  //Method original      
  {
    //printf("argc: %d\n", argc);
    if( strcmp(argv[n], "-help" ) == 0 )
    {
      showHelp();
      exit(0);
    }
    else if (strcmp(argv[n], "-t") == 0) //now let's copy from what we had for help and 
    //implement "-t"
    {
      NOThread = atoi(argv[n+1]);
      /*if NOThread==0
      {
        printf("There");

        return 1;
        //exit(0);
      }
      */
    }
    else if (argc < 2) //if arg < 2 then the number of the thread is 1
    {
      NOThread = 1;
    }
  }
 

  fp = fopen( "data/tycho-trimmed.csv", "r" ); //dont mess with

  if( fp == NULL ) //dont mess with this method
  {
    printf("ERROR: Unable to open the file data/tycho-trimmed.csv\n");
    exit(1);
  }
  //from original
  char line[MAX_LINE];
  while (fgets(line, 1024, fp))
  {
    uint32_t column = 0;

    char* tok;
    for (tok = strtok(line, " ");
            tok && *tok;
            tok = strtok(NULL, " "))
    {
       switch( column )
       {
          case 0:
              star_array[star_count].ID = atoi(tok);
              break;
       
          case 1:
              star_array[star_count].RightAscension = atof(tok); //atof??
              break;
       
          case 2:
              star_array[star_count].Declination = atof(tok);
              break;

          default: 
             printf("ERROR: line %d had more than 3 columns\n", star_count );
             exit(1);
             break;
       }
       column++;
    }
    star_count++;
  }
  printf("%d records read\n", star_count ); //Don't mess with
  //TIME TO GET THE TIME DONE
  //INITALIZE THe Threads in to an array with the number of threads
  pthread_t ThreadID[NOThread];

  //now we need to calculate the time while we go through 
  clock_t Time; //initializer
  //clock_t Time_Start, Time_End, Time;
  Time = clock(); //function to clock the time
  //yeah I'm gonna do a one liner with crackets because that seems more synthethsied.
  //It doesnt matter with the brackets
  for(i=0; i<NOThread; i++){pthread_create(&ThreadID[i],NULL,ThreadFuncOMain,NULL);}//make it null so that it can 
  for(i=0; i<NOThread; i++){pthread_join(ThreadID[i], NULL);}
  //AT First I Did: with Brackets, but it seemed shorter, and not accurate
  //for(i=0; i<NOThread; i++){
  //pthread_create(&ThreadID[i],NULL,ThreadFuncOMain,NULL); //make it null so that it can 
  //for(i=0; i<NOThread; i++)}
  //pthread_join(ThreadID[i], NULL);
  //}
  Time = clock() - Time;
  //The total of the Time = overall start - the time
  //it's a more simplistic way of doing clock(). usually you do star_clock & end. but I kept on getting errors with that one.
  //Getting hella weird answers
  //SOLUTION:
  //we need to some how to make all of the Time be doubled also / by the prim funct of CLOCKS_PER_SEC because then I was getting ginormous answers.
  double AOTTime = ((double)(Time))/ CLOCKS_PER_SEC;
  //double AOTTime= (double)(Time_End - Time_Start)/ CLOCKS_PER_SEC;

  // Find the average angular distance in the most inefficient way possible
  //double distance =  determineAverageAngularDistance( star_array );
  //from original
  //at the end of the function where we used a mutex for p threads it's gonna be mean NOT Distance
  //NOW Print Original + The Time.
  printf("Average distance found is %lf\n", mean ); //distance, is wrong
  printf("Minimum distance found is %lf\n", min );
  printf("Maximum distance found is %lf\n", max );
  printf("Time it Took: %lf\n", AOTTime);
  return 0; //end of program
}
