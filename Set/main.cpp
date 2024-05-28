#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#define MAXN 7
#define MAXT 8
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
CARTE Atu;
int nrPlayer;
int player;
int joc;

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

int main(int argc, char** argv){
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
    level++;    datePublice[i].valGhicita=StringToNr(level,argv);
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

  return rand()%joc;
}
