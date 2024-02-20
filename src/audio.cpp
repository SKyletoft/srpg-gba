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

void play_song() { mmStart(MOD_BAD_APPLE, MM_PLAY_LOOP); }

} // namespace audio
