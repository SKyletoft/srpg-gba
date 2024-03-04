#include "audio.h"

#include "soundbank_bin.h"
#include <maxmod.h>

extern "C" {
#include <tonc_irq.h>
}

namespace audio {

void initialise() {
	mmInitDefault((mm_addr)soundbank_bin, 8);
	mmSetVBlankHandler((void *)mmFrame);
}

void play_song(u32 song) { mmStart(song, MM_PLAY_LOOP); }

void play_sfx(u32 sfx) {
	mmEffect(sfx);
}
} // namespace audio
