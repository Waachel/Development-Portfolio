//-----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#include "PCSTreeForwardIterator.h"

namespace Azul
{
	PCSTreeForwardIterator::PCSTreeForwardIterator(PCSNode *rootNode)
	{
		this->current = rootNode;
		this->root = current;

	}

	PCSNode *PCSTreeForwardIterator::First()
	{
		PCSNode *tmp = this->root;

		this->current = tmp;
		return  this->current;
	}

	PCSNode *PCSTreeForwardIterator::Next()
	{
		PCSNode* tmp = this->current;
		PCSNode* back = tmp;

		PCSNode* child = tmp->GetChild();
		PCSNode* sibling = tmp->GetNextSibling();
		PCSNode* parent = tmp->GetParent();

		if (tmp->GetChild() == nullptr && tmp->GetParent() == nullptr && tmp->GetPrevSibling() == nullptr && tmp->GetNextSibling() == nullptr)
		{
			this->current = nullptr;
			return  this->current;
		}

		//has a child
		if (child != nullptr)
		{
			tmp = child;
		}
		//no children
		else
		{
			//has a sibling
			if (sibling != nullptr)
			{
				tmp = sibling;
			}

			//no siblings
			else
			{
				//find parent sibling
				while (parent != nullptr)
				{
					tmp = parent->GetNextSibling();

					//has siblings
					if (tmp != nullptr)
					{
						//found a sibling
						break;
					}

					//parent has no siblings, go to next parent
					else
					{
						parent = parent->GetParent();
					}
				}
			}
		}

		this->current = tmp;

		if (tmp == (PCSNode*)0xdddddddd)
		{
			this->current = nullptr;
			return this->current;
		}

		if (this->current != nullptr)
		{
			this->current->SetReverse(back);
		}
		
		return  this->current;
	}

	bool PCSTreeForwardIterator::IsDone()
	{
		return  (this->current == nullptr);
	}

	PCSNode *PCSTreeForwardIterator::Current()
	{
		return  this->current;
	}
}

// ---  End of File ---------------
