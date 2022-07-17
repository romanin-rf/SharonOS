#include "types.h"
#include "printf.h"
#include "io.h"
#include "pci.h"

typedef struct 
{
	u32 class_code;
	char name[32];
} PCIClassName;

static PCIClassName g_PCIClassNames[] = 
{
	{ 0x00, "before PCI 2.0"},
	{ 0x01, "disk controller"},
	{ 0x02, "network interface"},
	{ 0x03, "graphics adapter"},
	{ 0x04, "multimedia controller"},
	{ 0x05, "memory controller"},
	{ 0x06, "bridge device"},
	{ 0x07, "communication controller"},
	{ 0x08, "system device"},
	{ 0x09, "input device"},
	{ 0x0a, "docking station"},
	{ 0x0b, "CPU"},
	{ 0x0c, "serial bus"},
	{ 0x0d, "wireless controller"},
	{ 0x0e, "intelligent I/O controller"},
	{ 0x0f, "satellite controller"},
	{ 0x10, "encryption controller"},
	{ 0x11, "signal processing controller"},
	{ 0xFF, "proprietary device"}
};

typedef union
{
	struct
	{
		u32 zero 		: 2;
		u32 reg_num     : 6;
		u32 func_num    : 3;
		u32 dev_num     : 5;
		u32 bus_num     : 8;
		u32 reserved    : 7;
		u32 enable_bit  : 1;
	};
	u32 val;
} PCIConfigAddres;

void ReadConfig32(u32 bus, u32 dev, u32 func, u32 reg, u32 *data)
{
	PCIConfigAddres addr;
	
	addr.val = 0;
	addr.enable_bit = 1;
	addr.reg_num =  reg;
	addr.func_num = func;
	addr.dev_num =  dev;
	addr.bus_num =  bus;		

	out32(PCI_CONFIG_PORT, addr.val);
	in32(PCI_DATA_PORT, data);
	return;
}

char *GetPCIDevClassName(u32 class_code)
{
	int i;
	for (i = 0; i < sizeof(g_PCIClassNames)/sizeof(g_PCIClassNames[0]); i++)
	{
		if (g_PCIClassNames[i].class_code == class_code)
			return g_PCIClassNames[i].name;
	}
	return NULL;
}

int ReadPCIDevHeader(u32 bus, u32 dev, u32 func, PCIDevHeader *p_pciDevice)
{
	int i;
	
	if (p_pciDevice == 0)
		return 1;
	
	for (i = 0; i < sizeof(p_pciDevice->header)/sizeof(p_pciDevice->header[0]); i++)
		ReadConfig32(bus, dev, func, i, &p_pciDevice->header[i]);
		
	if (p_pciDevice->option.vendorID == 0x0000 || 
		p_pciDevice->option.vendorID == 0xffff ||
		p_pciDevice->option.deviceID == 0xffff)
		return 1;
		
	return 0;
}

void PrintPCIDevHeader(u32 bus, u32 dev, u32 func, PCIDevHeader *p_pciDevice)
{
	char *class_name;
	
	printf("bus=0x%x dev=0x%x func=0x%x venID=0x%x devID=0x%x",
			bus, dev, func, p_pciDevice->option.vendorID, p_pciDevice->option.deviceID);
			
	class_name = GetPCIDevClassName(p_pciDevice->option.classCode);
	if (class_name)
		printf(" class_name=%s", class_name);
		
	printf("\n");
}

void PCIScan(void)
{
	int bus;
	int dev;
	
	for (bus = 0; bus < PCI_MAX_BUSES; bus++)
		for (dev = 0; dev < PCI_MAX_DEVICES; dev++)
		{
			u32 func = 0;
			PCIDevHeader pci_device;
			
			if (ReadPCIDevHeader(bus, dev, func, &pci_device))
				continue;
				
			PrintPCIDevHeader(bus, dev, func, &pci_device);
			
			if (pci_device.option.headerType & PCI_HEADERTYPE_MULTIFUNC)
			{
				for (func = 1; func < PCI_MAX_FUNCTIONS; func++)
				{
					if (ReadPCIDevHeader(bus, dev, func, &pci_device))
						continue;
					PrintPCIDevHeader(bus, dev, func, &pci_device);
				}
			}
		}
}
