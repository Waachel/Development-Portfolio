//-----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

// --------------------------------
// ---      DO NOT MODIFY       ---
// --------------------------------

#include "AzulCore.h"
#include "Game.h"
#include "GameApp.h"

GameApp * GameApp::privGameApp()
{
	static GameApp gameApp;
	return &gameApp;
}
   
GameApp::GameApp()
{     
	this->pCam3D = nullptr;
	this->pSphereLight = nullptr;
	this->pSpaceFrigateLight = nullptr;
	this->pSpaceFrigateTex = nullptr;
	this->pSphereTex = nullptr;
	this->pModelSpaceFrigate = nullptr;
	this->pModelSphere = nullptr;
	this->pShaderObject_textureLight = nullptr;
}

void GameApp::LoadDemo( int width, int height  )
{
	GameApp *pGameApp = GameApp::privGameApp();

	//---------------------------------------------------------------------------------------------------------
	// Setup the current 3D perspective Camera
	//---------------------------------------------------------------------------------------------------------
	 
	pGameApp->pCam3D = new Camera(Camera::Type::PERSPECTIVE_3D);
	assert(pGameApp->pCam3D);

	pGameApp->pCam3D->setViewport(0, 0, width, height);
	pGameApp->pCam3D->setPerspective(35.0f, float(width) / float(height), 1.0f, 500.0f);

	// Orient Camera
	Vect up3DCam(0.0f, 1.0f, 0.0f);
	Vect pos3DCam(0.0f, 120.0f, 120.0f);
	Vect lookAt3DCam(0.0f, 0.0f, -1.0f);
	pGameApp->pCam3D->setOrientAndPosition(up3DCam, lookAt3DCam, pos3DCam);

	//---------------------------------------------------------------------------------------------------------
	// Load the Models
	//---------------------------------------------------------------------------------------------------------

	pGameApp->pModelSpaceFrigate = new Model("space_frigate.azul");
	pGameApp->pModelSphere = new Model("sphere.azul");
	  
	//---------------------------------------------------------------------------------------------------------
	// Load the Shaders
	//---------------------------------------------------------------------------------------------------------

	pGameApp->pShaderObject_textureLight = new ShaderObject("textureLightRender");

	//---------------------------------------------------------------------------------------------------------
	// Load the Textures
	//---------------------------------------------------------------------------------------------------------

	// Direct loads
	pGameApp->pSpaceFrigateTex = new Texture("space_frigate.tga");
	pGameApp->pSphereTex = new Texture("sphere.tga");

	//---------------------------------------------------------------------------------------------------------
	// Graphics Objects
	//---------------------------------------------------------------------------------------------------------

	// Light
	Vect LightColor(1.50f, 1.50f, 1.50f, 1.0f);
	Vect LightPos(1.0f, 1.0f, 1.0f, 1.0f);
	pGameApp->pSphereLight = new GraphicsObject_TextureLight(pGameApp->pModelSphere, pGameApp->pShaderObject_textureLight, pGameApp->pSphereTex, LightColor, LightPos);
	pGameApp->pSpaceFrigateLight = new GraphicsObject_TextureLight(pGameApp->pModelSpaceFrigate, pGameApp->pShaderObject_textureLight, pGameApp->pSpaceFrigateTex, LightColor, LightPos);

}

void GameApp::UpdateDemo()
{
	GameApp *pGameApp = GameApp::privGameApp();

	// Update cameras - make sure everything is consistent
	pGameApp->pCam3D->updateCamera();

	// update objects
	Matrix world;

	Matrix RotY;
	Matrix RotZ;
	Matrix Scale;
	Matrix Trans;

	static float angle = 0.0f;
	angle += 0.04f;

	// Space Frigate Texture: 
	RotY.set(ROT_Y, 1.3f*angle);
	Trans.set(TRANS, 20.0f, 0.0f, 0.0f);
	Trans.set(TRANS, -20.0f, 10.0f, 0.0f);
	world = RotY * Trans;
	pGameApp->pSpaceFrigateLight->SetWorld(world);

	// Sphere Light
	Scale.set(SCALE, 15.0f, 15.0f, 15.0f);
	Trans.set(TRANS, -40.0f, -10.0f, 0.0f);
	Trans.set(TRANS, 40.0f, 0.0f, 0.0f);
	RotY.set(ROT_Y, angle);
	RotZ.set(ROT_Z, 0.25f*angle);
	world = Scale * RotY * RotZ * Trans;
	pGameApp->pSphereLight->SetWorld(world);

}

void GameApp::DrawDemo()
{
	GameApp *pGameApp = GameApp::privGameApp();

	// draw all objects
	pGameApp->pSpaceFrigateLight->Render(pGameApp->pCam3D);
	pGameApp->pSphereLight->Render(pGameApp->pCam3D);
}

GameApp::~GameApp()
{
	delete pCam3D;
	delete pSphereLight;
	delete pSpaceFrigateLight;
	delete pSpaceFrigateTex;
	delete pSphereTex;
	delete pModelSpaceFrigate;
	delete pModelSphere;
	delete pShaderObject_textureLight;
}

//---  End of File ---
