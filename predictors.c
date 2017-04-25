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
long long int xGlobal=0;//global predictor count
//-----------------------------------------------------------

//Prediction Logic-------------------------------------------
//-Tournament------------------------------------------------
int tour[10]={0};//0-3
int i;
for(i=0; i<10; i++){
	tour[i]=3;
}
int tourGshare[64]={0};
for(i=0; i<64; i++){
	tourGshare[i]=3;
}
int tG11= 0b111111;//Gshare state
int tourBimodal[10]={0};
for(i=0; i<10; i++){
	tourBimodal[i]=3;
}
//not taken = 1, taken = 0, 
//strongly taken = 0, strongly not taken =3, taken not taken = 1, not taken taken =2
//Read & Count-----------------------------------------------
char behavior;
int addr, next;
FILE * inputFile;
inputFile = fopen(argv[1],"r");
FILE * outputFile;
outputFile = fopen(argv[2],"w");

char local,global,selector,final;

while(fscanf(inputFile, "%d%c%d\n", &addr, &behavior, &next) != EOF) {
   fprintf(outputFile, "%d",addr);
   if(behavior=='t') {//taken-------------------
     xT++;//static

//Tournament-------------------------------------------------
final = 'n';
global = 'n';
local = 'n';

    if(tour[(addr%10)]==0 || tour[(addr%10)]==1){//prefer gshare
      selector = 'g';
      if(tourGshare[( tG11)]==0 || tourGshare[( tG11)]==1){//correct
        xGlobal++;
        xTournament++;
        final = 't';
	global = 't';
        if(tourBimodal[addr%10]==1 || tourBimodal[addr%10]==0){//bimodal also correct
	  xLocal++;
          local ='t';
          tour[(addr%10)]=tour[(addr%10)];
        }else{//bimodal not correct
          tour[(addr%10)]=0;
        }
      }else{//gshare incorrect
        if(tourBimodal[addr%10]==1 || tourBimodal[addr%10]==0){//but bi modal is ocrrect
          tour[(addr%10)]=tour[(addr%10)]+1;
          xLocal++;
	  local = 't';
        }
      }
    }else{//prefer bimodal
      selector = 'l';
      if(tourBimodal[addr%10]==1 || tourBimodal[addr%10]==0){//correct prdiction
        xTournament++;
        xLocal++;
        final = 't';
	local = 't';
        if(tourGshare[( tG11)]==0 || tourGshare[( tG11)]==1){//gshare also correct
          tour[(addr%10)]=tour[(addr%10)];
	  xGlobal++;
	  global = 't';
        }else{//gshare incorrect
          tour[(addr%10)]=3;
        }
      }else{//worng prediction
        if(tourGshare[(tG11)]==0 || tourGshare[(tG11)]==1){//gshare only correct
          tour[(addr%10)]=tour[(addr%10)]-1;
          xGlobal++;
	  global = 't';

        }
      }
    }
    tourBimodal[(addr%10)]=TwoBit(tourBimodal[(addr%10)],0);
    tourGshare[(tG11)]=TwoBit(tourGshare[(tG11)],0);
    tG11 = UpdateG(tG11, 6, (addr%10), 0);

 
   }else {//Not taken----------------------------------------
     xNT++;//static
final = 't';
global = 't';
local = 't';
//Tournament-------------------------------------------------

    if(tour[(addr%10)]==0 || tour[(addr%10)]==1){//prefer gshare
      selector = 'g';
      if(tourGshare[( tG11)]==2 || tourGshare[( tG11)]==3){//correct
        xTournament++;
	xGlobal++;
	final = 'n';
	global = 'n';
        if(tourBimodal[addr%10]==2 || tourBimodal[addr%10]==3){//bimodal also correct
          tour[(addr%10)]=tour[(addr%10)];
	  xLocal++;
	  local = 'n';
        }else{//bimodal not correct
          tour[(addr%10)]=0;
        }
      }else{//gshare incorrect
        if(tourBimodal[addr%10]==2 || tourBimodal[addr%10]==3){//but bi modal is ocrrect
          tour[(addr%10)]=tour[(addr%10)]+1;
	  xLocal++;
	  local ='n';
        }
      }
    }else{//prefer bimodal
      selector = 'l';
      if(tourBimodal[addr%10]==2 || tourBimodal[addr%10]==3){//correct prdiction
        xLocal++;
        xTournament++;
	final = 'n';
	local = 'n';
        if(tourGshare[( tG11)]==2 || tourGshare[( tG11)]==3){//gshare also correct
          tour[(addr%10)]=tour[(addr%10)];
	  xGlobal++;
	  global ='n';
        }else{//gshare incorrect
          tour[(addr%10)]=3;
        }
      }else{//worng prediction
        if(tourGshare[( tG11)]==2 || tourGshare[( tG11)]==3){//gshare only correct
          tour[(addr%10)]=tour[(addr%10)]-1;
	  xGlobal++;
	  global = 'n';
        }
      }
    }
    tourBimodal[(addr%10)]=TwoBit(tourBimodal[(addr%10)],1);
    tourGshare[( tG11)]=TwoBit(tourGshare[( tG11)],1);
    tG11 = UpdateG(tG11, 6, (addr%10), 1);
   }
   fprintf(outputFile, "%c%c%c%c%c\n",local,global,selector,final, behavior);
   y++;
}
fclose(inputFile);
//----------------------------------------------------------
printf( "Total Taken: %lld,%lld;\n", xT, y);
printf( "Total Not Taken: %lld,%lld;\n", xNT, y);
printf( "Total Local correct predictions: %lld,%lld;\n", xLocal, y);
printf( "Total Global correct predictions: %lld,%lld;\n", xGlobal, y);
printf( "Total Tournament correct predictions: %lld,%lld;\n", xTournament, y);
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

    }else{
      G= G<<1;
      G= G&modder;
      G= G|1;
  }
  return G;
}
