#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;


void print(vector<int> &v) {
    for (auto i : v) {
        cout << i << " ";
    }
    cout << endl;
}

int main() {
    int arr[] = {1, 6, 3, 2, 5};

    vector<int> vec(arr, arr + 5);
    cout << "begin(): " << *vec.begin() << endl;

    // 排序
    sort(vec.begin(), vec.end());
    print(vec);



    return 0;
}