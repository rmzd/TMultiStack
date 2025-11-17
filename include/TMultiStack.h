#pragma once

#include <iostream>
#include <stdexcept>
#include <vector>
#include <fstream>

template <class T>
class TStack
{
protected:
    T** data;
    int len;
    bool isNew;
    int top;
public:
    TStack();
    TStack(int len_);
    TStack(const TStack& obj);
    TStack(TStack&& obj);
    TStack(T** data_, int len_);
    ~TStack();

    int GetLen();
    int GetCount();

    void Resize(int len_);
    void SetData(T** data_, int len_);

    void Push(T value);
    T Pop();

    bool IsEmpty();
    bool IsFull();

    TStack& operator=(const TStack<T>& obj);
    TStack& operator=(TStack<T>&& obj);
    bool operator==(const TStack<T>& obj);
    bool operator!=(const TStack<T>& obj);

    template <class O>
    friend std::ostream& operator<<(std::ostream& o, TStack<O>& v);
    template <class I>
    friend std::istream& operator>>(std::istream& i, TStack<I>& v);

    T FindMin() const;
    void SaveToFile(const std::string& filename) const;
    void LoadFromFile(const std::string& filename);
};

template<class T>
inline TStack<T>::TStack() : data(nullptr), len(0), isNew(true), top(0) {}

template<class T>
inline TStack<T>::TStack(int len_) : TStack() {
    if (len_ < 0) throw std::invalid_argument("len < 0");
    if (len_ > 0) {
        len = len_;
        data = new T * [len]();
    }
}

template<class T>
inline TStack<T>::TStack(const TStack& obj) : TStack() {
    if (obj.len > 0) {
        len = obj.len;
        data = new T * [len]();
        for (int i = 0; i < obj.top; i++) {
            if (obj.data[i]) data[i] = new T(*obj.data[i]);
        }
    }
    top = obj.top;
}

template<class T>
inline TStack<T>::TStack(TStack&& obj) {
    len = obj.len;
    data = obj.data;
    top = obj.top;
    isNew = obj.isNew;

    obj.len = 0; obj.data = nullptr; obj.top = 0; obj.isNew = true;
}

template<class T>
inline TStack<T>::TStack(T** data_, int len_) : TStack() {
    if (len_ < 0) throw std::invalid_argument("len < 0");
    if (len_ > 0) {
        len = len_;
        data = data_;
        isNew = false;
        top = 0;
        for (int i = 0; i < len; i++) {
            if (!data[i]) break;
            top++;
        }
    }
}

template<class T>
inline TStack<T>::~TStack() {
    if (isNew && data) {
        for (int i = 0; i < len; i++) delete data[i];
        delete[] data;
    }
}

template<class T>
inline int TStack<T>::GetLen() { return len; }

template<class T>
inline int TStack<T>::GetCount() { return top; }

template<class T>
inline void TStack<T>::Resize(int len_) {
    if (len_ < 0) throw std::invalid_argument("len < 0");
    if (len_ == len) return;

    if (len_ == 0) {
        if (isNew && data) {
            for (int i = 0; i < len; i++) delete data[i];
            delete[] data;
        }
        data = nullptr; len = top = 0; isNew = true;
        return;
    }

    T** newData = new T * [len_]();
    int elementsToCopy = std::min(len, len_);
    for (int i = 0; i < elementsToCopy; i++) newData[i] = data[i];

    if (len_ < len) {
        for (int i = len_; i < len; i++) delete data[i];
        top = std::min(top, len_);
    }

    if (isNew && data) delete[] data;

    data = newData; len = len_; isNew = true;
}

template<class T>
inline void TStack<T>::SetData(T** data_, int len_) {
    if (len_ < 0) throw std::invalid_argument("len < 0");
    if (isNew && data) {
        for (int i = 0; i < len; i++) delete data[i];
        delete[] data;
    }
    data = len_ > 0 ? data_ : nullptr;
    len = len_;
    top = 0;
    for (int i = 0; i < len; i++) {
        if (!data[i]) break;
        top++;
    }
    isNew = false;
}

template<class T>
inline bool TStack<T>::IsEmpty() { return top == 0; }

template<class T>
inline bool TStack<T>::IsFull() { return top >= len; }

template<class T>
inline void TStack<T>::Push(T value) {
    if (IsFull()) throw std::logic_error("stack is full");
    data[top++] = new T(value);
}

template<class T>
inline T TStack<T>::Pop() {
    if (IsEmpty()) throw std::logic_error("stack is empty");
    top--;
    T val = *data[top];
    delete data[top];
    data[top] = nullptr;
    return val;
}

template<class T>
inline TStack<T>& TStack<T>::operator=(const TStack<T>& obj) {
    if (this == &obj) return *this;

    if (isNew && data) {
        for (int i = 0; i < len; i++) delete data[i];
        delete[] data;
    }

    len = obj.len; top = obj.top; isNew = true;
    if (obj.len > 0) {
        data = new T * [len];
        for (int i = 0; i < len; i++)
            data[i] = obj.data[i] ? new T(*obj.data[i]) : nullptr;
    }
    else data = nullptr;

    return *this;
}

template<class T>
inline TStack<T>& TStack<T>::operator=(TStack<T>&& obj) {
    if (this == &obj) return *this;

    if (isNew && data) {
        for (int i = 0; i < len; i++) delete data[i];
        delete[] data;
    }

    len = obj.len; data = obj.data; top = obj.top; isNew = obj.isNew;

    obj.len = 0; obj.data = nullptr; obj.top = 0; obj.isNew = true;

    return *this;
}

template<class T>
inline bool TStack<T>::operator==(const TStack<T>& obj) {
    if (top != obj.top) return false;
    for (int i = 0; i < top; i++)
        if (*data[i] != *obj.data[i]) return false;
    return true;
}

template<class T>
inline bool TStack<T>::operator!=(const TStack<T>& obj) { return !(*this == obj); }

template<class O>
inline std::ostream& operator<<(std::ostream& o, TStack<O>& v) {
    o << "TStack[len=" << v.len << ", top=" << v.top << "]\nData: ";
    for (int i = 0; i < v.top; i++) o << (v.data[i] ? *v.data[i] : O()) << (i < v.top - 1 ? ", " : "");
    o << "\n";
    return o;
}

template<class I>
inline std::istream& operator>>(std::istream& i, TStack<I>& v) {
    int newLen;
    i >> newLen;
    if (!i.good()) return i;
    if (newLen < 0) throw std::invalid_argument("len < 0");

    if (v.isNew && v.data) {
        for (int j = 0; j < v.len; j++) delete v.data[j];
        delete[] v.data;
    }

    v.data = newLen > 0 ? new I * [newLen]() : nullptr;
    v.len = newLen;
    v.top = 0;
    v.isNew = true;

    for (int j = 0; j < newLen && i.good(); j++) {
        I value;
        i >> value;
        if (i.good()) v.data[j] = new I(value), v.top++;
    }
    return i;
}

template<class T>
T TStack<T>::FindMin() const {
    if (top == 0) throw std::logic_error("Cannot find min in empty stack");
    T minValue = *data[0];
    for (int i = 1; i < top; i++)
        if (*data[i] < minValue) minValue = *data[i];
    return minValue;
}

template<class T>
void TStack<T>::SaveToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) throw std::runtime_error("Cannot open file: " + filename);
    for (int i = 0; i < top; i++) if (data[i]) file << *data[i] << std::endl;
}

template<class T>
void TStack<T>::LoadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) throw std::runtime_error("Cannot open file: " + filename);

    if (isNew && data) {
        for (int i = 0; i < len; i++) delete data[i];
        delete[] data;
    }

    std::vector<T> temp;
    T value;
    while (file >> value) temp.push_back(value);

    if (!temp.empty()) {
        len = temp.size();
        data = new T * [len];
        top = len;
        isNew = true;
        for (int i = 0; i < top; i++) data[i] = new T(temp[i]);
    }
    else {
        data = nullptr;
        len = top = 0;
        isNew = true;
    }
}
