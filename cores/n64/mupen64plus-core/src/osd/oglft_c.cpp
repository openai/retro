/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *   Mupen64plus - oglft_c.cpp                                             *
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

#include "oglft_c.h"

#include <OGLFT.h>

extern "C" bool OGLFT_Init_FT(void)
{
    try {
        return OGLFT::Init_FT();
    } catch(...) { /* swallow error */ }

    return false;
}

extern "C" bool OGLFT_Uninit_FT(void)
{
    try {
        return OGLFT::Uninit_FT();
    } catch(...) { /* swallow error */ }

    return false;
}

extern "C" bool OGLFT_Face_isValid(const struct OGLFT_Face* face)
{
    try {
        return reinterpret_cast<const OGLFT::Face*>(face)->isValid();
    } catch(...) { /* swallow error */ }

    return false;
}


extern "C" void OGLFT_Face_setForegroundColor(struct OGLFT_Face* face, float r, float g, float b, float a)
{
    try {
        reinterpret_cast<OGLFT::Face*>(face)->setForegroundColor(r, g, b, a);
    } catch(...) { /* swallow error */ }
}

extern "C" void OGLFT_Face_setBackgroundColor(struct OGLFT_Face* face, float r, float g, float b, float a)
{
    try {
        reinterpret_cast<OGLFT::Face*>(face)->setBackgroundColor(r, g, b, a);
    } catch(...) { /* swallow error */ }
}


extern "C" void OGLFT_Face_setHorizontalJustification(struct OGLFT_Face* face, enum OGLFT_Face_HorizontalJustification justification)
{
    try {
        reinterpret_cast<OGLFT::Face*>(face)->setHorizontalJustification(static_cast<OGLFT::Face::HorizontalJustification>(justification));
    } catch(...) { /* swallow error */ }
}

extern "C" void OGLFT_Face_setVerticalJustification(struct OGLFT_Face* face, enum OGLFT_Face_VerticalJustification justification)
{
    try {
        reinterpret_cast<OGLFT::Face*>(face)->setVerticalJustification(static_cast<OGLFT::Face::VerticalJustification>(justification));
    } catch(...) { /* swallow error */ }
}


extern "C" double OGLFT_Face_height(const struct OGLFT_Face* face)
{
    try {
        return reinterpret_cast<const OGLFT::Face*>(face)->height();
    } catch(...) { /* swallow error */ }

    return 0.0;
}


extern "C" void OGLFT_Face_measure(struct OGLFT_Face* face, const char* s, float sizebox[4])
{
    try {
        OGLFT::BBox bbox = reinterpret_cast<OGLFT::Face*>(face)->measure(s);
        sizebox[0] = bbox.x_min_;
        sizebox[1] = bbox.y_min_;
        sizebox[2] = bbox.x_max_;
        sizebox[3] = bbox.y_max_;
    } catch(...) { memset(sizebox, 0, 4*sizeof sizebox[0]); }
}

extern "C" void OGLFT_Face_measure_nominal(struct OGLFT_Face* face, const char* s, float sizebox[4])
{
    try {
        OGLFT::BBox bbox = reinterpret_cast<OGLFT::Face*>(face)->measure_nominal(s);
        sizebox[0] = bbox.x_min_;
        sizebox[1] = bbox.y_min_;
        sizebox[2] = bbox.x_max_;
        sizebox[3] = bbox.y_max_;
    } catch(...) { memset(sizebox, 0, 4*sizeof sizebox[0]); }
}

extern "C" void OGLFT_Face_draw(struct OGLFT_Face* face, float x, float y, const char* s, float sizebox[4])
{
    try {
        reinterpret_cast<OGLFT::Face*>(face)->draw(x, y, s, sizebox);
    } catch(...) { /* swallow error */ }
}


extern "C" struct OGLFT_Face* OGLFT_Monochrome_create(const char* filename, float point_size)
{
    try {
        return reinterpret_cast<struct OGLFT_Face*>(new OGLFT::Monochrome(filename, point_size));
    } catch(...) { /* swallow error */ }

    return NULL;
}

extern "C" void OGLFT_Face_destroy(struct OGLFT_Face* face)
{
    try {
        delete reinterpret_cast<OGLFT::Face*>(face);
    } catch(...) { /* swallow error */ }
}
