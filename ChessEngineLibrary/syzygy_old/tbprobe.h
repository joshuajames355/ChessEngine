#ifndef TBPROBE_H
#define TBPROBE_H

#pragma once
#include "../board.h"

extern int TBlargest; // 5 if 5-piece tables, 6 if 6-piece tables were found.

void init_tablebases(char *path);
int probe_wdl(Board& pos, int *success);
int probe_dtz(Board& pos, int *success);
int root_probe(Board& pos, Value& TBScore);
int root_probe_wdl(Board& pos, Value& TBScore);

#endif

