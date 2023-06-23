//
// Created by sianh on 2023/6/23.
//

#include "bintree.h"

#ifndef LEARN_CPP_DSA_BALANCEDBINARYTREE_H_
#define LEARN_CPP_DSA_BALANCEDBINARYTREE_H_

namespace sj {
template<typename T>
class BST : public BinTree<T> {

protected:
    typedef T value_type;
    typedef typename BinTree<T>::node_pointer node_pointer;

protected:
    node_pointer _hot;
    node_pointer connect34(node_pointer, node_pointer, node_pointer,
                           node_pointer, node_pointer, node_pointer, node_pointer);
    node_pointer rotateAt(node_pointer x);

public:
    virtual node_pointer &search(const T &e) {
        return searchIn(this->_root, e, _hot = nullptr);
    }
    virtual node_pointer insert(const T &e) {
        node_pointer &x = search(e);
        if (x) return x;
        x = new BinNode<T>(e, _hot);
        this->_size++;
        this->updateHeightAbove(x);
        return x;
    }
    virtual bool remove(const T &e) {
        node_pointer &x = search(e);
        if (!x) return false;
        removeAt(x, _hot);
        _size--;
        updateHeightAbove(_hot);
        return true;
    }

private:
    node_pointer &searchIn(node_pointer &v, const T &e, node_pointer &hot) {
        if (!v || (e == v->data)) return v;
        hot = v;
        return searchIn((e < v->data) ? v->left_child : v->right_child, e, hot);
    }

    node_pointer removeAt(node_pointer &x, node_pointer &hot) {
        node_pointer w = x;
        node_pointer succ = nullptr;
        if (!HasLChild(*x)) {
            succ = x = x->right_child;
        } else if (!HasRChild(*x)) {
            succ = x = x->left_child;
        } else {
            w = w->succ();
            std::swap(x->data, w->data);
            node_pointer u = w->parent;
            ((u == x) ? u->right_child : u->left_child) = succ = w->right_child;
        }
        hot = w->parent;
        if (succ) succ->parent = hot;
        return succ;
    }
};
}

#endif //LEARN_CPP_DSA_BALANCEDBINARYTREE_H_
