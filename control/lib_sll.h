#ifndef LIB_SLL_H
#define LIB_SLL_H

template <typename T>
class SLLNode {
	public:
	T key;
	SLLNode *next;
	SLLNode(const T& k = T(), SLLNode<T> *p = NULL) : key(k), next(p) {}

//	SLLNode<T>& operator = (const SLLNode<T>& rhs) {
//	  key = rhs.key;
//	  delete next;
//	  next = NULL;
//	  if (rhs.next != NULL) {
//      next = new SLLNode<T>();
//      *next = *rhs.next;
//	  }
//    return *this;
//	}
};

template <typename T>
class SLL {
	public:
	SLLNode<T> *head, *tail;
	int n;

	SLL() : head(NULL), tail(NULL), n(0) {}
	SLL(const SLL<T>& rhs) : head(NULL), tail(NULL), n(0) {
    if (! rhs.empty()) {
      n = rhs.n;

      head = new SLLNode<T>(rhs.head->key);
      tail = head;

      SLLNode<T> *p = rhs.head->next;

      while (p != NULL) {
        tail->next = new SLLNode<T>(p->key);
        tail = tail->next;
        p = p->next;
      }
    }
	}

	~SLL() {
		clear();
	}

	bool empty() const { return n == 0; }
	int size() const { return n; }

	void clear() {
		if (empty()) return;

		SLLNode<T> *p = NULL;

		while (n > 0) {
			p = head->next;
			delete head;
			head = p;
			--n;
		}

		head = NULL;
		tail = NULL;
	}

	bool insertHead(const T& v) {
		head = new SLLNode<T>(v, head);
		if (empty()) tail = head;
		++n;

		return true;
	}

	bool insertAfter(SLLNode<T> *p, const T& v) {
		if (p == NULL) return false;

		SLLNode<T> *new_node = new SLLNode<T>(v, p->next);
		p->next = new_node;
		if (p == tail) tail = new_node;
		++n;

		return true;
	}

	bool insertTail(const T& v) {
		if (empty()) return insertHead(v);

		tail->next = new SLLNode<T>(v);
		tail = tail->next;
		++n;

		return true;
	}

	// k : 0 base
	bool insertKth(int k, const T& v) {
		//change k to 0 if empty, k to n if k > n
		if (k >= n) k = n;

		if (k == 0) {
			return insertHead(v);
		} else if (k < n - 1) {
			SLLNode<T> *prev = head;
			for (int i = 0; i < k - 2; ++i) prev = prev->next;
			return insertAfter(prev, v);
		} else {
			return insertTail(v);
		}
	}

	/*
		0: is not sorted or empty or only one node
		1: is sorted inc.
		2: is sorted dec.
	*/
	int getSortedInfo() {
		if (empty()) return 0;

		int type = 1;
		SLLNode<T> *p = head;
		if (p->next == NULL) return 0;
		else if (p->key > p->next->key) type = 2;
		p = p->next;

		if (type == 1) {
			while (p->next != NULL) {
				if (p->key > p->next->key) return 0;
				p = p->next;
			}
			return 1;
		}
		while (p->next != NULL) {
			if (p->key < p->next->key) return 0;
			p = p->next;
		}

		return 2;
	}

	bool insertOrder(const T& v) {
		int state = getSortedInfo();
		if (state == 0) return false;

		SLLNode<T> *p = head;
		if (state == 1) {
			if (v <= head->key) return insertHead(v);
			else if (v >= tail->key) return insertTail(v);
			else {
				while (p != NULL && p->next->key < v) p = p->next;
				return insertAfter(p, v);
			}
		} else {
			if (v >= head->key) return insertHead(v);
			else if (v <= tail->key) return insertTail(v);
			else {
				while (p != NULL && p->next->key > v) p = p->next;
				return insertAfter(p, v);
			}
		}
	}

	/*
		Insert SLL list at index id (0 base)
		REM: Not create l_copy and link head or tail of it
		because after return out, l_copy is de-constructed.

		Don't use insertAfter, it's slow because of linking
		new_node and next_node each time.
	*/
	bool insertAnotherList(SLL<T>& l, int id = 0) {
		if (l.empty() || id < 0 || id > n) return false;

		SLLNode<T> *head_copy = new SLLNode<T>(l.head->key, NULL);
		SLLNode<T> *tail_copy = head_copy;
		SLLNode<T> *p = l.head->next;

		while (p != NULL) {
			tail_copy->next = new SLLNode<T>(p->key, NULL);
			tail_copy = tail_copy->next;
			p = p->next;
		}

		if (empty()) {
			head = head_copy;
			tail = tail_copy;
			n = l.size();
			return true;
		}

		if (id == 0) {
			tail_copy->next = head;
			head = head_copy;
		} else if (id < n) {
			p = head;
			for (int i = 0; i < id - 1; ++i) p = p->next;
			tail_copy->next = p->next;
			p->next = head_copy;
		} else { //id = n, insert at end.
			tail->next = head_copy;
			tail = tail_copy;
		}
		n += l.size();
		return true;
	}

	bool removeHead() {
		if (empty()) return false;

		SLLNode<T> *p = head;
		if (head == tail) {
      head = NULL;
      tail = NULL;
		}
		else head = head->next;
		delete p;
		--n;

		return true;
	}

	bool removeAfter(SLLNode<T> *p) {
		if (p == NULL) return false;
		if (p == tail) return false;

		SLLNode<T> *del_node = p->next;
		if (del_node == tail) tail = p;
		p->next = p->next->next;
		delete del_node;
		--n;

		return true;
	}

	bool remove(SLLNode<T> *p) {
		if (p == NULL) return false;
		if (empty()) return false;

		if (p == head) {
			return removeHead();
		} else {
			SLLNode<T> *prev = head;
			while (prev != NULL && prev->next != p) prev = prev->next;
			return removeAfter(prev);
		}
	}

	//k : 0 base
	T removeKth(int k) {
		assert(!empty() && 0 <= k && k < n);

		T ret = T();
		if (k == 0) {
			ret = head->key;
			removeHead();
		} else {
			SLLNode<T> *prev = head;
			for (int i = 0; i < k - 1; ++i) prev = prev->next;
			ret = prev->next->key;
			removeAfter(prev);
		}

		return ret;
	}

	bool removeTail() {
		return remove(tail);
	}

	SLLNode<T> *search(const T& v) {
		SLLNode<T> *p = head;

		while (p != NULL) {
			if (p->key == v) return p;
			p = p->next;
		}

		return NULL;
	}

	bool erase(const T& v) {
		SLLNode<T> *p = search(v);
		return remove(p);
	}

	//k: 0 base
	bool replace(int k, const T& v) {
		if (empty() || k < 0 || k >= n) return false;

		if (k == 0) head->key = v;
		else {
			SLLNode<T> *p = head;
			for (int i = 0; i < k; ++i) p = p->next;
			p->key = v;
		}

		return true;
	}

	void unique() {
		if(empty() || n == 1) return;

		sort();

		SLLNode<T> *p = head;
		while (p != NULL) {
			while (p->next != NULL && p->next->key == p->key) {
				removeAfter(p);
			}
			p = p->next;
		}
	}

	void push_back(const T& v) { insertTail(v); }
	void push_front(const T& v) { insertHead(v); }

	bool pop_back() { return removeTail(); }
	bool pop_front() { return removeHead(); }

	void reverse() {
		if (empty() || n == 1) return;

		T *data = new T[n];

		SLLNode<T> *p = head;
		for (int i = 0; i < n; ++i) {
			data[i] = p->key;
			p = p->next;
		}

		p = head;
		for (int i = 0; i < n; ++i) {
			p->key = data[n - 1 - i];
			p = p->next;
		}

		delete[] data;
	}

	//use selection-sort only
	void sort(bool (*f)(const T& x, const T& y)) {
		if (empty() || n == 1) return;

		SLLNode<T> *pmin;
		for (SLLNode<T> *p = head; p->next != NULL; p = p->next) {
			pmin = p;
			SLLNode<T> *q = p->next;
			for (; q != NULL; q = q->next) {
				if (!f(pmin->key, q->key)) {
					pmin = q;
				}
			}
			//swap it
			if (pmin->key != p->key) {
				T t = pmin->key; pmin->key = p->key; p->key = t;
			}
		}
	}

	void sort() {
		if (empty() || n == 1) return;

		SLLNode<T> *pmin;
		for (SLLNode<T> *p = head; p->next != NULL; p = p->next) {
			pmin = p;
			SLLNode<T> *q = p->next;
			for (; q != NULL; q = q->next) {
				if (pmin->key > q->key) {
					pmin = q;
				}
			}
			//swap it
			if (pmin->key != p->key) {
				T t = pmin->key; pmin->key = p->key; p->key = t;
			}
		}
	}

	SLL<T>& operator = (const SLL<T>& rhs) {
		clear();

		if (!rhs.empty()) {
      n = rhs.n;

      head = new SLLNode<T>(rhs.head->key);
      tail = head;

      SLLNode<T> *p = rhs.head->next;

      while (p != NULL) {
        tail->next = new SLLNode<T>(p->key);
        tail = tail->next;
        p = p->next;
      }
		}

		return *this;
	}
};

template <typename T>
std::ostream& operator << (std::ostream& os, const SLL<T>& l) {
	if (l.empty()) return os;
	SLLNode<T> *p = l.head;
	if (p != NULL) os << p->key;
	else return os;
	p = p->next;
	while (p != NULL) {
		os << ", " << p->key;
		p = p->next;
	}
	return os;
}

//x, y are already sorted Inc.
template <typename T>
SLL<T> merge(const SLL<T>& x, const SLL<T>& y) {
	SLL<T> ret;

	SLLNode<T> *px = x.head;
	SLLNode<T> *py = y.head;

	while (px != NULL && py != NULL) {
		if (px->key < py->key) {
			ret.push_back(px->key);
			px = px->next;
		} else {
			ret.push_back(py->key);
			py = py->next;
		}
	}
	while (px != NULL) {
		ret.push_back(px->key);
		px = px->next;
	}
	while (py != NULL) {
		ret.push_back(py->key);
		py = py->next;
	}
	return ret;
}

// template <typename T>
// void debug(const SLL<T>& l) {
	// std::cerr << "\nl[]: "; std::cerr << l << '\n';
	// std::cerr << "[s, h, t]: " << l.size() << ' '
	// << (l.head != NULL ? to_string(l.head->key) : "NULL")
	// << ' ' << (l.tail != NULL ? to_string(l.tail->key) : "NULL") << "\n\n";
// }

#endif
