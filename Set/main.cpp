#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#define MAXN 7
#define MAXT 8
#define ENCRYPT 256 ///Nu schimba
using namespace std;

struct CARTE{
  char val,culoare;
};

struct PUBLICPLAYER{
  CARTE cartiFolosite[MAXT];
  int puncte;
  int contorStreak;
  int valLicitata;
  int nrCastiguri;
};

FILE *input;
CARTE carti[MAXT];
PUBLICPLAYER datePublice[MAXN];
CARTE Atu,PrimaCarte;
int nrPlayer;
int player;
int nrMeci;
int joc;
int Caz;

void ReadWords(){
  char a;

  a=fgetc(input);
  while(a!=':'){
    a=fgetc(input);
  }
}

void Citire(){
  int i,j,level;
  char aux;
  string sir;

  ReadWords();
  fscanf(input,"%d",&joc);
  ReadWords();
  fscanf(input,"%d",&nrPlayer);
  ReadWords();
  fscanf(input,"%d",&player);
  ReadWords();
  fscanf(input,"%d",&nrMeci);

  ReadWords();
  for(i=0;i<nrPlayer;i++){
    fscanf(input,"%d",&datePublice[i].puncte);
  }

  ReadWords();
  for(i=0;i<nrPlayer;i++){
    fscanf(input,"%d",&datePublice[i].contorStreak);
  }

  ReadWords();
  for(i=0;i<nrPlayer;i++){
    fscanf(input,"%d",&datePublice[i].valLicitata);
  }

  ReadWords();
  fscanf(input," %c %c",&Atu.val,&Atu.culoare);

  ReadWords();
  for(i=0;i<joc;i++){
    fscanf(input," %c %c",&carti[i].val,&carti[i].culoare);
  }

  ReadWords();
  for(j=0;j<joc;j++){
    fscanf(input," ");
    fgetc(input);
    for(i=0;i<nrPlayer;i++){
      fscanf(input," %c %c ",&datePublice[i].cartiFolosite[j].val,&datePublice[i].cartiFolosite[j].culoare);
      fgetc(input);
    }
  }

  fscanf(input," ");
  aux=fgetc(input);

  if(aux=='L'){
    Caz=0;
  }else{
    Caz=1;
    ReadWords();
    ReadWords();
    fscanf(input," %c %c ",&PrimaCarte.val,&PrimaCarte.culoare);
  }

}

void Testing(){
  int i,j;

  printf("Atu : %c%c\n",Atu.val,Atu.culoare);

  printf("CARTILE MELE:\n");

  for(j=0;j<joc;j++){
    printf("%c%c\n",carti[j].val,carti[j].culoare);
  }

  for(i=0;i<nrPlayer;i++){
    printf("Puncte : %2d\n",datePublice[i].puncte);
    printf("Streak : %2d\n",datePublice[i].contorStreak);
    printf("ValGhi : %2d\n",datePublice[i].valLicitata);
    printf("NrCast : %2d\n",datePublice[i].nrCastiguri);
    for(j=0;j<joc;j++){
      printf("%c%c\n",datePublice[i].cartiFolosite[j].val,datePublice[i].cartiFolosite[j].culoare);
    }
  }

  if(Caz==1){
    printf("PrimaC : %c%c\n",PrimaCarte.val,PrimaCarte.culoare);
  }
}

int ValoareGhicita(){
  int rasp;

  rasp=rand()%joc;

  ///Codul Jucatorului

  return rasp;
}

int CarteGhicita(){
  CARTE rasp;

  rasp.val='A';
  rasp.culoare='I';
  ///Codul Jucatorului

  return rasp.val*ENCRYPT+rasp.culoare;///Nu schimba encryptarea
}

int main(){
  input=fopen("input.in","r");

  Citire();
  //Testing();

  fclose(input);
  if(Caz==0){ ///Doua moduri
    return ValoareGhicita();///Cel in care te pune sa setezi cat ghicesti
  }
  return CarteGhicita();///Cel in care te pune sa setezi cartea
}
