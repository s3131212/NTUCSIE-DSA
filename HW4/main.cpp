#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
using namespace std;
const int K = 29;
bool debug = false;
uint64_t power(uint64_t x, uint64_t y){
    uint64_t res = 1;

    while (y > 0) {
        if (y & 1) // if y is odd
            res = (res * x);

        y = y >> 1; // y = y/2
        x = (x * x);
    }
    return res;
}
void calculate_base_hash(string &str, vector<uint64_t> &base_hash){
    uint64_t hash_value = 0;
    for(int i = 0; i < str.length() ; i++){
        hash_value = ((hash_value * K) + (str[i] - 'a' + 1));
        base_hash[i] = hash_value;
    }
    if(debug) {
        for (int i = 0; i < str.length(); i++) {
            cout << base_hash[i] << " ";
        }
        cout << endl;
    }
}
uint64_t calculate_between_hash(int L, int R, vector<uint64_t> &base_hash){
    if(R < L) return 0;
    if(L == 0) return base_hash[R];
    //Hash( s[ L ] ~ s[ R ] ) = H( R ) - H( L - 1 ) * K^( R - L + 1 )
    return (base_hash[R] - (base_hash[L - 1] * power(K, R - L + 1)));
}
void problem1(int m, string &str, vector<uint64_t> &base_hash){
    int left = 0, right = str.length(), mid, rightmost = -1, rightmostlen = 0;
    uint64_t rh;
    bool hasfound = false;
    while(left <= right){
        unordered_map<uint64_t, int> counter;
        bool hasfoundinloop = false;
        mid = left + ((right - left + 1) >> 1);
        if(debug) cout << "Now checking length: " << mid << ", left = " << left << ",right: " << right << endl;
        for(int i = 0; i < str.length() - mid + 1; i++){
            rh = calculate_between_hash(i, i + mid - 1, base_hash);
            counter[rh]++;
            if(debug) cout << i << " " << (i+mid-1) << " " << rh << " " << counter[rh] << endl;
            if(counter[rh] >= m){
                hasfoundinloop = true;
                hasfound = true;
                if(mid >= rightmostlen){
                    rightmostlen = mid;
                    rightmost = i;
                }
            }
        }
        if(left == right) break;
        if(hasfoundinloop)
            left = mid;
        else
            right = mid - 1;
    }
    if(hasfound && rightmostlen != 0){
        cout << rightmostlen << " " << rightmost << "\n";
    }else{
        cout << "none" << "\n";
    }

}

void problem2(string &str, vector<uint64_t> &base_hash) {
    vector<int> possible_answer;
    for(int i = 1; i <= str.length() - 1; i++){
        if(calculate_between_hash(0, i - 1,base_hash) == calculate_between_hash(str.length() - i, str.length() - 1,base_hash)){
            possible_answer.push_back(i);
            if(debug) cout << i << " ";
        }
    }
    if(debug) cout << endl;

    bool hasfound = false;
    int left = 0, right = possible_answer.size() - 1, mid;
    while(left <= right){
        hasfound = false;
        mid = left + ((right - left + 1) >> 1);
        uint64_t rh = calculate_between_hash(0, possible_answer[mid]-1, base_hash);
        if(debug) cout << "now searching " << possible_answer[mid] << ", left " << left << ", right " << right << ", rh " << rh<< endl;
        for(int i = 1; i < str.length() - possible_answer[mid]; i++){
            if(debug) cout << i << " " << i + possible_answer[mid] - 1 << " " << calculate_between_hash(i, i + possible_answer[mid] - 1, base_hash) << endl;
            if(rh == calculate_between_hash(i, i + possible_answer[mid] - 1, base_hash)){
                hasfound = true;
                break;
            }
        }
        if(hasfound){
            if(left == right) break;
            left = mid;
        }else{
            right = mid - 1;
        }
    }
    if(hasfound){
        cout << str.substr(0, possible_answer[mid]) << "\n";
    }else{
        cout << "Just a legend\n";
    }
}
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int m;
    string str;
    while(cin >> m){
        if(m == 0) break;
        vector<uint64_t> base_hash;
        cin >> str;
        base_hash.reserve(str.length());
        calculate_base_hash(str, base_hash);
        problem1(m, str, base_hash);
        problem2(str, base_hash);
    }

    return 0;
}