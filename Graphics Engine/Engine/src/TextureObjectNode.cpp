//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "TextureObjectNode.h"
#include "StringThis.h"

TextureObjectNode::TextureObjectNode()
    : DLink(),
    poTextureObject(nullptr)
{
    this->privClear();
}

TextureObjectNode::~TextureObjectNode()
{
    this->privClear();
}

void TextureObjectNode::Set(TextureObject *pTextureObject)
{
   assert(pTextureObject);
   this->poTextureObject = pTextureObject;

   this->poTextureObject->SetName(pTextureObject->name);
}

TextureObject * TextureObjectNode::GetTextureObject()
{
    return this->poTextureObject;
}

void TextureObjectNode::privClear()
{
    if(this->poTextureObject)
    {
        delete this->poTextureObject;
    }
    this->poTextureObject = nullptr;
}

char * TextureObjectNode::GetName()
{
    char *pName = nullptr;
    if(this->poTextureObject)
    {
        pName = this->poTextureObject->GetName();
    }

    return pName;
}

void TextureObjectNode::Wash()
{
    this->privClear();
}

bool TextureObjectNode::Compare(DLink *pTargetNode)
{
    // This is used in ManBase.Find() 
    assert(pTargetNode);

    TextureObjectNode *pDataB = (TextureObjectNode *) pTargetNode;

    bool status = false;

    assert(pDataB->poTextureObject);
    assert(this->poTextureObject);

    if(this->poTextureObject->name == pDataB->poTextureObject->name)
    {
        status = true;
    }

    return status;
}

void TextureObjectNode::Dump()
{
    Trace::out("      TextureObjectNode(%p)\n", this);

    // Data:
    if(this->poTextureObject)
    {
        Trace::out("      TextureObject(%p) \n", this->poTextureObject);
        Trace::out("      Name: %s \n", StringMe(this->poTextureObject->name));
    }
    else
    {
        Trace::out("      Name: %s \n", "null");
    }

    DLink::Dump();
}


// --- End of File ---
