// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW_gammaCorrect:
//
// Gamma correct image I1. Output is in I2.
//
void
HW_gammaCorrect(ImagePtr I1, double gamma, ImagePtr I2)
{
	IP_copyImageHeader(I1, I2);
	int w = I1->width ();
	int h = I1->height();
	int total = w * h;

	// init gamma
	gamma = 1.0 / gamma;

	// init lookup table
	uchar lut[MXGRAY];
	for(int i=0; i<MXGRAY; i++)
		lut[i] = (int) (MaxGray * pow((double) i/MaxGray, gamma));

	// evaluate output: each input pixel indexes into lut[] to eval output
	int type;
	ChannelPtr<uchar> p1, p2, endd;
	for(int ch = 0; IP_getChannel(I1, ch, p1, type); ch++) {
		IP_getChannel(I2, ch, p2, type);
		for(endd = p1 + total; p1<endd;) *p2++ = lut[*p1++];
	}
}
