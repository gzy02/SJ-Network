#pragma once

#define PATTERN_SIZE 5
typedef struct Myoptarg
{
    int fnum;
    int size;
    int pnum;
    int ptime;
    int init; // 1 or 0
} Myoptarg;
extern const char *pattern[PATTERN_SIZE];
extern void Set_Config(int argc, char **argv, struct Myoptarg *arg);
extern void Print_Config(struct Myoptarg *arg);
