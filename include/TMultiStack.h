#pragma once

#include <iostream>
#include <stdexcept>
#include <vector>
#include <fstream>
#include <algorithm>

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

template <class T>
class TMultiStack
{
private:
    size_t len;
    size_t stackCount;
    T** data;
    TStack<T>* stacks;

public:
    TMultiStack();
    TMultiStack(size_t totalSize, size_t numStacks);
    TMultiStack(const TMultiStack& other);
    TMultiStack(TMultiStack&& other);
    ~TMultiStack();

    void Push(size_t stackIndex, const T& value);
    T Pop(size_t stackIndex);
    bool IsEmpty(size_t stackIndex) const;
    bool IsFull(size_t stackIndex) const;
    void Repack(int curStack);

    TMultiStack& operator=(const TMultiStack& other);
    TMultiStack& operator=(TMultiStack&& other);
    bool operator==(const TMultiStack& other) const;
    bool operator!=(const TMultiStack& other) const;
    TStack<T>& operator[](size_t stackIndex);
    const TStack<T>& operator[](size_t stackIndex) const;

    template <class U>
    friend std::ostream& operator<<(std::ostream& os, const TMultiStack<U>& multiStack);

    template <class U>
    friend std::istream& operator>>(std::istream& is, TMultiStack<U>& multiStack);

    size_t GetLen();
    size_t GetStackCount();
    T** GetData();
    TStack<T>* GetStacks();

    void InitializeStacks();
    size_t GetStackStart(size_t stackIndex);
    void UpdateStackPointers(size_t stackIndex, size_t newStart, size_t newCapacity);
};

template <class T>
TMultiStack<T>::TMultiStack() : len(0), stackCount(0), data(nullptr), stacks(nullptr) {}

template <class T>
TMultiStack<T>::TMultiStack(size_t totalSize, size_t numStacks)
{
    if (numStacks <= 0) throw std::invalid_argument("numStacks <= 0");

    len = totalSize;
    stackCount = numStacks;

    data = new T * [len];
    for (size_t i = 0; i < len; i++)
        data[i] = nullptr;

    stacks = new TStack<T>[stackCount];
    InitializeStacks();
}

template <class T>
TMultiStack<T>::TMultiStack(const TMultiStack& other)
{
    len = other.len;
    stackCount = other.stackCount;

    if (other.data && len > 0) {
        data = new T * [len];
        for (size_t i = 0; i < len; i++) {
            if (other.data[i])
                data[i] = new T(*other.data[i]);
            else
                data[i] = nullptr;
        }
    }
    else {
        data = nullptr;
    }

    if (other.stacks && stackCount > 0) {
        stacks = new TStack<T>[stackCount];
        for (size_t i = 0; i < stackCount; i++) {
            stacks[i] = other.stacks[i];
        }
    }
    else {
        stacks = nullptr;
    }
}

template <class T>
TMultiStack<T>::TMultiStack(TMultiStack&& other)
    : len(other.len), stackCount(other.stackCount), data(other.data), stacks(other.stacks)
{
    other.len = 0;
    other.stackCount = 0;
    other.data = nullptr;
    other.stacks = nullptr;
}

template <class T>
TMultiStack<T>::~TMultiStack()
{
    if (data) {
        for (size_t i = 0; i < len; i++)
            delete data[i];
        delete[] data;
    }
    delete[] stacks;
}

template <class T>
void TMultiStack<T>::Push(size_t stackIndex, const T& value)
{
    if (stackIndex >= stackCount) throw std::out_of_range("stackIndex >= stackCount");
    if (stacks[stackIndex].IsFull()) Repack(stackIndex);
    stacks[stackIndex].Push(value);
}

template <class T>
T TMultiStack<T>::Pop(size_t stackIndex)
{
    if (stackIndex >= stackCount) throw std::out_of_range("stackIndex >= stackCount");
    if (stacks[stackIndex].IsEmpty()) throw std::logic_error("stack is empty");
    return stacks[stackIndex].Pop();
}

template <class T>
bool TMultiStack<T>::IsEmpty(size_t stackIndex) const
{
    if (stackIndex >= stackCount) throw std::out_of_range("stackIndex >= stackCount");
    return stacks[stackIndex].IsEmpty();
}

template <class T>
bool TMultiStack<T>::IsFull(size_t stackIndex) const
{
    if (stackIndex >= stackCount) throw std::out_of_range("stackIndex >= stackCount");
    return stacks[stackIndex].IsFull();
}

template <class T>
void TMultiStack<T>::Repack(int curStack)
{
    size_t* stackSizes = new size_t[stackCount];
    size_t* stackStarts = new size_t[stackCount];

    size_t totalUsed = 0;
    for (size_t i = 0; i < stackCount; i++) {
        stackSizes[i] = stacks[i].GetCount();
        totalUsed += stackSizes[i];
    }

    size_t freeSpace = len - totalUsed;
    size_t baseFree = freeSpace / stackCount;
    size_t extraFree = freeSpace % stackCount;

    stackStarts[0] = 0;
    for (size_t i = 0; i < stackCount; i++) {
        size_t newSize = stackSizes[i] + baseFree + (i == static_cast<size_t>(curStack) ? extraFree : 0);
        if (i > 0) {
            stackStarts[i] = stackStarts[i - 1] + stackSizes[i - 1] + baseFree +
                ((i - 1) == static_cast<size_t>(curStack) ? extraFree : 0);
        }

        UpdateStackPointers(i, stackStarts[i], newSize);
    }

    delete[] stackSizes;
    delete[] stackStarts;
}

template <class T>
TMultiStack<T>& TMultiStack<T>::operator=(const TMultiStack& other)
{
    if (this == &other) return *this;

    if (data) {
        for (size_t i = 0; i < len; i++) delete data[i];
        delete[] data;
    }
    delete[] stacks;

    len = other.len;
    stackCount = other.stackCount;

    if (other.data && len > 0) {
        data = new T * [len];
        for (size_t i = 0; i < len; i++) {
            if (other.data[i])
                data[i] = new T(*other.data[i]);
            else
                data[i] = nullptr;
        }
    }
    else {
        data = nullptr;
    }

    if (other.stacks && stackCount > 0) {
        stacks = new TStack<T>[stackCount];
        for (size_t i = 0; i < stackCount; i++) {
            stacks[i] = other.stacks[i];
        }
    }
    else {
        stacks = nullptr;
    }

    return *this;
}

template <class T>
TMultiStack<T>& TMultiStack<T>::operator=(TMultiStack&& other)
{
    if (this == &other) return *this;

    if (data) {
        for (size_t i = 0; i < len; i++) delete data[i];
        delete[] data;
    }
    delete[] stacks;

    len = other.len;
    stackCount = other.stackCount;
    data = other.data;
    stacks = other.stacks;

    other.len = 0;
    other.stackCount = 0;
    other.data = nullptr;
    other.stacks = nullptr;

    return *this;
}

template <class T>
bool TMultiStack<T>::operator==(const TMultiStack& other) const
{
    if (len != other.len || stackCount != other.stackCount) return false;

    for (size_t i = 0; i < stackCount; i++) {
        if (stacks[i] != other.stacks[i]) return false;
    }

    return true;
}

template <class T>
bool TMultiStack<T>::operator!=(const TMultiStack& other) const
{
    return !(*this == other);
}

template <class T>
TStack<T>& TMultiStack<T>::operator[](size_t stackIndex)
{
    if (stackIndex >= stackCount) throw std::out_of_range("stackIndex >= stackCount");
    return stacks[stackIndex];
}

template <class T>
const TStack<T>& TMultiStack<T>::operator[](size_t stackIndex) const
{
    if (stackIndex >= stackCount) throw std::out_of_range("stackIndex >= stackCount");
    return stacks[stackIndex];
}

template <class T>
size_t TMultiStack<T>::GetLen() { return len; }

template <class T>
size_t TMultiStack<T>::GetStackCount() { return stackCount; }

template <class T>
T** TMultiStack<T>::GetData() { return data; }

template <class T>
TStack<T>* TMultiStack<T>::GetStacks() { return stacks; }

template <class T>
void TMultiStack<T>::InitializeStacks()
{
    size_t baseSize = len / stackCount;
    size_t remainder = len % stackCount;

    size_t currentStart = 0;
    for (size_t i = 0; i < stackCount; i++) {
        size_t stackSize = baseSize + (i < remainder ? 1 : 0);
        stacks[i].SetData(data + currentStart, stackSize);
        currentStart += stackSize;
    }
}

template <class T>
size_t TMultiStack<T>::GetStackStart(size_t stackIndex)
{
    if (stackIndex >= stackCount) throw std::out_of_range("stackIndex >= stackCount");

    size_t start = 0;
    for (size_t i = 0; i < stackIndex; i++) {
        start += stacks[i].GetLen();
    }
    return start;
}

template <class T>
void TMultiStack<T>::UpdateStackPointers(size_t stackIndex, size_t newStart, size_t newCapacity)
{
    if (stackIndex >= stackCount) throw std::out_of_range("stackIndex >= stackCount");
    if (newStart + newCapacity > len) throw std::invalid_argument("newStart + newCapacity > len");

    stacks[stackIndex].SetData(data + newStart, newCapacity);
}

template <class U>
std::ostream& operator<<(std::ostream& os, const TMultiStack<U>& multiStack)
{
    os << "TMultiStack[len=" << multiStack.len << ", stackCount=" << multiStack.stackCount << "]\n";
    for (size_t i = 0; i < multiStack.stackCount; i++) {
        os << "Stack " << i << ": " << multiStack.stacks[i];
    }
    return os;
}

template <class U>
std::istream& operator>>(std::istream& is, TMultiStack<U>& multiStack)
{
    size_t newLen, newStackCount;
    is >> newLen >> newStackCount;

    if (!is.good()) return is;

    TMultiStack<U> temp(newLen, newStackCount);
    multiStack = std::move(temp);

    return is;
}