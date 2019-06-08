#ifndef _STATE_H
#define _STATE_H

#include <retro_inline.h>

typedef struct
{
   uint8_t *data;
   uint32_t loc;
   uint32_t len;
   uint32_t malloced;
   uint32_t initial_malloc; // A setting!
} StateMem;

// Eh, we abuse the smem_* in-memory stream code
// in a few other places. :)
int32_t smem_read(StateMem *st, void *buffer, uint32_t len);
int32_t smem_write(StateMem *st, void *buffer, uint32_t len);
int32_t smem_putc(StateMem *st, int value);
int32_t smem_seek(StateMem *st, uint32_t offset, int whence);
int smem_write32le(StateMem *st, uint32_t b);
int smem_read32le(StateMem *st, uint32_t *b);

int MDFNSS_SaveSM(void *st, int, int, const void*, const void*, const void*);
int MDFNSS_LoadSM(void *st, int, int);

// Flag for a single, >= 1 byte native-endian variable
#define MDFNSTATE_RLSB            0x80000000

// 32-bit native-endian elements
#define MDFNSTATE_RLSB32          0x40000000

// 16-bit native-endian elements
#define MDFNSTATE_RLSB16          0x20000000

// 64-bit native-endian elements
#define MDFNSTATE_RLSB64          0x10000000

#define MDFNSTATE_BOOL		  0x08000000

typedef struct {
   void *v;		// Pointer to the variable/array
   uint32_t size;		// Length, in bytes, of the data to be saved EXCEPT:
   //  In the case of MDFNSTATE_BOOL, it is the number of bool elements to save(bool is not always 1-byte).
   // If 0, the subchunk isn't saved.
   uint32_t flags;	// Flags
   const char *name;	// Name
} SFORMAT;

INLINE bool SF_IS_BOOL(bool *) { return(1); }
INLINE bool SF_IS_BOOL(void *) { return(0); }

INLINE uint32_t SF_FORCE_AB(bool *) { return(0); }

INLINE uint32_t SF_FORCE_A8(int8_t *) { return(0); }
INLINE uint32_t SF_FORCE_A8(uint8_t *) { return(0); }

INLINE uint32_t SF_FORCE_A16(int16_t *) { return(0); }
INLINE uint32_t SF_FORCE_A16(uint16_t *) { return(0); }

INLINE uint32_t SF_FORCE_A32(int32_t *) { return(0); }
INLINE uint32_t SF_FORCE_A32(uint32_t *) { return(0); }

INLINE uint32_t SF_FORCE_A64(int64_t *) { return(0); }
INLINE uint32_t SF_FORCE_A64(uint64_t *) { return(0); }

INLINE uint32_t SF_FORCE_D(double *) { return(0); }

#define SFVARN(x, n) { &(x), SF_IS_BOOL(&(x)) ? 1 : (uint32_t)sizeof(x), MDFNSTATE_RLSB | (SF_IS_BOOL(&(x)) ? MDFNSTATE_BOOL : 0), n }
#define SFVAR(x) SFVARN((x), #x)

#define SFARRAYN(x, l, n) { (x), (uint32_t)(l), 0 | SF_FORCE_A8(x), n }
#define SFARRAY(x, l) SFARRAYN((x), (l), #x)

#define SFARRAYBN(x, l, n) { (x), (uint32_t)(l), MDFNSTATE_BOOL | SF_FORCE_AB(x), n }
#define SFARRAYB(x, l) SFARRAYBN((x), (l), #x)

#define SFARRAY16N(x, l, n) { (x), (uint32_t)((l) * sizeof(uint16_t)), MDFNSTATE_RLSB16 | SF_FORCE_A16(x), n }
#define SFARRAY16(x, l) SFARRAY16N((x), (l), #x)

#define SFARRAY32N(x, l, n) { (x), (uint32_t)((l) * sizeof(uint32_t)), MDFNSTATE_RLSB32 | SF_FORCE_A32(x), n }
#define SFARRAY32(x, l) SFARRAY32N((x), (l), #x)

#define SFARRAY64N(x, l, n) { (x), (uint32_t)((l) * sizeof(uint64_t)), MDFNSTATE_RLSB64 | SF_FORCE_A64(x), n }
#define SFARRAY64(x, l) SFARRAY64N((x), (l), #x)

#define SFARRAYDN(x, l, n) { (x), (uint32_t)((l) * 8), MDFNSTATE_RLSB64 | SF_FORCE_D(x), n }
#define SFARRAYD(x, l) SFARRAYDN((x), (l), #x)

#define SFEND { 0, 0, 0, 0 }

// State-Section Descriptor
class SSDescriptor
{
   public:
      SSDescriptor(SFORMAT *n_sf, const char *n_name, bool n_optional = 0)
      {
         sf = n_sf;
         name = n_name;
         optional = n_optional;
      }
      ~SSDescriptor(void)
      {

      }

      SFORMAT *sf;
      const char *name;
      bool optional;
};

int MDFNSS_StateAction(void *st, int load, int data_only, SFORMAT *sf, const char *name, bool optional = 0);

#endif
