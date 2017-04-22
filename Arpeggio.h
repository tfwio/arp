#ifndef __ARPEGGIO__
#define __ARPEGGIO__
#if _WIN32
#pragma once
#endif

#define COMPILE_INST 0
#include "IPlug_include_in_plug_hdr.h"

#include "ICControls.h"
#include <memory>
#include "IMidiQueue.h"
#include "MidiNode.h"

typedef std::unique_ptr<ISwitchControl> IPlugSwitch;
typedef std::unique_ptr<IKnobText>      IPlugKnob;

enum {
  PARAM_ARP_REST_OFF = 0,
  PARAM_ARP_REST_ON,
  pArpRestCount
}; // parameter helpers

enum KPARA {
  kNUM_BEAT,
  kNUM_DIVS,
  kARP_MODE,
  kARP_SYNC,
  kKEY_MOD,  // 
  kKEY_TRA,  // (bool) tells us that tracking is on or off.
  kKEY_AMP,  // if (!kKEY_TRA) key_amplitude=[1-127]
  kKEY_GATE, // note-on length
  kParamCount,
  kResetCount = -1
}; // parameter helpers


#define STR_BEAT_N    "quarters"
#define STR_BEAT_NICK STR_BEAT_N
#define STR_DIVS_N    "divs"
#define STR_DIVS_BEAT "q/divs"
#define STR_MODE_N    "dir"
#define STR_MODE_TYPE "direction"
#define STR_TRIG_N    "trig"
#define STR_TRIG_TYPE "trigger"

// signed byte (not used)
//typedef signed char sbyte;

// this is integer likely 512U
#define __M_PAGE_SIZE__ 512

// class MyGraphics : public IGraphics
// {
// public:
//   MyGraphics(IPlugBase *pPlug, int w, int h, int refreshFPS) : IGraphics(pPlug, w, h, refreshFPS){}
//   ~MyGraphics(){}
// };

#include "midiarpeggiator.h"
#include "IPlugInfo.h"

class Arpeggio
  : public iplug
  //, public Agriggiator
{
public:
  Arpeggio(IPlugInstanceInfo instanceInfo);
  ~Arpeggio();

  void InitializeControls();
  void InitializeParams();
  void Reset() override;
  void OnParamChange(int paramIdx) override;
  void ProcessDoubleReplacing(double** inputs, double** outputs, int nFrames) override;
  void ProcessMidiMsg(IMidiMsg *pMsg) override;
  int GetNumKeys();
  bool GetKeyStatus(int note);
  bool RemoveKey(int note);
  ///void SamIsNotMe() override {}

  int32 someCounter = 0; // this is just a number of frames elapsed
  using cstr = std::string;
  
  inline cstr tostr(int input) { return std::to_string(input); }
  inline cstr tostr(double input) { return std::to_string(input); }
  inline cstr tostr(float input) { return std::to_string(input); }

  inline double SecondsElapsed(double frameCt, double fs=44100.) { return frameCt / fs; }
  inline double SecondsElapsedM(double frameCt, double fs = 44100.) { return fmod(SecondsElapsed(frameCt, fs), 60); }
  inline double SecondsElapsedMF(double frameCt, double fs = 44100.) { return floor(SecondsElapsedM(frameCt, fs)); }

  void OnTimer() override
  {
    // (*) we could use this timer to check samples elapsed against a CPU time-check in this timer-frame.
    if (!mIsLoaded) return;

    IPlugBase::OnTimer();
    someCounter++; // we should check if its >= 44100 and possible increment another counter.
    someCounter = someCounter % 44100;

    GetMidiTime();
    //arp.setRate(mFs);
    if (!mTimeInfo.mTransportIsRunning) {
      cstr str = "rpos: " + tostr(int(SecondsElapsedMF(mSamplesElapsed))) + "\ninc: " + tostr(someCounter);
      WDL_String text = WDL_String(str.c_str());
      textBpm->SetTextFromPlug(text.Get());
    }
    else if (mTimeInfo.mTransportLoopEnabled) { // transport is running and looping
      cstr str = "spos: " + tostr(int(mTimeInfo.mSamplePos)) + "\ninc: " + tostr(someCounter);
      WDL_String text = WDL_String(str.c_str());
      textBpm->SetTextFromPlug(text.Get());
    }
    else/* if (time.mTransportIsRunning)*/ { // transport is running not looped
      cstr str = "spos: " + tostr(int(mTimeInfo.mSamplePos)) + "\ninc: " + tostr(someCounter);
      WDL_String text = WDL_String(str.c_str());
      textBpm->SetTextFromPlug(text.Get());
    }
  }

  // provide public access
  bool SendMidi(IMidiMsg *pMsg);

private:
  /**
  mBeatLength is set to arp.getNoteDuration() after arp.setMulti(...) */
  inline void GetMidiTime();

protected:

  int mIncr = 0;
  inline void Arpeggio::SetMe(int nn, int dn);
  inline void Arpeggio::SetMe(int am);

  void click_0_1(), click_0_2(), click_0_3(), click_0_4()
    , click_1_16(), click_1_8(), click_1_6(), click_1_4(), click_1_3(), click_1_2()
    , click_3_8(), click_3_6(), click_3_4(), click_3_3(), click_3_2(), click_3_1()
    , click_4_8(), click_4_6(), click_4_4(), click_4_3(), click_4_2(), click_4_1()
    , click_reset_notes();

private:

  double     mSamplesElapsed;

  //AudioParameterFloat *beat, *divs, *mode, *rest;
  MidiArpeggiator      arp;

  // there are no notes triggered; hence, we are waiting.
  // we want to set this to false once we actually `SeneMidiMessage(my_note_on)`.
  bool
      mIsLoaded       // ?? when it this set?
    , mAwaitFirstNote // 
    , mRun;           // transport is running
  ITimeInfo mTimeInfo;
  int32
      mLastBlockStart
    , mNoteLength;

  // IPLUG/IControl(s)
  IText textSm_c, textSm_l, textSm_r, textNums;
  ITextControl *textNoteCt, *textSmp;
  ITextTimeControl *textBpm;
  IControl* mKeyboard;
  IBitmapButton *btn_0, *btn_1, *btn_2, *btn_3;
  IKnobText *knobMode, *knobBeat, *knobDivs, *knobPitchMod, *knobGate;
  ISwitchControl *switchType, *switchKeyTrack;
  IContactControl *contactResetNotes;
};

#endif
