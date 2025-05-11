#pragma once

#include <atomic>
#include <vector>
#include <optional>
#include <iostream>

/**
 * Lock Free Ring buffer
 * Since orders are atomic we can have these be the bid and ask priority queues
 */
template<typename T>
class LockFreeOrderQueue {
    public: 
        explicit LockFreeOrderQueue(size_t size)
         : buffer(size), capacity(size), head(0), tail(0) {}

         bool enqueue(const T& order) {
            size_t curr_tail;

            do {
                curr_tail = tail.load(std::memory_order_relaxed);
                size_t next_tail = increment(curr_tail);
                if (next_tail == head.load(std::memory_order_acquire)) {
                    //buffer full
                    return false;
                }
            } while (!tail.compare_exchange_weak(curr_tail, increment(curr_tail), std::memory_order_acquire, std::memory_order_relaxed));
            
            buffer[curr_tail] = order;
            return true;
        }

        std::optional<T> dequeue() {
            size_t curr_head = head.load(std::memory_order_relaxed);

            if (curr_head == tail.load(std::memory_order_acquire)) {
                // buffer empty
                return std::nullopt;
            }

            T item = buffer[curr_head];
            head.store(increment(curr_head), std::memory_order_release);
            return item;
        }

    private:
        //fun circles
        size_t increment(size_t index) const {
            return (index + 1) % capacity;
        }

        std::vector<T> buffer;
        const size_t capacity;

        std::atomic<size_t> head;
        std::atomic<size_t> tail;
};