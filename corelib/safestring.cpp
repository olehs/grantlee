/*
  This file is part of the Grantlee template system.

  Copyright (c) 2009,2010 Stephen Kelly <steveire@gmail.com>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either version
  2 of the Licence, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Library General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "safestring.h"

using namespace Grantlee;

SafeString::SafeString()
  : m_nestedString( this ),
    m_safety( IsNotSafe ),
    m_needsescape( false )
{

}

SafeString::SafeString( const Grantlee::SafeString& safeString )
  : m_nestedString( safeString.get(), this ),
    m_safety( safeString.m_safety ),
    m_needsescape( safeString.m_needsescape )
{

}

SafeString::SafeString( const QString &str, bool safe )
  : m_nestedString( str, this ),
    m_safety( safe ? IsSafe : IsNotSafe ),
    m_needsescape( false )
{

}

SafeString::SafeString( const QString &str, Safety safety )
  : m_nestedString( str, this ),
    m_safety( safety ),
    m_needsescape( false )
{

}

SafeString::~SafeString()
{
}

void SafeString::setNeedsEscape( bool needsEscape )
{
  m_needsescape = needsEscape;
}

bool SafeString::needsEscape() const
{
  return m_needsescape;
}

void SafeString::setSafety( Grantlee::SafeString::Safety safety )
{
  m_safety = safety;
}

bool SafeString::isSafe() const
{
  return m_safety == IsSafe;
}

SafeString::NestedString::NestedString( SafeString *safeString )
    : m_safeString( safeString )
{

}

SafeString::NestedString::NestedString( const QString &content, SafeString *safeString )
  : QString( content ),
    m_safeString( safeString )
{

}

SafeString& SafeString::operator=( const SafeString &str )
{
  m_nestedString = str.m_nestedString;
  m_nestedString.m_safeString = this;
  m_safety = str.m_safety;
  m_needsescape = str.m_needsescape;
  return *this;
}

SafeString SafeString::operator+( const QString &str )
{
  return SafeString( m_nestedString + str, IsNotSafe );
}

SafeString SafeString::operator+( const SafeString &str )
{
  if ( !str.isSafe() )
    return SafeString( m_nestedString + str.get(), IsNotSafe );
  return SafeString( m_nestedString + str.get(), m_safety );
}

SafeString &SafeString::operator+=( const QString &str )
{
  m_safety = IsNotSafe;

  m_nestedString += str;
  return *this;
}

SafeString &SafeString::operator+=( const SafeString &str )
{
  if ( !str.isSafe() )
    m_safety = IsNotSafe;

  m_nestedString += str.get();
  return *this;
}

bool SafeString::operator==( const Grantlee::SafeString &other ) const
{
  // Don't compare safety or account for future escaping here.
  // See TestBuiltins testEscaping
  return m_nestedString == other.get();
}

bool SafeString::operator==( const QString &other ) const
{
  return m_nestedString == other;
}

void SafeString::NestedString::chop( int n )
{
  QString::chop( n );
  m_safeString->m_safety = false;
}


