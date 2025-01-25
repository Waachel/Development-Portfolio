//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef GLTF_H
#define GLTF_H

#include "tiny_gltf.h"
#include "meshData.h"

using namespace tinygltf;

struct GLB_header
{
	uint32_t magic;
	uint32_t version;
	uint32_t length;
};

struct CHUNK_header
{
	uint32_t chunkLength;
	uint32_t chunkType;
	unsigned char chunkData[];
};

class GLTF
{
public:
	static bool LoadBinary(Model &model, const char *const pFileName);
	static bool LoadASCII(Model &model, const char *const pFileName);

	static bool GetGLBRawBuffer(char *&pBuff, unsigned int &BuffSize, const char *const pFileName);
	static bool GetGLBHeader(GLB_header &header, const char *const pBuff, unsigned int BuffSize);
	static bool GetRawJSON(char *&pJSON, unsigned int &JsonSize, const char *const pBuff, unsigned int BuffSize);
	static bool GetBinaryBuffPtr(char *&pBinaryBuff, unsigned int &BinaryBuffSize, const char *const pBuff, unsigned int BuffSize);

	static bool SetVBO(Model &gltfModel, const char *pKey, vboData &vbo, char *pBinaryBuff, int meshIndex);
	static bool SetVBO_index(Model &gltfModel, const char *pKey, vboData &vbo, char *pBinaryBuff, int meshIndex);
	static bool SetTexture(Model &gltfModel, const char *pKey, textureData &text, char *pBinaryBuff);

	static bool GetAzulProtoType(const char *&pProtoFileType, meshData &_meshData);


	static bool SetCustomVBO(vboData &vbo,
							 void *data,
							 unsigned int sizeInBytes,
							 unsigned int count,
							 vboData::VBO_COMPONENT componentType,
							 vboData::VBO_TYPE type,
							 vboData::VBO_TARGET target);

	static bool SetCustomVBO_index(vboData &vbo,
								   void *data,
								   unsigned int sizeInBytes,
								   unsigned int count,
								   vboData::VBO_COMPONENT componentType,
								   vboData::VBO_TYPE type,
								   vboData::VBO_TARGET target);
};

#endif

// --- End of File ---
