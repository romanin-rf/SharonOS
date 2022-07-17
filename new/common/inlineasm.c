unsigned char inb (unsigned short _port)
{
    unsigned char rv;
    __asm__ ("inb %0, %1" : "=a" (rv) : "dN" (_port));
    return rv;
}

void outb (unsigned short _port, unsigned char _data)
{
    __asm__ ("outb %0, %1" : : "dN" (_port), "a" (_data));
}

unsigned char inw (unsigned short _port)
{
    unsigned char rv;
    __asm__ ("inw %0, %1" : "=a" (rv) : "dN" (_port));
    return rv;
}

void outw (unsigned short _port, unsigned char _data)
{
    __asm__ ("outw %0, %1" : : "dN" (_port), "a" (_data));
}