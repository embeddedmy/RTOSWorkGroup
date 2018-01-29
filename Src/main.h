#ifndef _MAIN_H
#define _MAIN_H

#include "cmsis_os.h"

extern osMailQId  dataWritequeue;
extern osMailQId  dataReadqueue;
extern osMessageQId dataEventqueue;
extern osMessageQId ReadEventCompleteQueue;

#endif
