#ifndef __SERIAL_H
#define __SERIAL_H

#include <stdio.h>

typedef int BOOL;

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#include "../fceu-types.h"

void SendCmd(uint8 *cmd, int size);
int ReadResp(uint8 *resp, int size);

#define SEND(cmd) SendCmd((uint8*)&cmd[0], sizeof(cmd))
#define GET(buf, size) ReadResp((uint8*)&buf, size)
#define SENDGET(cmd, buf, size) SEND(cmd); GET(buf, size)

BOOL SerialOpen(int port, int baud);
void SerialClose(void);
BOOL SerialSendChar(int c);
int  SerialIsOpen(void);
int  SerialGetChar(void);

#endif
