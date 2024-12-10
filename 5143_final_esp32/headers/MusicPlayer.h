/* Controls the DY-SV5W music player device using UART communication */
#ifndef MUSIC_PLAYER.h
#define MUSIC_PLAYER.h
#include <cstdint>
#include "Data.h"

// On startup, sets the tracks to loop, EQ to classical, and begins track 1
void startMusicPlayer();

// Send a single command to the music player
void sendCommand();

// Sends command to set the EQ to classical for better sound quality
void setEQ();

// Sends the command to loop tracks when they complete
void setLoopMode();

// Sends the command to play a specified music track
void playTrack(int soundTrack);

// Stops all tracks from playing
void stopPlaying();

#endif
