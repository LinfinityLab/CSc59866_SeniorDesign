// ======================================================================
// IMPROC: Image Processing Software Package
// Copyright (C) 2016 by George Wolberg
//
// ImagePtr.h - ImagePtr interface.
//
// Written by: George Wolberg, 2016
// ======================================================================

//! \file	ImagePtr.h
//! \brief	ImagePtr class.
//! \author	George Wolberg, 2016

#ifndef IMAGEPTR_H
#define IMAGEPTR_H

#include "Image.h"

namespace IP {

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ImagePtr class declaration
//! \brief      ImagePtr class.
//! \details	A smart pointer to the Image class.
//
class ImagePtr {
public:
	// constructors
	ImagePtr();			// default constructor
	ImagePtr(const ImagePtr &p);	// copy constructor
	ImagePtr(Image *p);		// construct from image ptr

	// destructor
	~ImagePtr();

	// null pointer / null image flag
	bool	 isNull	() const;

	// overloaded operators
	ImagePtr &operator=(const ImagePtr &p);	// assignment
	ImagePtr &operator=(Image *);		// assignment
	Image	 *operator->() const;		// dereference
	Image	 &operator* () const;		// value-at
	ImagePtr &operator++();			// prefix  increment
	ImagePtr  operator++(int);		// postfix increment
	ImagePtr &operator--();			// prefix  decrement
	ImagePtr  operator--(int);		// postfix decrement
	bool	  operator!() const;		// not
	const Channel *operator[](int i) const;	// index into buffer
	      Channel *operator[](int i);

	// equality operator
	friend bool operator==(const ImagePtr &I1, const ImagePtr &I2);

	// not-equals operator
	friend bool operator!=(const ImagePtr &I1, const ImagePtr &I2);

	// less-than operator
	friend bool operator<(const ImagePtr &I1,  const ImagePtr &I2);

	// greater-than operator
	friend bool operator>(const ImagePtr &I1,  const ImagePtr &I2);

	// pointer addition operator
	friend ImagePtr operator+(const ImagePtr &I1, int k);

	// pointer subtraction operator
	friend ImagePtr operator-(const ImagePtr &I1, int k);

private:
	Image	*m_ptr;

	// free pointer and associated image
	void	_freePtr();
};



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ImagePtr::ImagePtr:
//
// Default constructor.
//! \brief	Default constructor.
//! \details	Allocate an image.
//
inline
ImagePtr::ImagePtr()
{
	m_ptr = new Image;
	if(m_ptr) {
		int t[] = {-1};
		m_ptr->allocImage(0, 0, t);
		m_ptr->link();
	}
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ImagePtr::ImagePtr:
//
// Copy constructor (from ImagePtr).
//! \brief	Copy constructor (from ImagePtr).
//! \details	Set image pointer to the pointer associated with \a p.
//
inline
ImagePtr::ImagePtr(const ImagePtr &p) : m_ptr(p.m_ptr)
{
	if(m_ptr) m_ptr->link();
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ImagePtr::ImagePtr:
//
// Copy constructor (from Image pointer).
//! \brief	Copy constructor (from Image pointer).
//! \details	Set image pointer to the pointer associated with \a p.
//
inline
ImagePtr::ImagePtr(Image *p) : m_ptr(p)
{
	if(m_ptr) m_ptr->link();
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ImagePtr::~ImagePtr:
//
// Destructor.
//! \brief	Destructor.
//! \details	Free ImagePtr.
//
inline
ImagePtr::~ImagePtr()
{
	_freePtr();
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ImagePtr::_freePtr:
//
// Free the image pointed to by m_ptr..
//! \brief	Free the image pointed to by m_ptr.
//! \details	Decrement the reference count, deleting the image
//!		if the count reaches 0.
//
inline void
ImagePtr::_freePtr()
{
	if(!m_ptr) return;

	int link =  m_ptr->unlink();
	if(!link) {
		if(m_ptr->freeFlag()) {
			m_ptr->freeImage();		// free channels
			delete (Image *) m_ptr;
			return;
		} else m_ptr->setFreeFlag(true);	// reset flag
	}
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ImagePtr::isNull:
//
// Test if the image pointer is null.
//! \brief	Test if the image pointer is null.
//! \details	If the image pointer is null, or if the image has
//!		dimensions 0x0, return true, otherwise return false.
//! \return	A boolean value.
//
inline bool
ImagePtr::isNull() const
{
	// ptr is also considered null if existing image has dimensions 0x0
	return ((m_ptr == NULL) || (!m_ptr->width() && !m_ptr->height()));
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ImagePtr::operator=:
//
// Assignment operator (from ImagePtr).
//! \brief	Assignment operator (from ImagePtr).
//! \details	Copy image pointer associated with \a p.
//! \param[in]	p - An ImagePtr object.
//! \return	A self-reference. 
//
inline ImagePtr &
ImagePtr::operator=(const ImagePtr &p)
{
	if(m_ptr) {
		int  links = m_ptr->unlink();
		if (!links) {
			m_ptr->freeImage();
			delete (Image *) m_ptr;
		}
	}
	m_ptr = p.m_ptr;
	if(m_ptr) m_ptr->link();
	return *this;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ImagePtr::operator=:
//
// Assignment operator (from Image pointer).
//! \brief	Assignment operator (from Image pointer).
//! \details	Copy image pointer associated with \a p.
//! \param[in]	p - An pointer to an Image object.
//! \return	A self-reference. 
//
inline ImagePtr &
ImagePtr::operator=(Image *p)
{
	if(m_ptr) {
		int  links = m_ptr->unlink();
		if (!links) {
			m_ptr->freeImage();
			delete (Image *) m_ptr;
		}
	}
	m_ptr = p;
	if(m_ptr) m_ptr->link();
	return *this;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ImagePtr::operator->:
//
// Member access operator.
//! \brief	Member access operator.
//! \details	Return a member of the Image class.
//! \return	A pointer to Image.
//
inline Image *
ImagePtr::operator->() const
{
	return  m_ptr;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ImagePtr::operator*:
//
// Dereference operator.
//! \brief	Dereference operator.
//! \details	Return the Image pointed to by this object.
//! \return	A reference to Image.
//
inline Image &
ImagePtr::operator*() const
{
	return *m_ptr;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ImagePtr::operator++:
//
// Prefix increment operator.
//! \brief	Prefix increment operator.
//! \details	Increment the Image pointer.
//! \return	A self-reference.
//
inline ImagePtr &
ImagePtr::operator++()
{
     ++m_ptr;
     return *this;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ImagePtr::operator++:
//
// Postfix increment operator.
//! \brief	Postfix increment operator.
//! \details	Increment the Image pointer.
//! \return	A copy of the old ImagePtr object.
//
inline ImagePtr
ImagePtr::operator++(int)
{
	ImagePtr tmp = *this;
	++*this;
	return tmp;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ImagePtr::operator--:
//
// Prefix decrement operator.
//! \brief	Prefix decrement operator.
//! \details	Decrement the Image pointer.
//! \return	A self-reference.
//
inline ImagePtr &
ImagePtr::operator--()
{
	--m_ptr;
	return *this;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ImagePtr::operator--:
//
// Postfix decrement operator.
//! \brief	Postfix decrement operator.
//! \details	Decrement the Image pointer.
//! \return	A copy of the old ImagePtr object.
//
inline ImagePtr
ImagePtr::operator--(int)
{
	ImagePtr tmp = *this;
	--*this;
	return tmp;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ImagePtr::operator!:
//
// Negation operator.
//! \brief	Negation operator.
//! \details	Return true if the pointer is null, otherwise return false.
//! \return	A boolean value.
//
inline bool
ImagePtr::operator!() const
{
	return(m_ptr == NULL);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ImagePtr::operator[]:
//
// Array index operator.
//! \brief	Array index operator.
//! \details	Return the <i>ch</i>'th channel.
//! \return	A pointer to Channel.
//
inline const Channel *
ImagePtr::operator[](int ch) const
{
	if(ch < 0 || ch > MXCHANNEL || m_ptr->channelType(ch) < 0)
		return (Channel *) 0;
	return (*m_ptr)[ch];
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ImagePtr::operator[]:
//
// Array index operator.
//! \brief	Array index operator.
//! \details	Return the <i>ch</i>'th channel.
//! \return	A pointer to Channel.
//
inline Channel *
ImagePtr::operator[](int ch)
{
	if(ch < 0 || ch > MXCHANNEL || m_ptr->channelType(ch) < 0)
		return (Channel *) 0;
	return (*m_ptr)[ch];
}

//! \addtogroup image
//@{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// operator==:
//
// Equality operator.
//! \brief	Equality operator.
//! \details	Return true if I1 and I2 point to the same image;
//!		otherwise, return false.
//! \return	A boolean value.
//
inline bool
operator==(const ImagePtr &I1, const ImagePtr &I2)
{
	return(I1.m_ptr == I2.m_ptr);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// operator!=:
//
// Inequality operator.
//! \brief	Equality operator.
//! \details	Return true if I1 and I2 do not point to the same image;
//!		otherwise, return false.
//! \return	A boolean value.
//
inline bool
operator!=(const ImagePtr &I1, const ImagePtr &I2)
{
	return(I1.m_ptr != I2.m_ptr);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// operator<:
//
// Less-than operator.
//! \brief	Less-than operator.
//! \details	Check if image \a I1's pointer is less than that of \a I2.
//! \return	A boolean value.
//
inline bool
operator<(const ImagePtr &I1, const ImagePtr &I2)
{
	return(I1.m_ptr < I2.m_ptr);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// operator>:
//
// Greater-than operator.
//! \brief	Greater-than operator.
//! \details	Check if image \a I1's pointer is greater than that of \a I2.
//! \param[in]	I1,I2 - ImagePtr objects.
//! \return	A boolean value.
//
inline bool
operator>(const ImagePtr &I1, const ImagePtr &I2)
{
	return(I1.m_ptr > I2.m_ptr);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// operator+:
//
// Pointer addition operator.
//! \brief	Pointer addition operator.
//! \details	Return \a I1 + \a k. Does not affect \a p1.
//! \param[in]	I1 - ImagePtr object.
//! \param[in]	k - Integer offset.
//! \return	ImagePtr.
//
inline ImagePtr
operator+(const ImagePtr &I1, int k)
{
	ImagePtr I2 = I1;
	I2.m_ptr   += k;
	return I2;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// operator-:
//
//  Pointer subtraction operator.
//! \brief	Pointer subtraction operator.
//! \details	Return \a I1 - \a k. Does not affect \a I1.
//! \param[in]	I1 - ImagePtr.
//! \param[in]	k - Integer offset.
//! \return	ImagePtr.
//
inline ImagePtr
operator-(const ImagePtr &I1, int k)
{
	ImagePtr I2 = I1;
	I2.m_ptr   -= k;
	return I2;
}
//@}

}	// namespace IP

#endif	// IMAGEPTR_H
