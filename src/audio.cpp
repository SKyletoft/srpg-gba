#include "audio.h"

extern "C" {
#include "soundbank.h"
#include "soundbank_bin.h"
#include <maxmod.h>
}

namespace audio {

void initialise() {
	mmInitDefault((mm_addr)soundbank_bin, 8);
	// irq_add(II_VBLANK, mmFrame);
	mmSetVBlankHandler((void *)mmFrame);
}

void play_song(u32 song) { mmStart(song, MM_PLAY_LOOP); }

} // namespace audio
