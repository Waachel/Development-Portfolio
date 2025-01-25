//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "GLTF.h"
#include "ConvertModel.h"
#include "CreateTexture.h"
#include "CubeMesh.h"
#include "SphereMesh.h"
#include "PyramidMesh.h"
#include "ConvertAnim.h"
#include "ChickenBot.h"

using namespace Azul;

int main()
{
	// Verify that the version of the library that we linked against is
	// compatible with the version of the headers we compiled against.
	GOOGLE_PROTOBUF_VERIFY_VERSION;
	
	//Animation 1: Walk
	//ConvertAnim("walk_mesh.glb");
	// 
	//Animation 2: Die Left
	ConvertAnim("die_left+mesh.glb");
	// 
	//Animation 3: Side Step Right
	//ConvertAnim("sidestep_right+mesh.glb");
	// 
	//Animation 4:Run
	//ConvertAnim("run+mesh.glb");
	
	
	//ConvertModel("crate.glb");

	CreateCubeMesh();
	CreateSphereMesh();	
	CreatePyramidMesh();

	CreateTextureTGA("Duckweed.tga");
	CreateTextureTGA("RedBrick.tga");
	CreateTextureTGA("Rocks.tga");
	CreateTextureTGA("Stone.tga");
	CreateTexturePNG("ChickenbotDiffuse.png");  //<--- use this for the model
	CreateChickenBotModels();


	// clean up data directory
	system("del *.png");
	system("del *.tga");
	system("del *.glb");

	google::protobuf::ShutdownProtobufLibrary();

	return 0;
}

// ---  End of File ---------------
