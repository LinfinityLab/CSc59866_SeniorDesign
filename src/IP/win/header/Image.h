// ======================================================================
// IMPROC: Image Processing Software Package
// Copyright (C) 2016 by George Wolberg
//
// Image.h - Image class.
//
// Written by: George Wolberg, 2016
// ======================================================================

//! \file	Image.h
//! \brief	Image class.
//! \author	George Wolberg, 2016

#ifndef IMAGE_H
#define IMAGE_H

#include "IPdefs.h"
#include "Channel.h"

namespace IP {

extern char	 CHTYPE_LTR[];

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Image class declaration
//! \brief      Image class.
//! \details    The Image class handles image memory manipulation.
//
class Image {
public:
	// constructors
	Image();			// default constructor
	Image(int, int, int *);		// construct from typelist

	// destructor
	~Image();

	// free image
	void	freeImage	();
	void	freeChannel	(int);
	void	freeChannels	(int);

	// memory allocation
	void	 allocImage  (int, int, const int*);
	void	 allocImage_I(Image &);
	Channel	*allocChannel(int size, int type);

	// reference counting
	int	  link(bool=true);
	int	unlink();

	// get methods
	int  width  () const;		// image width
	int  height () const;		// image height
	int  xoffset() const;		// column offset
	int  yoffset() const;		// row offset
	int  links  () const;		// number of links
	int  imageType   ()    const;	// image type
	int  channelType (int) const;	// channel type
	int* channelTypes();		// list of channel types
	bool freeFlag	 () const;	// free flag
	int  maxChannel	 ();		// max channel
	int  maxType	 ();		// max channel type

	// set methods
	void setWidth	   (int);
	void setHeight	   (int);
	void setXOffset	   (int);
	void setYOffset	   (int);
	void setImageType  (int);
	void setChannel	   (int, Channel *);
	void setChannelType(int, int);
	void setFreeFlag   (bool);

	// array index operator
	const Channel	*operator[](int) const;
	      Channel	*operator[](int);

	// channel operations
	void replaceChannel(int, int, int, int);
	void initChannels  (int *chtype);

	// equality operator
	friend bool operator==(const Image &I1, const Image &I2);

	// smart pointer
	friend class ImagePtr;

private:
	Channel	*m_buf    [MXCHANNEL];		// channel ptrs
	int	 m_chtype [MXCHANNEL];		// channel types
	int	 m_width,   m_height ;		// image dimensions
	int	 m_xoffset, m_yoffset;		// offset from origin
	bool	 m_delete;			// destructor delete flag
	int	 m_type;			// image type label
	int	 m_links;			// # of links
};



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Binary Operators:
//   Numerical equality operator: ==
//   Output stream operator: <<
//! \defgroup image IP: Binary Operators
//
extern bool operator==(const Image&, const Image&);
extern std::ostream &operator<<(std::ostream &out, const Image &I);



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Image::Image:
//
// Default constructor.
//! \brief	Default constructor.
//! \details	Default initialization of image components.
//!		The components are initialized to zero.
//
inline
Image::Image()
	: m_width  (0),
	  m_height (0),
	  m_xoffset(0),
	  m_yoffset(0),
	  m_delete (1),
	  m_type   (0),
	  m_links  (0)
{
	for(int ch=0; ch<MXCHANNEL; ch++) {
		m_buf   [ch] = NULL;
		m_chtype[ch] = -1;
	}
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Image::Image:
//
// Constructor (from type list).
//! \brief	Constructor (from type list).
//! \details	Initialization of image components from type list.
//
inline
Image::Image(int w, int h, int *chtype)
	: m_width  (w),
	  m_height (h),
	  m_xoffset(0),
	  m_yoffset(0),
	  m_delete (1),
	  m_type   (0),
	  m_links  (0)
{
	// number of pixels in each channel
	int size = w * h;

	// create image channels
	int ch;
	for(ch=0; chtype[ch]>=0; ch++) {
		// allocate new channel of type t[ch] and save type info
		m_buf   [ch] = allocChannel(size, chtype[ch]);
		m_chtype[ch] = chtype[ch];
	}
	m_chtype[ch] = -1;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Image::~Image:
//
// Destructor. Free memory and reset members.
//! \brief	Destructor.
//! \details	Destructor. Free memory and reset members.
//
inline
Image::~Image()
{
	freeImage();
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Image::freeImage:
//
// Free memory and reset members.
//! \brief	Free memory.
//! \details	Free image channels if there are no other links to image.
//
inline void
Image::freeImage()
{
	// error checking
	if (m_links) return;

	// free image channels
	freeChannels(0);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Image::freeChannel:
//
// Free the buffer of channel ch and the channel pointer.
//! \brief     Free the buffer of channel c and the channel pointer.
//! \param[in] ch - Input channel.
//
inline void
Image::freeChannel(int ch)
{
	if(m_buf[ch] && m_buf[ch]->buf()) {
		m_buf[ch]->free();
		delete  m_buf[ch];
		m_buf[ch] = NULL;
	}
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Image::freeChannels:
//
// Free all image channels, starting from channel strt.
//! \brief	Free all image channels, starting from channel strt.
//! \param[in]  strt - Starting channel index.
//
inline void
Image::freeChannels(int strt)
{
	for(int i=strt; i<MXCHANNEL; i++) {
		if(m_buf[i] == NULL) continue;
		freeChannel(i);
		setChannel (i,  NULL);
		setChannelType(i, -1);
	}
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Image::allocImage:
//
// Allocate an image of width w, height h, and type chtype.
//! \brief	Allocate image memory.
//! \details	Allocate an image of width w, height h, and type chtype.
//! \param[in]	w,h - Image width and height.
//! \param[in]	chtype - Pointer to list of enumerated channel types.
//! \see	IP::channel_types
//
inline void
Image::allocImage(int w, int h, const int *chtype)
{
	m_width   = w;
	m_height  = h;
	m_xoffset = 0;
	m_yoffset = 0;
	m_delete  = 1;
	m_type    = 0;

	// number of pixels in each channel
	int size  = w * h;

	// create image channels
	int ch;
	for(ch=0; chtype[ch]>=0; ch++) {
		// allocate new channel of type t[ch] and save type info
		m_buf   [ch] = allocChannel(size, chtype[ch]);
		m_chtype[ch] = chtype[ch];
	}
	m_chtype[ch] = -1;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Image::allocImage_I:
//
// Allocate an image having the same properties as I.
//! \brief	Allocate image memory.
//! \details	Allocate an image having the same properties as I.
//! \param[in]	I - Copy properties of image I for allocated image.
//
inline void
Image::allocImage_I(Image &I)
{
	m_width   = I.width ();
	m_height  = I.height();
	m_xoffset = 0;
	m_yoffset = 0;
	m_delete  = 1;
	m_type    = 0;

	// number of pixels in each channel
	int size = m_width * m_height;

	// create image channels
	int  ch;
	int *chtype = I.channelTypes();
	for (ch=0; chtype[ch]>=0; ch++) {
		// allocate new channel of type t[ch] and save type info
		m_buf   [ch] = allocChannel(size, chtype[ch]);
		m_chtype[ch] = chtype[ch];
	}
	m_chtype[ch] = -1;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Image::allocChannel:
//
// Allocate channel buffer of specified size and datatype.
//! \brief	Allocate channel buffer of specified size and datatype.
//! \param[in]	size	- Buffer size.
//! \param[in]	type	- Channel datatype.
//
inline Channel *
Image::allocChannel(int size, int type)
{
	int	total = size;

	switch(type) {
	case  UCHAR_TYPE: total = size * sizeof(uchar ); break;
	case  SHORT_TYPE: total = size * sizeof(short ); break;
	case    INT_TYPE: total = size * sizeof(int   ); break;
	case   LONG_TYPE: total = size * sizeof(long  ); break;
	case  FLOAT_TYPE: total = size * sizeof(float ); break;
	case DOUBLE_TYPE: total = size * sizeof(double); break;
	};

	Channel *c = new Channel;
	c->resize(total);
	return c;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Image::initChannels:
//
// Init channel info in image: dimensions, buf, and chtype.
//! \brief		Init channel info in image: dimensions, buf, and chtype.
//! \param[in]	chtype - Array of channel types.
//
inline void
Image::initChannels(int *chtype)
{

	int ch;
	for(ch=0; ch<MXCHANNEL && chtype[ch]>=0; ch++) { // init buf, chtype
		replaceChannel(ch, width(), height(), chtype[ch]);
	}	   
	freeChannels(ch);	// free remainder
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Image::replaceChannel:
//
// Replace channel ch of image with new channel that holds w*h pixels
// of specified type. Free existing channel before replacement.
//! \brief	Replace channel ch of image I with new channel that
//!		holds w*h pixels of specified type.
//! \param[in]	ch	  - Input channel index.
//! \param[in]	w	  - Image width.
//! \param[in]	h	  - Image height.
//! \param[out]	type	  - Channel datatype.
//
inline void
Image::replaceChannel(int ch, int w, int h, int type)
{
	// replace channel ch of image I
	freeChannel(ch);				// delete channel
	setChannel(ch, allocChannel(w*h, type));	// alloc new channel

	// reset image properties
	setWidth (w);
	setHeight(h);
	setChannelType(ch, type);
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Image::maxChannel:
//
// Return the number of channels.
//! \brief	Return the number of channels.
//! \return	Number of channels.
//
inline int
Image::maxChannel()
{
	int ch;
	for(ch=0; ch<MXCHANNEL && channelType(ch)>=0; ch++) ;
	return ch;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Image::maxType:
//
// Return the maximum channel type in image.
//! \brief	Return the maximum channel type in image.
//! \return	Maximum channel type.
//
inline int
Image::maxType()
{
	int ch, t, tmax;

	for(ch=tmax=0; ch<MXCHANNEL && channelType(ch)>=0; ch++) {
		t = channelType(ch);
		if(t > tmax) tmax = t;
	}
	return tmax;
}





// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Image::link:
//
// Reference counting method.
//! \brief	Reference counting.
//! \details	Increment link and allow channel to be deleted, when links
//!		drop to 0, if flag is 1.
//!		Return the number of links to image.
//! \param[in]	flag - A boolean value.
//! \return	Integer.
//
inline int
Image::link(bool flag)
{
	setFreeFlag(flag);
	return  ++m_links;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Image::unlink:
//
// Reference counting method.
//! \brief	Reference counting.
//! \details	Decrement link and return the number of remaining links.
//! \return	Integer.
//
inline int
Image::unlink()
{
	if(m_links) --m_links;
	return        m_links;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Image::width:
//
// Image width.
//! \brief	Image width.
//! \details	Return image width.
//! \return	Integer.
//
inline int
Image::width() const
{
	return m_width;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Image::height:
//
// Image width.
//! \brief	Image height.
//! \details	Return image height.
//! \return	Integer.
//
inline int
Image::height() const
{
	return m_height;
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Image::xoffset:
//
// Horizontal offset into image.
//! \brief	Horizontal offset.
//! \details	Return horizontal offset into image from upper left corner.
//! \return	Integer.
//
inline int
Image::xoffset() const
{
	return m_xoffset;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Image::yoffset:
//
// Vertical offset into image.
//! \brief	Vertical offset.
//! \details	Return vertical offset into image from upper left corner.
//! \return	Integer.
//
inline int
Image::yoffset() const
{
	return m_yoffset;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Image::links:
//
// Number of other images linked to this image.
//! \brief	Number of other images linked to this image.
//! \details	Return the number of other images linked to this image.
//! \return	Integer.
//
inline int
Image::links() const
{
	return m_links;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Image::imageType:
//
// Type of image
//! \brief	Image type.
//! \details	Return the image type (an enumerated value).
//! \see	IP::image_types
//! \return	Integer.
//
inline int
Image::imageType() const
{
	return m_type;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Image::channelType:
//
// Type of channel.
//! \brief	Type of channel.
//! \details	Return the channel type (an enumerated value) for channel ch.
//! \param[in]	ch - Channel index.
//! \see	IP::channel_types.
//! \return	Integer.
//
inline int
Image::channelType(int ch) const
{
	return m_chtype[ch];
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Image::channelTypes:
//
// Channel type list.
//! \brief	Channel type list.
//! \details	Return the list of channel types (enumerated values).
//! \see	IP::channel_types
//! \return	List of integers.
//
inline int *
Image::channelTypes()
{
	return m_chtype;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Image::freeFlag:
//
// Flag that indicates whether channel can be deleted after links to image
// have dropped to 0.
//! \brief	Free flag.
//! \details	Return value of flag that indicates whether channel can
//!		be deleted after links to image have dropped to 0.
//! \return	Integer.
//
inline bool
Image::freeFlag() const
{
	return m_delete;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Image::setWidth:
//
// Set image width.
//! \brief	Set image width.
//! \details	Set image width.
//! \param[in]	w - Image width.
//
inline void
Image::setWidth(int w)
{
	m_width = w;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Image::setHeight:
//
// Set image height.
//! \brief	Set image height.
//! \details	Set image height.
//! \param[in]	h - Image height.
//
inline void
Image::setHeight(int h)
{
	m_height = h;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Image::setXOffset:
//
// Set horizontal offset into image.
//! \brief	Set horizontal offset into image.
//! \details	Set horizontal offset into image from upper left corner.
//! \param[in]	x - Offset value.
//
inline void
Image::setXOffset(int x)
{
	m_xoffset = x;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Image::setYOffset:
//
// Set vertical offset into image.
//! \brief	Set vertical offset into image.
//! \details	Set vertical offset into image from upper left corner.
//! \param[in]	y - Offset value.
//
inline void
Image::setYOffset(int y)
{
	m_yoffset = y;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Image::setImageType:
//
// Set type of image.
//! \brief	Set type of image.
//! \details	Set the image type to the enumerated value t.
//! \param[in]	t - Enumerated image type.
//! \see	IP::image_types
//
inline void
Image::setImageType(int t)
{
	m_type = t;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Image::setChannel:
//
// Assign channel pointer c to channel ch.
//! \brief	Set channel pointer.
//! \details	Assign channel pointer c to channel ch.
//! \warning	Clobbers the old buffer.
//! \param[in]	ch - Channel index.
//! \param[in]	c  - Channel pointer.
//
inline void
Image::setChannel(int ch, Channel *c)
{
	if (ch >= MXCHANNEL - 1)  // channel index is out of bounds
		return;

	freeChannel(ch);
	m_buf[ch] = c;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Image::setChannelType:
//
// Set the channel type of channel ch to the enumerated value t.
//! \brief	Set channel type.
//! \details	Set the channel type of channel ch to the enumerated value t.
//! \param[in]	ch - Channel index.
//! \param[in]	t  - Enumerated channel type.
//! \see	IP::channel_types
//
inline void
Image::setChannelType(int ch, int t)
{
	if (ch >= MXCHANNEL - 1)  // channel index is out of bounds
		return;

	m_chtype[ch] = t;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Image::setFreeFlag:
//
// Set the flag that indicates whether the channel can be deleted
// after links to image have dropped to 0.
//! \brief	Set free flag.
//! \details	Set the flag that indicates whether the channel can
//!		be deleted after links to image have dropped to 0.
//! \param[in]	flag - A boolean value assigned to free flag.
//
inline void
Image::setFreeFlag(bool flag)
{
	m_delete = flag;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Image::operator[]:
//
// Array index operator.
//! \brief	Array index operator.
//! \details	Return channel pointer of channel ch.
//! \return	Channel pointer.
inline const Channel *
Image::operator[](int ch) const
{
	return m_buf[ch];
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Image::operator[]:
//
// Array index operator.
//! \brief	Array index operator.
//! \details	Return channel pointer of channel ch.
//! \return	Channel pointer.
inline Channel *
Image::operator[](int ch)
{
	return m_buf[ch];
}


}	// namespace IP

#endif	// IMAGE_H
