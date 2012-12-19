extern "C" void kernel()
{
    while (true)
    {
        asm (
            "cli \n"
            "hlt \n"
        );
    }
}
