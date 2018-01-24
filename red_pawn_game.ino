#include <SD.h>
#include <ArduinoSound.h>

const int MAX_S_WAVS = 2;
const int MAX_D_WAVS = 2;
const String S_WAV_NAME = "s_";
const String D_WAV_NAME = "d_";
const String WAV_EXT = ".wav";
const String S_MODE_SEL = "s_sel";
const String D_MODE_SEL = "d_sel";
const String SD_MODE_SEL = "sd_sel";
const String STARTUP_SOUND = "start";
const String ERROR_SOUND = "error";

const int SD_PIN = 10;
const int PAWN_PIN = 13;
const int S_MODE_PIN = 12;
const int D_MODE_PIN = 11;
const int B_MODE_PIN = 6;

SDWaveFile waveFile;

bool playingAudio = false;
bool sMode = false;
bool dMode = false;

void setup() {
  Serial.begin(9600);
  delay(3000); // To give console enough time to connect
  randomSeed(analogRead(1));

  Serial.print("Initializing SD card...");
  if (!SD.begin(SD_PIN)) {
    Serial.println("ERROR: Initialization of SD card failed!");
    while(1);
  }
  Serial.println("Initialization of SD card done.");

  pinMode(PAWN_PIN, INPUT);
  pinMode(S_MODE_PIN, INPUT);
  pinMode(D_MODE_PIN, INPUT);
  pinMode(B_MODE_PIN, INPUT);

  checkSwitches(true);

  playWaveFile(STARTUP_SOUND);
}

void loop() {
  if (playingAudio && !AudioOutI2S.isPlaying()) {
    playingAudio = false;
    Serial.println("playback finished");
  }
  else if(playingAudio) {
    return; // Let the audio finish before doing anything else
  }
  
  checkPawn();

  if(!playingAudio) {
    checkSwitches(false);
  }
}

void checkPawn() {
  int playWhich = -1;

  // check if the Pawn was pressed
  if(digitalRead(PAWN_PIN)) {
    Serial.println("Pawn pressed!");
    if(sMode && dMode) {
      if(random(10) < 3) {
        playWhich = 1;
      }
      else {
        playWhich = 0;
      }
    }
    else if(sMode) {
      playWhich = 1;
    }
    else if(dMode) {
      playWhich = 0;
    }
  }

  if(playWhich == 0) {
    playWaveFile(D_WAV_NAME + random(MAX_D_WAVS));
  }
  else if(playWhich == 1) {
    playWaveFile(S_WAV_NAME + random(MAX_S_WAVS));
  }
}

void checkSwitches(bool init) {

  bool changed = false;
  if(digitalRead(B_MODE_PIN) && !(sMode && dMode)) {
    Serial.println("Both activated!");
    sMode = true;
    dMode = true;
    changed = true;
  }
  else if(sMode && dMode && !digitalRead(B_MODE_PIN)) {
    Serial.println("Dual mode deactivated");
    sMode = false;
    dMode = false;
    changed = true;
  }
  
  if(digitalRead(S_MODE_PIN) && !sMode) {
    Serial.println("S mode selected!");
    sMode = true;
    changed = true;
  }
  else if(digitalRead(D_MODE_PIN) && !dMode) {
    Serial.println("D mode selected!");
    dMode = true;
    changed = true;
  }

  if(!init && changed) {
    if(sMode && dMode) playWaveFile(SD_MODE_SEL);
    else if(sMode) playWaveFile(S_MODE_SEL);
    else if(dMode) playWaveFile(D_MODE_SEL);
    else {
      Serial.println("ERROR: No mode selected!");
      playWaveFile(ERROR_SOUND);
    }
  }
}

void playWaveFile(String filename) {
  Serial.println("Playing '" + filename + "'");
  waveFile = SDWaveFile(filename + WAV_EXT);
  if (!waveFile) {
    Serial.println("ERROR: Wave file '" + filename + "' is invalid!");
  }

  AudioOutI2S.volume(3);
  if (!AudioOutI2S.canPlay(waveFile)) {
    Serial.println("ERROR: Unable to play wave file '" + filename + "'!");
  }

  // non-blocking playback
  playingAudio = true;
  AudioOutI2S.play(waveFile);
}

