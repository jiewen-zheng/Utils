//
// Created by moon on 2023/10/29.
//

#ifndef FIFO_ARRAY_H
#define FIFO_ARRAY_H

#include "cstdint"

template<typename T>
class FifoArray {
public:
    FifoArray();
    FifoArray(size_t size, bool cover = true);
    ~FifoArray();

    void linkMemory(T* t, size_t size);

    uint32_t available();
    bool write(T t);
    bool write(T *t, uint32_t size);
    T read();
    uint32_t read(T *t, uint32_t size);
    void clear();

private:
    T        *memory = nullptr;
    bool     cover   = true;
    size_t _size;
    uint32_t occupy;

    T *writePtr;
    T *readPtr;
};


#endif //FIFO_ARRAY_H
