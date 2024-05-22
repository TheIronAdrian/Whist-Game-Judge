#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <time.h>
#include <random>
#include <chrono>
#include <assert.h>
#include <string>
#define MAXN 7
#define MAXT 8
#define PACHET 52
#define MAXNNAME 20

using namespace std;

///I-Inima rosie
///C-Caro(Romb)
///P-Pica(Inima Neagra)
///T-Trefla

///Cartile sunt 1,2,3,4,5,6,7,8,9,J,Q,K,A (in aceasta ordine)

///Reguli :
/*
  - Sistemul de pus carti :
    - Daca ai culoarea pusa de primu, esti obligat sa o pui
    - Daca nu ai culoarea esti obligat sa pui culoarea atu-ului
    - Daca nu ai niciuna poti pune orice carte

  - Sistemul de puncte:
    - Daca ai facut corect este 5+valGhicita
    - Daca nu este -abs(valGhicita-nrCastiguri)

  - De mai implementat :
      - Sa inteleg cum functioneaza header-ul
      - Sa fac jocul de 1 cand vezi restul cartilor
*/
struct CARTE{
  char val,culoare,used;
};

typedef int (*AGhici)(int a,int b,int c,CARTE val[MAXT]);
typedef int (*ADaCarte)(int joc,int nrPuncteRamas,CARTE PrimaCarte,CARTE Atu,CARTE val[MAXT]);

struct PLAYER{
  AGhici SetGhicit;
  ADaCarte GiveCarte;
  CARTE carti[MAXT];
  char name[MAXNNAME];
  int puncte;
  int valGhicita;
  int nrCastiguri;
};

FILE *fout;
PLAYER date[MAXN];
CARTE cartiOrd[PACHET];
CARTE cartiUtil[PACHET];
char culoriPosibile[4]={'I','C','P','T'}; ///Putem sa le schimbam
char jqka[4]={'J','Q','K','A'};
int nrCartiPachet;
int nrMeci;
int n;

///----------------FunctiiJucatori----------

int TTCOSetGhicit(int joc,int catNuAiVoie,int catSaGhicit,CARTE val[MAXT]){
  int aux;

  do{
    aux=rand()%(joc+1);
  }while(aux==catNuAiVoie);

  return aux;
}

int TestSetGhicit(int joc,int catNuAiVoie,int catSaGhicit,CARTE val[MAXT]){
  int aux;

  do{
    aux=rand()%(joc+1);
  }while(aux==catNuAiVoie);

  return aux;
}

int TTCOGiveCarte(int joc,int nrPuncteRamas,CARTE PrimaCarte,CARTE Atu,CARTE val[MAXT]){
  int x;

  x=0;
  if(PrimaCarte.culoare==-1){
    while(val[x].used==1){
      x++;
    }
    return x;
  }

  x=0;
  while(x<joc && (val[x].used!=0 || val[x].culoare==PrimaCarte.culoare)){
    x++;
  }
  if(x<joc){
    return x;
  }

  x=0;
  while(x<joc && (val[x].used!=0 || val[x].culoare==Atu.culoare)){
    x++;
  }
  if(x<joc){
    return x;
  }

  x=0;
  while(val[x].used==1){
    x++;
  }

  /*if(x>2){
    exit(x);
  }*/
  return x;
}

///----------------FunctiiJucatori----------

int CalculPutereCarte(char a){
  int z,aux;

  if(a>='1' && a<='9'){
    return a-'0';
  }
  aux=10;
  z=0;
  while(jqka[z]!=a && z<4){
    z++;
    aux++;
  }

  if(z<4){
    return aux;
  }

  return -1;
}

bool CarteMaiMare(char a,char b){
  return CalculPutereCarte(a)>CalculPutereCarte(b);
}

int Joc(int nrCarti,int firstPlayer){
  int i,j,c,s,poz,x,aux,z;
  CARTE Atu,ma,carteJucator,primaCarte;

  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  fprintf(fout,"RANDOM SHUFFLE SEED : %d\n",seed);
  std::default_random_engine rng(seed);

  shuffle(cartiOrd,cartiOrd+nrCartiPachet,rng);

  /*
  cartiOrd[0]={'A','I',0};
  cartiOrd[1]={'K','I',0};
  cartiOrd[2]={'J','P',0};
  //*/

  poz=0;
  for(j=0;j<nrCarti;j++){
    i=firstPlayer;
    for(c=1;c<=n;c++){
      date[i].carti[j]=cartiOrd[poz];
      date[i].carti[j].used=0;
      i=(i+1)%n;
      poz++;
    }
  }

  for(i=0;i<n;i++){
    date[i].nrCastiguri=0;
  }

  if(nrCarti==8){
    Atu={-1,-1,-1};
  }else{
    Atu=cartiOrd[poz];
  }

  fprintf(fout,"\n------MECI %2d------\n\n",nrMeci);

  if(nrCarti!=8){
    fprintf(fout,"Atu -- %c %c --\n",Atu.val,Atu.culoare);
  }

  s=0;
  i=firstPlayer;
  for(c=1;c<n;c++){
    date[i].valGhicita=date[i].SetGhicit(nrCarti,-1,s,date[i].carti);
    s+=date[i].valGhicita;
    i=(i+1)%n;
  }
  date[i].valGhicita=date[i].SetGhicit(nrCarti,nrCarti-s,s,date[i].carti);

  for(i=0;i<n;i++){
    fprintf(fout,"\n%s - Nr Alese - %2d\n",date[i].name,date[i].valGhicita);
    for(j=0;j<nrCarti;j++){
      fprintf(fout,"%c %c\n",date[i].carti[j].val,date[i].carti[j].culoare);
    }
  }
  fprintf(fout,"\n");

  cartiUtil[0]=Atu;
  poz=1;

  for(z=0;z<nrCarti;z++){
    i=firstPlayer;
    x=firstPlayer;

    aux=date[x].GiveCarte(nrCarti,date[x].valGhicita-date[x].nrCastiguri,{-1,-1,-1},Atu,date[x].carti);
    if(date[x].carti[aux].used==0 && aux<=nrCarti){
      date[x].carti[aux].used=1;
      carteJucator=date[x].carti[aux];
      primaCarte=carteJucator;
      ma=carteJucator;
    }else{
      assert(0 && "L de tip carte gresita data");
    }
    fprintf(fout,"%s : %c %c\n",date[i].name,carteJucator.val,carteJucator.culoare);
    for(c=1;c<n;c++){
      i=(i+1)%n;
      aux=date[i].GiveCarte(nrCarti,date[i].valGhicita-date[i].nrCastiguri,primaCarte,Atu,date[i].carti);
      if(date[i].carti[aux].used==0 && aux<=nrCarti){
        date[i].carti[aux].used=1;
        carteJucator=date[i].carti[aux];
        if(ma.culoare==Atu.culoare){
          if(ma.culoare==carteJucator.culoare){
            if(CarteMaiMare(carteJucator.val,ma.val)){
              ma=carteJucator;
              x=i;
            }
          }
        }else{
          if(ma.culoare==carteJucator.culoare){
            if(CarteMaiMare(carteJucator.val,ma.val)){
              ma=carteJucator;
              x=i;
            }
          }else{
            if(carteJucator.culoare==Atu.culoare){
              ma=carteJucator;
              x=i;
            }
          }
        }
      }else{
        assert(0 && "L de tip carte gresita data");
      }
      fprintf(fout,"%s : %c %c\n",date[i].name,carteJucator.val,carteJucator.culoare);
    }

    firstPlayer=x;
    fprintf(fout,"CASTIGATOR RUNDA %d %s\n",z,date[x].name);
    date[x].nrCastiguri++;
  }


  fprintf(fout,"\n");
  for(i=0;i<n;i++){
    if(date[i].nrCastiguri==date[i].valGhicita){
      date[i].puncte+=5+date[i].nrCastiguri;
    }else{
      date[i].puncte-=abs(date[i].nrCastiguri-date[i].valGhicita);
    }
    fprintf(fout,"%3d ",date[i].puncte);
  }
  fprintf(fout,"\n");

  nrMeci++;
}

void Init(){
  int i,j,z;

  int aux=time(NULL);
  srand(aux);

  fout=fopen("final.out","w");
  fprintf(fout,"RANDOM SEED : %d\n",aux);
  date[0].SetGhicit=TTCOSetGhicit;
  date[1].SetGhicit=TestSetGhicit;
  date[2].SetGhicit=TestSetGhicit;

  date[0].GiveCarte=TTCOGiveCarte;
  date[1].GiveCarte=TTCOGiveCarte;
  date[2].GiveCarte=TTCOGiveCarte;

  strcpy(date[0].name,  "TTCO ");
  strcpy(date[1].name,  "Test1");
  strcpy(date[2].name,  "Test2");
  n=3;
  nrMeci=0;

  j=0;
  for(i=10-(n-2)*2+1;i<=10;i++){
    for(z=0;z<4;z++){
      cartiOrd[j*4+z]={'0'+i-1,culoriPosibile[z],0};
    }
    j++;
  }
  for(i=0;i<4;i++){
    for(z=0;z<4;z++){
      cartiOrd[j*4+z]={jqka[i],culoriPosibile[z],0};
    }
    j++;
  }

  nrCartiPachet=j*4;
}

int main() {
    int i,firstPlayer,winner;

    /*system("cmd.exe");
    system("bash");


    return 0;*/

    Init();///Initializeaza Jocul

    firstPlayer=0; ///Jocurile sunt de n ori 1
    for(i=1;i<=n;i++){
      Joc(1,firstPlayer);
      firstPlayer=(firstPlayer+1)%n;
    }
    for(i=2;i<=7;i++){///si dupa 2 3 ... 7
      Joc(i,firstPlayer);
      firstPlayer=(firstPlayer+1)%n;
    }
    for(i=1;i<=n;i++){///de n ori 8
      Joc(8,firstPlayer);
      firstPlayer=(firstPlayer+1)%n;
    }
    for(i=7;i>=2;i--){///si dupa 7 6 ... 2
      Joc(i,firstPlayer);
      firstPlayer=(firstPlayer+1)%n;
    }
    for(i=1;i<=n;i++){///de n ori 1 (nu stiu inca daca jucam cu cartile pe fata)
      Joc(1,firstPlayer);
      firstPlayer=(firstPlayer+1)%n;
    }

    winner=0; ///Se ia valoarea maxima
    for(i=1;i<n;i++){
      if(date[i].puncte>date[winner].puncte){
        winner=i;
      }
    }

    ///Si toate persoanele cu punctajul maxim
    ///Castig
    fprintf(fout,"\n\n---SCOR  FINAL---\n");
    for(i=0;i<n;i++){
      fprintf(fout,"%s %2d = %3d",date[i].name,i,date[i].puncte);

      if(date[i].puncte==date[winner].puncte){
        fprintf(fout," WINNER");
      }

      fprintf(fout,"\n");
    }

    fclose(fout);
    return 0;
}
