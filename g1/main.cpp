#include "g1.h"

g1::g1()
{
	CtrlLayout(*this, "Window title");
	Sizeable(true).Zoomable(true);
	edFilter.WhenAction= THISBACK(OnActionEdFilter);
	graf.WhenNodeSet = THISBACK(OnNode);
}

void g1::OnActionEdFilter()
{
	String s= ~edFilter;
	graf.SetFilter(s);
}
void g1::OnNode(GrafNode* node)
{
	eTitle.SetData(node->label);
	eRadius.SetData(node->r);
}
GUI_APP_MAIN
{
	g1().Run();
}
