/*
 can_clone_file.c -- does the current filesystem support cloning
 Copyright (C) 1999-2017 Dieter Baron and Thomas Klausner

 This file is part of libzip, a library to manipulate ZIP archives.
 The authors can be contacted at <libzip@nih.at>

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions
 are met:
 1. Redistributions of source code must retain the above copyright
 notice, this list of conditions and the following disclaimer.
 2. Redistributions in binary form must reproduce the above copyright
 notice, this list of conditions and the following disclaimer in
 the documentation and/or other materials provided with the
 distribution.
 3. The names of the authors may not be used to endorse or promote
 products derived from this software without specific prior
 written permission.

 THIS SOFTWARE IS PROVIDED BY THE AUTHORS ``AS IS'' AND ANY EXPRESS
 OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY
 DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
 IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"

#ifdef HAVE_CLONEFILE
#include <sys/attr.h>
#include <sys/param.h>
#include <sys/mount.h>
#include <unistd.h>
#endif

int
main(int argc, char *argv[])
{
#ifdef HAVE_CLONEFILE
    struct statfs fs;
    struct attrlist attribute_list;
    struct {
        uint32_t size;
        vol_capabilities_attr_t capabilities;
    } volume_attributes;

    if (statfs(".", &fs) < 0) {
        fprintf(stderr, "%s: can't get mount point of current directory: %s\n", argv[0], strerror(errno));
        exit(1);
    }

    /* Not all volumes support clonefile().  A volume can be tested for
       clonefile() support by using getattrlist(2) to get the volume
       capabilities attribute ATTR_VOL_CAPABILITIES, and then testing the
       VOL_CAP_INT_CLONE flag. */

    memset(&attribute_list, 0, sizeof(attribute_list));
    attribute_list.bitmapcount = ATTR_BIT_MAP_COUNT;
    attribute_list.volattr = ATTR_VOL_INFO|ATTR_VOL_CAPABILITIES;
    memset(&volume_attributes, 0, sizeof(volume_attributes));

    if (getattrlist(fs.f_mntonname, &attribute_list, &volume_attributes, sizeof(volume_attributes), 0) < 0) {
        fprintf(stderr, "%s: can't get volume capabilities of '%s': %s\n", argv[0], fs.f_mntonname, strerror(errno));
        exit(1);
    }

    if (volume_attributes.capabilities.capabilities[VOL_CAPABILITIES_INTERFACES] & VOL_CAP_INT_CLONE) {
        exit(0);
    }
#endif

    exit(1);
}
