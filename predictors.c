//Raymond Rolston, Shawn Bailey
//Project 1
//CS520
//Branch Prediction

#include<stddef.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>

//two bit state machine
int TwoBit(int, int);

int UpdateG(int, int, int, int);

int main(int argc, char *argv[]) {

//Arguments--------------------------------------------------
if(argc!=3){
  fprintf(stderr,"Unacceptable number of arguments %d encountered.", argc);
  fprintf(stderr, "Format: ./predictors <input_trace.txt> <output.txt>\n" );
  exit(1);
}
//-----------------------------------------------------------

//Variables--------------------------------------------------
long long int y=0;//number of branches
long long int xT=0;//always taken
long long int xNT=0;//always not taken
long long int xTournament=0;//Tournament predictor count
long long int xLocal=0;//local predictor count 
long long int xGLoabl=0;//global predictor count
//-----------------------------------------------------------

//Prediction Logic-------------------------------------------
//-Tournament------------------------------------------------
int tour[10]={0};//0-3
int tourGshare[10]={0};
int tG11=0;//Gshare state
int tourBimodal[10]={0};
//not taken = 1, taken = 0, 
//strongly taken = 0, strongly not taken =3, taken not taken = 1, not taken taken =2
//Read & Count-----------------------------------------------
char behavior;
int addr, next;
FILE * inputFile;
inputFile = fopen(argv[1],"r");
FILE * outputFile;
outputFile = fopen(argv[2],"w");

while(fscanf(inputFile, "%d%c%d\n", &branch, &behavior, &next) != EOF) {
   if(behavior=='t') {//taken-------------------
     xT++;//static

//Tournament-------------------------------------------------

    if(tour[(addr%10)]==0 || tour[(addr%10)]==1){//prefer gshare
      if(tourGshare[(addr ^ tG11)%10]==0 || tourGshare[(addr ^ tG11)%10]==1){//correct
        xTournament++;
        if(tourBimodal[addr%10]==1 || tourBimodal[addr%10]==0){//bimodal also correct
          tour[(addr%10)]=tour[(addr%10)];
        }else{//bimodal not correct
          tour[(addr%10)]=0;
        }
      }else{//gshare incorrect
        if(tourBimodal[addr%10]==1 || tourBimodal[addr%10]==0){//but bi modal is ocrrect
          tour[(addr%10)]=tour[(addr%10)]+1;
        }
      }
    }else{//prefer bimodal
      if(tourBimodal[addr%10]==1 || tourBimodal[addr%10]==0){//correct prdiction
        xTournament++;
        if(tourGshare[(addr ^ tG11)%10]==0 || tourGshare[(addr ^ tG11)%10]==1){//gshare also correct
          tour[(addr%10)]=tour[(addr%10)];
        }else{//gshare incorrect
          tour[(addr%10)]=3;
        }
      }else{//worng prediction
        if(tourGshare[(addr ^ tG11)%10]==0 || tourGshare[(addr ^ tG11)%10]==1){//gshare only correct
          tour[(addr%10)]=tour[(addr%10)]-1;
        }
      }
    }
    tourBimodal[(addr%10)]=TwoBit(tourBimodal[(addr%10)],0);
    tourGshare[(addr ^ tG11)%10]=TwoBit(tourGshare[(addr ^ tG11)%10],0);
    tG11 = UpdateG(tG11, 11, (addr%10), 0);


   }else {//Not taken----------------------------------------
     xNT++;//static

//Tournament-------------------------------------------------

    if(tour[(addr%10)]==0 || tour[(addr%10)]==1){//prefer gshare
      if(tourGshare[(addr ^ tG11)%10]==2 || tourGshare[(addr ^ tG11)%10]==3){//correct
        xTournament++;
        if(tourBimodal[addr%10]==2 || tourBimodal[addr%10]==3){//bimodal also correct
          tour[(addr%10)]=tour[(addr%10)];
        }else{//bimodal not correct
          tour[(addr%10)]=0;
        }
      }else{//gshare incorrect
        if(tourBimodal[addr%10]==2 || tourBimodal[addr%10]==3){//but bi modal is ocrrect
          tour[(addr%10)]=tour[(addr%10)]+1;
        }
      }
    }else{//prefer bimodal
      if(tourBimodal[addr%10]==2 || tourBimodal[addr%10]==3){//correct prdiction
        xTournament++;
        if(tourGshare[(addr ^ tG11)%10]==2 || tourGshare[(addr ^ tG11)%10]==3){//gshare also correct
          tour[(addr%10)]=tour[(addr%10)];
        }else{//gshare incorrect
          tour[(addr%10)]=3;
        }
      }else{//worng prediction
        if(tourGshare[(addr ^ tG11)%10]==2 || tourGshare[(addr ^ tG11)%10]==3){//gshare only correct
          tour[(addr%10)]=tour[(addr%10)]-1;
        }
      }
    }
    tourBimodal[(addr%10)]=TwoBit(tourBimodal[(addr%10)],1);
    tourGshare[(addr ^ tG11)%10]=TwoBit(tourGshare[(addr ^ tG11)%10],1);
    tG11 = UpdateG(tG11, 11, (addr%10), 1);

   y++;
}
fclose(inputFile);
//----------------------------------------------------------
fprintf(outputFile, "%lld,%lld;\n", xT, y);
fprintf(outputFile, "%lld,%lld;\n", xNT, y);
fprintf(outputFile, "%lld,%lld;\n", xTournament, y);
fclose(outputFile);
//-----------------------------------------------------------

//Clean-Up---------------------------------------------------

//-----------------------------------------------------------

return 0;
}

//two bit state machine
int TwoBit(int hist, int act){//takes current state and actual state
  int newPred;
  if(act==0){//branch taken
    if(hist==1 || hist==0){
      newPred=0;
    }else if(hist==2){
      newPred=1;
    }else{
      newPred=2;
    }
  }else{
    if(hist==2 || hist==3){
      newPred=3;
    }else if(hist==1){
      newPred=2;
    }else{
      newPred=1;
    }
  }
  return newPred;//returns new prediction to store in table
}

int UpdateG(int G, int Gsize, int Gaddr, int act){
  int modder=1;
  int i;
  for(i=0;i<Gsize-1;i++){
    modder = modder<<1;
    modder = modder|1;
  }
    if(act==0){//taken
      G= G<<1;
      G= G&modder;
      G= G|1;
    }else{
      G= G<<1;
      G= G&modder;
  }
  return G;
}
