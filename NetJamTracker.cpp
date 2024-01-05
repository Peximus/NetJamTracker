#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <cstdint>
#include <cstdio>
#include <math.h>
#include <windowsx.h>
#include <Commctrl.h>
#include <commdlg.h>
#include <shobjidl.h>
#include <Shlwapi.h>
#include <mmreg.h>
#include "portaudio.h"
#include "resource.h"
#include <mmsystem.h>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <stdio.h>

#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "Comctl32.lib")
#pragma comment(lib, "ws2_32.lib")
//#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")


#define NETWORK_ERROR -1
#define NETWORK_OK     0
#define WINSOCK_CALL   WM_USER + 100

#define DEFAULT_PORT "27015"

#define IDT_TIMER1 1000
#define IDT_TIMER2 1001



int rc;

SOCKET theSocket;

HWND hwnd_tcp;

int size = 0;
int position = 0;

#define MAX_LOADSTRING 100

#define TABLE_SIZE   (200)

#define M_PI  (3.14159265)

#define NUM_SECONDS   (5)
#define SAMPLE_RATE   (44100)
#define FRAMES_PER_BUFFER  (512)

#pragma pack(push,1)
struct WaveHeaderType
{
    char chunkId[4];
    unsigned long chunkSize;
    char format[4];
    char subChunkId[4];
    unsigned long subChunkSize;
    unsigned short audioFormat;
    unsigned short numChannels;
    unsigned long sampleRate;
    unsigned long bytesPerSecond;
    unsigned short blockAlign;
    unsigned short bitsPerSample;
    char dataChunkId[4];
    unsigned long dataSize;
};
#pragma pack(pop)


WaveHeaderType waveFileHeader;

WaveHeaderType PlayFormat;

WAVEFORMATEX waveFormat;

int16_t* waveData;
float* waveFloat[32];
int bufferSize[32];
int octave = 63;
float vib_wav[16][1024];
int o = 0;
HWND hWndComboParam;
int vibindex = 0;

int effectindex = 0;
int paramindex = 0;

bool piano = true;
bool play_preview = false;
bool apreggio = false;
bool apreggio_preview = false;
bool vibactive = false;
bool vib_preview = false;
bool pitch_note = false;
bool pitch_preview = false;


float start_period = 0;
float porta_value = 0;

int apreg_note = 0;
int apreg_param = 0;


int ticks = 0;
int tickcount = 0;
int tick_speed = 882;
int period = 0;


float osc1[128];
float osc2[128];
bool osc_preview = false;
float two_pi;
int index = 0;
bool osc_mix = false;

int peak = 100;
float peakfloat = 0;
int peak_count = 0;
bool onoff = true;
COLORREF color = 0x0000FFFF;
HBRUSH brush;

bool sine_rad = true;
bool saw_rad = false;
bool tri_rad = false;

int vib_len = 0;
float radians = 0.1;
float amplitude[17] = { 0 }; // Amplitude of the sine wave
float frequency[17] = { 0 }; // Frequency of the sine wave
float phase = 0.0; // Phase of the sine wave
float freqdiv = 100.0f;
float ampdiv = 100.0f;
float angle = 0;
int old = 50;

//SCROLLINFO si;


typedef struct
{
    float sine[TABLE_SIZE];
    int left_phase;
    int right_phase;
    char message[20];
}
paTestData;



PaStreamParameters outputParameters;
PaStream* stream;
PaError err;
paTestData data;
const PaDeviceInfo* deviceInfo;

struct Config
{
    UINT midiport;

};


Config config;

DWORD_PTR nMidiPort = 0;

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    ChildWndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    ChildWndProc2(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    ChildWndProcPeak(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    VIB(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    Setup(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    TCP(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    OSC(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    DONATE(HWND, UINT, WPARAM, LPARAM);



RECT rect;
RECT play_mask;
RECT pitchslide;
RECT up_mask;
RECT down_mask;
RECT button1_mask;
RECT button2_mask;
RECT stop_mask;
RECT chan_mask;
RECT pat_mask_up;
RECT pat_mask_down;
RECT len_mask_up;
RECT len_mask_down;
RECT vibtoggle;

RECT drum_bpm_up;
RECT drum_bpm_down;

RECT drum_pat_up;
RECT drum_pat_down;

RECT drum_len_up;
RECT drum_len_down;


HWND hWndMain;
static HWND hChildWindow1;
static HWND hChildWindow2;
HWND hWndComboBox;
HWND hWndCombo;
HWND hWndCombo2;
HWND hwndTrack[16];
HWND listbox;
LRESULT pos;
HWND hChildPeak;
HWND hWndPitch;
HWND hWndMidi;

int ItemIndex = 0;

POINT point;
POINT point_main;

TCHAR str[256] = TEXT("\0");

char txt[100];
int textlength;
int slider = 50;
int xc = 219;
int yc = 180;
int xcount = 0;
int xNewPos = 0;
int xOldPos = 0;
int xstart = 0;
int xNewPos2 = 0;


int xorig, yorig;
int findex = 0;
int findex2 = 0;
int ychan = 200;

int psel = 0;

TCHAR samp_files[32][256];
TCHAR samp_files_dir[32][256];




int xPos = 0;
int yPos = 0;

int xcur = 0;
int ycur = 0;

int x = 0;
int y = 0;


int xlock;
int ylock;
int clicks = 0;
bool hscroll = false;
int hxregion = 0;
bool first = true;

HBITMAP piano_note;
HBITMAP piano_grid;
HBITMAP play;
HBITMAP stop;
HBITMAP circle;
HBITMAP pitchblank;
HBITMAP up;
HBITMAP down;
HBITMAP matrix;
HBITMAP unlit;
HBITMAP lit;
HBITMAP pianoroll;
HBITMAP button1;
HBITMAP chan_bitmap;
HBITMAP chan_blank;
HBITMAP patblt;
HBITMAP bpm_bmp;
HBITMAP pat_bmp;
HBITMAP len_bmp;
HBITMAP togoff;
HBITMAP togon;
HBITMAP vibnote;
HBITMAP pitchnote;
HBITMAP appregnote;

int piano_pat[128][24][128] = { 0 };
int piano_effects[128][24][128] = { 0 };
int effect_param[128][24][128] = { 0 };
int cur_pat = 0;
int cur_note = 0;
int pot[128];
int num_pat = 0;
int song_position = 0;
int len[128] = { 0 };
int bpm[128];

float ji[16] = { 0 };
float jinc[16] = { 0 };
bool active_note[16] = { false };
float decay[16] = { 1.0f };
bool effect_active[16] = { false };
int num_chan = 16;
int samples_beat = 88200;
int sample_count = 0;
int cur_chan = 0;
float output = 0;


int drum_pat[128][8][128] = { 0 };
bool drum_active[8] = { false };
float xi[8] = { 0 };
float xinc[8] = { 0 };
int drum_beat = 44100;
int drum_count = 0;
int drum_pos = 0;
int drum_pattern = 0;
int dlen[128] = { 0 };
int dbpm[128] = { 0 };
int dot[128];


bool playing = false;
bool playing_mix = false;
bool midi_data = false;
bool drums = false;


float note;
float dinc[32][32] = { 0 };
float si[32][32] = { 0 };
bool note_active[32][8] = { false };
static int chan = 0;
static float vinc = 0;
static float delta = 0;
int num_notes = 8;
int j[8] = { 0 };
bool chan_active[16] = { false };



unsigned char midi_note = 0;
unsigned char midi_status = 0;
unsigned char velocity;
unsigned char midi_count = 0;

unsigned char midi_chan[10] = { 0 };
float midi_delta[10] = { 0 };
float minc[10] = { 0 };
int vx = 0;

int numberoftimes = 0;
bool good = false;

bool LoadWaveFile (PWSTR filename, int findex);
float Get_Note(unsigned char note);
float Get_Period(unsigned char note);
float Get_Frequency(float period);
bool Save();
bool Load();
void start_pattern();
bool play_snd(int sindex);
void Init_PortAudio();
bool InitMidi();
void CALLBACK MidiInProc(HMIDIIN hMidiIn, UINT wMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2);
bool LoadConfig();
bool WriteConfig();
void start_pattern_mix();
void init_pot();
void init_vib();
void start_vib();
void start_drums();


static int patestCallback(const void* inputBuffer, void* outputBuffer,
    unsigned long framesPerBuffer,
    const PaStreamCallbackTimeInfo* timeInfo,
    PaStreamCallbackFlags statusFlags,
    void* userData)
{
    paTestData* data = (paTestData*)userData;
    float* out = (float*)outputBuffer;
    unsigned long i;

    (void)timeInfo; /* Prevent unused variable warnings. */
    (void)statusFlags;
    (void)inputBuffer;


    for (i = 0; i < framesPerBuffer; i++)
    {

        output = 0;


        if (osc_preview)
        {

            output = osc2[index];

            if (osc_mix)
            {
                output += osc1[index];
            }

            index++;

            if (index >= 128) index = 0;

        }



        if (midi_data)
        {

            for (int c = 0; c < 10; c++)
            {

                if (midi_chan[c] == 0) continue;

                int di = (int)minc[c];

                float x = minc[c] - (int)minc[c];

                output += waveFloat[cur_chan][di] + (waveFloat[cur_chan][di + 1] - waveFloat[cur_chan][di]) * x;

                minc[c] += midi_delta[c];

                if (di > bufferSize[cur_chan])
                {
             
                    midi_chan[c] = 0;
                }
            }
            
        }

        if (play_preview)
        {

            float x = vinc - (int)vinc;

            vinc += delta;

            int di = (int)vinc;

            output = waveFloat[cur_chan][di] + (waveFloat[cur_chan][di + 1] - waveFloat[cur_chan][di]) * x;

            if (di > bufferSize[cur_chan])
            {
                play_preview = false;
            }


        }


        if (vib_preview)
        {

            float x = vinc - (int)vinc;

            vinc += delta;

            int di = (int)vinc;

            output = waveFloat[cur_chan][di] + (waveFloat[cur_chan][di + 1] - waveFloat[cur_chan][di]) * x;

            output *= vib_wav[1][o];

            o++;

            if (o > vib_len) o = 0;

            if (di > bufferSize[cur_chan])
            {
                
                vinc = 0;
                vib_preview = false;
            }
        }


        if (pitch_preview)
        {

            tickcount++;

            if (tickcount >= tick_speed)
            {
                tickcount = 0;

                start_period++;

                delta = Get_Frequency(start_period) / SAMPLE_RATE;

            }

            float x = vinc - (int)vinc;

            vinc += delta;

            int di = (int)vinc;

            output = waveFloat[cur_chan][di] + (waveFloat[cur_chan][di + 1] - waveFloat[cur_chan][di]) * x;

            if (di > bufferSize[cur_chan])
            {
                pitch_preview = false;
            
                tickcount = 0;
            
            }


        }


        if (apreggio_preview)
        {

            tickcount++;

            if (tickcount >= tick_speed)
            {
                tickcount = 0;

                ticks++;

                if (ticks >= 6) ticks = 0;


                switch (ticks % 3)
                {

                case 0:

                    

                    note = Get_Note(apreg_note);

                    delta = note / SAMPLE_RATE;

                    break;

                case 1:

                    note = Get_Note(apreg_note + HIBYTE(apreg_param));

                    delta = note / SAMPLE_RATE;

                    break;

                case 2:

                    note = Get_Note(apreg_note + LOBYTE(apreg_param));

                    delta = note / SAMPLE_RATE;

                    break;


                }


            }
            
            

            float x = vinc - (int)vinc;

            vinc += delta;

            int di = (int)vinc;

            output = waveFloat[cur_chan][di] + (waveFloat[cur_chan][di + 1] - waveFloat[cur_chan][di]) * x;

            if (di > bufferSize[cur_chan])
            {
                apreggio_preview = false;

                tickcount = 0;

            }


        }


        if (playing)
        {


            for (int chan = 0; chan < num_chan; chan++)
            {

                if (active_note[chan] == false) continue;

                float x = ji[chan] - (int)ji[chan];

                int di = (int)ji[chan];

                float lerp = waveFloat[chan][di] + (waveFloat[chan][di + 1] - waveFloat[chan][di]) * x;

                if (sample_count > (samples_beat - 100) && piano_pat[cur_pat][chan][cur_note + 1] != 0)
                {
                    lerp *= decay[chan];

                    decay[chan] -= 0.01f;
                }

                if (effect_active[chan] == true)
                {


                    switch (piano_effects[cur_pat][chan][cur_note])
                    {

                    case 4:
                    {
                        int vibvalue = piano_effects[cur_pat][chan][cur_note];

                        lerp *= vib_wav[vibindex][o];

                        o++;

                        if (o > vib_len) o = 0;

                    }
                    break;

                    
                    case 3:
                    {
                        //down

                    }
                    break;

                    }

                    tickcount++;

                    if (tickcount >= tick_speed)
                    {
                        tickcount = 0;
                        ticks++;

                        switch (piano_effects[cur_pat][chan][cur_note])
                        {

                        case 2:
                        {
                                start_period++;

                                jinc[chan] = Get_Frequency(start_period) / SAMPLE_RATE;

                        }
                        break;

                        case 1:
                        {
                            
                            switch (ticks % 3)
                            {
                            
                            case 0:

                                note = Get_Note(piano_pat[cur_pat][chan][cur_note]);

                                jinc[chan] = note / SAMPLE_RATE;

                                break;

                            case 1:

                                note = Get_Note(piano_pat[cur_pat][chan][cur_note] + HIBYTE(effect_param[cur_pat][cur_chan][cur_note]));

                                jinc[chan] = note / SAMPLE_RATE;

                                break;

                            case 2:

                                note = Get_Note(piano_pat[cur_pat][chan][cur_note] + LOBYTE(effect_param[cur_pat][cur_chan][cur_note]));

                                jinc[chan] = note / SAMPLE_RATE;

                                break;

                            
                            }
                            

                        }
                        break;

                        }



                    }

                    if (ticks >= 6) ticks = 0;


                }


                output += lerp;

                ji[chan] += jinc[chan];


                if (ji[chan] > bufferSize[chan] - 10)
                {
          
                    active_note[chan] = false;

                    effect_active[chan] = false;

                    ji[chan] = 0;
                }

            }


            if (sample_count > samples_beat)
            {

                cur_note++;

                if (cur_note >= len[cur_pat])
                {
                    cur_note = 0;

                    //song_position++;

                    //cur_pat = pot[song_position];

                    //samples_beat = 60 / bpm[cur_pat] * SAMPLE_RATE;


                }


                for (int chan = 0; chan < num_chan; chan++)
                {

                    if (piano_pat[cur_pat][chan][cur_note] != 0)
                    {
                    
                        
                        note = Get_Note(piano_pat[cur_pat][chan][cur_note]);

                        ji[chan] = 0;
                        
                        jinc[chan] = note / SAMPLE_RATE;

                        active_note[chan] = true;
                    
                        decay[chan] = 1.0f;

                    }




                    if (piano_effects[cur_pat][chan][cur_note] > 0)
                    {
                        
                        effect_active[chan] = true;

                        switch (piano_effects[cur_pat][chan][cur_note])
                        {



                            start_period = Get_Period(piano_pat[cur_pat][chan][cur_note]);

                            period = start_period;


                        }
                    }



                }

                sample_count = 0;

            }

            sample_count++;

            



        }


        if (drums)
        {

            for (int chan = 0; chan < 8; chan++)
            {

                if (drum_active[chan] == false) continue;

                float x = xi[chan] - (int)xi[chan];

                int zi = (int)xi[chan];

                float lerp = waveFloat[chan + 16][zi] + (waveFloat[chan + 16][zi + 1] - waveFloat[chan + 16][zi]) * x;

                if (drum_count > (drum_beat - 100) && drum_pat[drum_pattern][chan][drum_pos + 1] != 0)
                {
                    lerp *= decay[chan + 16];

                    decay[chan + 16] -= 0.01f;


                }

                output += lerp;

                xi[chan] += xinc[chan];


                if (xi[chan] > bufferSize[chan + 16] - 10)
                {

                    drum_active[chan] = false;

                    xi[chan] = 0;
                }

            }


            if (drum_count > drum_beat)
            {

                drum_pos++;

                if (drum_pos >= 15)
                {

                    drum_pos = 0;

                }

                /*if (drum_pos >= dlen[drum_pattern])
                {
                    drum_pos = 0;

                    //song_position++;

                    //cur_pat = pot[song_position];

                    //samples_beat = 60 / bpm[cur_pat] * SAMPLE_RATE;


                }
                */

                for (int chan = 0; chan < 8; chan++)
                {


                    if (drum_pat[drum_pattern][chan][drum_pos] == 1)
                    {

                        xinc[chan] = 1;

                        xi[chan] = 0;

                        drum_active[chan] = true;

                        decay[chan + 16] = 1.0f;

                    }

                }

                drum_count = 0;

            }

            drum_count++;

        }




        if (playing_mix)
        {
            for (int chan = 0; chan < num_chan; chan++)
            {

                if (chan_active[chan] != true) continue;

                for (int v = 0; v < num_notes; v++)
                {

                    if (note_active[chan][v] == true)
                    {

                        if ((int)si[chan][v] > bufferSize[chan] - 10)
                        {

                            note_active[chan][v] = false;

                            si[chan][v] = 0;

                            continue;

                        }

                        si[chan][v] += dinc[chan][v];

                        float xi = si[chan][v] - (int)si[chan][v];

                        int vi = (int)si[chan][v];

                        output += waveFloat[chan][vi] + (waveFloat[chan][vi + 1] - waveFloat[chan][vi]) * xi;

                    }

                }

            }

                if (sample_count > samples_beat)
                {

                    cur_note++;

                    for (int chan = 0; chan < num_chan; chan++)
                    {

                        if (j[chan] >= num_notes - 1) j[chan] = 0;

                        if (piano_pat[cur_pat][chan][cur_note] != 0)
                        {
                            note = Get_Note(piano_pat[cur_pat][chan][cur_note]);

                            int k = j[chan];

                            dinc[chan][k] = note / SAMPLE_RATE;

                            note_active[chan][k] = true;

                            si[chan][k] = 0;

                            j[chan]++;


                        }
                    }
                    sample_count = 0;
                }

                sample_count++;

            }

        output *= 0.6f;

        peakfloat += output;

        peak_count++;

        if (peak_count > 2940)
        {
         peak_count = 0;

         peakfloat /= 2940.0f;
            
         peak = peakfloat * 1000.0f;

         peakfloat = 0;

         InvalidateRect(hChildPeak, NULL, TRUE);

        }

       
         *out++ = output;

            

    }
    
    return paContinue;

}


 /* This routine is called by portaudio when playback is done.
 */
static void StreamFinished(void* userData)
{
    paTestData* data = (paTestData*)userData;
    printf("Stream Completed: %s\n", data->message);
}


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);


    rect.left = 0;
    rect.top = 0;
    rect.right = 20;
    rect.bottom = 0;


    piano_note = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_PIANO));
    if (piano_note == NULL)
        MessageBox(hWndMain, L"Could not load UNLITBLUE", L"Error", MB_OK | MB_ICONEXCLAMATION);

    piano_grid = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_PIANOGRID));
    if (piano_note == NULL)
        MessageBox(hWndMain, L"Could not load UNLITBLUE", L"Error", MB_OK | MB_ICONEXCLAMATION);

    play = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_PLAY));
    if (piano_note == NULL)
        MessageBox(hWndMain, L"Could not load UNLITBLUE", L"Error", MB_OK | MB_ICONEXCLAMATION);

    stop = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_STOP));
    if (piano_note == NULL)
        MessageBox(hWndMain, L"Could not load UNLITBLUE", L"Error", MB_OK | MB_ICONEXCLAMATION);

    circle = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_CIRCLE));
    if (piano_note == NULL)
        MessageBox(hWndMain, L"Could not load UNLITBLUE", L"Error", MB_OK | MB_ICONEXCLAMATION);


    pitchblank = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_PITCHBLANK));
    if (piano_note == NULL)
        MessageBox(hWndMain, L"Could not load UNLITBLUE", L"Error", MB_OK | MB_ICONEXCLAMATION);

    up = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_UP));
    if (piano_note == NULL)
        MessageBox(hWndMain, L"Could not load UNLITBLUE", L"Error", MB_OK | MB_ICONEXCLAMATION);

    down = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_DOWN));
    if (piano_note == NULL)
        MessageBox(hWndMain, L"Could not load UNLITBLUE", L"Error", MB_OK | MB_ICONEXCLAMATION);

    matrix = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_MATRIX));
    if (piano_note == NULL)
        MessageBox(hWndMain, L"Could not load UNLITBLUE", L"Error", MB_OK | MB_ICONEXCLAMATION);

    unlit = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_UNLIT));
    if (piano_note == NULL)
        MessageBox(hWndMain, L"Could not load UNLITBLUE", L"Error", MB_OK | MB_ICONEXCLAMATION);

    lit = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_LIT));
    if (piano_note == NULL)
        MessageBox(hWndMain, L"Could not load UNLITBLUE", L"Error", MB_OK | MB_ICONEXCLAMATION);

    pianoroll = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_PIANOROLL));
    if (piano_note == NULL)
        MessageBox(hWndMain, L"Could not load UNLITBLUE", L"Error", MB_OK | MB_ICONEXCLAMATION);

    button1 = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BUTTON1));
    if (piano_note == NULL)
        MessageBox(hWndMain, L"Could not load UNLITBLUE", L"Error", MB_OK | MB_ICONEXCLAMATION);

    chan_bitmap = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_CHAN));
    if (piano_note == NULL)
        MessageBox(hWndMain, L"Could not load UNLITBLUE", L"Error", MB_OK | MB_ICONEXCLAMATION);

    chan_blank = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_CHANBLANK));
    if (piano_note == NULL)
        MessageBox(hWndMain, L"Could not load UNLITBLUE", L"Error", MB_OK | MB_ICONEXCLAMATION);

    patblt = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_PATBLT));
    if (piano_note == NULL)
        MessageBox(hWndMain, L"Could not load UNLITBLUE", L"Error", MB_OK | MB_ICONEXCLAMATION);

    bpm_bmp = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BPM));
    if (piano_note == NULL)
        MessageBox(hWndMain, L"Could not load UNLITBLUE", L"Error", MB_OK | MB_ICONEXCLAMATION);

    pat_bmp = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_PAT));
    if (piano_note == NULL)
        MessageBox(hWndMain, L"Could not load UNLITBLUE", L"Error", MB_OK | MB_ICONEXCLAMATION);

    len_bmp = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_LEN));
    if (piano_note == NULL)
        MessageBox(hWndMain, L"Could not load UNLITBLUE", L"Error", MB_OK | MB_ICONEXCLAMATION);

    togoff = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_TOGOFF));
    if (piano_note == NULL)
        MessageBox(hWndMain, L"Could not load UNLITBLUE", L"Error", MB_OK | MB_ICONEXCLAMATION);

    togon = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_TOGON));
    if (piano_note == NULL)
        MessageBox(hWndMain, L"Could not load UNLITBLUE", L"Error", MB_OK | MB_ICONEXCLAMATION);

    vibnote = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_VIBNOTE));
    if (piano_note == NULL)
        MessageBox(hWndMain, L"Could not load UNLITBLUE", L"Error", MB_OK | MB_ICONEXCLAMATION);

    pitchnote = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_PITCHNOTE));
    if (piano_note == NULL)
        MessageBox(hWndMain, L"Could not load UNLITBLUE", L"Error", MB_OK | MB_ICONEXCLAMATION);

    appregnote = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_APPREGNOTE));
    if (piano_note == NULL)
        MessageBox(hWndMain, L"Could not load UNLITBLUE", L"Error", MB_OK | MB_ICONEXCLAMATION);

    

    play_mask.left = 0;
    play_mask.top = 0;
    play_mask.right = 35;
    play_mask.bottom = 41;

    stop_mask.left = 36;
    stop_mask.top = 0;
    stop_mask.right = 71;
    stop_mask.bottom = 41;

    pitchslide.left = 200;
    pitchslide.top = 0;
    pitchslide.right = 392;
    pitchslide.bottom = 34;
    
    up_mask.left = 160;
    up_mask.top = 0;
    up_mask.right = 194;
    up_mask.bottom = 37;

    down_mask.left = 160;
    down_mask.top = 59;
    down_mask.right = 194;
    down_mask.bottom = 96;

    button1_mask.left = 0;
    button1_mask.top = 200;
    button1_mask.right = 186;
    button1_mask.bottom = 812;

    button2_mask.left = 930;
    button2_mask.top = 200;
    button2_mask.right = 1116;
    button2_mask.bottom = 812;
    
    chan_mask.left = 186;
    chan_mask.top = 200;
    chan_mask.right = 218;
    chan_mask.bottom = 812;

    pat_mask_up.left = 250;
    pat_mask_up.right = 286;
    pat_mask_up.top = 0;
    pat_mask_up.bottom = 37;

    pat_mask_down.left = 250;
    pat_mask_down.right = 286;
    pat_mask_down.top = 59;
    pat_mask_down.bottom = 96;

    len_mask_up.left = 360;
    len_mask_up.right = 396;
    len_mask_up.top = 0;
    len_mask_up.bottom = 37;

    len_mask_down.left = 360;
    len_mask_down.right = 396;
    len_mask_down.top = 59;
    len_mask_down.bottom = 96;

// Drum Input

    drum_bpm_up.left = 1260;
    drum_bpm_up.right = 1294;
    drum_bpm_up.top = 0;
    drum_bpm_up.bottom = 37;

    drum_bpm_down.left = 1260;
    drum_bpm_down.right = 1294;
    drum_bpm_down.top = 59;
    drum_bpm_down.bottom = 96;

    drum_pat_up.left = 1360;
    drum_pat_up.right = 1394;
    drum_pat_up.top = 0;
    drum_pat_up.bottom = 37;

    drum_pat_down.left = 1360;
    drum_pat_down.right = 1394;
    drum_pat_down.top = 59;
    drum_pat_down.bottom = 96;


    drum_len_up.left = 1460;
    drum_len_up.right = 1494;
    drum_len_up.top = 0;
    drum_len_up.bottom = 37;

    drum_len_down.left = 1460;
    drum_len_down.right = 1494;
    drum_len_down.top = 59;
    drum_len_down.bottom = 96;



    vibtoggle.left = 480;
    vibtoggle.right = 544;
    vibtoggle.bottom = 32;
    vibtoggle.top = 0;

    for (int i = 0; i < 128; i++)
    {

        len[i] = 32;

    }

    for (int i = 0; i < 128; i++)
    {

        bpm[i] = 128;

    }

    for (int i = 0; i < 128; i++)
    {

        dlen[i] = 32;

    }

    for (int i = 0; i < 128; i++)
    {

        dbpm[i] = 128;

    }
    
    for (int i = 0; i < 16; i++)
    {
        frequency[i] = 180.0f;
    }

    for (int i = 0; i < 16; i++)
    {
        //amplitude[i] = 50.0f / ampdiv;
        amplitude[i] = 0.5f;
    }

    for (int i = 0; i < 32; i++)
    {
        decay[i] = 1.0f;
    }
    
    

    start_vib();

    /*    if (!LoadWaveFile("C:\\synth.wav"))
    {
        MessageBox(hWndMain, L"Could not load .WAV File not 16 bit Mono 44100hz", L"Error", MB_OK | MB_ICONEXCLAMATION);
    }
    */
    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_NETJAMTRACKER, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_NETJAMTRACKER));


    //init_tcp();

    two_pi = M_PI * 2.0f;


    //DialogBox(hInst, MAKEINTRESOURCE(IDD_DONATE), hWndMain, DONATE);

    MSG msg;

    //    SetTimer(hWnd, 10001, 300, (TIMERPROC)NULL);
    SetWindowText(hWndMain, L"-=NetJamTracker=- Ver 1.0");
     
    LoadConfig();

//    InitMidi();

    Init_PortAudio();
    

        // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }


    //midiInStop(hMidiDevice);
    //midiInClose(hMidiDevice);
    //midiInStop(0);
    //midiInClose(0);
    //hMidiDevice = NULL;

    err = Pa_StopStream(stream);

    err = Pa_CloseStream(stream);

    Pa_Terminate();

    //closesocket(theSocket);

    //WSACleanup();

    return (int)msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;
    WNDCLASSEXW wcex_child;
    WNDCLASSEXW wcex_child2;
    WNDCLASSEXW wcex_childpeak;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_NETJAMTRACKER));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_NETJAMTRACKER);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));


    wcex_child.cbSize = sizeof(WNDCLASSEX);
    wcex_child.style = CS_HREDRAW | CS_VREDRAW;
    wcex_child.lpfnWndProc = ChildWndProc;
    wcex_child.cbClsExtra = 0;
    wcex_child.cbWndExtra = 0;
    wcex_child.hInstance = hInstance;
    wcex_child.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_NETJAMTRACKER));
    wcex_child.hCursor = LoadCursor(nullptr, IDC_ARROW);
    //wcex_child.hbrBackground = CreateSolidBrush(0x000000ff);
    wcex_child.hbrBackground = NULL;
    wcex_child.lpszMenuName = MAKEINTRESOURCE(IDI_NETJAMTRACKER);
    wcex_child.lpszClassName = L"MY_CHILD_WINDOW";
    wcex_child.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
    wcex_child.hbrBackground = NULL;

    wcex_child2.cbSize = sizeof(WNDCLASSEX);
    wcex_child2.style = CS_HREDRAW | CS_VREDRAW;
    wcex_child2.lpfnWndProc = ChildWndProc2;
    wcex_child2.cbClsExtra = 0;
    wcex_child2.cbWndExtra = 0;
    wcex_child2.hInstance = hInstance;
    wcex_child2.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_NETJAMTRACKER));
    wcex_child2.hCursor = LoadCursor(nullptr, IDC_ARROW);
    //wcex_child.hbrBackground = CreateSolidBrush(0x000000ff);
    wcex_child2.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex_child2.lpszMenuName = MAKEINTRESOURCE(IDI_NETJAMTRACKER);
    wcex_child2.lpszClassName = L"MY_CHILD_WINDOW2";
    wcex_child2.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
    wcex_child2.hbrBackground = NULL;

    wcex_childpeak.cbSize = sizeof(WNDCLASSEX);
    wcex_childpeak.style = CS_HREDRAW | CS_VREDRAW;
    wcex_childpeak.lpfnWndProc = ChildWndProcPeak;
    wcex_childpeak.cbClsExtra = 0;
    wcex_childpeak.cbWndExtra = 0;
    wcex_childpeak.hInstance = hInstance;
    wcex_childpeak.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_NETJAMTRACKER));
    wcex_childpeak.hCursor = LoadCursor(nullptr, IDC_ARROW);
    //wcex_child.hbrBackground = CreateSolidBrush(0x000000ff);
    wcex_childpeak.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex_childpeak.lpszMenuName = MAKEINTRESOURCE(IDI_NETJAMTRACKER);
    wcex_childpeak.lpszClassName = L"MY_CHILD_WINDOWPEAK";
    wcex_childpeak.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
    wcex_childpeak.hbrBackground = NULL;

    RegisterClassExW(&wcex_child);

    RegisterClassExW(&wcex_child2);

    RegisterClassExW(&wcex_childpeak);

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // Store instance handle in our global variable

    hWndMain = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
        0, 0, 1920, 1080, nullptr, nullptr, hInstance, nullptr);

    if (!hWndMain)
    {
        return FALSE;
    }

    ShowWindow(hWndMain, nCmdShow);
    UpdateWindow(hWndMain);

    return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

    switch (message)
    {

    case WM_CREATE:
    {
        hChildWindow1 = CreateWindowW(TEXT("MY_CHILD_WINDOW"), L"Piano", WS_CHILDWINDOW | WS_VISIBLE | WS_BORDER | WS_HSCROLL,
            378, 200, 545, 592, hWnd, nullptr, GetModuleHandle(NULL), nullptr);


        SetScrollRange(hChildWindow1, SB_HORZ, 0, len[cur_pat] - 16, true);

        hChildWindow2 = CreateWindowW(TEXT("MY_CHILD_WINDOW2"), L"Drums", WS_CHILDWINDOW | WS_VISIBLE | WS_BORDER | WS_HSCROLL,
            1200, 200, 664, 592, hWnd, nullptr, GetModuleHandle(NULL), nullptr);


        hChildPeak = CreateWindowW(TEXT("MY_CHILD_WINDOWPEAK"), L"Peak", WS_CHILDWINDOW | WS_VISIBLE | WS_BORDER,
            1000, 20, 20, 100, hWnd, nullptr, GetModuleHandle(NULL), nullptr);

        InvalidateRect(hChildPeak, NULL, TRUE);

        //SetTimer(hChildPeak,             // handle to main window 
         //   IDT_TIMER1,            // timer identifier 
          //  500,                 // 10-second interval 
           // (TIMERPROC)NULL);

        SetTimer(hChildPeak,             // handle to main window 
            IDT_TIMER2,            // timer identifier 
            300,                 // 10-second interval 
            (TIMERPROC)NULL);



        SetScrollRange(hChildWindow2, SB_HORZ, 0, dlen[drum_pattern] - 16, true);

        //        InvalidateRect(hChildWindow1, NULL, FALSE);

        InitCommonControls(); // loads common control's DLL 

        int yt = 200;

        for (int i = 0; i < 16; i++)
        {


                hwndTrack[i] = CreateWindowEx(
                0,                               // no extended styles 
                TRACKBAR_CLASS,                  // class name 
                L"Trackbar Control",              // title (caption) 
                WS_CHILD |
                WS_VISIBLE |
                TBS_AUTOTICKS |
                TBS_ENABLESELRANGE,              // style 
                218, yt,                          // position 
                88, 30,                         // size 
                hWnd,                         // parent window 
                (HMENU) 0,                     // control identifier 
                hInst,                         // instance 
                NULL                             // no WM_CREATE parameter 
            );
        
            yt += 32;

            SendMessage(hwndTrack[i], TBM_SETPOS,
                (WPARAM)TRUE,                   // redraw flag 
                (LPARAM)50);


        }
        /*SendMessage(hwndTrack, TBM_SETRANGE,
            (WPARAM)TRUE,                   // redraw flag 
            (LPARAM)MAKELONG(iMin, iMax));  // min. & max. positions

        SendMessage(hwndTrack, TBM_SETPAGESIZE,
            0, (LPARAM)4);                  // new page size 

        SendMessage(hwndTrack, TBM_SETSEL,
            (WPARAM)FALSE,                  // redraw flag 
            (LPARAM)MAKELONG(iSelMin, iSelMax));
            */
            
       // SetFocus(hwndTrack);
        HFONT hFont1 = CreateFont(24, 0, 0, 0, FW_DONTCARE, FALSE, TRUE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
            CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Arial"));



        listbox = CreateWindowEx(0,
            L"Listbox",
            L"Listbox",
            WS_CHILD | WS_VISIBLE | WS_HSCROLL | LBS_MULTICOLUMN | LBS_NOINTEGRALHEIGHT | WS_BORDER,
            20,
            110,
            500,
            35,
            hWnd,
            NULL,
            hInst,
            NULL);

        
        SendMessage(listbox, WM_SETFONT, (WPARAM) hFont1,
            NULL);

            SendMessage(listbox, LB_SETCOLUMNWIDTH, (WPARAM) 28,
                NULL);
          

        HWND hwndButton = CreateWindowEx(0,
            L"BUTTON",  // Predefined class; Unicode assumed 
            L"Insert",      // Button text 
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
            520,         // x position 
            110,         // y position 
            60,        // Button width
            18,        // Button height
            hWnd,     // Parent window
            (HMENU) IDC_INSERT,       // No menu.
            hInst,
            NULL);      // Pointer not needed.

        HWND hwndButton2 = CreateWindowEx(0,
            L"BUTTON",  // Predefined class; Unicode assumed 
            L"Delete",      // Button text 
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
            520,         // x position 
            128,         // y position 
            60,        // Button width
            18,        // Button height
            hWnd,     // Parent window
            (HMENU) IDC_DELETE,       // No menu.
            hInst,
            NULL);      // Pointer not needed.


            hWndComboBox = CreateWindowEx(0, WC_COMBOBOX, TEXT(""),
            CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
            450, 0, 120, 300, hWnd, (HMENU) IDC_EFFECT, hInst,
            NULL);
            
            SendMessage(hWndComboBox, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)L"None");
            SendMessage(hWndComboBox, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)L"Apreggio");
            SendMessage(hWndComboBox, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)L"Pitch Slide Up");
            SendMessage(hWndComboBox, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)L"Pitch Slide Down");
            SendMessage(hWndComboBox, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)L"Vibrato");

            SendMessage(hWndComboBox, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);

            hWndComboParam = CreateWindowEx(0, WC_COMBOBOX, TEXT(""),
                CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
                600, 0, 120, 300, hWnd, (HMENU) IDC_PARAM, hInst,
                NULL);



            HWND hwndButton3 = CreateWindowEx(0,
                L"BUTTON",  // Predefined class; Unicode assumed 
                L"Edit",      // Button text 
                WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
                740,         // x position 
                0,         // y position 
                60,        // Button width
                18,        // Button height
                hWnd,     // Parent window
                (HMENU)IDC_EDIT,       // No menu.
                hInst,
                NULL);      // Pointer not needed.



            HWND hwndButton4 = CreateWindowEx(0,
                L"BUTTON",  // Predefined class; Unicode assumed 
                L"Oscillator",      // Button text 
                WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
                900,         // x position 
                0,         // y position 
                80,        // Button width
                18,        // Button height
                hWnd,     // Parent window
                (HMENU)IDC_OSC,       // No menu.
                hInst,
                NULL);      // Pointer not needed.



    }

    case WM_HSCROLL:
    {

        //pos = SendMessage(hwndTrack, TBM_GETPOS, 0, 0);

        //InvalidateRect(hWnd, NULL, TRUE);


    }
    break;

    case WM_COMMAND:
    {

        if (HIWORD(wParam) == CBN_SELCHANGE)
        {
            if (LOWORD(wParam) == IDC_EFFECT)
            {
                effectindex = SendMessage((HWND)lParam, CB_GETCURSEL, (WPARAM)0, (LPARAM)0);

                SendMessage(hWndComboParam, CB_RESETCONTENT, (WPARAM)0, (LPARAM)0);
                

                switch (effectindex)
                {

                case 0: // None
                {
                    piano = true;
                    apreggio = false;
                    vibactive = false;
                    pitch_note = false;

                }
                break;
                
                case 1: // Apreggio
                {

                    piano = false;
                    apreggio = true;
                    vibactive = false;
                    pitch_note = false;

                    const char* paramstr[] = { "Minor 1 2 7" ,
                                               "Major 1 5 7" };


                    for (int i = 0; i < 2; i++)
                    {
                        SendMessageA(hWndComboParam, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)paramstr[i]);
                    }
                }
                break;

                case 2: // Pitch up
                {

                    piano = false;
                    apreggio = false;
                    vibactive = false;
                    pitch_note = true;


                    for (int i = 0; i < 16; i++)
                    {
                        textlength = wsprintf(str, L"Portamento %d", i);

                        SendMessage(hWndComboParam, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM) str);
                    }
                }
                break;

                case 3: // Pitch down
                {

                    piano = false;
                    apreggio = false;
                    vibactive = false;
                    pitch_note = true;


                    for (int i = 0; i < 16; i++)
                    {
                        textlength = wsprintf(str, L"Portamento %d", i);

                        SendMessage(hWndComboParam, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)str);
                    }
                }
                break;

                case 4: // Vibrato
                {

                    piano = false;
                    apreggio = false;
                    vibactive = true;
                    pitch_note = false;


                    for (int i = 0; i < 16; i++)
                    {
                        textlength = wsprintf(str, L"Vibrato %d", i);

                        SendMessage(hWndComboParam, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)str);
                    }
                }
                break;

                }

                SendMessage(hWndComboParam, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);

            }

            if (LOWORD(wParam) == IDC_PARAM)
            {


                paramindex = SendMessage((HWND)lParam, CB_GETCURSEL, (WPARAM)0, (LPARAM)0);

            }



            InvalidateRect(hWndMain, NULL, TRUE);


            break;
        }


        int wmId = LOWORD(wParam);

        // Parse the menu selections:
        switch (wmId)
        {

        
        case IDC_OSC:

            DialogBox(hInst, MAKEINTRESOURCE(IDD_OSC), hWndMain, OSC);

            break;

        case IDC_EDIT:
   
            vibindex = SendMessage(hWndComboParam, CB_GETCURSEL, (WPARAM)0, (LPARAM)0);

            vibindex++;

            init_vib();

            DialogBox(hInst, MAKEINTRESOURCE(IDD_VIB), hWndMain, VIB);

            break;

        case IDC_INSERT:


            psel = SendMessage(listbox, LB_GETCURSEL, 0, 0);


            if (psel == LB_ERR)
            {
                psel = -1;
            }

            else

            {
                psel++;
            }

            num_pat++;

            TCHAR pstr[10];

            textlength = wsprintf(pstr, L"%d", cur_pat);

            SendMessage(listbox, LB_INSERTSTRING, (WPARAM) psel,
                (LPARAM) pstr);


            break;
        case IDC_DELETE:

            psel = SendMessage(listbox, LB_GETCURSEL, 0, 0);


            if (psel == LB_ERR)
            {

                break;
            }

            else

            {
                num_pat--;
                SendMessage(listbox, LB_DELETESTRING, (WPARAM) psel, (LPARAM) 0);
            }
                


            break;
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;

        case ID_LOAD:
            Load();
            break;
        case ID_SAVE:
            Save();
            break;
        
        case ID_SETUP:
        {
            DialogBox(hInst, MAKEINTRESOURCE(IDD_SETUP), hWndMain, Setup);
        }
        break;

        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;

    case WM_TIMER:
    {

        // MessageBox(hWnd, L"Arpreggio", L"Error", MB_OK | MB_ICONEXCLAMATION);

        InvalidateRect(hWnd, NULL, FALSE);


    }
    break;


    case WM_LBUTTONDOWN:
    {

        xPos = LOWORD(lParam);  // horizontal position of cursor 
        yPos = HIWORD(lParam);  // vertical position of cursor    


        point_main.x = xPos;
        point_main.y = yPos;

        if (PtInRect(&play_mask, point_main))
        {

            start_pattern();

            playing = true;
            //start_drums();

            //drums = true;

           // start_pattern_mix();

           // playing_mix = true;


        }


        if (PtInRect(&stop_mask, point_main))
        {
            //playing = false;
            //song_position = 0;
            //cur_note = 0;

            drums = false;
            drum_pos = 0;
            drum_pattern = 0;

        }

        if (PtInRect(&chan_mask, point_main) && play_preview == false)
        {

            if (chan_active[(yPos - 200) / 32] == true)
            {
                cur_chan = (yPos - 200) / 32;

                ychan = ((yPos - 200) / 32) * 32;
                ychan += 200;

            }

            else
            {

                MessageBox(hWndMain, L"This Channel Does Not Have a Sample Loaded", L"Error", MB_OK | MB_ICONEXCLAMATION);


            }


        }


        if (PtInRect(&button1_mask, point_main))
        {

            findex = (yPos - button1_mask.top) / 32;

            HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED |
                COINIT_DISABLE_OLE1DDE);
            if (SUCCEEDED(hr))
            {
                IFileOpenDialog* pFileOpen;

                // Create the FileOpenDialog object.
                hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
                    IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

                if (SUCCEEDED(hr))
                {
                    IShellItem* folder;
                    HRESULT result = SHCreateItemFromParsingName(L"C:\\NetJamTracker\\Samples\\", NULL, IID_PPV_ARGS(&folder));
                    // Show the Open dialog box.
                    hr = pFileOpen->SetFolder(folder);

                    hr = pFileOpen->Show(NULL);

                    // Get the file name from the dialog box.
                    if (SUCCEEDED(hr))
                    {
                        IShellItem* pItem;
                        hr = pFileOpen->GetResult(&pItem);
                        if (SUCCEEDED(hr))
                        {
                            PWSTR pszFilePath;
                            
                            hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);
                                                                                  

                            // Display the file name to the user.
                            if (SUCCEEDED(hr))
                            {
                                StrCpyW(samp_files_dir[findex], pszFilePath);
                        
                                if (!LoadWaveFile(pszFilePath, findex))
                                {
                                    MessageBox(hWndMain, L"Could not load .WAV File not 16 bit Mono 44100hz", L"Error", MB_OK | MB_ICONEXCLAMATION);
                                }

                                chan_active[findex] = true;

                                PathStripPath(pszFilePath);
                                
                                StrCpyW(samp_files[findex], pszFilePath);
                                
                                //MessageBoxW(NULL, pszFilePath, L"File Path", MB_OK);
                                CoTaskMemFree(pszFilePath);
                            }


                            pItem->Release();
                        }
                    }
                    pFileOpen->Release();
                }
                CoUninitialize();
            }
     

        }
    

        if (PtInRect(&button2_mask, point_main))
        {

            findex = (yPos - button2_mask.top) / 32;

            findex += 16;

            HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED |
                COINIT_DISABLE_OLE1DDE);
            if (SUCCEEDED(hr))
            {
                IFileOpenDialog* pFileOpen;

                // Create the FileOpenDialog object.
                hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
                    IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

                if (SUCCEEDED(hr))
                {
                    IShellItem* folder;
                    HRESULT result = SHCreateItemFromParsingName(L"C:\\NetJamTracker\\Samples\\", NULL, IID_PPV_ARGS(&folder));
                    // Show the Open dialog box.
                    hr = pFileOpen->SetFolder(folder);

                    hr = pFileOpen->Show(NULL);

                    // Get the file name from the dialog box.
                    if (SUCCEEDED(hr))
                    {
                        IShellItem* pItem;
                        hr = pFileOpen->GetResult(&pItem);
                        if (SUCCEEDED(hr))
                        {
                            PWSTR pszFilePath;

                            hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);


                            // Display the file name to the user.
                            if (SUCCEEDED(hr))
                            {
                                StrCpyW(samp_files_dir[findex], pszFilePath);

                                if (!LoadWaveFile(pszFilePath, findex))
                                {
                                    MessageBox(hWndMain, L"Could not load .WAV File not 16 bit Mono 44100hz", L"Error", MB_OK | MB_ICONEXCLAMATION);
                                }

                                chan_active[findex] = true;

                                PathStripPath(pszFilePath);

                                StrCpyW(samp_files[findex], pszFilePath);

                                //MessageBoxW(NULL, pszFilePath, L"File Path", MB_OK);
                                CoTaskMemFree(pszFilePath);
                            }


                            pItem->Release();
                        }
                    }
                    pFileOpen->Release();
                }
                CoUninitialize();
            }


        }






        if (PtInRect(&pitchslide, point_main))
        {
            //pitch_note = true;
        }

        // Piano Input

        if (PtInRect(&up_mask, point_main))
        {
            bpm[cur_pat]++;

            if (bpm[cur_pat] >= 999) bpm[cur_pat] = 999;
        }

        if (PtInRect(&down_mask, point_main))
        {
            bpm[cur_pat]--;

            if (bpm[cur_pat] <= 0) bpm[cur_pat] = 0;

        }

        if (PtInRect(&pat_mask_up, point_main))
        {
            cur_pat++;

            SetScrollRange(hChildWindow1, SB_HORZ, 0, len[cur_pat] - 16, true);
        }

        if (PtInRect(&pat_mask_down, point_main))
        {
            cur_pat--;

            if (cur_pat <= 0) cur_pat = 0;

            SetScrollRange(hChildWindow1, SB_HORZ, 0, len[cur_pat] - 16, true);

        }



        if (PtInRect(&len_mask_up, point_main))
        {

            SetScrollRange(hChildWindow1, SB_HORZ, 0, len[cur_pat] - 16, true);

            len[cur_pat]++;

        }

        if (PtInRect(&len_mask_down, point_main))
        {


            SetScrollRange(hChildWindow1, SB_HORZ, 0, len[cur_pat] - 16, true);

            len[cur_pat]--;
            if (len[cur_pat] <= 0) len[cur_pat] = 0;

        }

        // Drum Input

        if (PtInRect(&drum_bpm_up, point_main))
        {
            dbpm[drum_pattern]++;

            if (dbpm[drum_pattern] >= 999) dbpm[drum_pattern] = 999;
        }

        if (PtInRect(&drum_bpm_down, point_main))
        {
            dbpm[drum_pattern]--;

            if (dbpm[drum_pattern] <= 0) dbpm[drum_pattern] = 0;

        }

        if (PtInRect(&drum_pat_up, point_main))
        {
            drum_pattern++;

            SetScrollRange(hChildWindow2, SB_HORZ, 0, dlen[drum_pattern] - 16, true);
        }

        if (PtInRect(&drum_pat_down, point_main))
        {
            drum_pattern--;

            if (drum_pattern <= 0) drum_pattern = 0;

            SetScrollRange(hChildWindow2, SB_HORZ, 0, dlen[drum_pattern] - 16, true);

        }



        if (PtInRect(&drum_len_up, point_main))
        {

            SetScrollRange(hChildWindow2, SB_HORZ, 0, dlen[drum_pattern] - 16, true);

            dlen[drum_pattern]++;

        }

        if (PtInRect(&drum_len_down, point_main))
        {


            SetScrollRange(hChildWindow2, SB_HORZ, 0, dlen[drum_pattern] - 16, true);

            dlen[drum_pattern]--;
            if (dlen[drum_pattern] <= 0) dlen[drum_pattern] = 0;

        }





        /*/if (PtInRect(&vibtoggle, point_main))
        {
            vibactive = (vibactive == 1) ? 0 : 1;
        }
        */

        InvalidateRect(hWndMain, NULL, TRUE);
        InvalidateRect(hChildWindow1, NULL, FALSE);

    }
    break;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        BITMAP bm;

        HDC hdc = BeginPaint(hWnd, &ps);

        HDC hdcMem = CreateCompatibleDC(hdc);

        SelectObject(hdcMem, play);

        GetObject(play, sizeof(bm), &bm);

        BitBlt(hdc, 0, 0, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);

        SelectObject(hdcMem, stop);

        GetObject(stop, sizeof(bm), &bm);

        BitBlt(hdc, 41, 0, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);

        SelectObject(hdcMem, pianoroll);

        GetObject(pianoroll, sizeof(bm), &bm);

        BitBlt(hdc, 308, 200, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);


        int yb = 200;

        for (int i = 0; i < 16; i++)
        {
            SelectObject(hdcMem, button1);

            GetObject(button1, sizeof(bm), &bm);

            BitBlt(hdc, 0, yb, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);
        
            yb += 32;
        
        }

        yb = 200;

        for (int i = 0; i < 16; i++)
        {
            SelectObject(hdcMem, button1);

            GetObject(button1, sizeof(bm), &bm);

            BitBlt(hdc, 930, yb, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);

            yb += 32;

        }

        for (int i = 0; i < 16; i++)
        {
            SelectObject(hdcMem, chan_blank);

            GetObject(chan_blank, sizeof(bm), &bm);

            BitBlt(hdc, 186, i * 32 + 200, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);
        }

        SelectObject(hdcMem, chan_bitmap);

        GetObject(chan_bitmap, sizeof(bm), &bm);

        BitBlt(hdc, 186, ychan, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);


        // Piano GUI

        SelectObject(hdcMem, bpm_bmp);

        GetObject(bpm_bmp, sizeof(bm), &bm);

        BitBlt(hdc, 100, 0, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);


        SelectObject(hdcMem, up);

        GetObject(up, sizeof(bm), &bm);

        BitBlt(hdc, 160, 0, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);

        SelectObject(hdcMem, down);

        GetObject(down, sizeof(bm), &bm);

        BitBlt(hdc, 160, 59, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);



        SelectObject(hdcMem, pat_bmp);

        GetObject(pat_bmp, sizeof(bm), &bm);

        BitBlt(hdc, 200, 0, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);


        SelectObject(hdcMem, up);

        GetObject(up, sizeof(bm), &bm);

        BitBlt(hdc, 250, 0, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);

        SelectObject(hdcMem, down);

        GetObject(down, sizeof(bm), &bm);

        BitBlt(hdc, 250, 59, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);


        SelectObject(hdcMem, len_bmp);

        GetObject(len_bmp, sizeof(bm), &bm);

        BitBlt(hdc, 300, 0, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);


        SelectObject(hdcMem, up);

        GetObject(up, sizeof(bm), &bm);

        BitBlt(hdc, 360, 0, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);

        SelectObject(hdcMem, down);

        GetObject(down, sizeof(bm), &bm);

        BitBlt(hdc, 360, 59, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);

        // Drum GUI

        SelectObject(hdcMem, bpm_bmp);

        GetObject(bpm_bmp, sizeof(bm), &bm);

        BitBlt(hdc, 1200, 0, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);


        SelectObject(hdcMem, up);

        GetObject(up, sizeof(bm), &bm);

        BitBlt(hdc, 1260, 0, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);

        SelectObject(hdcMem, down);

        GetObject(down, sizeof(bm), &bm);

        BitBlt(hdc, 1260, 59, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);

        

        SelectObject(hdcMem, pat_bmp);

        GetObject(pat_bmp, sizeof(bm), &bm);

        BitBlt(hdc, 1300, 0, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);

        
        SelectObject(hdcMem, up);

        GetObject(up, sizeof(bm), &bm);

        BitBlt(hdc, 1360, 0, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);

        SelectObject(hdcMem, down);

        GetObject(down, sizeof(bm), &bm);

        BitBlt(hdc, 1360, 59, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);


        SelectObject(hdcMem, len_bmp);

        GetObject(len_bmp, sizeof(bm), &bm);

        BitBlt(hdc, 1400, 0, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);

        
        SelectObject(hdcMem, up);

        GetObject(up, sizeof(bm), &bm);

        BitBlt(hdc, 1460, 0, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);

        SelectObject(hdcMem, down);

        GetObject(down, sizeof(bm), &bm);

        BitBlt(hdc, 1460, 59, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);


        /*/if (vibactive)
        {
            SelectObject(hdcMem, togon);

            GetObject(togon, sizeof(bm), &bm);

            BitBlt(hdc, 480, 0, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);
        }

        else
        {
            SelectObject(hdcMem, togoff);

            GetObject(togoff, sizeof(bm), &bm);

            BitBlt(hdc, 480, 0, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);
        }
        */


        SelectObject(hdcMem, matrix);

        GetObject(matrix, sizeof(bm), &bm);

        //Piano Matrix

        BitBlt(hdc, 178, 37, 16, bm.bmHeight, hdcMem, (bpm[cur_pat] % 10) * 16, 0, SRCCOPY);

        BitBlt(hdc, 162, 37, 16, bm.bmHeight, hdcMem, (bpm[cur_pat] % 100 / 10) * 16, 0, SRCCOPY);

        BitBlt(hdc, 146, 37, 16, bm.bmHeight, hdcMem, (bpm[cur_pat] % 1000 / 100) * 16, 0, SRCCOPY);


        BitBlt(hdc, 268, 37, 16, bm.bmHeight, hdcMem, (cur_pat % 10) * 16, 0, SRCCOPY);

        BitBlt(hdc, 252, 37, 16, bm.bmHeight, hdcMem, (cur_pat % 100 / 10) * 16, 0, SRCCOPY);

        BitBlt(hdc, 236, 37, 16, bm.bmHeight, hdcMem, (cur_pat % 1000 / 100) * 16, 0, SRCCOPY);


        BitBlt(hdc, 378, 37, 16, bm.bmHeight, hdcMem, (len[cur_pat] % 10) * 16, 0, SRCCOPY);

        BitBlt(hdc, 362, 37, 16, bm.bmHeight, hdcMem, (len[cur_pat] % 100 / 10) * 16, 0, SRCCOPY);

        BitBlt(hdc, 346, 37, 16, bm.bmHeight, hdcMem, (len[cur_pat] % 1000 / 100) * 16, 0, SRCCOPY);

        //Drum Matrix

        BitBlt(hdc, 1278, 37, 16, bm.bmHeight, hdcMem, (dbpm[drum_pattern] % 10) * 16, 0, SRCCOPY);

        BitBlt(hdc, 1262, 37, 16, bm.bmHeight, hdcMem, (dbpm[drum_pattern] % 100 / 10) * 16, 0, SRCCOPY);

        BitBlt(hdc, 1246, 37, 16, bm.bmHeight, hdcMem, (dbpm[drum_pattern] % 1000 / 100) * 16, 0, SRCCOPY);


        BitBlt(hdc, 1378, 37, 16, bm.bmHeight, hdcMem, (drum_pattern % 10) * 16, 0, SRCCOPY);

        BitBlt(hdc, 1362, 37, 16, bm.bmHeight, hdcMem, (drum_pattern % 100 / 10) * 16, 0, SRCCOPY);

        BitBlt(hdc, 1346, 37, 16, bm.bmHeight, hdcMem, (drum_pattern % 1000 / 100) * 16, 0, SRCCOPY);


        BitBlt(hdc, 1478, 37, 16, bm.bmHeight, hdcMem, (dlen[drum_pattern] % 10) * 16, 0, SRCCOPY);

        BitBlt(hdc, 1462, 37, 16, bm.bmHeight, hdcMem, (dlen[drum_pattern] % 100 / 10) * 16, 0, SRCCOPY);

        BitBlt(hdc, 1446, 37, 16, bm.bmHeight, hdcMem, (dlen[drum_pattern] % 1000 / 100) * 16, 0, SRCCOPY);

        // SelectObject(hdcMem, pitchblank);

      //  GetObject(pitchblank, sizeof(bm), &bm);

      //  BitBlt(hdc, 200, 0, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);

        SetTextAlign(hdc, TA_CENTER);

        xcount = xNewPos;

        xc = 398;

        for (int i = 0; i < 16; i++)
        {
            textlength = wsprintf(str, L"%d", xcount);
            TextOut(hdc, xc, yc, str, textlength);
            xcount++;
            xc += 34;
        }

        xcount = xNewPos2;

        xc = 1220;

        for (int i = 0; i < 16; i++)
        {
            textlength = wsprintf(str, L"%d", xcount);
            TextOut(hdc, xc, yc, str, textlength);
            xcount++;
            xc += 42;
        }

        SetBkMode(hdc, TRANSPARENT);


        SetTextAlign(hdc, TA_LEFT);

        int yn = 208;

        for (int i = 0; i < 16; i++)
        {
            textlength = wsprintf(str, L"%s", samp_files[i]);
            TextOut(hdc, 30, yn, str, textlength);

            yn += 32;
        }

        yn = 208;

        for (int i = 16; i < 32; i++)
        {


            textlength = wsprintf(str, L"%s", samp_files[i]);
            TextOut(hdc, 960, yn, str, textlength);

            yn += 32;

        }

        textlength = wsprintf(str, L"Porta = %d", porta_value);
        TextOut(hdc, 800, 40, str, textlength);

        textlength = wsprintf(str, L"Vibrato = %d", vibindex);
        TextOut(hdc, 800, 60, str, textlength);

        textlength = wsprintf(str, L"MIDI Note = %d", midi_note);
        TextOut(hdc, 800, 80, str, textlength);
        
        textlength = wsprintf(str, L"MIDI Status = %x", midi_status);
        TextOut(hdc, 800, 100, str, textlength);

        textlength = wsprintf(str, L"Key Velocity = %d", velocity);
        TextOut(hdc, 800, 120, str, textlength);

        textlength = wsprintf(str, L"Chan = %d", cur_chan);
        TextOut(hdc, 800, 140, str, textlength);

        textlength = wsprintf(str, L"Slider = %d", pos);
        TextOut(hdc, 800, 160, str, textlength);


        ValidateRect(hWnd, NULL);

        DeleteDC(hdcMem);

        DeleteDC(hdc);
        // TODO: Add any drawing code that uses hdc here...
        EndPaint(hWnd, &ps);

    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}


LRESULT CALLBACK ChildWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {


    case WM_HSCROLL:
    {


        switch (LOWORD(wParam))
        {
            // User dragged the scroll box.
        case SB_THUMBTRACK:
        {

            xNewPos = HIWORD(wParam);

            if (xNewPos > xOldPos)
            {

                xstart++;

            }


            if (xNewPos < xOldPos)
            {

                xstart--;

            }

            xOldPos = xNewPos;


            SetScrollPos(hWnd, SB_HORZ, xNewPos, TRUE);

            InvalidateRect(hWndMain, NULL, TRUE);
            InvalidateRect(hWnd, NULL, TRUE);

            break;
        }

        case SB_THUMBPOSITION:
        {

            xNewPos = HIWORD(wParam);
            SetScrollPos(hWnd, SB_HORZ, xNewPos, TRUE);

            break;

        }


        break;

        }



        break;

    }

    case WM_MOUSEMOVE:
    {
        xcur = GET_X_LPARAM(lParam);
        ycur = GET_Y_LPARAM(lParam);


        //if (pitch_note) 
       // {

         //   InvalidateRect(hWnd, NULL, TRUE);
       // }



    }
    break;


    case WM_LBUTTONDOWN:
    {

        //fwKeys = wParam;        // key flags 
        xPos = LOWORD(lParam);  // horizontal position of cursor 
        yPos = HIWORD(lParam);  // vertical position of cursor    


        point.x = xPos;
        point.y = yPos;


        if (piano)
        {
            piano_pat[cur_pat][cur_chan][xPos / 34 + xNewPos] = yPos / 12 + octave;
            piano_effects[cur_pat][cur_chan][xPos / 34 + xNewPos] = 0;

            if (chan_active[cur_chan] == true) play_preview = true;

        }

        if (apreggio)
        {

            piano_pat[cur_pat][cur_chan][xPos / 34 + xNewPos] = yPos / 12 + octave;

            piano_effects[cur_pat][cur_chan][xPos / 34 + xNewPos] = 1;

            effect_param[cur_pat][cur_chan][xPos / 34 + xNewPos] = MAKEWORD(2, 7);

            apreg_note = yPos / 12 + octave;

            apreg_param = MAKEWORD(2, 7);


            if (chan_active[cur_chan] == true) apreggio_preview = true;
            

        }

        if (pitch_note == true)
        {

            piano_pat[cur_pat][cur_chan][xPos / 34 + xNewPos] = yPos / 12 + octave;

            piano_effects[cur_pat][cur_chan][xPos / 34 + xNewPos] = 2;

            effect_param[cur_pat][cur_chan][xPos / 34 + xNewPos] = porta_value;


            if (chan_active[cur_chan] == true) pitch_preview = true;

        }




        if (vibactive)
        {
            piano_pat[cur_pat][cur_chan][xPos / 34 + xNewPos] = yPos / 12 + octave;

            piano_effects[cur_pat][cur_chan][xPos / 34 + xNewPos] = 4;

            int vib_preset = SendMessage(hWndComboParam, CB_GETCURSEL, (WPARAM)0, (LPARAM)0);

            vib_preset++;

            effect_param[cur_pat][cur_chan][xPos / 34 + xNewPos] = vib_preset;

            if (chan_active[cur_chan] == true) vib_preview = true;

        }





        vinc = 0;

        delta = Get_Note(yPos / 12 + octave) / SAMPLE_RATE;

        start_period = Get_Period(yPos / 12 + octave);

        InvalidateRect(hWnd, NULL, FALSE);
    }
    break;


    case WM_PAINT:
    {

        BITMAP bm;
        RECT Client_Rect;
        GetClientRect(hWnd, &Client_Rect);
        int win_width = Client_Rect.right - Client_Rect.left;
        int win_height = Client_Rect.bottom + Client_Rect.left;
        PAINTSTRUCT ps;
        HDC Memhdc;
        HDC hdc;
        HBITMAP Membitmap;
        hdc = BeginPaint(hWnd, &ps);
        Memhdc = CreateCompatibleDC(hdc);

        int xmin = 0; int ymin = 0;
        int xmax = win_width; int ymax = win_height;

        int x1, y1, x2, y2;

        SelectObject(Memhdc, piano_grid);

        GetObject(piano_grid, sizeof(bm), &bm);

        BitBlt(hdc, 0, 0, bm.bmWidth, bm.bmHeight, Memhdc, 0, 0, SRCCOPY);



        x = 0;

        for (int i = xNewPos; i < xNewPos + 16; i++)
        {


            switch(piano_effects[cur_pat][cur_chan][i])
            {

            case 0:
            {
                GetObject(piano_note, sizeof(bm), &bm);
                SelectObject(Memhdc, piano_note);
                break;
            }
                    
            case 1:
            {   
                GetObject(appregnote, sizeof(bm), &bm);
                SelectObject(Memhdc, appregnote);
                break;
            }

            case 2:
            {
                GetObject(pitchnote, sizeof(bm), &bm);
                SelectObject(Memhdc, pitchnote);
                break;
            }

            case 3:
            {
                GetObject(pitchnote, sizeof(bm), &bm);
                SelectObject(Memhdc, pitchnote);
                break;
            }

            case 4:
            {
                GetObject(vibnote, sizeof(bm), &bm);
                SelectObject(Memhdc, vibnote);
                break;
            }


            }

            BitBlt(hdc, x, (piano_pat[cur_pat][cur_chan][i] - octave) * 12, bm.bmWidth, bm.bmHeight, Memhdc, 0, 0, SRCCOPY);

            x += 34;

        }


        ValidateRect(hWnd, NULL);
        DeleteDC(Memhdc);
        DeleteDC(hdc);
        EndPaint(hWnd, &ps);

    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}


    LRESULT CALLBACK ChildWndProc2(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        switch (message)
        {


        case WM_HSCROLL:
        {


            switch (LOWORD(wParam))
            {
                // User dragged the scroll box.
            case SB_THUMBTRACK:
            {


                xNewPos2 = HIWORD(wParam);


                SetScrollPos(hWnd, SB_HORZ, xNewPos2, TRUE);

                InvalidateRect(hWndMain, NULL, TRUE);
                InvalidateRect(hWnd, NULL, TRUE);

                break;
            }

            case SB_THUMBPOSITION:
            {

                xNewPos2 = HIWORD(wParam);
                SetScrollPos(hWnd, SB_HORZ, xNewPos2, TRUE);

                break;

            }


            break;

            }

            break;

        }

        case WM_MOUSEMOVE:
        {
            xcur = GET_X_LPARAM(lParam);
            ycur = GET_Y_LPARAM(lParam);

        }
        break;


        case WM_LBUTTONDOWN:
        {

            //fwKeys = wParam;        // key flags 
            xPos = LOWORD(lParam);  // horizontal position of cursor 
            yPos = HIWORD(lParam);  // vertical position of cursor    


            point.x = xPos;
            point.y = yPos;

//            piano_pat[cur_pat][cur_chan][xPos / 34 + xNewPos] = yPos / 12 + octave;
            //int piano_pat[128][24][128] = { 0 };

            if (drum_pat[drum_pattern][(yPos / 70)][xPos / 42 + xNewPos2] == 0)
            {

                drum_pat[drum_pattern][(yPos / 70)][xPos / 42 + xNewPos2] = 1;

            }

            else

            {
                drum_pat[drum_pattern][(yPos / 70)][xPos / 42 + xNewPos2] = 0;

            }




            //InvalidateRect(hWnd, NULL, TRUE);
            InvalidateRect(hChildWindow2, NULL, TRUE);


        }
        break;


        case WM_PAINT:
        {

            BITMAP bm;
            RECT Client_Rect;
            GetClientRect(hWnd, &Client_Rect);
            int win_width = Client_Rect.right - Client_Rect.left;
            int win_height = Client_Rect.bottom + Client_Rect.left;
            PAINTSTRUCT ps;
            HDC Memhdc;
            HDC hdc;
            HBITMAP Membitmap;
            hdc = BeginPaint(hWnd, &ps);
            Memhdc = CreateCompatibleDC(hdc);

            int x = 0;
            int y = 0;


            for (int j = 0; j < 8; j++)
            {
                for (int i = xNewPos2; i < xNewPos + 16; i++)
                {


                    if (drum_pat[drum_pattern][j][i] == 1)
                    {

                        SelectObject(Memhdc, lit);

                        GetObject(lit, sizeof(bm), &bm);

                        BitBlt(hdc, x, y, bm.bmWidth, bm.bmHeight, Memhdc, 0, 0, SRCCOPY);

                    }

                    else
                    {

                        SelectObject(Memhdc, unlit);

                        GetObject(unlit, sizeof(bm), &bm);

                        BitBlt(hdc, x, y, bm.bmWidth, bm.bmHeight, Memhdc, 0, 0, SRCCOPY);


                    }

                    x += 42;
                }

                x = 0;

                y += 70;

            }

            ValidateRect(hWnd, NULL);
            DeleteDC(Memhdc);
            DeleteDC(hdc);
            EndPaint(hWnd, &ps);

        }
        break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        return 0;
    }


    LRESULT CALLBACK ChildWndProcPeak(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        switch (message)
        {
        case WM_TIMER:
        {

            switch (wParam)
            {
            case IDT_TIMER1:

                peak = 100;

                InvalidateRect(hChildPeak, NULL, TRUE);

                break;

            case IDT_TIMER2:

                peak--;

                if (peak < 0) peak = 0;

                InvalidateRect(hChildPeak, NULL, TRUE);

                break;
            }
            


        }
        break;

        case WM_PAINT:
        {

            BITMAP bm;
            RECT Client_Rect;
            GetClientRect(hWnd, &Client_Rect);
            int win_width = Client_Rect.right - Client_Rect.left;
            int win_height = Client_Rect.bottom + Client_Rect.left;
            PAINTSTRUCT ps;
            HDC Memhdc;
            HDC hdc;
            HBITMAP Membitmap;
            hdc = BeginPaint(hWnd, &ps);


            
            /*
            if (onoff)
            {

                peak = 100;
                onoff = false;

            }
            else
            {
                peak = 20;
                onoff = true;
            }
            */

            color = 0x000000FF;
            brush = CreateSolidBrush(color);
            SelectObject(hdc, brush);

            Rectangle(hdc, 0, peak - 100, 20, 100);

            color = 0x00000000;
            brush = CreateSolidBrush(color);
            SelectObject(hdc, brush);

            
            Rectangle(hdc, 0, 0, 20, 100 - peak);


            ValidateRect(hWnd, NULL);
            DeleteDC(hdc);
            EndPaint(hWnd, &ps);

        }
        break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        return 0;
    }
// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}


INT_PTR CALLBACK DONATE(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        
        good = LoadConfig();
        

        SetDlgItemInt(hDlg, IDC_TEXT, numberoftimes, false);

        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {            

            numberoftimes--;

            if (numberoftimes <= 0)
            {

                numberoftimes = 0;
                good = WriteConfig();

                PostQuitMessage(1);
            }


            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}




INT_PTR CALLBACK Setup(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
    {
        hWndCombo = GetDlgItem(hDlg, IDC_COMBO1);
    
        MIDIINCAPS caps;

        UINT nMidiDeviceNum;

        nMidiDeviceNum = midiInGetNumDevs();

        for (unsigned int i = 0; i < nMidiDeviceNum; ++i)
        {

            midiInGetDevCaps(i, &caps, sizeof(MIDIINCAPS));

            SendMessage(hWndCombo, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)caps.szPname);

        }

        SendMessage(hWndCombo, CB_SETCURSEL, 0, 0);

    
        hWndCombo2 = GetDlgItem(hDlg, IDC_COMBO2);


        int numDevices = Pa_GetDeviceCount();

        for (int i = 0; i < numDevices; i++)
        {
            deviceInfo = Pa_GetDeviceInfo(i);

            TCHAR devs[60];

           // mbstowcs(devs, deviceInfo->name, 40);

            SendMessage(hWndCombo2, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)devs);


        }

        SendMessage(hWndCombo2, CB_SETCURSEL, 0, 0);


    }


        return (INT_PTR)TRUE;

    case WM_COMMAND:

        if (HIWORD(wParam) == CBN_SELCHANGE)
        {

                ItemIndex = SendMessage(hWndCombo, (UINT)CB_GETCURSEL,
                (WPARAM)0, (LPARAM)0);

                InvalidateRect(hDlg, NULL, TRUE);

            /*/TCHAR  ListItem[256];
            (TCHAR)SendMessage((HWND)lParam, (UINT)CB_GETLBTEXT,
                (WPARAM)ItemIndex, (LPARAM)ListItem);
            MessageBox(hWndMain, (LPCWSTR)ListItem, TEXT("Item Selected"), MB_OK);
            */
        }

        if (LOWORD(wParam) == IDSAVE)
        {

            int error;
            FILE* filePtr;
            unsigned int count;

            error = fopen_s(&filePtr, "C:\\NetJamTracker\\config.dat", "wb");
            if (error != 0)
            {
                return false;
            }


            config.midiport = ItemIndex;

            for (int i = 0; i < 8; i++)
            {
                fwrite(&config, sizeof(config), 1, filePtr);
            }

            error = fclose(filePtr);
            if (error != 0)
            {
                return false;
            }

            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;

        }


        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        BITMAP bm;

        HDC hdc = BeginPaint(hDlg, &ps);

        HDC hdcMem = CreateCompatibleDC(hdc);

        textlength = wsprintf(str, L"Midi Port = %d", ItemIndex);
        TextOut(hdc, 350, 50, str, textlength);

        DeleteDC(hdc);
        EndPaint(hDlg, &ps);


    }


    }
    return (INT_PTR)FALSE;
}



bool Save()
{

    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED |
        COINIT_DISABLE_OLE1DDE);
    if (SUCCEEDED(hr))
    {
        IFileSaveDialog* pFileSave;

        // Create the FileOpenDialog object.
        hr = CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_ALL,
            IID_IFileSaveDialog, reinterpret_cast<void**>(&pFileSave));

        if (SUCCEEDED(hr))
        {

            IShellItem* folder;
            HRESULT result = SHCreateItemFromParsingName(L"C:\\NetJamTracker\\Songs\\", NULL, IID_PPV_ARGS(&folder));
            // Show the Open dialog box.
            hr = pFileSave->SetFolder(folder);

            hr = pFileSave->SetDefaultExtension(L"jam");

            UINT num = 1;

            COMDLG_FILTERSPEC ComDlgFS[1] =
            {
              {L"NetJamTracker JAM files", L"*.jam"} };

            hr = pFileSave->SetFileTypes(num, ComDlgFS);

            hr = pFileSave->Show(NULL);

            // Get the file name from the dialog box.
            if (SUCCEEDED(hr))
            {
                IShellItem* pItem;
                hr = pFileSave->GetResult(&pItem);
                if (SUCCEEDED(hr))
                {
                    PWSTR pszFilePath;

                    hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);


                    // Display the file name to the user.
                    if (SUCCEEDED(hr))
                    {
                        HANDLE hFile;
                        
                        DWORD dwBytesWritten = 0;
                        //StrCpyW(samp_files_dir[findex], pszFilePath);
                        hFile = CreateFile(pszFilePath,                // name of the write
                            GENERIC_WRITE,          // open for writing
                            0,                      // do not share
                            NULL,                   // default security
                            CREATE_ALWAYS,             // overwrite
                            FILE_ATTRIBUTE_NORMAL,  // normal file
                            NULL);

                        WriteFile(hFile, &piano_pat, sizeof(piano_pat), &dwBytesWritten, NULL);

                        WriteFile(hFile, &len, sizeof(len), &dwBytesWritten, NULL);

                        WriteFile(hFile, &samp_files_dir, sizeof(samp_files_dir), &dwBytesWritten, NULL);

                        WriteFile(hFile, &samp_files, sizeof(samp_files), &dwBytesWritten, NULL);

                        WriteFile(hFile, &chan_active, sizeof(chan_active), &dwBytesWritten, NULL);

                        init_pot();

                        WriteFile(hFile, &pot, sizeof(pot), &dwBytesWritten, NULL);

                        WriteFile(hFile, &num_pat, sizeof(num_pat), &dwBytesWritten, NULL);

                        CloseHandle(hFile);

                        MessageBoxW(NULL, pszFilePath, L"File Path", MB_OK);
                        CoTaskMemFree(pszFilePath);
                    }


                    pItem->Release();
                }
            }
            pFileSave->Release();
        }
        CoUninitialize();
    }


  

    

    return true;

}

bool Load()
{

    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED |
        COINIT_DISABLE_OLE1DDE);
    if (SUCCEEDED(hr))
    {
        IFileOpenDialog* pFileOpen;

        // Create the FileOpenDialog object.
        hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
            IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

        if (SUCCEEDED(hr))
        {
            IShellItem* folder;
            HRESULT result = SHCreateItemFromParsingName(L"C:\\NetJamTracker\\Songs\\", NULL, IID_PPV_ARGS(&folder));
            // Show the Open dialog box.
            hr = pFileOpen->SetFolder(folder);

            hr = pFileOpen->Show(NULL);

            // Get the file name from the dialog box.
            if (SUCCEEDED(hr))
            {
                IShellItem* pItem;
                hr = pFileOpen->GetResult(&pItem);
                if (SUCCEEDED(hr))
                {
                    PWSTR pszFilePath;

                    hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);


                    // Display the file name to the user.
                    if (SUCCEEDED(hr))
                    {

                        HANDLE hf;
                        DWORD dwBytesRead = 0;

                        hf = CreateFile(pszFilePath,
                            GENERIC_READ,
                            0,
                            (LPSECURITY_ATTRIBUTES)NULL,
                            OPEN_EXISTING,
                            FILE_ATTRIBUTE_NORMAL,
                            (HANDLE)NULL);

                        ReadFile(hf, &piano_pat, sizeof(piano_pat), &dwBytesRead, NULL);

                        ReadFile(hf, &len, sizeof(len), &dwBytesRead, NULL);

                        ReadFile(hf, &samp_files_dir, sizeof(samp_files_dir), &dwBytesRead, NULL);

                        ReadFile(hf, &samp_files, sizeof(samp_files), &dwBytesRead, NULL);

                        ReadFile(hf, &chan_active, sizeof(chan_active), &dwBytesRead, NULL);

                        ReadFile(hf, &pot, sizeof(pot), &dwBytesRead, NULL);

                        ReadFile(hf, &num_pat, sizeof(num_pat), &dwBytesRead, NULL);

                        CloseHandle(hf);

                        //MessageBoxW(NULL, pszFilePath, L"File Path", MB_OK);
                        CoTaskMemFree(pszFilePath);
                    }


                    pItem->Release();
                }
            }
            pFileOpen->Release();
        }
        CoUninitialize();
    }


    for (int i = 0; i < 16; i++)
    {

        if (chan_active[i])
        {
            if (!LoadWaveFile(samp_files_dir[i], i))
            {
                MessageBox(hWndMain, L".WAV File Missing", L"Error", MB_OK | MB_ICONEXCLAMATION);
            }
        }
    }


    
        SendMessage(listbox, LB_RESETCONTENT, (WPARAM) 0,
            (LPARAM) 0);


        for (int i = 0; i < num_pat; i++)
        {

            TCHAR pstr[10];

            textlength = wsprintf(pstr, L"%d", pot[i]);

            SendMessage(listbox, LB_ADDSTRING, NULL,
                (LPARAM) pstr);
        }


    InvalidateRect(hWndMain, NULL, FALSE);

    InvalidateRect(hChildWindow1, NULL, FALSE);

    return true;

}

float Get_Note(unsigned char note)
{

    float period = 7680.f - note * 64.f;

    return 8363.f * powf(2.f, (4608.f - period) / 768.f);
}


float Get_Period(unsigned char note)
{

    float period = 7680.f - note * 64.f;

    return period;
}


float Get_Frequency(float period)
{

    return 8363.f * powf(2.f, (4608.f - period) / 768.f);
}



bool LoadWaveFile(TCHAR *filename, int findex)
{
    HANDLE hf;

    hf = CreateFile(filename,
        GENERIC_READ,
        0,
        (LPSECURITY_ATTRIBUTES)NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        (HANDLE)NULL);



    // Read in the wave file header.
    ReadFile(hf, &waveFileHeader, sizeof(waveFileHeader), NULL, NULL);

    // Check that the chunk ID is the RIFF format.
    if ((waveFileHeader.chunkId[0] != 'R') || (waveFileHeader.chunkId[1] != 'I') ||
        (waveFileHeader.chunkId[2] != 'F') || (waveFileHeader.chunkId[3] != 'F'))
    {
        return false;
    }

    // Check that the file format is the WAVE format.
    if ((waveFileHeader.format[0] != 'W') || (waveFileHeader.format[1] != 'A') ||
        (waveFileHeader.format[2] != 'V') || (waveFileHeader.format[3] != 'E'))
    {
        return false;
    }

    // Check that the sub chunk ID is the fmt format.
    if ((waveFileHeader.subChunkId[0] != 'f') || (waveFileHeader.subChunkId[1] != 'm') ||
        (waveFileHeader.subChunkId[2] != 't') || (waveFileHeader.subChunkId[3] != ' '))
    {
        return false;
    }



    // Check that the audio format is WAVE_FORMAT_PCM.
    if (waveFileHeader.audioFormat != WAVE_FORMAT_PCM)
    {
        return false;
    }

    // Check that the wave file was recorded in stereo format.
    if (waveFileHeader.numChannels != 1)
    {
        return false;
    }

    // Check that the wave file was recorded at a sample rate of 44.1 KHz.
    if (waveFileHeader.sampleRate != 44100)
    {
        return false;
    }

    // Ensure that the wave file was recorded in 16 bit format.
    if (waveFileHeader.bitsPerSample != 16)
    {
        return false;
    }

    // Check for the data chunk header.
    if ((waveFileHeader.dataChunkId[0] != 'd') || (waveFileHeader.dataChunkId[1] != 'a') ||
        (waveFileHeader.dataChunkId[2] != 't') || (waveFileHeader.dataChunkId[3] != 'a'))
    {
        return false;
    }

    // Set the wave format of secondary buffer that this wave file will be loaded onto.
    waveFormat.wFormatTag = WAVE_FORMAT_PCM;
    waveFormat.nSamplesPerSec = 44100;
    waveFormat.wBitsPerSample = 16;
    waveFormat.nChannels = 1;
    waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
    waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
    waveFormat.cbSize = 0;


    if (waveData != NULL)
    {
        delete[] waveData;
    }


    if (waveFloat[findex] != NULL)
    {
        delete[] waveFloat[findex];
    }


    // Create a temporary buffer to hold the wave file data.
    waveData = new int16_t[waveFileHeader.dataSize / 2];
    waveFloat[findex] = new float[waveFileHeader.dataSize / 2];



    // Read in the wave file data into the newly created buffer.
    ReadFile(hf, waveData, waveFileHeader.dataSize, NULL, NULL);

    CloseHandle(hf);


    for (int i = 0; i < waveFileHeader.dataSize / 2; i++)
    {
        waveFloat[findex][i] = waveData[i] * (1.0f / 32768.0f);
    }




    bufferSize[findex] = waveFileHeader.dataSize / 2;

}


bool play_snd(int sindex)
{

    PlayFormat.chunkId[0] = 'R';
    PlayFormat.chunkId[1] = 'I';
    PlayFormat.chunkId[2] = 'F';
    PlayFormat.chunkId[3] = 'F';


    PlayFormat.format[0] = 'W';
    PlayFormat.format[1] = 'A';
    PlayFormat.format[2] = 'V';
    PlayFormat.format[3] = 'E';

    PlayFormat.subChunkId[0] = 'f';
    PlayFormat.subChunkId[0] = 'm';
    PlayFormat.subChunkId[0] = 't';
    PlayFormat.subChunkId[0] = ' ';

    PlayFormat.dataChunkId[0] = 'd';
    PlayFormat.dataChunkId[1] = 'a';
    PlayFormat.dataChunkId[2] = 't';
    PlayFormat.dataChunkId[3] = 'a';


    PlayFormat.chunkSize = 44 + (bufferSize[0] * 2) - 8;
    PlayFormat.subChunkSize = 16;
    PlayFormat.audioFormat = 1;
    PlayFormat.numChannels = 1;
    PlayFormat.sampleRate = 44100;
    PlayFormat.bitsPerSample = 16;
    PlayFormat.blockAlign = 2;
    PlayFormat.bytesPerSecond = PlayFormat.sampleRate / 8;
    PlayFormat.dataSize = bufferSize[0];

    // if (waveStruct != NULL) delete[] waveStruct;
    
    char* waveStruct;

    waveStruct = new char(sizeof(WaveHeaderType) + bufferSize[0]);


     memcpy(waveStruct, &PlayFormat, sizeof(WaveHeaderType));

     waveStruct += sizeof(WaveHeaderType);

     memcpy(waveStruct, waveData, bufferSize[0]);
   
  
     PlaySound((LPCWSTR)waveStruct, NULL, SND_MEMORY | SND_SYNC);

     //PlaySoundA(waveStruct, hInst, SND_MEMORY);
     return true;
}


void Init_PortAudio()
{

    err = Pa_Initialize();


    int numDevices = Pa_GetDeviceCount();
    if (numDevices < 0)
    {
        MessageBox(hWndMain, L"No Sound Card Detected", L"Error", MB_OK | MB_ICONEXCLAMATION);
    
    }

    for (int i = 0; i < numDevices; i++)
    {
        deviceInfo = Pa_GetDeviceInfo(i);
    }

    outputParameters.device = Pa_GetDefaultOutputDevice();
    if (outputParameters.device == paNoDevice) {

        PostQuitMessage(1);

    }
    outputParameters.channelCount = 1;
    outputParameters.sampleFormat = paFloat32;
    outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputParameters.device)->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;

    err = Pa_OpenStream(
        &stream,
        NULL, //no input
        &outputParameters,
        SAMPLE_RATE,
        FRAMES_PER_BUFFER,
        paClipOff,
        patestCallback,
        &data);
    if (err != paNoError) PostQuitMessage(1);

    sprintf_s(data.message, "No Message");
    err = Pa_SetStreamFinishedCallback(stream, &StreamFinished);
    if (err != paNoError) PostQuitMessage(1);

    err = Pa_StartStream(stream);
    if (err != paNoError) PostQuitMessage(1);

}


void CALLBACK MidiInProc(HMIDIIN hMidiIn, UINT wMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2)
{
    switch (wMsg) {
    case MIM_OPEN:
        printf("wMsg=MIM_OPEN\n");
        break;
    case MIM_CLOSE:
        printf("wMsg=MIM_CLOSE\n");
        break;
    case MIM_DATA:

        midi_note = HIBYTE(dwParam1);
        midi_status = LOBYTE(dwParam1);

        velocity = LOBYTE(HIWORD(dwParam1));



        if (midi_status == 0xB0)
        {
        
            midi_count++;


            if (midi_note == 3 && midi_count == 2)
            {

                cur_chan++;

                while (chan_active[cur_chan] != true)
                {
                    cur_chan++;
                }
                midi_count = 0;

                
                ychan = (cur_chan * 32) + 200;


            }
        
            if (midi_note == 2 && midi_count == 2)
            {

                cur_chan--;

                while (chan_active[cur_chan] != true)
                {
                    cur_chan--;
                }
                midi_count = 0;

                ychan = (cur_chan * 32) + 200;


            }

        }
        if (midi_status == 0x90) // Note On
        {

            midi_chan[vx] = midi_note;

            midi_delta[vx] = Get_Note(midi_note) / SAMPLE_RATE;;

            minc[vx] = 0;

            vx++;

            if (vx >= 9) vx = 0;

        }

        if (midi_status == 0x80) // Note Off
        {
            
            int i = 0;
            int k = vx;

            while (i < 10)
            {

                if (midi_chan[k] == midi_note) midi_chan[k] = 0;

                k++;

                if (k >= 9) k = 0;

                i++;

            }
            
        }

        InvalidateRect(hWndMain, NULL, TRUE);


        break;
    case MIM_LONGDATA:
        printf("wMsg=MIM_LONGDATA\n");
        break;
    case MIM_ERROR:
        printf("wMsg=MIM_ERROR\n");
        break;
    case MIM_LONGERROR:
        printf("wMsg=MIM_LONGERROR\n");
        break;
    case MIM_MOREDATA:
        printf("wMsg=MIM_MOREDATA\n");
        break;
    default:
        printf("wMsg = unknown\n");
        break;
    }
    return;
}



bool InitMidi()
{
    HMIDIIN hMidiDevice = NULL;;
    UINT nMidiDeviceNum;
    MIDIINCAPS caps;
    MMRESULT rv;
 
    nMidiDeviceNum = midiInGetNumDevs();
    if (nMidiDeviceNum == 0) {

        MessageBox(hWndMain, L"No MIDI Device Detected", L"Error", MB_OK | MB_ICONEXCLAMATION);

        return false;
    }
    

    rv = midiInOpen(&hMidiDevice, nMidiPort, (DWORD_PTR)(void*)MidiInProc, 0, CALLBACK_FUNCTION);
    if (rv != MMSYSERR_NOERROR) {

        MessageBox(hWndMain, L"MIDI Device Error", L"Error", MB_OK | MB_ICONEXCLAMATION);
        return -1;
    }

    midiInStart(hMidiDevice);
  
}

void start_pattern()
{


    //init_pot();

        
    for (int chan = 0; chan < 16; chan++)
    {
        if (piano_pat[cur_pat][chan][cur_note] != 0)
        {
            note = Get_Note(piano_pat[cur_pat][chan][cur_note]);

            jinc[chan] = note / SAMPLE_RATE;

            active_note[chan] = true;

            ji[chan] = 0;

        }

        if (piano_effects[cur_pat][chan][cur_note] > 0)
        {
            effect_active[chan] = true;

            start_period = Get_Period(piano_pat[cur_pat][chan][cur_note]);
        }


    }
   


}


void start_drums()
{
 
    for (int chan = 0; chan < 8; chan++)
    {
        if (drum_pat[drum_pattern][chan][drum_pos] == 1)
        {

            xinc[chan] = 1;

            drum_active[chan] = true;

            xi[chan] = 0;
        }

    }

}



void start_pattern_mix()
{

    init_pot();
    
    for (int chan = 0; chan < num_chan; chan++)
    {
        if (piano_pat[chan][cur_note] != 0)
        {
            note = Get_Note(piano_pat[cur_pat][chan][cur_note]);

            dinc[chan][0] = note / SAMPLE_RATE;

            note_active[chan][0] = true;

            si[chan][0] = 0;

            j[chan]++;

        }
    }

}

void init_pot()
{

    for (int i = 0; i < num_pat; i++)
    {
        TCHAR str[10];

        int err = SendMessage(listbox, LB_GETTEXT, (WPARAM)i, (LPARAM)str);

        if (err == LB_ERR)
        {
            MessageBox(hWndMain, L"No Patterns Inserted", L"Error", MB_OK | MB_ICONEXCLAMATION);
        }

        pot[i] = std::stoi(str);



    }
}

bool LoadConfig()
{

    HANDLE hf;
    DWORD dwBytesRead = 0;

    hf = CreateFile(L"Config.xxx",
        GENERIC_READ,
        0,
        (LPSECURITY_ATTRIBUTES)NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        (HANDLE)NULL);

    ReadFile(hf, &numberoftimes, sizeof(numberoftimes), &dwBytesRead, NULL);

    CloseHandle(hf);

    return true;

}


bool WriteConfig()
{

    HANDLE hFile;

    DWORD dwBytesWritten = 0;
    //StrCpyW(samp_files_dir[findex], pszFilePath);
    hFile = CreateFile(L"Config.xxx",                // name of the write
        GENERIC_WRITE,          // open for writing
        0,                      // do not share
        NULL,                   // default security
        CREATE_ALWAYS,             // overwrite
        FILE_ATTRIBUTE_NORMAL,  // normal file
        NULL);


    WriteFile(hFile, &numberoftimes, sizeof(numberoftimes), &dwBytesWritten, NULL);

    CloseHandle(hFile);

    return true;

}


INT_PTR CALLBACK OSC(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
    {
        /*/CheckRadioButton(hDlg, IDC_RADIO1, IDC_RADIO3, IDC_RADIO1);

        HWND hWndSlider = GetDlgItem(hDlg, IDC_SLIDER1);

        SendMessageW(hWndSlider, TBM_SETRANGE, TRUE, (LPARAM)MAKELONG(180, 360));

        // SendMessageW(hWndSlider, TBM_SETPOS, TRUE, 180);

        hWndSlider = GetDlgItem(hDlg, IDC_SLIDER2);

        SendMessageW(hWndSlider, TBM_SETPOS, TRUE, 50);

        InvalidateRect(hDlg, NULL, TRUE);
        */
        InvalidateRect(hDlg, NULL, TRUE);

        return (INT_PTR)TRUE;
    }

    case WM_COMMAND:
        if (LOWORD(wParam) == IDEXIT || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }

        if (LOWORD(wParam) == IDPLAY)
        {

            osc_preview = true;

            break;
        }

        if (LOWORD(wParam) == IDMIX)
        {

            osc_mix = true;

            break;
        }


        if (LOWORD(wParam) == IDC_RADIO1)
        {
            sine_rad = true;
            saw_rad = false;
            tri_rad = false;
        }

        if (LOWORD(wParam) == IDC_RADIO2)
        {
            sine_rad = false;
            saw_rad = true;
            tri_rad = false;
        }

        if (LOWORD(wParam) == IDC_RADIO3)
        {
            sine_rad = false;
            saw_rad = false;
            tri_rad = true;
        }

        InvalidateRect(hDlg, NULL, TRUE);

        break;


    case WM_LBUTTONDOWN:
    {

    }
    break;

    case WM_HSCROLL:
    {


        switch (LOWORD(wParam))
        {
            // User dragged the slider
        case SB_THUMBTRACK:
        {

            frequency[vibindex] = HIWORD(wParam);

            SetDlgItemInt(hDlg, IDC_EDIT1, HIWORD(wParam), false);
            InvalidateRect(hDlg, NULL, TRUE);
        }
        break;

        }
    }
    break;

    case WM_VSCROLL:
    {


        switch (LOWORD(wParam))
        {
            // User dragged the slider
        case SB_THUMBTRACK:
        {


            amplitude[vibindex] = (float)HIWORD(wParam) / ampdiv;
            SetDlgItemInt(hDlg, IDC_EDIT2, HIWORD(wParam), false);
            InvalidateRect(hDlg, NULL, TRUE);
        }
        break;

        }
    }
    break;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        BITMAP bm;

        HDC hdc = BeginPaint(hDlg, &ps);

        HDC hdcMem = CreateCompatibleDC(hdc);

        int y = 0;



        x = 10;



        float inc = two_pi / 128 - 1;

        float angle = 0.0f;

        float frequency = 2.0f;

        for (int i = 0; i < 128; i++)
        {

            osc1[i] = sin(angle);

            angle += inc;

            SetPixel(hdc, x, osc1[i] * 100 + 150, RGB(0, 0, 255));

            x++;



        }

        angle = 0.0f;

        for (int i = 0; i < 128; i++)
        {

            osc2[i] = sin(angle * frequency);

            angle += inc;

            SetPixel(hdc, x, osc2[i] * 100 + 300, RGB(0, 0, 255));

            x++;



        }






    }
    break;


    }
    return (INT_PTR)FALSE;
}



// Message handler for about box.
INT_PTR CALLBACK VIB(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
    {
        CheckRadioButton(hDlg, IDC_RADIO1, IDC_RADIO3, IDC_RADIO1);

        HWND hWndSlider = GetDlgItem(hDlg, IDC_SLIDER1);

        SendMessageW(hWndSlider, TBM_SETRANGE, TRUE, (LPARAM)MAKELONG(180, 360));

       // SendMessageW(hWndSlider, TBM_SETPOS, TRUE, 180);

        hWndSlider = GetDlgItem(hDlg, IDC_SLIDER2);

        SendMessageW(hWndSlider, TBM_SETPOS, TRUE, 50);

        InvalidateRect(hDlg, NULL, TRUE);

        return (INT_PTR)TRUE;
    }

    case WM_COMMAND:
        if (LOWORD(wParam) == IDEXIT || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }

        if (LOWORD(wParam) == IDVIB)
        {

            vib_preview = true;

        }

        break;

        if (LOWORD(wParam) == IDC_RADIO1)
        {
            sine_rad = true;
            saw_rad = false;
            tri_rad = false;
        }

        if (LOWORD(wParam) == IDC_RADIO2)
        {
            sine_rad = false;
            saw_rad = true;
            tri_rad = false;
        }

        if (LOWORD(wParam) == IDC_RADIO3)
        {
            sine_rad = false;
            saw_rad = false;
            tri_rad = true;
        }

        InvalidateRect(hDlg, NULL, TRUE);

        break;


    case WM_LBUTTONDOWN:
    {

    }
    break;

    case WM_HSCROLL:
    {


        switch (LOWORD(wParam))
        {
            // User dragged the slider
        case SB_THUMBTRACK:
        {

            frequency[vibindex] = HIWORD(wParam);

            SetDlgItemInt(hDlg, IDC_EDIT1, HIWORD(wParam), false);
            InvalidateRect(hDlg, NULL, TRUE);
        }
        break;

        }
    }
    break;

    case WM_VSCROLL:
    {


        switch (LOWORD(wParam))
        {
            // User dragged the slider
        case SB_THUMBTRACK:
        {


            amplitude[vibindex] = (float)HIWORD(wParam) / ampdiv;
            SetDlgItemInt(hDlg, IDC_EDIT2, HIWORD(wParam), false);
            InvalidateRect(hDlg, NULL, TRUE);
        }
        break;

        }
    }
    break;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        BITMAP bm;

        HDC hdc = BeginPaint(hDlg, &ps);

        HDC hdcMem = CreateCompatibleDC(hdc);

        int y = 0;


        if (sine_rad)
        {

            init_vib();
            //textlength = wsprintf(str, L"%d", i);
            //TextOut(hdc, 0, 0, str, textlength);
            //vib_wav[i] = sinf(((float)i / (float)phase) * M_PI * 2.) * slider;
        }


        /*/if (saw_rad)
        {
            vib_wav[i] = y;

            y += saw_y;

            if (y == phase)
            {
                saw_y = -1;
            }

            if (y == -phase)
            {
                saw_y = 1;
            }

        }

            if (tri_rad)
            {

                vib_wav[i] = y;

                y += saw_y;

                if (y == phase)
                {



                    y = -phase;
                    saw_y = 1;
                }

                if (y == -phase)
                {
                    MoveToEx(hdc, x + 30, -phase, NULL);

                    LineTo(hdc, x + 30, abs(y));
                    saw_y = 1;
                }


            }
            */

        x = 10;

        for (int i = 0; i < vib_len; i++)
        {
            SetPixel(hdc, x, vib_wav[vibindex][i] * 100 + 150, RGB(0, 0, 255));

            x++;
        }


        //SelectObject(hdcMem, play);

        //GetObject(play, sizeof(bm), &bm);

        //BitBlt(hdc, 0, 0, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);
    }
    break;


    }
    return (INT_PTR)FALSE;
}

void init_vib()
{

    float inc = two_pi / 128 - 1;

    float angle = 0.0f;

    float frequency = 2.0f;

    for (int i = 0; i < 128; i++)
    {
        osc1[i] = sin(angle);

        angle += inc;

    }



}


void start_vib()
{


    for (int j = 0; j < 16; j++)
    {
        vib_len = 0;

        angle = 0;

        for (int i = 0; i < frequency[j] * 2; i++)
        {
            radians = M_PI / frequency[j];

            vib_wav[j][vib_len] = amplitude[j] * sin(angle);

            angle += radians;

            vib_len++;
        }
    }

}


int SockConnect()

{

    WSADATA wsaData;
    SOCKET theSocket = INVALID_SOCKET;
    struct addrinfo* result = NULL,
        * ptr = NULL,
        hints;
    int iResult;


    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    iResult = getaddrinfo("127.0.0.1", DEFAULT_PORT, &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // Attempt to connect to an address until one succeeds
    for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

        // Create a SOCKET for connecting to server
        theSocket = socket(ptr->ai_family, ptr->ai_socktype,
            ptr->ai_protocol);
        if (theSocket == INVALID_SOCKET) {
            printf("socket failed with error: %ld\n", WSAGetLastError());
            WSACleanup();
            return 1;
        }

        // Connect to server.
        iResult = connect(theSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(theSocket);
            theSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if (theSocket == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        WSACleanup();
        return 1;
    }

    return 0;

}

int SockListen(void)

{
    WSADATA wsaData;
    int iResult;

    SOCKET ListenSocket = INVALID_SOCKET;

    struct addrinfo* result = NULL;
   struct addrinfo hints;

    int iSendResult;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the server address and port
    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // Create a SOCKET for the server to listen for client connections.
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    // Setup the TCP listening socket
    iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    freeaddrinfo(result);

    iResult = listen(ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    // Accept a client socket
    theSocket = accept(ListenSocket, NULL, NULL);
    if (theSocket == INVALID_SOCKET) {
        printf("accept failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    // No longer need server socket
    closesocket(ListenSocket);


}



INT_PTR CALLBACK TCP(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:


        SetDlgItemText(hDlg, IDC_CHAT, L"Hello");


        return (INT_PTR)TRUE;

    case WINSOCK_CALL:

        switch (WSAGETSELECTEVENT(lParam))

        {

        case FD_READ:

            char buff[256];

            ZeroMemory(buff, 256);

            UINT size = recv((SOCKET)wParam, buff, 256, 0);

            SetDlgItemTextA(hDlg, IDC_CHAT, buff);

            break;
        }
    
    
    case WM_COMMAND:

        if (LOWORD(wParam) == IDC_LISTEN)
        {

            SockListen();
            
            rc = WSAAsyncSelect(theSocket, hDlg, WINSOCK_CALL, FD_READ);


        }

        if (LOWORD(wParam) == IDC_CONNECT)
        {
            
            SockConnect();

            rc = WSAAsyncSelect(theSocket, hDlg, WINSOCK_CALL, FD_READ);

        }


        if (LOWORD(wParam) == IDC_SEND)
        {

            char buf_in[256];

            int size = GetDlgItemTextA(hDlg, IDC_INPUT, buf_in, 256);

            //strcat(buf_in, "\r\n");

            send(theSocket, buf_in, size, 0);



            //strcat(buf_in, "\r\n");

            //strcat(buf, buf_in);


            //SetDlgItemTextA(hDlg, IDC_CHAT, buf);

        }


        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
          

    }
    return (INT_PTR)FALSE;
}


