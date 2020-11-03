#ifndef DSCAUHOITHI_T_MODEL_H_INCLUDED
#define DSCAUHOITHI_T_MODEL_H_INCLUDED

//Type: AVL tree
class DSCauHoiThi_t {
private:
  static DSCauHoiThi_t *danhSachCauHoiThiHienHanh;

  struct Node {
    CauHoiThi key;
    int height;
    Node *left, *right;

		Node() : key(CauHoiThi()), height(0), left(NULL), right(NULL) {}
		Node(const CauHoiThi& k, int h, Node *l, Node *r)
			: key(k), height(h), left(l), right(r) {}
  };

  //ROOT
  Node *root;
  int sz;

  int height(Node *p) {
		return (p == NULL ? -1 : p->height);
	}

	void update(Node *p) {
		assert(p != NULL);
		p->height = 1 + std::max(height(p->left), height(p->right));
	}

	Node *rotateLeft(Node *p) {
	  if (p == NULL) return p;
	  if (p->right == NULL) return p;

		Node *rightP = p->right;
		p->right = rightP->left;
		rightP->left = p;

		update(p);
		update(rightP);

		return rightP;
	}

	Node *rotateRight(Node *p) {
		if (p == NULL) return p;
		if (p->left == NULL) return p;

		Node *leftP = p->left;
		p->left = leftP->right;
		leftP->right = p;

		update(p);
		update(leftP);

		return leftP;
	}

	Node *rebalance(Node *p) {
		if (p == NULL) return NULL;

		if (height(p->left) - height(p->right) > 1) {
			if (p->left != NULL && height(p->left->left) < height(p->left->right)) {
				p->left = rotateLeft(p->left);
			}

			p = rotateRight(p);
		} else if (height(p->right) - height(p->left) > 1) {
			if (p->right != NULL && height(p->right->right) < height(p->right->left)) {
				p->right = rotateRight(p->right);
			}

			p = rotateLeft(p);
		}

		update(p);
		assert(abs(height(p->right) - height(p->left)) <= 1);

		return p;
	}

	Node *insert(Node *p, const CauHoiThi& v) {
		if (p == NULL) {
			p = new Node(v, 0, NULL, NULL);
			++sz;
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

	Node *remove(Node *p, const CauHoiThi& v) {
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
      --sz;
			return p;
		}

		return rebalance(p);
	}

  Node *find(const CauHoiThi& v) const {
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

	//Lay danh sach cau hoi thi theo ma mon
  void layDanhSachCauHoiTheoMon(const string& maMon, Node *p, Vector<CauHoiThi *> &list) {
    if (p == NULL) return;

    layDanhSachCauHoiTheoMon(maMon, p->left, list);

    if (p->key.maMon == maMon) {
      list.push_back(&(p->key));
    }

    layDanhSachCauHoiTheoMon(maMon, p->right, list);
	}

	//Lay toan bo danh sach cau hoi thi
  void layHetDanhSachCauHoi(Node *p, Vector<CauHoiThi *> &list) {
    if (p == NULL) return;

    layHetDanhSachCauHoi(p->left, list);

    list.push_back(&(p->key));

    layHetDanhSachCauHoi(p->right, list);
	}

  //Writing to file
	void writeElement(ostream& os, Node *p) const {
		if (p == NULL) return;

		writeElement(os, p->left);

		os << p->key << '\n';

		writeElement(os, p->right);
	}

  Node *clone(Node *p) {
		if (p == NULL) return NULL;

		Node *ret = new Node(p->key, p->height, clone(p->left), clone(p->right));

		update(ret);

		return ret;
	}

	void clear(Node *p) {
		if (p == NULL) return;

		clear(p->left);
		clear(p->right);
		delete p;
	}

  bool phatHienCauHoiKhongTheXoa(Node *p) {
    if (p == NULL) return false;

    if (phatHienCauHoiKhongTheXoa(p->left)) return true;

    return (p->key.getCanModifyFlag() == false);

    if (phatHienCauHoiKhongTheXoa(p->right)) return true;
	}

  DSCauHoiThi_t() : root(NULL), sz(0) {}

  friend ostream& operator << (ostream& os, const DSCauHoiThi_t& rhs);
  friend istream& operator >> (istream& is, DSCauHoiThi_t& rhs);

public:
	~DSCauHoiThi_t() {
	  xoaDS();
  }

	bool empty() { return root == NULL; }

	int getSoCauHoi() { return sz; }

	void xoaDS() {
		clear(root);
		root = NULL;
		sz = 0;
	}

  bool phatHienCauHoiKhongTheXoa() {
    return phatHienCauHoiKhongTheXoa(root);
	}

	//Lay toan bo danh sach cau hoi thi
	Vector<CauHoiThi *> layHetDanhSachCauHoi() {
    Vector<CauHoiThi *> result;

    layHetDanhSachCauHoi(root, result);

    return result;
  }

	//Lay danh sach cau hoi thi theo ma mon
	void layDanhSachCauHoiTheoMon(const string& maMon, bool randomFlag, Vector<CauHoiThi *> &list) {
	  list.clear();

    layDanhSachCauHoiTheoMon(maMon, root, list);

    if (randomFlag) {
      //random-shuffle list
      RandomUtility::randomShuffle(list);
    }
	}

  void xoaCauHoiThiTheoMaMon(const string& maMon) {
//    xoaCauHoiThiTheoMaMon(maMon, root);
    Vector<CauHoiThi *> list;
    layDanhSachCauHoiTheoMon(maMon, false, list);
    for (int i = 0; i < list.size(); ++i) {
      root = remove(root, *list[i]);
    }
  }

  void remove(int id) {
    CauHoiThi ob;
		ob.setId(id);

		Node *p = find(ob);

    root = remove(root, ob);
  }

  void insert(CauHoiThi& newCauHoiThi) {
    root = insert(root, newCauHoiThi);
  }

  CauHoiThi *getCauHoi(int id) {
    CauHoiThi ob;
    ob.setId(id);

    return &(find(ob)->key);
  }

  DSCauHoiThi_t& operator = (const DSCauHoiThi_t& rhs) {
		xoaDS();

		root = clone(rhs.root);
		sz = rhs.sz;

		return *this;
	}

	//===================================================================
	static DSCauHoiThi_t *getDSCauHoiThiHienHanh() { return danhSachCauHoiThiHienHanh; }
};

DSCauHoiThi_t *DSCauHoiThi_t::danhSachCauHoiThiHienHanh = new DSCauHoiThi_t;

ostream& operator << (ostream& os, const DSCauHoiThi_t& rhs) {
  //Ghi so luong cau hoi
  os << rhs.sz << '\n';

  rhs.writeElement(os, rhs.root);

  return os;
}

istream& operator >> (istream& is, DSCauHoiThi_t& rhs) {
  //clear old data
  rhs.xoaDS();

  string line = "";

  if (getline(is, line)) {
    int soCau = atoi(line.c_str());

    for (int i = 0; i < soCau; ++i) {
      CauHoiThi ob;
      is >> ob;
      rhs.root = rhs.insert(rhs.root, ob);
    }
  }

  return is;
}

#endif // DSCAUHOITHI_T_MODEL_H_INCLUDED
