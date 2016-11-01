// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW_histoStretch:
//
// Apply histogram stretching to I1. Output is in I2.
// Stretch intensity values between t1 and t2 to fill the range [0,255].
//
void
HW_histoStretch(ImagePtr I1, int t1, int t2, ImagePtr I2)
{
	IP_copyImageHeader(I1, I2);
	int w = I1->width ();
	int h = I1->height();
	int total = w * h;

	// init clip lut
	int lutClip[MXGRAY];
	for(int i=0; i<MXGRAY; i++) {
		if	(i < t1) lutClip[i] = t1;
		else if	(i > t2) lutClip[i] = t2;
		else		 lutClip[i] = i;
	}

	// evaluate output
	int type;
	int lutScale[MXGRAY];
	ChannelPtr<uchar> p1, p2, endd;
	double	scale;
	for(int ch = 0; IP_getChannel(I1, ch, p1, type); ch++) {
		IP_getChannel(I2, ch, p2, type);

		// clip image: apply lutClip table
		for(endd = p1 + total; p1<endd;) *p2++ = lutClip[*p1++];

		// error checking: avoid divide-by-zero error later
		if(t1 == t2) t2++;

		// init lookup table
		scale = (double) MaxGray / (t2 - t1);
		for(int i=0; i<MXGRAY; i++)
			lutScale[i] = (i-t1)*scale;

		// apply lookup table
		IP_getChannel(I2, ch, p2, type);
		for(endd = p2 + total; p2<endd; p2++) *p2 = lutScale[*p2];
	}
}
