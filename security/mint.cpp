#include <stdio.h>
#include <assert.h>
#include <cmath>
#include <string.h>


typedef unsigned int uns;

#define MAX_REFS (8192*79)
#define AUTOREF_CORRECTION 1

uns TREFI=73;
uns TARGET_BANK_MTTF_YRS=10*1000;
uns NUM_ATTACK_LINES=TREFI;
uns INCLUDE_VICTIM_REFRESH=0; 
uns NUM_TREFW=8192;

double pvals[MAX_REFS];


/////////////////////////////////////////////////////
/////////////////////////////////////////////////////

double get_mttf(uns th, uns copies, uns num_attack_lines, uns REFS){
  double rel_time_to_attack;

  // double P=(1/((double)TREFI+INCLUDE_VICTIM_REFRESH))*copies;
  double P=(1/(24.0+INCLUDE_VICTIM_REFRESH))*copies;

  uns new_th = (double)(th)/(double)(copies);

  
  for(uns ii=0; ii<REFS; ii++){

    if(ii<new_th){
      pvals[ii]=0;
    }

    if(ii==new_th){
      pvals[ii]=pow(1-P,new_th);
    }

    if(ii> new_th){
      double term1 = pvals[ii-1];
      double term2 = pow(1-P,new_th)*P;
      double term3 = 1-pvals[ii-new_th-1];
      pvals[ii] = term1 + term2*term3;
    }
	
  }

  double auto_ref_factor = 1;

  if(AUTOREF_CORRECTION){
    rel_time_to_attack = (double)(new_th)/(double)(8192);
    auto_ref_factor = (1-rel_time_to_attack);
    if(auto_ref_factor < 0) {auto_ref_factor=0; }
  }
  //
  double pfail = num_attack_lines * auto_ref_factor * pvals[REFS-1];

  //printf("PFail: %4.2E TH: %u Copies: %u RelTime: %4.2f AutoRef: %4.2f \n", pfail, th, copies, rel_time_to_attack, auto_ref_factor);
  
  double mttf_ms = 32/pfail;
  double mttf_sec = mttf_ms/1000;
  double mttf_yrs = mttf_sec/(3600*24*365);

  return mttf_yrs;
}


/////////////////////////////////////////////////////
/////////////////////////////////////////////////////

uns get_trh_star(uns num_copies, uns num_attack_lines, uns REFS){
  double start=1;
  double end=100000;
  double mid=0;

  while(end-start > 1){
     mid=(start+end)/2.0;

     if(get_mttf(mid, num_copies, num_attack_lines, REFS) < TARGET_BANK_MTTF_YRS){
       start = mid;
     }else{
       end = mid;
     }

  }

  return mid;

}


/////////////////////////////////////////////////////
/////////////////////////////////////////////////////

void print_copies_mttf(){

   for(uns ii=1; ii<= TREFI; ii++){
    uns copies=ii;
    uns trh_star = get_trh_star(copies,1, NUM_TREFW);
    printf("TRH_STAR for %u copies is %u\n", ii, trh_star);
  }
   
}



/////////////////////////////////////////////////////
/////////////////////////////////////////////////////

void print_circular_mttf(){

  uns REFS=NUM_TREFW;
  
   for(uns ii=1; ii<= 2*TREFI; ii++){
     REFS=NUM_TREFW;
     
    if(ii>TREFI){
      double factor = (double)(ii)/(double)(TREFI);
      double new_refs = (double) NUM_TREFW/factor;
      REFS = (uns) new_refs;
    }
    uns trh_star = get_trh_star(1,ii, REFS);
    printf("%u\t%u\n", ii, trh_star);
  }
   

}


/////////////////////////////////////////////////////
/////////////////////////////////////////////////////

void print_copies_circular_mttf(uns budget){
  uns REFS=NUM_TREFW;
  
  for(uns ii=1; ii<=TREFI; ii++){
    uns copies = ii;
    uns num_attack_lines=budget/ii;
    REFS = NUM_TREFW;
    if( (copies*num_attack_lines)>TREFI){
      double factor = (double)(copies*num_attack_lines)/(double)(TREFI);
      double new_refs = (double)(NUM_TREFW)/factor;
      REFS = (uns) new_refs;
    }
    uns trh_star = get_trh_star(copies, num_attack_lines, REFS);
    //    printf("TRH_STAR for %u copies %u lines (budget:%u) is %u\n", ii, num_attack_lines, budget, trh_star);
    printf("%u\t%u\n",copies, trh_star);
    
  }
   
}



/////////////////////////////////////////////////////
/////////////////////////////////////////////////////

void print_ttf_trh(){
  uns prev_val1 = TARGET_BANK_MTTF_YRS;

  
  for(uns ii=10; ii<=1000*1000; ii*=10){
    TARGET_BANK_MTTF_YRS=ii;
    uns trh_star = get_trh_star(1,NUM_ATTACK_LINES, NUM_TREFW);
    printf("TRH_STAR for %u Yrs(Bank-MTTF)  is %u\n", ii, trh_star);
  }
   
  TARGET_BANK_MTTF_YRS=prev_val1;
}



/////////////////////////////////////////////////////
/////////////////////////////////////////////////////

void print_trefi_trh(){
  uns prev_val1 = TREFI;

  
  for(uns ii=65; ii<=80; ii++){
    TREFI=ii;
    uns trh_star = get_trh_star(1,NUM_ATTACK_LINES, NUM_TREFW);
    //printf("TRH_STAR for TREFI %u is %u TRH-D is %u\n", ii, trh_star, trh_star/2);
    printf("%u\t%u\n", ii, trh_star/2);
  }
   
  TREFI=prev_val1;
}

/////////////////////////////////////////////////////
/////////////////////////////////////////////////////

void print_rfm_trh(){
  uns prev_val = TREFI;
  uns RFMTH, trh_star;

  RFMTH=73*2;
  TREFI=RFMTH;
  trh_star = get_trh_star(1,RFMTH, (prev_val*NUM_TREFW)/RFMTH);
  printf("TRH_STAR for RFMTH %u is %u\n", RFMTH, trh_star);

  RFMTH=32;
  TREFI=RFMTH;
  trh_star = get_trh_star(1,RFMTH, (prev_val*NUM_TREFW)/RFMTH);
  printf("TRH_STAR for RFMTH %u is %u\n", RFMTH, trh_star);

  RFMTH=16;
  TREFI=RFMTH;
  trh_star = get_trh_star(1,RFMTH, (prev_val*NUM_TREFW)/RFMTH);
  printf("TRH_STAR for RFMTH %u is %u\n", RFMTH, trh_star);
   
  TREFI=prev_val;
}

/////////////////////////////////////////////////////
/////////////////////////////////////////////////////

int main(int argc, char* argv[]){
  uns analyze_attack_lines = 0;
  uns analyze_copies=0;
  uns analyze_trefi=0;
  
  for (uns ii = 1; ii < argc; ii++) {

    if (!strcmp(argv[ii], "-d")) {
      INCLUDE_VICTIM_REFRESH=1; 
    }

    else if (!strcmp(argv[ii], "-a")) {
      analyze_attack_lines=1;
    }
	  
    else if (!strcmp(argv[ii], "-c")) {
      analyze_copies=1;
    }

    else if (!strcmp(argv[ii], "-t")) {
      analyze_trefi=1;
    }

    else {
      printf("Invalid option %s", argv[ii]);
      assert(0);
    }    
  }


  if(analyze_attack_lines){
    print_circular_mttf();
    return 0;
  }
  
  if(analyze_copies){
    print_copies_circular_mttf(TREFI);
    return 0;
  }

  
  if(analyze_trefi){
    INCLUDE_VICTIM_REFRESH=1;
    print_trefi_trh();
    return 0;
  }

  return 0;
}


