#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
    TreeMap * new=(TreeMap *)malloc(sizeof(TreeMap));
    if(new==NULL) exit(EXIT_FAILURE);
    new->root=NULL;
    new->current=NULL;
    new->lower_than = lower_than;
    return new;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {
    if(tree==NULL) exit(EXIT_FAILURE);
    if(tree->root==NULL){
        tree->root=createTreeNode(key,value);
        return; 
    }
    tree->current=tree->root;
    TreeNode* new=createTreeNode(key,value);
    if(new==NULL) exit(EXIT_FAILURE);
    while(tree->current!=NULL){
        if(tree->lower_than(key,tree->current->pair->key)==1){
            if(tree->current->left==NULL){
                tree->current->left=new;
                new->parent=tree->current;
                tree->current=new;
                return;
            }
            else tree->current=tree->current->left;
        }
        else if(tree->lower_than(tree->current->pair->key,key)==1){
            if(tree->current->right==NULL){
                tree->current->right=new;
                new->parent=tree->current;
                tree->current=new;
                return;
            }
            else tree->current=tree->current->right;
        }
        else{
            free(new->pair);
            free(new);
            return;
        }
    }
}

TreeNode * minimum(TreeNode * x){
    if(x==NULL) return NULL;
    if(x->left==NULL) return x;
    while(x->left!=NULL){
        x=x->left;
    }
    return x;
}


void removeNode(TreeMap * tree, TreeNode* node) {
    if(node==NULL) return;
    if(node->left==NULL && node->right==NULL){
        if(node->parent==NULL) tree->root=NULL;
        else if(node->parent->left==node) node->parent->left=NULL;
        else node->parent->right=NULL;
        free(node->pair);
        free(node);
    }
    else if(node->left==NULL || node->right==NULL){
        if(node->left!=NULL){
            if(node->parent==NULL)tree->root=node->left;
            else if(node->parent->left==node)node->parent->left=node->left;
            else node->parent->right=node->left;
            node->left->parent = node->parent;
        }
        else{
            if(node->parent==NULL)tree->root=node->right;
            else if(node->parent->left==node)node->parent->left=node->right;
            else node->parent->right=node->right;
            node->right->parent = node->parent;
        }
        free(node->pair);
        free(node);
    }
    else{
        TreeNode* min=minimum(node->right);
        node->pair->key=min->pair->key;
        node->pair->value=min->pair->value;
        removeNode(tree,min);
    }
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}




Pair * searchTreeMap(TreeMap * tree, void* key) {
    if(tree==NULL|| tree->root==NULL) return NULL;
    tree->current=tree->root;
    while(tree->current!=NULL){
        if(tree->lower_than(key,tree->current->pair->key)==1) tree->current=tree->current->left;
        else if(tree->lower_than(tree->current->pair->key,key)==1) tree->current=tree->current->right;
        else return tree->current->pair;
    }
    return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) {
    return NULL;
}

Pair * firstTreeMap(TreeMap * tree) {
    if(tree==NULL || tree->root==NULL) return NULL;
    if(tree->root->left==NULL) return tree->root->pair;
    tree->current=tree->root;
    while(tree->current->left!=NULL){
        tree->current=tree->current->left;
    }
    return tree->current->pair;
}

Pair * nextTreeMap(TreeMap * tree) {
    if(tree==NULL || tree->current==NULL) return NULL;
    if(tree->current->right!=NULL){
        tree->current=tree->current->right;
        while(tree->current->left!=NULL){
            tree->current=tree->current->left;
        }
        return tree->current->pair;
    }
    else{
        TreeNode* parent=tree->current->parent;
        while(parent!=NULL && parent->right==tree->current){
            tree->current=parent;
            parent=parent->parent;
        }
        tree->current = parent;
        if (parent != NULL)
        return tree->current->pair;
    }
    return NULL;
}
