#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include <Def/Def.hpp>
#include <Settings/SettingsWrapper.hpp>

#include "MIDI_MessageTypes.hpp"

BEGIN_CS_NAMESPACE

enum class MIDIReadEvent : uint8_t {
    NO_MESSAGE = 0,
    CHANNEL_MESSAGE = 1,
    SYSEX_MESSAGE = 2,
    REALTIME_MESSAGE = 3,
};

class MIDI_Parser {
  public:
    /** Get the latest MIDI channel message. */
    ChannelMessage getChannelMessage() { return midimsg; }
    /** Get the latest MIDI real-time message. */
    RealTimeMessage getRealTimeMessage() { return rtmsg; }
#if !IGNORE_SYSEX
    /** Get the latest SysEx message. */
    virtual SysExMessage getSysExMessage() const = 0;
#else
    SysExMessage getSysExMessage() const { return {nullptr, 0, 0}; }
#endif
    /** Get the pointer to the SysEx data. */
    const uint8_t *getSysExBuffer() const { return getSysExMessage().data; }
    /** Get the length of the SysEx message. */
    size_t getSysExLength() const { return getSysExMessage().length; }

  protected:
    ChannelMessage midimsg = {0xFF, 0x00, 0x00, 0x0};
    RealTimeMessage rtmsg = {0xFF, 0x0};

  public:
    /** Check if the given byte is a MIDI header byte. */
    static bool isStatus(uint8_t data) { return data & (1 << 7); }
    /** Check if the given byte is a MIDI data byte. */
    static bool isData(uint8_t data) { return (data & (1 << 7)) == 0; }
};

END_CS_NAMESPACE