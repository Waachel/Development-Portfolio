//-----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#include "PCSTreeReverseIterator.h"
#include <PCSTreeForwardIterator.h>

namespace Azul
{
	PCSTreeReverseIterator::PCSTreeReverseIterator(PCSNode *rootNode)
	{
		//go forward to find last node
		PCSTreeForwardIterator pForIter(rootNode);
		this->forRoot = rootNode;
		this->current = rootNode;
		this->root = current;
		this->prevNode = nullptr;

		PCSNode* lastNode = this->root;

		// Initilize the reverse pointer
		PCSNode* tmp = pForIter.First();

		while (!pForIter.IsDone())
		{
			if (this->root->GetChild() == nullptr && this->root->GetParent() == nullptr && this->root->GetPrevSibling() == nullptr && this->root->GetNextSibling() == nullptr)
			{
				break;
			}
			lastNode = tmp;

			tmp = pForIter.Next();
		}

		this->current = lastNode;
		this->root = current;
	}

	PCSNode *PCSTreeReverseIterator::First()
	{
		PCSNode* tmp = this->root;

		this->current = tmp;
		return  this->current;
	}

	PCSNode *PCSTreeReverseIterator::Next()
	{
		//save spot
		PCSNode* saveCurrent = this->current;
		PCSNode* savePrev = this->current;

		//go forward to find last node
		PCSTreeForwardIterator pForIter(this->forRoot);
		
		PCSNode* tmp = this->forRoot;
		if (saveCurrent == tmp)
		{
			return this->current = nullptr;
		}

		while (tmp != saveCurrent)
		{
			savePrev = tmp;
			tmp = pForIter.Next();
		}
		this->current = savePrev;

		if (this->current != nullptr)
		{
			this->current->SetForward(saveCurrent);
		}

		return  this->current;
	}

	bool PCSTreeReverseIterator::IsDone()
	{
		return  this->current == nullptr;
	}

	PCSNode *PCSTreeReverseIterator::Current()
	{
		return  this->current;
	}

}

// ---  End of File ---------------
