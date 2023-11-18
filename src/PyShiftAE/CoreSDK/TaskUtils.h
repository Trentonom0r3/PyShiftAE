// TaskUtils.h
#pragma once
#include "MessageQueue.h"

#include <tuple>

template<typename Func, typename... Args>
auto enqueueSyncTask(Func&& func, Args&&... args) {
    using ReturnType = std::invoke_result_t<Func, Args...>;

    auto task = [func = std::forward<Func>(func), argsTuple = std::make_tuple(std::forward<Args>(args)...)]() mutable -> ReturnType {
        return std::apply(func, argsTuple);
    };

    auto message = std::make_shared<AESyncMessage<ReturnType>>(std::move(task));

    // Enqueue the message and return it
    // Ensure that `queue` is accessible and properly declared
    MessageQueue::getInstance().enqueue(message);
    return message;
}



/*

AEGP_ItemH someFunction(AEGP_ItemH itemHandle) {
    auto message = enqueueSyncTask(createNewItem, itemHandle);
    message->wait(); // Wait for the task to complete

    return message->getResult<AEGP_ItemH>();
}

Need to bring in the wrapper funcs from my new version, and utilize those.

Then the python classes will be exposed through using the wrapped message queue funcs.

*/