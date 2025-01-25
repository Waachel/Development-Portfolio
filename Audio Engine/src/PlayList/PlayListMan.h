//----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef Playlist_MAN_H
#define Playlist_MAN_H 

#include "ManBase.h"
#include "DLinkMan.h"
#include "Snd.h"
#include "Playlist.h"
#include "Voice.h"
#include "Wave.h"

class PlaylistMan : public ManBase
{

    //----------------------------------------------------------------------
    // Constructor
    //----------------------------------------------------------------------
private:
    PlaylistMan(int reserveNum = 3, int reserveGrow = 1);
    ~PlaylistMan();

    //----------------------------------------------------------------------
    // Static Methods
    //----------------------------------------------------------------------
public:
    static void Create(int reserveNum = 3, int reserveGrow = 1);
    static void Destroy();

    static Playlist *Add(Snd::ID snd_id, Wave::ID wave_id, VoiceCallback *pCallback);
    static Playlist *Find(Snd::ID snd_id);

    static void Remove(Playlist *pNode);
    static void Dump();

    //----------------------------------------------------------------------
    // Private methods
    //----------------------------------------------------------------------
private:
    static PlaylistMan *privGetInstance();

    //----------------------------------------------------------------------
    // Override Abstract methods
    //----------------------------------------------------------------------
protected:
    DLink *derivedCreateNode() override;


    //----------------------------------------------------------------------
    // Data: unique data for this manager 
    //----------------------------------------------------------------------
private:
    Playlist *poNodeCompare;
    static PlaylistMan *posInstance;

};


#endif