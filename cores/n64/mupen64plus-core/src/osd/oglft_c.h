/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *   Mupen64plus - oglft_c.h                                               *
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

/* Minimal C-API interface to OGLFT
 * We only expose what we use.
 */

#ifndef M64P_OSD_OGLFT_C_H
#define M64P_OSD_OGLFT_C_H

#ifndef __cplusplus
#include <stdbool.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

bool OGLFT_Init_FT(void);
bool OGLFT_Uninit_FT(void);

struct OGLFT_Face;

enum OGLFT_Face_HorizontalJustification
{
    OGLFT_FACE_HORIZONTAL_JUSTIFICATION_LEFT,
    OGLFT_FACE_HORIZONTAL_JUSTIFICATION_ORIGIN,
    OGLFT_FACE_HORIZONTAL_JUSTIFICATION_CENTER,
    OGLFT_FACE_HORIZONTAL_JUSTIFICATION_RIGHT
};

enum OGLFT_Face_VerticalJustification
{
    OGLFT_FACE_VERTICAL_JUSTIFICATION_BOTTOM,
    OGLFT_FACE_VERTICAL_JUSTIFICATION_BASELINE,
    OGLFT_FACE_VERTICAL_JUSTIFICATION_MIDDLE,
    OGLFT_FACE_VERTICAL_JUSTIFICATION_TOP
};

struct OGLFT_Face* OGLFT_Monochrome_create(const char* filename, float point_size);
void OGLFT_Face_destroy(struct OGLFT_Face* face);

bool OGLFT_Face_isValid(const struct OGLFT_Face* face);
void OGLFT_Face_setForegroundColor(struct OGLFT_Face* face, float r, float g, float b, float a);
void OGLFT_Face_setBackgroundColor(struct OGLFT_Face* face, float r, float g, float b, float a);

void OGLFT_Face_setHorizontalJustification(struct OGLFT_Face* face, enum OGLFT_Face_HorizontalJustification justification);
void OGLFT_Face_setVerticalJustification(struct OGLFT_Face* face, enum OGLFT_Face_VerticalJustification justification);


double OGLFT_Face_height(const struct OGLFT_Face* face);

void OGLFT_Face_measure(struct OGLFT_Face* face, const char* s, float sizebox[4]);
void OGLFT_Face_measure_nominal(struct OGLFT_Face* face, const char* s, float sizebox[4]);

void OGLFT_Face_draw(struct OGLFT_Face* face, float x, float y, const char* s, float sizebox[4]);

#ifdef __cplusplus
}
#endif

#endif
