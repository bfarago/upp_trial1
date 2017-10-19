#include "g1.h"

g1::g1()
{
	CtrlLayout(*this, "Window title");
	Sizeable(true).Zoomable(true);
	edFilter.WhenAction= THISBACK(OnActionEdFilter);
}

void g1::OnActionEdFilter(){
	String s= ~edFilter;
	graf.SetFilter(s);
}

GUI_APP_MAIN
{
	g1().Run();
}
