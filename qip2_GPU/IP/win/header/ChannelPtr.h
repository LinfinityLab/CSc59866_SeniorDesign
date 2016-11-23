// ======================================================================
// IMPROC: Image Processing Software Package
// Copyright (C) 2016 by George Wolberg
//
// ChannelPtr.h - Channel pointer class.
//
// Written by: George Wolberg, 2016
// ======================================================================

//! \file	ChannelPtr.h
//! \brief	ChannelPtr class.
//! \author	George Wolberg, 2016

#ifndef CHANNELPTR_H
#define CHANNELPTR_H

#include "Channel.h"

namespace IP {

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ChannelPtr class declaration
//! \brief      ChannelPtr class.
//! \details	A smart pointer to the Channel class.
//
template<class T>
class ChannelPtr {
public:
	// constructors
	ChannelPtr();				// default constructor
	ChannelPtr(const ChannelPtr &);		// copy constructor
	ChannelPtr(Channel *c);			// construct from channel ptr

	// destructor
	~ChannelPtr();

	// get a pointer to the channel buffer
	const T *buf() const;
	T       *buf();

	// overloaded operators
	ChannelPtr &operator=(const ChannelPtr &); // assignment
	T	   &operator* ()    const;	// value-at
	T	   &operator[](int) const;	// index into buffer
	ChannelPtr &operator++();		// prefix  increment
	ChannelPtr  operator++(int);		// postfix increment
	ChannelPtr &operator--();		// prefix  decrement
	ChannelPtr  operator--(int);		// postfix decrement
	ChannelPtr &operator+=(int);		// addition
	ChannelPtr &operator-=(int);		// subtraction

private:
	T         *m_buf;			// channel buffer ptr
};

}	// namespace IP

#include "ChannelPtr.tpp"

#endif	// CHANNELPTR_H
