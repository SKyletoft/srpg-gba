main-loop:
	update inputs
	logic:
		switch mode
		logic-per-mode
	draw:
		wait for vblank
		actually draw


notes:
	graphics:
		We can draw 3x4 hex tiles over two layers without duplicates by offsetting the camera on the odd layer by 4 pixels (half a tile)
