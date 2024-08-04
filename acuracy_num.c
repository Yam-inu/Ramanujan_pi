// gcc acuracy_num.c -o acuracy_num -lgmp -fopenmp && ./acuracy_num
#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>

#define N 1000001 // 読み込み桁数
int INIT_BIT = N*3.4;

char fname_calc[] = "calc_pi.txt";
char fname_correct[] = "correct_pi.txt";

int main(){
    FILE *fp_calc,*fp_correct; // FILE型構造体
    
	char str_calc[N];
	char str_correct[N];

    // ファイルを開く。失敗するとNULLを返す。
	fp_calc = fopen(fname_calc, "r");
	fp_correct = fopen(fname_correct, "r");

	if(fp_calc == NULL) {
		printf("%s file not open!\n", fname_calc);
		return -1;
	}

    if(fp_correct == NULL) {
		printf("%s file not open!\n", fname_correct);
		return -1;
	}
    
	fgets(str_calc, N, fp_calc);
    fgets(str_correct, N, fp_correct);

    mpf_t correct_num, calc_num, diff;
    mpf_init2(correct_num, INIT_BIT);  //正確な値
    mpf_init2(calc_num, INIT_BIT);     //計算値
    mpf_init2(diff, INIT_BIT);         //計算誤差

    mpf_set_str(calc_num,str_calc,10);
    mpf_set_str(correct_num,str_correct,10);

    if(mpf_cmp(correct_num, calc_num) > 0)  //correct_num > calc_num
        mpf_sub(diff, correct_num, calc_num);
    else
        mpf_sub(diff, calc_num, correct_num);

    // gmp_printf("diff: %.100000Ff\n", diff);

    int count_digit = 0;
    while(1){
        if(mpf_cmp_ui(diff, 0) == 0){
            count_digit = N;
            break;
        }
        if(mpf_cmp_ui(diff, 1) > 0) break;
        mpf_mul_ui(diff, diff, 10);
        count_digit++;
    }

    printf("精度: 小数点以下%d桁\n", count_digit-1);
    
    mpf_clear(correct_num);
    mpf_clear(calc_num);
    mpf_clear(diff);

    // ファイルを閉じる
	fclose(fp_calc); 
    fclose(fp_correct);
 
	return 0;
}
