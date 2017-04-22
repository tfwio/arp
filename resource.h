#define PLUG_MFR "tfw.io"
#define PLUG_NAME "Arpeggio"

#define PLUG_CLASS_NAME Arpeggio

#define BUNDLE_MFR "tfw.io"
#define BUNDLE_NAME "Arpeggio"

#define PLUG_ENTRY Arpeggio_Entry
#define PLUG_VIEW_ENTRY Arpeggio_ViewEntry

#define PLUG_ENTRY_STR "Arpeggio_Entry"
#define PLUG_VIEW_ENTRY_STR "Arpeggio_ViewEntry"

#define VIEW_CLASS Arpeggio_View
#define VIEW_CLASS_STR "Arpeggio_View"

// Format        0xMAJR.MN.BG - in HEX! so version 10.1.5 would be 0x000A0105
#define PLUG_VER 0x00010000
#define VST3_VER_STR "1.0.0"

// http://service.steinberg.de/databases/plugin.nsf/plugIn?openForm
// 4 chars, single quotes. At least one capital letter
#define PLUG_UNIQUE_ID 'Arp3'
// make sure this is not the same as BUNDLE_MFR
#define PLUG_MFR_ID 'TFW7'

// ProTools stuff

#if (defined(AAX_API) || defined(RTAS_API)) && !defined(_PIDS_)
  #define _PIDS_
  const int PLUG_TYPE_IDS[2] = {'EFN1', 'EFN2'};
  const int PLUG_TYPE_IDS_AS[2] = {'EFA1', 'EFA2'}; // AudioSuite
#endif

#define PLUG_MFR_PT "tfw.io\ntfw.io\ntfw7"
#define PLUG_NAME_PT "Arpeggio\nIPEF"
#define PLUG_TYPE_PT "Effect"
#define PLUG_DOES_AUDIOSUITE 0

/* PLUG_TYPE_PT can be "None", "EQ", "Dynamics", "PitchShift", "Reverb", "Delay", "Modulation", 
"Harmonic" "NoiseReduction" "Dither" "SoundField" "Effect" 
instrument determined by PLUG _IS _INST
*/

#define PLUG_LATENCY 0
#define PLUG_DOES_MIDI 1

#if defined(AS_INSTRUMENT)
#define PLUG_IS_INST 1
#else
#define PLUG_IS_INST 0
#endif

#if PLUG_IS_INST
//#  define PLUG_CHANNEL_IO ""
# define PLUG_CHANNEL_IO "1-1 2-2"
#else
#  define PLUG_CHANNEL_IO ""
#endif

// if this is 0 RTAS can't get tempo info

#define PLUG_DOES_STATE_CHUNKS 0

#define prefer_knob    "resources/img/demo_f64-2_48-v.png"
#define silver_knob36c "resources/img/knob_36_silver-c.png"
#define silver_knob48  "resources/img/knob_48_silver.png"
#define silver_knob2   "resources/img/knob_48_silver-c.png"
#define silver_tiny24  "resources/img/simple_e_24-v.png"
// IMAGES knob_48_silver demo_f64-2_36-v
#define RES_KNOB_FN     silver_knob36c
#define RES_IMG_KNOB24_FN "resources/img/simple_e_24-v.png"
#define RES_BG_FN       "resources/img/arpeggio.png"
#define RES_BUTTON_FN   "resources/img/img_sync.png"
#define RES_IMG_1_2_FN  "resources/img/img_1_2.png"
#define RES_IMG_1_3_FN  "resources/img/img_1_3.png"
#define RES_IMG_1_4_FN  "resources/img/img_1_4.png"
#define RES_IMG_1_6_FN  "resources/img/img_1_6.png"
#define RES_IMG_1_8_FN  "resources/img/img_1_8.png"
#define RES_IMG_1_16_FN "resources/img/img_1_16.png"
#define RES_IMG_3_8_FN  "resources/img/img_3_8.png"
#define RES_IMG_3_6_FN  "resources/img/img_3_6.png"
#define RES_IMG_3_4_FN  "resources/img/img_3_4.png"
#define RES_IMG_3_3_FN  "resources/img/img_3_3.png"
#define RES_IMG_3_2_FN  "resources/img/img_3_2.png"
#define RES_IMG_3_1_FN  "resources/img/img_3_1.png"
#define RES_IMG_4_8_FN  "resources/img/img_4_8.png"
#define RES_IMG_4_6_FN  "resources/img/img_4_6.png"
#define RES_IMG_4_4_FN  "resources/img/img_4_4.png"
#define RES_IMG_4_3_FN  "resources/img/img_4_3.png"
#define RES_IMG_4_2_FN  "resources/img/img_4_2.png"
#define RES_IMG_4_1_FN  "resources/img/img_4_1.png"
#define RES_IMG_UP_FN   "resources/img/img_up.png"
#define RES_IMG_DN_FN   "resources/img/img_dn.png"
#define RES_IMG_UD_FN   "resources/img/img_ud.png"
#define RES_IMG_DU_FN   "resources/img/img_du.png"
#define RES_IMG_PANIC_FN "resources/img/img_panic.png"
#define RES_IMG_KEYSE_FN "resources/img/img_keyse.png"
#define RES_IMG_KEYSI_FN "resources/img/img_keysi.png"
#define RES_IMG_TAMP_FN "resources/img/img_key_amp.png"

#define RES_KNOB_ID    101
#define RES_BG_ID      102

#define RES_BUTTON_ID  103
#define RES_IMG_1_2_ID 104
#define RES_IMG_1_3_ID 105
#define RES_IMG_1_4_ID 106
#define RES_IMG_1_6_ID 107
#define RES_IMG_1_8_ID 108
#define RES_IMG_1_16_ID 109

#define RES_IMG_4_8_ID 110
#define RES_IMG_4_6_ID 111
#define RES_IMG_4_4_ID 112
#define RES_IMG_4_3_ID 113
#define RES_IMG_4_2_ID 114
#define RES_IMG_4_1_ID 115

#define RES_IMG_3_8_ID 116
#define RES_IMG_3_6_ID 117
#define RES_IMG_3_4_ID 118
#define RES_IMG_3_3_ID 119
#define RES_IMG_3_2_ID 120
#define RES_IMG_3_1_ID 121

#define RES_IMG_UP_ID  122
#define RES_IMG_DN_ID  123
#define RES_IMG_UD_ID  124
#define RES_IMG_DU_ID  125

#define RES_IMG_PANIC_ID  126

#define RES_IMG_KEYSE_ID  127
#define RES_IMG_KEYSI_ID  128

#define RES_IMG_KNOB24_ID 129

#define RES_IMG_TAMP_ID   130

// GUI default dimensions
#define GUI_WIDTH  615
#define GUI_HEIGHT 125

// on MSVC, you must define SA_API in the resource editor preprocessor macros as well as the c++ ones
#if defined(SA_API)
#include "app_wrapper/app_resource.h"
#endif

// vst3 stuff
#define MFR_URL "github.com/tfwio"
#define MFR_EMAIL "tfwroble@gmail.com"
#define EFFECT_TYPE_VST3 "Fx"

/* "Fx|Analyzer"", "Fx|Delay", "Fx|Distortion", "Fx|Dynamics", "Fx|EQ", "Fx|Filter",
"Fx", "Fx|Instrument", "Fx|InstrumentExternal", "Fx|Spatial", "Fx|Generator",
"Fx|Mastering", "Fx|Modulation", "Fx|PitchShift", "Fx|Restoration", "Fx|Reverb",
"Fx|Surround", "Fx|Tools", "Instrument", "Instrument|Drum", "Instrument|Sampler",
"Instrument|Synth", "Instrument|Synth|Sampler", "Instrument|External", "Spatial",
"Spatial|Fx", "OnlyRT", "OnlyOfflineProcess", "Mono", "Stereo",
"Surround"
*/
