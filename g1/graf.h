#ifndef _g1_graf_h_
#define _g1_graf_h_

#include <math.h>

#define pi M_PI
#define SCREEN_DIVX (50.0)
#define SCREEN_DIVY (50.0)
struct GrafNode{
	float x;
	float y;
	int sx;
	int sy;
	float r;
	String label;
	bool sel;
	bool filter;
	int opid;
};
struct GrafEdge{
	int source;
	int target;
	bool directed;
	float weight;
	String label;
};

struct Graf : public Ctrl {
    Array<GrafNode> nodes;
    Array<GrafEdge> edges;
    int sel;
    int opid;
    Point mOffset;
    Point mTool;
    float mZoomLevel;
    bool filter;
    Event<GrafNode*> WhenNodeSet;
    void SetFilter(const String& f){
        if (f.IsEmpty()){
            filter=false;;
        }else
        for(int i=0; i<nodes.GetCount(); i++){
            GrafNode& n= nodes[i];
            if (n.label.Find(f)>0){
                n.filter=true;
                filter=true;
            }else{
                n.filter=false;
            }
        }
        Refresh();
    }
    void initDb(){
        #define MAXNODE (90)
        for(int i=0; i<MAXNODE; i++){
            GrafNode& n= nodes.Add();
            n.x= sin(i*2*pi/(float)MAXNODE)*100.0+110;
            n.y= cos(i*2*pi/(float)MAXNODE)*100.0+110;
            n.r=5*(float)rand()/RAND_MAX+5;
            n.label="Label"+AsString(i);
            n.sel=false;
            n.opid=0;
        }
        for(int i=0; i<MAXNODE; i++){
            GrafEdge& e= edges.Add();
            e.source=i;
            e.target=MAXNODE-i-1;
            e.label=AsString(e.source)+"-"+AsString(e.target);
            e.weight=(float)rand()/RAND_MAX;
        }
        for(int i=0; i<MAXNODE/2; i++){
            GrafEdge& e= edges.Add();
            e.source=i;
            e.target=i+1;
            e.label=AsString(e.source)+"-"+AsString(e.target);
            e.weight=(float)rand()/RAND_MAX;
        }
        mOffset.x=50;
        mOffset.y=50;
        mZoomLevel=50;
    }
    void PaintNode(Draw& w, GrafNode& n){
        float w1=n.r;
        float w2=n.r*2;
        int pen=1;
        Color color=LtGray();
        Color pencolor=LtGray();
        Color textcolor=LtGray();
        if (sel<0){
            textcolor=Black();
            color=Gray();
        }
        if (filter){
            textcolor=Color(0xe0, 0xe0, 0xe0);
            color=Color(0xf0, 0xf0, 0xf0);
            if (n.filter){
	            textcolor=Black();
	            color=LtGray();
	        }
        }
        
        if (n.sel){
            color=LtRed();
            textcolor=Black();
        }
        w.DrawEllipse( n.sx-w1, n.sy-w1, w2, w2, color, pen, pencolor);
        //w.DrawText(n.sx, n.sy-10, AsString(i), Arial(), textcolor);
        w.DrawText(n.sx, n.sy, n.label, Arial(), textcolor);
    }
    virtual void Paint(Draw& w) {
        w.DrawRect(GetSize(), White());
        Color defaulttextcolor=LtGray();
        Color defaultcolor=LtGray();
        
        if (sel<0){
            defaulttextcolor=Black();
            defaultcolor=Gray();
        }
        if (filter){
            defaulttextcolor=Color(0xe0, 0xe0, 0xe0);
            defaultcolor=Color(0xf0, 0xf0, 0xf0);
        }
        for(int i=0; i<nodes.GetCount(); i++){
            GrafNode& n= nodes[i];
            n.sx=n.x*mZoomLevel/SCREEN_DIVX+mOffset.x;
            n.sy=n.y*mZoomLevel/SCREEN_DIVY+mOffset.y;
        }
        //background
        for(int i=0; i<edges.GetCount(); i++){
            GrafEdge& e=edges[i];
            GrafNode& source= nodes[e.source];
            GrafNode& target= nodes[e.target];
            Color color=defaultcolor;
            Color textcolor=defaulttextcolor;
            if (source.sel || target.sel){
				continue;
            }
            if (source.filter || target.filter){
                textcolor=Black();
                color=LtGray();
            }
            w.DrawLine(source.sx, source.sy, target.sx, target.sy, 15*e.weight, color);
            float dy=(target.sy-source.sy);
            float dx=(target.sx-source.sx);
            if (dx==0.0) dx=0.1;
            float angle=-1800*atan(dy/dx)/pi;
            dx=(target.sx+source.sx)/2;
            dy=(target.sy+source.sy)/2;
            w.DrawText(dx, dy, angle, e.label, Arial(), textcolor);
        }
        for(int i=0; i<nodes.GetCount(); i++){
            GrafNode& n= nodes[i];
            if (n.sel){
                continue;
            }
            PaintNode(w, n);
        }
        //foreground
        for(int i=0; i<edges.GetCount(); i++){
            GrafEdge& e=edges[i];
            GrafNode& source= nodes[e.source];
            GrafNode& target= nodes[e.target];
            Color color=defaultcolor;
            Color textcolor=color;
            if (source.sel || target.sel){
				color=LtRed();
				textcolor=Black();
				w.DrawLine(source.sx, source.sy, target.sx, target.sy, 15*e.weight, color);
	            float dy=(target.sy-source.sy);
	            float dx=(target.sx-source.sx);
	            if (dx==0.0) dx=0.1;
	            float angle=-1800*atan(dy/dx)/pi;
	            dx=(target.sx+source.sx)/2;
	            dy=(target.sy+source.sy)/2;
	            w.DrawText(dx, dy, angle, e.label, Arial(), textcolor);
            }
        }
        for(int i=0; i<nodes.GetCount(); i++){
            GrafNode& n= nodes[i];
            if (n.sel){
                PaintNode(w, n);
            }
        }
    }
    void setSel(int i){
        if (sel!=i){
            sel=i;
            if (sel>=0) WhenNodeSet(&nodes[sel]);
        }
    }
    virtual void LeftDown(Point p, dword) {
        int hit=-1;
        for(int i=0; i<nodes.GetCount(); i++){
            GrafNode& n= nodes[i];
            float dx= (n.sx-p.x);
            float dy= (n.sy-p.y);
            float dd= dx*dx+dy*dy;
            n.sel= ((n.r*n.r)>dd);
            if (n.sel){
	            hit=i;
	            //break;
            }
        }
        setSel(hit);
        mTool=p;
        Refresh();
    }
    virtual void LeftUp(Point p, dword){
        setSel(-1);
    }
    void RecurseDist(Point p, int n, int deep){
        if (n<0)return;
        deep--;
        if (deep<0) return;
        if (nodes[n].opid==opid) return;
        nodes[n].opid=opid;
        nodes[n].x=p.x;
        nodes[n].y=p.y;
        for(int i=0;i<edges.GetCount(); i++){
            GrafEdge& e=edges[i];
            int id=-1;
            float l=nodes[n].r*2;
            if (n == e.source){
                id=e.target;
            }
            if (n == e.target){
				id=e.source;
		    }
		    if (id>=0){
                GrafNode& t=nodes[id];
                Point z;
                Point d;
                d.x=(t.x-p.x);
                d.y=(t.y-p.y);
                if (d.x>0){
                    z.x=p.x+l;
                }else{
                    z.x=p.x-l;
                }
                if (d.y>0){
                    z.y=p.y+l;
                }else{
                    z.y=p.y-l;
                }
                RecurseDist(z, id, deep);
            }
        }
    }
    
    virtual void MouseMove(Point p, dword dw)
    {

        if (sel<0){
            if (K_MOUSELEFT&dw){
				if (K_ALT&dw){ //set origo
				mOffset=p;
				}else{
					mOffset-=mTool-p;
					mTool=p;
				}
				Refresh();
            }
			return;
        }
        if (!(K_MOUSELEFT&dw)) return;
        opid++;
        p=p-mOffset;
        p.x=p.x*SCREEN_DIVX/mZoomLevel;
        p.y=p.y*SCREEN_DIVX/mZoomLevel;
        RecurseDist(p, sel, 1);
        Refresh();
    }
     void MouseWheel(Point p, int zdelta, dword keyflags)override{
        const double oldZoomLevel = mZoomLevel;
        mZoomLevel = max(1.0, min(mZoomLevel + (zdelta > 0.0 ? 1.0 : -1.0), 900.0));
        const Point cursorPosRelativeToOffset = p - mOffset;

        if (mZoomLevel != oldZoomLevel) {
            mOffset.x += cursorPosRelativeToOffset.x;
            mOffset.y += cursorPosRelativeToOffset.y;
            Refresh();
        }
    }
    Graf();

};
#endif
