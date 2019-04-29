/**
 * @brief OpenBMC PCI inventory.
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

#include <sdbusplus/bus.hpp>

/** @class Inventory
 *  @brief PCI inventory support.
 */
class Inventory
{
  public:
    /** @brief Constructor. */
    Inventory();

    /** @brief Reset the PCI device objects description in the inventory.
     *         The function removes all properties and sets the Present flag to
     *         false for all existing PCI devices in the inventory.
     */
    void reset();

    /** @brief Add PCI device to the inventory.
     *
     *  @param[in] dev - PCI device description
     */
    void add(const PciDevice& dev);

  private:
    using Properties =
        std::map<std::string, sdbusplus::message::variant<std::string, bool>>;
    using Interfaces = std::map<std::string, Properties>;
    using Object = std::map<sdbusplus::message::object_path, Interfaces>;

    /** @brief Create an inventory object from PCI device description.
     *
     *  @param[in] obj - PCI device description
     *
     *  @return inventory object
     */
    Object createFromDevice(const PciDevice& dev) const;

    /** @brief Create an empty inventory object.
     *
     *  @param[in] path - path to the PCI device object description
     *
     *  @return inventory object
     */
    Object createEmpty(const std::string& path) const;

    /** @brief Save the object to the inventory.
     *
     *  @param[in] obj - inventory object to save
     */
    void saveObject(const Object& obj);

  private:
    /** @brief DBus connection. */
    sdbusplus::bus::bus bus_;
};
