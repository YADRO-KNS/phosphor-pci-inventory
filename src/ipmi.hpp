/**
 * @brief IPMI OEM message definitions.
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

#include <stdint.h>

/** @brief NetFn used to send PCI device description (OEM). */
constexpr uint8_t PCIINV_IPMI_NETFN = 0x2e;
/** @brief Command number used to send PCI device description. */
constexpr uint8_t PCIINV_IPMI_CMD = 0x2a;
/** @brief IANA number of YADRO, used to identify OEM command group. */
constexpr uint16_t PCIINV_IANA_YADRO = 49769;

/** @struct IpmiPciDevice
 *  @brief PCI device description (originally comes in BE byte order format).
 */
struct IpmiPciDevice
{
    /** @brief Domain number. */
    uint16_t domainNumber;
    /** @brief Bus number. */
    uint8_t busNumber;
    /** @brief Device number. */
    uint8_t deviceNumber;
    /** @brief Function number. */
    uint8_t functionNumber;
    /** @brief Vendor Id. */
    uint16_t vendorId;
    /** @brief Device Id. */
    uint16_t deviceId;
    /** @brief Device class code. */
    uint32_t classCode;
    /** @brief Revision number. */
    uint8_t revision;
} __attribute__((packed));

/** @struct IpmiPciMessage
 *  @brief IPMI OEM message packet with PCI device description.
 *
 *  Each IPMI PCI message that skiboot sends begins with a 24-bits IANA number.
 *  Unlike the skiboot message structure, the plug-in implementation on the
 *  BMC-side doesn't contain this field, because the IPMI subsystem uses it
 *  to search for an appropriate OEM handler, this part of the IPMI message
 *  content is automatically truncated before the data is passed to the OEM
 *  handler.
 */
struct IpmiPciMessage
{
    /** @brief Reset flag. */
    uint8_t reset;
    /** @brief PCI device description. */
    IpmiPciDevice device;
} __attribute__((packed));
