//--------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#include <d3d11.h>
#include <d3dcompiler.h>
#include <WinUser.h>
#include "Game.h"
#include "Engine.h"
#include "MathEngine.h"
#include "Camera.h"
#include "Colors.h"
#include "GameObject.h"
#include "ShaderObject.h"
#include "ShaderObject_ColorByVertex.h"
#include "ShaderObject_FlatTexture.h"
#include "ShaderObject_LightTexture.h"
#include "ShaderObject_ConstColor.h"
#include "Mesh.h"
#include "MeshProto.h"
#include "GraphicsObject_ColorByVertex.h"
#include "GraphicsObject_Wireframe.h"
#include "GraphicsObject_FlatTexture.h"
#include "GraphicsObject_LightTexture.h"
#include "GraphicsObject_ConstColor.h"
#include "GameObjectMan.h"
#include "MeshNodeMan.h"
#include "ShaderObjectNodeMan.h"
#include "TextureObject.h"
#include "TextureProto.h"
#include "TexNodeMan.h"
#include "DirectXDeviceMan.h"
#include "CameraMan.h"
#include "CameraDemoInput.h"
#include "CameraMesh.h"
#include "AnimTimer.h"
#include "MathApp.h"
#include "AnimWalk.h"
#include "AnimDemoInput.h"
#include "GameObjectBasic.h"
#include "TimerController.h"
#include "AnimController.h"
#include "Clip.h"

using namespace Azul;

namespace Azul
{
	// Todo: clean up
	AnimController *pAnimControllerA;
	Clip *pWalkClip = nullptr;

	AnimController* pAnimControllerB;
	Clip* pRunClip = nullptr;

	AnimController* pAnimControllerC;
	Clip* pDieLeftClip = nullptr;

	AnimController* pAnimControllerD;
	Clip* pSideStepRightClip = nullptr;

	AnimController* pAnimControllerMain;
	Clip* pMainClip = nullptr;

	//-----------------------------------------------------------------------------
	//  Game::Game()
	//		Game Engine Constructor
	//-----------------------------------------------------------------------------

	Game::Game(const char *const pName, int width, int height)
		: Engine(pName, width, height)
	{
		this->globalTimer.Tic();
		this->intervalTimer.Tic();
	}

	//-----------------------------------------------------------------------------
	//  Game::~Game()
	//		Game Engine destructor
	//-----------------------------------------------------------------------------
	Game::~Game()
	{
	}

	//-----------------------------------------------------------------------------
	// Game::LoadContent()
	//		Allows you to load all content needed for your engine,
	//	    such as objects, graphics, etc.
	//-----------------------------------------------------------------------------
	bool Game::LoadContent()
	{
		ShaderObjectNodeMan::Create();
		MeshNodeMan::Create();
		GameObjectMan::Create();
		TexNodeMan::Create();
		CameraMan::Create();

		// --------------------------
		// Camera setup
		// --------------------------

		// Camera_0		
		Camera *pCam0 = new Camera();

		pCam0->setViewport(0, 0, this->WindowWidth, this->WindowHeight);
		pCam0->setPerspective(35.0f, float(pCam0->getScreenWidth()) / float(pCam0->getScreenHeight()), 1.0f, 10000.0f);



		pCam0->setOrientAndPosition(Vec3(1.0f, 0.0f, 0.0f),
									Vec3(-1.0f, 0.0f, 0.0f),
									Vec3(-1.0f, 12.0f, 0.0f));


		pCam0->updateCamera();
		CameraMan::Add(Camera::Name::CAMERA_0, pCam0);
		CameraMan::SetCurrent(Camera::Name::CAMERA_0);

		// ------------------------------------------
		//   Model + Shaders --> GraphicsObject
		// ------------------------------------------


		// -----------------------------------
		// Load the Mesh from Data file
		// -----------------------------------

		Mesh *pPyramidMesh = new MeshProto("PyramidMesh.m.proto.azul");
		MeshNodeMan::Add(Mesh::Name::PYRAMID, pPyramidMesh);

		Mesh *pCubeMesh = new MeshProto("CubeMesh.m.proto.azul");
		MeshNodeMan::Add(Mesh::Name::CUBE, pCubeMesh);

		TextureObject *pTexChickenBot = new TextureProto("ChickenbotDiffuse.t.proto.azul");
		TexNodeMan::Add(TextureObject::Name::ChickenBot, pTexChickenBot);


		Mesh *pBone_Mesh = new MeshProto("Bone.m.proto.azul");
		Mesh *pBone_001_Mesh = new MeshProto("Bone_001.m.proto.azul");
		Mesh *pBone_L_Mesh = new MeshProto("Bone_L.m.proto.azul");
		Mesh *pBone_L_001_Mesh = new MeshProto("Bone_L_001.m.proto.azul");
		Mesh *pBone_L_002_Mesh = new MeshProto("Bone_L_002.m.proto.azul");
		Mesh *pBone_R_Mesh = new MeshProto("Bone_R.m.proto.azul");
		Mesh *pBone_R_001_Mesh = new MeshProto("Bone_R_001.m.proto.azul");
		Mesh *pBone_R_002_Mesh = new MeshProto("Bone_R_002.m.proto.azul");

		MeshNodeMan::Add(Mesh::Name::Bone, pBone_Mesh);
		MeshNodeMan::Add(Mesh::Name::Bone_001, pBone_001_Mesh);
		MeshNodeMan::Add(Mesh::Name::Bone_L, pBone_L_Mesh);
		MeshNodeMan::Add(Mesh::Name::Bone_L_001, pBone_L_001_Mesh);
		MeshNodeMan::Add(Mesh::Name::Bone_L_002, pBone_L_002_Mesh);
		MeshNodeMan::Add(Mesh::Name::Bone_R, pBone_R_Mesh);
		MeshNodeMan::Add(Mesh::Name::Bone_R_001, pBone_R_001_Mesh);
		MeshNodeMan::Add(Mesh::Name::Bone_R_002, pBone_R_002_Mesh);


		// --------------------------------
		//  ShaderObject  ColorByVertex
		// --------------------------------
		ShaderObject *poShaderA = new ShaderObject_ColorByVertex(ShaderObject::Name::ColorByVertex);
		ShaderObject *poShaderB = new ShaderObject_FlatTexture(ShaderObject::Name::FlatTexture);
		ShaderObject *poShaderC = new ShaderObject_LightTexture(ShaderObject::Name::LightTexture);
		ShaderObject *poShaderD = new ShaderObject_ConstColor(ShaderObject::Name::ConstColor);

		ShaderObjectNodeMan::Add(poShaderA);
		ShaderObjectNodeMan::Add(poShaderB);
		ShaderObjectNodeMan::Add(poShaderC);
		ShaderObjectNodeMan::Add(poShaderD);

		// -----------------------------------
		//  Texture
		// -----------------------------------

		TextureObject *pTexDuck = new TextureProto("Duckweed.t.proto.azul");
		TextureObject *pTexBrick = new TextureProto("RedBrick.t.proto.azul");
		TextureObject *pTexStone = new TextureProto("Stone.t.proto.azul");
		TextureObject *pTexRocks = new TextureProto("Rocks.t.proto.azul");

		TexNodeMan::Add(TextureObject::Name::Duckweed, pTexDuck);
		TexNodeMan::Add(TextureObject::Name::Brick, pTexBrick);
		TexNodeMan::Add(TextureObject::Name::Stone, pTexStone);
		TexNodeMan::Add(TextureObject::Name::Rocks, pTexRocks);

		// ---------------------------------
		// Create Animation
		// ---------------------------------

		// Clip... (eventually part of the AnimMan)
		pWalkClip = new Clip(NUM_BONES, 1);
		assert(pWalkClip);

		// Anim controller
		AnimTime delta = 0.2f * AnimTime(AnimTime::Duration::FILM_24_FRAME);
		pAnimControllerA = new AnimController(delta, AnimController::Type::WALK);
		assert(pAnimControllerA);

		//Die Left
		pDieLeftClip = new Clip(NUM_BONES, 2);
		assert(pDieLeftClip);

		// Anim controller
		delta = 0.2f * AnimTime(AnimTime::Duration::FILM_24_FRAME);
		pAnimControllerC = new AnimController(delta, AnimController::Type::DIE_LEFT);
		assert(pAnimControllerC);

		//Side Step Right
		pSideStepRightClip = new Clip(NUM_BONES, 3);
		assert(pSideStepRightClip);

		// Anim controller
		delta = 0.2f * AnimTime(AnimTime::Duration::FILM_24_FRAME);
		pAnimControllerD = new AnimController(delta, AnimController::Type::SIDE_STEP_RIGHT);
		assert(pAnimControllerD);

		//Run
		pRunClip = new Clip(NUM_BONES, 4);
		assert(pRunClip);

		// Anim controller
		delta = 0.2f * AnimTime(AnimTime::Duration::FILM_24_FRAME);
		pAnimControllerB = new AnimController(delta, AnimController::Type::RUN);
		assert(pAnimControllerB);

		//Main Bot
		pMainClip = new Clip(NUM_BONES, 4);
		assert(pMainClip);

		// Anim controller
		delta = 0.2f * AnimTime(AnimTime::Duration::FILM_24_FRAME);
		pAnimControllerMain = new AnimController(delta, AnimController::Type::MAIN);
		assert(pAnimControllerMain);

		return true;
	}

	//-----------------------------------------------------------------------------
	// Game::Update()
	//      Called once per frame, update data, tranformations, etc
	//      Use this function to control process order
	//      Input, AI, Physics, Animation, and Graphics
	//-----------------------------------------------------------------------------
	void Game::Update(float)
	{
		this->intervalTimer.Toc();
		this->intervalTimer.Tic();

		// -----------------------
		// Read inputs
		// -----------------------
		CameraDemoInput();
		AnimDemoInput(pMainClip, pAnimControllerMain);
		// ------------------------------------
		// Camera update
		// ------------------------------------
		Camera *pCam = CameraMan::GetCurrent();
		pCam->updateCamera();

		// ------------------------------------
		// Animate Me
		// ------------------------------------
		pAnimControllerA->Update();
		pAnimControllerB->Update();
		pAnimControllerC->Update();
		pAnimControllerD->Update();
		pAnimControllerMain->Update();

		// ------------------------------------
		// Update GameObjects
		// ------------------------------------
		GameObjectMan::Update(this->globalTimer.Toc());

	}


	//-----------------------------------------------------------------------------
	// Game::Render()
	//		This function is called once per frame
	//	    Use this for draw graphics to the screen.
	//      Only do rendering here
	//-----------------------------------------------------------------------------
	void Game::Render()
	{
		this->SetDefaultTargetMode();

		GameObjectMan::Draw();
	}


	//-----------------------------------------------------------------------------
	// Game::UnLoadContent()
	//       unload content (resources loaded above)
	//       unload all content that was loaded before the Engine Loop started
	//-----------------------------------------------------------------------------
	void Game::UnloadContent()
	{
		CameraMan::Destroy();
		ShaderObjectNodeMan::Destroy();
		MeshNodeMan::Destroy();
		GameObjectMan::Destroy();
		TexNodeMan::Destroy();
		DirectXDeviceMan::Destroy();

		delete pAnimControllerA;
		delete pWalkClip;
		delete pAnimControllerB;
		delete pRunClip;
		delete pAnimControllerC;
		delete pDieLeftClip;
		delete pAnimControllerD;
		delete pSideStepRightClip;
		delete pAnimControllerMain;
		delete pMainClip;
	}

	//------------------------------------------------------------------
	// Game::ClearBufferFunc()
	// Allows user to change the way the clear buffer function works
	// 
	// Clear the color and depth buffers.
	//------------------------------------------------------------------
	void Game::ClearDepthStencilBuffer()
	{
#ifdef _DEBUG
		const Vec4 ClearColor = Azul::Colors::LightGray;
#else
		const Vec4 ClearColor = Azul::Colors::Wheat;
#endif
		float clearDepth = 1.0f;
		uint8_t clearStencil = 0;
		g_d3dDeviceContext->ClearRenderTargetView(g_d3dRenderTargetView, (const float *)&ClearColor);
		g_d3dDeviceContext->ClearDepthStencilView(g_d3dDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, clearDepth, clearStencil);
	}

}

// --- End of File ---
