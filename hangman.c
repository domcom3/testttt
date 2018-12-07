#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <ctype.h>
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

int stringContains(const char letters_guessed[], const char letter) {

    int len = strlen(letters_guessed);

    for (int i = 0; i < len; i++) {
        if (letters_guessed[i] == letter) {
            return 1;
        }
    }

    return 0;
}

int is_word_guessed(const char secret[], const char letters_guessed[]) {

    int secretLen = strlen(secret);

    for (int i = 0; i < secretLen; i++) {
        if (!stringContains(letters_guessed, secret[i])) {
            return 0;
        }
    }

    // word is guessed
    return 1;
}

void get_guessed_word(const char secret[], const char letters_guessed[], char guessed_word[]) {

    strcpy(guessed_word, secret);

    if (!is_word_guessed(secret, letters_guessed)) {
        int secretLen = strlen(secret);
        for (int i = 0; i < secretLen; i++) {
            if (stringContains(letters_guessed, secret[i])) {
                guessed_word[i] = secret[i];
            } else {
                guessed_word[i] = '_';
            }
        }
    }
    

    return;
}

void get_available_letters(const char letters_guessed[], char available_letters[]) {

    char alphabet[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '\0'};
    int finalCounter = 0;

    for (int i = 0; i < 26; i++) {
        if (stringContains(letters_guessed, alphabet[i])) {
            continue;
        } else {
            available_letters[finalCounter] = alphabet[i];
            finalCounter++;
        }
    }

    available_letters[finalCounter] = '\0';

    return;
}

int nullTerminstorStringPos(const char word[]) {
    int wordLen = strlen(word);

    for (int i = 0; i <= wordLen; i++) {
        if (word[i] == '\0')
            return i;
    }

    return -1;
}

void hangman(const char secret[]) {

    int guessesCount = 8;
    int secretLen = strlen(secret);
    char input[secretLen+2];
    char letters_guessed[27];
    char available_letters[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '\0'};
    char guessed_word[secretLen+1];

    memset(input, '\0', secretLen+2);
    memset(letters_guessed, '\0', 27);
    memset(guessed_word, '\0', secretLen+1);

    printf("Welcome to the game, Hangman!\n");
    printf("I am thinking of a word that is %d letters long.\n", secretLen);

    while (guessesCount != 0) {
        printf("-------------\n");

        if (is_word_guessed(secret, letters_guessed)) {
            printf("Congratulations, you won!\n");
            return;
        }

        printf("You have %d guesses left.\n", guessesCount);
        get_available_letters(letters_guessed, available_letters);
        printf("Available letters: %s\n", available_letters);
        printf("Please guess a letter: ");

        scanf("%s", input);
        if (strlen(input) > 1) {
            if (is_word_guessed(secret, input)) {
                printf("Congratulations, you won!\n");
                return;
            } else {
                printf("Sorry, bad guess. The word was %s.\n", secret);
                return;
            }
        } else {
            if (!isalpha(input[0])) {
                printf("Oops! '%c' is not a valid letter: ", input[0]);
                get_guessed_word(secret, letters_guessed, guessed_word);

                int guessedLen = strlen(guessed_word);
                for (int j = 0; j < guessedLen; j++) {
                    printf("%c", guessed_word[j]);
                    if (j != guessedLen-1)
                        printf(" ");
                    else
                        printf("\n");
                }
            } else {
                if (!islower(input[0]))
                    input[0] = tolower(input[0]);
                if (stringContains(letters_guessed, input[0])) {
                    printf("Oops! You've already guessed that letter: ");
                    get_guessed_word(secret, letters_guessed, guessed_word);

                    int guessedLen = strlen(guessed_word);
                    for (int j = 0; j < guessedLen; j++) {
                        printf("%c", guessed_word[j]);
                        if (j != guessedLen-1)
                            printf(" ");
                        else
                            printf("\n");
                    }
                } else {
                    letters_guessed[nullTerminstorStringPos(letters_guessed)] = input[0];
                    if (stringContains(secret, input[0])) {
                        printf("Good guess: ");
                        get_guessed_word(secret, letters_guessed, guessed_word);

                        int guessedLen = strlen(guessed_word);
                        for (int j = 0; j < guessedLen; j++) {
                            printf("%c", guessed_word[j]);
                            if (j != guessedLen-1)
                                printf(" ");
                            else
                                printf("\n");
                        }
                    } else {
                        printf("Oops! That letter is not in my word: ");
                        get_guessed_word(secret, letters_guessed, guessed_word);

                        int guessedLen = strlen(guessed_word);
                        for (int j = 0; j < guessedLen; j++) {
                            printf("%c", guessed_word[j]);
                            if (j != guessedLen-1)
                                printf(" ");
                            else
                                printf("\n");
                        }
                        guessesCount--;
                    }
                }
            }
        }
    }

    printf("-------------\n");
    printf("Sorry, you ran out of guesses. The word was %s.\n", secret);

    return;
}
