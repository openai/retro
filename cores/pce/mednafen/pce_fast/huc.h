int HuCLoad(const uint8 *data, uint32 len) MDFN_COLD;
int HuCLoadCD(const char *bios_path) MDFN_COLD;
void HuC_Close(void) MDFN_COLD;
int HuC_StateAction(StateMem *sm, int load, int data_only);

void HuC_Power(void) MDFN_COLD;

DECLFR(PCE_ACRead);
DECLFW(PCE_ACWrite);

extern bool PCE_IsCD;
