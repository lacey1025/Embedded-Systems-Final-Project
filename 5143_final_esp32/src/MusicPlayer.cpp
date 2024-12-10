#include "headers/MusicPlayer.h"

uint8_t commandLength;
uint8_t command[6];
int16_t checkSum = 0;

void startMusicPlayer() {
  setEQ();
  setLoopMode();
  playTrack(1);
}

void sendCommand() {
  int q;
  for (q = 0; q < commandLength; q++) {
    Serial2.write(command[q]);
  }
}

void setEQ() {
  command[0] = 0xAA;
  command[1] = 0x1A;
  command[2] = 0x01;
  command[3] = 0x04;
  checkSum = 0;
  for (int q = 0; q < 4; q++) {
    checkSum +=  command[q];
  }
  command[4] = (checkSum & 0xFF);//SM check bit... low bit of the sum of all previous values
  commandLength = 5;
  sendCommand();
}

void setLoopMode() {
  command[0] = 0xAA;
  command[1] = 0x18;
  command[2] = 0x01;
  command[3] = 0x01;
  checkSum = 0;
  for (int q = 0; q < 4; q++) {
    checkSum += command[q];
  }
  command[4] = (checkSum & 0xFF);
  commandLength = 5;
  sendCommand();
}

void playTrack(int soundTrack) {
  //select track
  command[0] = 0xAA; //first byte says it's a command
  command[1] = 0x07;
  command[2] = 0x02;
  command[3] = (soundTrack>>8) & 0xFF; //snh...track HIGH bit
  command[4] = soundTrack & 0xFF; //SNL... track low bit
  checkSum = 0;
  for (int q = 0; q < 5; q++) {
    checkSum +=  command[q];
  }
  command[5] = (checkSum & 0xFF); //SM check bit... low bit of the sum of all previous values
  
  commandLength = 6;
  
  sendCommand();
}

void stopPlaying() {
  command[0] = 0xAA;
  command[1] = 0x04;
  command[2] = 0x00;
  command[3] = 0xAE;
  commandLength = 4;
  sendCommand();
}