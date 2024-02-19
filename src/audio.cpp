#include "audio.h"

extern "C" {
#include "soundbank_bin.h"
#include "soundbank.h"
#include <maxmod.h>
}

namespace audio {

void initialise() { mmInitDefault((mm_addr)soundbank_bin, 8); }

void play_song() {
	mmStart(MOD_BAD_APPLE, MM_PLAY_LOOP);
}

} // namespace audio
