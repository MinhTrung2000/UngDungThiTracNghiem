#ifndef LIB_STACK_H
#define LIB_STACK_H

#include <cassert>

template <typename T>
class Stack {
	private:
	int size;
	int cap;
	T *arr;

	public:
	Stack(int c = 100) : size(0), cap(c) {
		arr = new T[cap];
	}

	~Stack() {
			delete [] arr;
	}

	bool empty() const { return size == 0; }

	bool full() const { return size == cap; }

	void push(const T & v) {
		if (!full()) {
			arr[size++] = v;
		} else {
			T *new_arr = new T[cap * 2];
			cap *= 2;
			for (int i = 0; i < size; ++i) {
				new_arr[i] = arr[i];
			}
			delete [] arr;
			new_arr[size++] = v;
			arr = new_arr;
			delete [] new_arr;
		}
	}

	void pop() {
		assert(!empty());
		--size;
	}

	T & top() {
		assert(!empty());
		return arr[size - 1];
	}

	Stack& operator = (const Stack<T>& rhs) {
    size = rhs.size;
    cap = rhs.cap;
    delete[] arr;
    arr = new T[cap];
    for (int i = 0; i < size; ++i) {
      arr[i] = rhs.arr[i];
    }
    return *this;
	}

	// void debug() { for (int i = 0; i < size; ++i) cerr << arr[i] << ' '; cerr << '\n'; }
};

#endif
