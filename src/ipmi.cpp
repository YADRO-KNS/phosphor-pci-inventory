/**
 * @brief IPMI OEM message handler.
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

#include "ipmi.hpp"

#include "workqueue.hpp"

#include <ipmid/api.hpp>
#include <phosphor-logging/log.hpp>

using namespace phosphor::logging;

/** @brief Working queue. */
WorkQueue workQueue_;

/** @brief Callback - IPMI OEM message handler.
 *
 *  Unfortunately, current IPMI API can't work with structures in input
 *  parameters, to solve this problem we use fixed size array which will be
 *  interpreted as a pointer to OEM packet.
 *
 *  @param[in] payload - message's payload data
 */
static ipmi::RspType<> pciInventoryHandler(
    const std::array<uint8_t, sizeof(IpmiPciPacket)>& payload)
{
    const IpmiPciPacket* pack =
        reinterpret_cast<const IpmiPciPacket*>(payload.data());

    if (pack->reset)
    {
        workQueue_.reset();
    }

    workQueue_.push(pack->device);

    return ipmi::responseSuccess();
}

/** @brief Register IPMI OEM message handler. */
void registerPciInventoryHandler() __attribute__((constructor));
void registerPciInventoryHandler()
{
    log<level::INFO>("Registering PCI inventory handler");
    ipmi::registerOemHandler(ipmi::prioOpenBmcBase, PCIINV_IANA_YADRO,
                             PCIINV_IPMI_CMD, ipmi::Privilege::Admin,
                             pciInventoryHandler);
}
