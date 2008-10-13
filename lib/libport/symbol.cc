/**
 ** \file libport/symbol.cc
 ** \brief Implementation of libport::Symbol.
 */

//<<-
#include <cctype>
//->>
#include <ostream>
#include <sstream>

#include <boost/functional/hash.hpp>
#include <boost/static_assert.hpp>

#include <libport/containers.hh>
#include <libport/escape.hh>
#include <libport/symbol.hh>

namespace libport
{
  // That symbols are as large as pointers is part of the
  // interface: they should be handled by copy, not by reference.
  BOOST_STATIC_ASSERT(sizeof(Symbol) == sizeof(void*));


  //<<
  Symbol::Symbol (const std::string& s)
    : str_ (&*string_set_instance ().insert (s).first)
  {
  }

  Symbol::string_set_type&
  Symbol::string_set_instance ()
  {
    static string_set_type map;
    return map;
  }

  Symbol::string_size_type
  Symbol::string_map_size ()
  {
    return string_set_instance ().size ();
  }
  //>>

  Symbol
  Symbol::fresh ()
  {
    return fresh (Symbol("a"));
  }

  Symbol
  Symbol::fresh (const std::string& s)
  {
    // Counter for unique symbols.
    static unsigned c = 0;
    std::ostringstream o;
    do {
      o.str ("");
      o << s << "_" << c;
      ++c;
    } while (mhas(string_set_instance (), o.str ()));
    return Symbol(o.str ());
  }

  Symbol
  Symbol::fresh (const Symbol& s)
  {
    return fresh(s.name_get ());
  }

  std::size_t
  hash_value(libport::Symbol s)
  {
    return boost::hash<const void*>()(&s.name_get());
  }

  std::ostream&
  Symbol::print_escaped (std::ostream& ostr) const
  {
    assert (str_);

    // Check if the symbol is not a standard one, i.e. if it does not match
    // [A-Za-z_][A-Za-z0-9_]*
    unsigned i = 0;
    if (str_->length() && ((*str_)[0] == '_' || isalpha((*str_)[0])))
    {
      i++;
      while (i < str_->length() && ((*str_)[i] == '_' || isalnum((*str_)[i])))
	i++;
    }

    // If the symbol contains special characters, escape them, and print the
    // symbol between single quotes.
    if (!str_->length() || i < str_->length())
      return ostr << '\'' << escape(*str_) << '\'';
    else
      return ostr << *str_;
  }

} // namespace libport
