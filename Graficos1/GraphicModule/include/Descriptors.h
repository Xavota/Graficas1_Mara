#pragma once
#include <windows.h>

#if defined(DX11)

#include <xnamath.h>

#elif defined(OGL)

#include <glm/gtc/matrix_transform.hpp>

#endif
//#include <d3d11.h>

#define MODEL_LOAD_FORMAT_TRIANGLES 0X1
#define MODEL_LOAD_FORMAT_WIREFRAME 0X2
#define MODEL_LOAD_FORMAT_POINTS 0X4
#define MODEL_LOAD_FORMAT_RGBA 0X8
#define MODEL_LOAD_FORMAT_BGRA 0X10

#define MAX_SWAP_CHAIN_BUFFERS        ( 16 )
#define FLOAT32_MAX	( 3.402823466e+38f )

#define CPU_ACCESS_NONE    ( 0 )
#define CPU_ACCESS_DYNAMIC    ( 1 )
#define CPU_ACCESS_READ_WRITE    ( 2 )
#define CPU_ACCESS_SCRATCH    ( 3 )
#define CPU_ACCESS_FIELD        15
#define USAGE_SHADER_INPUT             ( 1L << (0 + 4) )
#define USAGE_RENDER_TARGET_OUTPUT     ( 1L << (1 + 4) )
#define USAGE_BACK_BUFFER              ( 1L << (2 + 4) )
#define USAGE_SHARED                   ( 1L << (3 + 4) )
#define USAGE_READ_ONLY                ( 1L << (4 + 4) )
#define USAGE_DISCARD_ON_PRESENT       ( 1L << (5 + 4) )
#define USAGE_UNORDERED_ACCESS         ( 1L << (6 + 4) )


#define PI               3.141592654f
#define _2PI              6.283185307f
#define _1DIVPI           0.318309886f
#define _1DIV2PI          0.159154943f
#define PIDIV2           1.570796327f
#define PIDIV4           0.785398163f

#define SELECT_0         0x00000000
#define SELECT_1         0xFFFFFFFF

#define PERMUTE_0X       0x00010203
#define PERMUTE_0Y       0x04050607
#define PERMUTE_0Z       0x08090A0B
#define PERMUTE_0W       0x0C0D0E0F
#define PERMUTE_1X       0x10111213
#define PERMUTE_1Y       0x14151617
#define PERMUTE_1Z       0x18191A1B
#define PERMUTE_1W       0x1C1D1E1F

#define CRMASK_CR6       0x000000F0
#define CRMASK_CR6TRUE   0x00000080
#define CRMASK_CR6FALSE  0x00000020
#define CRMASK_CR6BOUNDS XM_CRMASK_CR6FALSE

#define CACHE_LINE_SIZE  64

enum FORMAT
{
	FORMAT_UNKNOWN = 0,
	FORMAT_R32G32B32A32_TYPELESS = 1,
	FORMAT_R32G32B32A32_FLOAT = 2,
	FORMAT_R32G32B32A32_UINT = 3,
	FORMAT_R32G32B32A32_SINT = 4,
	FORMAT_R32G32B32_TYPELESS = 5,
	FORMAT_R32G32B32_FLOAT = 6,
	FORMAT_R32G32B32_UINT = 7,
	FORMAT_R32G32B32_SINT = 8,
	FORMAT_R16G16B16A16_TYPELESS = 9,
	FORMAT_R16G16B16A16_FLOAT = 10,
	FORMAT_R16G16B16A16_UNORM = 11,
	FORMAT_R16G16B16A16_UINT = 12,
	FORMAT_R16G16B16A16_SNORM = 13,
	FORMAT_R16G16B16A16_SINT = 14,
	FORMAT_R32G32_TYPELESS = 15,
	FORMAT_R32G32_FLOAT = 16,
	FORMAT_R32G32_UINT = 17,
	FORMAT_R32G32_SINT = 18,
	FORMAT_R32G8X24_TYPELESS = 19,
	FORMAT_D32_FLOAT_S8X24_UINT = 20,
	FORMAT_R32_FLOAT_X8X24_TYPELESS = 21,
	FORMAT_X32_TYPELESS_G8X24_UINT = 22,
	FORMAT_R10G10B10A2_TYPELESS = 23,
	FORMAT_R10G10B10A2_UNORM = 24,
	FORMAT_R10G10B10A2_UINT = 25,
	FORMAT_R11G11B10_FLOAT = 26,
	FORMAT_R8G8B8A8_TYPELESS = 27,
	FORMAT_R8G8B8A8_UNORM = 28,
	FORMAT_R8G8B8A8_UNORM_SRGB = 29,
	FORMAT_R8G8B8A8_UINT = 30,
	FORMAT_R8G8B8A8_SNORM = 31,
	FORMAT_R8G8B8A8_SINT = 32,
	FORMAT_R16G16_TYPELESS = 33,
	FORMAT_R16G16_FLOAT = 34,
	FORMAT_R16G16_UNORM = 35,
	FORMAT_R16G16_UINT = 36,
	FORMAT_R16G16_SNORM = 37,
	FORMAT_R16G16_SINT = 38,
	FORMAT_R32_TYPELESS = 39,
	FORMAT_D32_FLOAT = 40,
	FORMAT_R32_FLOAT = 41,
	FORMAT_R32_UINT = 42,
	FORMAT_R32_SINT = 43,
	FORMAT_R24G8_TYPELESS = 44,
	FORMAT_D24_UNORM_S8_UINT = 45,
	FORMAT_R24_UNORM_X8_TYPELESS = 46,
	FORMAT_X24_TYPELESS_G8_UINT = 47,
	FORMAT_R8G8_TYPELESS = 48,
	FORMAT_R8G8_UNORM = 49,
	FORMAT_R8G8_UINT = 50,
	FORMAT_R8G8_SNORM = 51,
	FORMAT_R8G8_SINT = 52,
	FORMAT_R16_TYPELESS = 53,
	FORMAT_R16_FLOAT = 54,
	FORMAT_D16_UNORM = 55,
	FORMAT_R16_UNORM = 56,
	FORMAT_R16_UINT = 57,
	FORMAT_R16_SNORM = 58,
	FORMAT_R16_SINT = 59,
	FORMAT_R8_TYPELESS = 60,
	FORMAT_R8_UNORM = 61,
	FORMAT_R8_UINT = 62,
	FORMAT_R8_SNORM = 63,
	FORMAT_R8_SINT = 64,
	FORMAT_A8_UNORM = 65,
	FORMAT_R1_UNORM = 66,
	FORMAT_R9G9B9E5_SHAREDEXP = 67,
	FORMAT_R8G8_B8G8_UNORM = 68,
	FORMAT_G8R8_G8B8_UNORM = 69,
	FORMAT_BC1_TYPELESS = 70,
	FORMAT_BC1_UNORM = 71,
	FORMAT_BC1_UNORM_SRGB = 72,
	FORMAT_BC2_TYPELESS = 73,
	FORMAT_BC2_UNORM = 74,
	FORMAT_BC2_UNORM_SRGB = 75,
	FORMAT_BC3_TYPELESS = 76,
	FORMAT_BC3_UNORM = 77,
	FORMAT_BC3_UNORM_SRGB = 78,
	FORMAT_BC4_TYPELESS = 79,
	FORMAT_BC4_UNORM = 80,
	FORMAT_BC4_SNORM = 81,
	FORMAT_BC5_TYPELESS = 82,
	FORMAT_BC5_UNORM = 83,
	FORMAT_BC5_SNORM = 84,
	FORMAT_B5G6R5_UNORM = 85,
	FORMAT_B5G5R5A1_UNORM = 86,
	FORMAT_B8G8R8A8_UNORM = 87,
	FORMAT_B8G8R8X8_UNORM = 88,
	FORMAT_R10G10B10_XR_BIAS_A2_UNORM = 89,
	FORMAT_B8G8R8A8_TYPELESS = 90,
	FORMAT_B8G8R8A8_UNORM_SRGB = 91,
	FORMAT_B8G8R8X8_TYPELESS = 92,
	FORMAT_B8G8R8X8_UNORM_SRGB = 93,
	FORMAT_BC6H_TYPELESS = 94,
	FORMAT_BC6H_UF16 = 95,
	FORMAT_BC6H_SF16 = 96,
	FORMAT_BC7_TYPELESS = 97,
	FORMAT_BC7_UNORM = 98,
	FORMAT_BC7_UNORM_SRGB = 99,
	FORMAT_FORCE_UINT = 0xffffffff
};

enum USAGE
{
	USAGE_DEFAULT = 0,
	USAGE_IMMUTABLE = 1,
	USAGE_DYNAMIC = 2,
	USAGE_STAGING = 3
};


enum BIND_FLAG
{
	BIND_VERTEX_BUFFER = 0x1L,
	BIND_INDEX_BUFFER = 0x2L,
	BIND_CONSTANT_BUFFER = 0x4L,
	BIND_SHADER_RESOURCE = 0x8L,
	BIND_STREAM_OUTPUT = 0x10L,
	BIND_RENDER_TARGET = 0x20L,
	BIND_DEPTH_STENCIL = 0x40L,
	BIND_UNORDERED_ACCESS = 0x80L
};

enum DSV_DIMENSION
{
	DSV_DIMENSION_UNKNOWN = 0,
	DSV_DIMENSION_TEXTURE1D = 1,
	DSV_DIMENSION_TEXTURE1DARRAY = 2,
	DSV_DIMENSION_TEXTURE2D = 3,
	DSV_DIMENSION_TEXTURE2DARRAY = 4,
	DSV_DIMENSION_TEXTURE2DMS = 5,
	DSV_DIMENSION_TEXTURE2DMSARRAY = 6
};

enum SRV_DIMENSION
{
	SRV_DIMENSION_UNKNOWN = 0,
	DIMENSION_BUFFER = 1,
	SRV_DIMENSION_TEXTURE1D = 2,
	SRV_DIMENSION_TEXTURE1DARRAY = 3,
	SRV_DIMENSION_TEXTURE2D = 4,
	SRV_DIMENSION_TEXTURE2DARRAY = 5,
	SRV_DIMENSION_TEXTURE2DMS = 6,
	SRV_DIMENSION_TEXTURE2DMSARRAY = 7,
	SRV_DIMENSION_TEXTURE3D = 8,
	SRV_DIMENSION_TEXTURECUBE = 9,
	SRV_DIMENSION_TEXTURECUBEARRAY = 10,
	SRV_DIMENSION_BUFFEREX = 11
};

enum RTV_DIMENSION
{
	RTV_DIMENSION_UNKNOWN = 0,
	RTV_DIMENSION_BUFFER = 1,
	RTV_DIMENSION_TEXTURE1D = 2,
	RTV_DIMENSION_TEXTURE1DARRAY = 3,
	RTV_DIMENSION_TEXTURE2D = 4,
	RTV_DIMENSION_TEXTURE2DARRAY = 5,
	RTV_DIMENSION_TEXTURE2DMS = 6,
	RTV_DIMENSION_TEXTURE2DMSARRAY = 7,
	RTV_DIMENSION_TEXTURE3D = 8
};

enum INPUT_CLASSIFICATION
{
	INPUT_PER_VERTEX_DATA = 0,
	INPUT_PER_INSTANCE_DATA = 1
};

enum FILTER
{
	FILTER_MIN_MAG_MIP_POINT = 0,
	FILTER_MIN_MAG_POINT_MIP_LINEAR = 0x1,
	FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT = 0x4,
	FILTER_MIN_POINT_MAG_MIP_LINEAR = 0x5,
	FILTER_MIN_LINEAR_MAG_MIP_POINT = 0x10,
	FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR = 0x11,
	FILTER_MIN_MAG_LINEAR_MIP_POINT = 0x14,
	FILTER_MIN_MAG_MIP_LINEAR = 0x15,
	FILTER_ANISOTROPIC = 0x55,
	FILTER_COMPARISON_MIN_MAG_MIP_POINT = 0x80,
	FILTER_COMPARISON_MIN_MAG_POINT_MIP_LINEAR = 0x81,
	FILTER_COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT = 0x84,
	FILTER_COMPARISON_MIN_POINT_MAG_MIP_LINEAR = 0x85,
	FILTER_COMPARISON_MIN_LINEAR_MAG_MIP_POINT = 0x90,
	FILTER_COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR = 0x91,
	FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT = 0x94,
	FILTER_COMPARISON_MIN_MAG_MIP_LINEAR = 0x95,
	FILTER_COMPARISON_ANISOTROPIC = 0xd5
};

enum TEXTURE_ADDRESS_MODE
{
	TEXTURE_ADDRESS_WRAP = 1,
	TEXTURE_ADDRESS_MIRROR = 2,
	TEXTURE_ADDRESS_CLAMP = 3,
	TEXTURE_ADDRESS_BORDER = 4,
	TEXTURE_ADDRESS_MIRROR_ONCE = 5
};

enum COMPARISON_FUNC
{
	COMPARISON_NEVER = 1,
	COMPARISON_LESS = 2,
	COMPARISON_EQUAL = 3,
	COMPARISON_LESS_EQUAL = 4,
	COMPARISON_GREATER = 5,
	COMPARISON_NOT_EQUAL = 6,
	COMPARISON_GREATER_EQUAL = 7,
	COMPARISON_ALWAYS = 8
};

enum CLEAR_FLAG
{
	CLEAR_DEPTH = 0x1L,
	CLEAR_STENCIL = 0x2L
};

enum FILL_MODE
{
	FILL_WIREFRAME = 2,
	FILL_SOLID = 3
};

enum CULL_MODE
{
	CULL_NONE = 1,
	CULL_FRONT = 2,
	CULL_BACK = 3
};

enum PRIMITIVE_TOPOLOGY
{
	PRIMITIVE_TOPOLOGY_UNDEFINED = 0,
	PRIMITIVE_TOPOLOGY_POINTLIST = 1,
	PRIMITIVE_TOPOLOGY_LINELIST = 2,
	PRIMITIVE_TOPOLOGY_LINESTRIP = 3,
	PRIMITIVE_TOPOLOGY_TRIANGLELIST = 4,
	PRIMITIVE_TOPOLOGY_TRIANGLESTRIP = 5,
	PRIMITIVE_TOPOLOGY_LINELIST_ADJ = 10,
	PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ = 11,
	PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ = 12,
	PRIMITIVE_TOPOLOGY_TRIANGLESTRIP_ADJ = 13,
	PRIMITIVE_TOPOLOGY_1_CONTROL_POINT_PATCHLIST = 33,
	PRIMITIVE_TOPOLOGY_2_CONTROL_POINT_PATCHLIST = 34,
	PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST = 35,
	PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST = 36,
	PRIMITIVE_TOPOLOGY_5_CONTROL_POINT_PATCHLIST = 37,
	PRIMITIVE_TOPOLOGY_6_CONTROL_POINT_PATCHLIST = 38,
	PRIMITIVE_TOPOLOGY_7_CONTROL_POINT_PATCHLIST = 39,
	PRIMITIVE_TOPOLOGY_8_CONTROL_POINT_PATCHLIST = 40,
	PRIMITIVE_TOPOLOGY_9_CONTROL_POINT_PATCHLIST = 41,
	PRIMITIVE_TOPOLOGY_10_CONTROL_POINT_PATCHLIST = 42,
	PRIMITIVE_TOPOLOGY_11_CONTROL_POINT_PATCHLIST = 43,
	PRIMITIVE_TOPOLOGY_12_CONTROL_POINT_PATCHLIST = 44,
	PRIMITIVE_TOPOLOGY_13_CONTROL_POINT_PATCHLIST = 45,
	PRIMITIVE_TOPOLOGY_14_CONTROL_POINT_PATCHLIST = 46,
	PRIMITIVE_TOPOLOGY_15_CONTROL_POINT_PATCHLIST = 47,
	PRIMITIVE_TOPOLOGY_16_CONTROL_POINT_PATCHLIST = 48,
	PRIMITIVE_TOPOLOGY_17_CONTROL_POINT_PATCHLIST = 49,
	PRIMITIVE_TOPOLOGY_18_CONTROL_POINT_PATCHLIST = 50,
	PRIMITIVE_TOPOLOGY_19_CONTROL_POINT_PATCHLIST = 51,
	PRIMITIVE_TOPOLOGY_20_CONTROL_POINT_PATCHLIST = 52,
	PRIMITIVE_TOPOLOGY_21_CONTROL_POINT_PATCHLIST = 53,
	PRIMITIVE_TOPOLOGY_22_CONTROL_POINT_PATCHLIST = 54,
	PRIMITIVE_TOPOLOGY_23_CONTROL_POINT_PATCHLIST = 55,
	PRIMITIVE_TOPOLOGY_24_CONTROL_POINT_PATCHLIST = 56,
	PRIMITIVE_TOPOLOGY_25_CONTROL_POINT_PATCHLIST = 57,
	PRIMITIVE_TOPOLOGY_26_CONTROL_POINT_PATCHLIST = 58,
	PRIMITIVE_TOPOLOGY_27_CONTROL_POINT_PATCHLIST = 59,
	PRIMITIVE_TOPOLOGY_28_CONTROL_POINT_PATCHLIST = 60,
	PRIMITIVE_TOPOLOGY_29_CONTROL_POINT_PATCHLIST = 61,
	PRIMITIVE_TOPOLOGY_30_CONTROL_POINT_PATCHLIST = 62,
	PRIMITIVE_TOPOLOGY_31_CONTROL_POINT_PATCHLIST = 63,
	PRIMITIVE_TOPOLOGY_32_CONTROL_POINT_PATCHLIST = 64,
};

enum DRIVER_TYPE
{
	DRIVER_TYPE_UNKNOWN =	0,
	DRIVER_TYPE_HARDWARE =	(DRIVER_TYPE_UNKNOWN + 1),
	DRIVER_TYPE_REFERENCE = (DRIVER_TYPE_HARDWARE + 1),
	DRIVER_TYPE_NULL =		(DRIVER_TYPE_REFERENCE + 1),
	DRIVER_TYPE_SOFTWARE =	(DRIVER_TYPE_NULL + 1),
	DRIVER_TYPE_WARP =		(DRIVER_TYPE_SOFTWARE + 1)
};

enum FEATURE_LEVEL
{
	FEATURE_LEVEL_9_1 = 0x9100,
	FEATURE_LEVEL_9_2 = 0x9200,
	FEATURE_LEVEL_9_3 = 0x9300,
	FEATURE_LEVEL_10_0 = 0xa000,
	FEATURE_LEVEL_10_1 = 0xa100,
	FEATURE_LEVEL_11_0 = 0xb000
};

enum MODE_SCANLINE_ORDER
{
	MODE_SCANLINE_ORDER_UNSPECIFIED = 0,
	MODE_SCANLINE_ORDER_PROGRESSIVE = 1,
	MODE_SCANLINE_ORDER_UPPER_FIELD_FIRST = 2,
	MODE_SCANLINE_ORDER_LOWER_FIELD_FIRST = 3
};

enum MODE_SCALING
{
	MODE_SCALING_UNSPECIFIED = 0,
	MODE_SCALING_CENTERED = 1,
	MODE_SCALING_STRETCHED = 2
};

enum SWAP_EFFECT
{
	SWAP_EFFECT_DISCARD = 0,
	SWAP_EFFECT_SEQUENTIAL = 1
};

enum CREATE_DEVICE_FLAG
{
	CREATE_DEVICE_SINGLETHREADED = 0x1,
	CREATE_DEVICE_DEBUG = 0x2,
	CREATE_DEVICE_SWITCH_TO_REF = 0x4,
	CREATE_DEVICE_PREVENT_INTERNAL_THREADING_OPTIMIZATIONS = 0x8,
	CREATE_DEVICE_BGRA_SUPPORT = 0x20
};

struct Vector4
{
	float x;
	float y;
	float z;
	float w;
};

struct Vector3 {
	float x;
	float y;
	float z;
};

struct Vector2 {
	float x;
	float y;
};

struct Vertex
{
	Vector3 Pos;
	Vector2 Tex;
	Vector3 Normales;
};

struct MATRIX
{
	union
	{
		Vector4 r[4];
		struct
		{
			float _11, _12, _13, _14;
			float _21, _22, _23, _24;
			float _31, _32, _33, _34;
			float _41, _42, _43, _44;
		};
		float m[4][4];
	};

	MATRIX()
	{
		_11 = 1; _12 = 0; _13 = 0; _14 = 0;
		_21 = 0; _22 = 1; _23 = 0; _24 = 0;
		_31 = 0; _32 = 0; _33 = 1; _34 = 0;
		_41 = 0; _42 = 0; _43 = 0; _44 = 1;
	};
	MATRIX(Vector4 R0, Vector4 R1, Vector4 R2, Vector4 R3) {
		r[0] = R0; r[1] = R1; r[2] = R2; r[3] = R3;
	}
	MATRIX(float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23,
		float m30, float m31, float m32, float m33)
	{
		_11 = m00; _12 = m01; _13 = m02; _14 = m03;
		_21 = m10; _22 = m11; _23 = m12; _24 = m13;
		_31 = m20; _32 = m21; _33 = m22; _34 = m23;
		_41 = m30; _42 = m31; _43 = m32; _44 = m33;
	}
	MATRIX(const float* pArray)
	{
		memset(m, 0, 4 * 4 * sizeof(float));
		memcpy(m, pArray, 4 * 4 * sizeof(float));
	}

	float  operator() (UINT Row, UINT Column) const { return m[Row][Column]; }
	float& operator() (UINT Row, UINT Column) { return m[Row][Column]; }

	Vector3 operator* (Vector4 vec)
	{
		Vector3 vecR;
		vecR.x = _11 * vec.x + _12 * vec.y + _13 * vec.z + _14 * vec.w;
		vecR.y = _21 * vec.x + _22 * vec.y + _23 * vec.z + _24 * vec.w;
		vecR.z = _31 * vec.x + _32 * vec.y + _33 * vec.z + _34 * vec.w;
		return vecR;
	}

	MATRIX TransposeMatrix()
	{
		MATRIX r;
		r._11 = _11; r._21 = _12; r._31 = _13; r._41 = _14;
		r._12 = _21; r._22 = _22; r._32 = _23; r._42 = _24;
		r._13 = _31; r._23 = _32; r._33 = _33; r._43 = _34;
		r._14 = _41; r._24 = _42; r._34 = _43; r._44 = _44;
		return r;
	}

};

struct Color
{
	float r;
	float g;
	float b;
	float a;
};

struct Material
{
	float specular;
	float shininess;
	float unused1;
	float unused2;
};

struct ViewPosition
{
	Vector4 pos;
};

struct DirectionalLight
{
	Vector4 lightDir;

	Vector4 ambient;
	Vector4 diffuse;
	Vector4 specular;
};

struct PointLight
{
	Vector4 lightPos;

	Vector4 diffuse;
	Vector4 specular;

	float blurDistance;

	float unused1;
	float unused2;
	float unused3;
};

struct SpotLight
{
	Vector4 lightPos;
	Vector4 lightDir;

	Vector4 diffuse;
	Vector4 specular;

	float cutOff;
	float outerCutOff;

	float blurDistance;
	
	float unused1;
};

struct ModelMat
{
	MATRIX model;
};

struct ViewMat
{
	MATRIX view;
};

struct ProjectionMat
{
	MATRIX projection;
};

struct SAMPLE_DESC
{
	unsigned int Count;
	unsigned int Quality;
};

struct RATIONAL
{
	unsigned int Numerator;
	unsigned int Denominator;
};

struct MODE_DESC
{
	unsigned int Width;
	unsigned int Height;
	RATIONAL RefreshRate;
	FORMAT Format;
	MODE_SCANLINE_ORDER ScanlineOrdering;
	MODE_SCALING Scaling;
};

struct SWAP_CHAIN_DESC
{
	MODE_DESC BufferDesc;
	SAMPLE_DESC SampleDesc;
	unsigned int BufferUsage;
	unsigned int BufferCount;
	HWND OutputWindow;
	long Windowed;
	SWAP_EFFECT SwapEffect;
	unsigned int Flags;
};

struct VIEWPORT
{
	float TopLeftX;
	float TopLeftY;
	float Width;
	float Height;
	float MinDepth;
	float MaxDepth;
};

struct BOX
{
	unsigned int left;
	unsigned int top;
	unsigned int front;
	unsigned int right;
	unsigned int bottom;
	unsigned int back;
};

struct RASTERIZER_DESC
{
	FILL_MODE FillMode;
	CULL_MODE CullMode;
	long FrontCounterClockwise;
	int DepthBias;
	float DepthBiasClamp;
	float SlopeScaledDepthBias;
	long DepthClipEnable;
	long ScissorEnable;
	long MultisampleEnable;
	long AntialiasedLineEnable;
};

struct SAMPLER_DESC
{
	FILTER Filter;
	TEXTURE_ADDRESS_MODE AddressU;
	TEXTURE_ADDRESS_MODE AddressV;
	TEXTURE_ADDRESS_MODE AddressW;
	float MipLODBias;
	unsigned int MaxAnisotropy;
	COMPARISON_FUNC ComparisonFunc;
	float BorderColor[4];
	float MinLOD;
	float MaxLOD;
};

struct INPUT_ELEMENT_DESC
{
	const char* SemanticName;
	unsigned int SemanticIndex;
	FORMAT Format;
	unsigned int InputSlot;
	unsigned int AlignedByteOffset;
	INPUT_CLASSIFICATION InputSlotClass;
	unsigned int InstanceDataStepRate;
};

struct BUFFER_RTV
{
	union
	{
		unsigned int FirstElement;
		unsigned int ElementOffset;
	};
	union
	{
		unsigned int NumElements;
		unsigned int ElementWidth;
	};
};

struct TEX1D_RTV
{
	unsigned int MipSlice;
};

struct TEX1D_ARRAY_RTV
{
	unsigned int MipSlice;
	unsigned int FirstArraySlice;
	unsigned int ArraySize;
};

struct TEX2D_RTV
{
	unsigned int MipSlice;
};

struct TEX2D_ARRAY_RTV
{
	unsigned int MipSlice;
	unsigned int FirstArraySlice;
	unsigned int ArraySize;
};

struct TEX2DMS_RTV
{
	unsigned int UnusedField_NothingToDefine;
};

struct TEX2DMS_ARRAY_RTV
{
	unsigned int FirstArraySlice;
	unsigned int ArraySize;
};

struct TEX3D_RTV
{
	unsigned int MipSlice;
	unsigned int FirstWSlice;
	unsigned int WSize;
};

struct RENDER_TARGET_VIEW_DESC
{
	FORMAT Format;
	RTV_DIMENSION ViewDimension;
	union
	{
		BUFFER_RTV Buffer;
		TEX1D_RTV Texture1D;
		TEX1D_ARRAY_RTV Texture1DArray;
		TEX2D_RTV Texture2D;
		TEX2D_ARRAY_RTV Texture2DArray;
		TEX2DMS_RTV Texture2DMS;
		TEX2DMS_ARRAY_RTV Texture2DMSArray;
		TEX3D_RTV Texture3D;
	};
};

struct TEX1D_DSV
{
	unsigned int MipSlice;
};

struct TEX1D_ARRAY_DSV
{
	unsigned int MipSlice;
	unsigned int FirstArraySlice;
	unsigned int ArraySize;
};

struct TEX2D_DSV
{
	unsigned int MipSlice;
};

struct TEX2D_ARRAY_DSV
{
	unsigned int MipSlice;
	unsigned int FirstArraySlice;
	unsigned int ArraySize;
};

struct TEX2DMS_DSV
{
	unsigned int UnusedField_NothingToDefine;
};

struct TEX2DMS_ARRAY_DSV
{
	unsigned int FirstArraySlice;
	unsigned int ArraySize;
};

struct DEPTH_STENCIL_VIEW_DESC
{
	FORMAT Format;
	DSV_DIMENSION ViewDimension;
	unsigned int Flags;
	union
	{
		TEX1D_DSV Texture1D;
		TEX1D_ARRAY_DSV Texture1DArray;
		TEX2D_DSV Texture2D;
		TEX2D_ARRAY_DSV Texture2DArray;
		TEX2DMS_DSV Texture2DMS;
		TEX2DMS_ARRAY_DSV Texture2DMSArray;
	};
};

struct BUFFER_SRV
{
	union
	{
		unsigned int FirstElement;
		unsigned int ElementOffset;
	};
	union
	{
		unsigned int NumElements;
		unsigned int ElementWidth;
	};
};

struct TEX1D_SRV
{
	unsigned int MostDetailedMip;
	unsigned int MipLevels;
};

struct TEX1D_ARRAY_SRV
{
	unsigned int MostDetailedMip;
	unsigned int MipLevels;
	unsigned int FirstArraySlice;
	unsigned int ArraySize;
};

struct TEX2D_SRV
{
	unsigned int MostDetailedMip;
	unsigned int MipLevels;
};

struct TEX2D_ARRAY_SRV
{
	unsigned int MostDetailedMip;
	unsigned int MipLevels;
	unsigned int FirstArraySlice;
	unsigned int ArraySize;
};

struct TEX2DMS_SRV
{
	unsigned int UnusedField_NothingToDefine;
};

struct TEX2DMS_ARRAY_SRV
{
	unsigned int FirstArraySlice;
	unsigned int ArraySize;
};

struct TEX3D_SRV
{
	unsigned int MostDetailedMip;
	unsigned int MipLevels;
};

struct TEXCUBE_SRV
{
	unsigned int MostDetailedMip;
	unsigned int MipLevels;
};

struct TEXCUBE_ARRAY_SRV
{
	unsigned int MostDetailedMip;
	unsigned int MipLevels;
	unsigned int First2DArrayFace;
	unsigned int NumCubes;
};

struct BUFFEREX_SRV
{
	unsigned int FirstElement;
	unsigned int NumElements;
	unsigned int Flags;
};

struct SHADER_RESOURCE_VIEW_DESC
{
	FORMAT Format;
	SRV_DIMENSION ViewDimension;
	union
	{
		BUFFER_SRV Buffer;
		TEX1D_SRV Texture1D;
		TEX1D_ARRAY_SRV Texture1DArray;
		TEX2D_SRV Texture2D;
		TEX2D_ARRAY_SRV Texture2DArray;
		TEX2DMS_SRV Texture2DMS;
		TEX2DMS_ARRAY_SRV Texture2DMSArray;
		TEX3D_SRV Texture3D;
		TEXCUBE_SRV TextureCube;
		TEXCUBE_ARRAY_SRV TextureCubeArray;
		BUFFEREX_SRV BufferEx;
	};
};

struct TEXTURE2D_DESC
{
	unsigned int Width;
	unsigned int Height;
	unsigned int MipLevels;
	unsigned int ArraySize;
	FORMAT Format;
	SAMPLE_DESC SampleDesc;
	USAGE Usage;
	unsigned int BindFlags;
	unsigned int CPUAccessFlags;
	unsigned int MiscFlags;
};

struct BUFFER_DESC
{
	unsigned int ByteWidth;
	USAGE Usage;
	unsigned int BindFlags;
	unsigned int CPUAccessFlags;
	unsigned int MiscFlags;
	unsigned int StructureByteStride;
};

struct SUBRESOURCE_DATA
{
	const void* pSysMem;
	unsigned int SysMemPitch;
	unsigned int SysMemSlicePitch;
};