#include<iostream>
#include<queue>
using namespace std;
struct Node{
    int data;
    Node * lchild;
    Node *rchild;
}*root=NULL;
void createTree(){
    root = new Node;
    queue<Node*> q;
    int x;
    cout<<"Enter root value ";
    cin>>x;
    root->data=x;
    root->lchild=root->rchild=NULL;
    q.push(root);
    while(!q.empty()){
    Node *temp = q.front();
    q.pop();
    cout<<"Enter left child of "<<temp->data<<" ";
    cin>>x;
    if(x!=-1){
        Node *t = new Node;
        t->data=x;
        t->lchild=t->rchild=NULL;
        temp->lchild=t;
        q.push(t);
    }
    cout<<"Enter right child of "<<temp->data<<" ";
    cin>>x; 
    if(x!=-1){
        Node *t = new Node;
        t->data=x;
        t->lchild=t->rchild=NULL;
        temp->rchild=t;
        q.push(t);
    }
    }

}
void Preorder(Node *p){
    if(p){
        cout<<p->data<<" ";
        Preorder(p->lchild);
        Preorder(p->rchild);
    }
}
void Inorder(Node *p){
    if(p){
        Inorder(p->lchild);
        cout<<p->data<<" ";
        Inorder(p->rchild);
    }
}
void Postorder(Node *p){
    if(p){
        Postorder(p->lchild);
        Postorder(p->rchild);
        cout<<p->data<<" ";
    }
}
void Level_Order_Traversal(Node *p){
    queue<Node *> q;
    q.push(root);
    while(!q.empty()){
        int size = q.size();
        for(int i=0;i<size;i++){
            Node *temp = q.front();
            q.pop();
            cout<<temp->data<<" ";
            if(temp->lchild) q.push(temp->lchild);
            if(temp->rchild) q.push(temp->rchild);

        }
    }
}
int main()
{
createTree();
cout<<"Preorder is ";
Preorder(root);
cout<<"\nInorder is ";
Inorder(root);
cout<<"\nPostorder is ";
Postorder(root);
cout<<"\nLevel Order Traversal is ";
Level_Order_Traversal(root);
}

