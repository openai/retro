/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *   Mupen64plus - video_capture_backend.c                                 *
 *   Mupen64Plus homepage: https://mupen64plus.org/                        *
 *   Copyright (C) 2018 Bobby Smiles                                       *
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

#include "video_capture_backend.h"

#include <string.h>

/* exported video capture backends */
extern const struct video_capture_backend_interface g_idummy_video_capture_backend;
#if defined(M64P_OPENCV)
extern const struct video_capture_backend_interface g_iopencv_video_capture_backend;
#endif


const struct video_capture_backend_interface* g_video_capture_backend_interfaces[] =
{
#if defined(M64P_OPENCV)
    &g_iopencv_video_capture_backend,
#endif
    &g_idummy_video_capture_backend,
    NULL /* sentinel - must be last element */
};


const struct video_capture_backend_interface* get_video_capture_backend(const char* name)
{
    const struct video_capture_backend_interface** i;

    /* passing NULL or empty string gives the dummy video input backend interface */
    if (!name || strlen(name) == 0) { return &g_idummy_video_capture_backend; }

    /* iterate through interfaces to find matching name */
    for (i = g_video_capture_backend_interfaces; (*i) != NULL; ++i) {
        if (strcmp((*i)->name, name) == 0) { return (*i); }
    }

    return NULL;
}
