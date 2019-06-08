#include <sys/stat.h>
#include <dirent.h>

#include "utils.h"
#include "osd.h"

void create_default_directories(void) {
    const char *homedir;
    if ((homedir = getenv("HOME")) == NULL) {
        homedir = getpwuid(getuid())->pw_dir;
    }
   
    char pathname[MAXPATHLEN];
     
	/* base directory */
    sprintf (pathname, "%s%s", homedir, DEFAULT_PATH);
    DIR *dir = opendir(pathname);
    if (dir) closedir(dir);
    else mkdir(pathname,S_IRWXU);
    
    /* default SRAM & Savestate files directories */ 
    sprintf (pathname, "%s%s/saves", homedir, DEFAULT_PATH);
    dir = opendir(pathname);
    if (dir) closedir(dir);
    else mkdir(pathname,S_IRWXU);
    sprintf (pathname, "%s%s/saves/md", homedir, DEFAULT_PATH);
    dir = opendir(pathname);
    if (dir) closedir(dir);
    else mkdir(pathname,S_IRWXU);
    sprintf (pathname, "%s%s/saves/ms", homedir, DEFAULT_PATH);
    dir = opendir(pathname);
    if (dir) closedir(dir);
    else mkdir(pathname,S_IRWXU);
    sprintf (pathname, "%s%s/saves/gg", homedir, DEFAULT_PATH);
    dir = opendir(pathname);
    if (dir) closedir(dir);
    else mkdir(pathname,S_IRWXU);
    sprintf (pathname, "%s%s/saves/sg", homedir, DEFAULT_PATH);
    dir = opendir(pathname);
    if (dir) closedir(dir);
    else mkdir(pathname,S_IRWXU);
    sprintf (pathname, "%s%s/saves/cd", homedir, DEFAULT_PATH);
    dir = opendir(pathname);
    if (dir) closedir(dir);
    else mkdir(pathname,S_IRWXU);

    /* default Snapshot files directories */ 
    sprintf (pathname, "%s%s/snaps", homedir, DEFAULT_PATH);
    dir = opendir(pathname);
    if (dir) closedir(dir);
    else mkdir(pathname,S_IRWXU);
    sprintf (pathname, "%s%s/snaps/md", homedir, DEFAULT_PATH);
    dir = opendir(pathname);
    if (dir) closedir(dir);
    else mkdir(pathname,S_IRWXU);
    sprintf (pathname, "%s%s/snaps/ms", homedir, DEFAULT_PATH);
    dir = opendir(pathname);
    if (dir) closedir(dir);
    else mkdir(pathname,S_IRWXU);
    sprintf (pathname, "%s%s/snaps/gg", homedir, DEFAULT_PATH);
    dir = opendir(pathname);
    if (dir) closedir(dir);
    else mkdir(pathname,S_IRWXU);
    sprintf (pathname, "%s%s/snaps/sg", homedir, DEFAULT_PATH);
    dir = opendir(pathname);
    if (dir) closedir(dir);
    else mkdir(pathname,S_IRWXU);
    sprintf (pathname, "%s%s/snaps/cd", homedir, DEFAULT_PATH);
    dir = opendir(pathname);
    if (dir) closedir(dir);
    else mkdir(pathname,S_IRWXU);

    /* default Cheat files directories */ 
    sprintf (pathname, "%s%s/cheats", homedir, DEFAULT_PATH);
    dir = opendir(pathname);
    if (dir) closedir(dir);
    else mkdir(pathname,S_IRWXU);
    sprintf (pathname, "%s%s/cheats/md", homedir, DEFAULT_PATH);
    dir = opendir(pathname);
    if (dir) closedir(dir);
    else mkdir(pathname,S_IRWXU);
    sprintf (pathname, "%s%s/cheats/ms", homedir, DEFAULT_PATH);
    dir = opendir(pathname);
    if (dir) closedir(dir);
    else mkdir(pathname,S_IRWXU);
    sprintf (pathname, "%s%s/cheats/gg", homedir, DEFAULT_PATH);
    dir = opendir(pathname);
    if (dir) closedir(dir);
    else mkdir(pathname,S_IRWXU);
    sprintf (pathname, "%s%s/cheats/sg", homedir, DEFAULT_PATH);
    dir = opendir(pathname);
    if (dir) closedir(dir);
    else mkdir(pathname,S_IRWXU);
    sprintf (pathname, "%s%s/cheats/cd", homedir, DEFAULT_PATH);
    dir = opendir(pathname);
    if (dir) closedir(dir);
    else mkdir(pathname,S_IRWXU);

    /* default BIOS ROM files directories */ 
    sprintf (pathname, "%s%s/bios", homedir, DEFAULT_PATH);
    dir = opendir(pathname);
    if (dir) closedir(dir);
    else mkdir(pathname,S_IRWXU);

    /* default LOCK-ON ROM files directories */ 
    sprintf (pathname, "%s%s/lock-on", homedir, DEFAULT_PATH);
    dir = opendir(pathname);
    if (dir) closedir(dir);
    else mkdir(pathname,S_IRWXU);

}

char* get_save_directory(void) {
	const char *homedir;
	const char *system_dir;
    if ((homedir = getenv("HOME")) == NULL) {
        homedir = getpwuid(getuid())->pw_dir;
    }
   
    char pathname[MAXPATHLEN];
    
	if(system_hw <= SYSTEM_MARKIII){
		system_dir = "/saves/sg";
	} else if (system_hw > SYSTEM_MARKIII && system_hw <= SYSTEM_SMS2) {
		system_dir = "/saves/ms";
	} else if (system_hw > SYSTEM_SMS2 && system_hw <= SYSTEM_GGMS) {
		system_dir = "/saves/gg";
	} else if (system_hw == SYSTEM_MD) {
		system_dir = "/saves/md";
	} else if (system_hw == SYSTEM_MCD) {
		system_dir = "/saves/cd";
	} else {
		system_dir = "/saves/";
	}
	
    sprintf (pathname, "%s%s%s", homedir, DEFAULT_PATH, system_dir);
    
    return pathname;
	
}

char* gcw0_get_key_name(int keycode)
{
	     if (keycode == SDLK_UP)			return "Up";
	else if (keycode == SDLK_DOWN)  		return "Down";
	else if (keycode == SDLK_LEFT)  		return "Left";
	else if (keycode == SDLK_RIGHT) 		return "Right";
	else if (keycode == SDLK_LCTRL)     	return "A";
	else if (keycode == SDLK_LALT)     	return "B";
	else if (keycode == SDLK_LSHIFT)     	return "X";
	else if (keycode == SDLK_SPACE)     	return "Y";
	else if (keycode == SDLK_TAB)     		return "L";
	else if (keycode == SDLK_BACKSPACE)	return "R";
	else if (keycode == SDLK_RETURN)		return "Start";
	else if (keycode == SDLK_ESCAPE)		return "Select";
	else return "...";
}

char *get_file_name(char *full_path) {
	char file_name[256];
	sprintf(file_name, "%s", basename(full_path));
	
	/* remove file extension */
	int i = strlen(file_name) - 1;
	while ((i > 0) && (file_name[i] != '.')) i--;
	if (i > 0) file_name[i] = 0;
	
	return file_name;
}

