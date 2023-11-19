// MessageQueue.h
#pragma once
#include <functional>
#include <memory>
#include <mutex>
#include <queue>
#include <future>

/*
 * File: MessageQueue.h
 * Description: Manages asynchronous message processing within the After Effects plugin environment.
 *
 * Guidelines for Contributors:
 * 1. Understand the Flow: Grasp how messages are enqueued, processed, and dequeued.
 * 2. Thread Safety: Ensure that any modifications or additions respect the thread-safe nature of the queue.
 * 3. Interoperability: New messages or modifications should integrate seamlessly with existing asynchronous tasks.
 * 4. No Alteration: This file should not be changed. Understanding its functionality is key for effective contributions elsewhere.
 */


class IAsyncMessage {
public:
    virtual ~IAsyncMessage() = default;
    virtual void execute() = 0;
    virtual void wait() = 0;
};

template<typename T>
class AESyncMessage : public IAsyncMessage {
    std::function<T()> task;
    std::promise<T> finished;

public:
    std::future<T> resultFuture;

    AESyncMessage(std::function<T()> taskFunc) : task(std::move(taskFunc)) {
        resultFuture = finished.get_future();
    }

    void execute() {
        if (task) {
            finished.set_value(task());
        }
    }

    T getResult() {
        return resultFuture.get();
    }

    void wait() override {
        resultFuture.wait();
    }
};

// Specialization for void
template<>
class AESyncMessage<void> : public IAsyncMessage {
    std::function<void()> task;
    std::promise<void> finished;

public:
    std::future<void> resultFuture;

    AESyncMessage(std::function<void()> taskFunc) : task(std::move(taskFunc)) {
        resultFuture = finished.get_future();
    }

    void execute() {
        if (task) {
            task();
            finished.set_value();
        }
    }

};


class MessageQueue {
private:
    std::queue<std::shared_ptr<IAsyncMessage>> queue;
    std::mutex queueMutex;

    // Private constructor
    MessageQueue() {}

    // Deleted copy constructor and assignment operator
    MessageQueue(const MessageQueue&) = delete;
    MessageQueue& operator=(const MessageQueue&) = delete;

public:
    // Static method for accessing the singleton instance
    static MessageQueue& getInstance() {
        static MessageQueue instance;
        return instance;
    }

    void enqueue(std::shared_ptr<IAsyncMessage> message) {
        std::lock_guard<std::mutex> lock(queueMutex);
        queue.push(message);
    }

    std::shared_ptr<IAsyncMessage> dequeue() {
        std::lock_guard<std::mutex> lock(queueMutex);
        if (!queue.empty()) {
            auto& message = queue.front();
            queue.pop();
            return message;
        }
        return nullptr;
    }
};
