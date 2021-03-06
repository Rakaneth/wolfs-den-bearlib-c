#include "mathutils.h"
int max(int a, int b)
{
    return (a > b) ? a : b;
}

int min(int a, int b)
{
    return (a < b) ? a : b;
}

int clamp(int val, int low, int high)
{
    int result;
    if (val < low)
    {
        result = low;
    }
    else if (val > high)
    {
        result = high;
    }
    else
    {
        result = val;
    }
    return result;
}

bool between(int val, int low, int high)
{
    return clamp(val, low, high) == val;
}

int abs(int x)
{
    return (x < 0) ? -x : x;
}