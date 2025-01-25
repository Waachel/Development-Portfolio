#ifndef AZUL_CORE_H
#define AZUL_CORE_H

// System includes
#include <math.h>
#include <assert.h>

// External Libraries
#include "sb7.h"
#include "sb7ktx.h"
#include "MathEngine.h"

// Helper includes
#include "AzulHelper.h"
#include "DebugOut.h"

// Inputs includes
#include "Keyboard.h"
#include "Mouse.h"

// Engine 
#include "Engine.h"
#include "Model.h"
#include "DebugOut.h"
#include "Camera.h"
#include "ShaderObject.h"
#include "Texture.h"
#include "Image.h"
#include "Rect.h"

// Graphics Objects
#include "GraphicsObject.h"
#include "GraphicsObject_ColorByPosition.h"
#include "GraphicsObject_ColorNoTexture.h"
#include "GraphicsObject_WireframeConstantColor.h"
#include "GraphicsObject_WireframeAutoColor.h"
#include "GraphicsObject_TextureFlat.h"
#include "GraphicsObject_ColorNoTextureLight.h"
#include "GraphicsObject_TextureLight.h"
#include "GraphicsObject_Sprite.h"
#include "GraphicsObject_SpriteBox.h"

#include "AzulTime.h"
#include "AzulTimer.h"

#endif