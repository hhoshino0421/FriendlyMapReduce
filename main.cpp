#include <stdio.h>
#include <time.h>




//関数プロトタイプ宣言
int gcd(int u, int v);
void FriendlyNumbers(int start, int end);

// エントリポイント
int main() {

    //テスト用範囲定義
    int start   = 1;
    int end     = 1000;

    //処理時間計測
    clock_t start_clock = clock();

    //メイン処理
    FriendlyNumbers(start, end);

    clock_t end_clock = clock();

    double processTime = (double)end_clock - start_clock;

    printf("処理時間(ミリ秒): %f", processTime);

    return 0;
}

int gcd(int u, int v) {

    if(v == 0) {
        return u;
    }

    return gcd(v, u% v);
}

void FriendlyNumbers(int start, int end) {

    int last        = end - start + 1;
    int *the_num    = new int[last];
    int *num        = new int[last];
    int *den        = new int[last];
//#pragma omp parallel
    {
        int i, j, factor, ii, sum, done, n;

#pragma omp for schedule (dynamic, 16)

        for (i = start; i <= end; i++) {

            ii          = i - start;
            sum         = 1 + i;
            the_num[ii] = i;
            done        = i;
            factor      = 2;

            while (factor < done) {
                if ((i % factor) == 0) {
                    sum += (factor + (i / factor));
                    if ((done = i / factor) == factor) {
                        sum -= factor;
                    }
                }
                factor++;

            }

            num[ii] = sum;
            den[ii] = i;
            n = gcd(num[ii], den[ii]);
            num[ii] /= n;
            den[ii] /= n;

        }   //end for

#pragma omp for schedule (static, 8)

        for (i = 0; i < last; i++) {
            for (j = i + 1; j < last; j++) {
                if ((num[i] == num[j]) && (den[i] == den[j])) {
                    printf("%d and %d are FRIENDLY \n", the_num[i], the_num[j]);
                }
            }
        }
    }

    //後始末
    delete[] the_num;
    delete[] num;
    delete[] den;

}