// ======================================================================
// IMPROC: Image Processing Software Package
// Copyright (C) 2016 by George Wolberg
//
// ChannelPtr.tpp - Template functions for channel pointer class.
//
// Written by: George Wolberg, 2016
// ======================================================================

//! \file	ChannelPtr.tpp
//! \brief	ChannelPtr implementation.
//! \author	George Wolberg, 2016

namespace IP {

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ChannelPtr::ChannelPtr:
//
// Default constructor.
//! \brief	Default constructor.
//! \details	Set channel pointer to null.
//
template<class T>
ChannelPtr<T>::ChannelPtr()
	: m_buf(0)
{}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ChannelPtr::ChannelPtr:
//
// Copy constructor (from ChannelPtr).
//! \brief	Copy constructor (from ChannelPtr).
//! \details	Set channel pointer to the pointer associated with \a p.
//
template<class T>
ChannelPtr<T>::ChannelPtr(const ChannelPtr &p)
	: m_buf(p.m_buf)
{}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ChannelPtr::ChannelPtr:
//
// Copy constructor function (from Channel pointer).
//! \brief	Copy constructor (from Channel pointer).
//! \details	Set channel pointer to the pointer associated with \a c.
//
template<class T>
ChannelPtr<T>::ChannelPtr(Channel *c)
	: m_buf(0)
{
	if(c) m_buf = static_cast<T *>(c->buf());
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ChannelPtr::~ChannelPtr:
//
// Destructor.
//! \brief	Destructor.
//! \details	Free ChannelPtr.
//
template<class T>
ChannelPtr<T>::~ChannelPtr()
{}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ChannelPtr::buf:
//
// Pointer to Channel memory.
//! \brief	Pointer to Channel memory.
//! \details	Return channel memory pointer.
//! \return	Pointer to data of type \a T (const version).
//
template<class T>
const T *
ChannelPtr<T>::buf() const
{
	return m_buf;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ChannelPtr::buf:
//
// Pointer to Channel memory.
//! \brief	Pointer to Channel memory.
//! \details	Return channel memory pointer.
//! \return	Pointer to data of type \a T.
//
template<class T>
T *
ChannelPtr<T>::buf()
{
	return m_buf;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ChannelPtr::operator=:
//
// Assignment operator.
//! \brief	Assignment operator.
//! \details	copy channel memory pointer associated with \a p.
//! \param[in]	p - A channel pointer.
//! \return	A self-reference.
//
template<class T>
inline ChannelPtr<T> &
ChannelPtr<T>::operator=(const ChannelPtr<T> &p)
{
	m_buf = const_cast<T *>(p.buf());
	return *this;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Pointer operators: *, ++, --, +=, --, <, >, +, -
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ChannelPtr::operator*:
//
// Value-at operator.
//! \brief	Value-at operator.
//! \details	Return data pointed to by pointer.
//
template<class T>
inline T &
ChannelPtr<T>::operator*() const
{
	return *m_buf;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ChannelPtr::operator++:
//
// Prefix increment operator.
//! \brief	Prefix increment operator.
//! \details	Increment the Channel memory pointer.
//! \return	A self-reference.
//
template<class T>
inline ChannelPtr<T> &
ChannelPtr<T>::operator++()
{
	++m_buf;
	return *this;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ChannelPtr::operator++:
//
// Postfix increment operator.
//! \brief	Postfix increment operator.
//! \details	Increment the Channel memory pointer.
//! \return	A self-reference.
//
template<class T>
inline ChannelPtr<T>
ChannelPtr<T>::operator++(int)
{
	ChannelPtr<T> tmp = *this;
	++*this;
	return tmp;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ChannelPtr::operator--:
//
// Prefix decrement operator.
//! \brief	Prefix decrement operator.
//! \details	Decrement the Channel memory pointer.
//! \return	A self-reference.
//
template<class T>
inline ChannelPtr<T> &
ChannelPtr<T>::operator--()
{
	--m_buf;
	return *this;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ChannelPtr::operator--:
//
// Postfix decrement operator.
//! \brief	Postfix decrement operator.
//! \details	Decrement the Channel memory pointer
//! \return	A self-reference.
//
template<class T>
inline ChannelPtr<T>
ChannelPtr<T>::operator--(int)
{
	ChannelPtr<T> tmp = *this;
	--*this;
	return tmp;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ChannelPtr::operator+=:
//
// Addition assignment operator.
//! \brief	Addition assignment operator.
//! \details	\a buf = \a buf + \a k
//! \param[in]	k - Integer offset.
//! \return	A self-reference.
//
template<class T>
inline ChannelPtr<T> &
ChannelPtr<T>::operator+=(int k)
{
	m_buf += k;
	return *this;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ChannelPtr::operator-=:
//
// Subtraction assignment operator.
//! \brief	Subtraction assignment operator.
//! \details	\a buf = \a buf - \a k
//! \param[in]	k - Integer offset.
//! \return	A self-reference.
//
template<class T>
inline ChannelPtr<T> &
ChannelPtr<T>::operator-=(int k)
{
	m_buf -= k;
	return *this;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ChannelPtr::operator[]:
//
// Array index operator.
//! \brief	Array index operator.
//! \details	Return element \a i. No error checking.
//! \return	Element of type T.
//
template<class T>
inline T &
ChannelPtr<T>::operator[](int i) const
{
	return m_buf[i];
}

//! \addtogroup image
//@{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// operator<:
//
// Less-than operator.
//! \brief	Less-than operator.
//! \details	Check if channel \a c1's pointer is less than that of \a c2.
//! \return	A boolean value.
//
template<class T>
inline bool
operator<(const ChannelPtr<T> &c1, const ChannelPtr<T> &c2)
{
	return(c1.buf() < c2.buf());
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// operator>:
//
// Greater-than operator.
//! \brief	Greater-than operator.
//! \details	Check if channel \a c1's pointer is greater than that of \a c2.
//! \return	A boolean value.
//
template<class T>
inline bool
operator>(const ChannelPtr<T> &c1, const ChannelPtr<T> &c2)
{
	return(c1.buf() > c2.buf());
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// operator+:
//
// Pointer addition operator.
//! \brief	Pointer addition operator.
//! \details	Return \a p1 + \a k. Does not affect \a p1.
//! \param[in]	p1 - ChannelPtr.
//! \param[in]	k - Integer offset.
//! \return	ChannelPtr.
//
template<class T>
inline ChannelPtr<T>
operator+(const ChannelPtr<T> &p1, int k)
{
	ChannelPtr<T> p2 = p1;
	p2 += k;
	return p2;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// operator-:
//
// Binary subtraction operator.
//! \brief	Binary subtraction operator.
//! \details	Return difference between pointers \a p1 and \a p2.
//! \param[in]	p1,p2 - ChannelPtr.
//! \return	Integer.
//
template<class T>
inline int
operator-(const ChannelPtr<T> &p1, const ChannelPtr<T> &p2)
{
	return p1.buf() - p2.buf();
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// operator-:
//
//  Pointer subtraction operator.
//! \brief	Pointer subtraction operator.
//! \details	Return \a p1 - \a k. Does not affect \a p1.
//! \param[in]	p1 - ChannelPtr.
//! \param[in]	k - Integer offset.
//! \return	ChannelPtr.
//
template<class T>
inline ChannelPtr<T>
operator-(const ChannelPtr<T> &p1, int k)
{
	ChannelPtr<T> p2 = p1;
	p2 -= k;
	return p2;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// operator==:
//
// Equality operator.
//! \brief	Equality operator.
//! \details	Compare the equality of pointers from channels \a c1 and \a c2.
//! \return	A boolean value.
//
template<class T>
inline bool
operator==(const ChannelPtr<T> &c1, const ChannelPtr<T> &c2)
{
	return(c1.buf() == c2.buf());
}




// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// operator!=:
//
// Inequality operator.
//! \brief	Inequality operator.
//! \details	Compare the inequality of ptrs from channels \a c1 and \a c2.
//! \return	A boolean value.
//
template<class T>
inline bool
operator!=(const ChannelPtr<T> &c1, const ChannelPtr<T> &c2)
{
	return(c1.buf() != c2.buf());
}
//@}

}	// namespace IP
