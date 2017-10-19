#include "g1.h"

g1::g1()
{
	CtrlLayout(*this, "Window title");
	Sizeable(true).Zoomable(true);
}

GUI_APP_MAIN
{
	g1().Run();
}
