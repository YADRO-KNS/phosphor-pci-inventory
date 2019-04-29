# PCI Inventory
Plug-in module (shared library) for phosphor-ipmi-host, used to store PCI
device list in the OpenBMC inventory.

## Architecture
At the host system startup, Skiboot sends the list of installed PCI devices
to the OpenBMC via IPMI (OEM messages).
This project implements an IPMI OEM message handler to process incoming data
and write PCI device descriptions to the OpenBMC inventory.

The followed scheme describes the synchronization sequence in a general way.
Implementation has a cache queue to perform all long-time operation
in a separated thread, which allows to send IPMI response to Skiboot without
waiting for completion of PCI device registration inside the OpenBMC inventory
manager.
```
      Skiboot                IPMI OEM handler      OpenBMC Inventory
      -------             ---------------------    -----------------
         |                Message |   | Working            |
         |                handler | Q | thread             |
         |                --------| U |--------            |
         |                        | E |                    |
         | IPMI PCI Device 1 info | U |                    |
         |     reset flag = 1     | E |                    |
         |----------------------->|   |                    |
         |   IPMI Response (OK)   |-->|   Reset PCI list   |
         |<-----------------------|   |------------------->|
         |                        |   |   Add PCI device   |
         |                        |   |------------------->|
         | IPMI PCI Device 2 info |   |                    |
         |     reset flag = 0     |   |                    |
         |----------------------->|   |                    |
         |   IPMI Response (OK)   |-->|   Add PCI device   |
         |<-----------------------|   |------------------->|
                         . . . . . . . . . . .
         | IPMI PCI Device N info |   |                    |
         |     reset flag = 0     |   |                    |
         |----------------------->|   |                    |
         |   IPMI Response (OK)   |-->|   Add PCI device   |
         |<-----------------------|   |------------------->|
```

## IPMI OEM message format

| Position | Size | Value    | Description |
| -------- | ---- | -------- | ----------- |
| 0        | 1    | 0x2e     | NetFn OEM |
| 1        | 1    | 0x2a     | Command number |
| 2        | 3    | 0x00c269 | IANA ID (YADRO) |
| 5        | 1    | 0 or 1   | Reset flag |
| 6        | 14   | Any      | PCE device description |

## Build
Build scripts of the project based on autotools:
1. Remake the GNU Build System files:
   `./bootstrap.sh`
2. Configure the project:
   `./configure`
3. Build the library:
   `make`

## Install
The library must be placed into the directory of IPMI providers, usually
`/usr/lib/ipmid-providers`.

### Prerequisites
1. Interface for a PCI device inventory item must be registered in OpenBMC via
   `phosphor-dbus-interfaces`. Example yaml file:
   `./xyz/openbmc_project/Inventory/Item/PCI.interface.yaml`;
   It is necessary to rebuild dbus-interfaces project to apply changes.
2. White list of allowed IPMI messages must contain a rule to pass OEM command
   with PCI device descriptions. This can be done by adding the following line
   to `phosphor-host-ipmid/host-ipmid-whitelist.conf`:
   `0x3A:0x2a    //<OEM IBM>:<Register PCI device>`
   It is necessary to rebuild host-ipmid project to apply changes.
