#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)
#include <climits>
#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here
void findDepthRange(Node* root, int depth, 
  int& min_depth, int& max_depth);

bool equalPaths(Node * root)
{
    // Add your code below
    if (root == nullptr) return true;

    int min_depth = INT_MAX;
    int max_depth = INT_MIN;

    findDepthRange(root, 0, min_depth, max_depth);

    return (min_depth == max_depth);

}

void findDepthRange(Node* root, int depth, int& min_depth, int& max_depth) {
  if (root == nullptr) return;

  // check if leaf
  if (root -> left == nullptr && root -> right == nullptr) {
    if (depth < min_depth) min_depth = depth;
    if (depth > max_depth) max_depth = depth;
    return;
  }

  // call on children if not leaf
  findDepthRange(root -> left, depth + 1, min_depth, max_depth);
  findDepthRange(root -> right, depth + 1, min_depth, max_depth);
}

