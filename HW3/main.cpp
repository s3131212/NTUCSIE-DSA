#include <iostream>
#include <unordered_map>
#include <string.h>
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))
#define INF 256
 
using namespace std;
bool debug = false;
unordered_map<unsigned long long int, int> gameState;
int cards[2][20];
int amountOfCardsPerPlayer;
 
inline int card2int(char c[2]){
    int k = 0;
 
    if(c[0] == 'C') k = 0;
    else if(c[0] == 'D') k = 13;
    else if(c[0] == 'H') k = 26;
    else if(c[0] == 'S') k = 39;
 
    if(c[1] == 'A') k += 1;
    else if(c[1] == 'T') k += 10;
    else if(c[1] == 'J') k += 11;
    else if(c[1] == 'Q') k += 12;
    else if(c[1] == 'K') k += 13;
    else k += c[1] - '0';
 
    return k;
}
inline int getNumber(int v){
    return ((v % 13 != 0) ? (v % 13) : 13);
}
inline int getSuit(int v){
    return ((v - 1) / 13);
}
inline char getSuitReadable(int v){
    if(v == 0) return 'N';
    if(((v - 1) / 13) == 0) return 'C';
    if(((v - 1) / 13) == 1) return 'D';
    if(((v - 1) / 13) == 2) return 'H';
    if(((v - 1) / 13) == 3) return 'S';
    return 'N';
}
int getPoint(unsigned long long int availableCards){  // from viewpoint of Alice
    int sum = 0;
    if((((1 << amountOfCardsPerPlayer) - 1) & (availableCards >> (0))) == 0){ // Alice win
        for(int i = 0; i < amountOfCardsPerPlayer; i++) {
            if (((availableCards >> (i+17)) & 1)) {
                sum += getNumber(cards[1][i]);
            }
        }
        return sum;
    }else{ // Alice lose
        for(int i = 0; i < amountOfCardsPerPlayer; i++) {
            if (((availableCards >> i) & 1)) {
                sum += getNumber(cards[0][i]);
            }
        }
        return -sum;
    }
}
 
int minimax(unsigned long long int availableCards, int alpha, int beta, int player, int prevCard){
    // game ended
    if((((1 << amountOfCardsPerPlayer) - 1) & (availableCards >> (0))) == 0 || (((1 << amountOfCardsPerPlayer) - 1) & (availableCards >> (17))) == 0)
        return getPoint(availableCards);
 
    // If already calculate
    unsigned long long int gameStateValue = availableCards
                                           + ((unsigned long long int)player << 34)
                                           + ((unsigned long long int)alpha << 35)
                                           + ((unsigned long long int)beta << 44)
                                           + ((unsigned long long int)prevCard << 53);
    auto search = gameState.find(gameStateValue);
    if(search != gameState.end()){
        return search->second;
    }
 
    // reach max node
    if(player == 0){
        int maxValue = -INF;
        int re;
 
        int currentCheckCard;
        bool hasCard = false;
        for(int i = 0; i < amountOfCardsPerPlayer; i++) {
            if (!((availableCards >> i) & 1)) continue;
 
            currentCheckCard = cards[0][i];
 
            if (prevCard == 0 || getNumber(currentCheckCard) == getNumber(prevCard) || getSuit(currentCheckCard) == getSuit(prevCard)){
                if(debug) cout << currentCheckCard << " " << getSuitReadable(currentCheckCard) << getNumber(currentCheckCard) << endl;
                re = minimax(availableCards ^ (1ULL << i), alpha, beta, 1, currentCheckCard);
                hasCard = true;
                maxValue = MAX(maxValue, re);
                alpha = MAX(alpha, re);
                if(beta <= alpha) break;
            }
        }
        if(!hasCard){
            if(debug) cout << "PASS" << endl;
            re = minimax(availableCards, alpha, beta, 1, 0);
            maxValue = MAX(maxValue, re);
            alpha = MAX(alpha, re);
        }
        if(debug) cout << endl;
        gameState[gameStateValue] = maxValue;
 
        return maxValue;
    }else{
        int minValue = INF;
        int re;
 
        int currentCheckCard;
        bool hasCard = false;
        for(int i = 0; i < amountOfCardsPerPlayer; i++) {
            if (!((availableCards >> (i+17)) & 1)) continue;
 
            currentCheckCard = cards[1][i];
 
            if (prevCard == 0 || getNumber(currentCheckCard) == getNumber(prevCard) || getSuit(currentCheckCard) == getSuit(prevCard)){
                if(debug) cout << currentCheckCard << " " << getSuitReadable(currentCheckCard) << getNumber(currentCheckCard) << endl;
                re = minimax(availableCards ^ (1ULL << (i+17)), alpha, beta, 0, currentCheckCard);
                hasCard = true;
                minValue = MIN(minValue, re);
                beta = MIN(beta, re);
                if(beta <= alpha) break;
            }
        }
        if(!hasCard){
            if(debug) cout << "PASS" << endl;
            re = minimax(availableCards, alpha, beta, 0, 0);
            minValue = MIN(minValue, re);
            beta = MIN(beta, re);
        }
        if(debug) cout << endl;
        gameState[gameStateValue] = minValue;
 
        return minValue;
    }
}
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);
 
    char cardtemp[3];
 
    cin >> amountOfCardsPerPlayer;
 
    //Alice
    for(int i = 0; i < amountOfCardsPerPlayer; i++){
        cin >> cardtemp;
        cards[0][i] = card2int(cardtemp);
    }
    //Bob
    for(int i = 0; i < amountOfCardsPerPlayer; i++){
        cin >> cardtemp;
        cards[1][i] = card2int(cardtemp);
    }
 
    if(debug) cout << "start calculation" << endl;
 
    int result = minimax(17179869183, -INF, INF, 0, 0);
    if(result > 0){
        cout << "Alice" << "\n";
        cout << result;
    }else{
        cout << "Bob" << "\n";
        cout << (-result);
    }
}