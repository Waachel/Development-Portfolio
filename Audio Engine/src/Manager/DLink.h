//----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef DLINK_H
#define DLINK_H

class DLink 
{
public:
    DLink();

    // Becomes optional with a virtual with default implementation
    virtual char *GetName();

    // Abstract methods - must be implemented
    virtual void Wash() = 0;
    virtual bool Compare(DLink *pTargetNode) = 0;

    void Clear();
    virtual void Dump();


    // Data: -----------------------------
    DLink *pNext;
    DLink *pPrev;

};


#endif

// --- End of File ---