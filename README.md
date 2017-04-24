<!-- Author:tfw
Author-meta:tfw
Title: Arpeggio
Subtitle: 
Date:20170417
Encoding:utf8
version:tfwio.wordpress.com
mainfont:Roboto Slab
monofont:FreeMono
monoscale:0.8
dh:8in
dw:5in
top:0.75in
bottom:0.75in
lr:0.35in -->

> [WIP] a simple arpeggiator vst instrument/effect using iplug/wdl-ol;

![of 20170421_2251cst](resources/img/20170421_2251cst.png)

its a fairly simple arpeggiator called 'arpeggio' for the moment.  
guess we'll see where this goes.

it may very well turn out to be my swiss-army knife for midi generation/transformation.

(pre-release)  binaries for Windows 7 and later [VC2013rt]: [64 bit](https://github.com/tfwio/arp/releases/download/20170422_1623_CST/tfwio_arpeggio_vst-x64-20170422.zip "tfwio_arpeggio_vst-x64-20170422.zip") and [32 bit](https://github.com/tfwio/arp/releases/download/20170422_1623_CST/tfwio_arpeggio_vst-x86-20170422.zip "tfwio_arpeggio_vst-x86-20170422.zip") DAW.

See the [releases](https://github.com/tfwio/arp/releases) page.

----

**PITCH CONTROL** knob (-128 to 127) pitch-key up or down the scale; Notes fall out if not within [0-127] range.  
Like the GATE knob/feature, this is one of the last things added could use some work on predicting/preventing note-on/off-overlap quirks.

**UI KEYBOARD** &mdash; a little buggy when it comes to turning off the last note depressed since I had spent the least amount of time working a few changes into Tale's MIDI-Keyboard `IControl`.  (mouse-wheel) middle-click to stick a key down, right-click-drag to wipe clean specific depressed notes.  
The bug is where/in when you right-click, the last key stuck down gets un-stuck&mdash;will get to it.  

- left mouse-down/drag/up sends midi note-on/off messages to quantized, arpeggiated and triggered.
- right mouse-down/drag to clear depressed notes
- middle-click on a keys to make it stick down

"**K-AMP**" on/off switch/button toggles enabling a amplitude knob which forces midi messages to the particular amp or otherwise relies on what you press in your midi keyboard (or in the tiny UI keys---the lower the louder)

**GATE** &mdash; knob percentage from 1 to 200, generally for shrinking down the length of a generated note.  
CAUTION: Generated NOTE_ON/NOTE_OFF combonation will interfere when the same key gets repeated.  This is an acceptable quirk as long as you're aware.
Like the KEY_PITCH knob/feature, this is one of the last things added could use some work on predicting note-on/off-overlaps.

**TIMING**: BEATS (1-16) VS DIVS (1-64) knobs for controlling as well as...  
 FIRST top row of fraction looking buttons are [1/n] divisions; Second row: [3/n] divisions and [4/n] divisions in the last.  *will probably be re-written/designed*

**SYNC** button [on/off] should sync the timing to the DAW transport/clock if the transport is running.  
  It can generate MIDI with the DAW in idle mode so play/stop DAW-transport to potentially re-sync if sync is on.

**PANIC** button (triangle near midi-kb) releases all depressed keys/notes

**RAMPING** 4 ramp modes at the moment: up, down, up-down and down-up.  more to come of this area.

----

If you're not sure how to use a MIDI effect in live, it has to be VSTi.  (See [this img](manual/live.gif))  
Most DAW explain how they support MIDI effects if they do.  
Ardour, Cubase, and REAPER among a few other DAW I'm aware of properly support MIDI VST effects (not compiled as instrument) chains properly.



# WHAT IS AN ARPEGGIATOR?

[Secrets of the Arpeggiator](http://archive.oreilly.com/pub/a/oreilly/digitalmedia/2006/06/29/secrets-of-the-arpeggiator.html) — by Jim Atkin of O'Reilly Digital-Media (20060629).  
Wikipedia — [Arpeggio](https://en.wikipedia.org/wiki/Arpeggio)

Firstly, most DAW include some form of an arpeggiator.

The following table is what has it my radar.  There are more.

| name         | type       | vendor uri |
|--------------|------------|------------|
| Blue Arp     | free       | [graywolf2004.net](http://graywolf2004.net/wp/)
| Catanya      | commercial | [see me](http://www.synthtopia.com/content/2009/08/07/catanya-vst-midi-arpeggiator-plugin/), search youtube.
| Kirnu        | commercial\* | [kirnuarp.com](http://www.kirnuarp.com/)
| Nora, NoraCM | commercial\* | [squaredheads.com](https://squaredheads.com/)
| MIDI Strum   | Lv2       | [x42-plugins.com](http://x42-plugins.com/x42/x42-midifilter)

- \**Kirnu demo doesn't allow you to store presets but works fine otherwise (I'm told)*
- \**NoraCM is distributed with Computer Music magazine (including digital editions).*



## CLOCK/TIMING

```cpp
// timing calculation
// - Bb (class member) is our calculated beat
// - Lb (class member) is the length of a beat division.
inline int32 calculate()
{
  Bb = ((60.0 * mSampleRate / mBpm));
  Lb = double(mBeat) * (1. / double(mDiv)); 
  if (Lb <= 0.) Lb = 0.00001; // prevent zero division

  double actualLength = Bb * Lb;
  mNoteDurationOn = int32(floor(actualLength));
  mNoteDurationOff = int32(floor((actualLength * mGateDiv))); // *bad

  return mNoteDurationOn;
}
```

## WINDOWS DEV

- wdl-ol/iplug
- see github.com/tfwio/twix
    - common includes and few cpp dependencies are in there.
    - wihtout the notes on the readme there this would be fairly difficult to compile.
- using VisualStudio 2013 Express load the SLN (**Arpeggio.sln**)
    - **arpeggio_i-vst2.vcproj** some DAW that can't handle VST-midi-effect specs so there's an instrument version.
    - **arpeggio-vst2.vcproj** is the effect version that works in DAWs properly supporting VST midi effects.

## NON-WINDOWS (noop)

It would be nice to see this working on a mac but I'm probably not going to be getting to that, though I'd like to write up some makefiles for mingw/msys2.
If you would like to get it working in something other than windows, you're going to have to reference the vcproj's until that imaginary day makefiles or some other reference comes into existance.
Also (again), we're going to need to parallel and clean up a couple of things like a few mouse-related API (that aren't implemented yet here in a particular number-control), the timer mechanism noted in the 'twix' readme and perhaps some other things that are hard-wired to windows if I missed something I didn't document —there really shouldn't be any real difficulty I'm thinking, once all the right files, paths and preprocessor definitions are in place.


# TODO

- few more ramp modes
- MIDI channel specific notes
- preset, bank?
- sub-intervals, div level 2 (another layer of divisions with pitch or other controller ramping methods applied)
- MODULATION MATRIX editor making for more PARAMs
    - mod   wheel
    - pitch wheel
    - LFO/CC-trigger
- UI
    - param triggers for key-offset (pitch knob)
        - maybe note 'collision prevention' if it works out
    - skinning possibilities / *.ini er whatever
- presets and/or midi import/export
- *There may or may not be prevention/detection for this in the future.*
- what else?

# license

MIT

<!--https://vignette4.wikia.nocookie.net/looneytunes/images/b/b1/Daffypearl.gif/revision/latest?cb=20060212075938-->

![](manual/license.jpg)