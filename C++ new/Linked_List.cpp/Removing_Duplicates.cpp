#include<iostream>
using namespace std;
struct Node{
int data;
struct Node *next;
}*first;
void create(int A[],int n){
    first=new Node;
    Node *t,*last;
    first->data=A[0];
    first->next=NULL;
    last=first;
    for(int i=1;i<n;i++){
        t=new Node;
        t->data=A[i];
        t->next=NULL;
        last->next=t;
        last=t;
    }

}
void Remove(struct Node *p){
    p=first;
    struct Node *q=p->next;
    while(q!=NULL){
        if(p->data!=q->data){
            p=q;
            q=q->next;
        }
        else {
            p->next=q->next;
            free(q);
            q=p->next;
        }
    }
}
void display(struct Node *p){
    while(p!=NULL){
        cout<<p->data<<" ";
        p=p->next;
    }
}

int main()
{
int A[]={1,2,4,5,6,6,6,7,8,9,9};
create(A,11);
Remove(first);
display(first);
}