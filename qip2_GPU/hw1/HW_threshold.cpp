// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW_threshold:
//
// Threshold I1 using threshold thr. Output is in I2.
// input<thr: output=0;	 input >= thr: output=MaxGray (255)
//
void
HW_threshold(ImagePtr I1, int thr, ImagePtr I2)
{
	IP_copyImageHeader(I1, I2);
	int w = I1->width ();
	int h = I1->height();
	int total = w * h;

	// init lookup table
	int i, lut[MXGRAY];
	for(i=0; i<thr && i<MXGRAY; ++i) lut[i] = 0;
	for(   ;          i<MXGRAY; ++i) lut[i] = MaxGray;

	// evaluate output: each input pixel indexes into lut[] to eval output
	int type;
	ChannelPtr<uchar> p1, p2, endd;
	for(int ch = 0; IP_getChannel(I1, ch, p1, type); ch++) {
		IP_getChannel(I2, ch, p2, type);
		for(endd = p1 + total; p1<endd;) *p2++ = lut[*p1++];
	}
}
