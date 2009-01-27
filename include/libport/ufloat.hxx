/// \file libport/ufloat.hxx
/// \brief Inline implementations for ufloat.

#ifndef LIBPORT_UFLOAT_HXX
# define LIBPORT_UFLOAT_HXX

# include <libport/ufloat.hh>


/*-------------------.
| Ufloat tabulated.  |
`-------------------*/

# ifdef LIBPORT_URBI_UFLOAT_TABULATED
namespace libport
{
  /** @return the tabulated arccosinus of given @a angle, in radian, using
   * linear interpolation.  */
  inline ufloat tabulatedACos(ufloat angle)
  {
    return (PI >> 1) - tabulatedASin(angle);
  }

  inline ufloat sin(ufloat angle) {return tabulatedSin(angle);}
  inline ufloat cos(ufloat angle) {return tabulatedCos(angle);}
  inline ufloat tan(ufloat angle) {return sin(angle)/cos(angle);}

  inline ufloat asin(ufloat angle) {return tabulatedASin(angle);}
  inline ufloat acos(ufloat angle) {return tabulatedACos(angle);}
}
# endif

# include <libport/config.h>
/* round is not C++ standard (not even POSIX) and neither gnulib nor Boost
 * provide one.  So here is my quick replacement.  */
# ifndef LIBPORT_HAVE_ROUND
#  include <cmath>
namespace libport
{
  inline float round(float d)
  {
    return floor(d + 0.5f + FLT_EPSILON);
  }

  inline double round(double d)
  {
    return floor(d + 0.5 + DBL_EPSILON);
  }

  inline long double round(long double d)
  {
    return floor(d + 0.5 + LDBL_EPSILON);
  }

}
# endif /* !LIBPORT_HAVE_ROUND */

/* trunc is not C++ standard (not even POSIX) and although gnulib says it
 * provides a replacement in its manual, in fact it doesn't, nor does Boost.
 * So here is my quick replacement.  */
# ifndef LIBPORT_HAVE_TRUNC
#  include <cmath>
namespace libport
{
  inline double trunc (double d)
  {
    if (d < 0.0)
      return ceil (d);
    else
      return floor (d);
  }
}
# endif /* !LIBPORT_HAVE_TRUNC */

#endif // !LIBPORT_UFLOAT_HXX
