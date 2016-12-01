float correlation(ImagePtr I1, ImagePtr I2, int &xx, int &yy) {
    // init vars to suppress compiler warnings
	int	  dx = 0;
	int	  dy = 0;
	int   lowres = 0;
	float	mag  = 0;
	float	corr = 0;
	double	xsz, ysz;

	// image dimensions
	int w = I1->width ();
	int h = I1->height();

	// template dimensions
	int ww = I2->width ();
	int hh = I2->height();

	// error checking: size of image I1 must be >= than template I2
	if(!(ww<=w && hh<=h)) {
		fprintf(stderr, "Correlation: image is smaller than template\n");
		return 0.;
	}

	// cast image into buffer of type float
	ImagePtr II1;
	if(I1->channelType(0) != FLOAT_TYPE) {
		II1 = IP_allocImage(I1->width(), I1->height(), FLOATCH_TYPE);
		IP_castChannel(I1, 0, II1, 0, FLOAT_TYPE);
	} else	II1 = I1;

	// cast template into buffer of type float
	ImagePtr II2;
	if(I2->channelType(0) != FLOAT_TYPE) {
		II2 = IP_allocImage(I2->width(), I2->height(), FLOATCH_TYPE);
		IP_castChannel(I2, 0, II2, 0, FLOAT_TYPE);
	} else	II2 = I2;

	// create image and template pyramids with original images at base;
	// if no multiresolution is used, pyramids consist of only one level.
	int mxlevel;
	ImagePtr pyramid1[8], pyramid2[8];
	pyramid1[0] = II1;		// base: original image
	pyramid2[0] = II2;		// base: original template
	mxlevel = 0;

	// init search window
	int x1 = 0;
	int y1 = 0;
	int x2 = (w-ww)>>mxlevel;
	int y2 = (h-hh)>>mxlevel;

	// declarations
	int		  total;
	float		  sum1, sum2, avg, tmpl_pow;
	ChannelPtr<float> image, templ;
	ImagePtr	  Iblur, Ifft1, Ifft2;

	// multiresolution correlation: use results of lower-res correlation
	// (at the top of the pyramid) to narrow the search in the higher-res
	// correlation (towards the base of the pyramid).
	for(int n=mxlevel; n>=0; n--) {
	    // init vars based on pyramid at level n
	    w  = pyramid1[n]->width(); h  = pyramid1[n]->height();
	    ww = pyramid2[n]->width(); hh = pyramid2[n]->height();

	    // pointers to image and template data
	    ChannelPtr<float> p1 = pyramid1[n][0];	// image    ptr
	    ChannelPtr<float> p2 = pyramid2[n][0];	// template ptr

	    // init min and max
	    float min = 10000000.;
	    float max = 0.;

		for(int y=y1; y<=y2; y++) {		// visit rows
		    for(int x=x1; x<=x2; x++) {		// slide window
			sum1  = sum2 = 0;
			image = p1 + y*w + x;
			templ = p2;
			for(int i=0; i<hh; i++) {	// convolution
		   		for(int j=0; j<ww; j++) {
					sum1 += (templ[j] * image[j]);
					sum2 += (image[j] * image[j]);
				}
				image += w;
				templ += ww;
			}
			if(sum2 == 0) continue;

			corr = sum1 / sqrt(sum2);
			if(corr > max) {
		   		max = corr;
		   		dx  = x;
		   		dy  = y;
			}
		    }
		}

		// update search window or normalize final correlation value
		if(n) {		// set search window for next pyramid level
			x1 = MAX(0,   2*dx - n);
			y1 = MAX(0,   2*dy - n);
			x2 = MIN(2*w, 2*dx + n);
			y2 = MIN(2*h, 2*dy + n);
		} else {	// normalize correlation value at final level
			tmpl_pow = 0;
			total	 = ww * hh;
			for(int i=0; i<total; i++)
				tmpl_pow += (p2[i] * p2[i]);
			corr =	max / sqrt(tmpl_pow);
		}
	}

	xx = dx;
	yy = dy;
	return corr;
}


float* HW_correlation(ImagePtr I1, ImagePtr Itemplate, ImagePtr I2) {
    ImagePtr tempImage;
    IP_copyImageHeader(I1, I2);
    IP_copyImageHeader(I1, tempImage);

    int width = I1->width();
    int height = I1->width();

    int width_k = Itemplate->width();
    int height_k = Itemplate->height();
    int xx, yy; // coordinate of where template image sits in input image

    // get cross-correlation value
    float corr = correlation(I1, Itemplate, xx, yy);

    int type;
    ChannelPtr<uchar> p1, p2, p3, endd;
	for(int ch = 0; IP_getChannel(I1, ch, p1, type); ch++) {
        IP_getChannel(I2, ch, p2, type);
        IP_getChannel(Itemplate, ch, p3, type);
		for(int i = 0; i < width; i++) {
            for(int j = 0; j < height; j++) {
                if(i < yy) {    // if we are above template image
                    *p2++ = *p1/2;
                    p1++;
                } else if(i >= yy && i <= (yy-1+height_k)) {    // if we are on the rows where template image resides
                    if(j < xx) {    // if we are before the template image columns
                        *p2 = *p1/2;
                    } else if(j >= xx && j < (xx+width_k)) {    // if we are on the columns where template resides
                        *p2 = *p1/2 + *p3/2;
                        p3++;
                    } else {    // if we are after the template image columns
                        *p2 = *p1/2;
                    }
                    p1++;
                    p2++;
                } else {    // if we are below the template images
                    *p2++ = *p1/2;
                    p1++;
                }
            }
        }
	}
    float *buf = new float[3];
    buf[0] = xx;
    buf[1] = yy;
    buf[2] = corr;
    return buf;
}
