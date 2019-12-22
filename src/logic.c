#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "./headers/logic.h"
#include "./headers/scene.h"
#include "./headers/image.h"
#include "./headers/light.h"

#define MAX (1700)

extern Level lvl;

/* Inicijalizacija globalnih promenljivih */
void initialize(){
    time_parameter = 0;
    timer_active = 0;

    moves[0] = 0;
    moves[1] = 0;

    x = 0;
    x_pom = 1;
    z = 0;

    lives = 3;
    hit = 0;
}

/* Funkcija za ucitavanje nivoa iz datoteke */
char** loadLevel(char * path, int *rowNumber, int *obstacleNumberInRow){

    FILE *in;
    in = fopen(path, "r");
    if(in == NULL){
        fprintf(stderr, "Error! Couldn't open file.\n");
        exit(EXIT_FAILURE);
    }

    char buffer[MAX];

    fscanf(in, "%d\n", rowNumber);
    fscanf(in, "%d\n", obstacleNumberInRow);

    char **levelMatrix = NULL;
    levelMatrix = (char**) malloc(sizeof(char*) * (*rowNumber));
    if(levelMatrix == NULL){
        fprintf(stderr, "Error! Unsuccessful 1st malloc.\n");
        exit(EXIT_FAILURE);
    }

    int i = 0;
    while(fgets(buffer, 100, in) != NULL){

        levelMatrix[i] = NULL;
        levelMatrix[i] = (char*) malloc(sizeof(char) * (*obstacleNumberInRow));
        if(levelMatrix[i] == NULL){
            deallocLevel(levelMatrix, i);
            fprintf(stderr, "Error! Unsuccessful 2nd malloc.");
            exit(EXIT_FAILURE);
        }
        else{
            for(int j = 0; j < *obstacleNumberInRow; j++)
                levelMatrix[i][j] = buffer[j*2];
        }

        i++;
    }

    return levelMatrix;
}

/* Brisanje nivoa i oslobadjanje memorije */
void deallocLevel(char **levelMatrix, int rowNumber){
    for (int i = 0; i < rowNumber; i++)
        free(levelMatrix[i]);
    free(levelMatrix);
}

/* Funkcija za obradu kolizija */
bool hasCollision(char** lvlMatrix, int rowNumber){
        
    int i = (int)z;
    int j = x_pom; 
       
    if (i < rowNumber){
        /* Prilikom sudara sa kockom gubi se jedan zivot. */
        if (lvlMatrix[i][j] == '#' && hit == 0){ 
            lives--;
            hit = 1;
        }
        /* Prilikom prolaska pored objekta enemy, u slucaju da ga nismo pogodili,
           gubimo jedan zivot. Inace, on menja boju u sivu. */
        else if ((lvlMatrix[i][0] == 'o' || lvlMatrix[i][1] == 'o' || lvlMatrix[i][2] == 'o') && hit == 0){   
            // if(enemyNotShot()){
            //     lives--;
            // }
            // else{
            //     enemyDisappear();
            //     ili boja enemy postaje siva
            // }
            lives--;
            hit = 1;
        }
        /* U slucaju nailaska na simbol heal, ako nemamo sve zivote, dobijamo
           jedan dodatan zivot. */
        else if (lvlMatrix[i][j] == 'x' && hit == 0){
            if (lives < 3)
                lives++;
            hit = 1;
        }
        else if(lvlMatrix[i][j] != '#' && lvlMatrix[i][j] != 'x'
             && lvlMatrix[i][0] != 'o' && lvlMatrix[i][1] != 'o' && lvlMatrix[i][2] != 'o'){
            hit = 0;
        }

        /* Ako imamo 0 zivota, izgubili smo. */
        if (lives == 0)
            return true;
    }        

    return false;
}