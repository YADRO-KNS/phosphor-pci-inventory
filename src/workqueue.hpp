/**
 * @brief Working queue.
 *
 * Copyright (c) 2019 YADRO
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include "pcidevice.hpp"

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <thread>

/** @class WorkQueue
 *  @brief Synchronized FIFO queue, each element processed in a working thread.
 */
class WorkQueue
{
  public:
    WorkQueue();
    ~WorkQueue();

    /** @brief Push item to queue.
     *
     *  @param[in] dev - PCI device description to push
     */
    void push(const PciDevice& dev);

    /** @brief Reset PCI device list.
     *         Using to notify the waiting thread that new session has begun.
     */
    void reset();

    /** @brief Cancel queue processing.
     *         Using to notify the waiting thread that it must be terminated.
     */
    void cancel();

  private:
    /** @brief Working thread routine. */
    void workingThread();

  private:
    using Queue = std::queue<PciDevice>;

    /** @brief Queue container. */
    Queue queue_;
    /** @brief Working thread. */
    std::thread thread_;
    /** @brief Synchronization object. */
    std::mutex mutex_;
    /** @brief Event notifier. */
    std::condition_variable cond_;
    /** @brief Pending event: cancel processing. */
    std::atomic_bool pendingCancel_ = false;
    /** @brief Pending event: reset PCI device list. */
    std::atomic_bool pendingReset_ = false;
};
