#include <iostream>
#include <bits/stdc++.h>
using namespace std;
void Insert(int A[], int n)
{
    int i = n;
    int temp = A[n];
    while (i > 1 && temp > A[i / 2])
    {
        A[i] = A[i / 2];
        i = i / 2;
    }
    A[i] = temp;
}
int Delete(int A[], int n)
{
    int i, j, x;
    int val = A[1];
    x = A[n];
    A[1] = A[n];
    i = 1;
    j = 2 * i;
    while (j < n - 1)
    {
        if (A[j] < A[j + 1])
        {
            j = j + 1;
        }
        if (A[i] < A[j])
        {
            swap(A[i], A[j]);
            i = j;
            j = 2 * j;
        }
        else
        {
            break;
        }
    }
    A[n] = val;
    return val;
}

void display(int A[], int n)
{
    for (int i = 1; i < n; i++)
    {
        cout << A[i] << " ";
    }
    cout << endl;
}
int main()
{
    int H[] = {0, 10, 20, 30, 25, 5, 40, 35};

    for (int i = 2; i < 8; i++)
    {
        Insert(H, i);
    }
    display(H, 8);

  //  cout << "Deleted value is " << Delete(H, 7) << " " << endl;
   // cout << "Deleted value is " << Delete(H, 6) << " " << endl;
   
    // HEAP SORT
    for(int i=7;i>1;i--){
        Delete(H,i);
    }
    cout<<"______________HEAP SORT_____________________"<<endl;
 display(H, 8);

}