//-----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#include "PCSTree.h"
#include "PCSNode.h"
#include <stdlib.h>

namespace Azul
{
	// constructor
	PCSNode::PCSNode()
	{
		pParent = NULL;
		pChild = NULL;
		pNextSibling = NULL;
		pPrevSibling = NULL;
		pForward = NULL;
		pReverse = NULL;
		for (unsigned int i = 0; i < NAME_SIZE; i++)
		{
			pName[i] = 0;
		}
		//AZUL_REPLACE_ME(0);
	}

	// copy constructor
	PCSNode::PCSNode(const PCSNode& in)
	{
		pParent = in.pParent;
		pChild = in.pChild;
		pNextSibling = in.pNextSibling;
		pPrevSibling = in.pPrevSibling;
		for (int i = 0; i < NAME_SIZE; i++)
		{
			this->pName[i] = in.pName[i];
		}
	}

	// Specialize Constructor
	PCSNode::PCSNode(PCSNode* const pInParent, PCSNode* const pInChild, PCSNode* const pInNextSibling, PCSNode* const pInPrevSibling, const char* const pInName)
	{
		pParent = pInParent;
		pChild = pInChild;
		pNextSibling = pInNextSibling;
		pPrevSibling = pInPrevSibling;
		for (int i = 0; i < NAME_SIZE; i++)
		{
			this->pName[i] = pInName[i];
		}
	}

	PCSNode::PCSNode(const char* const pInName)
	{
		pParent = NULL;
		pChild = NULL;
		pNextSibling = NULL;
		pPrevSibling = NULL;
		for (int i = 0; i < NAME_SIZE; i++)
		{
			this->pName[i] = pInName[i];
		}
	}

	// destructor
	PCSNode::~PCSNode()
	{
		//do nothing
	}

	// assignment operator
	PCSNode& PCSNode::operator = (const PCSNode& in)
	{
		this->pParent = in.pParent;
		this->pChild = in.pChild;
		this->pNextSibling = in.pNextSibling;
		this->pPrevSibling = in.pPrevSibling;
		for (int i = 0; i < NAME_SIZE; i++)
		{
			this->pName[i] = in.pName[i];
		}
		return *this;
	}

	void PCSNode::SetParent(PCSNode* const pIn)
	{
		this->pParent = pIn;
	}

	void PCSNode::SetChild(PCSNode* const pIn)
	{
		this->pChild = pIn;

	}

	void PCSNode::SetNextSibling(PCSNode* const pIn)
	{
		this->pNextSibling = pIn;
	}

	void PCSNode::SetPrevSibling(PCSNode* const pIn)
	{
		this->pPrevSibling = pIn;
	}

	void PCSNode::SetForward(PCSNode* const pIn)
	{
		this->pForward = pIn;
	}

	void PCSNode::SetReverse(PCSNode* const pIn)
	{
		this->pReverse = pIn;
	}

	PCSNode* PCSNode::GetParent(void) const
	{
		return this->pParent;
	}

	PCSNode* PCSNode::GetChild(void) const
	{
		return this->pChild;
	}

	PCSNode* PCSNode::GetNextSibling(void) const
	{
		return this->pNextSibling;
	}

	PCSNode* PCSNode::GetPrevSibling(void) const
	{
		return this->pPrevSibling;
	}

	PCSNode* PCSNode::GetForward(void) const
	{
		return this->pForward;
	}

	PCSNode* PCSNode::GetReverse(void) const
	{
		return this->pReverse;
	}

	PCSNode::Code PCSNode::SetName(const char* const pInName)
	{
		if (pInName == NULL)
		{
			return Code::FAIL_NULL_PTR;
		}

		for (int i = 0; i < NAME_SIZE - 1; i++)
		{
			this->pName[i] = pInName[i];
		}
		return Code::SUCCESS;
	}

	PCSNode::Code PCSNode::GetName(char* const pOutBuffer, unsigned int sizeOutBuffer) const
	{
		if (pOutBuffer == NULL)
		{
			return Code::FAIL_NULL_PTR;
		}

		if (sizeOutBuffer < NAME_SIZE)
		{
			for (unsigned int i = 0; i < sizeOutBuffer; i++)
			{
				if (i == sizeOutBuffer - 1)
				{
					pOutBuffer[i] = NULL;
					return Code::SUCCESS;
				}
				pOutBuffer[i] = this->pName[i];
			}
		}
		for (unsigned int i = 0; i < sizeOutBuffer; i++)
		{
			pOutBuffer[i] = this->pName[i];
		}
		return Code::SUCCESS;
	}

	void PCSNode::PrintNode() const
	{
		Trace::out2("   Node Addr: 0x%p\n", this);
		Trace::out2("   Node Name: %s\n", this->pName);
		if (this->pParent != NULL)
		{
			Trace::out2("      Parent: %s\n", this->pParent->pName);
		}
		else
		{
			Trace::out2("      Parent: %s\n", "NULL");
		}
		if (this->pNextSibling != NULL)
		{
			Trace::out2("Next Sibling: %s\n", this->pNextSibling->pName);
		}
		else
		{
			Trace::out2("Next Sibling: %s\n", "NULL");
		}

		if (this->pPrevSibling != NULL)
		{
			Trace::out2("Prev Sibling: %s\n", this->pPrevSibling->pName);
		}
		else
		{
			Trace::out2("Prev Sibling: %s\n", "NULL");
		}


		PCSNode* tmpNode = this->GetChild();
		if (tmpNode != NULL)
		{
			Trace::out2("       Child: %s\n", tmpNode->pName);
			tmpNode = tmpNode->GetNextSibling();
			while (tmpNode != NULL)
			{
				Trace::out2("       Child: %s\n", tmpNode->pName);
				tmpNode = tmpNode->GetNextSibling();
			}
		}

		else
		{
			Trace::out2("       Child: %s\n", "NULL");
		}

	}

	void PCSNode::PrintChildren() const
	{
		PCSNode* tmpNode = this->GetChild();
		if (tmpNode != NULL)
		{
			Trace::out2("       Child: %s\n", tmpNode->pName);
			tmpNode = tmpNode->GetNextSibling();
			while (tmpNode != NULL)
			{
				Trace::out2("       Child: %s\n", tmpNode->pName);
				tmpNode = tmpNode->GetNextSibling();
			}
		}

		else
		{
			Trace::out2("       Child: %s\n", "NULL");
		}
	}


	void PCSNode::PrintSiblings() const
	{
		PCSNode* tmpNode = this->GetPrevSibling();
		if (tmpNode != NULL)
		{
			while (tmpNode->GetPrevSibling() != NULL)
			{
				tmpNode = tmpNode->GetPrevSibling();
			}
		}

		else
		{
			Trace::out2("     Sibling: %s\n", this->pName);
			tmpNode = this->GetNextSibling();
		}

		while (tmpNode != NULL)
		{
			Trace::out2("     Sibling: %s\n", tmpNode->pName);
			tmpNode = tmpNode->GetNextSibling();
		}

	}

	int PCSNode::GetNumSiblings() const
	{
		//start with one because each node has itself
		int count = 1;
		PCSNode* tmpNode = this->GetPrevSibling();

		if (tmpNode == NULL)
		{
			//do nothing
		}
		else
		{
			//find first sibling
			while (tmpNode->GetPrevSibling() != NULL)
			{
				tmpNode = tmpNode->GetPrevSibling();
			}
		}

		if (tmpNode == NULL && this->GetNextSibling() == NULL)
		{
			//do nothing
		}
		else if (tmpNode == NULL)
		{
			tmpNode = this->GetNextSibling();
			while (tmpNode != NULL)
			{
				count++;
				tmpNode = tmpNode->GetNextSibling();
			}
		}
		else
		{
			tmpNode = tmpNode->GetNextSibling();
			while (tmpNode != NULL)
			{
				count++;
				tmpNode = tmpNode->GetNextSibling();
			}
		}

		return count;
	}

	int PCSNode::GetNumChildren() const
	{
		int count = 0;
		if (this->GetChild() == NULL)
		{
			//do nothing - no children
		}
		else
		{
			PCSNode* tmpNode = this->GetChild()->GetNextSibling();
			//it has atleast one child
			count++;

			//find remaining children
			while (tmpNode != NULL)
			{
				count++;
				tmpNode = tmpNode->GetNextSibling();
			}
		}

		return count;
	}
}

// ---  End of File ---
