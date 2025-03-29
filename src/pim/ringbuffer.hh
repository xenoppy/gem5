#ifndef RINGBUFFER_H
#define RINGBUFFER_H
#include <vector>
#include <atomic>
template<typename T>
class RingBuffer {
public:
    RingBuffer(size_t size) : buffer(size), capacity(size){}

    void set(std::atomic<size_t> &tail, const T& item) {
        buffer[tail.fetch_add(1)% capacity] = item;
        return;
    }

    void get(std::atomic<size_t> &head, T& item) {
        buffer[head.fetch_add(1)% capacity] = item;
        return;
    }

private:
    std::vector<T> buffer;
    size_t capacity;
};
#endif // RINGBUFFER_H
