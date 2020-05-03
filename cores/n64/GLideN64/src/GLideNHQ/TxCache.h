/*
 * Texture Filtering
 * Version:  1.0
 *
 * Copyright (C) 2007  Hiroshi Morii   All Rights Reserved.
 * Email koolsmoky(at)users.sourceforge.net
 * Web   http://www.3dfxzone.it/koolsmoky
 *
 * this is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * this is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GNU Make; see the file COPYING.  If not, write to
 * the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef __TXCACHE_H__
#define __TXCACHE_H__

#include <list>
#include <map>
#include <memory>

#include "TxInternal.h"
#include "TxUtil.h"

struct  Checksum
{
	union
	{
		uint64 _checksum; /* checksum hi:palette low:texture */
		struct
		{
			uint32 _low;
			uint32 _hi;
		};
	};

	Checksum(uint64 checksum) : _checksum(checksum) {}
	operator bool() {
		return _checksum != 0;
	}
	operator uint64() {
		return _checksum;
	}
};

class TxCacheImpl;

class TxCache
{
private:
	std::unique_ptr<TxCacheImpl> _pImpl;

protected:
	tx_wstring _ident;
	tx_wstring _cachePath;
	dispInfoFuncExt _callback;

	bool save();
	bool load(bool force);
	bool del(Checksum checksum); /* checksum hi:palette low:texture */
	bool isCached(Checksum checksum); /* checksum hi:palette low:texture */
	void clear();
	uint64 size() const; // number of elements
	uint64 totalSize() const; // size of elements in bytes
	uint64 cacheLimit() const;
	uint32 getOptions() const;
	void setOptions(uint32 options);

	virtual tx_wstring _getFileName() const = 0;
	virtual int _getConfig() const = 0;

public:
	virtual ~TxCache();
	TxCache(uint32 options, uint64 cacheLimit, const wchar_t *cachePath, const wchar_t *ident, dispInfoFuncExt callback);
	bool add(Checksum checksum, GHQTexInfo *info, int dataSize = 0);
	bool get(Checksum checksum, GHQTexInfo *info);
	bool empty() const;
};

#endif /* __TXCACHE_H__ */
