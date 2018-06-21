#ifndef _g1_g1_h
#define _g1_g1_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;
#include "graf.h"
#define LAYOUTFILE <g1/g1.lay>
#include <CtrlCore/lay.h>


class g1 : public Withg1Layout<TopWindow> {
public:
	typedef g1 CLASSNAME;
	g1();
	void OnActionEdFilter();
	void OnNode(GrafNode* node);
};

#endif
