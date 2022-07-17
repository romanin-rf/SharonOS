#ifndef _PCI_H
#define _PCI_H

#include "types.h"

#define PCI_CONFIG_PORT      0x0CF8
#define PCI_DATA_PORT        0x0CFC

#define PCI_MAX_BUSES        255
#define PCI_MAX_DEVICES      32
#define PCI_MAX_FUNCTIONS    8

#define PCI_HEADERTYPE_NORMAL        0
#define PCI_HEADERTYPE_BRIDGE        1
#define PCI_HEADERTYPE_CARDBUS       2
#define PCI_HEADERTYPE_MULTIFUNC     0x80

typedef union 
{
	struct
	{
		u16 vendorID;
		u16 deviceID;
		u16 commandReg;
		u16 statusReg;
		u8 revisionID;
		u8 progIF;
		u8 subClassCode;
		u8 classCode;
		u8 cachelineSize;
		u8 latency;
		u8 headerType;
		u8 BIST;
	} __attribute__((packed)) option;
	u32 header[4];
} __attribute__((packed)) PCIDevHeader;

void ReadConfig32(u32 bus, u32 dev, u32 func, u32 reg, u32 *data);
char *GetPCIDevClassName(u32 class_code);
void PCIScan();

#endif