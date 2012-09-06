/*
 * This file is part of MPlayer.
 *
 * MPlayer is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * MPlayer is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with MPlayer; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "config.h"
#include "img_format.h"
#include "stdio.h"
#include "mpbswap.h"

// Used by option parsing and codecs.conf parsing
const struct imgfmt_name mp_imgfmt_list[] = {
    {"444p16le", IMGFMT_444P16_LE},
    {"444p16be", IMGFMT_444P16_BE},
    {"444p10le", IMGFMT_444P10_LE},
    {"444p10be", IMGFMT_444P10_BE},
    {"444p9le", IMGFMT_444P9_LE},
    {"444p9be", IMGFMT_444P9_BE},
    {"422p16le", IMGFMT_422P16_LE},
    {"422p16be", IMGFMT_422P16_BE},
    {"422p10le", IMGFMT_422P10_LE},
    {"422p10be", IMGFMT_422P10_BE},
    {"422p9le", IMGFMT_422P9_LE},
    {"422p9be", IMGFMT_422P9_BE},
    {"420p16le", IMGFMT_420P16_LE},
    {"420p16be", IMGFMT_420P16_BE},
    {"420p10le", IMGFMT_420P10_LE},
    {"420p10be", IMGFMT_420P10_BE},
    {"420p9le", IMGFMT_420P9_LE},
    {"420p9be", IMGFMT_420P9_BE},
    {"444p16", IMGFMT_444P16},
    {"444p10", IMGFMT_444P10},
    {"444p9", IMGFMT_444P9},
    {"422p16", IMGFMT_422P16},
    {"422p10", IMGFMT_422P10},
    {"422p9", IMGFMT_422P9},
    {"420p10", IMGFMT_420P10},
    {"420p9", IMGFMT_420P9},
    {"420p16", IMGFMT_420P16},
    {"420a", IMGFMT_420A},
    {"444p", IMGFMT_444P},
    {"422p", IMGFMT_422P},
    {"411p", IMGFMT_411P},
    {"440p", IMGFMT_440P},
    {"yuy2", IMGFMT_YUY2},
    {"yvyu", IMGFMT_YVYU},
    {"uyvy", IMGFMT_UYVY},
    {"yvu9", IMGFMT_YVU9},
    {"if09", IMGFMT_IF09},
    {"yv12", IMGFMT_YV12},
    {"i420", IMGFMT_I420},
    {"iyuv", IMGFMT_IYUV},
    {"clpl", IMGFMT_CLPL},
    {"hm12", IMGFMT_HM12},
    {"y800", IMGFMT_Y800},
    {"y8", IMGFMT_Y8},
    {"nv12", IMGFMT_NV12},
    {"nv21", IMGFMT_NV21},
    {"bgr24", IMGFMT_BGR24},
    {"bgr32", IMGFMT_BGR32},
    {"bgr16", IMGFMT_BGR16},
    {"Bgr15", IMGFMT_BGR15},
    {"bgr12", IMGFMT_BGR12},
    {"bgr8", IMGFMT_BGR8},
    {"bgr4", IMGFMT_BGR4},
    {"bg4b", IMGFMT_BG4B},
    {"bgr1", IMGFMT_BGR1},
    {"rgb48be", IMGFMT_RGB48BE},
    {"rgb48le", IMGFMT_RGB48LE},
    {"rgb48ne", IMGFMT_RGB48NE},
    {"rgb24", IMGFMT_RGB24},
    {"rgb32", IMGFMT_RGB32},
    {"rgb16", IMGFMT_RGB16},
    {"rgb15", IMGFMT_RGB15},
    {"rgb12", IMGFMT_RGB12},
    {"rgb8", IMGFMT_RGB8},
    {"rgb4", IMGFMT_RGB4},
    {"rg4b", IMGFMT_RG4B},
    {"rgb1", IMGFMT_RGB1},
    {"rgba", IMGFMT_RGBA},
    {"argb", IMGFMT_ARGB},
    {"bgra", IMGFMT_BGRA},
    {"abgr", IMGFMT_ABGR},
    {"gbrp", IMGFMT_GBRP},
    {"gbrp9", IMGFMT_GBRP9},
    {"gbrp9le", IMGFMT_GBRP9LE},
    {"gbrp9be", IMGFMT_GBRP9BE},
    {"gbrp10", IMGFMT_GBRP10},
    {"gbrp10le", IMGFMT_GBRP10LE},
    {"gbrp10be", IMGFMT_GBRP10BE},
    {"mjpeg", IMGFMT_MJPEG},
    {"mjpg", IMGFMT_MJPEG},
    { NULL, 0 }
};

// used in codecs.conf only
static const struct imgfmt_name special_imgfmts[] = {
    {"mpes", IMGFMT_MPEGPES},
    {"vdpau_h264", IMGFMT_VDPAU_H264},
    {"vdpau_mpeg1", IMGFMT_VDPAU_MPEG1},
    {"vdpau_mpeg2", IMGFMT_VDPAU_MPEG2},
    {"vdpau_mpeg4", IMGFMT_VDPAU_MPEG4},
    {"vdpau_wmv3", IMGFMT_VDPAU_WMV3},
    {"vdpau_vc1", IMGFMT_VDPAU_VC1},
    { NULL, 0 }
};

unsigned int imgfmt_parse(struct bstr name, bool special_fmts)
{
    for (int i = 0; mp_imgfmt_list[i].name; i++)
        if (!bstrcasecmp0(name, mp_imgfmt_list[i].name))
            return mp_imgfmt_list[i].fmt;
    if (!special_fmts)
        return 0;
    for (int i = 0; special_imgfmts[i].name; i++)
        if (!bstrcasecmp0(name, special_imgfmts[i].name))
            return special_imgfmts[i].fmt;
    return 0;
}

const char *vo_format_name(int format)
{
    static char unknown_format[20];
    switch (format) {
    case IMGFMT_RGB1: return "RGB 1-bit";
    case IMGFMT_RGB4: return "RGB 4-bit";
    case IMGFMT_RG4B: return "RGB 4-bit per byte";
    case IMGFMT_RGB8: return "RGB 8-bit";
    case IMGFMT_RGB12: return "RGB 12-bit";
    case IMGFMT_RGB15: return "RGB 15-bit";
    case IMGFMT_RGB16: return "RGB 16-bit";
    case IMGFMT_RGB24: return "RGB 24-bit";
//  case IMGFMT_RGB32: return "RGB 32-bit";      // equals either RGBA or ABGR
    case IMGFMT_RGB48LE: return "RGB 48-bit LE";
    case IMGFMT_RGB48BE: return "RGB 48-bit BE";
    case IMGFMT_BGR1: return "BGR 1-bit";
    case IMGFMT_BGR4: return "BGR 4-bit";
    case IMGFMT_BG4B: return "BGR 4-bit per byte";
    case IMGFMT_BGR8: return "BGR 8-bit";
    case IMGFMT_BGR12: return "BGR 12-bit";
    case IMGFMT_BGR15: return "BGR 15-bit";
    case IMGFMT_BGR16: return "BGR 16-bit";
    case IMGFMT_BGR24: return "BGR 24-bit";
//  case IMGFMT_BGR32: return "BGR 32-bit";      // equals either BGRA or ARGB
    case IMGFMT_ABGR: return "ABGR";
    case IMGFMT_BGRA: return "BGRA";
    case IMGFMT_ARGB: return "ARGB";
    case IMGFMT_RGBA: return "RGBA";
    case IMGFMT_GBRP: return "Planar GBR 24-bit";
    case IMGFMT_GBRP9LE: return "Planar GBR 27-bit little-endian";
    case IMGFMT_GBRP9BE: return "Planar GBR 27-bit big-endian";
    case IMGFMT_GBRP10LE: return "Planar GBR 30-bit little-endian";
    case IMGFMT_GBRP10BE: return "Planar GBR 30-bit big-endian";
    case IMGFMT_YVU9: return "Planar YVU9";
    case IMGFMT_IF09: return "Planar IF09";
    case IMGFMT_YV12: return "Planar YV12";
    case IMGFMT_I420: return "Planar I420";
    case IMGFMT_IYUV: return "Planar IYUV";
    case IMGFMT_CLPL: return "Planar CLPL";
    case IMGFMT_Y800: return "Planar Y800";
    case IMGFMT_Y8: return "Planar Y8";
    case IMGFMT_420P16_LE: return "Planar 420P 16-bit little-endian";
    case IMGFMT_420P16_BE: return "Planar 420P 16-bit big-endian";
    case IMGFMT_420P10_LE: return "Planar 420P 10-bit little-endian";
    case IMGFMT_420P10_BE: return "Planar 420P 10-bit big-endian";
    case IMGFMT_420P9_LE: return "Planar 420P 9-bit little-endian";
    case IMGFMT_420P9_BE: return "Planar 420P 9-bit big-endian";
    case IMGFMT_422P16_LE: return "Planar 422P 16-bit little-endian";
    case IMGFMT_422P16_BE: return "Planar 422P 16-bit big-endian";
    case IMGFMT_422P10_LE: return "Planar 422P 10-bit little-endian";
    case IMGFMT_422P10_BE: return "Planar 422P 10-bit big-endian";
    case IMGFMT_422P9_LE:  return "Planar 422P 9-bit little-endian";
    case IMGFMT_422P9_BE:  return "Planar 422P 9-bit big-endian";
    case IMGFMT_444P16_LE: return "Planar 444P 16-bit little-endian";
    case IMGFMT_444P16_BE: return "Planar 444P 16-bit big-endian";
    case IMGFMT_444P10_LE: return "Planar 444P 10-bit little-endian";
    case IMGFMT_444P10_BE: return "Planar 444P 10-bit big-endian";
    case IMGFMT_444P9_LE: return "Planar 444P 9-bit little-endian";
    case IMGFMT_444P9_BE: return "Planar 444P 9-bit big-endian";
    case IMGFMT_420A: return "Planar 420P with alpha";
    case IMGFMT_444P: return "Planar 444P";
    case IMGFMT_422P: return "Planar 422P";
    case IMGFMT_411P: return "Planar 411P";
    case IMGFMT_NV12: return "Planar NV12";
    case IMGFMT_NV21: return "Planar NV21";
    case IMGFMT_HM12: return "Planar NV12 Macroblock";
    case IMGFMT_IUYV: return "Packed IUYV";
    case IMGFMT_IY41: return "Packed IY41";
    case IMGFMT_IYU1: return "Packed IYU1";
    case IMGFMT_IYU2: return "Packed IYU2";
    case IMGFMT_UYVY: return "Packed UYVY";
    case IMGFMT_UYNV: return "Packed UYNV";
    case IMGFMT_cyuv: return "Packed CYUV";
    case IMGFMT_Y422: return "Packed Y422";
    case IMGFMT_YUY2: return "Packed YUY2";
    case IMGFMT_YUNV: return "Packed YUNV";
    case IMGFMT_YVYU: return "Packed YVYU";
    case IMGFMT_Y41P: return "Packed Y41P";
    case IMGFMT_Y211: return "Packed Y211";
    case IMGFMT_Y41T: return "Packed Y41T";
    case IMGFMT_Y42T: return "Packed Y42T";
    case IMGFMT_V422: return "Packed V422";
    case IMGFMT_V655: return "Packed V655";
    case IMGFMT_CLJR: return "Packed CLJR";
    case IMGFMT_YUVP: return "Packed YUVP";
    case IMGFMT_UYVP: return "Packed UYVP";
    case IMGFMT_MPEGPES: return "Mpeg PES";
    case IMGFMT_VDPAU_MPEG1: return "MPEG1 VDPAU acceleration";
    case IMGFMT_VDPAU_MPEG2: return "MPEG2 VDPAU acceleration";
    case IMGFMT_VDPAU_H264: return "H.264 VDPAU acceleration";
    case IMGFMT_VDPAU_MPEG4: return "MPEG-4 Part 2 VDPAU acceleration";
    case IMGFMT_VDPAU_WMV3: return "WMV3 VDPAU acceleration";
    case IMGFMT_VDPAU_VC1: return "VC1 VDPAU acceleration";
    }
    snprintf(unknown_format, 20, "Unknown 0x%04x", format);
    return unknown_format;
}

int mp_get_chroma_shift(int format, int *x_shift, int *y_shift,
                        int *component_bits)
{
    int xs = 0, ys = 0;
    int bpp;
    int err = 0;
    int bits = 8;
    if ((format & 0xff0000f0) == 0x34000050)
        format = bswap_32(format);
    if ((format & 0xf00000ff) == 0x50000034) {
        switch (format >> 24) {
        case 0x50:
            break;
        case 0x51:
            bits = 16;
            break;
        case 0x52:
            bits = 10;
            break;
        case 0x53:
            bits = 9;
            break;
        default:
            err = 1;
            break;
        }
        switch (format & 0x00ffffff) {
        case 0x00343434: // 444
            xs = 0;
            ys = 0;
            break;
        case 0x00323234: // 422
            xs = 1;
            ys = 0;
            break;
        case 0x00303234: // 420
            xs = 1;
            ys = 1;
            break;
        case 0x00313134: // 411
            xs = 2;
            ys = 0;
            break;
        case 0x00303434: // 440
            xs = 0;
            ys = 1;
            break;
        default:
            err = 1;
            break;
        }
    } else
        switch (format) {
        case IMGFMT_420A:
        case IMGFMT_I420:
        case IMGFMT_IYUV:
        case IMGFMT_YV12:
            xs = 1;
            ys = 1;
            break;
        case IMGFMT_IF09:
        case IMGFMT_YVU9:
            xs = 2;
            ys = 2;
            break;
        case IMGFMT_Y8:
        case IMGFMT_Y800:
            xs = 31;
            ys = 31;
            break;
        default:
            err = 1;
            break;
        }
    if (x_shift)
        *x_shift = xs;
    if (y_shift)
        *y_shift = ys;
    if (component_bits)
        *component_bits = bits;
    bpp = 8 + ((16 >> xs) >> ys);
    if (format == IMGFMT_420A)
        bpp += 8;
    bpp *= (bits + 7) >> 3;
    return err ? 0 : bpp;
}
