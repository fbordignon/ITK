/*=========================================================================
 *
 *  Copyright Insight Software Consortium
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/
#ifndef itkSmartPointer_h
#define itkSmartPointer_h

#include <iostream>
#include <utility>
#include <type_traits>
#include "itkConfigure.h"


namespace itk
{
/** \class SmartPointer
 * \brief Implements transparent reference counting.
 *
 * SmartPointer implements reference counting by overloading
 * operator -> (and *) among others. This allows natural interface
 * to the class referred to by the pointer without having to invoke
 * special Register()/UnRegister() methods directly.
 *
 * To compile / test this class
 * Windows: cl SmartPointerTest.cxx; .\\SmartPointerTest.exe
 * linux:   c++ SmartPointerTest.cxx ./a.out
 * other:   CCcompiler SmartPointerTest.cxx  ./a.out
 *
 * The state of the SmartPointer after a `move` is with a value of
 * `nullptr`.
 *
 * \ingroup ITKSystemObjects
 * \ingroup DataAccess
 * \ingroup ITKCommon
 */
template< typename TObjectType >
class SmartPointer
{
public:
  using ObjectType = TObjectType;

  template<typename T> using EnableIfConvertible =
    typename std::enable_if<std::is_convertible<T*, TObjectType*>::value>;

  /** Constructor  */
  constexpr SmartPointer () ITK_NOEXCEPT:
    m_Pointer(nullptr)
  { }

  /** Copy constructor  */
  SmartPointer (const SmartPointer& p) ITK_NOEXCEPT:
    m_Pointer(p.m_Pointer)
  { this->Register(); }

  constexpr SmartPointer (std::nullptr_t p) ITK_NOEXCEPT:
    m_Pointer(p)
    {}

  /** constructor with implicit conversion of pointer type */
  template<typename T,
           typename = typename EnableIfConvertible<T>::type>
  SmartPointer(const SmartPointer<T> &p) ITK_NOEXCEPT:
    m_Pointer(p.m_Pointer)
    {
      this->Register();
    }

  /** Move constructor */
  SmartPointer (SmartPointer<ObjectType > &&p) ITK_NOEXCEPT :
    m_Pointer(p.m_Pointer)
    { p.m_Pointer = nullptr; }

  /** move constructor with implicit conversion of pointer type */
  template<typename T,
           typename = typename EnableIfConvertible<T>::type >
  SmartPointer( SmartPointer<T> &&p) ITK_NOEXCEPT:
    m_Pointer(p.m_Pointer)
    { p.m_Pointer = nullptr; }

  /** Constructor to pointer p  */
  SmartPointer (ObjectType *p) ITK_NOEXCEPT:
    m_Pointer(p)
  { this->Register(); }

  /** Destructor  */
  ~SmartPointer ()
  {
    this->UnRegister();
    m_Pointer = nullptr;
  }

  /** Overload operator ->  */
  ObjectType * operator->() const ITK_NOEXCEPT
  { return m_Pointer; }

  ObjectType &operator*() const ITK_NOEXCEPT
    {return *m_Pointer;}

  explicit operator bool() const ITK_NOEXCEPT
    { return m_Pointer != nullptr; }

  /** Return pointer to object.  */
  operator ObjectType *() const ITK_NOEXCEPT
    { return m_Pointer; }

  /** Test if the pointer is not NULL. */
  bool IsNotNull() const ITK_NOEXCEPT
  { return m_Pointer != nullptr; }

  /** Test if the pointer is NULL. */
  bool IsNull() const ITK_NOEXCEPT
  { return m_Pointer == nullptr; }


  /** Access function to pointer. */
  ObjectType * GetPointer() const ITK_NOEXCEPT
  { return m_Pointer; }

  /** Overload operator assignment.
   *
   * This method is also implicitly used for move semantics.
   * Additionally, it relies on constructors for additional conversion
   * for pointer types.
   */
  // cppcheck-suppress operatorEqVarError
  SmartPointer & operator=(SmartPointer  r) ITK_NOEXCEPT
  {
    // The Copy-Swap idiom is used, with the implicit copy from the
    // value-based argument r (intentionally not reference). If a move
    // is requested it will be moved into r with the move constructor.
    this->Swap(r);
    return *this;
  }

  SmartPointer &operator=(std::nullptr_t)  ITK_NOEXCEPT
    {
      this->UnRegister();
      this->m_Pointer = nullptr;
      return *this;
    }

  /** Function to print object pointed to  */
  ObjectType * Print(std::ostream & os) const
  {
    if( this->IsNull() )
      {
      os << "(null)";
      }
    else
      {
      // This prints the object pointed to by the pointer
      ( *m_Pointer ).Print(os);
      }
    return m_Pointer;
  }

#if ! defined ( ITK_LEGACY_REMOVE )
  void swap(SmartPointer &other) ITK_NOEXCEPT
    {
      this->Swap(other);
    }
#endif

  void Swap(SmartPointer &other)  ITK_NOEXCEPT
    {
      ObjectType *tmp = this->m_Pointer;
      this->m_Pointer = other.m_Pointer;
      other.m_Pointer = tmp;
    }

private:
  /** The pointer to the object referred to by this smart pointer. */
  ObjectType *m_Pointer;

  template<typename T>
  friend class SmartPointer;

  void Register()  ITK_NOEXCEPT
  {
    if ( m_Pointer ) { m_Pointer->Register(); }
  }

  void UnRegister() ITK_NOEXCEPT
  {
    if ( m_Pointer ) { m_Pointer->UnRegister(); }
  }
};


/** Comparison of pointers. Equality comparison. */
template < class T, class TU >
bool operator==( const SmartPointer<T>& l, const SmartPointer<TU>& r )  ITK_NOEXCEPT
{ return ( l.GetPointer() == r.GetPointer() ); }
template < class T >
bool operator==( const SmartPointer<T>& l, std::nullptr_t )  ITK_NOEXCEPT
{ return ( l.GetPointer() == nullptr ); }
template < class T >
bool operator==( std::nullptr_t, const SmartPointer<T>& r )  ITK_NOEXCEPT
{ return ( nullptr == r.GetPointer() ); }

/** Comparison of pointers. Not equal comparison. */
template < class T, class TU >
bool operator!=( const SmartPointer<T>& l, const SmartPointer<TU>& r )  ITK_NOEXCEPT
{ return ( l.GetPointer() != r.GetPointer() ); }
template < class T >
bool operator!=( const SmartPointer<T>& l, std::nullptr_t )  ITK_NOEXCEPT
{ return ( l.GetPointer() != nullptr ); }
template < class T >
bool operator!=( std::nullptr_t, const SmartPointer<T>& r )  ITK_NOEXCEPT
{ return ( nullptr != r.GetPointer() ); }


/** Comparison of pointers. Less than comparison.  */
template < class T, class TU >
bool operator<( const SmartPointer<T>& l, const SmartPointer<TU>& r )  ITK_NOEXCEPT
{ return ( l.GetPointer() < r.GetPointer() ); }

/** Comparison of pointers. Greater than comparison.  */
template < class T, class TU >
bool operator>( const SmartPointer<T>& l, const SmartPointer<TU>& r )  ITK_NOEXCEPT
{ return ( l.GetPointer() > r.GetPointer() ); }

/** Comparison of pointers. Less than or equal to comparison.  */
template < class T, class TU >
bool operator<=( const SmartPointer<T>& l, const SmartPointer<TU>& r )  ITK_NOEXCEPT
{ return ( l.GetPointer() <= r.GetPointer() ); }

/** Comparison of pointers. Greater than or equal to comparison.  */
template < class T, class TU >
bool operator>=( const SmartPointer<T>& l, const SmartPointer<TU>& r ) ITK_NOEXCEPT
{ return ( l.GetPointer() >= r.GetPointer() ); }

template< typename T >
std::ostream & operator<<(std::ostream & os, SmartPointer< T > p)
{
  p.Print(os);
  return os;
}

template<typename T>
inline void swap( SmartPointer<T> &a, SmartPointer<T> &b ) ITK_NOEXCEPT
{
  a.Swap(b);
}

} // end namespace itk

#endif
