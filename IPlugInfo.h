#ifndef __MY_IPLUG_INFO__
#define __MY_IPLUG_INFO__
#ifdef _WIN32
#pragma once
#endif

// [2] for intelli-sense only
#include "IPlug_include_in_plug_hdr.h"
#include "ICControls.h"

// #ifdef PLUG_DOES_STATE_CHUNKS
// #undef PLUG_DOES_STATE_CHUNKS
// #endif
// #ifndef PLUG_DOES_STATE_CHUNKS
// #define PLUG_DOES_STATE_CHUNKS true
// #endif

class iplug : public IPlug {

protected:
  IGraphics *pGraphics;

public:

  /**
  @param instanceinfo   you know
  @param par            number of parameters
  @param pgm            number of programs
  */
  iplug(IPlugInstanceInfo instanceInfo, int par, int pgm)
    : IPlug(instanceInfo
    , par // number of params
    , PLUG_CHANNEL_IO
    , pgm // number of ? selected program?
    , "arpeggio" // public name
    , "arpeggio" // product name
    , PLUG_MFR
    , PLUG_VER
    , PLUG_UNIQUE_ID // 'ds2i' // PLUG_UNIQUE_ID
    , PLUG_MFR_ID
    , PLUG_LATENCY
    , PLUG_DOES_MIDI
    , PLUG_DOES_STATE_CHUNKS
    , PLUG_IS_INST
    , 0 //PLUG_SC_CHANS
    )
  {
  }

private:
  //friend IPlugBase;
  //virtual bool iplug::LoadProgramFromFXP(WDL_String *fileName) override;
  //virtual bool iplug::SaveProgramAsFXP(WDL_String *fileName) override;

protected:

  inline void SetControlText(ITextControl *actor, const char *caption, double value)
  {
    std::string stro = std::string(caption);
    stro.append(std::to_string(value));
    actor->SetTextFromPlug((char*)stro.c_str());
  }
  inline void SetControlText(ITextControl *actor, const char *caption, int32 value)
  {
    std::string stro = std::string(caption);
    stro.append(std::to_string(value));
    actor->SetTextFromPlug((char*)stro.c_str());
  }
  inline void SetControlText(ITextControl *actor, const char *caption, float value)
  {
    std::string stro = std::string(caption);
    stro.append(std::to_string(value));
    actor->SetTextFromPlug((char*)stro.c_str());
  }

#pragma region    BEGIN AddPanel
  ////////////////////////////////////////////////////////////////////////
  // AddPanel
  ////////////////////////////////////////////////////////////////////////
  
  // Create ICPanelControl
  inline void AddPanel(IRECT &rect, const IColor *colour, const IColor *colour2, IntPoint point)   {
    pGraphics->AttachControl(new ICPanelControl(this, rect, colour, colour2, point));
  }
#pragma endregion        END AddPanel

#pragma region BEGIN AddSwitchFrames
  ////////////////////////////////////////////////////////////////////////
  // AddSwitchFrames
  ////////////////////////////////////////////////////////////////////////
  
  // Create ISwitchFramesControl
  inline void AddSwitchFrames(IBitmap *bmp, IRECT &rect, int param, int sc = 2)   {
    pGraphics->AttachControl(new ISwitchFramesControl(this, rect.L, rect.T, param, bmp));
  }
  // Create ISwitchFramesControl
  inline void AddSwitchFrames(IBitmap *bmp, int r_id, char *r_fn, IRECT &rect, int param, int sc = 2) {
    *bmp = pGraphics->LoadIBitmap(r_id, r_fn, sc);
    pGraphics->AttachControl(new ISwitchFramesControl(this, rect.L, rect.T, param, bmp));
  }
#pragma endregion END AddSwitchFrames

#pragma region BEGIN AddBitmap
  ////////////////////////////////////////////////////////////////////////
  // AddSwitch
  ////////////////////////////////////////////////////////////////////////
  
  // Create ISwitchControl
  inline ISwitchControl *AddSwitch(IBitmap *bmp, IRECT &rect, int param, int sc = 2) {
    ISwitchControl *swit = new ISwitchControl(this, rect.L, rect.T, param, bmp);
    pGraphics->AttachControl(swit);
    return swit;
  }
  // Create ISwitchControl
  inline ISwitchControl *AddSwitch(IBitmap *bmp, int r_id, char *r_fn, IRECT &rect, int param, int sc = 2) {
    *bmp = pGraphics->LoadIBitmap(r_id, r_fn, sc);
    return AddSwitch(bmp, rect, param, sc);
  }
  // Create ISwitchControl
  inline ISwitchControl *AddSwitch(ISwitchControl *swit, IBitmap *bmp, int r_id, char *r_fn, IRECT &rect, int param, int sc = 2) {
    *bmp = pGraphics->LoadIBitmap(r_id, r_fn, sc);
    return AddSwitch(swit, bmp, rect, param, sc);
  }
  // Create ISwitchControl
  inline ISwitchControl *AddSwitch(ISwitchControl *swit, IBitmap *bmp, IRECT &rect, int param, int sc = 2)
  {
    swit = new ISwitchControl(this, rect.L, rect.T, param, bmp);
    pGraphics->AttachControl(swit);
    return swit;
  }
#pragma endregion       END AddBitmap

#pragma region    BEGIN AddKnobSwitch
  ////////////////////////////////////////////////////////////////////////
  // AddKnobSwitch
  ////////////////////////////////////////////////////////////////////////

  // Create IKnobMultiControl
  inline void AddKnobSwitch(IBitmap *bmp, IRECT &rect, int param) {
    pGraphics->AttachControl(new IKnobMultiControl(this, rect.L, rect.T, param, bmp));
  }
  // Create IKnobMultiControl
  inline void AddKnobSwitch(IBitmap *bmp, int r_id, char *r_fn, IRECT &rect, int param, int sc) {
    *bmp = pGraphics->LoadIBitmap(r_id, r_fn, sc);
    pGraphics->AttachControl(new IKnobMultiControl(this, rect.L, rect.T, param, bmp));
  }
#pragma endregion   END AddKnobSwitch

#pragma region    BEGIN AddBitmap
  ////////////////////////////////////////////////////////////////////////
  // AddBitmap
  ////////////////////////////////////////////////////////////////////////

  // Create IBitmapControl
  inline void AddBitmap(IBitmap *bmp, int r_id, char *r_fn, IRECT &rect, int sc = 1) {
    *bmp = pGraphics->LoadIBitmap(r_id, r_fn, sc);
    pGraphics->AttachControl(new IBitmapControl(this, rect.L, rect.T, bmp));
  }
  // Create IBitmapControl
  inline void AddBitmap(IBitmap *bmp, IRECT &rect, int sc = 1) {
    pGraphics->AttachControl(new IBitmapControl(this, rect.L, rect.T, bmp));
  }
#pragma endregion       END AddBitmap

#pragma region BEGIN IBitmapButton AddButton
  ////////////////////////////////////////////////////////////////////////
  // IBitmapButton
  ////////////////////////////////////////////////////////////////////////

  // Create IBitmapButton
  inline IBitmapButton *AddButton(IBitmap *bmp, int r_id, char *r_fn, IRECT &rect, int sc = 1) {
    *bmp = pGraphics->LoadIBitmap(r_id, r_fn, sc);
    IBitmapButton *btn = new IBitmapButton(this, rect.L, rect.T, bmp);
    pGraphics->AttachControl(btn);
    return btn;
  }
  // Create IBitmapButton
  inline IBitmapButton *AddButton(IBitmap *bmp, IRECT &rect) {
    IBitmapButton *btn = new IBitmapButton(this, rect.L, rect.T, bmp);
    pGraphics->AttachControl(btn);
    return btn;
  }

  //// Create IBitmapButton
  //inline IBitmapButtonFoo *AddButton(IBitmap *bmp, IRECT &rect, int ntargets, int*ids, int*val, bool hasControls) {
  //  IBitmapButtonFoo *btn = new IBitmapButtonFoo(this, rect.L, rect.T, bmp, ntargets, ids, val, hasControls);
  //  pGraphics->AttachControl(btn);
  //  return btn;
  //}

  //inline void AddTaggedBitmap(double tag, IBitmap *bmp, int r_id, char *r_fn, IRECT &rect, int sc = 1) {
  //  *bmp = pGraphics->LoadIBitmap(r_id, r_fn, sc);
  //  pGraphics->AttachControl(new ITaggedBitmapControl(this, rect.L, rect.T, bmp, tag));
  //}
#pragma endregion END IBitmapButton AddButton

#pragma region    BEGIN AddKnobText
  ////////////////////////////////////////////////////////////////////////
  // AddKnobText
  ////////////////////////////////////////////////////////////////////////
  
  // Create IKnobText
  inline IKnobText *AddKnobText(IBitmap *bmp, int r_id, char *r_fn, IRECT &rect, int param, IText *text, char *tooltop = 0, int sc = 65) {
    *bmp = pGraphics->LoadIBitmap(r_id, r_fn, sc);
    IKnobText *knob = new IKnobText(this, rect.L, rect.T, param, bmp, text, rect.W(), rect.H());
    if (0 != tooltop) knob->SetTooltip(tooltop);
    pGraphics->AttachControl(knob);
    return knob;
  }
  // Create IKnobText
  inline void AddKnobText(IBitmap *bmp, IRECT &rect, int param, IText *text) {
    //if (0 != tooltop) knob->SetTooltip(tooltop);
    pGraphics->AttachControl(new IKnobText(this, rect.L, rect.T, param, bmp, text, rect.W(), rect.H()));
  }
  // Create IKnobText
  inline IKnobText *AddKnobText(IBitmap *bmp, IRECT &rect, int param, IText *text, char *cToolTip) {
    IKnobText *knob = new IKnobText(this, rect.L, rect.T, param, bmp, text, rect.W(), rect.H());
    knob->SetTooltip(cToolTip);
    pGraphics->AttachControl(knob);
    return knob;
  }
#pragma endregion     END AddKnobText

#pragma region    BEGIN AddKnobInt003 AddKnobInt003a
  ////////////////////////////////////////////////////////////////////////
  // AddKnobInt003 AddKnobInt003a
  ////////////////////////////////////////////////////////////////////////
  
  // Create IKnobText
  inline IKnobText *AddKnobInt003(IBitmap *bmp, IRECT &rect, int param, IText *text, char *cToolTip) {
    IKnobText *knob = new IKnobText(this, rect.L, rect.T, param, bmp, text, rect.W(), rect.H());
    //IKnobText *knob = new IKnobText(this, rect.L, rect.T, param, bmp, text, rect.W(), rect.H(), 12, kVertical, "%0.0f");
    knob->SetTooltip(cToolTip);
    pGraphics->AttachControl(knob);
    return knob;
  }
  // Create IKnobText
  inline IKnobText *AddKnobInt003a(IBitmap *bmp, IRECT &rect, int param, IText *text, char *cToolTip) {
    IKnobText *knob = new IKnobText(this, rect.L, rect.T, param, bmp, text, rect.W(), rect.H());
    //IKnobText *knob = new IKnobText(this, rect.L, rect.T, param, bmp, text, rect.W(), rect.H(), 12, kVertical, "%0.0f");
    knob->SetTooltip(cToolTip);
    knob->showEdgeMarks = false;
    pGraphics->AttachControl(knob);
    return knob;
  }
  ITextTimeControl *AddTextTimeControl(IRECT rect, IText *fontstyle)
  {
    auto *textControl = new ITextTimeControl(this, rect, fontstyle);
    pGraphics->AttachControl(textControl);
    return textControl;
  }
  // Create ITextControl
  ITextControl *AddTextControl(IRECT rect, IText *fontstyle)
  {
    ITextControl *textControl = new ITextControl(this, rect, fontstyle);
    pGraphics->AttachControl(textControl);
    return textControl;
  }
  // Create IContactControl
  IContactControl *AddContactControl(IRECT rect, int param, IBitmap *bmp)
  {
    IContactControl *textControl = new IContactControl(this, rect.L, rect.T, param, bmp);
    pGraphics->AttachControl(textControl);
    return textControl;
  }
#pragma endregion END AddKnobInt003 AddKnobInt003a

#pragma region BEGIN AddTextFloat12
  // Create IFloatNumberPanel
  inline void AddTextFloat12(IFloatNumberPanel *fp, IRECT &rect, int param, IText *text, char *tooltip = nullptr) {
    rect.SetWidthHeight(((7 + 1) * 13) - 1, 12);
    fp = new IFloatNumberPanel(this, 7, rect, (int)param, text, tooltip);
    pGraphics->AttachControl(fp);
  }
#pragma endregion  END AddTextFloat12

};

#endif
