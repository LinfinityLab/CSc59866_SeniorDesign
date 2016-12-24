// ======================================================================
// IMPROC: Image Processing Software Package
// Copyright (C) 2016 by George Wolberg
//
// IPtoUI.h - Header for IPtoUI.cpp file
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

//! \file	IPtoUI.h
//! \brief	Image Processing Package header file for IPtoUI.cpp
//! \author	George Wolberg, 2016

#ifndef IPTOUI_H
#define IPTOUI_H

namespace IP {

extern void	IP_QImageToIP(const QImage&, ImagePtr &);
extern void	IP_IPtoQImage(const ImagePtr&, QImage &);

}	// namespace IP

#endif	// IPTOUI_H
