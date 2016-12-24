// ======================================================================
// IMPROC: Image Processing Software Package
// Copyright (C) 2016 by George Wolberg
//
// IPdefs.h - Header for IP symbolic constants (#defines) and globals.
//
// Written by: George Wolberg, 2016
// ======================================================================

//! \file	IPdefs.h
//! \brief	Header for IP symbolic constants (#defines) and globals.
//! \author	George Wolberg, 2016

#ifndef IPDEFS_H
#define IPDEFS_H

// ----------------------------------------------------------------------
// standard include files
//
#include <cstdio>
#include <iostream>
#include <cmath>
#include <cstring>
#include <cstdlib>
#include <cstdarg>
#include <vector>
#include <map>
#include <QtGlobal>

namespace IP {

// ----------------------------------------------------------------------
// datatype typedefs
//
// comment out uchar typedef because it is already defined in Qt library
// typedef	unsigned char uchar;



// ----------------------------------------------------------------------
// math definitions
//
#define PI		3.1415926535897931160E0
#define PI2		6.2831853071795862320E0
#define PI_2		1.5707963267948965580E0
#define DEGtoRAD	0.0174532927777777777E0
#define RADtoDEG	57.295778666661658617E0
#define NEG_INF		-9999999



// ----------------------------------------------------------------------
// miscellaneous definitions
//
#define LSB		0x0001
#define MXBLUR		256
#define MXCH		8192
#define MXCHANNEL	16
#define MXGRAY		256
#define MXHALFTONE	10
#define MXRES		4096
#define MXSTRLEN	1024
#define MXWINDOWS	16



// ----------------------------------------------------------------------
// channel sizes
//
#define  UCHAR_SZ	sizeof(uchar)
#define  SHORT_SZ	sizeof(short)
#define    INT_SZ	sizeof(int)
#define   LONG_SZ	sizeof(long)
#define  FLOAT_SZ	sizeof(float)
#define DOUBLE_SZ	sizeof(double)



// ----------------------------------------------------------------------
// pixel sizes
//
#define   BW_PXLSZ	   UCHAR_SZ
#define  BWA_PXLSZ	(2*UCHAR_SZ)
#define  RGB_PXLSZ	(3*UCHAR_SZ)
#define RGBA_PXLSZ	(4*UCHAR_SZ)
#define  HSV_PXLSZ	(3*SHORT_SZ)
#define  YIQ_PXLSZ	(3*SHORT_SZ)



// ----------------------------------------------------------------------
// useful macros
// Note: ROUND   rounds arg to nearest integer
// Note: FLOOR   rounds arg to nearest integer towards -infinity
// Note: CEILING rounds arg to nearest integer towards +infinity
//
#define ABS(A)		((A) >= 0 ? (A) : -(A))
#define SGN(A)		((A) >  0 ? 1 : ((A) < 0 ? -1 : 0 ))
#define ROUND(A)	((A) >= 0 ? (int)((A)+.5) : -(int)(.5-(A)))
#define FLOOR(A)	((A)==(int)(A) ? (int)(A) : (A)>0 ? (int)(A):(int)(A)-1)
#define CEILING(A)	((A)==(int)(A) ? (int)(A) : (A)>0 ? (int)(A)+1:(int)(A))
#define CLIP(A,L,H)	((A)<=(L) ? (L) : (A)<=(H) ? (A) : (H))
#define MAX(A,B)	((A) > (B) ? (A) : (B))
#define MIN(A,B)	((A) < (B) ? (A) : (B))
#define SWAP(A,B)	{ double temp=(A); (A)  = (B);	(B)  = temp; }
#define SWAP_INT(A,B)	{ (A) ^= (B);	   (B) ^= (A);	(A) ^= (B);  }

#define NEWIMAGE				IP_allocImage(0,  0, NULL_TYPE)
#define IP_copyImageHeader(I1, I2)		IP_copyHeader(I1, 1, I2)
#define IP_copyImageHeaderOnly(I1, I2)		IP_copyHeader(I1, 0, I2)
#define IP_copyImageHeader2(I1, I2, I3)		IP_copyHeader2(I1, I2, 1, I3)
#define IP_copyImageHeaderOnly2(I1, I2, I3)	IP_copyHeader2(I1, I2, 0, I3)



// ----------------------------------------------------------------------
// type casting macro:
// cast total objects in p1 (of type t1) into p2 (with type t2)
// with strides s1 and s2, respectively
//
#define MEM_CAST(p1, t1, s1, p2, t2, s2, total)\
	{\
	t1 *src;\
	t2 *dst;\
	int i;\
	src = (t1 *) p1;\
	dst = (t2 *) p2;\
	for(i=0; i<total; i++,src+=s1,dst+=s2) *dst = (t2) *src;\
	}



// ----------------------------------------------------------------------
// image and channel types
// IMPORTANT:	CHTYPE_IMAGE[] and CHTYPE_CH[] are dependent on numbering
//		used in image and channel types, respectively.
//
enum image_types {
	 NULL_IMAGE,
	   BW_IMAGE,
	  BWA_IMAGE,
	  RGB_IMAGE,
	 RGBA_IMAGE,
	  HSV_IMAGE,
	  YIQ_IMAGE,
	  LUT_IMAGE,
	  MAT_IMAGE,
	  FFT_IMAGE,
	   GL_IMAGE
};

enum channel_types {
	 UCHAR_TYPE,
	 SHORT_TYPE,
	   INT_TYPE,
	  LONG_TYPE,
	 FLOAT_TYPE,
	DOUBLE_TYPE
};



// ----------------------------------------------------------------------
// filter kernels for image resizing
//
enum filter_kernels {
	BOX,
	TRIANGLE,
	CSPLINE,
	CUBIC_CONV,
	LANCZOS,
	HANN
};



// ----------------------------------------------------------------------
// dither options for error diffusion
//
enum dither_options {
	FLOYD_STEINBERG,
	OSTROMOUKHOV,
	JARVIS_JUDICE_NINKE,
	FAN,
	STUCKI,
	BURKES,
	SIERRA,
	STEVENSON_ARCE,
	USER_SPECIFIED
};



// ----------------------------------------------------------------------
// correlation options
//
enum correlation_options {
	CROSS_CORR,
	SSD,
	CORR_COEFF,
	PHASE_CORR
};



// ----------------------------------------------------------------------
// pad options
//
enum pad_options {
	CONSTANT,
	REPLICATE,
	REFLECT,
	EXTRAPOLATE,
	WRAPAROUND
};

}	// namespace IP

#endif	// IPDEFS_H
