#include <SD.h>
#include <ArduinoSound.h>

const int MAX_S_WAVS = 2;
const int MAX_D_WAVS = 2;
const String S_WAV_NAME = "s_";
const String D_WAV_NAME = "d_";
const String WAV_EXT = ".wav";
const String S_MODE_SEL = "s_mode_sel";
const String D_MODE_SEL = "d_mode_sel";
const String SD_MODE_SEL = "sd_mode_sel";

const int SD_PIN = 10;

SDWaveFile waveFile;

bool playingAudio = false;
bool sMode = false;
bool dMode = false;

void setup() {
  Serial.begin(9600);
  delay(3000); // To give console enough time to connect

  Serial.print("Initializing SD card...");
  if (!SD.begin(SD_PIN)) {
    Serial.println("ERROR: Initialization of SD card failed!");
    while(1);
  }
  Serial.println("Initialization of SD card done.");

  checkSwitches(true);
}

void loop() {
  if (playingAudio && !AudioOutI2S.isPlaying()) {
    playingAudio = false;
    Serial.println("playback finished");
  }
  
  checkPawn();

  checkSwitches();
}

void checkPawn() {

  // check if the Pawn was pressed
}

void checkSwitches(bool init) {
  if(playingAudio) {
    return;
  }

  // check if a switch changed

  if(!init) {
    if(sMode && dMode) playWaveFile(SD_MODE_SEL);
    else if(sMode) playWaveFile(S_MODE_SEL);
    else if(dMode) playWaveFile(D_MODE_SEL);
    else {
      Serial.println("ERROR: No mode selected!");
      while(1);
    }
  }
}

void playWaveFile(String filename) {
  waveFile = SDWaveFile(filename);
  if (!waveFile) {
    Serial.println("ERROR: Wave file '" + filename + "' is invalid!");
    while (1); // do nothing
  }

  AudioOutI2S.volume(5);
  if (!AudioOutI2S.canPlay(waveFile)) {
    Serial.println("ERROR: Unable to play wave file '" + filename + "'!");
    while (1); // do nothing
  }

  // non-blocking playback
  AudioOutI2S.play(waveFile);
}

