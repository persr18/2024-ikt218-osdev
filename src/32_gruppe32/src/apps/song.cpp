// The music player provided in the course has been used and slightly modified

extern "C"
{

#include <pit.h>
#include <io.h>
#include <libc/stdint.h>
#include <terminal.h>
}
#include <song.h>

void enable_speaker()
{
    uint8_t speaker_state = inb(PC_SPEAKER_PORT);

    /*
    Bit 0: Speaker gate
            0: Speaker disabled
            1: Speaker enabled
    Bit 1: Speaker data
            0: Data is not passed to the speaker
            1: Data is passed to the speaker
    */
    // Check if bits 0 and 1 are not set (0 means that the speaker is disabled)
    if (speaker_state != (speaker_state | 3))
    {
        // If bits 0 and 1 are not set, enable the speaker by setting bits 0 and 1 to 1
        outb(PC_SPEAKER_PORT, speaker_state | 3);
    }
}

void disable_speaker()
{
    // Turn off the PC speaker
    uint8_t speaker_state = inb(PC_SPEAKER_PORT);
    outb(PC_SPEAKER_PORT, speaker_state & 0xFC);
}

void play_sound(uint32_t frequency)
{
    if (frequency == 0)
        return;

    uint16_t divisor = PIT_BASE_FREQUENCY / frequency;

    outb(PIT_CMD_PORT, 0xB6);
    outb(PIT_CHANNEL2_PORT, divisor & 0xFF);
    outb(PIT_CHANNEL2_PORT, (divisor >> 8) & 0xFF);

    enable_speaker();
}

void play_song_impl(Song *song)
{
    int progress = 0;
    for (uint32_t i = 0; i < song->length; i++)
    {
        progress = (100 * (i + 1)) / song->length;
        Note *note = &song->notes[i];
        printf("\rNote: %d,   Freq=%d,   Sleep=%d,   Progress=(%d/100)       ", i, note->frequency, note->duration, progress);
        play_sound(note->frequency);
        sleep_busy(note->duration);
        disable_speaker();
    }
}


SongPlayer *create_song_player()
{
    auto *player = new SongPlayer();
    player->play_song = play_song_impl;
    return player;
}