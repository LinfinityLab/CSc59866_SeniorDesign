// ======================================================================
// IMPROC: Image Processing Software Package
// Copyright (C) 2015 by George Wolberg
//
// IPblur.tpp - Template functions for blurring 1D scanlines.
//
// Written by: George Wolberg, 2015
// ======================================================================

//! \file	IPblur.tpp
//! \brief	Image blurring template functions.
//! \author	George Wolberg, 2015

using namespace IP;

extern Image *IP_allocImage(int, int, int*);
template<class T> void blur1D_odd(ChannelPtr<T>, int, int, double, ChannelPtr<T>);

//! \addtogroup filtnbr
//@{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// IP_blur1D:
//
// dst <- Blur src with 1D box filter of width ww.
//! \brief	Image blurring (1D version).
//! \details	\a dst <- Blur \a src with 1D box filter of width \a ww.
//! param[in]	src - ChannelPtr to input scanline.
//! param[in]	len - Number of pixels in \a src.
//! param[in]	stride - Distance between successive input pixels (in pixels).
//!		stride = 1 for processing rows.
//!		stride = scanline width for processing columns.
//! param[in]	ww - Width of blurring kernel.
//! param[out]	dst - ChannelPtr to output scanline.
//!		Distance between successive output pixels is \a stride.
//
template<class T>
void
IP_blur1D(ChannelPtr<T> src, int len, int stride, double ww, ChannelPtr<T> dst)
{
	// error checking: filter width exceeds scanline length
	if(ww > len) {
		fprintf(stderr, "IP_blur1D: filter exceeds scanline (%f>%d)\n",
			ww, len);
		return;
	}

	// trivial case: filter width is less than a pixel
	if(ww <= 1.) {
		if(src != dst) {
			// copy src to dst
			for(int i=0; i<len; ++i) {
				*dst  = *src;
				 dst += stride;
				 src += stride;
			}
		}
		return;
	}

	// check for odd window size: use simpler blur fct
	if(ww == (int) ww && (int) ww % 2) {
		blur1D_odd(src, len, stride, ww, dst);
		return;
	}

	// allocate sufficiently large working buffer
	ImagePtr II = IP_allocImage(MXRES, 1, FLOATCH_TYPE);

	// init vars
	ChannelPtr<T> buf = II[0];		// buffer channel
	double ww2 = (ww  -  1.) / 2.;		// filter half-width
	double wt1 =  ww2 - (int) ww2;		// partial coverage on right
	double wt2 =  1. - wt1;			// partial coverage on left

	// srcp is ptr to row buffer of input pixels; make copy if necessary
	ChannelPtr<T> srcp;
	if(src!=dst && stride==1)
		srcp = src;
	else {
		for(int i=0; i<len; ++i, src+=stride) buf[i] = *src;
		srcp = buf;
	}

	// accumulate sufficient pixels to fill right half of window
	// centered at the left-most pixel in scanline
	double	num = ww2 + 1;			// +1: include center pixel
	int	lim = (int) num;		// number of pixels to fetch
	double	sum = 0;			// accumulator
	int		i = 0;			// dummy variable
	for(; i<lim; ++i) sum += *srcp++;	// sum of pixels in window
	int trail = i;				// number of positions to fetch at right end later

	// continue collecting pixels to fill entire window
	double s;
	for(; num < ww; ++i) {
		 // s is right fractional part; add it to integral sum
		 s    = *srcp * wt1;		// pixel contribution
		*dst  = (T) ((sum+s) / num);	// weighted sum
		 dst +=  stride;		// advance pointer
		 sum += *srcp++;		// slide window over full pixel
		 num +=  1;			// number of accumulated pixels
	}

	// the window totally fits; continue until the end of the scanline;
	// exploit the fact that the running sum can be computed incrementally
	// by subtracting the outgoing pixel and adding the incoming one.
	int offset = -i;			// offset to outgoing pixel
	for(; i<len; ++i) {
		 // s is difference of right and left fractional parts
		 s    = (*srcp * wt1) - (srcp[offset] * wt2);
		*dst  = (T) ((sum+s) / ww);	// weighted sum
		 dst +=  stride;		// advance pointer
		 sum -=  srcp[offset];		// subtract outgoing pixel
		 sum += *srcp++;		// add incoming pixel
	}

	// the window now falls off the trailing end of the scanline
	srcp += offset;
	num   = ww - wt1;
	for(i=0; i<trail; ++i) {
		 s    = *srcp * wt2;		// pixel contribution
		*dst  = (T) ((sum-s) / num);	// weighted sum
		 dst +=  stride;		// advance pointer
		 sum -= *srcp++;		// slide window off scanline
		 num -= 1;			// number of accumulated pixels
	}
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// blur1D_odd:
//
// dst <- Blur src with 1D box filter of width ww (odd)
//! \brief	Image blurring (1D version) for odd-sized kernel width.
//! \details	\a dst <- Blur \a src with 1D box filter of width \a ww,
//!		when \a ww is known to be odd-valued.
//! param[in]	src - ChannelPtr to input scanline.
//! param[in]	len - Number of pixels in \a src.
//! param[in]	stride - Distance between successive input pixels (in pixels).
//!		stride = 1 for processing rows.
//!		stride = scanline width for processing columns.
//! param[in]	ww - Width of blurring kernel.
//! param[out]	dst - ChannelPtr to output scanline.
//!		Distance between successive output pixels is \a stride.
//
template<class T> void
blur1D_odd(ChannelPtr<T> src, int len, int stride, double ww, ChannelPtr<T> dst)
{
	// allocate sufficiently large working buffer
	ImagePtr II = IP_allocImage(MXRES, 1, FLOATCH_TYPE);

	// srcp points to row buffer of input pixels; make copy if necessary
	ChannelPtr<T> srcp;			// ptr to input pixels
	ChannelPtr<T> buf = II[0];		// buffer channel
	if(src!=dst && stride==1)
		srcp = src;
	else {
		for(int i=0; i<len; ++i, src+=stride) buf[i] = *src;
		srcp = buf;
	}

	// init vars
	int ww2    = (int)  (ww/2);		// filter half-width
	int offset = (int) (-ww  );		// offset to outgoing pixel

	// accumulate sufficient pixels to fill right half of window
	// centered at the left-most pixel in scanline
	double	num = ww2;			// doesn't include center pixel
	int	lim = (int) num;		// number of pixels to fetch
	double	sum = 0;			// accumulator
	int	i   = 0;			// dummy variable
	for(; i<lim; ++i) sum += *srcp++;	// sum of pixels in window

	// continue collecting pixels to fill entire window
	for(; num < ww; ++i) {
		*dst  =  (T) (sum / num);	// weighted sum
		 dst +=  stride;		// advance pointer
		 sum += *srcp++;		// running  sum
		 num +=  1;			// number of accumulated pixels
	}

	// the window totally fits; continue until the end of the scanline;
	// exploit the fact that the running sum can be computed incrementally
	// by subtracting the outgoing pixel and adding the incoming one.
	for(; i<len; ++i) {
		 sum -=  srcp[offset];		// subtract outgoing pixel
		 sum += *srcp++;		// add incoming pixel
		*dst  = (T) (sum / ww);		// weighted sum
		 dst +=  stride;		// advance pointer
	}

	// the window now falls off the trailing end of the scanline
	srcp += offset;
	num   = ww - 1;
	for(i=0; i<ww2; ++i) {
		 sum -= srcp[i];		// subtract outgoing pixel
		*dst  = (T) (sum / num);	// weighted sum
		 dst += stride;			// advance pointer
		 num -= 1;			// number of accumulated pixels
	}
}
//@}
