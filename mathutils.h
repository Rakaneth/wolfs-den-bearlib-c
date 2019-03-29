#pragma once
#undef max
#undef min

int max(int a, int b);
int min(int a, int b);
int clamp(int val, int low, int high);
int between(int val, int low, int high);
int abs(int x);