//
//   Copyright (C) 2007 by sinamas <sinamas at users.sourceforge.net>
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License version 2 as
//   published by the Free Software Foundation.
//
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License version 2 for more details.
//
//   You should have received a copy of the GNU General Public License
//   version 2 along with this program; if not, write to the
//   Free Software Foundation, Inc.,
//   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//

#ifndef SERIAL_IO_H
#define SERIAL_IO_H

namespace gambatte {

class SerialIO
{
	public:
		virtual ~SerialIO() {};

		virtual bool check(unsigned char out, unsigned char& in, bool& fastCgb) = 0;
		virtual unsigned char send(unsigned char data, bool fastCgb) = 0;
};

}

#endif
