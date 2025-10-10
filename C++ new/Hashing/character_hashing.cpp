#include<iostream>
using namespace std;
int main(){
    string s;
    char c;
    cin>>s;
    cout<<"Enter the char to search for"<<endl;
    cin>>c;
  /*  int H[26] ={0};
    for(int i=0;i<s.size();i++){
        H[s[i] - 'a']++;
    }
    cout<<H[c-'a'];  
    IF only lowercase letters are present 
    */ 
   // IF Uppercase letters are also present
   int H[256]={0};
   for(int i=0;i<s.size();i++){
    H[s[i]]++;
   }
   cout<<H[c];

}