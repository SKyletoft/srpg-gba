#include "interrupts.h"

extern "C" {
#include <maxmod.h>
#include <tonc_irq.h>
}

namespace interrupts {

void initialise() {
	irq_init(nullptr);
	irq_add(II_VBLANK, mmVBlank);
	irq_enable(II_VBLANK);
}

} // namespace interrupts