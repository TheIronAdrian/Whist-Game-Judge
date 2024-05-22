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
#define MAXCULORI 4
#define MAXTIPURICARTI 13
#define CONSTWINSTREAK 5
#define WINSTREAKPOINTS 10

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
  int contorStreak;
  int valGhicita;
  int nrCastiguri;
};

FILE *fout;
PLAYER date[MAXN];
CARTE cartiOrd[PACHET];
CARTE cartiUtil[PACHET];
char culoriPosibile[MAXCULORI]={'I','C','P','T'}; ///Putem sa le schimbam
char tipuriCarti[MAXTIPURICARTI]={'1','2','3','4','5','6','7','8','9','J','Q','K','A'}; ///Putem sa le schimbam
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

  aux=0;
  z=0;
  while(tipuriCarti[z]!=a && z<MAXTIPURICARTI){
    z++;
    aux++;
  }

  if(z<MAXTIPURICARTI){
    return aux;
  }

  return -1;
}

bool CarteMaiMare(char a,char b){
  return CalculPutereCarte(a)>CalculPutereCarte(b);
}

CARTE InitMana(int nrCarti,int firstPlayer){
  int i,j,c,poz;

  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  fprintf(fout,"RANDOM SHUFFLE SEED : %d\n",seed);
  std::default_random_engine rng(seed);

  shuffle(cartiOrd,cartiOrd+nrCartiPachet,rng);

  /*
  cartiOrd[0]={'A','C',0};
  cartiOrd[1]={'9','T',0};
  cartiOrd[2]={'A','I',0};
  cartiOrd[3]={'9','P',0};
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
    return {-1,-1,-1};
  }
  return cartiOrd[poz];
}

bool CheckValitateCarte(int carteaData,int jucator,int nrCarti,CARTE Atu,CARTE PrimaPusa){
  int i;
  CARTE aux;

  if(PrimaPusa.culoare==-1){
    return 1;
  }

  aux=date[jucator].carti[carteaData];

  assert(!(carteaData<0 || carteaData>nrCarti) && "Cartea Data Out Of Bounds");

  assert(aux.used==0 && "Cartea Data A Fost Folosita Deja");

  if(aux.culoare==PrimaPusa.culoare){
    return 1;
  }

  i=0;
  while(i<nrCarti && !(date[jucator].carti[i].culoare==PrimaPusa.culoare && date[jucator].carti[i].culoare==0)){
    i++;
  }

  assert(i>=nrCarti && "Cartea Data Nu Respecta Culoarea");

  if(Atu.culoare==-1 || date[jucator].carti[carteaData].culoare==Atu.culoare){
    return 1;
  }

  i=0;
  while(i<nrCarti && !(date[jucator].carti[i].culoare==Atu.culoare && date[jucator].carti[i].culoare==0)){
    i++;
  }

  assert(i>=nrCarti && "Cartea Data Nu Este Atu");

  return 1;
}

void AfisJocStart(CARTE Atu,int nrCarti){
  int i,j;

  fprintf(fout,"\n------MECI %2d------\n\n",nrMeci);

  if(nrCarti!=8){
    fprintf(fout,"Atu -- %c %c --\n",Atu.val,Atu.culoare);
  }

  for(i=0;i<n;i++){
    fprintf(fout,"\n%s - Nr Alese - %2d\n",date[i].name,date[i].valGhicita);
    for(j=0;j<nrCarti;j++){
      fprintf(fout,"%c %c\n",date[i].carti[j].val,date[i].carti[j].culoare);
    }
  }
  fprintf(fout,"\n");
}

bool Maxim(CARTE a,CARTE b,CARTE Atu){
  int valA,valB;

  valA=CalculPutereCarte(a.val);
  valB=CalculPutereCarte(b.val);

  if(a.culoare==Atu.culoare){
    valA+=MAXTIPURICARTI+1;
  }
  if(b.culoare==Atu.culoare){
    valB+=MAXTIPURICARTI+1;
  }

  return valA<valB;
}

int Joc(int nrCarti,int firstPlayer){
  int i,j,c,s,poz,x,aux,z;
  CARTE Atu,ma,carteJucator,primaCarte;


  Atu=InitMana(nrCarti,firstPlayer);///Initializare

  ///-----------------Start Zona Ghicit------------------------
  s=0;
  i=firstPlayer;
  for(c=1;c<n;c++){
    date[i].valGhicita=date[i].SetGhicit(nrCarti,-1,s,date[i].carti);
    assert(date[i].valGhicita<=nrCarti && date[i].valGhicita>=0 && "Ai ghicit un numar de carti imposibil");
    s+=date[i].valGhicita;
    i=(i+1)%n;
  }
  date[i].valGhicita=date[i].SetGhicit(nrCarti,nrCarti-s,s,date[i].carti);

  assert(date[i].valGhicita<=nrCarti && date[i].valGhicita>=0 && "Ai ghicit un numar de carti imposibil");
  assert(date[i].valGhicita!=nrCarti-s && "Ultima persoana a spus fix numarul carti pe care nu are voie");

  ///-----------------Final Zona Ghicit------------------------

  AfisJocStart(Atu,nrCarti);

  cartiUtil[0]=Atu;
  poz=1;

  for(z=0;z<nrCarti;z++){
    i=firstPlayer;
    x=firstPlayer;

    aux=date[x].GiveCarte(nrCarti,date[x].valGhicita-date[x].nrCastiguri,{-1,-1,-1},Atu,date[x].carti);

    CheckValitateCarte(aux,i,nrCarti,Atu,{-1,-1,-1});

    date[x].carti[aux].used=1;
    carteJucator=date[x].carti[aux];

    primaCarte=carteJucator;
    ma=carteJucator;

    fprintf(fout,"%s : %c %c\n",date[i].name,carteJucator.val,carteJucator.culoare);

    for(c=1;c<n;c++){
      i=(i+1)%n;
      aux=date[i].GiveCarte(nrCarti,date[i].valGhicita-date[i].nrCastiguri,primaCarte,Atu,date[i].carti);
      CheckValitateCarte(aux,i,nrCarti,Atu,primaCarte);

      date[i].carti[aux].used=1;
      carteJucator=date[i].carti[aux];

      if(Maxim(ma,carteJucator,Atu)){
        ma=carteJucator;
        x=i;
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
      if(date[i].contorStreak>=0){
        date[i].contorStreak++;
      }else{
        date[i].contorStreak=-1;
      }
    }else{
      date[i].puncte-=abs(date[i].nrCastiguri-date[i].valGhicita);
      if(date[i].contorStreak<=0){
        date[i].contorStreak--;
      }else{
        date[i].contorStreak=1;
      }
    }
    if(abs(date[i].contorStreak)==CONSTWINSTREAK){
      date[i].puncte+=WINSTREAKPOINTS*(date[i].contorStreak/CONSTWINSTREAK);
      date[i].contorStreak=0;
      fprintf(fout,"#");
    }
    fprintf(fout,"%s : %3d ",date[i].name,date[i].puncte);
    fprintf(fout,"\n");

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
  for(i=MAXTIPURICARTI-n*2;i<MAXTIPURICARTI;i++){
    for(z=0;z<4;z++){
      cartiOrd[j*4+z]={tipuriCarti[i],culoriPosibile[z],0};
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
