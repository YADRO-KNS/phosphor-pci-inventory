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

#include "inventory.hpp"

#include <limits>
#include <phosphor-logging/log.hpp>

using namespace phosphor::logging;

/** DBus ObjectMapper interface */
static const char* ObjectMapperPath = "/xyz/openbmc_project/object_mapper";
static const char* ObjectMapperIface = "xyz.openbmc_project.ObjectMapper";

/** DBus Inventory interface */
static const char* InventoryPath = "/xyz/openbmc_project/inventory";
static const char* InventoryIface = "xyz.openbmc_project.Inventory.Manager";
static const char* CommonInventoryItem = "xyz.openbmc_project.Inventory.Item";
static const char* PciInventoryItem = "xyz.openbmc_project.Inventory.Item.PCI";
static const char* PciInventoryRoot = "/system/chassis/motherboard/";

/** Properties from xyz.openbmc_project.Inventory.Item interface */
static const char* PropPresent = "Present";
static const char* PropPrettyName = "PrettyName";
/** Properties from xyz.openbmc_project.Inventory.PCI interface */
static const char* PropLocation = "Location";
static const char* PropDeviceID = "DeviceID";
static const char* PropVendorID = "VendorID";
static const char* PropRevision = "Revision";
static const char* PropClassCode = "ClassCode";

/** Bits per byte */
constexpr uint8_t BITS_PER_BYTE = 8;

/** @brief Convert number to a hexadecimal string.
 *
 *  @param[in] val - value to convert
 *  @param[in] bits - number of significant bits
 *
 *  @return hexadecimal string representation of the source value
 */
template <typename T>
static std::string toHex(T val, uint8_t bits = sizeof(T) * BITS_PER_BYTE)
{
    // Number of significant bytes, we can't show less than half byte
    const int sbytes =
        bits / (BITS_PER_BYTE / 2) + (bits % (BITS_PER_BYTE / 2) ? 1 : 0);

    // Remove non-significant bits
    const uint8_t shift = sizeof(val) * BITS_PER_BYTE - bits;
    val &= std::numeric_limits<T>::max() >> shift;

    // Construct text string
    std::string text(sbytes + 3, 0); // 3 = prefix (0x) + terminating null byte
    const int pl = snprintf(&text[0], text.size(), "0x%0*x", sbytes, val);
    text.resize(static_cast<size_t>(pl));

    return text;
}

Inventory::Inventory() : bus_{sdbusplus::bus::new_default()}
{
}

void Inventory::reset()
{
    log<level::INFO>("Reset PCI inventory");

    // Get all existing PCI devices from inventory
    auto method = bus_.new_method_call(ObjectMapperIface, ObjectMapperPath,
                                       ObjectMapperIface, "GetSubTreePaths");
    method.append(std::string(InventoryPath));
    method.append(0);
    method.append(std::vector<std::string>({PciInventoryItem}));
    auto response = bus_.call(method);
    if (response.is_method_error())
    {
        log<level::ERR>("Failed to enumerate PCI inventory");
        return;
    }

    // Enumerate PCI devices and reset state (save empty data)
    std::vector<std::string> paths;
    response.read(paths);
    const size_t rootPartLen = strlen(InventoryPath);
    for (auto it = paths.begin(); it != paths.end(); ++it)
    {
        // Remove root part from inventory path
        const std::string path = it->substr(rootPartLen);
        // Reset state - it's impossible to remove inventory item, so we
        // write an empty description to corresponded path
        Object obj = createEmpty(path);
        saveObject(obj);
    }
}

void Inventory::add(const PciDevice& dev)
{
    Object obj = createFromDevice(dev);
    saveObject(obj);
}

Inventory::Object Inventory::createFromDevice(const PciDevice& dev) const
{
    const std::string objectPath = PciInventoryRoot + dev.getShortName();
    // clang-format off
    Object obj = {{
        objectPath, {
            {
                CommonInventoryItem, {
                    { PropPresent, true },
                    { PropPrettyName, dev.getPrettyName() }
                }
            },
            {
                PciInventoryItem, {
                    { PropLocation, dev.getLocation() },
                    { PropDeviceID, toHex(dev.deviceId) },
                    { PropVendorID, toHex(dev.vendorId) },
                    { PropRevision, toHex(dev.revision) },
                    { PropClassCode, toHex(dev.classCode, 24) }
                }
            }
        }
    }};
    return obj;
    // clang-format on
}

Inventory::Object Inventory::createEmpty(const std::string& path) const
{
    // clang-format off
    Object obj = {{
        path, {
            {
                CommonInventoryItem, {
                    { PropPresent, false },
                    { PropPrettyName, std::string() }
                }
            },
            {
                PciInventoryItem, {
                    { PropLocation, std::string() },
                    { PropDeviceID, std::string() },
                    { PropVendorID, std::string() },
                    { PropRevision, std::string() },
                    { PropClassCode, std::string() }
                }
            }
        }
    }};
    return obj;
    // clang-format on
}

void Inventory::saveObject(const Object& obj)
{
    auto method = bus_.new_method_call(InventoryIface, InventoryPath,
                                       InventoryIface, "Notify");
    method.append(obj);
    auto response = bus_.call(method);
    if (response.is_method_error())
    {
        log<level::ERR>("Failed to write PCI device description to inventory");
    }
}
