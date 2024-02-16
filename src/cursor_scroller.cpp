#include "cursor_scroller.h"

namespace cursor_scroller {

void CursorScroller::update() {
	this->update_layer(this->layer0);
	this->update_layer(this->layer1);
}

} // namespace cursor_scroller
