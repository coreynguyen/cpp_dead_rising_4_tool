#ifndef IMG_DDS_H
#define IMG_DDS_H

//#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
//#include <cstring>
//#include <windows.h>
//#include <stdio.h>
#include <cstdint>		// needed for types such as uint8_t, uint16_t, uint32_t
//#include <tchar.h>
//#include <texconv.h>
//#include <ddsview.h>
//#include "resource.h"
//#include "manifest.h"


using namespace std;


// Map Direct3D 9 Formats to Direct3D 10
// https://docs.microsoft.com/en-us/windows/win32/direct3d10/d3d10-graphics-programming-guide-resources-legacy-formats
/*
D3DFMT_UNKNOWN                          DXGI_FORMAT_UNKNOWN
D3DFMT_R8G8B8                           Not_available
D3DFMT_A8R8G8B8                         DXGI_FORMAT_B8G8R8A8_UNORM
D3DFMT_X8R8G8B8                         DXGI_FORMAT_B8G8R8X8_UNORM
D3DFMT_R5G6B5                           DXGI_FORMAT_B5G6R5_UNORM
D3DFMT_X1R5G5B5                         Not_available
D3DFMT_A1R5G5B5                         DXGI_FORMAT_B5G5R5A1_UNORM
D3DFMT_A4R4G4B4                         DXGI_FORMAT_B4G4R4A4_UNORM
D3DFMT_R3G3B2                           Not_available
D3DFMT_A8                               DXGI_FORMAT_A8_UNORM
D3DFMT_A8R3G3B2                         Not_available
D3DFMT_X4R4G4B4                         Not_available
D3DFMT_A2B10G10R10                      DXGI_FORMAT_R10G10B10A2
D3DFMT_A8B8G8R8                         DXGI_FORMAT_R8G8B8A8_UNORM
D3DFMT_X8B8G8R8                         Not_available
D3DFMT_G16R16                           DXGI_FORMAT_R16G16_UNORM
D3DFMT_A2R10G10B10                      Not_available
D3DFMT_A16B16G16R16                     DXGI_FORMAT_R16G16B16A16_UNORM
D3DFMT_A8P8                             Not_available
D3DFMT_P8                               Not_available
D3DFMT_L8                               DXGI_FORMAT_R8_UNORM
D3DFMT_A8L8                             Not_available
D3DFMT_A4L4                             Not_available
D3DFMT_V8U8                             DXGI_FORMAT_R8G8_SNORM
D3DFMT_L6V5U5                           Not_available
D3DFMT_X8L8V8U8                         Not_available
D3DFMT_Q8W8V8U8                         DXGI_FORMAT_R8G8B8A8_SNORM
D3DFMT_V16U16                           DXGI_FORMAT_R16G16_SNORM
D3DFMT_W11V11U10                        Not_available
D3DFMT_A2W10V10U10                      Not_available
D3DFMT_UYVY                             Not_available
D3DFMT_R8G8_B8G8                        DXGI_FORMAT_G8R8_G8B8_UNORM
D3DFMT_YUY2                             Not_available
D3DFMT_G8R8_G8B8                        DXGI_FORMAT_R8G8_B8G8_UNORM
D3DFMT_DXT1                             DXGI_FORMAT_BC1_UNORM
D3DFMT_DXT2                             DXGI_FORMAT_BC2_UNORM
D3DFMT_DXT3                             DXGI_FORMAT_BC2_UNORM
D3DFMT_DXT4                             DXGI_FORMAT_BC3_UNORM
D3DFMT_DXT5                             DXGI_FORMAT_BC3_UNORM
D3DFMT_D16                              and
D3DFMT_D32                              Not_available
D3DFMT_D15S1                            Not_available
D3DFMT_D24S8                            Not_available
D3DFMT_D24X8                            Not_available
D3DFMT_D24X4S4                          Not_available
D3DFMT_D16                              DXGI_FORMAT_D16_UNORM
D3DFMT_D32F_LOCKABLE                    DXGI_FORMAT_D32_FLOAT
D3DFMT_D24FS8                           Not_available
D3DFMT_S1D15                            Not_available
D3DFMT_S8D24                            DXGI_FORMAT_D24_UNORM_S8_UINT
D3DFMT_X8D24                            Not_available
D3DFMT_X4S4D24                          Not_available
D3DFMT_L16                              DXGI_FORMAT_R16_UNORM
D3DFMT_INDEX16                          DXGI_FORMAT_R16_UINT
D3DFMT_INDEX32                          DXGI_FORMAT_R32_UINT
D3DFMT_Q16W16V16U16                     DXGI_FORMAT_R16G16B16A16_SNORM
D3DFMT_MULTI2_ARGB8                     Not_available
D3DFMT_R16F                             DXGI_FORMAT_R16_FLOAT
D3DFMT_G16R16F                          DXGI_FORMAT_R16G16_FLOAT
D3DFMT_A16B16G16R16F                    DXGI_FORMAT_R16G16B16A16_FLOAT
D3DFMT_R32F                             DXGI_FORMAT_R32_FLOAT
D3DFMT_G32R32F                          DXGI_FORMAT_R32G32_FLOAT
D3DFMT_A32B32G32R32F                    DXGI_FORMAT_R32G32B32A32_FLOAT
D3DFMT_CxV8U8                           Not_available
D3DDECLTYPE_FLOAT1                      DXGI_FORMAT_R32_FLOAT
D3DDECLTYPE_FLOAT2                      DXGI_FORMAT_R32G32_FLOAT
D3DDECLTYPE_FLOAT3                      DXGI_FORMAT_R32G32B32_FLOAT
D3DDECLTYPE_FLOAT4                      DXGI_FORMAT_R32G32B32A32_FLOAT
D3DDECLTYPED3DCOLOR                     Not_available
D3DDECLTYPE_UBYTE4                      DXGI_FORMAT_R8G8B8A8_UINT
D3DDECLTYPE_SHORT2                      DXGI_FORMAT_R16G16_SINT
D3DDECLTYPE_SHORT4                      DXGI_FORMAT_R16G16B16A16_SINT
D3DDECLTYPE_UBYTE4N                     DXGI_FORMAT_R8G8B8A8_UNORM
D3DDECLTYPE_SHORT2N                     DXGI_FORMAT_R16G16_SNORM
D3DDECLTYPE_SHORT4N                     DXGI_FORMAT_R16G16B16A16_SNORM
D3DDECLTYPE_USHORT2N                    DXGI_FORMAT_R16G16_UNORM
D3DDECLTYPE_USHORT4N                    DXGI_FORMAT_R16G16B16A16_UNORM
D3DDECLTYPE_UDEC3                       Not_available
D3DDECLTYPE_DEC3N                       Not_available
D3DDECLTYPE_FLOAT16_2                   DXGI_FORMAT_R16G16_FLOAT
D3DDECLTYPE_FLOAT16_4                   DXGI_FORMAT_R16G16B16A16_FLOAT
*/


enum DDS_DWCAPS {
	DDSCAPS_COMPLEX = 0x08,
	DDSCAPS_MIPMAP = 0x400000,
	DDSCAPS_TEXTURE = 0x1000
	};
enum D3D10_RESOURCE_MISC_FLAG {
	D3D10_RESOURCE_MISC_GENERATE_MIPS,
	D3D10_RESOURCE_MISC_SHARED,
	D3D10_RESOURCE_MISC_TEXTURECUBE,
	D3D10_RESOURCE_MISC_SHARED_KEYEDMUTEX,
	D3D10_RESOURCE_MISC_GDI_COMPATIBLE
	};
enum D3D10_RESOURCE_DIMENSION {
	D3D10_RESOURCE_DIMENSION_UNKNOWN,
	D3D10_RESOURCE_DIMENSION_BUFFER,
	D3D10_RESOURCE_DIMENSION_TEXTURE1D,
	D3D10_RESOURCE_DIMENSION_TEXTURE2D,
	D3D10_RESOURCE_DIMENSION_TEXTURE3D
	};
enum DDS_HEADER_DWFLAGS {
	DDSD_CAPS = 0x01,
	DDSD_HEIGHT = 0x02,
	DDSD_WIDTH = 0x04,
	DDSD_PITCH = 0x08,
	DDSD_PIXELFORMAT = 0x1000,
	DDSD_MIPMAPCOUNT = 0x20000,
	DDSD_LINEARSIZE = 0x80000,
	DDSD_DEPTH = 0x800000
	};
enum DDS_PIXELFORMAT_DWFLAGS {
	DDPF_ALPHAPIXELS = 0x01,
	DDPF_ALPHA = 0x02,
	DDPF_FOURCC = 0x04,
	DDPF_RGB = 0x40,
	DDPF_YUV = 0x200,
	DDPF_LUMINANCE = 0x20000
	};
enum DXGI_FORMAT {
	DXGI_FORMAT_UNKNOWN,
	DXGI_FORMAT_R32G32B32A32_TYPELESS,
	DXGI_FORMAT_R32G32B32A32_FLOAT,
	DXGI_FORMAT_R32G32B32A32_UINT,
	DXGI_FORMAT_R32G32B32A32_SINT,
	DXGI_FORMAT_R32G32B32_TYPELESS,
	DXGI_FORMAT_R32G32B32_FLOAT,
	DXGI_FORMAT_R32G32B32_UINT,
	DXGI_FORMAT_R32G32B32_SINT,
	DXGI_FORMAT_R16G16B16A16_TYPELESS,
	DXGI_FORMAT_R16G16B16A16_FLOAT,
	DXGI_FORMAT_R16G16B16A16_UNORM,
	DXGI_FORMAT_R16G16B16A16_UINT,
	DXGI_FORMAT_R16G16B16A16_SNORM,
	DXGI_FORMAT_R16G16B16A16_SINT,
	DXGI_FORMAT_R32G32_TYPELESS,
	DXGI_FORMAT_R32G32_FLOAT,
	DXGI_FORMAT_R32G32_UINT,
	DXGI_FORMAT_R32G32_SINT,
	DXGI_FORMAT_R32G8X24_TYPELESS,
	DXGI_FORMAT_D32_FLOAT_S8X24_UINT,
	DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS,
	DXGI_FORMAT_X32_TYPELESS_G8X24_UINT,
	DXGI_FORMAT_R10G10B10A2_TYPELESS,
	DXGI_FORMAT_R10G10B10A2_UNORM,
	DXGI_FORMAT_R10G10B10A2_UINT,
	DXGI_FORMAT_R11G11B10_FLOAT,
	DXGI_FORMAT_R8G8B8A8_TYPELESS,
	DXGI_FORMAT_R8G8B8A8_UNORM,
	DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
	DXGI_FORMAT_R8G8B8A8_UINT,
	DXGI_FORMAT_R8G8B8A8_SNORM,
	DXGI_FORMAT_R8G8B8A8_SINT,
	DXGI_FORMAT_R16G16_TYPELESS,
	DXGI_FORMAT_R16G16_FLOAT,
	DXGI_FORMAT_R16G16_UNORM,
	DXGI_FORMAT_R16G16_UINT,
	DXGI_FORMAT_R16G16_SNORM,
	DXGI_FORMAT_R16G16_SINT,
	DXGI_FORMAT_R32_TYPELESS,
	DXGI_FORMAT_D32_FLOAT,
	DXGI_FORMAT_R32_FLOAT,
	DXGI_FORMAT_R32_UINT,
	DXGI_FORMAT_R32_SINT,
	DXGI_FORMAT_R24G8_TYPELESS,
	DXGI_FORMAT_D24_UNORM_S8_UINT,
	DXGI_FORMAT_R24_UNORM_X8_TYPELESS,
	DXGI_FORMAT_X24_TYPELESS_G8_UINT,
	DXGI_FORMAT_R8G8_TYPELESS,
	DXGI_FORMAT_R8G8_UNORM,
	DXGI_FORMAT_R8G8_UINT,
	DXGI_FORMAT_R8G8_SNORM,
	DXGI_FORMAT_R8G8_SINT,
	DXGI_FORMAT_R16_TYPELESS,
	DXGI_FORMAT_R16_FLOAT,
	DXGI_FORMAT_D16_UNORM,
	DXGI_FORMAT_R16_UNORM,
	DXGI_FORMAT_R16_UINT,
	DXGI_FORMAT_R16_SNORM,
	DXGI_FORMAT_R16_SINT,
	DXGI_FORMAT_R8_TYPELESS,
	DXGI_FORMAT_R8_UNORM,
	DXGI_FORMAT_R8_UINT,
	DXGI_FORMAT_R8_SNORM,
	DXGI_FORMAT_R8_SINT,
	DXGI_FORMAT_A8_UNORM,
	DXGI_FORMAT_R1_UNORM,
	DXGI_FORMAT_R9G9B9E5_SHAREDEXP,
	DXGI_FORMAT_R8G8_B8G8_UNORM,
	DXGI_FORMAT_G8R8_G8B8_UNORM,
	DXGI_FORMAT_BC1_TYPELESS,
	DXGI_FORMAT_BC1_UNORM,
	DXGI_FORMAT_BC1_UNORM_SRGB,
	DXGI_FORMAT_BC2_TYPELESS,
	DXGI_FORMAT_BC2_UNORM,
	DXGI_FORMAT_BC2_UNORM_SRGB,
	DXGI_FORMAT_BC3_TYPELESS,
	DXGI_FORMAT_BC3_UNORM,
	DXGI_FORMAT_BC3_UNORM_SRGB,
	DXGI_FORMAT_BC4_TYPELESS,
	DXGI_FORMAT_BC4_UNORM,
	DXGI_FORMAT_BC4_SNORM,
	DXGI_FORMAT_BC5_TYPELESS,
	DXGI_FORMAT_BC5_UNORM,
	DXGI_FORMAT_BC5_SNORM,
	DXGI_FORMAT_B5G6R5_UNORM,
	DXGI_FORMAT_B5G5R5A1_UNORM,
	DXGI_FORMAT_B8G8R8A8_UNORM,
	DXGI_FORMAT_B8G8R8X8_UNORM,
	DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM,
	DXGI_FORMAT_B8G8R8A8_TYPELESS,
	DXGI_FORMAT_B8G8R8A8_UNORM_SRGB,
	DXGI_FORMAT_B8G8R8X8_TYPELESS,
	DXGI_FORMAT_B8G8R8X8_UNORM_SRGB,
	DXGI_FORMAT_BC6H_TYPELESS,
	DXGI_FORMAT_BC6H_UF16,
	DXGI_FORMAT_BC6H_SF16,
	DXGI_FORMAT_BC7_TYPELESS,
	DXGI_FORMAT_BC7_UNORM,
	DXGI_FORMAT_BC7_UNORM_SRGB,
	DXGI_FORMAT_AYUV,
	DXGI_FORMAT_Y410,
	DXGI_FORMAT_Y416,
	DXGI_FORMAT_NV12,
	DXGI_FORMAT_P010,
	DXGI_FORMAT_P016,
	DXGI_FORMAT_420_OPAQUE,
	DXGI_FORMAT_YUY2,
	DXGI_FORMAT_Y210,
	DXGI_FORMAT_Y216,
	DXGI_FORMAT_NV11,
	DXGI_FORMAT_AI44,
	DXGI_FORMAT_IA44,
	DXGI_FORMAT_P8,
	DXGI_FORMAT_A8P8,
	DXGI_FORMAT_B4G4R4A4_UNORM,
	DXGI_FORMAT_P208,
	DXGI_FORMAT_V208,
	DXGI_FORMAT_V408,
	DXGI_FORMAT_FORCE_UINT = 0xFFFFFFFF
	};

const char* DXGI_FORMAT_TABLE[120] = {
	"UNKNOWN",
	"R32G32B32A32_TYPELESS",
	"R32G32B32A32_FLOAT",
	"R32G32B32A32_UINT",
	"R32G32B32A32_SINT",
	"R32G32B32_TYPELESS",
	"R32G32B32_FLOAT",
	"R32G32B32_UINT",
	"R32G32B32_SINT",
	"R16G16B16A16_TYPELESS",
	"R16G16B16A16_FLOAT",
	"R16G16B16A16_UNORM",
	"R16G16B16A16_UINT",
	"R16G16B16A16_SNORM",
	"R16G16B16A16_SINT",
	"R32G32_TYPELESS",
	"R32G32_FLOAT",
	"R32G32_UINT",
	"R32G32_SINT",
	"R32G8X24_TYPELESS",
	"D32_FLOAT_S8X24_UINT",
	"R32_FLOAT_X8X24_TYPELESS",
	"X32_TYPELESS_G8X24_UINT",
	"R10G10B10A2_TYPELESS",
	"R10G10B10A2_UNORM",
	"R10G10B10A2_UINT",
	"R11G11B10_FLOAT",
	"R8G8B8A8_TYPELESS",
	"R8G8B8A8_UNORM",
	"R8G8B8A8_UNORM_SRGB",
	"R8G8B8A8_UINT",
	"R8G8B8A8_SNORM",
	"R8G8B8A8_SINT",
	"R16G16_TYPELESS",
	"R16G16_FLOAT",
	"R16G16_UNORM",
	"R16G16_UINT",
	"R16G16_SNORM",
	"R16G16_SINT",
	"R32_TYPELESS",
	"D32_FLOAT",
	"R32_FLOAT",
	"R32_UINT",
	"R32_SINT",
	"R24G8_TYPELESS",
	"D24_UNORM_S8_UINT",
	"R24_UNORM_X8_TYPELESS",
	"X24_TYPELESS_G8_UINT",
	"R8G8_TYPELESS",
	"R8G8_UNORM",
	"R8G8_UINT",
	"R8G8_SNORM",
	"R8G8_SINT",
	"R16_TYPELESS",
	"R16_FLOAT",
	"D16_UNORM",
	"R16_UNORM",
	"R16_UINT",
	"R16_SNORM",
	"R16_SINT",
	"R8_TYPELESS",
	"R8_UNORM",
	"R8_UINT",
	"R8_SNORM",
	"R8_SINT",
	"A8_UNORM",
	"R1_UNORM",
	"R9G9B9E5_SHAREDEXP",
	"R8G8_B8G8_UNORM",
	"G8R8_G8B8_UNORM",
	"BC1_TYPELESS",
	"BC1_UNORM",
	"BC1_UNORM_SRGB",
	"BC2_TYPELESS",
	"BC2_UNORM",
	"BC2_UNORM_SRGB",
	"BC3_TYPELESS",
	"BC3_UNORM",
	"BC3_UNORM_SRGB",
	"BC4_TYPELESS",
	"BC4_UNORM",
	"BC4_SNORM",
	"BC5_TYPELESS",
	"BC5_UNORM",
	"BC5_SNORM",
	"B5G6R5_UNORM",
	"B5G5R5A1_UNORM",
	"B8G8R8A8_UNORM",
	"B8G8R8X8_UNORM",
	"R10G10B10_XR_BIAS_A2_UNORM",
	"B8G8R8A8_TYPELESS",
	"B8G8R8A8_UNORM_SRGB",
	"B8G8R8X8_TYPELESS",
	"B8G8R8X8_UNORM_SRGB",
	"BC6H_TYPELESS",
	"BC6H_UF16",
	"BC6H_SF16",
	"BC7_TYPELESS",
	"BC7_UNORM",
	"BC7_UNORM_SRGB",
	"AYUV",
	"Y410",
	"Y416",
	"NV12",
	"P010",
	"P016",
	"420_OPAQUE",
	"YUY2",
	"Y210",
	"Y216",
	"NV11",
	"AI44",
	"IA44",
	"P8",
	"A8P8",
	"B4G4R4A4_UNORM",
	"P208",
	"V208",
	"V408",
	"FORCE_UINT"
	};

const int DXGI_FORMAT_BPP_TABLE[120] = {
	0,	// UNKNOWN
	128,	// R32G32B32A32_TYPELESS
	128,	// R32G32B32A32_FLOAT
	128,	// R32G32B32A32_UINT
	128,	// R32G32B32A32_SINT
	96,	// R32G32B32_TYPELESS
	96,	// R32G32B32_FLOAT
	96,	// R32G32B32_UINT
	96,	// R32G32B32_SINT
	64,	// R16G16B16A16_TYPELESS
	64,	// R16G16B16A16_FLOAT
	64,	// R16G16B16A16_UNORM
	64,	// R16G16B16A16_UINT
	64,	// R16G16B16A16_SNORM
	64,	// R16G16B16A16_SINT
	64,	// R32G32_TYPELESS
	64,	// R32G32_FLOAT
	64,	// R32G32_UINT
	64,	// R32G32_SINT
	64,	// R32G8X24_TYPELESS
	64,	// D32_FLOAT_S8X24_UINT
	64,	// R32_FLOAT_X8X24_TYPELESS
	64,	// X32_TYPELESS_G8X24_UINT
	32,	// R10G10B10A2_TYPELESS
	32,	// R10G10B10A2_UNORM
	32,	// R10G10B10A2_UINT
	32,	// R11G11B10_FLOAT
	32,	// R8G8B8A8_TYPELESS
	32,	// R8G8B8A8_UNORM
	32,	// R8G8B8A8_UNORM_SRGB
	32,	// R8G8B8A8_UINT
	32,	// R8G8B8A8_SNORM
	32,	// R8G8B8A8_SINT
	32,	// R16G16_TYPELESS
	32,	// R16G16_FLOAT
	32,	// R16G16_UNORM
	32,	// R16G16_UINT
	32,	// R16G16_SNORM
	32,	// R16G16_SINT
	32,	// R32_TYPELESS
	32,	// D32_FLOAT
	32,	// R32_FLOAT
	32,	// R32_UINT
	32,	// R32_SINT
	32,	// R24G8_TYPELESS
	32,	// D24_UNORM_S8_UINT
	32,	// R24_UNORM_X8_TYPELESS
	32,	// X24_TYPELESS_G8_UINT
	16,	// R8G8_TYPELESS
	16,	// R8G8_UNORM
	16,	// R8G8_UINT
	16,	// R8G8_SNORM
	16,	// R8G8_SINT
	16,	// R16_TYPELESS
	16,	// R16_FLOAT
	16,	// D16_UNORM
	16,	// R16_UNORM
	16,	// R16_UINT
	16,	// R16_SNORM
	16,	// R16_SINT
	8,	// R8_TYPELESS
	8,	// R8_UNORM
	8,	// R8_UINT
	8,	// R8_SNORM
	8,	// R8_SINT
	8,	// A8_UNORM
	1,	// R1_UNORM
	32,	// R9G9B9E5_SHAREDEXP
	32,	// R8G8_B8G8_UNORM
	32,	// G8R8_G8B8_UNORM
	4,	// BC1_TYPELESS
	4,	// BC1_UNORM
	4,	// BC1_UNORM_SRGB
	8,	// BC2_TYPELESS
	8,	// BC2_UNORM
	8,	// BC2_UNORM_SRGB
	8,	// BC3_TYPELESS
	8,	// BC3_UNORM
	8,	// BC3_UNORM_SRGB
	4,	// BC4_TYPELESS
	4,	// BC4_UNORM
	4,	// BC4_SNORM
	8,	// BC5_TYPELESS
	8,	// BC5_UNORM
	8,	// BC5_SNORM
	16,	// B5G6R5_UNORM
	16,	// B5G5R5A1_UNORM
	32,	// B8G8R8A8_UNORM
	32,	// B8G8R8X8_UNORM
	32,	// R10G10B10_XR_BIAS_A2_UNORM
	32,	// B8G8R8A8_TYPELESS
	32,	// B8G8R8A8_UNORM_SRGB
	32,	// B8G8R8X8_TYPELESS
	32,	// B8G8R8X8_UNORM_SRGB
	8,	// BC6H_TYPELESS
	8,	// BC6H_UF16
	8,	// BC6H_SF16
	8,	// BC7_TYPELESS
	8,	// BC7_UNORM
	8,	// BC7_UNORM_SRGB
	8,	// AYUV
	8,	// Y410
	8,	// Y416
	8,	// NV12
	8,	// P010
	8,	// P016
	24,	// 420_OPAQUE
	2,	// YUY2
	12,	// Y210
	18,	// Y216
	2,	// NV11
	8,	// AI44
	8,	// IA44
	8,	// P8
	16,	// A8P8
	16,	// B4G4R4A4_UNORM
	8,	// P208
	8,	// V208
	8,	// V408
	8	// FORCE_UINT
	};

const bool DXGI_COMPRESSED_TABLE[120] = {
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	true,
	true,
	true,
	true,
	true,
	true,
	true,
	true,
	true,
	true,
	true,
	true,
	true,
	true,
	true,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	true,
	true,
	true,
	true,
	true,
	true,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false
	};

class img_dds {
private:
	//unsigned long max (unsigned long a, unsigned long b) {
	//	return (a<b)?b:a;
	//	}
	bool isDX10;
	unsigned long dwMagic;
	unsigned long dwSize;
	unsigned long dwFlags;
	unsigned long dwHeight;
	unsigned long dwWidth;
	unsigned long dwPitch;
	unsigned long dwLinearSize;
	unsigned long dwDepth;
	unsigned long dwMipMapCount;
	unsigned long dwReserved1[11];
	unsigned long fmSize;
	unsigned long fmFlags;
	unsigned long fmFourCC;
	unsigned long dxgiFormat;
	unsigned long resourceDimension;
	unsigned long miscFlag;
	unsigned long arraySize;
	unsigned long miscFlags2;
	unsigned long dwRGBBitCount;
	unsigned long dwRBitMask;
	unsigned long dwGBitMask;
	unsigned long dwBBitMask;
	unsigned long dwABitMask;
	unsigned long dwCaps;
	unsigned long dwCaps2;
	unsigned long dwCaps3;
	unsigned long dwCaps4;
	unsigned long dwReserved2;

public:
	unsigned long width;
	unsigned long height;
	unsigned long mipCount;
	unsigned long type;
	img_dds (unsigned long texWidth = 0, unsigned long texHeight = 0, unsigned long texMips = 1, unsigned long texType = 0) {
		isDX10 = false;
		//pos = 0;
		//size = 0;
		width = texWidth;
		height = texHeight;
		type = texType;
		dwMagic = 0x20534444;
		dwSize = 0x7C;
		dwFlags = 0x0002100F;
		dwHeight = width;
		dwWidth = width;
		dwLinearSize = (width * 8 + 7) / 8;
		dwDepth = 0;
		dwMipMapCount = 0;
		dwReserved1[0] = 0;
		dwReserved1[1] = 0;
		dwReserved1[2] = 0;
		dwReserved1[3] = 0;
		dwReserved1[4] = 0;
		dwReserved1[5] = 0;
		dwReserved1[6] = 0;
		dwReserved1[7] = 0;
		dwReserved1[8] = 0;
		dwReserved1[9] = 0;
		dwReserved1[10] = 0;
		fmSize = 0x20;
		fmFlags = 0x04;
		fmFourCC = 0x30315844;
		dxgiFormat = 0;
		resourceDimension = 0;
		miscFlag = 0;
		arraySize = 0;
		miscFlags2 = 0;
		dwRGBBitCount = 0x00401008;
		dwRBitMask = 0;
		dwGBitMask = 0;
		dwBBitMask = 0;
		dwABitMask = 0;
		dwCaps = 0x00401008;
		dwCaps2 = 3;
		dwCaps3 = 0;
		dwCaps4 = 1;
		dwReserved2 = 0;
		}
	std::string getFormatName (unsigned long id) {
		return ((std::string)DXGI_FORMAT_TABLE[id]);
		}
	unsigned long SET_DWFLAGS (bool CAPS = false, bool HEIGHT = false, bool WIDTH = false, bool PITCH = false, bool PIXELFORMAT = false, bool MIPMAPCOUNT = false, bool LINEARSIZE = false, bool DEPTH = false) {
		return (DDSD_CAPS * CAPS | DDSD_HEIGHT * HEIGHT | DDSD_WIDTH * WIDTH | DDSD_PITCH * PITCH | DDSD_PIXELFORMAT * PIXELFORMAT | DDSD_MIPMAPCOUNT * MIPMAPCOUNT | DDSD_LINEARSIZE * LINEARSIZE | DDSD_DEPTH * DEPTH);
		}
	DXGI_FORMAT getFormatFromName (std::string name) {
		DXGI_FORMAT result = DXGI_FORMAT_UNKNOWN;
		if (name.size() > 0) {
			for (int i = 0; i < 120; i++) {
				if (name == (std::string)DXGI_FORMAT_TABLE[i]) {
					result = DXGI_FORMAT(i);
					break;
					}
				}
			}
		return result;
		}
	std::string formatID_to_formatName (unsigned long formatid) {
		return DXGI_FORMAT_TABLE[formatid];
		}
	bool isFormatDX10 () {
		return isDX10;
		}
	bool set_isDX10_flag (unsigned long id) {
		bool result = false;
		switch (id) {
			case DXGI_FORMAT_R32G32B32A32_UINT :
				result = true;
				break;
			case DXGI_FORMAT_R32G32B32A32_SINT :
				result = true;
				break;
			case DXGI_FORMAT_R32G32B32_FLOAT :
				result = true;
				break;
			case DXGI_FORMAT_R32G32B32_UINT :
				result = true;
				break;
			case DXGI_FORMAT_R32G32B32_SINT :
				result = true;
				break;
			case DXGI_FORMAT_R16G16B16A16_UINT :
				result = true;
				break;
			case DXGI_FORMAT_R16G16B16A16_SINT :
				result = true;
				break;
			case DXGI_FORMAT_R32G32_UINT :
				result = true;
				break;
			case DXGI_FORMAT_R32G32_SINT :
				result = true;
				break;
			case DXGI_FORMAT_R10G10B10A2_UNORM :
				result = true;
				break;
			case DXGI_FORMAT_R10G10B10A2_UINT :
				result = true;
				break;
			case DXGI_FORMAT_R11G11B10_FLOAT :
				result = true;
				break;
			case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB :
				result = true;
				break;
			case DXGI_FORMAT_R8G8B8A8_UINT :
				result = true;
				break;
			case DXGI_FORMAT_R8G8B8A8_SINT :
				result = true;
				break;
			case DXGI_FORMAT_R16G16_UINT :
				result = true;
				break;
			case DXGI_FORMAT_R16G16_SINT :
				result = true;
				break;
			case DXGI_FORMAT_R32_UINT :
				result = true;
				break;
			case DXGI_FORMAT_R32_SINT :
				result = true;
				break;
			case DXGI_FORMAT_R8G8_UINT :
				result = true;
				break;
			case DXGI_FORMAT_R8G8_SINT :
				result = true;
				break;
			case DXGI_FORMAT_R16_UINT :
				result = true;
				break;
			case DXGI_FORMAT_R16_SNORM :
				result = true;
				break;
			case DXGI_FORMAT_R16_SINT :
				result = true;
				break;
			case DXGI_FORMAT_R8_UINT :
				result = true;
				break;
			case DXGI_FORMAT_R8_SNORM :
				result = true;
				break;
			case DXGI_FORMAT_R8_SINT :
				result = true;
				break;
			case DXGI_FORMAT_R9G9B9E5_SHAREDEXP :
				result = true;
				break;
			case DXGI_FORMAT_BC1_UNORM_SRGB :
				result = true;
				break;
			case DXGI_FORMAT_BC2_UNORM_SRGB :
				result = true;
				break;
			case DXGI_FORMAT_BC3_UNORM_SRGB :
				result = true;
				break;
			case DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM :
				result = true;
				break;
			case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB :
				result = true;
				break;
			case DXGI_FORMAT_B8G8R8X8_UNORM_SRGB :
				result = true;
				break;
			case DXGI_FORMAT_BC6H_UF16 :
				result = true;
				break;
			case DXGI_FORMAT_BC6H_SF16 :
				result = true;
				break;
			case DXGI_FORMAT_BC7_UNORM :
				result = true;
				break;
			case DXGI_FORMAT_BC7_UNORM_SRGB :
				result = true;
				break;
			case DXGI_FORMAT_AYUV :
				result = true;
				break;
			case DXGI_FORMAT_Y410 :
				result = true;
				break;
			case DXGI_FORMAT_Y416 :
				result = true;
				break;
			case DXGI_FORMAT_Y210 :
				result = true;
				break;
			case DXGI_FORMAT_Y216 :
				result = true;
				break;
			default :
				result = false;
			}
		return result;
		}
	void storeLong (char* &buf, unsigned long idx, unsigned long value) {
		buf[idx+0] = (char)(value & 0x000000ff);
		buf[idx+1] = (char)((value & 0x0000ff00) >> 8);
		buf[idx+2] = (char)((value & 0x00ff0000) >> 16);
		buf[idx+3] = (char)((value & 0xff000000) >> 24);
		//idx+=4;
		}
	void setHeader (unsigned long texWidth, unsigned long texHeight, unsigned long texMips, DXGI_FORMAT texType) {
		isDX10 = set_isDX10_flag(texType);

		//cout << "texType " << texType << " " << isDX10 << endl;

		dwWidth = texWidth;
		dwHeight = texHeight;
		dwMipMapCount = texMips;

		//std::cout << ((unsigned int)DXGI_FORMAT_BPP_TABLE[texType]) << std::endl;
		//std::cout << (((float)DXGI_FORMAT_BPP_TABLE[texType] / 8.0f)) << std::endl;
		//std::cout << ((texWidth * texHeight)) << std::endl;
		//std::cout << ((texWidth * texHeight) * ((float)DXGI_FORMAT_BPP_TABLE[texType] / 8.0f)) << std::endl;

        long unsigned int nom = 1;
		dwPitch = (unsigned long)((float)(texWidth * texHeight)) * ((float)DXGI_FORMAT_BPP_TABLE[texType] / 8.0f);
		//std::cout << dwPitch << std::endl;
		switch (texType) {
			case DXGI_FORMAT_R32G32B32A32_FLOAT:
				fmFourCC = 0x00000074;
				dwRGBBitCount = 0x00;
				dwLinearSize = ( dwWidth * 128 + 7 ) / 8;
				break;
			case DXGI_FORMAT_R32G32B32A32_UINT:
				dwLinearSize = ( dwWidth * 128 + 7 ) / 8;
				dwCaps = 3;
				break;
			case DXGI_FORMAT_R32G32B32A32_SINT:
				dwLinearSize = ( dwWidth * 96 + 7 ) / 8;
				dwCaps = 4;
				break;
			case DXGI_FORMAT_R32G32B32_FLOAT:
				dwLinearSize = ( dwWidth * 96 + 7 ) / 8;
				dwCaps = 6;
				break;
			case DXGI_FORMAT_R32G32B32_UINT:
				dwLinearSize = ( dwWidth * 96 + 7 ) / 8;
				dwCaps = 7;
				break;
			case DXGI_FORMAT_R32G32B32_SINT:
				dwLinearSize = ( dwWidth * 96 + 7 ) / 8;
				dwCaps = 8;
				break;
			case DXGI_FORMAT_R16G16B16A16_FLOAT:
				fmFourCC = 0x00000071;
				dwLinearSize = ( dwWidth * 64 + 7 ) / 8;
				dwRGBBitCount = 0x00;
				break;
			case DXGI_FORMAT_R16G16B16A16_UNORM:
				fmFourCC = 0x00000024;
				dwLinearSize = ( dwWidth * 64 + 7 ) / 8;
				dwRGBBitCount = 0x00;
				break;
			case DXGI_FORMAT_R16G16B16A16_UINT:
				dwLinearSize = ( dwWidth * 64 + 7 ) / 8;
				dwCaps = 12;
				break;
			case DXGI_FORMAT_R16G16B16A16_SNORM:
				fmFourCC = 0x0000006E;
				dwLinearSize = ( dwWidth * 64 + 7 ) / 8;
				dwRGBBitCount = 0x00;
				break;
			case DXGI_FORMAT_R16G16B16A16_SINT:
				dwLinearSize = ( dwWidth * 64 + 7 ) / 8;
				dwCaps = 14;
				break;
			case DXGI_FORMAT_R32G32_FLOAT:
				fmFourCC = 0x00000073;
				dwLinearSize = ( dwWidth * 64 + 7 ) / 8;
				dwRGBBitCount = 0x00;
				break;
			case DXGI_FORMAT_R32G32_UINT:
				dwLinearSize = ( dwWidth * 64 + 7 ) / 8;
				dwCaps = 17;
				break;
			case DXGI_FORMAT_R32G32_SINT:
				dwLinearSize = ( dwWidth * 64 + 7 ) / 8;
				dwCaps = 18;
				break;
			case DXGI_FORMAT_R10G10B10A2_UNORM:
				dwLinearSize = ( dwWidth * 32 + 7 ) / 8;
				dwCaps = 24;
				break;
			case DXGI_FORMAT_R10G10B10A2_UINT:
				dwLinearSize = ( dwWidth * 32 + 7 ) / 8;
				dwCaps = 25;
				break;
			case DXGI_FORMAT_R11G11B10_FLOAT:
				dwLinearSize = ( dwWidth * 32 + 7 ) / 8;
				dwCaps = 26;
				break;
			case DXGI_FORMAT_R8G8B8A8_UNORM:
				dwLinearSize = ( dwWidth * 32 + 7 ) / 8;
				dwRGBBitCount = 0x20;
				fmFlags = 0x00000041;
				break;
			case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
				fmFourCC = 0x00000000;
				dwLinearSize = ( dwWidth * 32 + 7 ) / 8;
				dwCaps = 29;
				break;
			case DXGI_FORMAT_R8G8B8A8_UINT:
				dwLinearSize = ( dwWidth * 32 + 7 ) / 8;
				dwCaps = 30;
				break;
			case DXGI_FORMAT_R8G8B8A8_SNORM:
				fmFourCC = 0x00000000;
				dwLinearSize = ( dwWidth * 32 + 7 ) / 8;
				fmFlags = 0x00080000;
				dwRGBBitCount = 0x20;
				break;
			case DXGI_FORMAT_R8G8B8A8_SINT:
				dwLinearSize = ( dwWidth * 32 + 7 ) / 8;
				dwCaps = 32;
				break;
			case DXGI_FORMAT_R16G16_FLOAT:
				dwLinearSize = ( dwWidth * 32 + 7 ) / 8;
				dwRGBBitCount = 0x00;
				break;
			case DXGI_FORMAT_R16G16_UNORM:
				fmFourCC = 0x00000000;
				dwLinearSize = ( dwWidth * 32 + 7 ) / 8;
				fmFlags = 0x00000041;
				dwRGBBitCount = 0x20;
				break;
			case DXGI_FORMAT_R16G16_UINT:
				dwLinearSize = ( dwWidth * 32 + 7 ) / 8;
				dwCaps = 36;
				break;
			case DXGI_FORMAT_R16G16_SNORM:
				fmFourCC = 0x00000000;
				dwLinearSize = ( dwWidth * 32 + 7 ) / 8;
				fmFlags = 0x00080000;
				dwRGBBitCount = 0x20;
				break;
			case DXGI_FORMAT_R16G16_SINT:
				dwLinearSize = ( dwWidth * 32 + 7 ) / 8;
				dwCaps = 38;
				break;
			case DXGI_FORMAT_R32_FLOAT:
				fmFourCC = 0x00000072;
				dwLinearSize = ( dwWidth * 32 + 7 ) / 8;
				dwRGBBitCount = 0x00;
				break;
			case DXGI_FORMAT_R32_UINT:
				dwLinearSize = ( dwWidth * 32 + 7 ) / 8;
				dwCaps = 42;
				break;
			case DXGI_FORMAT_R32_SINT:
				dwLinearSize = ( dwWidth * 32 + 7 ) / 8;
				dwCaps = 43;
				break;
			case DXGI_FORMAT_R8G8_UNORM:
				fmFourCC = 0x00000000;
				dwLinearSize = ( dwWidth * 16 + 7 ) / 8;
				fmFlags = 0x00020001;
				dwRGBBitCount = 0x10;
				break;
			case DXGI_FORMAT_R8G8_UINT:
				dwLinearSize = ( dwWidth * 16 + 7 ) / 8;
				dwCaps = 50;
				break;
			case DXGI_FORMAT_R8G8_SNORM:
				fmFourCC = 0x00000000;
				dwLinearSize = ( dwWidth * 16 + 7 ) / 8;
				dwRGBBitCount = 0x10;
				fmFlags = 0x00080000;
				break;
			case DXGI_FORMAT_R8G8_SINT:
				dwLinearSize = ( dwWidth * 16 + 7 ) / 8;
				dwCaps = 52;
				break;
			case DXGI_FORMAT_R16_FLOAT:
				dwLinearSize = ( dwWidth * 8 + 7 ) / 8;
				dwRGBBitCount = 0x00;
				break;
			case DXGI_FORMAT_R16_UNORM:
				fmFourCC = 0x00000000;
				dwLinearSize = ( dwWidth * 8 + 7 ) / 8;
				fmFlags = 0x00020000;
				dwRGBBitCount = 0x10;
				break;
			case DXGI_FORMAT_R16_UINT:
				dwLinearSize = ( dwWidth * 8 + 7 ) / 8;
				dwCaps = 57;
				break;
			case DXGI_FORMAT_R16_SNORM:
				fmFourCC = 0x00000000;
				dwLinearSize = ( dwWidth * 8 + 7 ) / 8;
				dwCaps = 58;
				break;
			case DXGI_FORMAT_R16_SINT:
				dwLinearSize = ( dwWidth * 8 + 7 ) / 8;
				dwCaps = 59;
				break;
			case DXGI_FORMAT_R8_UNORM:
				fmFourCC = 0x00000000;
				dwLinearSize = ( dwWidth * 8 + 7 ) / 8;
				fmFlags = 0x00020000;
				dwRGBBitCount = 0x08;
				break;
			case DXGI_FORMAT_R8_UINT:
				dwLinearSize = ( dwWidth * 8 + 7 ) / 8;
				dwCaps = 62;
				break;
			case DXGI_FORMAT_R8_SNORM:
				dwLinearSize = ( dwWidth * 8 + 7 ) / 8;
				dwCaps = 63;
				break;
			case DXGI_FORMAT_R8_SINT:
				dwLinearSize = ( dwWidth * 8 + 7 ) / 8;
				dwCaps = 64;
				break;
			case DXGI_FORMAT_A8_UNORM:
				fmFourCC = 0x00000000;
				dwLinearSize = ( dwWidth * 8 + 7 ) / 8;
				dwRGBBitCount = 0x08;
				break;
			case DXGI_FORMAT_R9G9B9E5_SHAREDEXP:
				dwLinearSize = ( dwWidth * 32 + 7 ) / 8;
				dwCaps = 67;
				break;
			case DXGI_FORMAT_R8G8_B8G8_UNORM:
				fmFourCC = 0x47424752;
				dwLinearSize = ( dwWidth * 32 + 7 ) / 8;
				dwRGBBitCount = 0x00;
				break;
			case DXGI_FORMAT_G8R8_G8B8_UNORM:
				fmFourCC = 0x42475247;
				dwLinearSize = ( dwWidth * 32 + 7 ) / 8;
				dwRGBBitCount = 0x00;
				break;
			case DXGI_FORMAT_BC1_UNORM:
				fmFourCC = 0x31545844;	//DXT1
				dwLinearSize = max(nom, ((dwWidth + 3) / 4) ) * 4;
				dwFlags = 0x000A1007;
				dwRGBBitCount = 0x00;
				break;
			case DXGI_FORMAT_BC1_UNORM_SRGB:
				dwLinearSize = max(nom, ((dwWidth + 3) / 4) ) * 4;
				dwCaps = 72;
				dwFlags = 0x000A1007;
				break;
			case DXGI_FORMAT_BC2_UNORM:
				fmFourCC = 0x33545844;	//DXT3
				dwLinearSize = max(nom, ((dwWidth + 3) / 4) ) * 8;
				dwFlags = 0x000A1007;
				dwRGBBitCount = 0x00;
				break;
			case DXGI_FORMAT_BC2_UNORM_SRGB:
				dwLinearSize = max(nom, ((dwWidth + 3) / 4) ) * 8;
				dwCaps = 75;
				dwFlags = 0x000A1007;
				break;
			case DXGI_FORMAT_BC3_UNORM:
				fmFourCC = 0x35545844;	//DXT5
				dwLinearSize = max(nom, ((dwWidth + 3) / 4) ) * 8;
				dwFlags = 0x000A1007;
				dwRGBBitCount = 0x00;
				break;
			case DXGI_FORMAT_BC3_UNORM_SRGB:
				dwLinearSize = max(nom, ((dwWidth + 3) / 4) ) * 8;
				dwCaps = 78;
				dwFlags = 0x000A1007;
				break;
			case DXGI_FORMAT_BC4_UNORM:
				fmFourCC = 0x55344342;	//BC4U
				dwLinearSize = max(nom, ((dwWidth + 3) / 4) ) * 4;
				dwFlags = 0x000A1007;
				dwRGBBitCount = 0x00;
				break;
			case DXGI_FORMAT_BC4_SNORM:
				fmFourCC = 0x53344342;	// BC4S
				dwLinearSize = max(nom, ((dwWidth + 3) / 4) ) * 4;
				dwFlags = 0x000A1007;
				dwRGBBitCount = 0x00;
				break;
			case DXGI_FORMAT_BC5_UNORM:
				fmFourCC = 0x55354342;	// BC5U
				dwLinearSize = max(nom, ((dwWidth + 3) / 4) ) * 8;
				dwFlags = 0x000A1007;
				dwRGBBitCount = 0x00;
				break;
			case DXGI_FORMAT_BC5_SNORM:
				fmFourCC = 0x53354342;	// BC5S
				dwLinearSize = max(nom, ((dwWidth + 3) / 4) ) * 8;
				dwFlags = 0x000A1007;
				dwRGBBitCount = 0x00;
				break;
			case DXGI_FORMAT_B5G6R5_UNORM:
				fmFourCC = 0x00000000;
				dwLinearSize = ( dwWidth * 016 + 7 ) / 8;
				fmFlags = 0x00000040;
				dwRGBBitCount = 0x10;
				break;
			case DXGI_FORMAT_B5G5R5A1_UNORM:
				fmFourCC = 0x00000000;
				dwLinearSize = ( dwWidth * 016 + 7 ) / 8;
				fmFlags = 0x00000041;
				dwRGBBitCount = 0x10;
				break;
			case DXGI_FORMAT_B8G8R8A8_UNORM:
				fmFourCC = 0x00000000;
				dwLinearSize = ( dwWidth * 032 + 7 ) / 8;
				fmFlags = 0x00000041;
				dwRGBBitCount = 0x20;
				break;
			case DXGI_FORMAT_B8G8R8X8_UNORM:
				fmFourCC = 0x00000000;
				dwLinearSize = ( dwWidth * 032 + 7 ) / 8;
				fmFlags = 0x00000040;
				dwRGBBitCount = 0x20;
				break;
			case DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM:
				dwLinearSize = ( dwWidth * 032 + 7 ) / 8;
				dwCaps = 89;
				break;
			case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
				dwLinearSize = ( dwWidth * 032 + 7 ) / 8;
				dwCaps = 91;
				break;
			case DXGI_FORMAT_B8G8R8X8_UNORM_SRGB:
				dwLinearSize = ( dwWidth * 032 + 7 ) / 8;
				dwCaps = 93;
				break;
			case DXGI_FORMAT_BC6H_UF16:
				dwLinearSize = max(nom, ((dwWidth + 3) / 4) ) * 8;
				dwCaps = 95;
				dwFlags = 0x000A1007;
				break;
			case DXGI_FORMAT_BC6H_SF16:
				dwLinearSize = max(nom, ((dwWidth + 3) / 4) ) * 8;
				dwCaps = 96;
				dwFlags = 0x000A1007;
				break;
			case DXGI_FORMAT_BC7_UNORM:
				dwLinearSize = max(nom, ((dwWidth + 3) / 4) ) * 8;
				dwCaps = 98;
				dwFlags = 0x000A1007;
				break;
			case DXGI_FORMAT_BC7_UNORM_SRGB:
				dwLinearSize = max(nom, ((dwWidth + 3) / 4) ) * 8;
				dwCaps = 99;
				dwFlags = 0x000A1007;
				break;
			case DXGI_FORMAT_AYUV:
				dwLinearSize = ((dwWidth + 1) >> 1) * 4;
				dwCaps = 100;
				break;
			case DXGI_FORMAT_Y410:
				dwLinearSize = ((dwWidth + 1) >> 1) * 4;
				dwCaps = 101;
				break;
			case DXGI_FORMAT_Y416:
				dwLinearSize = ( dwWidth * 8 + 7 ) / 8;
				dwCaps = 102;
				break;
			case DXGI_FORMAT_YUY2:
				fmFourCC = 0x32595559;
				dwLinearSize = ((dwWidth + 1) >> 1) * 4;
				dwRGBBitCount = 0x00;
				break;
			case DXGI_FORMAT_Y210:
				dwLinearSize = ((dwWidth + 1) >> 1) * 4;
				dwCaps = 108;
				break;
			case DXGI_FORMAT_Y216:
				dwLinearSize = ((dwWidth + 1) >> 1) * 4;
				dwCaps = 109;
				break;
			case DXGI_FORMAT_B4G4R4A4_UNORM:
				fmFourCC = 0x00000000;
				dwLinearSize = ( dwWidth * 016 + 7 ) / 8;
				fmFlags = 0x00000041;
				dwRGBBitCount = 0x10;
				break;
			}
		}
	void writeHeader (char* &header, unsigned long pos = 0) {
		//if (isDX10) {size = 148;} else {size = 128;}
		//cout << "DX10 " << isDX10 << endl;
		// = new char[size];
		//pos = 0;
		storeLong(header, pos, dwMagic);pos+=4;
		storeLong(header, pos, dwSize);pos+=4;
		storeLong(header, pos, dwFlags);pos+=4;
		storeLong(header, pos, dwHeight);pos+=4;
		storeLong(header, pos, dwWidth);pos+=4;
		storeLong(header, pos, dwPitch);pos+=4;
		storeLong(header, pos, dwDepth);pos+=4;
		storeLong(header, pos, dwMipMapCount);pos+=4;
		storeLong(header, pos, dwReserved1[0]);pos+=4;
		storeLong(header, pos, dwReserved1[1]);pos+=4;
		storeLong(header, pos, dwReserved1[2]);pos+=4;
		storeLong(header, pos, dwReserved1[3]);pos+=4;
		storeLong(header, pos, dwReserved1[4]);pos+=4;
		storeLong(header, pos, dwReserved1[5]);pos+=4;
		storeLong(header, pos, dwReserved1[6]);pos+=4;
		storeLong(header, pos, dwReserved1[7]);pos+=4;
		storeLong(header, pos, dwReserved1[8]);pos+=4;
		storeLong(header, pos, dwReserved1[9]);pos+=4;
		storeLong(header, pos, dwReserved1[10]);pos+=4;
		storeLong(header, pos, fmSize);pos+=4;
		storeLong(header, pos, fmFlags);pos+=4;
		storeLong(header, pos, fmFourCC);pos+=4;
		if (isDX10) {
			storeLong(header, pos, dxgiFormat);pos+=4;
			storeLong(header, pos, resourceDimension);pos+=4;
			storeLong(header, pos, miscFlag);pos+=4;
			storeLong(header, pos, arraySize);pos+=4;
			storeLong(header, pos, miscFlags2);pos+=4;
			}
		storeLong(header, pos, dwRGBBitCount);pos+=4;
		storeLong(header, pos, dwRBitMask);pos+=4;
		storeLong(header, pos, dwGBitMask);pos+=4;
		storeLong(header, pos, dwBBitMask);pos+=4;
		storeLong(header, pos, dwABitMask);pos+=4;
		storeLong(header, pos, dwCaps);pos+=4;
		storeLong(header, pos, dwCaps2);pos+=4;
		storeLong(header, pos, dwCaps3);pos+=4;
		storeLong(header, pos, dwCaps4);pos+=4;
		storeLong(header, pos, dwReserved2);pos+=4;
        //OutFile.write(header, size);
		//delete[] header;
		}
	};


#endif // IMG_DDS_H
