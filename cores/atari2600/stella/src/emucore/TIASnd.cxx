//============================================================================
//
//   SSSS    tt          lll  lll       
//  SS  SS   tt           ll   ll        
//  SS     tttttt  eeee   ll   ll   aaaa 
//   SSSS    tt   ee  ee  ll   ll      aa
//      SS   tt   eeeeee  ll   ll   aaaaa  --  "An Atari 2600 VCS Emulator"
//  SS  SS   tt   ee      ll   ll  aa  aa
//   SSSS     ttt  eeeee llll llll  aaaaa
//
// Copyright (c) 1995-2014 by Bradford W. Mott, Stephen Anthony
// and the Stella Team
//
// See the file "License.txt" for information on usage and redistribution of
// this file, and for a DISCLAIMER OF ALL WARRANTIES.
//
// $Id: TIASnd.cxx 2838 2014-01-17 23:34:03Z stephena $
//============================================================================

#include "System.hxx"
#include "TIASnd.hxx"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
TIASound::TIASound(Int32 outputFrequency)
  : myChannelMode(Hardware2Stereo),
    myOutputFrequency(outputFrequency),
    myOutputCounter(0),
    myVolumePercentage(100)
{
  reset();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
TIASound::~TIASound()
{
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void TIASound::reset()
{
  // Fill the polynomials
  polyInit(Bit4, 4, 4, 3);
  polyInit(Bit5, 5, 5, 3);
  polyInit(Bit9, 9, 9, 5);

  // Initialize instance variables
  for(int chan = 0; chan <= 1; ++chan)
  {
    myVolume[chan] = 0;
    myDivNCnt[chan] = 0;
    myDivNMax[chan] = 0;
    myDiv3Cnt[chan] = 3;
    myAUDC[chan] = 0;
    myAUDF[chan] = 0;
    myAUDV[chan] = 0;
    myP4[chan] = 0;
    myP5[chan] = 0;
    myP9[chan] = 0;
  }

  myOutputCounter = 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void TIASound::outputFrequency(Int32 freq)
{
  myOutputFrequency = freq;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
string TIASound::channels(uInt32 hardware, bool stereo)
{
  if(hardware == 1)
    myChannelMode = Hardware1;
  else
    myChannelMode = stereo ? Hardware2Stereo : Hardware2Mono;

  switch(myChannelMode)
  {
    case Hardware1:       return "Hardware1";
    case Hardware2Mono:   return "Hardware2Mono";
    case Hardware2Stereo: return "Hardware2Stereo";
    default:              return EmptyString;
  }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void TIASound::set(uInt16 address, uInt8 value)
{
  int chan = ~address & 0x1;
  switch(address)
  {
    case 0x15:    // AUDC0
    case 0x16:    // AUDC1
      myAUDC[chan] = value & 0x0f;
      break;

    case 0x17:    // AUDF0
    case 0x18:    // AUDF1
      myAUDF[chan] = value & 0x1f;
      break;

    case 0x19:    // AUDV0
    case 0x1a:    // AUDV1
      myAUDV[chan] = (value & 0x0f) << AUDV_SHIFT;
      break;

    default:
      return;
  }

  uInt16 newVal = 0;

  // An AUDC value of 0 is a special case
  if (myAUDC[chan] == SET_TO_1 || myAUDC[chan] == POLY5_POLY5)
  {
    // Indicate the clock is zero so no processing will occur,
    // and set the output to the selected volume
    newVal = 0;
    myVolume[chan] = (myAUDV[chan] * myVolumePercentage) / 100;
  }
  else
  {
    // Otherwise calculate the 'divide by N' value
    newVal = myAUDF[chan] + 1;

    // If bits 2 & 3 are set, then multiply the 'div by n' count by 3
    if((myAUDC[chan] & DIV3_MASK) == DIV3_MASK && myAUDC[chan] != POLY5_DIV3)
      newVal *= 3;
  }

  // Only reset those channels that have changed
  if(newVal != myDivNMax[chan])
  {
    // Reset the divide by n counters
    myDivNMax[chan] = newVal;

    // If the channel is now volume only or was volume only,
    // reset the counter (otherwise let it complete the previous)
    if ((myDivNCnt[chan] == 0) || (newVal == 0))
      myDivNCnt[chan] = newVal;
  }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
uInt8 TIASound::get(uInt16 address) const
{
  switch(address)
  {
    case 0x15:    // AUDC0
      return myAUDC[0];

    case 0x16:    // AUDC1
      return myAUDC[1];

    case 0x17:    // AUDF0
      return myAUDF[0];

    case 0x18:    // AUDF1
      return myAUDF[1];

    case 0x19:    // AUDV0
      return myAUDV[0] >> AUDV_SHIFT;

    case 0x1a:    // AUDV1
      return myAUDV[1] >> AUDV_SHIFT;

    default:
      return 0;
  }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void TIASound::volume(uInt32 percent)
{
  if(percent <= 100)
    myVolumePercentage = percent;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void TIASound::process(Int16* buffer, uInt32 samples)
{
  // Make temporary local copy
  uInt8 audc0 = myAUDC[0], audc1 = myAUDC[1];
  uInt8 p5_0 = myP5[0], p5_1 = myP5[1];
  uInt8 div_n_cnt0 = myDivNCnt[0], div_n_cnt1 = myDivNCnt[1];
  Int16 v0 = myVolume[0], v1 = myVolume[1];

  // Take external volume into account
  Int16 audv0 = (myAUDV[0] * myVolumePercentage) / 100,
        audv1 = (myAUDV[1] * myVolumePercentage) / 100;

  // Loop until the sample buffer is full
  while(samples > 0)
  {
    // Process channel 0
    if (div_n_cnt0 > 1)
    {
      div_n_cnt0--;
    }
    else if (div_n_cnt0 == 1)
    {
      int prev_bit5 = Bit5[p5_0];
      div_n_cnt0 = myDivNMax[0];

      // The P5 counter has multiple uses, so we increment it here
      p5_0++;
      if (p5_0 == POLY5_SIZE)
        p5_0 = 0;

      // Check clock modifier for clock tick
      if ((audc0 & 0x02) == 0 ||
         ((audc0 & 0x01) == 0 && Div31[p5_0]) ||
         ((audc0 & 0x01) == 1 && Bit5[p5_0]) ||
         ((audc0 & 0x0f) == POLY5_DIV3 && Bit5[p5_0] != prev_bit5))
      {
        if (audc0 & 0x04)       // Pure modified clock selected
        {
          if ((audc0 & 0x0f) == POLY5_DIV3) // POLY5 -> DIV3 mode
          {
            if ( Bit5[p5_0] != prev_bit5 )
            {
              myDiv3Cnt[0]--;
              if ( !myDiv3Cnt[0] )
              {
                myDiv3Cnt[0] = 3;
                v0 = v0 ? 0 : audv0;
              }
            }
          }
          else
          {
            // If the output was set turn it off, else turn it on
            v0 = v0 ? 0 : audv0;
          }
        }
        else if (audc0 & 0x08)  // Check for p5/p9
        {
          if (audc0 == POLY9)   // Check for poly9
          {
            // Increase the poly9 counter
            myP9[0]++;
            if (myP9[0] == POLY9_SIZE)
              myP9[0] = 0;

            v0 = Bit9[myP9[0]] ? audv0 : 0;
          }
          else if ( audc0 & 0x02 )
          {
            v0 = (v0 || audc0 & 0x01) ? 0 : audv0;
          }
          else  // Must be poly5
          {
            v0 = Bit5[p5_0] ? audv0 : 0;
          }
        }
        else  // Poly4 is the only remaining option
        {
          // Increase the poly4 counter
          myP4[0]++;
          if (myP4[0] == POLY4_SIZE)
            myP4[0] = 0;

          v0 = Bit4[myP4[0]] ? audv0 : 0;
        }
      }
    }

    // Process channel 1
    if (div_n_cnt1 > 1)
    {
      div_n_cnt1--;
    }
    else if (div_n_cnt1 == 1)
    {
      int prev_bit5 = Bit5[p5_1];

      div_n_cnt1 = myDivNMax[1];

      // The P5 counter has multiple uses, so we increment it here
      p5_1++;
      if (p5_1 == POLY5_SIZE)
        p5_1 = 0;

      // Check clock modifier for clock tick
      if ((audc1 & 0x02) == 0 ||
         ((audc1 & 0x01) == 0 && Div31[p5_1]) ||
         ((audc1 & 0x01) == 1 && Bit5[p5_1]) ||
         ((audc1 & 0x0f) == POLY5_DIV3 && Bit5[p5_1] != prev_bit5))
      {
        if (audc1 & 0x04)       // Pure modified clock selected
        {
          if ((audc1 & 0x0f) == POLY5_DIV3)   // POLY5 -> DIV3 mode
          {
            if ( Bit5[p5_1] != prev_bit5 )
            {
              myDiv3Cnt[1]--;
              if ( ! myDiv3Cnt[1] )
              {
                myDiv3Cnt[1] = 3;
                v1 = v1 ? 0 : audv1;
              }
            }
          }
          else
          {
            // If the output was set turn it off, else turn it on
            v1 = v1 ? 0 : audv1;
          }
        }
        else if (audc1 & 0x08)  // Check for p5/p9
        {
          if (audc1 == POLY9)   // Check for poly9
          {
            // Increase the poly9 counter
            myP9[1]++;
            if (myP9[1] == POLY9_SIZE)
              myP9[1] = 0;

            v1 = Bit9[myP9[1]] ? audv1 : 0;
          }
          else if ( audc1 & 0x02 )
          {
            v1 = (v1 || audc1 & 0x01) ? 0 : audv1;
          }
          else  // Must be poly5
          {
            v1 = Bit5[p5_1] ? audv1 : 0;
          }
        }
        else  // Poly4 is the only remaining option
        {
          // Increase the poly4 counter
          myP4[1]++;
          if (myP4[1] == POLY4_SIZE)
            myP4[1] = 0;

          v1 = Bit4[myP4[1]] ? audv1 : 0;
        }
      }
    }

    myOutputCounter += myOutputFrequency;

    switch(myChannelMode)
    {
      case Hardware2Mono:  // mono sampling with 2 hardware channels
        while((samples > 0) && (myOutputCounter >= 31400))
        {
          Int16 byte = v0 + v1;
          *(buffer++) = byte;
          *(buffer++) = byte;
          myOutputCounter -= 31400;
          samples--;
        }
        break;

      case Hardware2Stereo:  // stereo sampling with 2 hardware channels
        while((samples > 0) && (myOutputCounter >= 31400))
        {
          *(buffer++) = v0;
          *(buffer++) = v1;
          myOutputCounter -= 31400;
          samples--;
        }
        break;

      case Hardware1:  // mono/stereo sampling with only 1 hardware channel
        while((samples > 0) && (myOutputCounter >= 31400))
        {
          *(buffer++) = v0 + v1;
          myOutputCounter -= 31400;
          samples--;
        }
        break;
    }
  }

  // Save for next round
  myP5[0] = p5_0;
  myP5[1] = p5_1;
  myVolume[0] = v0;
  myVolume[1] = v1;
  myDivNCnt[0] = div_n_cnt0;
  myDivNCnt[1] = div_n_cnt1;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void TIASound::polyInit(uInt8* poly, int size, int f0, int f1)
{
  int mask = (1 << size) - 1, x = mask;

  for(int i = 0; i < mask; i++)
  {
    int bit0 = ( ( size - f0 ) ? ( x >> ( size - f0 ) ) : x ) & 0x01;
    int bit1 = ( ( size - f1 ) ? ( x >> ( size - f1 ) ) : x ) & 0x01;
    poly[i] = x & 1;
    // calculate next bit
    x = ( x >> 1 ) | ( ( bit0 ^ bit1 ) << ( size - 1) );
  }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
const uInt8 TIASound::Div31[POLY5_SIZE] = {
  0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};
