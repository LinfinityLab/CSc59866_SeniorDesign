// ======================================================================
// IMPROC: Image Processing Software Package
// Copyright (C) 2016 by George Wolberg
//
// IPmmch.tpp - Template functions for memory management.
//
// Written by: George Wolberg, 2016
// ======================================================================

//! \file	IPmmch.tpp
//! \brief	Template functions for memory management.
//! \author	George Wolberg, 2016

using namespace IP;

//! \addtogroup mmch
//@{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// IP_getChannel:
//
// Pass ptr and type of channel num in I through args ptr and type.
// Return true for success, false for failure.
//! \brief	Pass ptr and type of channel num in I through args
//!		ptr and type.
//! \param[in]	I	- Input image.
//! \param[in]	ch	- Input channel index.
//! \param[out]	ptr	- Output channel.
//! \param[in]	type	- Output channel datatype.
//! \return	true for success, false for failure.
//
template<class T>
inline bool
IP_getChannel(ImagePtr I, int ch, ChannelPtr<T> &ptr, int &type)
{
	if(ch<0 || ch>MXCHANNEL) {
		fprintf(stderr, "IP_getChannel: Illegal channel %d\n", ch);
		return false;
	}
	if(ch>=I->maxChannel()) {
		return false;
	}

	ptr  = I[ch];
	type = I->channelType(ch);
	return true;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// IP_copyFromRow:
//
// Copy a row of an image channel to an output channel.
//! \brief	Copy a row of an image channel to an output channel.
//! \param[in]	p	- Input channel pointer.
//! \param[in]	row	- Row index.
//! \param[in]	w	- Row width.
//! \param[out]	q	- Output channel pointer.
//
template<class T1, class T2>
inline void
IP_copyFromRow(ChannelPtr<T1> p, int row, int w, ChannelPtr<T2> q)
{
	p += row * w;
	for(int i=0; i<w; ++i)
		q[i] = static_cast<T2>(p[i]);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// IP_copyFromRow:
//
// Copy a row of an image to a channel (See IPfft.cpp).
//! \brief	Copy a row of an image to a channel (See IPfft.cpp).
//! \param[in]	I	- Input image.
//! \param[in]	ch	- Input channel index.
//! \param[in]	row	- Input row index.
//! \param[out]	p	- Output channel.
//
template<class T>
inline void
IP_copyFromRow(ImagePtr I, int ch, int row, ChannelPtr<T> p)
{
	int w = I->width();
	int type = I->channelType(ch);

	switch(type) {
	case UCHAR_TYPE:
		{
		ChannelPtr<uchar> q;
		IP_getChannel(I, ch, q, type);
		IP_copyFromRow(q, row, w, p);
		break;
		}
	case SHORT_TYPE:
		{
		ChannelPtr<short> q;
		IP_getChannel(I, ch, q, type);
		IP_copyFromRow(q, row, w, p);
		break;
		}
	case INT_TYPE:
		{
		ChannelPtr<int> q;
		IP_getChannel(I, ch, q, type);
		IP_copyFromRow(q, row, w, p);
		break;
		}
	case LONG_TYPE:
		{
		ChannelPtr<long> q;
		IP_getChannel(I, ch, q, type);
		IP_copyFromRow(q, row, w, p);
		break;
		}
	case FLOAT_TYPE:
		{
		ChannelPtr<float> q;
		IP_getChannel(I, ch, q, type);
		IP_copyFromRow(q, row, w, p);
		break;
		}
	case DOUBLE_TYPE:
		{
		ChannelPtr<double> q;
		IP_getChannel(I, ch, q, type);
		IP_copyFromRow(q, row, w, p);
		break;
		}
	}
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// IP_copyToRow:
//
// Copy a channel to a row of an image channel.
//! \brief	Copy a channel to a row of an image channel.
//! \param[in]	p	- Input channel pointer.
//! \param[out]	q	- Output channel pointer.
//! \param[in]	row	- Row index.
//! \param[in]	w	- Row width.
//
template<class T1, class T2>
inline void
IP_copyToRow(ChannelPtr<T1> p, ChannelPtr<T2> q, int row, int w)
{
	q += row * w;
	for(int i=0; i<w; i++)
		q[i] = static_cast<T2>(p[i]);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// IP_copyToRow:
//
// Copy a channel's data into a row of an image (See IPfft.cpp).
//! \brief	Copy a channel's data into a row of an image (See IPfft.cpp).
//! \param[in]	p	- Input channel.
//! \param[out]	I	- Output image.
//! \param[in]	ch	- Output channel index.
//! \param[in]	row	- Output row index.
//
template<class T>
inline void
IP_copyToRow(ChannelPtr<T> p, ImagePtr I, int ch, int row)
{
	int w = I->width();
	int type = I->channelType(ch);

	switch(type) {
	case UCHAR_TYPE:
		{
		ChannelPtr<uchar> q;
		IP_getChannel(I, ch, q, type);
		IP_copyToRow(p, q, row, w);
		break;
		}
	case SHORT_TYPE:
		{
		ChannelPtr<short> q;
		IP_getChannel(I, ch, q, type);
		IP_copyToRow(p, q, row, w);
		break;
		}
	case INT_TYPE:
		{
		ChannelPtr<int> q;
		IP_getChannel(I, ch, q, type);
		IP_copyToRow(p, q, row, w);
		break;
		}
	case LONG_TYPE:
		{
		ChannelPtr<long> q;
		IP_getChannel(I, ch, q, type);
		IP_copyToRow(p, q, row, w);
		break;
		}
	case FLOAT_TYPE:
		{
		ChannelPtr<float> q;
		IP_getChannel(I, ch, q, type);
		IP_copyToRow(p, q, row, w);
		break;
		}
	case DOUBLE_TYPE:
		{
		ChannelPtr<double> q;
		IP_getChannel(I, ch, q, type);
		IP_copyToRow(p, q, row, w);
		}
	}
}
//@}
