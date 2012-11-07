/******************************************************************
*  Sparkfun Electronics MP3 Shield Library
*		details and example sketch: 
*			http://www.billporter.info/?p=1270
*
*		Brought to you by:
*              Bill Porter
*              www.billporter.info
*
*		See Readme for other info and version history
*	
*  
*This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or(at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
<http://www.gnu.org/licenses/>
*
*This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
*To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or
*send a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.
******************************************************************/
#ifndef SFEMP3Shield_h
#define SFEMP3Shield_h

// inslude the SPI library:
#include "SPI.h"



//Not neccessary, but just in case. 
#if ARDUINO > 22
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

//Add the SdFat Libraries
#include <SdFat.h>
#include <SdFatUtil.h> 



static void refill();
enum flush_m {post, pre, both, none};
static void	flush_cancel(flush_m);

void Mp3WriteRegister(uint8_t, uint8_t, uint8_t);
void Mp3WriteRegister(uint8_t, uint16_t);
uint16_t Mp3ReadRegister (uint8_t);
uint16_t Mp3ReadWRAM (uint16_t);

//Create the variables to be used by SdFat Library
extern Sd2Card card;
extern SdVolume volume;
extern SdFile root;
extern SdFile track;
extern uint8_t playing;

//buffer for music
extern uint8_t mp3DataBuffer[32];

//MP3 Player Shield pin mapping. See the schematic
#define MP3_XCS 6 //Control Chip Select Pin (for accessing SPI Control/Status registers)
#define MP3_XDCS 7 //Data Chip Select / BSYNC Pin
#define MP3_DREQ 2 //Data Request Pin: Player asks for more data
#define MP3_DREQINT 0 //Corresponding INTx for DREQ pin
#define MP3_RESET 8 //Reset is active low
#define SD_SEL 9 //select pin for SD card

//VS10xx SCI Registers
#define SCI_MODE 0x00
#define SCI_STATUS 0x01
#define SCI_BASS 0x02
#define SCI_CLOCKF 0x03
#define SCI_DECODE_TIME 0x04
#define SCI_AUDATA 0x05
#define SCI_WRAM 0x06
#define SCI_WRAMADDR 0x07
#define SCI_HDAT0 0x08
#define SCI_HDAT1 0x09
#define SCI_AIADDR 0x0A
#define SCI_VOL 0x0B
#define SCI_AICTRL0 0x0C
#define SCI_AICTRL1 0x0D
#define SCI_AICTRL2 0x0E
#define SCI_AICTRL3 0x0F

//VS10xx SCI_MODE bitmasks
#define SM_DIFF           0x0001
#define SM_LAYER12        0x0002
#define SM_RESET          0x0004
#define SM_CANCEL         0x0008
#define SM_EARSPEAKER_LO  0x0010
#define SM_TESTS          0x0020
#define SM_STREAM         0x0040
#define SM_EARSPEAKER_HI  0x0080
#define SM_DACT           0x0100
#define SM_SDIORD         0x0200
#define SM_SDISHARE       0x0400
#define SM_SDINEW         0x0800
#define SM_ADPCM          0x1000
//#define Not Defined     0x2000
#define SM_LINE1          0x4000
#define SM_CLK_RANGE      0x8000

// Extra Parameter in X memory (refer to p.58 of the datasheet)
#define para_chipID_0                               0x1E00
#define para_chipID_1                               0x1E01
#define para_version                                0x1E02
#define para_config1                                0x1E03
#define para_playSpeed                              0x1E04
#define para_byteRate                               0x1E05
#define para_endFillByte                            0x1E06
//
#define para_positionMsec_0                         0x1E27
#define para_positionMsec_1                         0x1E28
#define para_resync                                 0x1E29
#define TRUE  1
#define FALSE  0

//tag location offsets
#define TRACK_TITLE 3
#define TRACK_ARTIST 33
#define TRACK_ALBUM 63

/** Types of audio streams
 *
 */
enum AudioType
{
    WAV,                        /*!< WAVE audio stream */
    AAC,                        /*!< AAC audio stream (ADTS (.aac), MPEG2 ADIF (.aac) and MPEG4 AUDIO (.mp4 / .m4a / .3gp / .3g2)) */
    WMA,                        /*!< Windows Media Audio (WMA) stream */
    MIDI,                       /*!< Midi audio stream */   
    OGG_VORBIS,                 /*!< Ogg Vorbis audio stream */
    MP3,                        /*!< MPEG Audio Layer */
    UNKNOWN                     /*!< Unknown */
};
 
typedef enum AudioType AudioType;
 
/** Types of MPEG Audio Layer stream IDs.
 *
 */
enum MP3_ID
{
    MPG2_5a  = 0,               /*!< MPG 2.5, nonstandard, proprietary */
    MPG2_5b  = 1,               /*!< MPG 2.5, nonstandard, proprietary */
    MPG2_0   = 2,               /*!< ISO 13818-3 MPG 2.0 */
    MPG1_0   = 3                /*!< ISO 11172-3 MPG 1.0 */
};
 
typedef enum MP3_ID MP3_ID;
 
/** Types of MPEG Audio Layer channel modes.
 *
 */
enum MP3_MODE
{
    STEREO       = 0,           /*!< Stereo */
    JOINT_STEREO = 1,           /*!< Joint Stereo */
    DUAL_CHANNEL = 2,           /*!< Dual Channel */
    MONO         = 3            /*!< Mono */
};
 
typedef enum MP3_MODE MP3_MODE;
/** Struct for informations about audio streams.
 *
 */
typedef struct AudioInfo
{
   AudioType            type            : 4;        /*!< Type of the audio stream - important for the interpretation of the lower union */
   unsigned short       kBitRate;                   /*!< Average bitrate of the audio stream - in kBit/s */
   unsigned short       decodeTime;                 /*!< Decode time */
   union {
        struct {
            MP3_ID      id              : 2;        /*!< ID */
            char        layer           : 2;        /*!< Layer */
            char        protrectBit     : 1;        /*!< Protect bit, see p.44 of the datasheet */
            char        padBit          : 1;        /*!< Pad bit, see p.44 of the datasheet */
            MP3_MODE    mode            : 2;        /*!< Channel mode */
            char        extension       : 2;        /*!< Extension, see p.44 of the datasheet */
            char        copyright       : 1;        /*!< Copyright, see p.44 of the datasheet */
            char        original        : 1;        /*!< Original, see p.44 of the datasheet */
            char        emphasis        : 2;        /*!< Emphasis, see p.44 of the datasheet */
            char        kSampleRate     : 6;        /*!< Samplerate - in kHz (rounded) */
        } mp3;                                      /*!< MPEG Audio Layer */
        struct {
         
        } wma;                                      /*!< Windows Media Audio (WMA) stream */
        struct {
         
        } aac;                                      /*!< AAC audio stream */
        struct {
         
        } other;                                    /*!< Other */
   } ext;
    
} AudioInfo;

class SFEMP3Shield {
public:
uint8_t begin();
void SetVolume(uint8_t, uint8_t);
uint8_t playTrack(uint8_t);
uint8_t playMP3(char*);
void trackTitle(char*);
void trackArtist(char*);
void trackAlbum(char*);
void stopTrack();
uint8_t isPlaying();
bool skipTo(uint32_t);
uint32_t currentPosition();
void setBitRate(uint16_t);
void pauseDataStream();
void resumeDataStream();
AudioInfo AInfo;

private:
void getAudioInfo();
void getTrackInfo(uint8_t, char*);
uint8_t bitrate;
uint32_t start_of_music;
uint8_t VolL;
uint8_t VolR;
uint8_t VSLoadUserCode(char*);
};

char* strip_nonalpha_inplace(char *s);

union twobyte {
	uint16_t word;
	uint8_t  byte[2];
} ;	

#endif