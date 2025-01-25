//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "PyramidMesh.h"
#include "meshData.h"
#include "GLTF.h"
#include "File.h"
#include "Mesh.h"

using namespace tinygltf;

namespace Azul
{
	VertexPos g_PyramidVertices_pos[] =
	{
		Vec3f(0,  0, 2),  //0
		Vec3f(-1, -1, 0),  //1
		Vec3f(1, -1, 0),  //2
		Vec3f(1,  1, 0),  //3
		Vec3f(-1,  1, 0)   //4
	};

	VertexColor g_PyramidVertices_color[] =
	{
		 Vec4f(0.0f, 0.0f, 1.0f, 1.0f) , // 0
		 Vec4f(0.0f, 1.0f, 0.0f, 1.0f) , // 1
		 Vec4f(1.0f, 1.0f, 0.0f, 1.0f) , // 2
		 Vec4f(1.0f, 0.0f, 0.0f, 1.0f) , // 3
		 Vec4f(1.0f, 0.0f, 1.0f, 1.0f)   // 4
	};


	uint32_t g_PyramidIndices[] =
	{
		1, 0, 4,
		2, 0, 1,
		3, 0, 2,
		4, 0, 3,
		4, 2, 1,
		4, 3, 2
	};

	VertexTexCoord g_PyramidVertices_texCoord[] =
	{
		Vec2f(0.5f, 0.5f),
		Vec2f(0.0f, 1.0f),
		Vec2f(1.0f, 1.0f),
		Vec2f(1.0f, 0.0f),
		Vec2f(0.0f, 0.0f)
	};

	VertexNorm g_PyramidVertices_norm[] =
	{
		Vec3f(0.0f,  0.0f,  1.0f),
		Vec3f(-0.4f, -0.4f, -0.8f),
		Vec3f(0.4f, -0.4f, -0.8f),
		Vec3f(0.4f,  0.4f, -0.8f),
		Vec3f(-0.4f,  0.4f, -0.8f)

	};


	void CreatePyramidMesh()
	{
		// runtime model
		meshData  runModel;

		// Name
		std::string BaseName = "PyramidMesh";

		Trace::out("%12s ", BaseName.c_str());

		unsigned int numTris = (sizeof(g_PyramidIndices) / sizeof(g_PyramidIndices[0])) / 3;
		unsigned int numVerts = sizeof(g_PyramidIndices) / sizeof(g_PyramidIndices[0]);



		//---------------------------------
		// Model Name
		//---------------------------------
		const char *pMeshName = "PyramidMesh";
		memcpy_s(runModel.pMeshName, meshData::FILE_NAME_SIZE, pMeshName, strlen(pMeshName));

		//---------------------------------
		// Set vbo 
		//---------------------------------
		GLTF::SetCustomVBO(runModel.vbo_vert,
						   g_PyramidVertices_pos,
						   sizeof(g_PyramidVertices_pos),
						   numVerts,
						   vboData::VBO_COMPONENT::FLOAT,
						   vboData::VBO_TYPE::VEC3,
						   vboData::VBO_TARGET::ARRAY_BUFFER
		);
		//runModel.vbo_vert.Print("vbo_vert");

		GLTF::SetCustomVBO(runModel.vbo_norm,
						   g_PyramidVertices_norm,
						   sizeof(g_PyramidVertices_norm),
						   numVerts,
						   vboData::VBO_COMPONENT::FLOAT,
						   vboData::VBO_TYPE::VEC3,
						   vboData::VBO_TARGET::ARRAY_BUFFER
		);
		//runModel.vbo_norm.Print("vbo_norm");

		GLTF::SetCustomVBO(runModel.vbo_uv,
						   g_PyramidVertices_texCoord,
						   sizeof(g_PyramidVertices_texCoord),
						   numVerts,
						   vboData::VBO_COMPONENT::FLOAT,
						   vboData::VBO_TYPE::VEC2,
						   vboData::VBO_TARGET::ARRAY_BUFFER
		);
		//runModel.vbo_uv.Print("vbo_uv");

		GLTF::SetCustomVBO_index(runModel.vbo_index,
								 g_PyramidIndices,
								 sizeof(g_PyramidIndices),
								 sizeof(g_PyramidIndices) / sizeof(g_PyramidIndices[0]),
								 vboData::VBO_COMPONENT::UNSIGNED_INT,
								 vboData::VBO_TYPE::SCALAR,
								 vboData::VBO_TARGET::ELEMENT_ARRAY_BUFFER
		);
		//runModel.vbo_index.Print("vbo_index");

		//--------------------------------
		// PolyCount
		//--------------------------------
		runModel.triCount = numTris;
		runModel.vertCount = numVerts;

		meshData_proto mA_proto;
		runModel.Serialize(mA_proto);

		// -------------------------------
		//  Write to file
		//--------------------------------

		File::Handle fh;
		File::Error err;
		bool status;

		// Create output name
		const char *pProtoFileType = nullptr;
		status = GLTF::GetAzulProtoType(pProtoFileType, runModel);
		assert(status);

		std::string OutputFileName = BaseName + pProtoFileType + ".proto.azul";
		Trace::out(" --> %22s\n", OutputFileName.c_str());

		err = File::Open(fh, OutputFileName.c_str(), File::Mode::WRITE);
		assert(err == File::Error::SUCCESS);

		std::string strOut;
		mA_proto.SerializeToString(&strOut);

		File::Write(fh, strOut.data(), strOut.length());
		assert(err == File::Error::SUCCESS);

		err = File::Close(fh);
		assert(err == File::Error::SUCCESS);

		// -------------------------------
		// Read and recreate model data
		// -------------------------------

		err = File::Open(fh, OutputFileName.c_str(), File::Mode::READ);
		assert(err == File::Error::SUCCESS);

		err = File::Seek(fh, File::Position::END, 0);
		assert(err == File::Error::SUCCESS);

		DWORD FileLength;
		err = File::Tell(fh, FileLength);
		assert(err == File::Error::SUCCESS);

		char *poNewBuff = new char[FileLength]();
		assert(poNewBuff);

		err = File::Seek(fh, File::Position::BEGIN, 0);
		assert(err == File::Error::SUCCESS);

		err = File::Read(fh, poNewBuff, FileLength);
		assert(err == File::Error::SUCCESS);

		err = File::Close(fh);
		assert(err == File::Error::SUCCESS);

		std::string strIn(poNewBuff, FileLength);
		meshData_proto mB_proto;

		mB_proto.ParseFromString(strIn);

		meshData mB;
		mB.Deserialize(mB_proto);

		delete[] poNewBuff;

	}


}

// --- End of File ---
