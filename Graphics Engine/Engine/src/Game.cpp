//--------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#include <d3d11.h>
#include <d3dcompiler.h>
#include <WinUser.h>
#include "Game.h"
#include "Engine.h"
#include "MathEngine.h"
#include "CameraNodeMan.h"
#include "Colors.h"
#include "GameObject.h"
#include "ShaderObject.h"
#include "ShaderObject_ColorByVertex.h"
#include "ShaderObject_FlatTexture.h"
#include "ShaderObject_LightTexture.h"
#include "ShaderObject_ConstColor.h"
#include "Mesh.h"
#include "MeshCross.h"
#include "MeshCube.h"
#include "MeshPyramid.h"
#include "MeshSphere.h"
#include "MeshDiamond.h"
#include "GraphicsObject_ColorByVertex.h"
#include "GraphicsObject_Wireframe.h"
#include "GraphicsObject_Solid.h"
#include "GraphicsObject_FlatTexture.h"
#include "GraphicsObject_LightTexture.h"
#include "GraphicsObject_ConstColor.h"
#include "GraphicsObject_ConstColorSolid.h"
#include "GameObjectMan.h"
#include "MeshNodeMan.h"
#include "ShaderObjectNodeMan.h"
#include "TextureObjectNodeMan.h"


using namespace Azul;

// HACK FEST --- will eventually be in managers/singleton
ID3D11Device *pHackDevice;
ID3D11DeviceContext *pHackDeviceContext;
Camera *pHackCamera;
CameraNode* pCam2;
Camera* pCam;

// Demo parameters
TextureObject *pTexA = nullptr;
TextureObject *pTexB = nullptr;
TextureObject *pTexC = nullptr;
TextureObject *pTexD = nullptr;


// Demo
Vec3 Orig_up;
Vec3 Orig_tar;
Vec3 Orig_pos;
Vec3 Orig_upNorm;
Vec3 Orig_forwardNorm;
Vec3 Orig_RightNorm;

Game::Game(const char *const pName, int width, int height)
	: Engine(pName, width, height)
{
	mTimer.Reset();
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
	CameraNodeMan::Create();
	TextureObjectNodeMan::Create();

	// ---------------------------------
	//  Camera - Setup
	// ---------------------------------
	{
		pHackCamera = new Camera();
		assert(pHackCamera);

		Vec3 camPos(0, -10, 2);
		Vec3 tarVect(0, 0, -1);
		Vec3 upVect(0, 1, 0);

		pHackCamera->setOrientAndPosition(upVect, tarVect, camPos);
		pHackCamera->setPerspective(50.0f, GetAspectRatio(), 0.1f, 1000.0f);
		pHackCamera->SetName(Camera::Name::CAMERA_1);
		CameraNodeMan::Add(pHackCamera);

		Camera* pCamera2 = new Camera();
		assert(pCamera2);

		Vec3 camPos2(10, 5, 8);
		Vec3 tarVect2(0, -1, 2);
		Vec3 upVect2(0, 1, 0);

		pCamera2->setOrientAndPosition(upVect, tarVect, camPos2);
		pCamera2->setPerspective(50.0f, GetAspectRatio(), 0.1f, 1000.0f);
		pCamera2->SetName(Camera::Name::CAMERA_2);
		CameraNodeMan::Add(pCamera2);

		Camera* pCamera3 = new Camera();
		assert(pCamera3);

		Vec3 camPos3(10, 1, -8);
		Vec3 tarVect3(0, -1, 2);
		Vec3 upVect3(0, 1, 0);

		pCamera3->setOrientAndPosition(upVect, tarVect, camPos3);
		pCamera3->setPerspective(50.0f, GetAspectRatio(), 0.1f, 1000.0f);
		pCamera3->SetName(Camera::Name::CAMERA_3);
		CameraNodeMan::Add(pCamera3);

		Camera* pCamera4 = new Camera();
		assert(pCamera4);

		Vec3 camPos4(0, -1, 12);
		Vec3 tarVect4(0, 0, 1);
		Vec3 upVect4(0, -1, 0);

		pCamera4->setOrientAndPosition(upVect, tarVect, camPos4);
		pCamera4->setPerspective(50.0f, GetAspectRatio(), 0.1f, 1000.0f);
		pCamera4->SetName(Camera::Name::CAMERA_4);
		CameraNodeMan::Add(pCamera4);

		CameraNodeMan::SetCurrentCam(Camera::Name::CAMERA_1);
	}

	// ------------------------------------------
	//   Model + Shaders --> GraphicsObject
	// -------------------------------------------
	{
		// ---------------------------------
		//  Model - Cube
		// ---------------------------------

		Mesh *poMeshA = new MeshCube();
		assert(poMeshA);

		MeshNodeMan::Add(Mesh::Name::CUBE, poMeshA);

		Mesh *poMeshB = new MeshSphere();
		assert(poMeshB);

		MeshNodeMan::Add(Mesh::Name::SPHERE, poMeshB);

		Mesh* poMeshC = new MeshPyramid();
		assert(poMeshC);

		MeshNodeMan::Add(Mesh::Name::PYRAMID, poMeshC);

		Mesh* poMeshD = new MeshDiamond();
		assert(poMeshD);

		MeshNodeMan::Add(Mesh::Name::DIAMOND, poMeshD);

		Mesh* poMeshE = new MeshCross();
		assert(poMeshE);

		MeshNodeMan::Add(Mesh::Name::CROSS, poMeshE);

		//MeshNodeMan::Dump();

		// --------------------------------
		//  ShaderObject  ColorByVertex
		// --------------------------------
		ShaderObject *poShaderA = new ShaderObject_ColorByVertex(ShaderObject::Name::ColorByVertex);
		ShaderObjectNodeMan::Add(poShaderA);

		ShaderObject *poShaderB = new ShaderObject_FlatTexture(ShaderObject::Name::FlatTexture);
		ShaderObjectNodeMan::Add(poShaderB);

		ShaderObject *poShaderC = new ShaderObject_LightTexture(ShaderObject::Name::LightTexture);
		ShaderObjectNodeMan::Add(poShaderC);

		ShaderObject *poShaderD = new ShaderObject_ConstColor( ShaderObject::Name::ConstColor );
		ShaderObjectNodeMan::Add( poShaderD );

		

			// -----------------------------------
			//  Texture
			// -----------------------------------

		pTexA = new TextureObject(TextureObject::Name::Duckweed, L"Duckweed.tga");
		TextureObjectNodeMan::Add(pTexA);

		pTexB = new TextureObject( TextureObject::Name::Brick, L"RedBrick.tga" );
		TextureObjectNodeMan::Add(pTexB);

		pTexC = new TextureObject( TextureObject::Name::Stone, L"Stone.tga" );
		TextureObjectNodeMan::Add(pTexC);

		pTexD = new TextureObject( TextureObject::Name::Rocks, L"Rocks.tga" );
		TextureObjectNodeMan::Add(pTexD);

		TextureObjectNodeMan::Dump();
		// --------------------------------
		//  Graphics Object -- needs model + shader
		// --------------------------------
		//GraphicsObject *pGraphicsObject = new GraphicsObject_FlatTexture(poMeshB,
		//																 poShaderB,
		//																 pTexA);

		Vec3 LightColorC(1,0,0);
		Vec3 LightPosC(0,-100, 0);

		//Mesh* poMeshA = new MeshCube();
		{
			LightColorC *= 1.5f;
			GraphicsObject* pGraphicsObject = new GraphicsObject_LightTexture(poMeshA,
				poShaderC,
				pTexA,
				LightColorC,
				LightPosC);
			GameObject* pGameObj = new GameObject(pGraphicsObject);
			pGameObj->SetName("Light Textured Cube");
			pGameObj->SetPos(Vec3(0, 0, 0));
			pGameObj->SetScale(2.0f);

			GameObjectMan::Add(pGameObj);
		}

		{

			GraphicsObject* pGraphicsObject = new GraphicsObject_FlatTexture(poMeshA,
				poShaderB,
				pTexC);
			GameObject* pGameObj = new GameObject(pGraphicsObject);
			pGameObj->SetName("Flat Cube");
			pGameObj->SetPos(Vec3(2, 0, 0));
			pGameObj->SetScale(2.0f);

			GameObjectMan::Add(pGameObj);
		}

		{

			GraphicsObject* pGraphicsObject = new GraphicsObject_Solid(poMeshA,
				poShaderA);
			GameObject* pGameObj = new GameObject(pGraphicsObject);
			pGameObj->SetName("Solid Cube");
			pGameObj->SetPos(Vec3(-2, 0, 0));
			pGameObj->SetScale(2.0f);

			GameObjectMan::Add(pGameObj);
		}


		{
			Vec3 color(1.0f, 1.0f, 1.0f);
			GraphicsObject* pGraphicsObject = new GraphicsObject_ConstColor(poMeshA,
				poShaderD,
				color);
			GameObject* pGameObj = new GameObject(pGraphicsObject);
			pGameObj->SetName("Constant Color Cube");
			pGameObj->SetPos(Vec3(-4, 0, 0));
			pGameObj->SetScale(2.0f);

			GameObjectMan::Add(pGameObj);
		}

		Vec3 LightColor(1, 1, 1);
		Vec3 LightPos(0, -100, 0);
		//Mesh* poMeshB = new MeshSphere();
		{
			LightColor *= 1.5f;
			GraphicsObject* pGraphicsObject = new GraphicsObject_LightTexture(poMeshB,
				poShaderC,
				pTexC,
				LightColor,
				LightPos);
			GameObject* pGameObj = new GameObject(pGraphicsObject);
			pGameObj->SetName("Light Textured Sphere");
			pGameObj->SetPos(Vec3(0, 0, 2));
			pGameObj->SetScale(0.5f);

			GameObjectMan::Add(pGameObj);
		}

		{

			GraphicsObject* pGraphicsObject = new GraphicsObject_FlatTexture(poMeshB,
				poShaderB,
				pTexD);
			GameObject* pGameObj = new GameObject(pGraphicsObject);
			pGameObj->SetName("Flat Sphere");
			pGameObj->SetPos(Vec3(2, 0, 2));
			pGameObj->SetScale(0.5f);

			GameObjectMan::Add(pGameObj);
		}

		{

			GraphicsObject* pGraphicsObject = new GraphicsObject_Wireframe(poMeshB,
				poShaderA);
			GameObject* pGameObj = new GameObject(pGraphicsObject);
			pGameObj->SetName("Wireframe Sphere");
			pGameObj->SetPos(Vec3(-2, 0, 2));
			pGameObj->SetScale(0.5f);

			GameObjectMan::Add(pGameObj);
		}


		{
			Vec3 color(1.0f, 0.5f, 0.9f);
			GraphicsObject* pGraphicsObject = new GraphicsObject_ConstColorSolid(poMeshB,
				poShaderD,
				color);
			GameObject* pGameObj = new GameObject(pGraphicsObject);
			pGameObj->SetName("Constant Color Sphere");
			pGameObj->SetPos(Vec3(-4, 0, 2));
			pGameObj->SetScale(0.5f);

			GameObjectMan::Add(pGameObj);
		}

		Vec3 LightColorP(0, 0, 1);
		Vec3 LightPosP(0, 100, 0);
		//Mesh* poMeshC = new MeshPyramid();
		{
			LightColorP *= 3.0f;
			GraphicsObject* pGraphicsObject = new GraphicsObject_LightTexture(poMeshC,
				poShaderC,
				pTexA,
				LightColorP,
				LightPosP);
			GameObject* pGameObj = new GameObject(pGraphicsObject);
			pGameObj->SetName("Light Textured Pyramid");
			pGameObj->SetPos(Vec3(0, 0, -2));
			pGameObj->SetScale(0.5f);

			GameObjectMan::Add(pGameObj);
		}

		{

			GraphicsObject* pGraphicsObject = new GraphicsObject_FlatTexture(poMeshC,
				poShaderB,
				pTexB);
			GameObject* pGameObj = new GameObject(pGraphicsObject);
			pGameObj->SetName("Flat Pyramid");
			pGameObj->SetPos(Vec3(2, 0, -2));
			pGameObj->SetScale(0.5f);

			GameObjectMan::Add(pGameObj);
		}

		{

			GraphicsObject* pGraphicsObject = new GraphicsObject_Wireframe(poMeshC,
				poShaderA);
			GameObject* pGameObj = new GameObject(pGraphicsObject);
			pGameObj->SetName("Wireframe Pyramid");
			pGameObj->SetPos(Vec3(-2, 0, -2));
			pGameObj->SetScale(0.5f);

			GameObjectMan::Add(pGameObj);
		}


		{
			Vec3 color(0.5f, 0.0f, 0.3f);
			GraphicsObject* pGraphicsObject = new GraphicsObject_ConstColorSolid(poMeshC,
				poShaderD,
				color);
			GameObject* pGameObj = new GameObject(pGraphicsObject);
			pGameObj->SetName("Constant Color Pyramid");
			pGameObj->SetPos(Vec3(-4, 0, -2));
			pGameObj->SetScale(0.5f);

			GameObjectMan::Add(pGameObj);
		}

		//Mesh* poMeshD = new MeshDiamond();
		{
			LightColor *= 1.5f;
			GraphicsObject* pGraphicsObject = new GraphicsObject_LightTexture(poMeshD,
				poShaderC,
				pTexC,
				LightColor,
				LightPos);
			GameObject* pGameObj = new GameObject(pGraphicsObject);
			pGameObj->SetName("Light Textured Diamond");
			pGameObj->SetPos(Vec3(0, 0, 4));
			pGameObj->SetScale(1.0f);

			GameObjectMan::Add(pGameObj);
		}

		{

			GraphicsObject* pGraphicsObject = new GraphicsObject_FlatTexture(poMeshD,
				poShaderB,
				pTexD);
			GameObject* pGameObj = new GameObject(pGraphicsObject);
			pGameObj->SetName("Flat Diamond");
			pGameObj->SetPos(Vec3(2, 0, 4));
			pGameObj->SetScale(1.0f);

			GameObjectMan::Add(pGameObj);
		}

		{

			GraphicsObject* pGraphicsObject = new GraphicsObject_Solid(poMeshD,
				poShaderA);
			GameObject* pGameObj = new GameObject(pGraphicsObject);
			pGameObj->SetName("Wireframe Diamond");
			pGameObj->SetPos(Vec3(-2, 0, 4));
			pGameObj->SetScale(1.0f);

			GameObjectMan::Add(pGameObj);
		}


		{
			Vec3 color(0.5f, 0.5f, 0.9f);
			GraphicsObject* pGraphicsObject = new GraphicsObject_ConstColor(poMeshD,
				poShaderD,
				color);
			GameObject* pGameObj = new GameObject(pGraphicsObject);
			pGameObj->SetName("Constant Color Diamond");
			pGameObj->SetPos(Vec3(-4, 0, 4));
			pGameObj->SetScale(1.0f);

			GameObjectMan::Add(pGameObj);
		}

		//Mesh* poMeshE = new MeshCross();
		{
			LightColor *= 0.5f;
			GraphicsObject* pGraphicsObject = new GraphicsObject_LightTexture(poMeshE,
				poShaderC,
				pTexA,
				LightColor,
				LightPos);
			GameObject* pGameObj = new GameObject(pGraphicsObject);
			pGameObj->SetName("Light Textured Cross");
			pGameObj->SetPos(Vec3(0, 0, -4));
			pGameObj->SetScale(1.0f);

			GameObjectMan::Add(pGameObj);
		}

		{

			GraphicsObject* pGraphicsObject = new GraphicsObject_FlatTexture(poMeshE,
				poShaderB,
				pTexC);
			GameObject* pGameObj = new GameObject(pGraphicsObject);
			pGameObj->SetName("Flat Cross");
			pGameObj->SetPos(Vec3(2, 0, -4));
			pGameObj->SetScale(1.0f);

			GameObjectMan::Add(pGameObj);
		}

		{

			GraphicsObject* pGraphicsObject = new GraphicsObject_Solid(poMeshE,
				poShaderA);
			GameObject* pGameObj = new GameObject(pGraphicsObject);
			pGameObj->SetName("Wireframe Cross");
			pGameObj->SetPos(Vec3(-2, 0, -4));
			pGameObj->SetScale(1.0f);

			GameObjectMan::Add(pGameObj);
		}


		{
			Vec3 color(1.0f, 0.9f, 0.0f);
			GraphicsObject* pGraphicsObject = new GraphicsObject_ConstColor(poMeshE,
				poShaderD,
				color);
			GameObject* pGameObj = new GameObject(pGraphicsObject);
			pGameObj->SetName("Constant Color Cross");
			pGameObj->SetPos(Vec3(-4, 0, -4));
			pGameObj->SetScale(1.0f);

			GameObjectMan::Add(pGameObj);
		}
	}


	// squirrel for reset
	CameraNodeMan::Update();
	CameraNodeMan::GetActiveHelper(Orig_up, Orig_tar, Orig_pos, Orig_upNorm, Orig_forwardNorm, Orig_RightNorm);

	return true;
}

//-----------------------------------------------------------------------------
// Game::Update()
//      Called once per frame, update data, tranformations, etc
//      Use this function to control process order
//      Input, AI, Physics, Animation, and Graphics
//-----------------------------------------------------------------------------

Vec3 obj(0.0f, 0.0f, 0.0f);

void Game::Update(float)
{
	// ------------------------------------
	// Update the camera once per frame
	// ------------------------------------
	mTimer.Tick();
	//Camera* pCam = pHackCamera;	
	//Camera* pCam = CameraNodeMan::GetActiveCam();

	if ((GetKeyState('1') & 0x8000))
	{
		CameraNodeMan::SetCurrentCam(Camera::Name::CAMERA_1);
	}
	if ((GetKeyState('2') & 0x8000))
	{
		CameraNodeMan::SetCurrentCam(Camera::Name::CAMERA_2);
	}
	if ((GetKeyState('3') & 0x8000))
	{
		CameraNodeMan::SetCurrentCam(Camera::Name::CAMERA_3);
	}
	if ((GetKeyState('4') & 0x8000))
	{
		CameraNodeMan::SetCurrentCam(Camera::Name::CAMERA_4);
	}

	if ((GetKeyState('S') & 0x8000))
	{
		// Reset	
		obj.set(0, 0, 0);
		CameraNodeMan::SetActiveHelper(Orig_up, Orig_tar, Orig_pos);
	}

	if ((GetKeyState('W') & 0x8000))
	{
		Vec3 newObj(2, 0, 0);
		Vec3 pos;
		Vec3 tar;
		Vec3 up;
		Vec3 upNorm;
		Vec3 forwardNorm;
		Vec3 rightNorm;

		CameraNodeMan::GetActiveHelper(up, tar, pos, upNorm, forwardNorm, rightNorm);

		// OK, now 3 points... pos, tar, up
		//     now 3 normals... upNorm, forwardNorm, rightNorm

		Vec3 delta = newObj - obj;

		up += delta;
		tar += delta;
		pos += delta;

		obj = newObj;

		CameraNodeMan::SetActiveHelper(up, tar, pos);
	}

	if ((GetKeyState('E') & 0x8000))
	{
		Vec3 newObj(-2, 0, 0);
		Vec3 pos;
		Vec3 tar;
		Vec3 up;
		Vec3 upNorm;
		Vec3 forwardNorm;
		Vec3 rightNorm;

		CameraNodeMan::GetActiveHelper(up, tar, pos, upNorm, forwardNorm, rightNorm);

		// OK, now 3 points... pos, tar, up
		//     now 3 normals... upNorm, forwardNorm, rightNorm

		Vec3 delta = newObj - obj;

		up += delta;
		tar += delta;
		pos += delta;

		obj = newObj;

		CameraNodeMan::SetActiveHelper(up, tar, pos);
	}


	if ((GetKeyState('D') & 0x8000))
	{

		Vec3 newObj(0, 0, 0);
		Vec3 pos;
		Vec3 tar;
		Vec3 up;
		Vec3 upNorm;
		Vec3 forwardNorm;
		Vec3 rightNorm;

		CameraNodeMan::GetActiveHelper(up, tar, pos, upNorm, forwardNorm, rightNorm);

		// OK, now 3 points... pos, tar, up
		//     now 3 normals... upNorm, forwardNorm, rightNorm
		Vec3 delta = newObj - obj;

		up += delta;
		tar += delta;
		pos += delta;

		obj = newObj;

		CameraNodeMan::SetActiveHelper(up, tar, pos);
	}

	if ((GetKeyState('I') & 0x8000))
	{
		Vec3 pos;
		Vec3 tar;
		Vec3 up;
		Vec3 upNorm;
		Vec3 forwardNorm;
		Vec3 rightNorm;

		CameraNodeMan::GetActiveHelper(up, tar, pos, upNorm, forwardNorm, rightNorm);

		// OK, now 3 points... pos, tar, up
		//     now 3 normals... upNorm, forwardNorm, rightNorm
		//	tar = obj;

		pos = pos + 0.03f * forwardNorm;
		//tar = tar + 0.03f * forwardNorm;
		up = up + 0.03f * forwardNorm;

		CameraNodeMan::SetActiveHelper(up, tar, pos);
	}

	if ((GetKeyState('O') & 0x8000))
	{
		Vec3 pos;
		Vec3 tar;
		Vec3 up;
		Vec3 upNorm;
		Vec3 forwardNorm;
		Vec3 rightNorm;

		CameraNodeMan::GetActiveHelper(up, tar, pos, upNorm, forwardNorm, rightNorm);

		// OK, now 3 points... pos, tar, up
		//     now 3 normals... upNorm, forwardNorm, rightNorm
		//tar = obj;

		pos = pos - 0.03f * forwardNorm;
		up = up - 0.03f * forwardNorm;

		CameraNodeMan::SetActiveHelper(up, tar, pos);

	}

	if ((GetKeyState(VK_RIGHT) & 0x8000))
	{

		Vec3 pos;
		Vec3 tar;
		Vec3 up;
		Vec3 upNorm;
		Vec3 forwardNorm;
		Vec3 rightNorm;

		CameraNodeMan::GetActiveHelper(up, tar, pos, upNorm, forwardNorm, rightNorm);

		// OK, now 3 points... pos, tar, up
		//     now 3 normals... upNorm, forwardNorm, rightNorm
	//	tar = obj;

		Mat4 Trans(Mat4::Trans::XYZ, tar);
		Mat4 NegTrans(Mat4::Trans::XYZ, -tar);

		Mat4 Rot;
		Rot.set(Mat4::Rot::AxisAngle, upNorm, +0.03f);

		Mat4 M = NegTrans * Rot * Trans;

		up = Vec4(up, 1) * M;
		pos = Vec4(pos, 1) * M;
		tar = Vec4(tar, 1) * M;

		CameraNodeMan::SetActiveHelper(up, tar, pos);
	}

	if ((GetKeyState(VK_LEFT) & 0x8000))
	{

		Vec3 pos;
		Vec3 tar;
		Vec3 up;
		Vec3 upNorm;
		Vec3 forwardNorm;
		Vec3 rightNorm;

		CameraNodeMan::GetActiveHelper(up, tar, pos, upNorm, forwardNorm, rightNorm);

		// OK, now 3 points... pos, tar, up
		//     now 3 normals... upNorm, forwardNorm, rightNorm

	//	tar = obj;

		Mat4 Trans(Mat4::Trans::XYZ, tar);
		Mat4 NegTrans(Mat4::Trans::XYZ, -tar);

		Mat4 Rot;
		Rot.set(Mat4::Rot::AxisAngle, upNorm, -0.03f);

		Mat4 M = NegTrans * Rot * Trans;

		up = Vec4(up, 1) * M;
		pos = Vec4(pos, 1) * M;
		tar = Vec4(tar, 1) * M;

		CameraNodeMan::SetActiveHelper(up, tar, pos);
	}

	if ((GetKeyState(VK_DOWN) & 0x8000))
	{
		Vec3 pos;
		Vec3 tar;
		Vec3 up;
		Vec3 upNorm;
		Vec3 forwardNorm;
		Vec3 rightNorm;

		CameraNodeMan::GetActiveHelper(up, tar, pos, upNorm, forwardNorm, rightNorm);

		//	tar = obj;


			// OK, now 3 points... pos, tar, up
			//     now 3 normals... upNorm, forwardNorm, rightNorm

		Mat4 Trans(Mat4::Trans::XYZ, tar);
		Mat4 NegTrans(Mat4::Trans::XYZ, -tar);
		Mat4 Rot;
		Rot.set(Mat4::Rot::AxisAngle, rightNorm, 0.03f);

		Mat4 M = NegTrans * Rot * Trans;

		up = Vec4(up, 1) * M;
		pos = Vec4(pos, 1) * M;
		tar = Vec4(tar, 1) * M;

		CameraNodeMan::SetActiveHelper(up, tar, pos);
	}

	if ((GetKeyState(VK_UP) & 0x8000))
	{
		Vec3 pos;
		Vec3 tar;
		Vec3 up;
		Vec3 upNorm;
		Vec3 forwardNorm;
		Vec3 rightNorm;

		CameraNodeMan::GetActiveHelper(up, tar, pos, upNorm, forwardNorm, rightNorm);

		// OK, now 3 points... pos, tar, up
		//     now 3 normals... upNorm, forwardNorm, rightNorm
		//tar = obj;

		Mat4 Trans(Mat4::Trans::XYZ, tar);
		Mat4 NegTrans(Mat4::Trans::XYZ, -tar);
		Mat4 Rot;
		Rot.set(Mat4::Rot::AxisAngle, rightNorm, -0.03f);

		Mat4 M = NegTrans * Rot * Trans;

		up = Vec4(up, 1) * M;
		pos = Vec4(pos, 1) * M;
		tar = Vec4(tar, 1) * M;

		CameraNodeMan::SetActiveHelper(up, tar, pos);
	}

	CameraNodeMan::Update();
	GameObjectMan::Update(mTimer.TotalTime());
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
	//delete poMeshA;
	//delete poShaderA;
	//delete pHackCamera;

	//delete pTexA;
	//delete pTexB;
	//delete pTexC;
	//delete pTexD;

	ShaderObjectNodeMan::Destroy();
	MeshNodeMan::Destroy();
	GameObjectMan::Destroy();
	CameraNodeMan::Destroy();
	TextureObjectNodeMan::Destroy();
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

// --- End of File ---
