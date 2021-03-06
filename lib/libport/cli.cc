/*
 * Copyright (C) 2008-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#include <iostream>

#include <libport/cli.hh>
#include <libport/format.hh>
#include <libport/program-name.hh>

namespace libport
{

  void
  usage_error(const std::string& err)
  {
    throw std::runtime_error(format("%s\n"
                                    "Try `%s --help' for more information.",
                                    err, program_name()));
  }

  void
  required_argument(const std::string& opt)
  {
    usage_error(format("option `%s' requires an argument", opt));
  }

  void
  invalid_option(const std::string& opt)
  {
    usage_error(format("invalid option: %s", opt));
  }

  void
  invalid_option_val(const std::string& opt, const std::string& arg)
  {
    usage_error(format("invalid argument for option `%s': %s", opt, arg));
  }

}
