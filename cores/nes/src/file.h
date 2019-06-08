#ifndef _FCEU_FILE_H
#define _FCEU_FILE_H

#ifdef __LIBRETRO__
typedef struct {
	uint8 *data;
	uint32 size;
	uint32 location;
} MEMWRAP;
#endif

typedef struct {
#ifdef __LIBRETRO__
   MEMWRAP *fp;
#else
	void *fp;		// FILE* or ptr to ZIPWRAP
#endif
	uint32 type;	// 0=normal file, 1=gzip, 2=zip
} FCEUFILE;

#ifdef __LIBRETRO__
FCEUFILE *FCEU_fopen(const char *path, const char *ipsfn, char *mode, char *ext, uint8 *buffer, size_t bufsize);
#else
FCEUFILE *FCEU_fopen(const char *path, const char *ipsfn, char *mode, char *ext);
#endif
int FCEU_fclose(FCEUFILE*);
uint64 FCEU_fread(void *ptr, size_t size, size_t nmemb, FCEUFILE*);
uint64 FCEU_fwrite(void *ptr, size_t size, size_t nmemb, FCEUFILE*);
int FCEU_fseek(FCEUFILE*, long offset, int whence);
uint64 FCEU_ftell(FCEUFILE*);
void FCEU_rewind(FCEUFILE*);
int FCEU_read32le(uint32 *Bufo, FCEUFILE*);
int FCEU_read16le(uint16 *Bufo, FCEUFILE*);
int FCEU_fgetc(FCEUFILE*);
uint64 FCEU_fgetsize(FCEUFILE*);
int FCEU_fisarchive(FCEUFILE*);

#endif
