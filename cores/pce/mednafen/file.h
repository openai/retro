#ifndef MDFN_FILE_H
#define MDFN_FILE_H

#include <stdint.h>

#define MDFNFILE_EC_NOTFOUND	1
#define MDFNFILE_EC_OTHER	2

#ifdef __cplusplus
extern "C" {
#endif

struct MDFNFILE
{
   uint8_t *data;
   int64_t size;
   char *ext;
   int64_t location;
};

struct MDFNFILE *file_open(const char *path);

int file_close(struct MDFNFILE *file);

uint64_t file_read(struct MDFNFILE *file, void *ptr,
      size_t element_size, size_t nmemb);

int file_seek(struct MDFNFILE *file, int64_t offset, int whence);

char *file_fgets(struct MDFNFILE *file, char *s, int buffer_size);

#ifdef __cplusplus
}
#endif

#endif
