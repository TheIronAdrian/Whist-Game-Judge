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
  int valGhicita;
  int nrCastiguri;
};

CARTE carti[MAXT];
PUBLICPLAYER datePublice[MAXN];
CARTE Atu,PrimaCarte;
int nrPlayer;
int player;
int joc;
int Caz;
int n;

int StringToNr(int level, char** argv){
  int semn,nr,pozArgv;

  pozArgv=0;

  while(argv[level][pozArgv]==' '){
    pozArgv++;
  }

  if(argv[level][pozArgv]=='-'){
    semn=-1;
    pozArgv++;
  }else{
    semn=1;
  }
  nr=0;

  while(argv[level][pozArgv]>='0' && argv[level][pozArgv]<='9'){
    nr=nr*10+argv[level][pozArgv]-'0';
    pozArgv++;
  }

  return nr*semn;
}

void Citire(int argc, char** argv){
  int i,j,level;

  joc=StringToNr(1,argv);
  nrPlayer=StringToNr(2,argv);
  player=StringToNr(3,argv);
  level=4;

  for(i=0;i<joc;i++){
    carti[i].val=argv[level][0];
    carti[i].culoare=argv[level][1];
    level++;
  }

  for(i=0;i<nrPlayer;i++){
    datePublice[i].puncte=StringToNr(level,argv);
    level++;
    datePublice[i].contorStreak=StringToNr(level,argv);
    level++;
    datePublice[i].valGhicita=StringToNr(level,argv);
    level++;
    datePublice[i].nrCastiguri=StringToNr(level,argv);
    level++;
    for(j=0;j<joc;j++){
      datePublice[i].cartiFolosite[j].val=argv[level][0];
      datePublice[i].cartiFolosite[j].culoare=argv[level][1];
      level++;
    }
  }

  Atu.val=argv[level][0];
  Atu.culoare=argv[level][1];
  level++;

  Caz=argv[level][0];
  level++;

  if(Caz=='+'){
    PrimaCarte.val=argv[level][0];
    PrimaCarte.culoare=argv[level][1];
  }
}

void Testing(){
  int i,j;

  printf("Atu : %c%c\n",Atu.val,Atu.culoare);

  printf("CARTILE MELE:\n");

  for(j=0;j<MAXT;j++){
    printf("%c%c\n",carti[j].val,carti[j].culoare);
  }

  for(i=0;i<nrPlayer;i++){
    printf("Puncte : %2d\n",datePublice[i].puncte);
    printf("Streak : %2d\n",datePublice[i].contorStreak);
    printf("ValGhi : %2d\n",datePublice[i].valGhicita);
    printf("NrCast : %2d\n",datePublice[i].nrCastiguri);
    for(j=0;j<MAXT;j++){
      printf("%c%c\n",datePublice[i].cartiFolosite[j].val,datePublice[i].cartiFolosite[j].culoare);
    }
  }

  if(Caz=='+'){
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

int main(int argc, char** argv){

  Citire(argc,argv);
  //Testing();


  if(Caz=='-'){ ///Doua moduri
    return ValoareGhicita();///Cel in care te pune sa setezi cat ghicesti
  }
  return CarteGhicita();///Cel in care te pune sa setezi cartea
}
