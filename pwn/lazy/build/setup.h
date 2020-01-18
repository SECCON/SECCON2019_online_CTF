#ifndef _SETUP_H_
#define _SETUP_H_

#include <signal.h>

void setup(void);

void signal_handler(int sig, siginfo_t *siginfo, void *context);

#endif
