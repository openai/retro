/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *   Mupen64plus - dummy_video_capture.c                                   *
 *   Mupen64Plus homepage: https://mupen64plus.org/                        *
 *   Copyright (C) 2017 Bobby Smiles                                       *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.          *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "backends/api/video_capture_backend.h"

#include <stdlib.h>
#include <string.h>

/* Dummy video capture backend
 *
 * Returns a black frame
 */
struct dummy_video_capture
{
    /* BGR frame size (no stride) */
    size_t size;
};

static m64p_error dummy_init(void** vcap, const char* config)
{
    struct dummy_video_capture* dummy = malloc(sizeof(*dummy));
    if (dummy == NULL) {
        *vcap = NULL;
        return M64ERR_NO_MEMORY;
    }

    memset(dummy, 0, sizeof(*dummy));

    *vcap = dummy;
    return M64ERR_SUCCESS;
}

static void dummy_release(void* vcap)
{
    free(vcap);
}

static m64p_error dummy_open(void* vcap, unsigned int width, unsigned int height)
{
    struct dummy_video_capture* dummy = (struct dummy_video_capture*)vcap;
    dummy->size = 3 * width * height;
    return M64ERR_SUCCESS;
}

static void dummy_close(void* vcap)
{
    struct dummy_video_capture* dummy = (struct dummy_video_capture*)vcap;
    dummy->size = 0;
}

static m64p_error dummy_grab_image(void* vcap, void* data)
{
    struct dummy_video_capture* dummy = (struct dummy_video_capture*)vcap;
    memset(data, 0, dummy->size);
    return M64ERR_SUCCESS;
}

const struct video_capture_backend_interface g_idummy_video_capture_backend =
{
    "dummy",
    dummy_init,
    dummy_release,
    dummy_open,
    dummy_close,
    dummy_grab_image
};
