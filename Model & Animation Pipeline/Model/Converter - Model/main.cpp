//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "GLTF.h"
#include "ConvertModel.h"
#include "CreateTexture.h"
#include "CubeMesh.h"
#include "SphereMesh.h"
#include "PyramidMesh.h"

using namespace Azul;

int main()
{
	// Verify that the version of the library that we linked against is
	// compatible with the version of the headers we compiled against.
	GOOGLE_PROTOBUF_VERIFY_VERSION;

	//Group A - Small poly
	ConvertModel("BarramundiFish.glb");

	//Group B - Very large
	ConvertModel("Corset.glb");

	//Group C - Multi Mesh
	ConvertModel("AntiqueCamera.glb");

	//Group D - Student's Choice
	ConvertModel("dog.glb");

	CreateCubeMesh();
	CreateSphereMesh();
	CreatePyramidMesh();

	CreateTextureTGA("Duckweed.tga");
	CreateTextureTGA("RedBrick.tga");
	CreateTextureTGA("Rocks.tga");
	CreateTextureTGA("Stone.tga");

	// clean up data directory
	system("del *.png");
	system("del *.tga");
	system("del *.glb");

	google::protobuf::ShutdownProtobufLibrary();

	return 0;
}

// ---  End of File ---------------
