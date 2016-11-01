// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW_quantize:
//
// Quantize I1 to specified number of levels. Apply dither if flag is set.
// Output is in I2.
//
void
HW_quantize(ImagePtr I1, int levels, bool dither, ImagePtr I2)
{
	IP_copyImageHeader(I1, I2);
	int w = I1->width ();
	int h = I1->height();
	int total = w * h;

	// init lookup table
	double scale = (double) MXGRAY / levels;
	double bias  = scale / 2;
	int i, lut[MXGRAY];
	for(i=0; i<MXGRAY; ++i)
		lut[i] = (scale * (int) (i/scale)) + bias;

	if(!dither) {
		// evaluate output: each input pixel indexes into lut[] to eval output
		int type;
		ChannelPtr<uchar> p1, p2, endd;
		for(int ch = 0; IP_getChannel(I1, ch, p1, type); ch++) {
			IP_getChannel(I2, ch, p2, type);
			for(endd = p1 + total; p1<endd;) *p2++ = lut[*p1++];
		}
	} else {
		int type, j, k, s;
		ChannelPtr<uchar> p1, p2;
		for(int ch = 0; IP_getChannel(I1, ch, p1, type); ch++) {
			IP_getChannel(I2, ch, p2, type);
			for(int y=0; y<h; y++) {
				// first sign value alternates in each row
				s = (y%2) ? 1 : -1;

				// process all pixels in row, alternating sign value
				for(int x=0; x<w; x++) {
					// jitter is in [0,bias] range
					j  = ((double) rand() / RAND_MAX) * bias;

					// add signed jitter value
					k  = *p1++ + j*s;

					// alternate sign for next pixel
					s *= -1;

					// eval output using jittered value
					*p2++ = lut[ CLIP(k, 0, MaxGray) ];
				}
			}
		}
	}
}
