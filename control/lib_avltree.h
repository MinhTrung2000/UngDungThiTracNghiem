#ifndef LIB_AVLTREE_H
#define LIB_AVLTREE_H

using namespace std;

template <typename T>
class AVLTree {
private:
  struct Node {
    T key;
    int size;
    int height;

    Node *left, *right, *parent;

		Node() : key(T()), size(1), height(1), left(NULL), right(NULL), parent(NULL) {}

		Node(const T& k, int s, int h, Node *l, Node *r, Node *p)
			: key(k), size(s), height(h), left(l), right(r), parent(p) {}
  };

	Node *root;

	//The height of left subtree and right subtree can differ by at most 1
	//The heigth of an empty tree is defined to be -1
	int height(Node *p) {
		return (p == NULL ? 1 : p->height);
	}

	int size(Node *p) {
		return (p == NULL ? 1 : p->size);
	}

	void update(Node *p) {
		assert(p != NULL);
		p->size = 1 + size(p->left) + size(p->right);
		p->height = 1 + std::max(height(p->left), height(p->right));
		if (p->left != NULL)
			p->left->parent = p;
		if (p->right != NULL)
			p->right->parent = p;
	}

	Node *rotateLeft(Node *p) {
		assert(p != NULL);
		assert(p->right != NULL);
		Node *rightP = p->right;
		p->right = rightP->left;
		rightP->left = p;
		update(p);
		update(rightP);
		return rightP;
	}

	Node *rotateRight(Node *p) {
		assert(p != NULL);
		assert(p->left != NULL);
		Node *leftP = p->left;
		p->left = leftP->right;
		leftP->right = p;
		update(p);
		update(leftP);
		return leftP;
	}

	Node *rebalance(Node *p) {
		if (p == NULL) return NULL;
		update(p);
		if (height(p->left) - height(p->right) > 1) {
			if (height(p->left->left) < height(p->left->right))
				p->left = rotateLeft(p->left);
			p = rotateRight(p);
		} else if (height(p->right) - height(p->left) > 1) {
			if (height(p->right->right) < height(p->right->left))
				p->right = rotateRight(p->right);
			p = rotateLeft(p);
		}
		assert(abs(height(p->right) - height(p->left)) <= 1);
		return p;
	}

  //insert Node p at Node x
	Node *insert(Node *p, const T& v) {
		if (p == NULL) {
			p = new Node(v, 1, 1, NULL, NULL, NULL);
			return p;
		} else if (v < p->key) {
			p->left = insert(p->left, v);
		} else {
			p->right = insert(p->right, v);
		}
		return rebalance(p);
	}

	Node *minimum(Node *p) {
		if (p != NULL) {
			while (p->left != NULL) p = p->left;
		}
		return p;
	}

	Node *maximum(Node *p) {
		if (p != NULL) {
			while (p->right != NULL) p = p->right;
		}
		return p;
	}

	Node *remove(Node *p, const T& v) {
		if (p == NULL) return NULL;
		if (v < p->key) {
			p->left =  remove(p->left, v);
		} else if (v > p->key) {
			p->right = remove(p->right, v);
		} else if (p->left != NULL && p->right != NULL) {
			p->key = minimum(p->right)->key;
			p->right = remove(p->right, p->key);
		} else {
			Node *oldNode = p;
			p = (p->left != NULL ? p->left : p->right);
			delete oldNode;
			return p;
		}
		return rebalance(p);
	}

	Node *successor(Node *p) {
		if (p->right != NULL) return minimum(p->right);
    while (1) {
      Node *t = p->parent;
      if (t == NULL || p == t->left) return t;
      p = t;
    }
	}

	Node *find(const T &v) const {
		Node *p = root;
		while (p != NULL) {
			if (p->key == v) return p;
			if (p->key > v)
				p = p->left;
			else
				p = p->right;
		}
		return NULL;
	}

	//k : 0-base
	Node *kth(int k) const {
		if (k >= root->size) return NULL;
		Node *p = root;
		while (1) {
			assert(p != NULL);
			if (p->left->size == k) return p;
			if (p->left->size > k)
				p = p->left;
			else {
				k -= (p->left->size + 1);
				p = p->right;
			}
		}
	}

	Node *clone(Node *p) {
		if (p == NULL) return NULL;
		Node *ret = new Node(p->key, p->height, p->size, clone(p->left), clone(p->right), NULL);
		update(ret);
		return ret;
	}

	void clear(Node *p) {
		if (p == NULL) return;
		clear(p->left);
		clear(p->right);
		delete p;
	}

public:
  AVLTree() : root(NULL) {}

	AVLTree(const AVLTree& rhs) : root(NULL) {
		root = clone(rhs.root);
	}

	~AVLTree() {
	  clear();
  }

	bool empty() const { return root == NULL; }

	void clear() {
		clear(root);
		root = NULL;
	}

	bool contains(const T& v) {
		return find(v) != NULL;
	}

  //insert new Node with key(v)
	void insert(const T& v) {
		root = insert(root, v);
		root->parent = NULL;
	}

	void erase(const T& v) {
		root = remove(root, v);
	}

	T minValue() { return minimum(root)->key; }
	T maxValue() { return maximum(root)->key; }

	int rank(Node *p) const {
		if (p == NULL) return root->size;
    Node *t = p->left;
    int k = t->size;
    while (p != NULL) {
      if (p->left != t)
        k += (p->left->size + 1);
      t = p;
      p = p->parent;
    }
    return k;
	}

	void inorderTraverse(Node *p) {
		if (p == NULL) return;
		inorderTraverse(p->left);
		std::cout << p->key << ' ';
		inorderTraverse(p->right);
	}
	void inorderTraverse() { inorderTraverse(root); std::cout << '\n'; }

	AVLTree& operator = (const AVLTree& rhs) {
		clear();
		root = NULL;
		root = clone(rhs.root);
		return *this;
	}

	//-------------------- DEBUG -------------
	void debug(std::string prefix, Node *p, bool isLeft) {
		if (p == NULL) return;
		std::cerr << prefix << (isLeft ? "|--" : "'--") << p->key << '\n';
		debug(prefix + (isLeft ? "|  " : "   "), p->left, true);
		debug(prefix + (isLeft ? "|  " : "   "), p->right, false);
	}

	void debug() { debug("", root, true); }

	//------------------------------------------
};

// STL-like interface for AVLTree
//template<class T>
//class AVLSet : private AVLTree<T> {
//	typedef AVLNode<T> Node;
//
//public:
//	struct iterator {
//		typedef forward_iterator_tag iterator_category;
//		typedef T value_type;
//		typedef T &reference;
//		typedef T *pointer;
//		typedef ptrdiff_t difference_type;
//		Node *x;
//		iterator(Node *x_) : x(x_) {}
//		bool operator == (const iterator &i) const { return x == i.x; }
//		bool operator != (const iterator &i) const { return x != i.x; }
//		T &operator *() const { return x->key; }
//		void operator ++() { x = AVLTree<T>::successor(x); }
//	};
//
//	iterator begin() const { return iterator(AVLTree<T>::minimum(AVLTree<T>::root)); }
//	iterator end() const { return iterator(&AVLTree<T>::nil); }
//	iterator insert(const T &key) { return iterator(AVLTree<T>::insert(key)); }
//	iterator find(const T &key) const { return iterator(AVLTree<T>::find(key)); }
//	void erase(const T &key) { AVLTree<T>::erase(key); }
//	void erase(iterator it) { AVLTree<T>::erase(it.x->key); }
//	int size() const { return AVLTree<T>::root->size; }
//	bool empty() const { return size() == 0; }
//	int rank(iterator it) const { return AVLTree<T>::rank(it.x); }
//	iterator kth(int k) const { return iterator(AVLTree<T>::kth(k)); }
//};

#endif
