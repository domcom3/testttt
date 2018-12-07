#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "hangman.h"


int get_word(char secret[]){
    // check if file exists first and is readable
    FILE *fp = fopen(WORDLIST_FILENAME, "rb");
    if( fp == NULL ){
        fprintf(stderr, "No such file or directory: %s\n", WORDLIST_FILENAME);
        return 1;
    }

    // get the filesize first
    struct stat st;
    stat(WORDLIST_FILENAME, &st);
    long int size = st.st_size;

    do{
        // generate random number between 0 and filesize
        long int random = (rand() % size) + 1;
        // seek to the random position of file
        fseek(fp, random, SEEK_SET);
        // get next word in row ;)
        int result = fscanf(fp, "%*s %20s", secret);
        if( result != EOF )
            break;
    }while(1);

    fclose(fp);

    return 0;
}

 int is_word_guessed(const char secret[], const char letters_guessed[]){
 	int string_let = strlen(letters_guessed);
 	int string_sec = strlen(secret);
	for(int s = 0; s<string_sec;s++){
		for(int i = 0;i<string_let;i++){
			if(letters_guessed[i] != secret[s){
				return 0;
			}
		}	
	}
	 return 1;
 }
