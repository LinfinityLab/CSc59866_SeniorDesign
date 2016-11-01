// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW_histoMatch:
//
// Apply histogram matching to I1. Output is in I2.
//
void
HW_histoMatch(ImagePtr I1, ImagePtr Ilut, ImagePtr I2)
{
	IP_copyImageHeader(I1, I2);
	int w = I1->width ();
	int h = I1->height();
	int total = w * h;

	// scale dynamic range of I1 to full intensity range [0,255]
	IP_embedRange(I1, 0., (double)MaxGray, I2);

	// allocate memory
	int len = Ilut->width();
	if(len <= MaxGray)
		fprintf(stderr, "IP_histogramMatch: warning %d\n", len);
	int	*lut = new  int[len];
	double	*dd1 = new double[w];
	if(dd1 == NULL) IP_bailout("IP_histogramMatch: No memory");

	int	  h1[MXGRAY],   lim[MXGRAY], luttype=0, *h2, t=0, R;
	int	left[MXGRAY], right[MXGRAY], indx[MXGRAY];
	double	hmin, hmax;
	ChannelPtr<uchar> p, endd, lutp;
	for(int ch=0; IP_getChannel(I2, ch, p, t); ch++) {
		IP_getChannel(Ilut, ch, lutp, luttype);
		h2 = (int *) &lutp[0];

		// scale h2 to conform with dimensions of I1
		int Hsum =0;
		for(int i=0; i < MXGRAY; i++) Hsum += h2[i];
		double scale = (double) total / Hsum;
		if(scale != 1) {
			Hsum = 0;
			for(int i=0; i < MXGRAY; i++) {
				h2[i]  = ROUND(h2[i] * scale);
				Hsum  += h2[i];
				if(Hsum > total) {
					h2[i] -= (Hsum-total);
					for(; i < MXGRAY; i++) h2[i] = 0;
				}
			}
		}

		// eval h1 and init left[], right[], and lim[]
		IP_histogram(I2, ch, h1, MXGRAY, hmin, hmax);
		R = Hsum = 0;
		for(int i=0; i<MXGRAY; i++) {
			left[i] = indx[i] = R;	// left end of interval
			lim [i] = h2[R] - Hsum;	// leftover on left
			Hsum   += h1[i];	// cum. interval value

			// widen interval if Hsum>h2[R]
			while(Hsum>h2[R] && R<MaxGray) {
				Hsum -= h2[R];
				R++;
			}
			right[i] = R;		// right end of interval
		}

		// clear h1 and reuse it below
		for(int i=0; i<MXGRAY; i++) h1[i] = 0;

		IP_getChannel(I2, ch, p, t);
		for(endd=p+total; p<endd; p++) {
			int i = indx[*p];
			if(i == left[*p]) {
				if(lim[*p]-- <= 0)
					i=indx[*p] = MIN(i+1,MaxGray);
				*p = i;
			} else if(i < right[*p]) {
				if(h1[i] < h2[i]) *p = i;
				else *p=indx[*p] = MIN(i+1,MaxGray);
			} else	*p = i;
			h1[i]++;
		}
	}
	delete [] lut;
	delete [] dd1;
}
