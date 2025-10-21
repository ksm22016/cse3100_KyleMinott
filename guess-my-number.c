#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/types.h>
#include <dirent.h>

// search TODO to find the place you need to work on

void die(char *s)
{
    fprintf(stderr, "Error: %s\n", s);
    if (errno)
        perror("errno");
    exit(EXIT_FAILURE);
}

#define MAX_VALUE       1000
#define MSG_BUF_SIZE    100
#define PIPEFD_READ 0
#define PIPEFD_WRITE 1

typedef  struct {
    int v;
    int num_attempts;
    char message[MSG_BUF_SIZE]; 
} gmn_t;

void gmn_init(gmn_t *pg, int seed)
{
    srand(seed);
    pg->v = rand() % MAX_VALUE + 1;
    pg->num_attempts = 0;
    strcpy(pg->message, "You have not got the right number yet.\n");
} 

int gmn_check(gmn_t *pg, int guess)
{
    pg->num_attempts ++;
    if (pg->v == guess) {
        snprintf(pg->message, MSG_BUF_SIZE, 
                "It took you %d attempt(s) to guess the number %d.\n", 
                pg->num_attempts, pg->v);  
        return 0;
    }
    if (pg->v > guess)
        return 1;
    return -1;
}

char * gmn_get_message(gmn_t *pg)
{
    return pg->message;
}

int    gmn_get_max()
{
    return MAX_VALUE;
}

// this function runs the demo session
// all gmn_ functions should be called in child process 
// and then send the result to the parent process
void guess_my_number(int seed)
{
    gmn_t gmn;

    // initialize the game
    gmn_init(&gmn, seed);

    int min = 1;
    int max = gmn_get_max();
    int result;

    do {
        // make a guess
        int guess = (min + max)/2;
        printf("My guess: %d\n", guess);

        // check
        result = gmn_check(&gmn, guess);

        // if not correct, prepare for the next guess
        if(result > 0) 
            min = guess + 1;
        else if(result < 0)
            max = guess - 1;
    } while (result != 0);

    // print out the final message
    fputs(gmn_get_message(&gmn), stdout);
}


// Tasks for the child process. 
// 
// Parameters:
//  fdp:    the pipe for parent to write and for child to read 
//  fdc:    the pipe for child to write and for parent to read
//  seed:   seed for the random numer
//
// This function should not return.
// This function does not print any characters, except for error messages.
void    child_main(int fdp[], int fdc[], int seed)
{
    gmn_t   gmn;

    gmn_init(&gmn, seed);

    // TODO
    //  close unused file descriptors
    if (close(fdp[PIPEFD_WRITE]) == -1) die("child: close(fdp[WRITE])");
    if (close(fdc[PIPEFD_READ])  == -1) die("child: close(fdc[READ])");
    
    //  send max value to the parent
{
        int maxvalue = gmn_get_max();
        ssize_t n, left = sizeof(int);
        unsigned char *p = (unsigned char *)&maxvalue;
        while (left > 0) {
            n = write(fdc[PIPEFD_WRITE], p, left);
            if (n < 0) die("child: write max value to parent");
            p    += n;
            left -= n;
        }
    }
    //  repeat the following until guess from parent is correct
    while (1) {
        int guess;
    //      wait for a guess from parent
        {
            ssize_t n, left = sizeof(int);
            unsigned char *p = (unsigned char *)&guess;
            while (left > 0) {
                n = read(fdp[PIPEFD_READ], p, left);
                if (n < 0) die("child: read guess from parent");
                if (n == 0) die("child: unexpected error while reading guess");
                p    += n;
                left -= n;
            }
        }
    //      call gmn_check() 
    int result = gmn_check(&gmn, guess);
    //      send the result to parent
    {
            ssize_t n, left = sizeof(int);
            unsigned char *p = (unsigned char *)&result;
            while (left > 0) {
                n = write(fdc[PIPEFD_WRITE], p, left);
                if (n < 0) die("child: write result to parent");
                p    += n;
                left -= n;
            }
        }
    //  send the final message back (as a string) 
    if (result == 0) {
            const char *msg = gmn_get_message(&gmn);   // ends with '\n', no '\0' needed
            size_t len = strlen(msg);                  // length INCLUDING everything up to '\n'
            ssize_t n;
            const unsigned char *p = (const unsigned char *)msg;
            size_t left = len;
            while (left > 0) {
                n = write(fdc[PIPEFD_WRITE], p, left);
                if (n < 0) die("child: write final message");
                p    += n;
                left -= n;
            }
            break;
        }
    }
    //  close all pipe file descriptors
    if (close(fdp[PIPEFD_READ])   == -1) die("child: close(fdp[READ])");
    if (close(fdc[PIPEFD_WRITE])  == -1) die("child: close(fdc[WRITE])");
    

    exit(EXIT_SUCCESS);
}

void    print_file_descriptors(int child);

int main(int argc, char *argv[])
{
    int seed = 3100;
    int demo = 0;

    // parse the command line arguments

    for (int i = 1; i < argc; i ++) {
        if (! strcmp(argv[i], "demo")) {
            demo = 1;
        } else if (! strcmp(argv[i], "-h")) {
            fprintf(stderr, "Usage: %s [<seed>] [demo]\n", argv[0]);
            return 1;
        } else {
            seed = atoi(argv[i]);
            if (seed <= 0)
                die("seed must be a postive number.");
        }
    }

    if (demo) {
        guess_my_number(seed);
        exit(0);
    }

    // Now, we do it using two processes
    // The child generates a random number
    // The parent tries to guess the number.
    // The child tell the parent

    // two pipes
    // fdp : parent writes
    // fdc : child writes
    
    int fdp[2], fdc[2];

    //pipe creation
    if (pipe(fdp) == -1)
        die("pipe() failed.");

    if (pipe(fdc) == -1)
        die("pipe() failed.");

    pid_t pid;
    pid = fork();

    if (pid < 0)
        die("fork() failed.");

    if(pid == 0)
        child_main(fdp, fdc, seed); // never returns
    
    // parent continues
    
    int min = 1;
    int max;
    int guess;
    int result;

    // TODO
    //      close unused pipe file descriptor
    if (close(fdp[PIPEFD_READ]) == -1)  die("parent: close(fdp[READ])");
    if (close(fdc[PIPEFD_WRITE]) == -1) die("parent: close(fdc[WRITE])");
    
    //      get max from the child
    {
    
        ssize_t n, left = sizeof(int);
        unsigned char *p = (unsigned char *)&max;
        while (left > 0) {
            n = read(fdc[PIPEFD_READ], p, left);
            if (n < 0) die("parent: read max from child");
            if (n == 0) die("parent: unexpected error reading max");
            p    += n;
            left -= n;
    }
}
    do { 
        guess = (min + max)/2;
        printf("My guess: %d\n", guess);

        // TODO
        //     send guess to the child
    {
        ssize_t n, left = sizeof(int);
        unsigned char *p = (unsigned char *)&guess;
        while (left > 0) {
            n = write(fdp[PIPEFD_WRITE], p, left);
            if (n < 0) die("parent: write guess to child");
            p    += n;
            left -= n;
        }
    }
        //     wait for the result from the child
    {
        ssize_t n, left = sizeof(int);
        unsigned char *p = (unsigned char *)&result;
        while (left > 0) {
            n = read(fdc[PIPEFD_READ], p, left);
            if (n < 0) die("parent: read result from child");
            if (n == 0) die("parent: unexpected error reading result");
            p    += n;
            left -= n;
        }
    }
        if (result > 0)
            min = guess + 1;
        else if (result < 0)
            max = guess - 1;
    } while (result != 0);

    // flush stdout buffer
    fflush(stdout);

    // TODO
    //      receive the final message and print it to stdout
{
    char c;
    while (1) {
        ssize_t n = read(fdc[PIPEFD_READ], &c, 1);
        if (n < 0) die("parent: read final message");
        if (n == 0) die("parent: unexpected error reading final message");
        fputc(c, stdout);
        if (c == '\n') break;
    }
}
    //      close all pipe file descriptors
    if (close(fdp[PIPEFD_WRITE]) == -1) die("parent: close(fdp[WRITE])");
    if (close(fdc[PIPEFD_READ])  == -1) die("parent: close(fdc[READ])");

    //wait for the child process to finish
    wait(NULL);
    return 0;
}