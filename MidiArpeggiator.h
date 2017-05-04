#ifndef __MIDI_ARP_H__
#define __MIDI_ARP_H__

#if _WIN32
#pragma once
#endif

#include "SortedInt.h"
#include "TimingInterval.h"
#include "IMidiQueue.h"
#include "MidiNode.h"

//class PLUG_CLASS_NAME;

enum ARPMODE {
  PARAM_ARP_MODE_UP = 0, // arp mode up
  PARAM_ARP_MODE_DN,   // arp mode down
  PARAM_ARP_MODE_UD,   // arp mode up-down
  PARAM_ARP_MODE_DU,   // arp mode down-up
  pArpModeCount,       // arp mode down-up
};


class MidiArpeggiator {
//#ifdef VST_API
//  friend class IPlugVST;
//#endif
//  friend class IPlugBase;
public:

  MidiArpeggiator();
  ~MidiArpeggiator(){}

  bool IsEnabled() { return this->mArpIsOn; }

  bool HasMsg()       { return !mMidiQueue.Empty(); }
  IMidiMsg *MsgPeek() { return mMidiQueue.Peek(); }
  void MsgRemove()    { mMidiQueue.Remove(); }
  void MsgClean()     { mMidiQueue.Clear(); }

  inline void GetMidiTime(ITimeInfo *pTimeInfo, int pSamplePos)
  {
    setMulti(pTimeInfo->mTempo, mMode, mBeat, mDiv);
    mBeatLength = getNoteDurationOn();
    mRealTime = pSamplePos;

    // what is this doing here?
    // its looking like a forced sync
    if (mSync && pTimeInfo->mTransportIsRunning)
    {
      // preventing a divide by zero using the mod function?
      if (mRealTime == 0 && mBeatLength == 0) mModTime = 0;
      else mModTime = mRealTime % mBeatLength;
    }
    // time->mLastBar; // in ppq // time->mCycleStart // in ppq // time->mCycleEnd // in ppq
  }

  IMidiQueue *GetMidiQueue() { return &mMidiQueue; }

  void ProcessMidi(IMidiMsg *pMsg, ITimeInfo *pTimeInfo, int pSamplePos);

  int GetNewNoteMod()
  {
    if (mMode < PARAM_ARP_MODE_UD) return mNoteCount;
    if (mNoteCount < 3) return mNoteCount;
    return mNoteCount + mNoteCount - 2;
  }
  
  // will always increment mNoteIndex+1
  int GetNewNoteIndex();

  // --------------------------------
  // This guy's job is to append new notes to IMidiQueue so that after its finished
  // the plugin's AudioProcess can send the generated notes.
  // --------------------------------
  // (*) in the main-audio-thread, we want to track when mod changes the... (mModTime)
  // (*) we want to add a poly mode eventually.
  // --------------------------------
  void ProcessBuffer(int pSamplePos, int nFrames, ITimeInfo *pTimeInfo);

  int32 getNoteDurationOn()  const { return mNoteDurationOn; }
  int32 getNoteDurationOff() const { return mNoteDurationOff; }

  inline void setRate(double fs) { mSampleRate = int(fs); }

  // indirectly calls `calculate(double fs)`
  void setBpm(double value, double fs){ mBpm = value; calculate(); }
  double getBpm() const { return mBpm; }
  const char* getBpmStr() const { std::string bpmStr = "bpm: "; bpmStr.append(std::to_string(mBpm)); return bpmStr.c_str(); }

  void setBeat(int32 value){ mBeat = value; mBeatLength = calculate(); }

  void setDivs(int32 value){ mDiv = value; mBeatLength = calculate(); }

  void setMode(int32 value){ mMode = (ARPMODE)value; }
  int32 getMode() const { return int32(mMode); }

  void setSync(int nSync) { mSync = nSync; }

  void setKeyTrack(int kTrack) { mKeyTrack = kTrack; }
  void setKeyAmp(int kAmp) { mKeyAmp = kAmp; }

  void setKeyGate(int gate) { mGate = gate; mGateDiv = double(mGate) / 100.; calculate(); }
  int32 getKeyGate() { return mGate; }

  void SetKeyOffset(int offset) { mKeyOffset = offset; }

  /*inline*/ void setMulti(double bpm, int mode, int32 beat, int32 div)
  {
    mMode = (ARPMODE)mode;
    mBeat = beat;
    mDiv = div;
    setBpm(bpm, mSampleRate);
  }
  /*inline*/ int32 calculate()
  {
    Bb = ((60.0 * mSampleRate / mBpm));
    Lb = double(mBeat) * (1. / double(mDiv)); 
    if (Lb <= 0.) Lb = 0.00001; // prevent zero division

    double actualLength = Bb * Lb;
    mNoteDurationOn = int32(floor(actualLength));
    mNoteDurationOff = int32(floor((actualLength * mGateDiv))); // *bad

    return mNoteDurationOn;
  }

  inline int32 getNoteCountAlt(int32 count) { return count < 3 ? count : count + count - 2; }

  inline int  KeyCount     ()        { return mMidiNotes.size(); }
  inline bool KeyStatus    (int note){ return mMidiNotes.has_value(int(note)); }
  inline bool KeyRemove    (int note){ if (KeyStatus(note)) { mMidiNotes.remove(note);  return true; } return false; }
  inline void KeyClearAll  ()        { mMidiNotes.clear(); }

protected:

  IMidiMsg MakeNote(MidiNode &node, int offset, bool isOn = true);

private:
  bool mArpIsOn, mSync, mKeyTrack;
  ARPMODE mMode;
  IMidiQueue  mMidiQueue;
  SortedNotes mMidiNotes;
  double Lb, Bb, mBpm, mGateDiv;
    int32
      mNotePrior
    , mModTime, mRealTime
    , mNoteCount
    , mNoteIndex
    , mKeyOffset
    , mBeat
    , mBeatLength
    , mDiv
    , mFs
    , mKeyAmp
    , mGate
    , mNoteDurationOn  // is actually used as note-position
    , mNoteDurationOff // is used as note-length
    , mSampleRate;
};

MidiArpeggiator::MidiArpeggiator()

  : Bb(0.), Lb(0.), mBpm(120.)

  , mArpIsOn(1)
  , mSync(0)
  , mKeyTrack(0) // bool

  , mBeat(1), mDiv(16) //, mSpeed(0.) // int32
  , mBeatLength(0)
  , mNoteDurationOn(0), mNoteDurationOff(0)
  , mNoteCount(0), mNoteIndex(-1)
  , mKeyAmp(127)
  , mFs(0)
  , mModTime(0), mRealTime(0)

  , mMidiQueue()
  , mMidiNotes(0)
{
}
int MidiArpeggiator::GetNewNoteIndex()
{
  mNoteIndex = (mNoteIndex + 1) % GetNewNoteMod();
  switch (mMode)
  {
  case ARPMODE::PARAM_ARP_MODE_UP: return mNoteIndex; break;
  case ARPMODE::PARAM_ARP_MODE_DN: return mNoteCount - mNoteIndex - 1; break;
  case ARPMODE::PARAM_ARP_MODE_UD:
    if (mNoteIndex < mNoteCount) return mNoteIndex;
    else return (mNoteCount - ((mNoteIndex + 1) % mNoteCount) - 1);
    break;
  case ARPMODE::PARAM_ARP_MODE_DU:
    if (mNoteIndex < mNoteCount) return (mNoteCount - (mNoteIndex % mNoteCount) - 1);
    else return (mNoteIndex + 1) % mNoteCount;
  }
  return -1;
}
void MidiArpeggiator::ProcessMidi(IMidiMsg *pMsg, ITimeInfo *pTimeInfo, int pSamplePos)
{
  using mmt = IMidiMsg::EStatusMsg;

  mmt stat = pMsg->StatusMsg();
  if (!(stat == mmt::kNoteOn || stat == mmt::kNoteOff)) return;

  GetMidiTime(pTimeInfo, pSamplePos);

  int mNNL = int(calculate()); // new note length

  // if (!is_loaded) return; // for another plug --- if no preset then skip processing...
  int vel = pMsg->Velocity(), nn = pMsg->NoteNumber(), nOff = pMsg->mOffset;

  if (((stat == mmt::kNoteOn) && vel == 0) || stat == mmt::kNoteOff)
    mMidiNotes.remove(nn);
  else
    mMidiNotes.add(MidiNode(0, nn, vel, 0x0B, mKeyOffset));

  mNoteCount = mMidiNotes.size();
}
void MidiArpeggiator::ProcessBuffer(int pSamplePos, int nFrames, ITimeInfo *pTimeInfo)
{
  mNoteCount = mMidiNotes.size(); // just checking (for re-use)
  GetMidiTime(pTimeInfo, pSamplePos);

  if (nFrames <= 0) return;

  bool has_notes = mNoteCount > 0;
  bool has_note_on = (mModTime + nFrames) >= mBeatLength;

  if (has_note_on) // WTF is has_rest_on have to do with anything at this point? 
  {
    const int32 n_offset = IPMAX(0, IPMIN(mBeatLength - mModTime, nFrames - 1));
    const int32 n_offset_off = n_offset + mNoteDurationOff;

    if (has_notes)
    {
      int newNoteIndex = GetNewNoteIndex();
      MidiNode node = mMidiNotes.at(newNoteIndex);
      if (node.CanDo(mKeyOffset))
      {
        IMidiMsg nMsg = MakeNote(node, n_offset);
        mMidiQueue.Add(&nMsg);

        IMidiMsg nMsgOff = MakeNote(node, n_offset_off, false);
        mMidiQueue.Add(&nMsgOff);
      }
    }
  }

  mModTime = (mModTime + nFrames) % mBeatLength;
}
IMidiMsg MidiArpeggiator::MakeNote(MidiNode &node, int offset, bool isOn)
{
  IMidiMsg nMsg = IMidiMsg(offset);
  if (isOn) nMsg.MakeNoteOnMsg(
    node.GetNote() + mKeyOffset,
    (!mKeyTrack && isOn) ? mKeyAmp : node.GetAmp(),
    offset,
    0
    );
  else nMsg.MakeNoteOffMsg(node.GetNote() + mKeyOffset, offset, 0);
  return nMsg;
}
#endif