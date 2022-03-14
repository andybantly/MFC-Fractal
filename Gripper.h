// Copyright (C) 2012-2016 Andrew S. Bantly
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; version 2 of the License.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
//

#pragma once

class CGripper : public CScrollBar
{
// Construction
public:
	CGripper();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGripper)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGripper();

	// Generated message map functions
protected:
	//{{AFX_MSG(CGripper)
	afx_msg LRESULT OnNcHitTest(CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};
