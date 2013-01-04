#include "cstring"

namespace std
{
    // TODO: missing

    void *memset(void *dest, int ch, std::size_t count)
    {
        char *xs = reinterpret_cast<char *>(dest);

        while (count--)
        {
            *xs++ = ch;
        }

        return dest;
    }

    void *memcpy(void *dest, const void *src, std::size_t count)
    {
        if (dest == src)
        {
            return dest;
        }

        char *xd = reinterpret_cast<char *>(dest);
        const char *xs = reinterpret_cast<const char *>(src);

        while (count--)
        {
            *xd++ = *xs++;
        }

        return dest;
    }

    void *memmove(void *dest, const void *src, std::size_t count)
    {
        if (dest == src)
        {
            return dest;
        }

        char *xd;
        const char *xs;

        if (dest <= src)
        {
            xd = reinterpret_cast<char *>(dest);
            xs = reinterpret_cast<const char *>(src);

            while (count--)
            {
                *xd++ = *xs++;
            }
        }
        else
        {
            xd = reinterpret_cast<char *>(dest);
            xs = reinterpret_cast<const char *>(src);
            xd += count;
            xs += count;

            while (count--)
            {
                *--xd = *--xs;
            }
        }

        return dest;
    }

    // TODO: missing
}
