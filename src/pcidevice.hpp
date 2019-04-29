/**
 * @brief PCI device description.
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

#include "ipmi.hpp"

#include <string>

/** @struct PciDevice
 *  @brief PCI device description.
 */
struct PciDevice : public IpmiPciDevice
{
    /** @brief Constructor.
     *
     *  @param[in] dev - base PCI device description
     */
    PciDevice(const IpmiPciDevice& dev);

    /** @brief Construct short unique name of the PCI device.
     *         Used as Device name in the inventory.
     *
     *  @return short name
     */
    std::string getShortName() const;

    /** @brief Construct location to the PCI device.
     *
     *  @return PCI device location
     */
    std::string getLocation() const;

    /** @brief Construct pretty name of the PCI device.
     *
     *  @return pretty name of PCI device
     */
    std::string getPrettyName() const;
};
