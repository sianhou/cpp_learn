//
// Created by sianh on 2023/6/18.
//

#ifndef LEARN_CPP_DSA_BINTREE_H_
#define LEARN_CPP_DSA_BINTREE_H_

#include "algorithm"

namespace sj {

#define statue(p) ((p) ? (p)->height : -1)
typedef enum { RED, BLACK } RBColor;

template<class T>
struct BinNode {
    typedef T value_type;
    typedef BinNode<T> self;
    typedef self *node_pointer;

    value_type data;
    node_pointer parent;
    node_pointer left_child;
    node_pointer right_child;
    int height, npl;
    RBColor color;

    BinNode() : parent(nullptr), left_child(nullptr), right_child(nullptr), height(0), npl(1), color(RED) {}
    BinNode(T e, node_pointer p = nullptr, node_pointer lc = nullptr, node_pointer rc = nullptr, int h = 0, int l = 1, RBColor c = RED) :
        data(e), parent(p), left_child(lc), right_child(rc), height(h), npl(l), color(c) {}

    int size();
    node_pointer insertAsLC(const T &e) { return left_child = new self(e); }
    node_pointer insertAsRC(const T &e) { return right_child = new self(e); }
    node_pointer succ();
    template<class VST>
    void travLevel(VST &);
    template<class VST>
    void travPre(VST &);
    template<class VST>
    void travIn(VST &);
    template<class VST>
    void travPost(VST &);

    bool operator<(const self &bn) { return data < bn.data; }
    bool operator==(const self &bn) { return data == bn.data; }
};

template<class T, class VST>
void travPre_R(BinNode<T> *x, VST visit) {
    if (!x) return;
    visit(x->data);
    travPre_R(x->left_child, visit);
    travPre_R(x->right_child, visit);
}

template<class T, class VST>
void travIn_R(BinNode<T> *x, VST visit) {
    if (!x) return;
    travIn_R(x->left_child, visit);
    visit(x->data);
    travIn_R(x->right_child, visit);
}

template<class T, class VST>
void travPost_R(BinNode<T> *x, VST visit) {
    if (!x) return;
    travPost_R(x->left_child, visit);
    travPost_R(x->right_child, visit);
    visit(x->data);
}

// for a node
#define IsRoot(x) (!((x).parent))
#define IsLChild(x) (!IsRoot(x) && (&(x) == (x).parent->left_child))
#define IsRChild(x) (!IsRoot(x) && (&(x) == (x).parent->right_child))
#define HasParent(x) (!IsRoot(x))
#define HasLChild(x) ((x).left_child)
#define HasRChild(x) ((x).right_child)
#define HasChild(x) (HasLChild(x) || HasRChild(x))
#define HasBothChild(x) (HasLChild(x) && HasRChild(x))
#define IsLeaf(x) (!HasChild(x))
#define FromParentTo(x) ( \
    IsRoot(x) ? _root : ( \
    IsLChild(x) ? (x).parent->left_child : (x).parent->right_child \
    )                     \
)

template<class T>
class BinTree {
 protected:
    typedef T value_type;
    typedef BinNode<T> node;
    typedef typename node::node_pointer node_pointer;
    typedef BinTree<T> self;
    typedef self *tree_pointer;

 protected:
    int _size;
    node_pointer _root;
    virtual int updateHeight(node_pointer x) {
        return x->height = 1 + std::max(statue(x->left_child), statue(x->right_child));
    }
    void updateHeightAbove(node_pointer x) {
        while (x) {
            updateHeight(x);
            x = x->parent;
        }
    }

 public:
    BinTree() : _size(0), _root(nullptr) {}
    ~BinTree() {/*TODO(housian)*/}
    int size() const { return _size; }
    bool empty() const { return !_root; }
    node_pointer root() const { return _root; }
    node_pointer insertAsRoot(const T &e);
    node_pointer insertAsLC(node_pointer x, const T &e);
    node_pointer insertAsRC(node_pointer x, const T &e);
    node_pointer attachAsLC(node_pointer x, tree_pointer &T);
    node_pointer attachAsRC(node_pointer x, tree_pointer &T);
    int remove(node_pointer x) {
        FromParentTo(*x) = nullptr;
        updateHeightAbove(x->parent);
        int n = removeAt(x);
        _size -= n;
        return n;
    };
    int removeAt(node_pointer x) {
        if (!x) { return 0; }
        int n = 1 + removeAt(x->left_child) + removeAt(x->right_child);
        return n;
    }
    tree_pointer secede(node_pointer x) {
        FromParentTo(*x) = nullptr;
        updateHeightAbove(x->parent);
        tree_pointer s = new BinTree<T>;
        s->_root = x;
        x->parent = nullptr;
        s->_size = x->size();
        _size -= s->_size;
        return s;
    }
    template<class VST>
    void travLevel(VST &visit) {
        if (_root) {
            _root->travLevel(visit);
        }
    }
    template<class VST>
    void travPre(VST &visit) {
        if (_root) {
            _root->travPre(visit);
        }
    }
    template<class VST>
    void travIn(VST &visit) {
        if (_root) {
            _root->travIn(visit);
        }
    }
    template<class VST>
    void travPost(VST &visit) {
        if (_root) {
            _root->travPost(visit);
        }
    }
    bool operator<(const self &t) { return _root && t._root && (_root->data < t._root->data); }
    bool operator==(const self &t) { return _root && t._root && (_root->data == t._root->data); }
};

}

#endif //LEARN_CPP_DSA_BINTREE_H_
