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

using namespace Azul;

namespace Azul
{

	Game::Game(const char *const pName, int width, int height)
		: Engine(pName, width, height)
	{
		this->globalTimer.Tic();
		this->intervalTimer.Tic();
	}

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
		pCam0->setPerspective(40.0f, float(pCam0->getScreenWidth()) / float(pCam0->getScreenHeight()), 1.0f, 100000.0f);
		pCam0->setOrientAndPosition(Vec3(0.0f, 1.0f, 0.0f),
									Vec3(0.0f, 0.0f, 0.0f),
									Vec3(0.0f, 0.0f, 6.0f));
		pCam0->updateCamera();
		CameraMan::Add(Camera::Name::CAMERA_0, pCam0);
		CameraMan::SetCurrent(Camera::Name::CAMERA_0);

		// ------------------------------------------
		//   Model + Shaders --> GraphicsObject
		// -------------------------------------------

		Mesh *poMeshCube = new MeshProto("CubeMesh.m.proto.azul");
		Mesh *poMeshSphere = new MeshProto("SphereMesh.m.proto.azul");
		Mesh *poMeshPyramid = new MeshProto("PyramidMesh.m.proto.azul");
		//Mesh *pCrateMesh = new MeshProto("crate.mt.proto.azul");
		//BarramundiFish.mt.proto
		Mesh* pFishMesh = new MeshProto("BarramundiFish.mt.proto.azul");
		Mesh* pCorsetMesh = new MeshProto("Corset.mt.proto.azul");
		Mesh* pAntiqueCameraMesh = new MeshProto("AntiqueCamera.mt.proto.azul");
		Mesh* pAntiqueCameraMesh1 = new MeshProto("AntiqueCamera1.mt.proto.azul");

		Mesh* pDogMesh = new MeshProto("Dog.mt.proto.azul");
		Mesh* pDogMesh1 = new MeshProto("Dog1.mt.proto.azul");

		MeshNodeMan::Add(Mesh::Name::PYRAMID, poMeshPyramid);
		MeshNodeMan::Add(Mesh::Name::SPHERE, poMeshSphere);
		MeshNodeMan::Add(Mesh::Name::CUBE, poMeshCube);
		MeshNodeMan::Add(Mesh::Name::FISH, pFishMesh);
		MeshNodeMan::Add(Mesh::Name::CORSET, pCorsetMesh);
		MeshNodeMan::Add(Mesh::Name::ANTIQUE_CAMERA, pAntiqueCameraMesh);
		MeshNodeMan::Add(Mesh::Name::ANTIQUE_CAMERA, pAntiqueCameraMesh1);

		MeshNodeMan::Add(Mesh::Name::DOG, pDogMesh);
		MeshNodeMan::Add(Mesh::Name::DOG, pDogMesh1);

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
		TextureObject *pTexFish = new TextureProto("BarramundiFish.mt.proto.azul");
		TextureObject* pTexCorset = new TextureProto("Corset.mt.proto.azul");
		TextureObject* pTexAntiqueCamera = new TextureProto("AntiqueCamera.mt.proto.azul");
		TextureObject* pTexAntiqueCamera1 = new TextureProto("AntiqueCamera1.mt.proto.azul");
		TextureObject* pTexDog = new TextureProto("Dog.mt.proto.azul");
		TextureObject* pTexDog1 = new TextureProto("Dog1.mt.proto.azul");


		TexNodeMan::Add(TextureObject::Name::Duckweed, pTexDuck);
		TexNodeMan::Add(TextureObject::Name::Brick, pTexBrick);
		TexNodeMan::Add(TextureObject::Name::Stone, pTexStone);
		TexNodeMan::Add(TextureObject::Name::Rocks, pTexRocks);
		TexNodeMan::Add(TextureObject::Name::Fish, pTexFish);
		TexNodeMan::Add(TextureObject::Name::Corset, pTexCorset);
		TexNodeMan::Add(TextureObject::Name::Antique_Camera, pTexAntiqueCamera);
		TexNodeMan::Add(TextureObject::Name::Antique_Camera, pTexAntiqueCamera1);
		TexNodeMan::Add(TextureObject::Name::Dog, pTexDog);
		TexNodeMan::Add(TextureObject::Name::Dog, pTexDog1);

		// --------------------------------
		//  Graphics Object -- needs model + shader
		// --------------------------------

		GameObject *pGameObj(nullptr);
		GraphicsObject *pGraphicsObject(nullptr);

		Vec3 LightColor(1.0f, 1.0f, 1.0f);
		Vec3 LightPos(1.0f, 30.0f, 100.0f);

		pGraphicsObject = new GraphicsObject_LightTexture(pFishMesh,
														  poShaderC,
														  pTexFish,
														  LightColor,
														  LightPos);

		pGameObj = new GameObject(pGraphicsObject);
		pGameObj->SetName("Fish");
		pGameObj->SetPos(Vec3(0.5, 1, 0));
		pGameObj->SetScale(0.40f);

		GameObjectMan::Add(pGameObj);

		pGraphicsObject = new GraphicsObject_LightTexture(pCorsetMesh,
			poShaderC,
			pTexCorset,
			LightColor,
			LightPos);

		pGameObj = new GameObject(pGraphicsObject);
		pGameObj->SetName("Corset");
		pGameObj->SetPos(Vec3(-1.5, 1, 0));
		pGameObj->SetScale(10.0f);

		GameObjectMan::Add(pGameObj);

		pGraphicsObject = new GraphicsObject_LightTexture(pAntiqueCameraMesh,
			poShaderC,
			pTexAntiqueCamera,
			LightColor,
			LightPos);

		pGameObj = new GameObject(pGraphicsObject);
		pGameObj->SetName("Antique Camera");
		pGameObj->SetPos(Vec3(1.5, 1, 0));
		pGameObj->SetScale(0.05f);

		GameObjectMan::Add(pGameObj);

		pGraphicsObject = new GraphicsObject_LightTexture(pAntiqueCameraMesh1,
			poShaderC,
			pTexAntiqueCamera1,
			LightColor,
			LightPos);

		pGameObj = new GameObject(pGraphicsObject);
		pGameObj->SetName("Antique Camera");
		pGameObj->SetPos(Vec3(1.5, 1, 0));
		pGameObj->SetScale(0.05f);

		GameObjectMan::Add(pGameObj);

		pGraphicsObject = new GraphicsObject_LightTexture(pDogMesh,
			poShaderC,
			pTexDog,
			LightColor,
			LightPos);

		pGameObj = new GameObject(pGraphicsObject);
		pGameObj->SetName("Dog");
		pGameObj->SetPos(Vec3(-0.5, 1, 0));
		pGameObj->SetScale(0.05f);

		GameObjectMan::Add(pGameObj);

		//Dog Bounding Sphere
		/*
		Vec3 dogCenter = pDogMesh->boundingSphereCenter;
		float dogRadius = pDogMesh->boundingSphereRadius;
		
		pGraphicsObject = new GraphicsObject_Wireframe(poMeshSphere,
			poShaderD,
			LightColor);

		pGameObj = new GameObject(pGraphicsObject);
		pGameObj->SetName("Dog Bounding Sphere");
		pGameObj->SetPos(dogCenter);
		pGameObj->SetScale(dogRadius);
		GameObjectMan::Add(pGameObj);*/


		pGraphicsObject = new GraphicsObject_LightTexture(pDogMesh1,
			poShaderC,
			pTexDog1,
			LightColor,
			LightPos);

		pGameObj = new GameObject(pGraphicsObject);
		pGameObj->SetName("Dog");
		pGameObj->SetPos(Vec3(-0.5, 1, 0));
		pGameObj->SetScale(0.05f);

		GameObjectMan::Add(pGameObj);


		pGraphicsObject = new GraphicsObject_FlatTexture(poMeshCube,
														 poShaderB,
														 pTexDuck);

		pGameObj = new GameObject(pGraphicsObject);
		pGameObj->SetName("Dave");
		pGameObj->SetPos(Vec3(-1.5, 0, 0));
		pGameObj->SetScale(1.0f);

		GameObjectMan::Add(pGameObj);


		pGraphicsObject = new GraphicsObject_FlatTexture(poMeshSphere,
														 poShaderB,
														 pTexRocks);

		pGameObj = new GameObject(pGraphicsObject);
		pGameObj->SetName("Dave");
		pGameObj->SetPos(Vec3(0.5, 0, 0));
		pGameObj->SetScale(0.40f);

		GameObjectMan::Add(pGameObj);

		pGraphicsObject = new GraphicsObject_FlatTexture(poMeshCube,
														 poShaderB,
														 pTexBrick);

		pGameObj = new GameObject(pGraphicsObject);
		pGameObj->SetName("Dave");
		pGameObj->SetPos(Vec3(-0.5, 0, 0));
		pGameObj->SetScale(1.0f);

		GameObjectMan::Add(pGameObj);

		pGraphicsObject = new GraphicsObject_FlatTexture(poMeshPyramid,
														 poShaderB,
														 pTexStone);

		pGameObj = new GameObject(pGraphicsObject);
		pGameObj->SetName("Dave");
		pGameObj->SetPos(Vec3(1.5, 0, 0));
		pGameObj->SetScale(0.30f);

		GameObjectMan::Add(pGameObj);

		return true;
	}

	//-----------------------------------------------------------------------------
	// Game::Update()
	//      Called once per frame, update data, tranformations, etc
	//      Use this function to control process order
	//      Input, AI, Physics, Animation, and Graphics
	//-----------------------------------------------------------------------------


	void Game::Update(float )
	{
		// -----------------------
		// Read inputs
		// -----------------------
		CameraDemoInput();

		// ------------------------------------
		// Update the camera once per frame
		// ------------------------------------
		Camera *pCam = CameraMan::GetCurrent();
		pCam->updateCamera();

		// ------------------------------------
		// Update the Objects once per frame
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
