#include "rsp-mips.h"

u8 data[4] = { 0x10, 0x20, 0x30, 0x40 };

__attribute__((noinline))
int load_byte(int i)
{
	return data[i];
}

int count = 3;

int main(void)
{
	int res = 0;
	for (int i = 0; i < 3; i++)
	{
		res += load_byte(i);
	}
	rsp_debug_break(res, res, res, res);
}
