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

#include "workqueue.hpp"

#include "inventory.hpp"

#include <phosphor-logging/log.hpp>

using namespace phosphor::logging;

WorkQueue::WorkQueue() : thread_(&WorkQueue::workingThread, this)
{
}

WorkQueue::~WorkQueue()
{
    if (thread_.joinable())
    {
        cancel();
        thread_.join();
    }
}

void WorkQueue::push(const PciDevice& dev)
{
    std::unique_lock<std::mutex> lock(mutex_);
    queue_.push(dev);
    lock.unlock();
    cond_.notify_one();
}

void WorkQueue::reset()
{
    Queue empty;
    std::unique_lock<std::mutex> lock(mutex_);
    queue_.swap(queue_);
    lock.unlock();

    pendingReset_.store(true);
    cond_.notify_one();
}

void WorkQueue::cancel()
{
    pendingCancel_.store(true);
    cond_.notify_one();
}

void WorkQueue::workingThread()
{
    Inventory inv;

    while (!pendingCancel_)
    {
        try
        {
            if (pendingReset_)
            {
                pendingReset_ = false;
                inv.reset();
                continue;
            }
            std::unique_lock<std::mutex> lock(mutex_);
            if (queue_.empty())
            {
                cond_.wait(lock);
            }
            else
            {
                const PciDevice dev = queue_.front();
                queue_.pop();
                lock.unlock();
                inv.add(dev);
            }
        }
        catch (const std::exception& e)
        {
            log<level::ERR>("Unhandled exception at PCI working thread",
                            entry("EXCEPTION=%s", e.what()));
        }
    }
}
