#include <iostream>
#include <vector>
#include <unordered_map>
using namespace std;

struct TreeNode
{
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};

TreeNode *build(vector<int> &inorder, vector<int> &postorder,
                int inStart, int inEnd, int &postIndex,
                unordered_map<int, int> &inMap)
{
    if (inStart > inEnd)
        return NULL;

    TreeNode *root = new TreeNode(postorder[postIndex--]);

    if (inStart == inEnd)
        return root;

    int inRoot = inMap[root->val];

    root->right = build(inorder, postorder, inRoot + 1, inEnd, postIndex, inMap);
    root->left = build(inorder, postorder, inStart, inRoot - 1, postIndex, inMap);

    return root;
}

TreeNode *buildTree(vector<int> &inorder, vector<int> &postorder)
{
    unordered_map<int, int> inMap;
    for (int i = 0; i < inorder.size(); i++)
    {
        inMap[inorder[i]] = i;
    }
    int postIndex = postorder.size() - 1;
    return build(inorder, postorder, 0, inorder.size() - 1, postIndex, inMap);
}

void preorder(TreeNode *root)
{
    if (!root)
        return;
    cout << root->val << " ";
    preorder(root->left);
    preorder(root->right);
}

int main()
{
    int n;
    cout << "Enter number of nodes: ";
    cin >> n;

    vector<int> inorder(n), postorder(n);

    cout << "Enter inorder traversal: ";
    for (int i = 0; i < n; i++)
        cin >> inorder[i];

    cout << "Enter postorder traversal: ";
    for (int i = 0; i < n; i++)
        cin >> postorder[i];

    TreeNode *root = buildTree(inorder, postorder);

    cout << "Preorder traversal of constructed tree: ";
    preorder(root);
    cout << endl;

    return 0;
}
