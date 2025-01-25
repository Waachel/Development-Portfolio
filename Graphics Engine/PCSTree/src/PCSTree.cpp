//-----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#include "PCSTree.h"
#include "PCSNode.h"
using namespace std;

namespace Azul
{

	// constructor
	PCSTree::PCSTree()
	{
		mInfo.maxNumNodes = 0;
		mInfo.currNumNodes = 0;
		mInfo.currNumLevels = 0;
		mInfo.maxNumLevels = 0;
		pRoot = NULL;
	};

	// destructor
	PCSTree::~PCSTree()
	{
		AZUL_REPLACE_ME(0);
	};


	// get Root
	PCSNode* PCSTree::GetRoot() const
	{
		return this->pRoot;
	}


	// insert
	void PCSTree::Insert(PCSNode* const pInNode, PCSNode* const pParent)
	{
		if (this->pRoot == NULL)
		{
			this->pRoot = pInNode;
		}

		PCSNode* tmpNode;
		//PCSNode* parentNode = NULL;
		pInNode->SetParent(pParent);

		//check if pInNode has siblings
		if (pParent != NULL)
		{
			if (pParent->GetChild() == NULL)
			{
				pParent->SetChild(pInNode);
				mInfo.currNumNodes++;
				mInfo.maxNumNodes++;
			}

			else
			{
				tmpNode = pParent->GetChild();

				pInNode->SetNextSibling(tmpNode);
				tmpNode->SetPrevSibling(pInNode);
				pParent->SetChild(pInNode);
				mInfo.currNumNodes++;
				mInfo.maxNumNodes++;

			}
		}
		else
		{
			mInfo.currNumNodes++;
			mInfo.maxNumNodes++;
			mInfo.currNumLevels++;
			mInfo.maxNumLevels++;
		}

	}


	// Remove
	void PCSTree::Remove(PCSNode* const pInNode)
	{
		if (pRoot == NULL)
		{
			//do nothing, tree is empty
			return;
		}

		//no children
		if (pInNode->GetChild() == NULL)
		{
			//no siblings
			if (pInNode->GetNextSibling() == NULL && pInNode->GetPrevSibling() == NULL)
			{
				if (pInNode->GetReverse() != nullptr)
				{
					pInNode->GetReverse()->SetForward(nullptr);
				}
				if (pInNode->GetParent() != NULL)
				{
					pInNode->GetParent()->SetChild(NULL);
				}
				if (pInNode->GetParent() == NULL)
				{
					pInNode->SetForward(nullptr);
				}

				pInNode->SetParent(NULL);
				mInfo.currNumNodes--;
				return;
			}

			//last sibling
			else if (pInNode->GetNextSibling() == NULL && pInNode->GetPrevSibling() != NULL)
			{
				if (pInNode->GetReverse() != nullptr)
				{
					pInNode->GetReverse()->SetForward(nullptr);
				}

				pInNode->GetPrevSibling()->SetNextSibling(NULL);
				pInNode->SetNextSibling(NULL);
				pInNode->SetPrevSibling(NULL);
				pInNode->SetParent(NULL);
				mInfo.currNumNodes--;
				return;
			}

			//first sibling 
			else if (pInNode->GetNextSibling() != NULL && pInNode->GetPrevSibling() == NULL)
			{
				pInNode->GetNextSibling()->SetPrevSibling(NULL);
				pInNode->GetParent()->SetChild(pInNode->GetNextSibling());
				pInNode->SetNextSibling(NULL);
				pInNode->SetPrevSibling(NULL);
				pInNode->SetParent(NULL);
				mInfo.currNumNodes--;
				return;
			}

			//middle has siblings on both sides
			else
			{
				pInNode->GetNextSibling()->SetPrevSibling(pInNode->GetPrevSibling());
				pInNode->GetPrevSibling()->SetNextSibling(pInNode->GetNextSibling());
				pInNode->SetNextSibling(NULL);
				pInNode->SetPrevSibling(NULL);
				pInNode->SetParent(NULL);
				mInfo.currNumNodes--;
				return;
			}
		}

		//has children
		for (int i = 0; i < pInNode->GetNumChildren(); i++)
		{
			Remove(pInNode->GetChild());
			Remove(pInNode);
		}
	}

	// get tree info
	void PCSTree::GetInfo(Info& info)
	{
		info.currNumNodes = mInfo.currNumNodes;
		info.maxNumNodes = mInfo.maxNumNodes;
		mInfo.currNumLevels = MaxDepth(pRoot);
		if (mInfo.currNumNodes == 0)
		{
			info.currNumLevels = 0;
		}
		else
		{
			info.currNumLevels = mInfo.currNumLevels;
		}
		if (mInfo.currNumLevels > mInfo.maxNumLevels)
		{
			mInfo.maxNumLevels = mInfo.currNumLevels;
		}
		else
		{
			//no change
		}
		info.maxNumLevels = mInfo.maxNumLevels;
	}

	void PCSTree::Print()
	{
		PCSNode* start = this->GetRoot();
		PrintList(start);

	}

	void PCSTree::PrintList(PCSNode* const start)
	{
		if (start == NULL)
		{
			return;
		}

		start->PrintNode();
		Trace::out2("------------------\n", this);

		PrintList(start->GetChild());
		PrintList(start->GetNextSibling());
	}

	int PCSTree::MaxDepth(PCSNode* const root)
	{
		if (root == NULL)
		{
			return 0;
		}

		int max_depth = 1;
		PCSNode* tmpNode = NULL;
		for (int i = 0; i < root->GetNumChildren(); i++) {
			if (i == 0)
			{
				tmpNode = root->GetChild();
			}
			else
			{
				tmpNode = tmpNode->GetNextSibling();
			}
			max_depth = max(max_depth, 1 + MaxDepth(tmpNode));
		}
		return max_depth;

	}

}

// ---  End of File ---
