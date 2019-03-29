#include <iostream>
#include <deque>
#include <vector>
#include <algorithm>
using namespace std;
int binarysearch(deque<int>& dq, int left, int right, int target){
    if(left > right) return -1;
 
    int mid = left + (right - left) / 2;
    if(dq[mid] == target) return mid;
    if(dq[mid] > target) return binarysearch(dq, left, mid - 1, target);
    if(dq[mid] < target) return binarysearch(dq, mid + 1, right, target);
 
    return 0;
}
void binaryinsert(deque<int>& dq, int left, int right, int insertion){
    if(left > right) return;
 
    int mid = left + (right - left) / 2;
    if(mid != 0 && dq[mid] >= insertion && dq[mid - 1] <= insertion){
        dq.insert(dq.begin() + mid, insertion);
    }else if((mid == 0 && dq[mid] >= insertion)){
        dq.push_front(insertion);
    }else if(mid == dq.size() - 1 && dq[mid] <= insertion){
        dq.push_back(insertion);
    }else if(dq[mid] > insertion){
        binaryinsert(dq, left, mid - 1, insertion);
    }else if(dq[mid] < insertion){
        binaryinsert(dq, mid + 1, right, insertion);
    }
}
int main(int argc, const char * argv[]) {
    ios_base::sync_with_stdio(false);
    cin.tie(0);
 
    int length, window, kth, k;
    deque<int> dq;
    deque<int> int2remove;
    cin >> length >> window >> kth;
 
    int prev;
    for(int i = 0; i < window; i++){
        cin >> k;
        dq.push_back(k);
        int2remove.push_back(k);
    }
    prev = int2remove[0];
    int2remove.pop_front();
    sort(dq.begin(), dq.end());
    cout << dq[kth - 1] << "\n";
 
 
    for(int i = window; i < length; i++){
        /*for(int i = 0; i < dq.size(); i++){
            cout << dq[i] << " ";
        }
        cout << endl << "going to erase: " << prev << "at"
        << (binarysearch(dq, 0, dq.size() - 1, prev)) << endl;
         */
        dq.erase(dq.begin() + binarysearch(dq, 0, dq.size() - 1, prev));
        cin >> k;
        int2remove.push_back(k);
 
        //cout << endl << "going to insert: " << k << endl;
        binaryinsert(dq, 0, dq.size() - 1, k);
        /*for(int i = 0; i < dq.size(); i++){
            cout << dq[i] << " ";
        }
        cout << endl;
        */
        cout << dq[kth - 1] << "\n";
 
 
        prev = int2remove[0];
        int2remove.pop_front();
 
    }
 
}