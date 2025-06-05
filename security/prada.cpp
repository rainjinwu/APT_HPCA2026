#include <iostream>
#include <stdio.h>
#include <assert.h>
#include <cmath>
#include <string.h>


typedef unsigned int uns;

#define MAX_REFS (8192*79)  //(3900-295)/46=79
#define AUTOREF_CORRECTION 1

uns TREFI=72;
uns TARGET_BANK_MTTF_YRS=10*1000;
uns NUM_ATTACK_LINES=TREFI;
uns INCLUDE_VICTIM_REFRESH=0;
uns NUM_TREFW=8192;
uns MAX_SUBARRAY_ROWS=1200;
uns MIN_SUBARRAY_ROWS=500;
uns MAX_BLAST_RADIUS=18;

double pvals[MAX_REFS];

int main(int argc, char* argv[]){
    uns analyze_blast_radius = 0;
    uns analyze_initial_copy_row = 0;
    uns analyze_blast_radius_autorfm=0;
    uns analyze_mitigative_impact=0;
    uns analyze_fractal_mitigation=0;
    uns analyze_step_mitigation=0;
    uns analyze_proximate_row=0;
    uns analyze_general_height=0;
    uns analyze_mixed_attack=0;
    uns analyze_double_sided_attack=0;
    uns trh_double_br[MAX_BLAST_RADIUS + 1];

    for (uns ii = 1; ii < argc; ii++) {

        if (!strcmp(argv[ii], "-v")) {
            INCLUDE_VICTIM_REFRESH=1; 
        }
    
        else if (!strcmp(argv[ii], "-b")) {
            analyze_blast_radius=1;
        }
        else if (!strcmp(argv[ii], "-i")) {
            analyze_initial_copy_row=1;
        }
        else if (!strcmp(argv[ii], "-ba")) {
            analyze_blast_radius_autorfm=1;
        }
          
        else if (!strcmp(argv[ii], "-m")) {
            analyze_mitigative_impact=1;
        }
        else if (!strcmp(argv[ii], "-s")) {
            analyze_step_mitigation=1;
        }
        else if (!strcmp(argv[ii], "-p")) {
            analyze_proximate_row=1;
        }
        else if (!strcmp(argv[ii], "-g")) {
            analyze_general_height=1;
        }
        else if (!strcmp(argv[ii], "-f")) {
            analyze_fractal_mitigation=1;
        }

        else if (!strcmp(argv[ii], "-a")) {
            analyze_mixed_attack=1;
        }

        else if (!strcmp(argv[ii], "-d")) {
            analyze_double_sided_attack=1;
        }
    
        else {
            printf("Invalid option %s", argv[ii]);
            assert(0);
        }    
    }

    // if(analyze_blast_radius){
    //     double Damage = 0.0;
    //     uns trh_star = 0;
    //     printf ("%u\t%u\n", 1, trh_star);
    //     for (int ii = 2; ii <= MAX_BLAST_RADIUS; ++ii) {
    //         double Damage = 0.0;
    //         // Damage = abs(((1.0 - 1.0 / ii) * (1.0 - 1.0 / (2.0 * ii)) + 72.0 * (1.0 - 1.0 / (2.0 * ii))) * log(pow(10, -18)) / log((1.0 - 1.0 / ii) * (1.0 - 1.0 / (2.0 * ii))));
    //         Damage = abs(((1.0 / ii) + (ii - 2) * pow(ii, -2) + 72.0 * (1.0 - 1.0 / (2.0 * ii))) * log(pow(10, -18)) / log((1.0 - 1.0 / ii) * (1.0 - 1.0 / (2.0 * ii))));
    //         trh_star = Damage / (2 * ii);
    //         printf ("%u\t%u\n", ii, trh_star);
    //     }
    // }

    // if(analyze_blast_radius){
    //     double Damage = 0.0;
    //     uns trh_star = 0;
    //     printf ("%u\t%u\n", 1, trh_star);
    //     Damage = abs((0.5 + 72.0 * (1.0 - 1.0 / 4.0)) * log(pow(10, -18)) / log((1.0 - 1.0 / 2.0) * (1.0 - 2.0 / (2.0 * 2.0))));
    //     trh_star = Damage / (2 * 2);
    //     printf ("%u\t%u\n", 2, trh_star);
    //     for (int ii = 3; ii <= MAX_BLAST_RADIUS; ++ii) {
    //         double Damage = 0.0;
    //         // Damage = abs(((1.0 - 1.0 / ii) * (1.0 - 1.0 / (2.0 * ii)) + 72.0 * (1.0 - 1.0 / (2.0 * ii))) * log(pow(10, -18)) / log((1.0 - 1.0 / ii) * (1.0 - 1.0 / (2.0 * ii))));
    //         double kk = (1.0 / ii) * ((2.0 / ((ii - 1) / 2 + ii)) + 1 + 2.0 * (ii - 3.0) / ((ii - 1) / 2 + ii));
    //         double mm = (1.0 - 1.0 / ((ii - 1) / 2 + ii)) * (1.0 - 2.0 / (2.0 * ii));
    //         double ll = log(pow(10, -18));
    //         double nn = 72.0 * (1.0 - 1.0 / (2.0 * ii));
    //         // printf ("%u\t%f\t%f\t%f\t%f\n", ii, kk, mm, nn, ll);
    //         Damage = abs((kk + nn) * ll / log(mm));
    //         // printf ("%u\t%f\n", ii, Damage);
    //         trh_star = Damage / (2 * ii);
    //         printf ("%u\t%u\n", ii, trh_star);
    //     }
    // }

    // if(analyze_initial_copy_row){
    //     double Damage = 0.0;
    //     uns trh_star = 0;
    //     printf ("%u\t%u\n", 1, trh_star);
    //     for (int ii = 2; ii <= MAX_BLAST_RADIUS; ++ii) {
    //         double Damage = 0.0;
    //         double kk = 1.0 / ((ii - 1) / 2 + ii);
    //         double mm = 1.0 - kk;
    //         Damage = abs( (2 + kk) * log(pow(10, -18)) / log(mm));
    //         trh_star = Damage;
    //         printf ("%u\t%u\n", ii, trh_star);
    //     }
    // }

    // if(analyze_initial_copy_row){
    //     double Damage = 0.0;
    //     uns trh_star = 0;
    //     printf ("%u\t%u\n", 1, trh_star);
    //     for (int ii = 2; ii <= MAX_BLAST_RADIUS; ++ii) {
    //         double Damage = 0.0;
    //         // double kk = 1.0 / ((ii - 1) / 2 + ii);
    //         double mm = 1.0 - 1.0 / ii;
    //         Damage = abs( (2 + (1.0 / ii)) * log(pow(10, -18)) / log(mm));
    //         trh_star = Damage;
    //         printf ("%u\t%u\n", ii, trh_star);
    //     }
    // }

    if(analyze_blast_radius_autorfm){
        double p_escape = 0.0;
        double Damage = 0.0;
        double W = 34.0;
        // double TRH_D = log((W * 48.0 + 200.0 / W) / (TARGET_BANK_MTTF_YRS * 365.0 * 24.0 * 60.0 * 60.0 * 1000000000.0)) / log(1.0 - 1.0 / W) / 2.0;    
        /*假设W为34，则最有效的攻击是34个不同Row的ACT，所以一个tREFI（最多72 ACTs）
        会在REF时有三次刷新（34+34+4）。虽然最后一个entry是1/4选的，但实际上还有30个
        不同的row在下一个tREFI，而到下一个tREFI又是34+34+4，那30个在1/34里。*/
        // double TRH_D = log((W * 48.0 + 410.0 / 3 / W) 
        //                / (TARGET_BANK_MTTF_YRS * 365.0 * 24.0 * 60.0 * 60.0 * 1000000000.0))
        //                / log(1.0 - 1.0 / W) / 2.0;

        /*Each tREFI has one attack row among 72 different rows*/
        double TRH_D = log(3900.0/(TARGET_BANK_MTTF_YRS*365.0*24.0*60.0*60.0*1000000000.0)/W)
                        /log(1.0 - 1.0 / W) / 2.0;

        // double TRH_D = log(3900.0/(TARGET_BANK_MTTF_YRS*365.0*24.0*60.0*60.0*1000000000.0)/(W*2.0))
        //                 /log(pow(1.0 - 1.0 / W, 2.0)) / 2.0;
        
        uns trh_star = TRH_D;
        printf ("%u\t%u\n", 1, trh_star);
        for (int ii = 2; ii <= MAX_BLAST_RADIUS; ++ii) {
            double p_escape = 0.0;
            double damage = 0.0;
            int jj = ii;
            while (jj > 1) {
                double kk = (1.0 - pow(2, 1 - jj));
                double tt = (1.0 / (2.0 * ii)) * 2.0;
                damage += tt * (pow(2, 1 - (jj + 1)) + pow(2, 1 - (jj - 1)));
                p_escape += tt * kk;
                --jj;
                // printf ("%u\t%d\t%f\t%f\t%f\n", ii, jj, tt, kk, p_escape);
            }
            
            Damage = abs((damage + 72.0) * log(pow(10, -18)) / log(p_escape));
            // double W = TREFI;
            // uns TRH_D = log((W * 48.0 + 200.0 / W) / (TARGET_BANK_MTTF_YRS * 365 * 24 * 60 * 60 * 1000000000)) / log(1.0 - 1.0 / W) / 2;
            trh_star = Damage / (2 * ii) + TRH_D / (2 * ii);
            printf ("%u\t%u\n", ii, trh_star);
            // Damage = abs((1.0 - 1.0 / ii + 72.0) * log(pow(10, -18)) / log(1.0 - 1.0 / ii));
            // trh_star = Damage / (2 * (ii - 1));
            // printf ("%u\t%u\n", ii, trh_star);
        }
    }

    if(analyze_mitigative_impact){
        for (uns ii = MIN_SUBARRAY_ROWS; ii <= MAX_SUBARRAY_ROWS; ii += 10) {
            printf ("%-5u\t", ii);
            for (uns jj = 1; jj <= MAX_BLAST_RADIUS; ++jj) {
                double Damage = ii / 2;
                trh_double_br[jj] = Damage / (2 * jj);
                printf ("%-5u\t", trh_double_br[jj]);
            }
            printf("\n");
        }
        return 0;
    }

    // if(analyze_blast_radius){
    //     for (int ii = 1; ii <= MAX_BLAST_RADIUS; ++ii) {
    //     double kk = (2.0 * ii - 1) / (2.0 * ii);
    //     double ll = log(pow(10, -18));
    //     double mm = log(1 - 1 / (2.0 * ii));
    //     double Damage = abs(kk * ll / mm);
    //     uns trh_star = Damage / (2 * ii);
    //     // printf ("%f\t%f\t%f\n", kk, ll , mm);    
    //     printf ("%u\t%u\n", ii, trh_star);
    //     }
    //     return 0;
    // }

    if(analyze_fractal_mitigation){
        for (int ii = 1; ii <= MAX_BLAST_RADIUS; ++ii) {
            if (ii == 1) {
                printf ("%u\t%u\n", ii, 0);
                continue;
            }
            else{
                if (ii != MAX_BLAST_RADIUS) {
                    double Damage =  abs((pow(2, -ii + 2) + pow(2, -ii)) * log(pow(10, -18)) / log(1.0 - pow(2, -ii + 1)));
                    uns trh_d_star = Damage / 2;
                    printf ("%u\t%u\n", ii, trh_d_star);
                }
                else {
                    double Damage = abs(pow(2, -ii + 2) * log(pow(10, -18)) / log(1.0 - pow(2, -ii + 2)));
                    uns trh_d_star = Damage;
                    printf ("%u\t%u\n", ii, trh_d_star);
                }
            }
        }
        double Damage = NUM_TREFW * pow(2, -MAX_BLAST_RADIUS + 2);
        uns trh_star = Damage;
        printf ("%u\t%u\n", MAX_BLAST_RADIUS + 1, trh_star);
        Damage = NUM_TREFW * pow(2, -MAX_BLAST_RADIUS + 2);
        trh_star = Damage;
        printf ("%u\t%u\n", MAX_BLAST_RADIUS + 2, trh_star);
        return 0;
    }

    if(analyze_step_mitigation){
        double pvals[MAX_BLAST_RADIUS + 1];
        double p = 0.5;
        double step_init_size = 2.0;
        double mm = step_init_size;
        double Damage = 0.0;
        uns count = 1;
        while (mm <= 500.0) {
            if (mm == step_init_size) {
                mm = (1.0 / p) * step_init_size;
            }
            else {
                mm *= step_init_size;
            }
            
            p *= mm / (1.0 + mm);
            mm += 1.0;
            ++count;
        }
        for (int ii = 0; ii <= count; ++ii) {
            pvals[ii] = p;
            p /= step_init_size;
            // printf ("%u\t%f\n", ii, pvals[ii]);
        }
        double pvals_step[static_cast<int>(step_init_size) * count + 1];
        for (int ii = 0; ii <= count * step_init_size; ++ii) {
            if (ii == 0) {
                pvals_step[ii] = pvals[0];
            }
            else {
                pvals_step[ii] = pvals[(ii + static_cast<int>(step_init_size) - 1) / static_cast<int>(step_init_size)];
            }
            // printf ("%u\t%f\n", ii, pvals_step[ii]);
        }
        for (int ii = 0; ii <= count * step_init_size; ++ii) {
            if (ii == 0) {
                Damage = (1 + pvals_step[ii + 1]) * log(pow(10, -18)) / log((1.0 - pvals_step[ii]) * 5.0 / 6.0);
                uns trh_star = Damage / 2;
                printf ("%u\t%u\n", ii + 1, trh_star);
            }
            else {
                if (ii != count * step_init_size) {
                    Damage = (pvals_step[ii - 1] + pvals_step[ii + 1]) * log(pow(10, -18)) / log(1.0 - pvals_step[ii]);
                    uns trh_star = Damage / 2;
                    printf ("%u\t%u\n", ii + 1, trh_star);
                }
                else {
                    Damage = pvals_step[ii - 1] * log(pow(10, -18)) / log(1.0 - pvals_step[ii]);
                    uns trh_star = Damage;
                    printf ("%u\t%u\n", ii + 1, trh_star);
                }
            }
        }
        Damage = NUM_TREFW * pvals_step[static_cast<int>(count * step_init_size)];
        uns trh_star = Damage;
        printf ("%d\t%u\n", static_cast<int>(count * step_init_size) + 2, trh_star);
        return 0;
    }

    if(analyze_proximate_row){
        int h2 = 17; //test second step height of 2, 4, 8, 16
        double Damage1 = 0.0;
        double Damage1_with_extra_ref = 0.0;
        uns trh_star1[h2 + 1] = {0};
        uns trh_star1_with_extra_ref[h2 + 1] = {0};
        printf ("%u\t", 1);
        for (int ii = 2; ii < h2; ii *= 2) {
            printf ("%u\t%u\t", trh_star1[ii], trh_star1_with_extra_ref[ii]);
        }
        printf("\n");
        // printf ("%u\t%u\t%u\t%u\t%u\n", 1, trh_star1, trh_star1_with_extra_ref,
                //  trh_star2, trh_star2_with_extra_ref);

        for (int ii = 2; ii <= MAX_BLAST_RADIUS+20; ++ii) {
            printf ("%u\t", ii);
            for (int jj = 2; jj < h2; jj *= 2) {
                Damage1 = abs((1.0 + 1.0 / (ii * jj)) * log(pow(10, -18))
                            / log(1.0 - 1.0 / ii));
                Damage1_with_extra_ref = abs((1.0 + 1.0 / (ii * jj)) * log(pow(10, -18))
                                            / log((1.0 - 1.0 / ii) * (5.0 / 6.0)));

                trh_star1[jj] = Damage1 / 2;
                trh_star1_with_extra_ref[jj] = Damage1_with_extra_ref / 2;
                printf ("%u\t%u\t", trh_star1[jj], trh_star1_with_extra_ref[jj]);
            }
            printf("\n");
            
            // printf ("%u\t%u\n", ii, trh_star);
        }
    }

    if(analyze_general_height){
        int h = 17; //test step heights of 2, 4, 8, 16
        double Damage = 0.0;
        uns trh_star = 0;
        printf ("%5u\t", 1);
        for (int ii = 2; ii < h; ii *= 2) {
            printf ("%3u\t", trh_star);
        }
        printf("\n");
        
        for (int ii = 2; ii <= 10; ++ii) {
            printf ("%5u\t", ii);
            for (int jj = 2; jj < h; jj *= 2) {
                Damage = abs((1.0 / ii + 1.0 / (ii * jj)) * log(pow(10, -18))
                             / log(1.0 - 1.0 / (ii * jj)));
                trh_star = Damage / 2;
                printf ("%3u\t", trh_star);
            }
            printf("\n");
        }
        for (int ii = 20; ii <= 100; ii += 10) {
            printf ("%5u\t", ii);
            for (int jj = 2; jj < h; jj *= 2) {
                Damage = abs((1.0 / ii + 1.0 / (ii * jj)) * log(pow(10, -18))
                             / log(1.0 - 1.0 / (ii * jj)));
                trh_star = Damage / 2;
                printf ("%3u\t", trh_star);
            }
            printf("\n");
        }
        for (int ii = 200; ii <= 1000; ii += 100) {
            printf ("%5u\t", ii);
            for (int jj = 2; jj < h; jj *= 2) {
                Damage = abs((1.0 / ii + 1.0 / (ii * jj)) * log(pow(10, -18))
                             / log(1.0 - 1.0 / (ii * jj)));
                trh_star = Damage / 2;
                printf ("%3u\t", trh_star);
            }
            printf("\n");
        }
        for (int ii = 2000; ii <= 10000; ii += 1000) {
            printf ("%5u\t", ii);
            for (int jj = 2; jj < h; jj *= 2) {
                Damage = abs((1.0 / ii + 1.0 / (ii * jj)) * log(pow(10, -18))
                             / log(1.0 - 1.0 / (ii * jj)));
                trh_star = Damage / 2;
                printf ("%3u\t", trh_star);
            }
            printf("\n");
        }
    }

    if(analyze_double_sided_attack){
        for (int ii = 1; ii <= MAX_BLAST_RADIUS; ++ii) {
            double Damage = abs(72 * log(pow(10, -18)) / (2 * log(2 * ii - 1) - 3 * log(2 * ii)));
            uns trh_star = Damage / (2 * ii);
            printf ("%u\t%u\n", ii, trh_star);
        }
        return 0;
    }

    return 0;
}