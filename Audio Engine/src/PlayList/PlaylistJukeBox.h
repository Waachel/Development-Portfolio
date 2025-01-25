//----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef Playlist_JUKEBOX_H
#define Playlist_JUKEBOX_H 

#include "ManBase.h"
#include "DLinkMan.h"
#include "Snd.h"
#include "Playlist.h"
#include "Voice.h"
#include "Wave.h"
#include "PlaylistData.h"

class PlaylistJukeBox : public ManBase
{

    //----------------------------------------------------------------------
    // Constructor
    //----------------------------------------------------------------------
private:
    PlaylistJukeBox(int reserveNum = 3, int reserveGrow = 1);
    ~PlaylistJukeBox();

    //----------------------------------------------------------------------
    // Static Methods
    //----------------------------------------------------------------------
public:
    static void Create(int reserveNum = 3, int reserveGrow = 1);
    static void Destroy();

    static void Load(Snd::ID snd_id, PlaylistData *pPlaylistData);
    static Playlist *Add(Snd::ID snd_id, Wave::ID wave_id, VoiceCallback *pCallback);
    static Playlist *Find(Snd::ID snd_id);

    static void Remove(Playlist *pNode);
    static void Dump();

    //----------------------------------------------------------------------
    // Private methods
    //----------------------------------------------------------------------
private:
    static PlaylistJukeBox *privGetInstance();

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
    static PlaylistJukeBox *posInstance;

};


#endif