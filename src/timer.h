// Interface for all PIT-related functions

#pragma once

#include "common.h"

//#define DEBUG_TIMER

// Registers timer handler and sets desired frequency
void init_timer(int frequency);

// Waits ticks (suspending the thread)
void timer_wait(int ticks);
