#ifndef MDFN_SETTINGS_H
#define MDFN_SETTINGS_H

#include <string>

extern int setting_initial_scanline;
extern int setting_last_scanline;
extern int setting_pce_fast_nospritelimit;
extern int setting_pce_overclocked;
extern int setting_pce_hoverscan;
extern int setting_pce_fast_cddavolume;
extern int setting_pce_fast_adpcmvolume;
extern int setting_pce_fast_cdpsgvolume;
extern uint32_t setting_pce_fast_cdspeed;
extern std::string setting_pce_fast_cdbios;

// This should assert() or something if the setting isn't found, since it would
// be a totally tubular error!
uint64 MDFN_GetSettingUI(const char *name);
int64 MDFN_GetSettingI(const char *name);
double MDFN_GetSettingF(const char *name);
bool MDFN_GetSettingB(const char *name);
std::string MDFN_GetSettingS(const char *name);
#endif
