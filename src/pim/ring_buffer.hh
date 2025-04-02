#ifndef RINGBUFFER_H
#define RINGBUFFER_H
#include <vector>
#include <atomic>
#include <cassert>
template<typename T>
class RingBuffer {
public:
    RingBuffer(size_t size) : buffer(size), capacity(size){}

    void set(size_t tail, const T& item) {
        assert(tail<capacity);
        printf("entering set: tail=%ld\n", tail);
        buffer[tail] = item;
        return;
    }

    void get(size_t head, T& item) {
        assert(head<capacity);
        item=buffer[head];
        return;
    }
    bool is_empty(size_t head, size_t tail) {
        return head==tail;
    }
    bool is_full(size_t head, size_t tail) {
        return (tail+1)%capacity==head;
    }
    size_t get_size() {
        return capacity;
    }

private:
    std::vector<T> buffer;
    size_t capacity;
};
#endif // RINGBUFFER_H
