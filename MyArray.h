//
// Created by Vanka on 03.08.2022.
//
#ifndef MY_IDEAS_MYARRAY_H
#define MY_IDEAS_MYARRAY_H

#include <initializer_list>
#include <typeinfo>
#include <functional>
#include <iostream>
#include <vector>
using std::function;

namespace sorts {

    template<typename U>
    void heapSort(U *arr, int len) { //mine
        U temp;
        int prepareRoot = len / 2 - 1;
        int root, left, right;
        for (int limit = len; limit > 0;) {
            root = prepareRoot;
            temp = arr[root];
            while (true) {
                left = root * 2 + 1, right = root * 2 + 2;
                if (right < limit) {
                    if (temp < arr[left] || temp < arr[right]) {
                        if (arr[left] < arr[right]) {
                            arr[root] = arr[right];
                            root = right;
                        } else {
                            arr[root] = arr[left];
                            root = left;
                        }
                    } else {
                        arr[root] = temp;
                        break;
                    }
                } else if (left < limit && temp < arr[left]) {
                    arr[root] = arr[left];
                    root = left;
                } else {
                    arr[root] = temp;
                    break;
                }
            }
            if (prepareRoot)prepareRoot--;
            else {
                temp = arr[0];
                arr[0] = arr[--limit];
                arr[limit] = temp;
            }
        }
    }

    template<typename U>
    void insertSort(U *arr, int len) {
        U temp;
        for (int i = 0, j; i < len; ++i) {
            temp = arr[i];
            j = i - 1;
            while (j >= 0 && arr[j] > temp) {
                arr[j + 1] = arr[j];
                j--;
            }
            if (j + 1 != i)arr[j + 1] = temp;
        }
    }

    template<typename U>
    void mergeSort(U *arr, int len){
        unsigned int shift = 0, mid, right_step;
        unsigned int left_i, right_i;

        while ((unsigned int)(len-1)>>shift!=1)++shift;
        auto* temp = new U[1u<<shift];//arr len = max step value

        for (unsigned step = 1; step < len; step<<=1u) {
            for (unsigned left = 0; left < len - step; left+=step<<1u) {
                mid = left + step;
                if (mid+step>len)right_step = len-mid;
                else right_step = step;

                memcpy(temp, arr+left, sizeof(U)*step);
                left_i = 0;
                right_i = 0;

                while (left_i<step && right_i<right_step){
                    if (temp[left_i]<arr[mid+right_i]){
                        arr[left+left_i+right_i] = temp[left_i];
                        ++left_i;
                    }else{
                        arr[left+left_i+right_i] = arr[mid+right_i];
                        ++right_i;
                    }
                }

                while (left_i<step){
                    arr[left+left_i+right_i] = temp[left_i];
                    ++left_i;
                }
            }
        }
        delete[] temp;
    }

}



struct InvalidIndexException : public std::exception {
    const char *what() const throw() {
        return "Invalid index";
    }
};

struct ArrayIndexException : public std::exception {
    const char *what() const throw() {
        return "Index out of bounds exception";
    }
};

template<typename T>
class MyArray {
private:

    int _length;//make it constant
    bool sliced = false;
    T *_data;

    void init() {
        _data = new T[_length];

        std::type_info const &t = typeid(T);
        if (t == typeid(int) || t == typeid(long) || t == typeid(long long) ||
        t == typeid(short) || t == typeid(char) || t==typeid(double)|| t == typeid(float))fill(0);
    }

    MyArray(int length, T *array): _length(length){
        _data = array;
        sliced = true;
    }

public:

    inline int length(){
        return _length;
    }

    MyArray(): _length(0){ _data = nullptr;}

    MyArray(T *array, int length): _length(length) {
        _data = new T[length];
        memcpy(_data, array, sizeof(T) * length);
    }

    MyArray(int length): _length(length) {
        init();
    }

    MyArray(const std::initializer_list<T>& list): _length(list.size()){
        _data = new T[_length];
        memcpy(_data, list.begin(), sizeof(T) * _length);
    }

    //copy constructor
    MyArray(const MyArray<T> &arr): MyArray(arr._data, arr._length){std::cout<<"copy!\n";}

    MyArray(int length, const T &value): _length(length){
        _data = new T[length];
        fill(value);
    }

    MyArray(const std::vector<T> &v): MyArray(v.data(),v.size()){}

    ~MyArray() {
        if (!sliced) delete[] _data;
    }

    template<typename U> //cout print
    friend std::ostream& operator << (std::ostream &out, const MyArray<U> &arr);

    template<typename U> //cin
    friend std::istream& operator >> (std::istream &in, MyArray<U> &arr);

    T &operator[](int index) {
        if (index >= _length || index < 0 && -index > _length)throw ArrayIndexException();
        if (index < 0)return _data[_length + index];
        return _data[index];
    }

    MyArray &operator +=(const T &value){
        for (int i = 0; i < _length; ++i) _data[i]+=value;
        return *this;
    }

    MyArray &operator -=(const T &value){
        for (int i = 0; i < _length; ++i) _data[i]-=value;
        return *this;
    }

    MyArray &operator *=(const T &value){
        for (int i = 0; i < _length; ++i) _data[i]*=value;
        return *this;
    }

    MyArray &operator /=(const T &value){
        for (int i = 0; i < _length; ++i) _data[i]/=value;
        return *this;
    }

    MyArray &operator =(const T &value){
        return fill(value);
    }

    MyArray &operator =(const MyArray& arr){
        if (&arr != this) {
            _length = arr._length;
            delete[] _data;
            _data = new T[_length];
            memcpy(_data, arr._data, sizeof(T) * _length);
        }
        return *this;
    }

    bool operator ==(MyArray<T>& arr){
        return equals(arr);
    }

    T& first(){
        return operator[](0);
    }

    T& last(){
        return operator[](-1);
    }

    MyArray operator ()(int start, int end){
        return slice(start, end);
    }

    MyArray operator ()(int end){
        return slice(0, end);
    }

    bool equals(MyArray<T>& arr){
        if (&arr==this)return true;
        if (_length != arr._length)return false;
        for (int i = 0; i < _length; ++i) {
            if (_data[i]!=arr._data[i])return false;
        }
        return true;
    }

    MyArray &fill(const T &value, int start, int end) {
        if (start<0 || end > _length)throw InvalidIndexException();
        for (;start < end; ++start)_data[start] = value;
        return *this;
    }
    MyArray &fill(const T &value){
        return fill(value, 0, _length);
    }

    int indexOf(const T &value, int startIndex = 0){
        if (startIndex<0|| startIndex >= _length) throw InvalidIndexException();
        for (; startIndex < _length; ++startIndex)
            if (_data[startIndex] == value)return startIndex;
        return -1;
    }

    int lastIndexOf(const T &value, int startIndex){
        if (startIndex<0 || startIndex >= _length) throw InvalidIndexException();
        for (;startIndex >= 0; --startIndex)
            if (_data[startIndex] == value)return startIndex;
        return -1;
    }
    int lastIndexOf(const T &value){
        return lastIndexOf(value, _length - 1);
    }

    bool contains(const T &value){
        for (int i = 0; i < _length; ++i) {
            if (_data[i]==value)return true;
        }
        return false;
    }

    T min(){
        T m = operator[](0);
        for (int i = 1; i < _length; ++i)
            if (_data[i]<m)m=_data[i];
        return m;
    }

    T max(){
        T m = operator[](0);
        for (int i = 1; i < _length; ++i)
            if (_data[i]>m)m=_data[i];
        return m;
    }


    void forEach(const function<void(T &value)> &func){
        for (int i = 0; i < _length; ++i)
            func(_data[i]);
    }

    void forEach(const function<void(T &value, int index)> &func){
        for (int i = 0; i < _length; ++i)
            func(_data[i],i);
    }

    template<typename U>
    void forEach(const function<U(T &value)> &func){
        for (int i = 0; i < _length; ++i)
            func(_data[i]);
    }

    template<typename U>
    void forEach(const function<U(T &value, int index)> &func){
        for (int i = 0; i < _length; ++i)
            func(_data[i],i);
    }

    MyArray &map(const function<T(T &value)> &func){
        for (int i = 0; i < _length; ++i)
            _data[i] = func(_data[i]);
        return *this;
    }

    MyArray &map(const function<T(T &value, int index)> &func){
        for (int i = 0; i < _length; ++i)
            _data[i] = func(_data[i],i);
        return *this;
    }

    int count(const function<bool(T &value)> &func){
        int k = 0;
        for (int i = 0; i < _length; ++i)
            if (func(_data[i]))++k;
        return k;
    }

    int count(const function<bool(T &value, int index)> &func){
        int k = 0;
        for (int i = 0; i < _length; ++i)
            if (func(_data[i], i))++k;
        return k;
    }

    int count(const T &value){
        int k = 0;
        for (int i = 0; i < _length; ++i)
            if (_data[i]==value)++k;
        return k;
    }

    bool any(const function<bool(T &value)> &func){
        for (size_t i = 0; i < _length; ++i) {
            if (func(_data[i]))return true;
        }
        return false;
    }

    bool all(const function<bool(T &value)> &func){
        for (size_t i = 0; i < _length; ++i) {
            if (!func(_data[i]))return false;
        }
        return true;
    }

    T reduce(const function<T (T &val1, T &val2)> &func){
        T value = operator[](0);
        for (int i = 1; i < _length; ++i) {
            value = func(value, _data[i]);
        }
        return value;
    }

    T reduce(const function<T (T &val1, T &val2)> &func, const T &startValue){
        T value = startValue;
        for (int i = 0; i < _length; ++i) {
            value = func(value, _data[i]);
        }
        return value;
    }

    MyArray filter(const function<bool(T &value)> &func){
        T* temp = new T[_length];
        int new_len = 0;
        for (int i = 0; i < _length; ++i) {
            if (func(_data[i]))temp[new_len++] = _data[i];
        }
        MyArray arr(temp,new_len);
        delete[] temp;
        return arr;
    }

    MyArray filter(const function<bool (T &value, int index)> &func){
        T* temp = new T[_length];
        int new_len = 0;
        for (int i = 0; i < _length; ++i) {
            if (func(_data[i],i))temp[new_len++] = _data[i];
        }
        MyArray arr(temp,new_len);
        delete[] temp;
        return arr;
    }
    
    int binarySearch(int key, int fromIndex, int toIndex) {
        if (fromIndex<0)fromIndex= _length + fromIndex;
        if (toIndex<0)toIndex= _length + toIndex;
        if (fromIndex>toIndex|| toIndex > _length)throw InvalidIndexException();
        int low = fromIndex;
        int high = toIndex - 1;

        while (low <= high) {
            int mid = (low + high) >> 1;
            int midVal = _data[mid];

            if (midVal < key)
                low = mid + 1;
            else if (midVal > key)
                high = mid - 1;
            else
                return mid; // key found
        }
        return -(low + 1);  // key not found.
    }

    int binarySearch(int key){
        return binarySearch(key, 0, _length);
    }

    MyArray slice(int start, int end){
        if (start<0)start= _length + start;
        if (end<0)end= _length + end;
        if (start>end|| end > _length)throw InvalidIndexException();

        return MyArray(end-start, _data+start);
    }

    MyArray slice(int end){
        return slice(0, end);
    }

    T* toArray(){
        if (_length == 0)return nullptr;
        T* arr = new T[_length];
        memcpy(arr,_data, sizeof(T) * _length);
        return arr;
    }

    T* getData(){
        return _data;
    }

    MyArray &reverse(){
        T temp;
        int index, end = _length >> 1u;
        for (int i = 0; i < end; ++i) {
            index = _length - i - 1;
            temp = _data[i];
            _data[i] = _data[index];
            _data[index] = temp;
        }
        return *this;
    }

    MyArray &sort(bool descending = false){
        sorts::heapSort(_data, _length);
        return *this;
    }

    T sum(){
        T sum = operator[](0);
        for (int i = 1; i < _length; ++i)
            sum+=_data[i];
        return sum;
    }

    //iterator
    template<typename ValueType>
    class MyIterator: public std::iterator<std::input_iterator_tag, ValueType>{
        friend class MyArray<ValueType>;//for private constructor
    private:
        ValueType* p;
        MyIterator(ValueType* p):p(p) {}

    public:
        MyIterator(const MyIterator &it):p(it.p){}

        bool operator!=(MyIterator const& other) const {
            return p != other.p;;
        }

        bool operator==(MyIterator const& other) const{
            return p == other.p;
        }


        MyIterator& operator++() {
            ++p;
            return *this;
        }
        MyIterator& operator--() {
            --p;
            return *this;
        }

        MyIterator& operator+=(int num) {
            p+=num;
            return *this;
        }

        MyIterator& operator-=(int num) {
            p-=num;
            return *this;
        }

        typename MyIterator<ValueType>::reference operator*() const{
            return *p;
        };
    };

    typedef MyIterator<T> iterator;
    typedef MyIterator<const T> const_iterator;

    iterator begin(){
        return iterator(_data);
    }

    iterator end(){
        return iterator(_data + _length);
    }

    const_iterator begin() const{
        return const_iterator(_data);
    }

    const_iterator end() const{
        return const_iterator(_data + _length);
    }

};

template <typename U>
std::ostream& operator << (std::ostream &out, const MyArray<U> &arr){
    out<< "[";
    for (int i = 0; i < arr._length - 1; ++i)
        out << arr._data[i] << ", ";
    if (arr._length)out << arr._data[arr._length - 1];
    return out << "]";
}

template<typename U> //cin
std::istream& operator >> (std::istream &in, MyArray<U> &arr){
    for (int i = 0; i < arr._length; ++i) {
        in>>arr[i];
    }
    return in;
}

#endif //MY_IDEAS_MYARRAY_H

//fill
//indexOf, lastIndexOf
//contains
//sum
//min
//max

//extend constructor
//copy constructor

//forEach
//map
//count(bool func)

//sort
//reverse

//equals
//compare---
//binary search
//hash ---

//slice

//dimensions---

//any
//all