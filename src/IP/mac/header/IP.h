// ======================================================================
// IMPROC: Image Processing Software Package
// Copyright (C) 2016 by George Wolberg
//
// IP.h - Header for IP*.cpp files
//
// Written by: George Wolberg, 2016
// ======================================================================

//! \mainpage	Image Processing Package
//! \author	George Wolberg
//! \date	2016
//! \par	References:
//!		This code is part of the
//!		IMPROC Image Processing Software Package\n
//!		Copyright &copy; 2016 by George Wolberg

//! \file	IP.h
//! \brief	Image Processing Package main header file.
//! \author	George Wolberg and Gene Yu, 2016

#ifndef IP_H
#define IP_H

#include "IPdefs.h"
#include "Channel.h"
#include "ChannelPtr.h"
#include "Image.h"
#include "ImagePtr.h"
#include <QtWidgets>

namespace IP {

// ----------------------------------------------------------------------
// system variables (set/fixed at start of session)
//
extern int	 MaxGray;
extern int	 Verbose;
extern int	 SIZEOF[];

extern int	 NULL_TYPE[];
extern int	   BW_TYPE[];
extern int	  BWA_TYPE[];
extern int	  RGB_TYPE[];
extern int	 RGBA_TYPE[];
extern int	  HSV_TYPE[];
extern int	  YIQ_TYPE[];
extern int	  LUT_TYPE[];
extern int	  MAT_TYPE[];
extern int	  FFT_TYPE[];

extern int	  UCHARCH_TYPE[];
extern int	  SHORTCH_TYPE[];
extern int	    INTCH_TYPE[];
extern int	   LONGCH_TYPE[];
extern int	  FLOATCH_TYPE[];
extern int	 DOUBLECH_TYPE[];

extern int	  INT2_TYPE[];
extern int	FLOAT2_TYPE[];
extern int	  INT3_TYPE[];
extern int	FLOAT3_TYPE[];
extern int	  INT4_TYPE[];
extern int	FLOAT4_TYPE[];

extern int	*CHTYPE_CH[];
extern int	*CHTYPE_IMAGE[];
extern char	*CHTYPE_NAME[];
extern char	 CHTYPE_LTR[]; 

extern uchar	 BITMASK[];



// ----------------------------------------------------------------------
// remaining global variables (may change during session)
//

extern int	AbsVal;
extern int	BorderVal;
extern double	CubicConvA;
extern int	HistoFit;
extern int	ImagePadLen;
extern int	ImagePadMtd;
extern double	PlotScl;
extern int	SavePixmap;
extern int	Serpentine;
extern int	JpgQuality;
extern int	SwapBytes;


//		IPcastimg.cpp	- type casting routines for images
extern void	IP_castImage	   (ImagePtr, int, ImagePtr);
extern void	IP_initAlphaChannel(ImagePtr, int);

//		IPcastmem.cpp	- type casting routines for memory blocks
extern void	IP_castMemory	(void*, int, int, void*, int, int, int);

//		IPcolorconv.cpp	- conversion between color representations
extern void	IP_RGBtoYIQ1	(int, int, int, int&, int&, int&);
extern void	IP_YIQtoRGB1	(int, int, int, int&, int&, int&);
extern void	IP_RGBtoHSV1	(int, int, int, int&, int&, int&);
extern void	IP_HSVtoRGB1	(int, int, int, int&, int&, int&);
extern void	IP_clipRGB	(int&, int&, int&);

//		IPhisto.cpp	- histogram evaluation
extern void	IP_histogram	(ImagePtr, int, int*, int, double&, double&);

//		IPinit.cpp	- initialize global IP data structures
extern void	IP_init		(void);

//		IPio.cpp	- image input/output
extern ImagePtr	IP_readImage	      (const char*);
extern bool	IP_readImageIntoI     (const char*, ImagePtr);
extern bool	IP_readImageDimensions(const char*, int&, int&);
extern bool	IP_saveImage	      (ImagePtr, const char*, const char*);

//		IPmipmap.cpp	- mipmap routines
extern int	IP_mipmap	(Image, Image&);

//		IPmmch.cpp	- channel memory management
#include "IPmmch.tpp"
template<class T>
	bool	IP_getChannel 	  (ImagePtr, int, ChannelPtr<T>&, int &);
template<class T1, class T2>
	void	IP_copyFromRow	  (ChannelPtr<T1>, int, int, ChannelPtr<T2>);
template<class T>
	void	IP_copyFromRow	  (ImagePtr, int, int, ChannelPtr<T>);
template<class T>
	void    IP_copyToRow	  (ChannelPtr<T>, ImagePtr , int, int);
template<class T1, class T2>
	void	IP_copyToRow	  (ChannelPtr<T1>, ChannelPtr<T2>, int, int);

extern void	IP_copyChannel	  (ImagePtr, int, ImagePtr, int );
extern void	IP_copyChannelEnd (ImagePtr, int, ImagePtr);
extern void	IP_castChannel	  (ImagePtr, int, ImagePtr, int, int);
extern void	IP_castChannels	  (ImagePtr, int*,ImagePtr);
extern void	IP_castChannelsEq (ImagePtr, int, ImagePtr);
extern void	IP_castChannelsMin(ImagePtr, int, ImagePtr);
extern void	IP_castChannelsMax(ImagePtr, int, ImagePtr);
extern void	IP_clearChannel	  (ImagePtr, int, ImagePtr);
extern int	IP_checkDimensions(ImagePtr, ImagePtr);

//	IPmmimg.cpp
extern Image   *IP_allocImage	(int, int, int*);
extern Image   *IP_allocImage_I	(ImagePtr);
extern void	IP_allocImageInI(ImagePtr, int, int, int*);
extern void	IP_copyImage	(ImagePtr, ImagePtr);
extern void	IP_copyHeader	(ImagePtr, int, ImagePtr);
extern void	IP_copyHeader2	(ImagePtr, ImagePtr, int, ImagePtr);
extern void	IP_copyImageBuffer(ImagePtr, ImagePtr);


//	IPutil.cpp
extern void	IP_interleave	(ImagePtr, ImagePtr);
extern void	IP_interleave4	(ImagePtr, int, int, int, int, ImagePtr);
extern void	IP_uninterleave	(ImagePtr, ImagePtr);
extern void	IP_normalizeDimensions(ImagePtr, ImagePtr);
extern void	IP_crop		(ImagePtr, int, int, int, int, ImagePtr);
extern void	IP_shift	(ImagePtr, int,  int, ImagePtr);
extern void	IP_pad		(ImagePtr, int*, int, ImagePtr);
extern void	IP_pad1D	(ChannelPtr<uchar>, int, int, int, int,
				 ChannelPtr<uchar>&);
extern void	IP_fpad1D	(ChannelPtr<float>, int, int, int, int,
				 ChannelPtr<float>&);
extern void	IP_clearImage	(ImagePtr);
extern void	IP_setImage	(ImagePtr, double*);
extern void	IP_minmaxImage	(ImagePtr, double&, double&);
extern void	IP_minmaxChannel(ImagePtr, int, double&, double&);
extern void	IP_RGBdecouple	(ImagePtr, ImagePtr &, bool decouple=true);
extern void	IP_RGBcouple	(ImagePtr, ImagePtr, ImagePtr);
extern void	IP_bailout	(const char*, ...);
extern void	IP_clip		(ImagePtr, double, double, ImagePtr);
extern void	IP_embedRange	(ImagePtr I1, double t1, double t2, ImagePtr I2);
extern void	IP_scaleRange   (ImagePtr I1, double t1, double t2, ImagePtr I2);
//      IPtoUI.cpp
extern void     IP_printfErr(const char *fmt, ...);

}	// namespace IP

#endif	// IP_H
