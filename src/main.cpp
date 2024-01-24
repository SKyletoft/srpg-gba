#include <stdint.h>
#include <tonc.h>

int main() {
	*(uint32_t *)0x04000000 = 0x0403;

	((uint16_t *)0x06000000)[120 + 80 * 240] = 0x001F;
	((uint16_t *)0x06000000)[136 + 80 * 240] = 0x03E0;
	((uint16_t *)0x06000000)[120 + 96 * 240] = 0x7C00;

	REG_DISPCNT = DCNT_MODE0 | DCNT_BG0;

	// Init BG 0 for text on screen entries.
	tte_init_se_default(0, BG_CBB(0) | BG_SBB(31));

	tte_write("#{P:72,64}");         // Goto (72, 64).
	tte_write("Hello World again!"); // Print "Hello world!"

	while (1)
		;

	return 0;
}
