// ======================================================================
// IMPROC: Image Processing Software Package
// Copyright (C) 2016 by George Wolberg
//
// Channel.h - Channel class.
//
// Written by: George Wolberg, 2016
// ======================================================================

//! \file	Channel.h
//! \brief	Channel class.
//! \author	George Wolberg, 2016

#ifndef CHANNEL_H
#define CHANNEL_H

#include "IPdefs.h"

namespace IP {

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Channel class declaration
//! \brief      Channel class.
//! \details	The Channel class handles image channel data of various
//!		types.
//
class Channel {
public:
	// constructors
	Channel();			// default constructor
	Channel(const Channel &);	// copy constructor
	Channel(int);			// construct to specified # of bytes

	// destructor
	~Channel();

	// get methods
	const void *buf() const;	// buffer pointer
	void       *buf();		// buffer pointer

	// resize channel buffer
	void	resize(int);		// resize to specified # of bytes

	// free channel
	void	free();

private:
	void	*m_buf;			// channel buffer pointer
};



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Channel::Channel:
//
// Default constructor. A null pointer is assigned to the channel.
//! \brief	Default constructor.
//! \details	Assign a null pointer to the channel.
//
inline
Channel::Channel()
	: m_buf(0)
{}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Channel::Channel:
//
// Size constructor. Allocate specified number of bytes to channel.
//! \brief	Size constructor.
//! \details	Allocate specified number of bytes to channel.
//
inline
Channel::Channel(int size)
{
	m_buf = new uchar[size];
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Channel::~Channel:
//
// Destructor function. Free channel memory and set channel pointer to null.
//! \brief	Destructor.
//! \details	Free channel memory and set channel pointer to null.
//
inline
Channel::~Channel()
{
	if (m_buf) delete [] (uchar *) m_buf;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Channel::buf:
//
// Channel memory pointer.
//! \brief	Channel memory pointer.
//! \details	Return channel memory  pointer.
//! \return	Pointer to data of type void (const version).
//
inline const void *
Channel::buf() const
{
	return m_buf;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Channel::buf:
//
// Channel pointer.
//! \brief	Channel memory pointer.
//! \details	Return channel memory  pointer.
//! \return	Pointer to data of type void.
//
inline void *
Channel::buf()
{
	return m_buf;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Channel::resize:
//
// Resize channel buffer to have size bytes.
//! \brief	Resize channel buffer.
//! \details	Resize channel buffer to \a size bytes. Reset channel pointer.
//
inline void
Channel::resize(int size)
{
	m_buf = new uchar[size];
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Channel::free:
//
// Free channel buffer.
//! \brief	Free channel buffer.
//! \details	Free channel memory and reset channel pointer to null.
//
inline void
Channel::free() {
	if(m_buf) {
		delete [] (uchar*) m_buf;
		m_buf = 0;
	}
}

}	// namespace IP

#endif	// CHANNEL_H
