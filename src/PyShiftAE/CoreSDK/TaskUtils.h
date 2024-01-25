// TaskUtils.h
#pragma once
#include "MessageQueue.h"
#include "SuiteManager.h"

#include <tuple>

/*
 * File: TaskUtils.h
 * Description: Provides utility functions for enqueuing and managing tasks within the plugin.
 *
 * Guidelines for Contributors:
 * 1. Task Management: Familiarize yourself with how tasks are created, enqueued, and managed.
 * 2. Asynchronous Processing: Understand the asynchronous nature of the tasks being handled.
 * 3. Reusability: Write code that is reusable and easily integrated into the existing task management system.
 * 4. No Alteration: Changes to this file are not advised to maintain the integrity of task processing.
 */

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

    AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
    suites.UtilitySuite6()->AEGP_CauseIdleRoutinesToBeCalled();
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