static short *video = reinterpret_cast<short *>(0xb8000);
static short color = 0x0700;

typedef unsigned long uintptr_t;

void putc(const char c)
{
    if (c == '\r')
    {
        video = reinterpret_cast<short *>(reinterpret_cast<uintptr_t>(video) - ((reinterpret_cast<uintptr_t>(video) - 0xb8000) % 160));
    }
    else if (c == '\n')
    {
        video = reinterpret_cast<short *>(reinterpret_cast<uintptr_t>(video) - ((reinterpret_cast<uintptr_t>(video) - 0xb8000) % 160) + 160);
    }
    else
    {
    }

    *(video++) = color | c;
}

void puts(const char *s, int n)
{
    while (*s && n)
    {
        putc(*s);

        s++;
        n--;
    }
}

extern "C" void kernel()
{
    puts("Hello, world!", 13);

    while (true)
    {
        asm (
            "cli \n"
            "hlt \n"
        );
    }
}

extern "C" void loader_bootstrap()
{
    kernel();
}
