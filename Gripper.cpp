// Copyright (C) 2012-Present Andrew S. Bantly
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

#include "stdafx.h"
#include "Gripper.h"

/////////////////////////////////////////////////////////////////////////////
// CGripper
CGripper::CGripper()
{
}

CGripper::~CGripper()
{
}

BEGIN_MESSAGE_MAP(CGripper, CScrollBar)
	//{{AFX_MSG_MAP(CGripper)
	ON_WM_NCHITTEST()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGripper message handlers

LRESULT CGripper::OnNcHitTest(CPoint point) 
{
	LRESULT ht = CScrollBar::OnNcHitTest(point);
	if (ht == HTCLIENT)
	{
		CRect r;
		GetClientRect(r);
		ClientToScreen(r);
		point.x -= r.left;
		point.y -= r.top;
		if ((point.x + point.y)>= r.Width())
			ht = HTBOTTOMRIGHT;
	}
	return ht;
}
