#include <stdio.h>
#include <stdlib.h>
#include <string.h>
const int MOD = 1000000007;
bool debug = false;
bool** paper;
int** C;
long long dp[11][11][11][11][101];

unsigned long long solve(int L1, int R1, int L2, int R2, int K){
    if(dp[L1][R1][L2][R2][K] != -1)
        return dp[L1][R1][L2][R2][K];

    // Base case
    if(K <= 0){
        if(debug) printf("L1:%d, R1:%d, L2:%d, R2:%d, K:%d, sol:1\n", L1, R1, L2, R2, K);
        dp[L1][R1][L2][R2][K] = 1;
        return 1;
    }
    if(L1 == R1 || L2 == R2){
        if(debug) printf("L1:%d, R1:%d, L2:%d, R2:%d, K:%d, sol:0\n", L1, R1, L2, R2, K);
        dp[L1][R1][L2][R2][K] = 0;
        return 0;
    }
    if(debug) printf("L1:%d, R1:%d, L2:%d, R2:%d, K:%d start\n", L1, R1, L2, R2, K);

    unsigned long long sol = 0;
    // column
    if(R1 - L1 > 1) {
        for (int i = L1; i < R1; i++) {
            bool ok = true;
            for (int j = L2; j < R2; j++) {
                if (!paper[j][i]) ok = false;
            }
            if (ok) {
                int ka = 0;
                unsigned long long pre = 1;
                /*
                while(pre != 0) {
                    printf("hi");
                    pre = (solve(L1, i, L2, R2, ka) * solve(i + 1, R1, L2, R2, K-1-ka) * C[K-1][ka]);
                    sol += pre;
                    ka ++;
                }
                 */
                while (K - 1 - ka >= 0) {
                    pre = ((solve(L1, i, L2, R2, ka) * solve(i + 1, R1, L2, R2, K - 1 - ka) % MOD) * C[K - 1][ka]) % MOD;
                    sol += pre;
                    ka++;
                }
            }
        }
    }

    // row
    if(R2 - L2 > 1) {
        for (int i = L2; i < R2; i++) {
            bool ok = true;
            for (int j = L1; j < R1; j++) {
                if (!paper[i][j]) ok = false;
            }
            if (ok) {
                int ka = 0;
                unsigned long long pre = 1;
                /*
                while(pre != 0) {
                    printf("hi");
                    pre = (solve(L1, R1, L2, i, ka) * solve(L1, R1, i + 1, R2, K-1-ka) * C[K-1][ka]);
                    sol += pre;
                    ka ++;
                }*/
                while (K - 1 - ka >= 0) {
                    pre = ((solve(L1, R1, L2, i, ka) * solve(L1, R1, i + 1, R2, K - 1 - ka) % MOD ) * C[K - 1][ka]) % MOD;
                    sol += pre;
                    ka++;
                }
            }
        }
    }
    sol %= MOD;
    dp[L1][R1][L2][R2][K] = sol;
    if(debug) printf("L1:%d, R1:%d, L2:%d, R2:%d, K:%d, sol:%llu\n", L1, R1, L2, R2, K, sol);
    return sol;
}
void calculate_combination(int n){
    C = (int**)malloc(sizeof(int*) * (n + 1));
    for(int i = 0; i <= n; i++){
        C[i] = (int*)malloc(sizeof(int) * (n + 1));
        for(int j = 0; j <= i; j++){
            if(i == j || j == 0)
                C[i][j] = 1;
            else
                C[i][j] = (C[i - 1][j] + C[i - 1][j - 1]) % MOD;
        }
    }
}
int main(){
    int n, m;
    scanf("%d %d", &n, &m);
    calculate_combination(n * m);
    memset(dp, -1, sizeof(dp));

    // Input
    paper = (bool**)malloc(sizeof(bool*) * n);
    char temp[m + 1];
    for(int i = 0; i < n; i++){
        paper[i] = (bool*)malloc(sizeof(bool) * m);
        scanf("%s", temp);
        for(int j = 0; j < m; j++){
            paper[i][j] = (temp[j] == 'O');
        }
    }

    // Solve
    unsigned long long sol = 0, pre = 1, k = 0;

    while(pre != 0){
        pre = solve(0, m, 0, n, k);
        if(debug) printf("%llu: %llu\n", k, pre);
        sol += pre;
        sol %= MOD;
        k++;
    }
    printf("%llu", sol);
}