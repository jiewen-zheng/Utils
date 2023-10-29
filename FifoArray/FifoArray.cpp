//
// Created by moon on 2023/10/29.
//

#include "FifoArray.h"

template<typename T>
FifoArray<T>::FifoArray() {
    memory = nullptr;
    this->_size = 0;
    this->cover = false;
    occupy = 0;

    writePtr = nullptr;
    readPtr  = nullptr;
}

template<typename T>
FifoArray<T>::FifoArray(size_t size, bool cover) {
    T *tmp = new T[size];
    if (!tmp)
        return;

    memory = tmp;
    this->_size = size;
    this->cover = cover;
    occupy = 0;

    writePtr = memory;
    readPtr  = memory;
}

template<typename T>
FifoArray<T>::~FifoArray() {
    delete[]memory;
    memory = nullptr;
}

template<typename T>
void FifoArray<T>::linkMemory(T *t, size_t size) {
    if (t == nullptr)
        return;

    memory = t;
    _size  = size;
}

template<typename T>
uint32_t FifoArray<T>::available() {
    return occupy;
}

template<typename T>
bool FifoArray<T>::write(T t) {
    if (!memory) return false;

    uint32_t unoccupied = _size - occupy;

    if (unoccupied == 0 && !cover)
        return false;

    *writePtr++ = t;

    if (writePtr >= memory + _size) {
        writePtr = memory;
    }

    occupy++;
    return true;
}

template<typename T>
bool FifoArray<T>::write(T *t, uint32_t size) {
    if (!memory) return false;

    uint32_t unoccupied = size - occupy;
    if (unoccupied == 0 && !cover)
        return false;

    if (unoccupied < size) size = cover ? size : unoccupied;

    while (size--) {
        *writePtr++ = *t++;

        if (writePtr >= memory + _size) {
            writePtr = memory;
        }
        occupy      = (occupy < _size) ? occupy + 1 : size;
    }

    return true;
}

template<typename T>
T FifoArray<T>::read() {
    if (!memory || occupy == 0) return false;

    T data = *readPtr++;
    if (writePtr >= memory + _size) {
        writePtr = memory;
    }
    occupy--;
    return data;
}