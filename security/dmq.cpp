#include <stdio.h>
#include <assert.h>
#include <cmath>
#include <string.h>


typedef unsigned uns;

// max tREFI in refresh window
#define MAX_SIZE 8192



#define AUTOREF_CORRECTION  1

uns TARGET_BANK_MTTF_YRS=10*1000;

//--- set 1 for single sided, 2 for double sided
uns SIDED=1;

// insertion probability P (double for two-sided)
double P = SIDED/34.0; 
uns DMQ_ATTACK_WIDTH=397/SIDED; 
uns APT_TRH=659*2/SIDED;
uns NUM_ATTACK_LINES=72;

uns PFAIL_MULT=1; // 


//--- variables for prob of n insertions in TREFI window

double matrix[MAX_SIZE][MAX_SIZE]; // matrix
double mc_state_probs[MAX_SIZE]; //  state probability

double mc_state_cprobs[MAX_SIZE]; //  cumulative state probability


/////////////////////////////////////////////////////
/////////////////////////////////////////////////////

void init_markov_matrix(uns size){
  uns ii, jj;

  for(ii=0; ii<size; ii++){
    for(jj=0; jj<size; jj++){
      matrix[ii][jj]=0; // zeroed out
    }
  }

  // ----- All rows, except last------
  for(ii=0; ii<size-1; ii++){
    for(jj=0; jj<size; jj++){
      if( jj == 0 ){
	matrix[ii][jj]= P; 
      }
      if( jj == (ii+1) ){
	matrix[ii][jj]= (1-P); 
      }
    }
  }

  // ----- Last row always goes to state-0 (REF) ------
  matrix[size-1][0]= 1;

  /*
  for(ii=0; ii<size; ii++){
    printf("\n");
    for(jj=0; jj<size; jj++){
      printf("%5.3f\t", matrix[ii][jj]); 
    }
  }

  printf("\n");
  */

}

/////////////////////////////////////////////////////
/////////////////////////////////////////////////////

void init_mc_state_probs(uns size){
//--- init state_probs
  for(uns ii=0; ii<size; ii++){
    if(ii==0){
      mc_state_probs[ii]=1;
    }else{
      mc_state_probs[ii]=0;
    }
  }

}

/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
/*initial state vector multiplies with state transfer matrix*/
void step_markov_chain(uns size){
  uns ii, colid;
  double output[MAX_SIZE]; //  we will copy this to mc_state_prob

  for(ii=0; ii<size; ii++){
    output[ii]=0;
  }

  //-- column-0 is non-sparse, do all

  for(ii=0; ii<size; ii++){
    output[0] += ( mc_state_probs[ii] * matrix[ii][0] );
  }
  
  //-- other columns have exactly 1 non-zero entry 

  for(colid=1; colid<size; colid++){
    uns index = colid-1;
    output[colid] = ( mc_state_probs[index] * matrix[index][colid] );
  }

  //--  copy outputs to mc_state_probs
  for(ii=0; ii<size; ii++){
    mc_state_probs[ii] = output[ii];
  }

}

/////////////////////////////////////////////////////
/////////////////////////////////////////////////////

void fill_mc_state_cprobs(uns size, uns th){
  double total=0;

  //--- we are interested in TRH-x to TRH
  for(uns ii=th-DMQ_ATTACK_WIDTH; ii<th; ii++){
    total += mc_state_probs[ii];
  }
  
  
//--- init state_probs
  for(uns ii=th-DMQ_ATTACK_WIDTH; ii<th; ii++){
    mc_state_cprobs[ii]=total;
    total -= mc_state_probs[ii];
  }

}


/////////////////////////////////////////////////////
/////////////////////////////////////////////////////

double get_mttf(uns th, uns size){

  double auto_ref_factor = 1;

  if(AUTOREF_CORRECTION){
     /*(th/size) denotes the ratio of a refresh window
     that overlaps with the TRH back-to-back row activations.*/
    auto_ref_factor = (1-(double)(th)/size); //auto-refresh与TRH个back-to-back row activations无重叠的比例
  }

  fill_mc_state_cprobs(size, th);

  uns eff_th = 0;

  if(th > DMQ_ATTACK_WIDTH){
    eff_th = th - DMQ_ATTACK_WIDTH ; // 397 is acts in DMQ attack
  }

  /*
  1.先不看NUM_ATTACK_LINES和auto_ref_factor，mc_state_cprobs是状态向量
  eff_th(th-DMQ_ATTACK_WIDTH)到th的总和；
  2.最有效的攻击方式就是tREFI中只攻击一行，所以NUM_ATTACK_LINES就是72；
  3.PFAIL_MULT是每个tREFI的repeats数量，PFAIL_MULT*mc_state_cprobs[eff_th]
  等于DMQ_ATTACK_WIDTH个tREFI的未采样到实际攻击行概率；
  4.auto_ref_factor is the portion of a refresh window that
  falls outside the TRH back-to-back row activations.
  (An attacker does not have a way to infer or learn when a specific victim row is autorefreshed.
  Thus, the attacker cannot avoid the case when the “lucky” series of
  back-to-back unsampled row activations still fails to flip bits because
  it overlaps with the auto-refresh of the victim row.[Microsoft Row-sampling])*/
  double pfail = PFAIL_MULT * NUM_ATTACK_LINES * auto_ref_factor * mc_state_cprobs[eff_th];


  double mttf_ms = 32/pfail;
  double mttf_sec = mttf_ms/1000;
  double mttf_yrs = mttf_sec/(3600*24*365);

  //printf("Th: %u Prob: %5.2E CProb: %5.2E Pfail: %5.2E MTTF: %4.2f\n", th, mc_state_probs[eff_th], mc_state_cprobs[eff_th], pfail, mttf_yrs); 

  return mttf_yrs;

}

/////////////////////////////////////////////////////
/////////////////////////////////////////////////////

uns get_trh_star(uns size){
  double start=APT_TRH;
  double end=start+DMQ_ATTACK_WIDTH;
  double mid;
  
   while(end-start > 1){
     mid=(start+end)/2.0;

     if(get_mttf(mid, size) < TARGET_BANK_MTTF_YRS){
       start = mid;
     }else{
       end = mid;
     }

  }

  return mid;
}


/////////////////////////////////////////////////////
/////////////////////////////////////////////////////

uns print_trh_for_t(uns t, uns size){
  init_mc_state_probs(size);

  for(uns ii=0; ii<t; ii++){
    step_markov_chain(size);
  }

  fill_mc_state_cprobs(size, APT_TRH);

  /*

    printf("\n[T: %4u]\t", t);
    
  for(uns ii=APT_TRH-400; ii<=APT_TRH; ii+=50){
    printf("%4.1E\t", mc_state_cprobs[ii]);
  }

  printf("\n\n");

  */
  
  uns trh_star = get_trh_star(size);

  return trh_star;
}


/////////////////////////////////////////////////////
/////////////////////////////////////////////////////

uns get_highest_trh_star(uns size){
  uns highest_trh_star=0;

  for(uns t=100; t<=8000; t+=100){
    /*8192是每个tREFI只攻击一行，看第8192个ACT的P_REFW，从而确定TRH。这里在5xtREFI中
    连续攻击同一行397/2次，row selection必定刷新该行，所以到这就能知道TRH了，相当于达到
    8192次ACT了。等于总共只花费了t+DMQ_ATTACK_WIDTH个tREFI就达到了最大TRH，所以直接
    换算成从第1个到第t+DMQ_ATTACK_WIDTH个tREFI之间的每个tREFI有多少repeats就行。*/
    uns repeats_per_tREFI= MAX_SIZE/(t+ DMQ_ATTACK_WIDTH);
    PFAIL_MULT= repeats_per_tREFI;
    uns my_trh_star = print_trh_for_t(t,size);
    if(my_trh_star > highest_trh_star){
      highest_trh_star = my_trh_star;
    }
    printf("%u\t%u\n", t, my_trh_star);
    //printf(".");fflush(stdout);
  }

  return highest_trh_star;
}


/////////////////////////////////////////////////////
/////////////////////////////////////////////////////

void print_mttf_trh_star(uns size){
  uns prev_val = TARGET_BANK_MTTF_YRS;
  
for(uns ii=1000; ii<=1000*1000; ii *=10){
    TARGET_BANK_MTTF_YRS=ii;
    
    if(ii == 1000){APT_TRH=2634/SIDED;}
    if(ii == 10000){APT_TRH=2800/SIDED;}
    if(ii == 100000){APT_TRH=2965/SIDED;}
    if(ii == 1000000){APT_TRH=3129/SIDED;}
    
    uns my_highest_trh_star = get_highest_trh_star(size);
    printf("\nHighest TRH_STAR for TTF of %u Yrs is %u (%u)\n", ii, my_highest_trh_star, my_highest_trh_star/2);
  }

  printf("\n");
  TARGET_BANK_MTTF_YRS=prev_val;
}


/////////////////////////////////////////////////////
/////////////////////////////////////////////////////

void print_rfm_trh_star(){
  uns RFMTH, size, trh_star, rfm, ii;
  double prev_val1 = P;
  uns prev_val2 = DMQ_ATTACK_WIDTH;
  uns prev_val3 = APT_TRH;
  uns prev_val4 = NUM_ATTACK_LINES;
  uns prev_val5 = TARGET_BANK_MTTF_YRS;

  for(rfm=16; rfm<= 32; rfm*=2){
    RFMTH=rfm;
    P = SIDED/(RFMTH+1); 
    DMQ_ATTACK_WIDTH=(RFMTH*3)/SIDED;
    

    for(ii=1000; ii<= 1000*1000; ii*=10){
      TARGET_BANK_MTTF_YRS=ii;

    if(RFMTH==32){
      if(ii == 1000){APT_TRH=1208/SIDED;}
      if(ii == 10000){APT_TRH=1284/SIDED;}
      if(ii == 100000){APT_TRH=1357/SIDED;}
      if(ii == 1000000){APT_TRH=1432/SIDED;}
    }
    
    if(RFMTH==16){
       if(ii == 1000){APT_TRH=626/SIDED;}
       if(ii == 10000){APT_TRH=665/SIDED;}
       if(ii == 100000){APT_TRH=703/SIDED;}
       if(ii == 1000000){APT_TRH=741/SIDED;}
    }

    NUM_ATTACK_LINES=RFMTH;
    size=8192;

    trh_star = get_highest_trh_star(size);
    printf("\nTRH_STAR for RFMTH %u MTTF %u is %u (%u)\n", RFMTH, ii, trh_star, trh_star/2);
    }
  }


// revert
  P = prev_val1; 
  DMQ_ATTACK_WIDTH=prev_val2; 
  APT_TRH=prev_val3;
  NUM_ATTACK_LINES=prev_val4;
  TARGET_BANK_MTTF_YRS=prev_val5;
}

/////////////////////////////////////////////////////
/////////////////////////////////////////////////////

int main(int argc, char* argv[]){
  uns size= 8192;
  uns my_highest_trh_star;

  uns print_single_sided=0;
  uns print_double_sided=0;
  uns print_mttf=0;
  uns print_rfm=0;

   
  for (uns ii = 1; ii < argc; ii++) {

    if (!strcmp(argv[ii], "-s")) {
      print_single_sided=1; 
    }

    else if (!strcmp(argv[ii], "-d")) {
      print_double_sided=1; 
    }
	  
    else if (!strcmp(argv[ii], "-m")) {
      print_mttf=1; 
    }

    else if (!strcmp(argv[ii], "-r")) {
      print_rfm=1; 
    }

    else {
      printf("Invalid option %s", argv[ii]);
      assert(0);
    }    
  }



  if(print_single_sided){
    SIDED=1;
    P = SIDED/34.0; 
    DMQ_ATTACK_WIDTH=397/SIDED; 
    APT_TRH=659*2/SIDED;
    init_markov_matrix(size);
    my_highest_trh_star = get_highest_trh_star(size);
    //printf("\nHighest TRH_STAR is %u\n", my_highest_trh_star);
    return 0;
  }

   if(print_double_sided){
     SIDED=2;
     P = SIDED/34.0; 
     DMQ_ATTACK_WIDTH=397/SIDED; 
     APT_TRH=659*2/SIDED;
     init_markov_matrix(size);
     my_highest_trh_star = get_highest_trh_star(size);
     //printf("\nHighest TRH_STAR is %u\n", my_highest_trh_star);
     return 0;
  }

   init_markov_matrix(size);
   
   if(print_mttf){
     print_mttf_trh_star(size);
     return 0;
   }

   if(print_rfm){
     print_rfm_trh_star();
     return 0;
   }
}


