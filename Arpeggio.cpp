#ifdef SA_API
// comment me out or don't
//#define NO_MIDI_O
#endif

#include "Arpeggio.h"
#include "IPlug_include_in_plug_src.h"
#ifndef _ICONTROL_
#include "IControl.h"
#endif
#include "resource.h"

#include "IKeyboardControl-mod.h"

const int kNumPrograms = 1;

// these were broken on purpose
#define ttf1 "Montserrat"
#define ttf2 "Liberation Mono"

enum ELayout
{
  kWidth = GUI_WIDTH,
  kHeight = GUI_HEIGHT,

  kGainX = 100,
  kGainY = 100,
  kKnobFrames = 60
};
typedef std::function<bool(IMidiMsg*)> mm_cb;

Arpeggio::Arpeggio(IPlugInstanceInfo instanceInfo)
  : iplug(instanceInfo, kParamCount, kNumPrograms)
  , mSamplesElapsed(0)
  , arp()
  , mAwaitFirstNote(0), mRun(0)
  , mTimeInfo()
  , mIsLoaded(0)
  , mLastBlockStart(0)
  , mNoteLength(0)

  , textSm_l(11, &COLOR_WHITE, ttf1, IText::kStyleBold,   IText::kAlignNear,   0, IText::kQualityClearType)
  , textSm_c(11, &COLOR_WHITE, ttf1, IText::kStyleBold,   IText::kAlignCenter, 0, IText::kQualityClearType)
  , textSm_r(11, &COLOR_WHITE, ttf1, IText::kStyleBold,   IText::kAlignFar,    0, IText::kQualityClearType)
  , textNums(12, &COLOR_WHITE, ttf2, IText::kStyleNormal, IText::kAlignCenter, 0, IText::kQualityClearType)
{
  TRACE;

  InitializeParams();
  InitializeControls();
  //MakePreset("preset 1", ... );
  MakeDefaultPreset((char *) "arpeggio", kNumPrograms);
  //RestorePreset(0);
  SetMe(GetParam(kARP_MODE)->Int());
  pGraphics->SetAllControlsDirty();
  mIsLoaded = true;
  arp.setRate(GetSampleRate());

}

Arpeggio::~Arpeggio() {}


void Arpeggio::InitializeParams()
{
  GetParam(KPARA::kNUM_BEAT)->InitInt("beat", 1, 1, 16);
  GetParam(KPARA::kNUM_DIVS)->InitInt("divs", 1, 1, 64);
  IParam* p = GetParam(KPARA::kARP_MODE);
  p->InitEnum("mode", 4, 4, "mode");
  p->SetDisplayText(0, "up");
  p->SetDisplayText(1, "down");
  p->SetDisplayText(2, "up->down");
  p->SetDisplayText(3, "down->up");
  GetParam(KPARA::kARP_SYNC)->InitBool("rest", true);

  GetParam(KPARA::kKEY_MOD)->InitInt("key-shift", 0, -128, 127); // pitch adjustment
  GetParam(KPARA::kKEY_TRA)->InitBool("key-track", 0);           // fixed amplitude enabled
  GetParam(KPARA::kKEY_AMP)->InitInt("key-amp", 100, 1, 127);    // fixed amplitude value
  GetParam(KPARA::kKEY_GATE)->InitInt("key-gate", 100, 1, 200, "%");   // beats max = 16, divs max = 64

}

void Arpeggio::InitializeControls()
{
  IRECT
    rect_text, rect_beats, rect_divs, rect_mode, rect_sync, rect_smp, rect_bpm, rect_panic
    , rectButton_00, rectButton_01, rectButton_02, rectButton_03, rectButton_04, rectButton_05
    , rectButton_10, rectButton_11, rectButton_12, rectButton_13, rectButton_14, rectButton_15
    , rectButton_20, rectButton_21, rectButton_22, rectButton_23, rectButton_24, rectButton_25
    , rectButton_A0, rectButton_A1, rectButton_A2, rectButton_A3
    , rectPitchButton, rectKeyTrack, rectKeyAmp, rectGate
    , r3 // matrix push-button width, height
    , rectGUI
    ;
  const int
    mc1 = 40 // horizontal spacing from matrix button-x to next
    , rc1 = 28 // vertical spacing from matrix-y to next
    //, x001 = 164, x002 = 204
    , udx0 = 164 // first button-matrix x-location
    , udx1 = 204, m_col3 = 22, udy1 = 49
    , x101 = 355 // second button-matrix x-location // next line is all auto-calculated
    , x102 = x101 + mc1, x103 = x102 + mc1, x104 = x103 + mc1, x105 = x104 + mc1, x106 = x105 + mc1
    , m_col1 = 4, m_col2 = m_col1 + rc1, y103 = m_col2 + rc1 // matrix-1 column y-locations
    , knob_beat_x = 53
    , knob_div_x = 111
    , knob_top = 42, trig_top = 50, panic_top = 107
    , octave_x = 270, octave_y = 105, octave_n=8, octave_k = 12
    ;
  ctl_rect(rectGUI,0, 0, GUI_WIDTH, GUI_HEIGHT);
  ctl_rect(r3, 0, 0, 38, 26);

  ctl_rect(rect_beats, knob_beat_x, knob_top, 36, 36); // knob
  ctl_rect(rect_divs, knob_div_x, knob_top, 36, 36);   // knob
  ctl_rect(rect_mode, 169, 200, 36, 36);               // hidden knob

  ctl_rect(rect_text, 120, 109, 115, 10);      // (not used)
  //ctl_rect(rect_bpm, 120, 107, 115, 15);     // should move this.
  ctl_rect(rect_bpm, 10, 10, 115, 15);        // should move this.
  ctl_rect(rect_smp, 120, 97, 115, 15);       //  (not used)

  ctl_rect(rect_sync, 12, trig_top, 24, 24);  //
  ctl_rect(rect_panic, 214, 107, 16, 18);     // reset keys to zero

  ctl_rect(rectKeyTrack, 5, 74, 38, 26);      // this sucks right now

  ctl_rect(rectPitchButton, 321, 15, 24, 24);
  ctl_rect(rectKeyAmp,      291, 62, 24, 24);
  ctl_rect(rectGate,        321, 62, 24, 24);

  ctl_xy(rectButton_00, x101, m_col1, r3); ctl_xy(rectButton_10, x101, m_col2, r3); ctl_xy(rectButton_20, x101, y103, r3); ctl_xy(rectButton_A0, udx0, m_col3, r3);
  ctl_xy(rectButton_01, x102, m_col1, r3); ctl_xy(rectButton_11, x102, m_col2, r3); ctl_xy(rectButton_21, x102, y103, r3); ctl_xy(rectButton_A1, udx1, m_col3, r3);
  ctl_xy(rectButton_02, x103, m_col1, r3); ctl_xy(rectButton_12, x103, m_col2, r3); ctl_xy(rectButton_22, x103, y103, r3); ctl_xy(rectButton_A2, udx0, udy1, r3);
  ctl_xy(rectButton_03, x104, m_col1, r3); ctl_xy(rectButton_13, x104, m_col2, r3); ctl_xy(rectButton_23, x104, y103, r3); ctl_xy(rectButton_A3, udx1, udy1, r3);
  ctl_xy(rectButton_04, x105, m_col1, r3); ctl_xy(rectButton_14, x105, m_col2, r3); ctl_xy(rectButton_24, x105, y103, r3);
  ctl_xy(rectButton_05, x106, m_col1, r3); ctl_xy(rectButton_15, x106, m_col2, r3); ctl_xy(rectButton_25, x106, y103, r3);

  pGraphics = MakeGraphics(this, kWidth, kHeight, 30); // default fps is 25
  pGraphics->HandleMouseOver(true);
  pGraphics->EnableTooltips(true);

  IBitmap generic_knob, btn_sync, generic_bg, btn_panic
    , push_A0, push_A1, push_A2, push_A3
    , push_00, push_01, push_02, push_03, push_04, push_05
    , push_10, push_11, push_12, push_13, push_14, push_15
    , push_20, push_21, push_22, push_23, push_24, push_25
    , regular = pGraphics->LoadIBitmap(RES_IMG_KEYSI_ID, RES_IMG_KEYSI_FN, 6)
    , sharp = pGraphics->LoadIBitmap(RES_IMG_KEYSE_ID, RES_IMG_KEYSE_FN)
    , littleKnob, btnKeyAmp
    ;

  BMP_FRAMES_V(pGraphics, generic_knob, RES_KNOB_ID, RES_KNOB_FN, 65);
  BMP_FRAMES_V(pGraphics, btn_sync, RES_BUTTON_ID, RES_BUTTON_FN, 2);
  BMP_FRAMES_V(pGraphics, btn_panic, RES_IMG_PANIC_ID, RES_IMG_PANIC_FN, 2);
  BMP_FRAMES_V(pGraphics, littleKnob, RES_IMG_KNOB24_ID, RES_IMG_KNOB24_FN, 65);
  BMP_FRAMES_V(pGraphics, btnKeyAmp, RES_IMG_TAMP_ID, RES_IMG_TAMP_FN, 2);
  // ---> UP DN UD DU
  BMP_FRAMES_V(pGraphics, push_A0, RES_IMG_UP_ID, RES_IMG_UP_FN, 3);
  BMP_FRAMES_V(pGraphics, push_A1, RES_IMG_DN_ID, RES_IMG_DN_FN, 3);
  BMP_FRAMES_V(pGraphics, push_A2, RES_IMG_UD_ID, RES_IMG_UD_FN, 3);
  BMP_FRAMES_V(pGraphics, push_A3, RES_IMG_DU_ID, RES_IMG_DU_FN, 3);
  // ---> SINGLES
  BMP_FRAMES_V(pGraphics, push_00, RES_IMG_1_2_ID, RES_IMG_1_2_FN, 3);
  BMP_FRAMES_V(pGraphics, push_01, RES_IMG_1_3_ID, RES_IMG_1_3_FN, 3);
  BMP_FRAMES_V(pGraphics, push_02, RES_IMG_1_4_ID, RES_IMG_1_4_FN, 3);
  BMP_FRAMES_V(pGraphics, push_03, RES_IMG_1_6_ID, RES_IMG_1_6_FN, 3);
  BMP_FRAMES_V(pGraphics, push_04, RES_IMG_1_8_ID, RES_IMG_1_8_FN, 3);
  BMP_FRAMES_V(pGraphics, push_05, RES_IMG_1_16_ID, RES_IMG_1_16_FN, 3);
  // ---> HALVES
  BMP_FRAMES_V(pGraphics, push_10, RES_IMG_3_8_ID, RES_IMG_3_8_FN, 3);
  BMP_FRAMES_V(pGraphics, push_11, RES_IMG_3_6_ID, RES_IMG_3_6_FN, 3);
  BMP_FRAMES_V(pGraphics, push_12, RES_IMG_3_4_ID, RES_IMG_3_4_FN, 3);
  BMP_FRAMES_V(pGraphics, push_13, RES_IMG_3_3_ID, RES_IMG_3_3_FN, 3);
  BMP_FRAMES_V(pGraphics, push_14, RES_IMG_3_2_ID, RES_IMG_3_2_FN, 3);
  BMP_FRAMES_V(pGraphics, push_15, RES_IMG_3_1_ID, RES_IMG_3_1_FN, 3);
  // ---> THIRDS
  BMP_FRAMES_V(pGraphics, push_20, RES_IMG_4_8_ID, RES_IMG_4_8_FN, 3);
  BMP_FRAMES_V(pGraphics, push_21, RES_IMG_4_6_ID, RES_IMG_4_6_FN, 3);
  BMP_FRAMES_V(pGraphics, push_22, RES_IMG_4_4_ID, RES_IMG_4_4_FN, 3);
  BMP_FRAMES_V(pGraphics, push_23, RES_IMG_4_3_ID, RES_IMG_4_3_FN, 3);
  BMP_FRAMES_V(pGraphics, push_24, RES_IMG_4_2_ID, RES_IMG_4_2_FN, 3);
  BMP_FRAMES_V(pGraphics, push_25, RES_IMG_4_1_ID, RES_IMG_4_1_FN, 3);

  // background

  generic_bg = pGraphics->LoadIBitmap(RES_BG_ID, RES_BG_FN);
  AddBitmap(&generic_bg, rectGUI, 1);

  // three knobs

  knobBeat = AddKnobInt003a(&generic_knob, rect_beats, kNUM_BEAT, &textSm_c, "beats");
  knobDivs = AddKnobInt003a(&generic_knob, rect_divs, kNUM_DIVS, &textSm_c, "divs");
  knobMode = AddKnobInt003a(&generic_knob, rect_mode, kARP_MODE, &textSm_c, "mode");
  //knobMode->SetCharNames(4, (char**)RampTypes);
  knobMode->Hide(true); // we're just using this knob to set param values.

  knobPitchMod = AddKnobInt003a(&littleKnob, rectPitchButton, kKEY_MOD, &textSm_c, "change pitch");
  knobPitchMod->SetGearing(9);
  knobGate     = AddKnobInt003a(&littleKnob, rectGate,       kKEY_GATE, &textSm_c, "MIDI Gate");
  knobGate->SetGearing(9);
  knobAmplitude = AddKnobInt003a(&littleKnob, rectKeyAmp, kKEY_AMP, &textSm_c, "default key amplitude\nwhen key tracking is off.");
  (switchKeyTrack = AddSwitch(&btnKeyAmp, rectKeyTrack, kKEY_TRA))
    ->SetTooltip("Amplutude Key-Tracking");
  knobAmplitude->SetGearing(9);

  int coords[12] = { 0, 4, 6, 10, 12, 18, 22, 24, 28, 30, 34, 36 };
  mKeyboard = new IKeyboardControl(this, octave_x, octave_y, octave_k, octave_n, &regular, &sharp, coords);
  pGraphics->AttachControl(mKeyboard);

  switchType = AddSwitch(&btn_sync, rect_sync, kARP_SYNC);

  // shows active note count and maybe other info as well.
  textNoteCt = AddTextControl(rect_text, &textSm_c); // display notes
  textBpm    = AddTextTimeControl(rect_bpm, &textSm_l); // display nsamps
  textSmp    = AddTextControl(rect_smp, &textSm_l); // display nsamps


  // 'transport'

  btn_0 = AddButton(&push_A0, rectButton_A0)->Handler(std::bind(&Arpeggio::click_0_1, this), IBitmapButton::handle_down)->setParamInfo(kARP_MODE, 0);
  btn_1 = AddButton(&push_A1, rectButton_A1)->Handler(std::bind(&Arpeggio::click_0_2, this), IBitmapButton::handle_down)->setParamInfo(kARP_MODE, 1);
  btn_2 = AddButton(&push_A2, rectButton_A2)->Handler(std::bind(&Arpeggio::click_0_3, this), IBitmapButton::handle_down)->setParamInfo(kARP_MODE, 2);
  btn_3 = AddButton(&push_A3, rectButton_A3)->Handler(std::bind(&Arpeggio::click_0_4, this), IBitmapButton::handle_down)->setParamInfo(kARP_MODE, 3);

  AddButton(&push_00, rectButton_00)->Handler(std::bind(&Arpeggio::click_1_2, this), IBitmapButton::handle_down);
  AddButton(&push_01, rectButton_01)->Handler(std::bind(&Arpeggio::click_1_3, this), IBitmapButton::handle_down);
  AddButton(&push_02, rectButton_02)->Handler(std::bind(&Arpeggio::click_1_4, this), IBitmapButton::handle_down);
  AddButton(&push_03, rectButton_03)->Handler(std::bind(&Arpeggio::click_1_6, this), IBitmapButton::handle_down);
  AddButton(&push_04, rectButton_04)->Handler(std::bind(&Arpeggio::click_1_8, this), IBitmapButton::handle_down);
  AddButton(&push_05, rectButton_05)->Handler(std::bind(&Arpeggio::click_1_16, this), IBitmapButton::handle_down);

  AddButton(&push_10, rectButton_10)->Handler(std::bind(&Arpeggio::click_3_8, this), IBitmapButton::handle_down);
  AddButton(&push_11, rectButton_11)->Handler(std::bind(&Arpeggio::click_3_6, this), IBitmapButton::handle_down);
  AddButton(&push_12, rectButton_12)->Handler(std::bind(&Arpeggio::click_3_4, this), IBitmapButton::handle_down);
  AddButton(&push_13, rectButton_13)->Handler(std::bind(&Arpeggio::click_3_3, this), IBitmapButton::handle_down);
  AddButton(&push_14, rectButton_14)->Handler(std::bind(&Arpeggio::click_3_2, this), IBitmapButton::handle_down);
  AddButton(&push_15, rectButton_15)->Handler(std::bind(&Arpeggio::click_3_1, this), IBitmapButton::handle_down);

  AddButton(&push_20, rectButton_20)->Handler(std::bind(&Arpeggio::click_4_8, this), IBitmapButton::handle_down);
  AddButton(&push_21, rectButton_21)->Handler(std::bind(&Arpeggio::click_4_6, this), IBitmapButton::handle_down);
  AddButton(&push_22, rectButton_22)->Handler(std::bind(&Arpeggio::click_4_4, this), IBitmapButton::handle_down);
  AddButton(&push_23, rectButton_23)->Handler(std::bind(&Arpeggio::click_4_3, this), IBitmapButton::handle_down);
  AddButton(&push_24, rectButton_24)->Handler(std::bind(&Arpeggio::click_4_2, this), IBitmapButton::handle_down);
  AddButton(&push_25, rectButton_25)->Handler(std::bind(&Arpeggio::click_4_1, this), IBitmapButton::handle_down);

  AddButton(&btn_panic, rect_panic)->Handler(std::bind(&Arpeggio::click_reset_notes, this), IBitmapButton::handle_down);

  AttachGraphics(pGraphics);
}

// if (((status == IMidiMsg::EStatusMsg::kNoteOn) && vel == 0) || status == IMidiMsg::EStatusMsg::kNoteOff)
//   tDebugHelper2->SetTextFromPlug("OFF");
// else if ((status == IMidiMsg::EStatusMsg::kNoteOn) && vel)
//   tDebugHelper2->SetTextFromPlug("ON!");

void Arpeggio::ProcessMidiMsg(IMidiMsg *pMsg)
{
  GetTime(&mTimeInfo);
  arp.ProcessMidi(pMsg, &mTimeInfo, GetSamplePos());

  SetControlText(textNoteCt, "#keys: ", arp.KeyCount());
}

bool Arpeggio::SendMidi(IMidiMsg *pMsg) { return SendMidiMsg(pMsg); }

// do not draw in this thread
void Arpeggio::ProcessDoubleReplacing(double** inputs, double** outputs, int nFrames)
{
  int32 cFrame = GetSamplePos(); // current frame position
  int32 cNextFrame = nFrames + cFrame - 1;

  // Mutex is already locked for us.
  if (!mIsLoaded) return;
  if (!arp.IsEnabled()) return; // -------------------------------------------------

  GetTime(&mTimeInfo);
  arp.ProcessBuffer(cFrame, nFrames, &mTimeInfo);

  IMidiQueue *queue = arp.GetMidiQueue();
  while (arp.HasMsg())
  {
    IMidiMsg *nMsg = arp.MsgPeek();
    if (nMsg->mOffset > (nFrames - 1)) break;

    SendMidiMsg(nMsg);
    arp.MsgRemove();
  }

  mSamplesElapsed += nFrames; // used in UI
  queue->Flush(nFrames);
}

void Arpeggio::Reset()
{
  TRACE;
  IMutexLock lock(this);
  //mTime = GetSamplePos() % mBeatLength; // this does nothing
  arp.setRate(GetSampleRate());
  arp.MsgClean();// we would like to do this on MIDI_RESET controller-change message.
}

void Arpeggio::OnParamChange(int paramIdx)
{
  IMutexLock lock(this);

  switch (paramIdx)
  {
  case kNUM_BEAT: arp.setBeat(GetParam(kNUM_BEAT)->Int()); break;
  case kNUM_DIVS: arp.setDivs(GetParam(kNUM_DIVS)->Int()); break;
  case kARP_MODE: arp.setMode(GetParam(kARP_MODE)->Int()); break;
  case kARP_SYNC: arp.setSync(GetParam(kARP_SYNC)->Bool() ? 1 : 0); break; // we should probably force sync now, before the break.
  case kKEY_MOD:  arp.SetKeyOffset(GetParam(kKEY_MOD)->Int()); break; // NOT IMPLEMENTED
  case kKEY_TRA:  arp.setKeyTrack(GetParam(kKEY_TRA)->Bool() ? 1 : 0); break; // we should probably force sync now, before the break.
  case kKEY_AMP:  arp.setKeyAmp(GetParam(kKEY_AMP)->Int()); break; // we should probably force sync now, before the break.
  case kKEY_GATE: arp.setKeyGate(GetParam(kKEY_GATE)->Int()); break; //
  default: break;
  }
}

void Arpeggio::GetMidiTime()
{
  IMutexLock lock(this);
  GetTime(&mTimeInfo);
  arp.GetMidiTime(&mTimeInfo, GetSamplePos());
}

// for IKeyboardControl

inline int  Arpeggio::GetNumKeys() { return arp.KeyCount(); }
inline bool Arpeggio::GetKeyStatus(int note){ return arp.KeyStatus(note); }
inline bool Arpeggio::RemoveKey(int note){ return arp.KeyRemove(note); }

// callback methods

inline void Arpeggio::SetMe(int nn, int dn)
{
  knobBeat->SetValueFromPlug(ToNormalizedParam(nn, 1.0, 16., 1.)); knobBeat->SetDirty(true); knobBeat->Redraw();
  knobDivs->SetValueFromPlug(ToNormalizedParam(dn, 1.0, 64., 1.)); knobDivs->SetDirty(true); knobDivs->Redraw();
}
inline void Arpeggio::SetMe(int am)
{
  // auto para = GetParam(kARP_MODE); // para->SetNormalized(am);
  auto para = knobMode->GetParam();
  knobMode->SetValueFromPlug(ToNormalizedParam(am, para->GetMin(), para->GetMax(), 1.0)); knobMode->SetDirty(true); knobMode->Redraw();
  for (unsigned i = 0; i < 4; i++)
  {
    bool is_current = i == am;
    switch (i)
    {
    case 0: btn_0->SetValueFromPlug(is_current ? 1. : 0.); btn_0->SetDirty(true); btn_0->Redraw(); break;
    case 1: btn_1->SetValueFromPlug(is_current ? 1. : 0.); btn_1->SetDirty(true); btn_1->Redraw(); break;
    case 2: btn_2->SetValueFromPlug(is_current ? 1. : 0.); btn_2->SetDirty(true); btn_2->Redraw(); break;
    case 3: btn_3->SetValueFromPlug(is_current ? 1. : 0.); btn_3->SetDirty(true); btn_3->Redraw(); break;
    }
  }
  //using string = std::string;
  //string pa = std::to_string(para->GetNormalized()) +
  //  ", " +
  //  std::to_string(ToNormalizedParam(am, para->GetMin(), para->GetMax(), 1.0));
  //textBpm->SetTextFromPlug((char*)pa.c_str());
  //string pb = std::to_string(para->GetMin()) +
  //  ", " +
  //  std::to_string(para->GetMax());
  //textSmp->SetTextFromPlug((char*)pb.c_str());
}
void Arpeggio::click_0_1(){ SetMe(0); }
void Arpeggio::click_0_2(){ SetMe(1); }
void Arpeggio::click_0_3(){ SetMe(2); }
void Arpeggio::click_0_4(){ SetMe(3); }
void Arpeggio::click_1_16(){ SetMe(1, 16); }
void Arpeggio::click_1_8 (){ SetMe(1, 8 ); }
void Arpeggio::click_1_6 (){ SetMe(1, 6 ); }
void Arpeggio::click_1_4 (){ SetMe(1, 4 ); }
void Arpeggio::click_1_3 (){ SetMe(1, 3 ); }
void Arpeggio::click_1_2 (){ SetMe(1, 2 ); }
void Arpeggio::click_3_8(){ SetMe(3, 8); }
void Arpeggio::click_3_6(){ SetMe(3, 6); }
void Arpeggio::click_3_4(){ SetMe(3, 4); }
void Arpeggio::click_3_3(){ SetMe(3, 3); }
void Arpeggio::click_3_2(){ SetMe(3, 2); }
void Arpeggio::click_3_1(){ SetMe(3, 1); }
void Arpeggio::click_4_8(){ SetMe(4, 8); }
void Arpeggio::click_4_6(){ SetMe(4, 6); }
void Arpeggio::click_4_4(){ SetMe(4, 4); }
void Arpeggio::click_4_3(){ SetMe(4, 3); }
void Arpeggio::click_4_2(){ SetMe(4, 2); }
void Arpeggio::click_4_1(){ SetMe(4, 1); }
void Arpeggio::click_reset_notes(){ arp.KeyClearAll(); SetControlText(textNoteCt, "#keys: ", arp.KeyCount()); mKeyboard->SetDirty(true); mKeyboard->Redraw(); }
