// istream classes -*- C++ -*-

// Copyright (C) 1997, 1998, 1999, 2000, 2001, 2002, 2003, 2004, 2005
// Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 2, or (at your option)
// any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License along
// with this library; see the file COPYING.  If not, write to the Free
// Software Foundation, 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,
// USA.

// As a special exception, you may use this file as part of a free software
// library without restriction.  Specifically, if other files instantiate
// templates or use macros or inline functions from this file, or you compile
// this file and link it with other files to produce an executable, this
// file does not by itself cause the resulting executable to be covered by
// the GNU General Public License.  This exception does not however
// invalidate any other reasons why the executable file might be covered by
// the GNU General Public License.

/** @file istream.tcc
 *  This is an internal header file, included by other library headers.
 *  You should not attempt to use it directly.
 */

//
// ISO C++ 14882: 27.6.1  Input streams
//

#ifndef _ISTREAM_TCC
#define _ISTREAM_TCC 1

#pragma GCC system_header

#include <locale>
#include <ostream> // For flush()

_GLIBCXX_BEGIN_NAMESPACE(std)

  template<typename _CharT, typename _Traits>
    basic_istream<_CharT, _Traits>::sentry::
    sentry(basic_istream<_CharT, _Traits>& __in, bool __noskip) : _M_ok(false)
    {
      ios_base::iostate __err = ios_base::iostate(ios_base::goodbit);
      if (__in.good())
	{
	  if (__in.tie())
	    __in.tie()->flush();
	  if (!__noskip && (__in.flags() & ios_base::skipws))
	    {
	      const __int_type __eof = traits_type::eof();
	      __streambuf_type* __sb = __in.rdbuf();
	      __int_type __c = __sb->sgetc();

	      const __ctype_type& __ct = __check_facet(__in._M_ctype);
	      while (!traits_type::eq_int_type(__c, __eof)
		     && __ct.is(ctype_base::space, 
				traits_type::to_char_type(__c)))
		__c = __sb->snextc();

	      // _GLIBCXX_RESOLVE_LIB_DEFECTS
	      // 195. Should basic_istream::sentry's constructor ever
	      // set eofbit?
	      if (traits_type::eq_int_type(__c, __eof))
		__err |= ios_base::eofbit;
	    }
	}

      if (__in.good() && __err == ios_base::goodbit)
	_M_ok = true;
      else
	{
	  __err |= ios_base::failbit;
	  __in.setstate(__err);
	}
    }

  template<typename _CharT, typename _Traits>
    basic_istream<_CharT, _Traits>&
    basic_istream<_CharT, _Traits>::
    operator>>(__istream_type& (*__pf)(__istream_type&))
    { return __pf(*this); }

  template<typename _CharT, typename _Traits>
    basic_istream<_CharT, _Traits>&
    basic_istream<_CharT, _Traits>::
    operator>>(__ios_type& (*__pf)(__ios_type&))
    {
      __pf(*this);
      return *this;
    }

  template<typename _CharT, typename _Traits>
    basic_istream<_CharT, _Traits>&
    basic_istream<_CharT, _Traits>::
    operator>>(ios_base& (*__pf)(ios_base&))
    {
      __pf(*this);
      return *this;
    }

  template<typename _CharT, typename _Traits>
    template<typename _ValueT>
      basic_istream<_CharT, _Traits>&
      basic_istream<_CharT, _Traits>::
      _M_extract(_ValueT& __v)
      {
	sentry __cerb(*this, false);
	if (__cerb)
	  {
	    ios_base::iostate __err = ios_base::iostate(ios_base::goodbit);
	    try
	      {
		const __num_get_type& __ng = __check_facet(this->_M_num_get);
		__ng.get(*this, 0, *this, __err, __v);
	      }
	    catch(...)
	      { this->_M_setstate(ios_base::badbit); }
	    if (__err)
	      this->setstate(__err);
	  }
	return *this;
      }

  template<typename _CharT, typename _Traits>
    basic_istream<_CharT, _Traits>&
    basic_istream<_CharT, _Traits>::
    operator>>(__streambuf_type* __sbout)
    {
      ios_base::iostate __err = ios_base::iostate(ios_base::goodbit);
      sentry __cerb(*this, false);
      if (__cerb && __sbout)
	{
	  try
	    {
	      if (!__copy_streambufs(this->rdbuf(), __sbout))
		__err |= ios_base::failbit;
	    }
	  catch(...)
	    { this->_M_setstate(ios_base::failbit); }
	}
      else if (!__sbout)
	__err |= ios_base::failbit;
      if (__err)
	this->setstate(__err);
      return *this;
    }

  template<typename _CharT, typename _Traits>
    typename basic_istream<_CharT, _Traits>::int_type
    basic_istream<_CharT, _Traits>::
    get(void)
    {
      const int_type __eof = traits_type::eof();
      int_type __c = __eof;
      _M_gcount = 0;
      ios_base::iostate __err = ios_base::iostate(ios_base::goodbit);
      sentry __cerb(*this, true);
      if (__cerb)
	{
	  try
	    {
	      __c = this->rdbuf()->sbumpc();
	      // 27.6.1.1 paragraph 3
	      if (!traits_type::eq_int_type(__c, __eof))
		_M_gcount = 1;
	      else
		__err |= ios_base::eofbit;
	    }
	  catch(...)
	    { this->_M_setstate(ios_base::badbit); }
	}
      if (!_M_gcount)
	__err |= ios_base::failbit;
      if (__err)
	this->setstate(__err);
      return __c;
    }

  template<typename _CharT, typename _Traits>
    basic_istream<_CharT, _Traits>&
    basic_istream<_CharT, _Traits>::
    get(char_type& __c)
    {
      _M_gcount = 0;
      ios_base::iostate __err = ios_base::iostate(ios_base::goodbit);
      sentry __cerb(*this, true);
      if (__cerb)
	{
	  try
	    {
	      const int_type __cb = this->rdbuf()->sbumpc();
	      // 27.6.1.1 paragraph 3
	      if (!traits_type::eq_int_type(__cb, traits_type::eof()))
		{
		  _M_gcount = 1;
		  __c = traits_type::to_char_type(__cb);
		}
	      else
		__err |= ios_base::eofbit;
	    }
	  catch(...)
	    { this->_M_setstate(ios_base::badbit); }
	}
      if (!_M_gcount)
	__err |= ios_base::failbit;
      if (__err)
	this->setstate(__err);
      return *this;
    }

  template<typename _CharT, typename _Traits>
    basic_istream<_CharT, _Traits>&
    basic_istream<_CharT, _Traits>::
    get(char_type* __s, streamsize __n, char_type __delim)
    {
      _M_gcount = 0;
      ios_base::iostate __err = ios_base::iostate(ios_base::goodbit);
      sentry __cerb(*this, true);
      if (__cerb)
	{
	  try
	    {
	      const int_type __idelim = traits_type::to_int_type(__delim);
	      const int_type __eof = traits_type::eof();
	      __streambuf_type* __sb = this->rdbuf();
	      int_type __c = __sb->sgetc();

	      while (_M_gcount + 1 < __n
		     && !traits_type::eq_int_type(__c, __eof)
		     && !traits_type::eq_int_type(__c, __idelim))
		{
		  *__s++ = traits_type::to_char_type(__c);
		  ++_M_gcount;
		  __c = __sb->snextc();
		}
	      if (traits_type::eq_int_type(__c, __eof))
		__err |= ios_base::eofbit;
	    }
	  catch(...)
	    { this->_M_setstate(ios_base::badbit); }
	}
      // _GLIBCXX_RESOLVE_LIB_DEFECTS
      // 243. get and getline when sentry reports failure.
      if (__n > 0)
	*__s = char_type();
      if (!_M_gcount)
	__err |= ios_base::failbit;
      if (__err)
	this->setstate(__err);
      return *this;
    }

  template<typename _CharT, typename _Traits>
    basic_istream<_CharT, _Traits>&
    basic_istream<_CharT, _Traits>::
    get(__streambuf_type& __sb, char_type __delim)
    {
      _M_gcount = 0;
      ios_base::iostate __err = ios_base::iostate(ios_base::goodbit);
      sentry __cerb(*this, true);
      if (__cerb)
	{
	  try
	    {
	      const int_type __idelim = traits_type::to_int_type(__delim);
	      const int_type __eof = traits_type::eof();
	      __streambuf_type* __this_sb = this->rdbuf();
	      int_type __c = __this_sb->sgetc();
	      char_type __c2 = traits_type::to_char_type(__c);

	      while (!traits_type::eq_int_type(__c, __eof)
		     && !traits_type::eq_int_type(__c, __idelim)
		     && !traits_type::eq_int_type(__sb.sputc(__c2), __eof))
		{
		  ++_M_gcount;
		  __c = __this_sb->snextc();
		  __c2 = traits_type::to_char_type(__c);
		}
	      if (traits_type::eq_int_type(__c, __eof))
		__err |= ios_base::eofbit;
	    }
	  catch(...)
	    { this->_M_setstate(ios_base::badbit); }
	}
      if (!_M_gcount)
	__err |= ios_base::failbit;
      if (__err)
	this->setstate(__err);
      return *this;
    }

  template<typename _CharT, typename _Traits>
    basic_istream<_CharT, _Traits>&
    basic_istream<_CharT, _Traits>::
    getline(char_type* __s, streamsize __n, char_type __delim)
    {
      _M_gcount = 0;
      ios_base::iostate __err = ios_base::iostate(ios_base::goodbit);
      sentry __cerb(*this, true);
      if (__cerb)
        {
          try
            {
              const int_type __idelim = traits_type::to_int_type(__delim);
              const int_type __eof = traits_type::eof();
              __streambuf_type* __sb = this->rdbuf();
              int_type __c = __sb->sgetc();

              while (_M_gcount + 1 < __n
                     && !traits_type::eq_int_type(__c, __eof)
                     && !traits_type::eq_int_type(__c, __idelim))
                {
                  *__s++ = traits_type::to_char_type(__c);
                  __c = __sb->snextc();
                  ++_M_gcount;
                }
              if (traits_type::eq_int_type(__c, __eof))
                __err |= ios_base::eofbit;
              else
                {
                  if (traits_type::eq_int_type(__c, __idelim))
                    {
                      __sb->sbumpc();
                      ++_M_gcount;
                    }
                  else
                    __err |= ios_base::failbit;
                }
            }
          catch(...)
            { this->_M_setstate(ios_base::badbit); }
        }
      // _GLIBCXX_RESOLVE_LIB_DEFECTS
      // 243. get and getline when sentry reports failure.
      if (__n > 0)
	*__s = char_type();
      if (!_M_gcount)
        __err |= ios_base::failbit;
      if (__err)
        this->setstate(__err);
      return *this;
    }

  // We provide three overloads, since the first two are much simpler
  // than the general case. Also, the latter two can thus adopt the
  // same "batchy" strategy used by getline above.
  template<typename _CharT, typename _Traits>
    basic_istream<_CharT, _Traits>&
    basic_istream<_CharT, _Traits>::
    ignore(void)
    {
      _M_gcount = 0;
      sentry __cerb(*this, true);
      if (__cerb)
	{
	  ios_base::iostate __err = ios_base::iostate(ios_base::goodbit);
	  try
	    {
	      const int_type __eof = traits_type::eof();
	      __streambuf_type* __sb = this->rdbuf();

	      if (traits_type::eq_int_type(__sb->sbumpc(), __eof))
		__err |= ios_base::eofbit;
	      else
		_M_gcount = 1;
	    }
	  catch(...)
	    { this->_M_setstate(ios_base::badbit); }
	  if (__err)
	    this->setstate(__err);
	}
      return *this;
    }

  template<typename _CharT, typename _Traits>
    basic_istream<_CharT, _Traits>&
    basic_istream<_CharT, _Traits>::
    ignore(streamsize __n)
    {
      _M_gcount = 0;
      sentry __cerb(*this, true);
      if (__cerb && __n > 0)
        {
          ios_base::iostate __err = ios_base::iostate(ios_base::goodbit);
          try
            {
              const int_type __eof = traits_type::eof();
              __streambuf_type* __sb = this->rdbuf();
              int_type __c = __sb->sgetc();

	      // N.B. On LFS-enabled platforms streamsize is still 32 bits
	      // wide: if we want to implement the standard mandated behavior
	      // for n == max() (see 27.6.1.3/24) we are at risk of signed
	      // integer overflow: thus these contortions. Also note that,
	      // by definition, when more than 2G chars are actually ignored,
	      // _M_gcount (the return value of gcount, that is) cannot be
	      // really correct, being unavoidably too small.
	      bool __large_ignore = false;
	      while (true)
		{
		  while (_M_gcount < __n
			 && !traits_type::eq_int_type(__c, __eof))
		    {
		      ++_M_gcount;
		      __c = __sb->snextc();
		    }
		  if (__n == numeric_limits<streamsize>::max()
		      && !traits_type::eq_int_type(__c, __eof))
		    {
		      _M_gcount = numeric_limits<streamsize>::min();
		      __large_ignore = true;
		    }
		  else
		    break;
		}

	      if (__large_ignore)
		_M_gcount = numeric_limits<streamsize>::max();

	      if (traits_type::eq_int_type(__c, __eof))
                __err |= ios_base::eofbit;
            }
          catch(...)
            { this->_M_setstate(ios_base::badbit); }
          if (__err)
            this->setstate(__err);
        }
      return *this;
    }

  template<typename _CharT, typename _Traits>
    basic_istream<_CharT, _Traits>&
    basic_istream<_CharT, _Traits>::
    ignore(streamsize __n, int_type __delim)
    {
      _M_gcount = 0;
      sentry __cerb(*this, true);
      if (__cerb && __n > 0)
        {
          ios_base::iostate __err = ios_base::iostate(ios_base::goodbit);
          try
            {
              const int_type __eof = traits_type::eof();
              __streambuf_type* __sb = this->rdbuf();
              int_type __c = __sb->sgetc();

	      // See comment above.
	      bool __large_ignore = false;
	      while (true)
		{
		  while (_M_gcount < __n
			 && !traits_type::eq_int_type(__c, __eof)
			 && !traits_type::eq_int_type(__c, __delim))
		    {
		      ++_M_gcount;
		      __c = __sb->snextc();
		    }
		  if (__n == numeric_limits<streamsize>::max()
		      && !traits_type::eq_int_type(__c, __eof)
		      && !traits_type::eq_int_type(__c, __delim))
		    {
		      _M_gcount = numeric_limits<streamsize>::min();
		      __large_ignore = true;
		    }
		  else
		    break;
		}

	      if (__large_ignore)
		_M_gcount = numeric_limits<streamsize>::max();

              if (traits_type::eq_int_type(__c, __eof))
                __err |= ios_base::eofbit;
	      else if (traits_type::eq_int_type(__c, __delim))
		{
		  if (_M_gcount < numeric_limits<streamsize>::max())
		    ++_M_gcount;
		  __sb->sbumpc();
		}
            }
          catch(...)
            { this->_M_setstate(ios_base::badbit); }
          if (__err)
            this->setstate(__err);
        }
      return *this;
    }

  template<typename _CharT, typename _Traits>
    typename basic_istream<_CharT, _Traits>::int_type
    basic_istream<_CharT, _Traits>::
    peek(void)
    {
      int_type __c = traits_type::eof();
      _M_gcount = 0;
      sentry __cerb(*this, true);
      if (__cerb)
	{
	  ios_base::iostate __err = ios_base::iostate(ios_base::goodbit);
	  try
	    {
	      __c = this->rdbuf()->sgetc();
	      if (traits_type::eq_int_type(__c, traits_type::eof()))
		__err |= ios_base::eofbit;
	    }
	  catch(...)
	    { this->_M_setstate(ios_base::badbit); }
	  if (__err)
	    this->setstate(__err);
	}
      return __c;
    }

  template<typename _CharT, typename _Traits>
    basic_istream<_CharT, _Traits>&
    basic_istream<_CharT, _Traits>::
    read(char_type* __s, streamsize __n)
    {
      _M_gcount = 0;
      sentry __cerb(*this, true);
      if (__cerb)
	{
	  ios_base::iostate __err = ios_base::iostate(ios_base::goodbit);
	  try
	    {
	      _M_gcount = this->rdbuf()->sgetn(__s, __n);
	      if (_M_gcount != __n)
		__err |= (ios_base::eofbit | ios_base::failbit);
	    }
	  catch(...)
	    { this->_M_setstate(ios_base::badbit); }
	  if (__err)
	    this->setstate(__err);
	}
      return *this;
    }

  template<typename _CharT, typename _Traits>
    streamsize
    basic_istream<_CharT, _Traits>::
    readsome(char_type* __s, streamsize __n)
    {
      _M_gcount = 0;
      sentry __cerb(*this, true);
      if (__cerb)
	{
	  ios_base::iostate __err = ios_base::iostate(ios_base::goodbit);
	  try
	    {
	      // Cannot compare int_type with streamsize generically.
	      const streamsize __num = this->rdbuf()->in_avail();
	      if (__num > 0)
		_M_gcount = this->rdbuf()->sgetn(__s, std::min(__num, __n));
	      else if (__num == -1)
		__err |= ios_base::eofbit;
	    }
	  catch(...)
	    { this->_M_setstate(ios_base::badbit); }
	  if (__err)
	    this->setstate(__err);
	}
      return _M_gcount;
    }

  template<typename _CharT, typename _Traits>
    basic_istream<_CharT, _Traits>&
    basic_istream<_CharT, _Traits>::
    putback(char_type __c)
    {
      // _GLIBCXX_RESOLVE_LIB_DEFECTS
      // 60. What is a formatted input function?
      _M_gcount = 0;
      sentry __cerb(*this, true);
      if (__cerb)
	{
	  ios_base::iostate __err = ios_base::iostate(ios_base::goodbit);
	  try
	    {
	      const int_type __eof = traits_type::eof();
	      __streambuf_type* __sb = this->rdbuf();
	      if (!__sb
		  || traits_type::eq_int_type(__sb->sputbackc(__c), __eof))
		__err |= ios_base::badbit;
	    }
	  catch(...)
	    { this->_M_setstate(ios_base::badbit); }
	  if (__err)
	    this->setstate(__err);
	}
      return *this;
    }

  template<typename _CharT, typename _Traits>
    basic_istream<_CharT, _Traits>&
    basic_istream<_CharT, _Traits>::
    unget(void)
    {
      // _GLIBCXX_RESOLVE_LIB_DEFECTS
      // 60. What is a formatted input function?
      _M_gcount = 0;
      sentry __cerb(*this, true);
      if (__cerb)
	{
	  ios_base::iostate __err = ios_base::iostate(ios_base::goodbit);
	  try
	    {
	      const int_type __eof = traits_type::eof();
	      __streambuf_type* __sb = this->rdbuf();
	      if (!__sb
		  || traits_type::eq_int_type(__sb->sungetc(), __eof))
		__err |= ios_base::badbit;
	    }
	  catch(...)
	    { this->_M_setstate(ios_base::badbit); }
	  if (__err)
	    this->setstate(__err);
	}
      return *this;
    }

  template<typename _CharT, typename _Traits>
    int
    basic_istream<_CharT, _Traits>::
    sync(void)
    {
      // _GLIBCXX_RESOLVE_LIB_DEFECTS
      // DR60.  Do not change _M_gcount.
      int __ret = -1;
      sentry __cerb(*this, true);
      if (__cerb)
	{
	  ios_base::iostate __err = ios_base::iostate(ios_base::goodbit);
	  try
	    {
	      __streambuf_type* __sb = this->rdbuf();
	      if (__sb)
		{
		  if (__sb->pubsync() == -1)
		    __err |= ios_base::badbit;
		  else
		    __ret = 0;
		}
	    }
	  catch(...)
	    { this->_M_setstate(ios_base::badbit); }
	  if (__err)
	    this->setstate(__err);
	}
      return __ret;
    }

  template<typename _CharT, typename _Traits>
    typename basic_istream<_CharT, _Traits>::pos_type
    basic_istream<_CharT, _Traits>::
    tellg(void)
    {
      // _GLIBCXX_RESOLVE_LIB_DEFECTS
      // DR60.  Do not change _M_gcount.
      pos_type __ret = pos_type(-1);
      try
	{
	  if (!this->fail())
	    __ret = this->rdbuf()->pubseekoff(0, ios_base::cur, ios_base::in);
	}
      catch(...)
	{ this->_M_setstate(ios_base::badbit); }
      return __ret;
    }

  template<typename _CharT, typename _Traits>
    basic_istream<_CharT, _Traits>&
    basic_istream<_CharT, _Traits>::
    seekg(pos_type __pos)
    {
      // _GLIBCXX_RESOLVE_LIB_DEFECTS
      // DR60.  Do not change _M_gcount.
      ios_base::iostate __err = ios_base::iostate(ios_base::goodbit);
      try
	{
	  if (!this->fail())
	    {
	      // 136.  seekp, seekg setting wrong streams?
	      const pos_type __p = this->rdbuf()->pubseekpos(__pos,
							     ios_base::in);

	      // 129. Need error indication from seekp() and seekg()
	      if (__p == pos_type(off_type(-1)))
		__err |= ios_base::failbit;
	    }
	}
      catch(...)
	{ this->_M_setstate(ios_base::badbit); }
      if (__err)
	this->setstate(__err);
      return *this;
    }

  template<typename _CharT, typename _Traits>
    basic_istream<_CharT, _Traits>&
    basic_istream<_CharT, _Traits>::
    seekg(off_type __off, ios_base::seekdir __dir)
    {
      // _GLIBCXX_RESOLVE_LIB_DEFECTS
      // DR60.  Do not change _M_gcount.
      ios_base::iostate __err = ios_base::iostate(ios_base::goodbit);
      try
	{
	  if (!this->fail())
	    {
	      // 136.  seekp, seekg setting wrong streams?
	      const pos_type __p = this->rdbuf()->pubseekoff(__off, __dir,
							     ios_base::in);

	      // 129. Need error indication from seekp() and seekg()
	      if (__p == pos_type(off_type(-1)))
		__err |= ios_base::failbit;
	    }
	}
      catch(...)
	{ this->_M_setstate(ios_base::badbit); }
      if (__err)
	this->setstate(__err);
      return *this;
    }

  // 27.6.1.2.3 Character extraction templates
  template<typename _CharT, typename _Traits>
    basic_istream<_CharT, _Traits>&
    operator>>(basic_istream<_CharT, _Traits>& __in, _CharT& __c)
    {
      typedef basic_istream<_CharT, _Traits>		__istream_type;
      typedef typename __istream_type::int_type         __int_type;

      typename __istream_type::sentry __cerb(__in, false);
      if (__cerb)
	{
	  ios_base::iostate __err = ios_base::iostate(ios_base::goodbit);
	  try
	    {
	      const __int_type __cb = __in.rdbuf()->sbumpc();
	      if (!_Traits::eq_int_type(__cb, _Traits::eof()))
		__c = _Traits::to_char_type(__cb);
	      else
		__err |= (ios_base::eofbit | ios_base::failbit);
	    }
	  catch(...)
	    { __in._M_setstate(ios_base::badbit); }
	  if (__err)
	    __in.setstate(__err);
	}
      return __in;
    }

  template<typename _CharT, typename _Traits>
    basic_istream<_CharT, _Traits>&
    operator>>(basic_istream<_CharT, _Traits>& __in, _CharT* __s)
    {
      typedef basic_istream<_CharT, _Traits>		__istream_type;
      typedef typename __istream_type::__streambuf_type __streambuf_type;
      typedef typename _Traits::int_type		int_type;
      typedef _CharT					char_type;
      typedef ctype<_CharT>				__ctype_type;

      streamsize __extracted = 0;
      ios_base::iostate __err = ios_base::iostate(ios_base::goodbit);
      typename __istream_type::sentry __cerb(__in, false);
      if (__cerb)
	{
	  try
	    {
	      // Figure out how many characters to extract.
	      streamsize __num = __in.width();
	      if (__num <= 0)
		__num = numeric_limits<streamsize>::max();

	      const __ctype_type& __ct = use_facet<__ctype_type>(__in.getloc());

	      const int_type __eof = _Traits::eof();
	      __streambuf_type* __sb = __in.rdbuf();
	      int_type __c = __sb->sgetc();

	      while (__extracted < __num - 1
		     && !_Traits::eq_int_type(__c, __eof)
		     && !__ct.is(ctype_base::space,
				 _Traits::to_char_type(__c)))
		{
		  *__s++ = _Traits::to_char_type(__c);
		  ++__extracted;
		  __c = __sb->snextc();
		}
	      if (_Traits::eq_int_type(__c, __eof))
		__err |= ios_base::eofbit;

	      // _GLIBCXX_RESOLVE_LIB_DEFECTS
	      // 68.  Extractors for char* should store null at end
	      *__s = char_type();
	      __in.width(0);
	    }
	  catch(...)
	    { __in._M_setstate(ios_base::badbit); }
	}
      if (!__extracted)
	__err |= ios_base::failbit;
      if (__err)
	__in.setstate(__err);
      return __in;
    }

  // 27.6.1.4 Standard basic_istream manipulators
  template<typename _CharT, typename _Traits>
    basic_istream<_CharT,_Traits>&
    ws(basic_istream<_CharT,_Traits>& __in)
    {
      typedef basic_istream<_CharT, _Traits>		__istream_type;
      typedef typename __istream_type::__streambuf_type __streambuf_type;
      typedef typename __istream_type::__ctype_type	__ctype_type;
      typedef typename __istream_type::int_type		__int_type;

      const __ctype_type& __ct = use_facet<__ctype_type>(__in.getloc());
      const __int_type __eof = _Traits::eof();
      __streambuf_type* __sb = __in.rdbuf();
      __int_type __c = __sb->sgetc();

      while (!_Traits::eq_int_type(__c, __eof)
	     && __ct.is(ctype_base::space, _Traits::to_char_type(__c)))
	__c = __sb->snextc();

       if (_Traits::eq_int_type(__c, __eof))
	 __in.setstate(ios_base::eofbit);
      return __in;
    }

  // 21.3.7.9 basic_string::getline and operators
  template<typename _CharT, typename _Traits, typename _Alloc>
    basic_istream<_CharT, _Traits>&
    operator>>(basic_istream<_CharT, _Traits>& __in,
	       basic_string<_CharT, _Traits, _Alloc>& __str)
    {
      typedef basic_istream<_CharT, _Traits>		__istream_type;
      typedef typename __istream_type::int_type		__int_type;
      typedef typename __istream_type::__streambuf_type __streambuf_type;
      typedef typename __istream_type::__ctype_type	__ctype_type;
      typedef basic_string<_CharT, _Traits, _Alloc>	__string_type;
      typedef typename __string_type::size_type		__size_type;

      __size_type __extracted = 0;
      ios_base::iostate __err = ios_base::iostate(ios_base::goodbit);
      typename __istream_type::sentry __cerb(__in, false);
      if (__cerb)
	{
	  try
	    {
	      // Avoid reallocation for common case.
	      __str.erase();
	      _CharT __buf[128];
	      __size_type __len = 0;	      
	      const streamsize __w = __in.width();
	      const __size_type __n = __w > 0 ? static_cast<__size_type>(__w)
		                              : __str.max_size();
	      const __ctype_type& __ct = use_facet<__ctype_type>(__in.getloc());
	      const __int_type __eof = _Traits::eof();
	      __streambuf_type* __sb = __in.rdbuf();
	      __int_type __c = __sb->sgetc();

	      while (__extracted < __n
		     && !_Traits::eq_int_type(__c, __eof)
		     && !__ct.is(ctype_base::space, _Traits::to_char_type(__c)))
		{
		  if (__len == sizeof(__buf) / sizeof(_CharT))
		    {
		      __str.append(__buf, sizeof(__buf) / sizeof(_CharT));
		      __len = 0;
		    }
		  __buf[__len++] = _Traits::to_char_type(__c);
		  ++__extracted;
		  __c = __sb->snextc();
		}
	      __str.append(__buf, __len);

	      if (_Traits::eq_int_type(__c, __eof))
		__err |= ios_base::eofbit;
	      __in.width(0);
	    }
	  catch(...)
	    {
	      // _GLIBCXX_RESOLVE_LIB_DEFECTS
	      // 91. Description of operator>> and getline() for string<>
	      // might cause endless loop
	      __in._M_setstate(ios_base::badbit);
	    }
	}
      // 211.  operator>>(istream&, string&) doesn't set failbit
      if (!__extracted)
	__err |= ios_base::failbit;
      if (__err)
	__in.setstate(__err);
      return __in;
    }

  template<typename _CharT, typename _Traits, typename _Alloc>
    basic_istream<_CharT, _Traits>&
    getline(basic_istream<_CharT, _Traits>& __in,
	    basic_string<_CharT, _Traits, _Alloc>& __str, _CharT __delim)
    {
      typedef basic_istream<_CharT, _Traits>		__istream_type;
      typedef typename __istream_type::int_type		__int_type;
      typedef typename __istream_type::__streambuf_type __streambuf_type;
      typedef typename __istream_type::__ctype_type	__ctype_type;
      typedef basic_string<_CharT, _Traits, _Alloc>	__string_type;
      typedef typename __string_type::size_type		__size_type;

      __size_type __extracted = 0;
      const __size_type __n = __str.max_size();
      ios_base::iostate __err = ios_base::iostate(ios_base::goodbit);
      typename __istream_type::sentry __cerb(__in, true);
      if (__cerb)
	{
	  try
	    {
	      __str.erase();
	      const __int_type __idelim = _Traits::to_int_type(__delim);
	      const __int_type __eof = _Traits::eof();
	      __streambuf_type* __sb = __in.rdbuf();
	      __int_type __c = __sb->sgetc();

	      while (__extracted < __n
		     && !_Traits::eq_int_type(__c, __eof)
		     && !_Traits::eq_int_type(__c, __idelim))
		{
		  __str += _Traits::to_char_type(__c);
		  ++__extracted;
		  __c = __sb->snextc();
		}

	      if (_Traits::eq_int_type(__c, __eof))
		__err |= ios_base::eofbit;
	      else if (_Traits::eq_int_type(__c, __idelim))
		{
		  ++__extracted;		  
		  __sb->sbumpc();
		}
	      else
		__err |= ios_base::failbit;
	    }
	  catch(...)
	    {
	      // _GLIBCXX_RESOLVE_LIB_DEFECTS
	      // 91. Description of operator>> and getline() for string<>
	      // might cause endless loop
	      __in._M_setstate(ios_base::badbit);
	    }
	}
      if (!__extracted)
	__err |= ios_base::failbit;
      if (__err)
	__in.setstate(__err);
      return __in;
    }

  template<class _CharT, class _Traits, class _Alloc>
    inline basic_istream<_CharT,_Traits>&
    getline(basic_istream<_CharT, _Traits>& __in,
	    basic_string<_CharT,_Traits,_Alloc>& __str)
    { return getline(__in, __str, __in.widen('\n')); }

  // Inhibit implicit instantiations for required instantiations,
  // which are defined via explicit instantiations elsewhere.
  // NB:  This syntax is a GNU extension.
#if _GLIBCXX_EXTERN_TEMPLATE
  extern template class basic_istream<char>;
  extern template istream& ws(istream&);
  extern template istream& operator>>(istream&, char&);
  extern template istream& operator>>(istream&, char*);
  extern template istream& operator>>(istream&, unsigned char&);
  extern template istream& operator>>(istream&, signed char&);
  extern template istream& operator>>(istream&, unsigned char*);
  extern template istream& operator>>(istream&, signed char*);

  extern template class basic_iostream<char>;

#ifdef _GLIBCXX_USE_WCHAR_T
  extern template class basic_istream<wchar_t>;
  extern template wistream& ws(wistream&);
  extern template wistream& operator>>(wistream&, wchar_t&);
  extern template wistream& operator>>(wistream&, wchar_t*);

  extern template class basic_iostream<wchar_t>;
#endif
#endif

_GLIBCXX_END_NAMESPACE

#endif
