main-loop:
	update inputs
	logic:
		switch mode
		logic-per-mode
	draw:
		wait for vblank
		actually draw
