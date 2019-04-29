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

#include "pcidevice.hpp"

#include <stdio.h>

PciDevice::PciDevice(const IpmiPciDevice& dev) : IpmiPciDevice(dev)
{
    // Convert PCI device description to native structure,
    // original IPMI packet contains data in BE byte order
    domainNumber = be16toh(domainNumber);
    vendorId = be16toh(vendorId);
    deviceId = be16toh(deviceId);
    classCode = be32toh(classCode);
}

std::string PciDevice::getShortName() const
{
    char name[16];
    snprintf(name, sizeof(name), "PCI%04x%02x%02x%x", domainNumber, busNumber,
             deviceNumber, functionNumber);
    return name;
}

std::string PciDevice::getLocation() const
{
    char location[16];
    snprintf(location, sizeof(location), "%04x:%02x:%02x.%x", domainNumber,
             busNumber, deviceNumber, functionNumber);
    return location;
}

std::string PciDevice::getPrettyName() const
{
    // From https://pci-ids.ucw.cz/read/PD

    const uint8_t cl = classCode >> 16;
    const uint8_t sub = (classCode >> 8) & 0xff;
    const uint8_t pif = classCode & 0xff;

    // clang-format off
    switch (cl) {
        case 0x00:
            switch (sub) {
                case 0x00: return "Non-VGA unclassified device";
                case 0x01: return "VGA compatible unclassified device";
                default:   return "Unclassified device";
            }
        case 0x01:
            switch (sub) {
                case 0x00: return "SCSI storage controller";
                case 0x01: return "IDE interface";
                case 0x02: return "Floppy disk controller";
                case 0x03: return "IPI bus controller";
                case 0x04: return "RAID bus controller";
                case 0x05: return "ATA controller";
                case 0x06: return "SATA controller";
                case 0x07: return "Serial Attached SCSI controller";
                case 0x08: return "Non-Volatile memory controller";
                default:   return "Mass storage controller";
            }
        case 0x02:
            switch (sub) {
                case 0x00: return "Ethernet controller";
                case 0x01: return "Token ring network controller";
                case 0x02: return "FDDI network controller";
                case 0x03: return "ATM network controller";
                case 0x04: return "ISDN controller";
                case 0x05: return "WorldFip controller";
                case 0x06: return "PICMG controller";
                case 0x07: return "Infiniband controller";
                case 0x08: return "Fabric controller";
                default:   return "Network controller";
            }
        case 0x03:
            switch (sub) {
                case 0x00: return "VGA compatible controller";
                case 0x01: return "XGA compatible controller";
                case 0x02: return "3D controller";
                default:   return "Display controller";
            }
        case 0x04:
            switch (sub) {
                case 0x00: return "Multimedia video controller";
                case 0x01: return "Multimedia audio controller";
                case 0x02: return "Computer telephony device";
                case 0x03: return "Audio device";
                default:   return "Multimedia controller";
            }
        case 0x05:
            switch (sub) {
                case 0x00: return "RAM memory";
                case 0x01: return "FLASH memory";
                default:   return "Memory controller";
            }
        case 0x06:
            switch (sub) {
                case 0x00: return "Host bridge";
                case 0x01: return "ISA bridge";
                case 0x02: return "EISA bridge";
                case 0x03: return "MicroChannel bridge";
                case 0x04: return "PCI bridge";
                case 0x05: return "PCMCIA bridge";
                case 0x06: return "NuBus bridge";
                case 0x07: return "CardBus bridge";
                case 0x08: return "RACEway bridge";
                case 0x09: return "Semi-transparent PCI-to-PCI bridge";
                case 0x0a: return "InfiniBand to PCI host bridge";
                default:   return "Bridge";
            }
        case 0x07:
            switch (sub) {
                case 0x00: return "Serial controller";
                case 0x01: return "Parallel controller";
                case 0x02: return "Multiport serial controller";
                case 0x03: return "Modem";
                case 0x04: return "GPIB controller";
                case 0x05: return "Smard Card controller";
                default:   return "Communication controller";
            }
        case 0x08:
            switch (sub) {
                case 0x00: return "PIC";
                case 0x01: return "DMA controller";
                case 0x02: return "Timer";
                case 0x03: return "RTC";
                case 0x04: return "PCI Hot-plug controller";
                case 0x05: return "SD Host controller";
                case 0x06: return "IOMMU";
                default:   return "System peripheral";
            }
        case 0x09:
            switch (sub) {
                case 0x00: return "Keyboard controller";
                case 0x01: return "Digitizer Pen";
                case 0x02: return "Mouse controller";
                case 0x03: return "Scanner controller";
                case 0x04: return "Gameport controller";
                default:   return "Input device controller";
            }
        case 0x0a:
            switch (sub) {
                case 0x00: return "Generic Docking Station";
                default:   return "Docking Station";
            }
        case 0x0b:
            switch (sub) {
                case 0x00: return "386";
                case 0x01: return "486";
                case 0x02: return "Pentium";
                case 0x10: return "Alpha";
                case 0x20: return "Power PC";
                case 0x30: return "MIPS";
                case 0x40: return "Co-processor";
                default:   return "CPU";
            }
        case 0x0c:
            switch (sub) {
                case 0x00: return "FireWire (IEEE 1394)";
                case 0x01: return "ACCESS Bus";
                case 0x02: return "SSA";
                case 0x03:
                   switch (pif) {
                       case 0x00: return "USB-UHCI";
                       case 0x10: return "USB-OHCI";
                       case 0x20: return "USB-EHCI";
                       case 0x30: return "USB-XHCI";
                       case 0xfe: return "USB Device";
                       default:   return "USB";
                   }
                case 0x04: return "Fibre Channel";
                case 0x05: return "SMBus";
                case 0x06: return "InfiniBand";
                case 0x07: return "IPMI Interface";
                case 0x08: return "SERCOS interface";
                case 0x09: return "CANBUS";
                default:   return "Serial bus controller";
            }
        case 0x0d:
            switch (sub) {
                case 0x00: return "IRDA controller";
                case 0x01: return "Consumer IR controller";
                case 0x10: return "RF controller";
                case 0x11: return "Bluetoothtooth";
                case 0x12: return "Broadband";
                case 0x20: return "802.1a controller";
                case 0x21: return "802.1b controller";
                default:   return "Wireless controller";
            }
        case 0x0e: return "Intelligent controller";
        case 0x0f:
           switch(sub) {
               case 0x01: return "Satellite TV controller";
               case 0x02: return "Satellite audio communication controller";
               case 0x03: return "Satellite voice communication controller";
               case 0x04: return "Satellite data communication controller";
               default:   return "Satellite communications controller";
           }
        case 0x10:
           switch(sub) {
               case 0x00: return "Network and computing encryption device";
               case 0x01: return "Entertainment encryption device";
               default:   return "Encryption controller";
           }
        case 0x011:
           switch(sub) {
               case 0x00: return "DPIO module";
               case 0x01: return "Performance counters";
               case 0x10: return "Communication synchronizer";
               case 0x20: return "Signal processing management";
               default:   return "Signal processing controller";
           }
        case 0x12: return "Processing accelerators";
        case 0x13: return "Non-Essential Instrumentation";
        case 0x40: return "Coprocessor";
        default: return "PCI device";
    }
    // clang-format on
}
