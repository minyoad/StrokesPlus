//===========================================================================
// Copyright (c) 1996, Synaptics, Inc.
//
// This file is Confidential Synaptics Information.
//
// This file and all modified and derivative files thereof are
// Restricted Code for the Synaptics/Microsoft Driver Distribution Agreement.
//
// The Synaptics TouchPad and related software are protected by one or more
// of the U.S. patents whose numbers can be found in the PatentNumbers.h
// file in the lib directory of this source tree, as well as other U.S. and 
// International patents pending.
//
// RCS Header - Do not delete or modify.
//
// $RCSfile: SynError.h,v $
// $Source: /cvs/software/Driver/SynCom/SynCom/SynError.h,v $
// $Revision: 1.3 $
// $Date: 2003/02/19 18:33:48 $
// $Author: pausbeck $
// $Locker:  $
// $State: Exp $
//===========================================================================

//---------------------------------------------------------------------------
// SynError.h
// Possible Synaptics COM API return values.
//---------------------------------------------------------------------------

#ifndef __SYNERROR_H
#define __SYNERROR_H

#define SYN_OK ((HRESULT)0x00000000L)

#define SYN_FALSE ((HRESULT)0x00000000L)
#define SYN_TRUE ((HRESULT)0x00000001L)

#define SYNE_HANDLE E_HANDLE
#define SYNE_FAIL E_FAIL
#define SYNE_INVALIDARG E_INVALIDARG
#define SYNE_OUTOFMEMORY E_OUTOFMEMORY
#define SYNE_ACCESSDENIED E_ACCESSDENIED
#define SYNE_NOTIMPL E_NOTIMPL
#define SYNE_SEQUENCE E_UNEXPECTED

#define SYNE_NOTFOUND \
  MAKE_HRESULT(SEVERITY_ERROR, FACILITY_WIN32, ERROR_FILE_NOT_FOUND)

#endif