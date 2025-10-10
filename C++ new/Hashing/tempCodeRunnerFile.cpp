#include <iostream>
#include <bits/stdc++.h>
using namespace std;
int main()
{
    map<char, int> mpp; // key -> value pair
                        /*  int n;
                          cin>>n;
                          int A[n];
                          for(int i=0;i<n;i++){
                              cin>>A[i];
                              mpp[A[i]]++;
                          }
                           for(auto it:mpp){
                              cout<<it.first<<" "<<it.second<<endl;
                          }
                    
                          int m;
                          cout<<"Enter the key to search for"<<endl;
                          cin>>m;
                         cout<<mpp[m];  */
    string s;
    cin >> s;
    for (int i = 0; i < s.size(); i++)
    {
        mpp[s[i]]++;
    }
    for (auto it : mpp)
    {
        cout << it.first << " " << it.second<<endl;
    }
    char c;
    cout << "Enter the char to search for" << endl;
    cin >> c;
    cout<<mpp[c];
    return 0;
}