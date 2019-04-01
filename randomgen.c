#include "randomgen.h"
#include <time.h>
#include "mathutils.h"

static unsigned seed = 0;
static unsigned starting_seed = 0;

static void set_seed()
{
    seed = time(0);
    starting_seed = seed;
}

//Marsaglia's XOR shift
static void scramble()
{
    if (!seed)
    {
        set_seed();
    }
    seed ^= seed << 13;
    seed ^= seed >> 17;
    seed ^= seed << 5;
}

unsigned get_rand_int(int a, int b)
{
    int high, low;
    high = max(a, b);
    low = min(a, b);
    scramble();
    return seed % (high - low + 1) + low;
}

int get_rand_bool()
{
    scramble();
    return (seed & 1) == 1;
}

unsigned get_seed()
{
    return starting_seed;
}

unsigned get_state()
{
    return seed;
}