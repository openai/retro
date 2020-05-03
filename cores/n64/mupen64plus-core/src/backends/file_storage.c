/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *   Mupen64plus - file_storage.c                                          *
 *   Mupen64Plus homepage: https://mupen64plus.org/                        *
 *   Copyright (C) 2016 Bobby Smiles                                       *
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

#include "file_storage.h"

#include <stdlib.h>

#include "api/callbacks.h"
#include "api/m64p_types.h"
#include "backends/api/storage_backend.h"
#include "device/dd/dd_controller.h"
#include "main/util.h"

int open_file_storage(struct file_storage* fstorage, size_t size, const char* filename)
{
    /* ! Take ownership of filename ! */
    fstorage->filename = filename;
    fstorage->size = size;

    /* allocate memory for holding data */
    fstorage->data = malloc(fstorage->size);
    if (fstorage->data == NULL) {
        return -1;
    }

    /* try to load storage file content */
    return read_from_file(fstorage->filename, fstorage->data, fstorage->size);
}

int open_rom_file_storage(struct file_storage* fstorage, const char* filename)
{
    fstorage->data = NULL;
    fstorage->size = 0;
    fstorage->filename = NULL;

    file_status_t err = load_file(filename, (void**)&fstorage->data, &fstorage->size);

    if (err == file_ok) {
        /* ! take ownsership of filename ! */
        fstorage->filename = filename;
    }

    return err;
}

void close_file_storage(struct file_storage* fstorage)
{
    free((void*)fstorage->data);
    free((void*)fstorage->filename);
}


static uint8_t* file_storage_data(const void* storage)
{
    struct file_storage* fstorage = (struct file_storage*)storage;
    return fstorage->data;
}

static size_t file_storage_size(const void* storage)
{
    struct file_storage* fstorage = (struct file_storage*)storage;
    return fstorage->size;
}

static void file_storage_save(void* storage)
{
    //TODO: Fix storage
    return;

    struct file_storage* fstorage = (struct file_storage*)storage;

    switch(write_to_file(fstorage->filename, fstorage->data, fstorage->size))
    {
    case file_open_error:
        DebugMessage(M64MSG_WARNING, "couldn't open storage file '%s' for writing", fstorage->filename);
        break;
    case file_write_error:
        DebugMessage(M64MSG_WARNING, "failed to write storage file '%s'", fstorage->filename);
        break;
    default:
        break;
    }
}

static void file_storage_parent_save(void* storage)
{
    struct file_storage* fstorage = (struct file_storage*)((struct file_storage*)storage)->filename;
    file_storage_save(fstorage);
}

static void file_storage_dd_sdk_dump_save(void* storage)
{
    //TODO: Fix storage
    return;
    
    static uint8_t sdk_buffer[SDK_FORMAT_DUMP_SIZE];
    struct file_storage* fstorage = (struct file_storage*)storage;

    /* XXX: for now, don't overwrite the original file, because we don't want to corrupt dumps... */
    char* filename = formatstr("%s.save", fstorage->filename);
    if (filename == NULL) {
        DebugMessage(M64MSG_ERROR, "Failed to allocate memory for sdk_dump filename");
        return;
    }

    dd_convert_to_sdk(fstorage->data, sdk_buffer);

    switch(write_to_file(filename, sdk_buffer, SDK_FORMAT_DUMP_SIZE))
    {
    case file_open_error:
        DebugMessage(M64MSG_WARNING, "couldn't open storage file '%s' for writing", fstorage->filename);
        break;
    case file_write_error:
        DebugMessage(M64MSG_WARNING, "failed to write storage file '%s'", fstorage->filename);
        break;
    default:
        break;
    }

    free(filename);
}



const struct storage_backend_interface g_ifile_storage =
{
    file_storage_data,
    file_storage_size,
    file_storage_save
};


const struct storage_backend_interface g_ifile_storage_ro =
{
    file_storage_data,
    file_storage_size,
    NULL
};

const struct storage_backend_interface g_isubfile_storage =
{
    file_storage_data,
    file_storage_size,
    file_storage_parent_save
};

const struct storage_backend_interface g_ifile_storage_dd_sdk_dump =
{
    file_storage_data,
    file_storage_size,
    file_storage_dd_sdk_dump_save
};
