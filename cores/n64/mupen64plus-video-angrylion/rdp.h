#pragma once

#include <stdint.h>

// maximum data size of a single command in bytes
#define CMD_MAX_SIZE 176

// maximum data size of a single command in 32 bit integers
#define CMD_MAX_INTS (CMD_MAX_SIZE / sizeof(int32_t))

// extracts the command ID from a command buffer
#define CMD_ID(cmd) ((*(cmd) >> 24) & 0x3f)

// list of command IDs
#define CMD_ID_NO_OP                           0x00
#define CMD_ID_FILL_TRIANGLE                   0x08
#define CMD_ID_FILL_ZBUFFER_TRIANGLE           0x09
#define CMD_ID_TEXTURE_TRIANGLE                0x0a
#define CMD_ID_TEXTURE_ZBUFFER_TRIANGLE        0x0b
#define CMD_ID_SHADE_TRIANGLE                  0x0c
#define CMD_ID_SHADE_ZBUFFER_TRIANGLE          0x0d
#define CMD_ID_SHADE_TEXTURE_TRIANGLE          0x0e
#define CMD_ID_SHADE_TEXTURE_Z_BUFFER_TRIANGLE 0x0f
#define CMD_ID_TEXTURE_RECTANGLE               0x24
#define CMD_ID_TEXTURE_RECTANGLE_FLIP          0x25
#define CMD_ID_SYNC_LOAD                       0x26
#define CMD_ID_SYNC_PIPE                       0x27
#define CMD_ID_SYNC_TILE                       0x28
#define CMD_ID_SYNC_FULL                       0x29
#define CMD_ID_SET_KEY_GB                      0x2a
#define CMD_ID_SET_KEY_R                       0x2b
#define CMD_ID_SET_CONVERT                     0x2c
#define CMD_ID_SET_SCISSOR                     0x2d
#define CMD_ID_SET_PRIM_DEPTH                  0x2e
#define CMD_ID_SET_OTHER_MODES                 0x2f
#define CMD_ID_LOAD_TLUT                       0x30
#define CMD_ID_SET_TILE_SIZE                   0x32
#define CMD_ID_LOAD_BLOCK                      0x33
#define CMD_ID_LOAD_TILE                       0x34
#define CMD_ID_SET_TILE                        0x35
#define CMD_ID_FILL_RECTANGLE                  0x36
#define CMD_ID_SET_FILL_COLOR                  0x37
#define CMD_ID_SET_FOG_COLOR                   0x38
#define CMD_ID_SET_BLEND_COLOR                 0x39
#define CMD_ID_SET_PRIM_COLOR                  0x3a
#define CMD_ID_SET_ENV_COLOR                   0x3b
#define CMD_ID_SET_COMBINE                     0x3c
#define CMD_ID_SET_TEXTURE_IMAGE               0x3d
#define CMD_ID_SET_MASK_IMAGE                  0x3e
#define CMD_ID_SET_COLOR_IMAGE                 0x3f

struct rdp_state;

void rdp_create(struct rdp_state** rdp, uint32_t stride, uint32_t offset);
void rdp_destroy(struct rdp_state* rdp);

void rdp_invalid(struct rdp_state* rdp, const uint32_t* args);
void rdp_noop(struct rdp_state* rdp, const uint32_t* args);
void rdp_tri_noshade(struct rdp_state* rdp, const uint32_t* args);
void rdp_tri_noshade_z(struct rdp_state* rdp, const uint32_t* args);
void rdp_tri_tex(struct rdp_state* rdp, const uint32_t* args);
void rdp_tri_tex_z(struct rdp_state* rdp, const uint32_t* args);
void rdp_tri_shade(struct rdp_state* rdp, const uint32_t* args);
void rdp_tri_shade_z(struct rdp_state* rdp, const uint32_t* args);
void rdp_tri_texshade(struct rdp_state* rdp, const uint32_t* args);
void rdp_tri_texshade_z(struct rdp_state* rdp, const uint32_t* args);
void rdp_tex_rect(struct rdp_state* rdp, const uint32_t* args);
void rdp_tex_rect_flip(struct rdp_state* rdp, const uint32_t* args);
void rdp_sync_load(struct rdp_state* rdp, const uint32_t* args);
void rdp_sync_pipe(struct rdp_state* rdp, const uint32_t* args);
void rdp_sync_tile(struct rdp_state* rdp, const uint32_t* args);
void rdp_sync_full(struct rdp_state* rdp, const uint32_t* args);
void rdp_set_key_gb(struct rdp_state* rdp, const uint32_t* args);
void rdp_set_key_r(struct rdp_state* rdp, const uint32_t* args);
void rdp_set_convert(struct rdp_state* rdp, const uint32_t* args);
void rdp_set_scissor(struct rdp_state* rdp, const uint32_t* args);
void rdp_set_prim_depth(struct rdp_state* rdp, const uint32_t* args);
void rdp_set_other_modes(struct rdp_state* rdp, const uint32_t* args);
void rdp_set_tile_size(struct rdp_state* rdp, const uint32_t* args);
void rdp_load_block(struct rdp_state* rdp, const uint32_t* args);
void rdp_load_tlut(struct rdp_state* rdp, const uint32_t* args);
void rdp_load_tile(struct rdp_state* rdp, const uint32_t* args);
void rdp_set_tile(struct rdp_state* rdp, const uint32_t* args);
void rdp_fill_rect(struct rdp_state* rdp, const uint32_t* args);
void rdp_set_fill_color(struct rdp_state* rdp, const uint32_t* args);
void rdp_set_fog_color(struct rdp_state* rdp, const uint32_t* args);
void rdp_set_blend_color(struct rdp_state* rdp, const uint32_t* args);
void rdp_set_prim_color(struct rdp_state* rdp, const uint32_t* args);
void rdp_set_env_color(struct rdp_state* rdp, const uint32_t* args);
void rdp_set_combine(struct rdp_state* rdp, const uint32_t* args);
void rdp_set_texture_image(struct rdp_state* rdp, const uint32_t* args);
void rdp_set_mask_image(struct rdp_state* rdp, const uint32_t* args);
void rdp_set_color_image(struct rdp_state* rdp, const uint32_t* args);
void rdp_cmd(struct rdp_state* rdp, const uint32_t* args);
