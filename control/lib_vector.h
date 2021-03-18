#ifndef LIB_VECTOR_H
#define LIB_VECTOR_H

#include <iostream>

template <typename T>
class Vector
{
private:
    int sz;
    int cap;
    T *arr;

public:
    Vector() : sz(0), cap(0), arr(NULL) {}

    Vector(int s, const T& v = T()) : sz(s), cap(s)
    {
        arr = new T[cap];
        for (int i = 0; i < sz; ++i)
            arr[i] = v;
    }

    Vector(T *ptr, int n) : sz(n), cap(n)
    {
        arr = new T[cap];
        for (int i = 0; i < sz; ++i)
        {
            arr[i] = ptr[i];
        }
    }

    Vector(const Vector<T>& rhs) : sz(rhs.size()), cap(rhs.capacity())
    {
        arr = new T[cap];
        for (int i = 0; i < sz; ++i)
            arr[i] = rhs.arr[i];
    }

    ~Vector()
    {
        delete[] arr;
    }

    bool empty() const
    {
        return sz == 0;
    }

    int size() const
    {
        return sz;
    }

    int capacity() const
    {
        return cap;
    }

    //Requests that the vector capacity be at least enough to
    // contain newCap elements.
    void reserve(int newCap)
    {
        assert(newCap > 0);
        if (newCap <= cap)
            return;

        cap = newCap;
        T *newArr = new T[cap];
        if (!empty())
        {
            for (int i = 0; i < sz; ++i)
            {
                newArr[i] = arr[i];
            }
        }
        delete[] arr;
        arr = newArr;
    }

    /*
    If n is smaller than the current container size,
    	the content is reduced to its first n elements, removing those beyond (and destroying them).

    If n is greater than the current container size, the content is expanded by inserting at the
    	end as many elements as needed to reach a size of n. If val is specified, the new elements
    	are initialized as copies of val, otherwise, they are value-initialized.

    If n is also greater than the current container capacity, an automatic reallocation of
    	the allocated storage space takes place.
    */
    void resize(int newSize)
    {
        if (newSize == sz)
            return;

        if (newSize < sz)
        {
            sz = newSize;
            return;
        }

        if (newSize > cap)
        {
            reserve(newSize);
        }

        for (int i = sz; i < newSize; ++i)
        {
            arr[i] = T();
        }

        sz = newSize;
    }

    void resize(int newSize, const T& val)
    {
        if (newSize == sz)
            return;

        if (newSize < sz)
        {
            sz = newSize;
            return;
        }

        if (newSize > cap)
        {
            reserve(newSize);
        }

        for (int i = sz; i < newSize; ++i)
        {
            arr[i] = val;
        }

        sz = newSize;
    }

    void push_back(const T& x)
    {
        if (cap == 0)
            reserve(1);
        if (sz == cap)
            reserve(2 * cap);
        arr[sz++] = x;
    }

    void pop_back()
    {
        assert(!empty());
        --sz;
    }

    T& front()
    {
        assert(!empty());
        return arr[0];
    }

    T& back()
    {
        assert(!empty());
        return arr[sz - 1];
    }

    T& operator [] (int id)
    {
        assert(0 <= id && id < sz);
        return arr[id];
    }

    const T& operator [] (int id) const
    {
        assert(0 <= id && id < sz);
        return arr[id];
    }

    Vector<T>& operator = (const Vector<T>& rhs)
    {
        sz = rhs.size();
        cap = rhs.capacity();
        delete [] arr;

        arr = new T[cap];
        for (int i = 0; i < sz; ++i)
        {
            arr[i] = rhs.arr[i];
        }

        return *this;
    }

    void clear()
    {
        sz = 0;
    }

    //id: 0 - base
    bool remove(int id)
    {
        if (id < 0 || id >= sz)
            return false;
        for (int i = id; i < sz - 1; ++i)
            arr[i] = arr[i + 1];
        if (0 <= id && id < sz)
            --sz;
        return true;
    }

    bool erase(const T& val)
    {
        int i = 0;
        for (; i < sz; ++i)
        {
            if (arr[i] == val)
                break;
        }
        if (i == sz)
            return false;
        return remove(i);
    }

    void reverse()
    {
        if (empty())
            return;
        int i = 0;
        int j = sz - 1;
        while (i < j)
        {
            T tmp = arr[i];
            arr[i] = arr[j];
            arr[j] = tmp;
            ++i;
            --j;
        }
    }

    //id: 0 base
    void insert(int id, const T& val)
    {
        if (id < 0 && id >= sz)
            return;
        push_back(val);
        for (int i = sz; i >= id + 1; --i)
            arr[i] = arr[i - 1];
        arr[id] = val;
    }

    //quick sort
    void sort()
    {
        if (empty() || sz == 1)
            return;
        sort(0, sz - 1);
    }

    //quick sort
    void sort(int (*f)(const T&, const T&))
    {
        if (empty() || sz == 1)
            return;
        sort(0, sz - 1, f);
    }

    //quick sort
    void sort(int from, int to)
    {
        if (empty())
            return;
        if (! (0 <= from && from < sz && 0 <= to && to < sz && from < to))
            return;

        int i = from;
        int j = to;
        T key = arr[(from + to) / 2];
        while (i < j)
        {
            while (i < to && arr[i] < key)
                ++i;
            while (j > from && arr[j] > key)
                --j;
            if (i <= j)
            {
                T tmp = arr[i];
                arr[i] = arr[j];
                arr[j] = tmp;
                ++i;
                --j;
            }
        }
        if (from < j)
            sort(from, j);
        if (i < to)
            sort(i, to);
    }

    //can set function pointer
    void sort(int from, int to, int (*f)(const T&, const T&))
    {
        if (empty())
            return;
        if (! (0 <= from && from < sz && 0 <= to && to < sz && from < to))
            return;

        int i = from;
        int j = to;
        T key = arr[(from + to) / 2];
        while (i < j)
        {
            while (i < to && f(arr[i], key) == 1)
                ++i;
            while (j > from && f(arr[j], key) == -1 && arr[j] != key)
                --j;
            if (i <= j)
            {
                T tmp = arr[i];
                arr[i] = arr[j];
                arr[j] = tmp;
                ++i;
                --j;
            }
        }
        if (from < j)
            sort(from, j, f);
        if (i < to)
            sort(i, to, f);
    }

    Vector<T> unique()
    {
        if (empty())
            return Vector<T>();
        Vector<T> res;
        Vector<T> temp = *this;
        temp.sort();
        res.push_back(temp[0]);
        for (int i = 1; i < temp.size(); )
        {
            if (temp[i] == res.back())
                ++i;
            else
            {
                res.push_back(temp[i]);
            }
        }
        return res;
    }

    Vector<T> unique(int (*f)(const T&, const T&))
    {
        if (empty())
            return Vector<T>();
        Vector<T> res;
        Vector<T> temp = *this;
        temp.sort(f);
        res.push_back(temp[0]);
        for (int i = 1; i < temp.size(); )
        {
            if (temp[i] == res.back())
                ++i;
            else
            {
                res.push_back(temp[i]);
            }
        }
        return res;
    }
};

template <typename T>
std::ostream& operator << (std::ostream& os, const Vector<T>& a)
{
    if (!a.empty())
        os << a[0];
    for (int i = 1; i < a.size(); ++i)
        os << ", " << a[i];
    return os;
}

#endif
