#ifndef _CBOX
#define _CBOX

class CBox {
public:
	int left, bottom;
	int right, top;
	int width, height;
	
	CBox (): left(0), bottom(0), right(0), top(0) {}
	CBox (int l, int b, int r, int t) : left(l), bottom(b), right(r), top(t), width(r - l), height(t - b) {}
	bool inside (const CBox& box) const;
};

#endif
