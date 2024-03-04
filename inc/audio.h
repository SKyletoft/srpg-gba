#pragma once

extern "C" {
#include <tonc_types.h>
}

namespace audio {

void initialise();
void play_song(u32);
void play_sfx(u32);

} // namespace audio
