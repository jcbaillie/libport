/*
 * Copyright (C) 2009-2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#include <libport/cstdio>
#include <libport/debug.hh>
#include <libport/format.hh>
#include <libport/iostream>
#include <libport/path.hxx>
#include <libport/program-name.hh>
#include <libport/sysexits.hh>
#include <libport/xltdl.hh>

GD_CATEGORY(Libport.Xltdl);

extern "C"
{
  int
  libport_xlt_details_identity(int i)
  {
    return i;
  }
}

namespace libport
{
  namespace xlt
  {
    namespace details
    {

       /// \param verbosity  the current debug level.
       /// \param level      the level of this message.
       static
       int
       ltdebug(unsigned /* verbosity */,
               unsigned level, const char* format, va_list args)
       {
         int errors = 0;
         char* msg = 0;
         errors += vasprintf(&msg, format, args) < 0;
         if (!errors && msg)
         {
           LIBPORT_USE(level);
           GD_FINFO_DEBUG("%s%s", std::string(level * 2, ' '), msg);
           free(msg);
         }
         return errors;
       }

       void init()
       {
         static bool first = true;
         if (first)
         {
           first = false;
         }
       }
     }
   }


   /*------------------------.
   | xlt_advise::exception.  |
   `------------------------*/

   xlt_advise::exception::exception(const std::string& msg)
     : std::runtime_error(msg)
   {}


   /*-------------.
   | xlt_advise.  |
   `-------------*/

   xlt_advise::xlt_advise()
     throw(xlt_advise::exception)
     : path_()
   {
     global_ = true;
     xlt::details::init();
  }

  // FIXME: Bad: dtors must not throw.
  xlt_advise::~xlt_advise()
    throw(xlt_advise::exception)
  {
    // FIXME: lt_dlexit when we refcount.
  }

  xlt_advise&
  xlt_advise::global(bool global)
    throw(xlt_advise::exception)
  {
    this->global_ = global;
    return *this;
  }

  xlt_advise&
  xlt_advise::ext()
    throw(xlt_advise::exception)
  {
    return *this;
  }

  const file_library&
  xlt_advise::path() const throw()
  {
    return path_;
  }

  file_library&
  xlt_advise::path() throw()
  {
    return path_;
  }

  xlt_advise&
  xlt_advise::path(const file_library& p) throw()
  {
    path_ = p;
    return *this;
  }

  lt_dlhandle
  xlt_advise::dlopen_(const std::string& s) const
    throw(xlt_advise::exception)
  {
    lt_dlhandle res = dlopen(s.c_str(), RTLD_LAZY | (global_?RTLD_GLOBAL:RTLD_LOCAL));
    GD_FINFO_TRACE("loading %s: %s", s, res ? "pass" : "fail");
    return res;
  }

#if defined WIN32
# define APPLE_LINUX_WINDOWS(Apple, Linux, Windows) Windows
#elif defined __APPLE__
# define APPLE_LINUX_WINDOWS(Apple, Linux, Windows) Apple
#else
# define APPLE_LINUX_WINDOWS(Apple, Linux, Windows) Linux
#endif


  xlt_handle
  xlt_advise::open(const std::string& ss)
    throw(xlt_advise::exception)
  {
    std::string s(ss);
    GD_FINFO_TRACE("loading %s", s);

    // Clear the error flags from previous runs.
    //
    // FIXME: This should be done in libltdl itself.  The problem
    // probably arose from our patches to preserve the first error
    // when traversing the user path: now, even on success, the error
    // flag remains set, and the following run is hindered by it.  We
    // need to complete this patch with a means to ensure that the
    // error flags is restored to its previous state (typically
    // no-error) when eventually we managed to load the file.

    lt_dlhandle res = 0;
    const char* libext = APPLE_LINUX_WINDOWS(".dylib", ".so", ".dll");
    if (s.substr(s.length() - strlen(libext)) != libext)
      s += libext;
    std::string errors;
    // We cannot simply use search_file in file_library, because we
    // don't know the extension of the file we are looking for (*.la,
    // *.so, *.dyld etc.).  That's an implementation detail that ltdl
    // saves us from.
    if (path_.search_path_get().empty() || libport::path(s).absolute_get())
    {
      std::cerr <<"try " << s << std::endl;
      res = dlopen_(s);
    }
    else
      foreach (const libport::path& p, path_.search_path_get())
      {
        std::cerr << "try " << p << " " << s << std::endl;
        if ((res = dlopen_(p / s)))
          break;
       const char* e = dlerror();
       errors += boost::lexical_cast<std::string>(p)  + " : " + (e?e:"unknown error") + "\n";
      }
    if (!res)
    {
      GD_FINFO_TRACE("errors where: %s", errors);
      throw exception(errors);
    }
    return res;
  }


  /// Throw an exception, or exit with exit_failure_ if nonnull.
  ATTRIBUTE_NORETURN
  void
  xlt_advise::fail(std::string msg)
    throw(xlt_advise::exception)
  {
    perror("fail");
    msg += ": ";
    const char* err = dlerror();
    msg += err?err:"unknown error";
    throw exception(msg);
  }



  /*-------------.
  | xlt_handle.  |
  `-------------*/

  xlt_handle::xlt_handle(lt_dlhandle h)
    : handle(h)
  {}

  xlt_handle::~xlt_handle()
  {
    // FIXME: We can't close -- yet.  We need to keep track of the
    // number of trackers.  Otherwise a simple "handle h =
    // advise.open" will close the handle when cleaning the temporary
    // made by "advise.open".

    // close();
  }

  void
  xlt_handle::close()
    throw(xlt_handle::exception)
  {
    if (handle)
    {
      int errors = dlclose(handle);
      handle = 0;
      if (errors)
        fail("cannot close");
    }
  }

  void
  xlt_handle::detach()
  {
    aver(handle);
    handle = 0;
  }

  void
  xlt_handle::attach(lt_dlhandle h)
  {
    aver(!handle);
    handle = h;
  }

  /// Throw an exception, or exit with exit_failure_ if nonnull.
  ATTRIBUTE_NORETURN
  void
  xlt_handle::fail(const std::string& msg)
    throw(xlt_advise::exception)
  {
    xlt_advise::fail(msg);
  }


  /*-------------.
  | Standalone.  |
  `-------------*/

  xlt_handle
  xlt_openext(const std::string& s, bool global, int exit_failure)
    throw (xlt_advise::exception)
  {
    try
    {
      return xlt_advise().global(global).ext().open(s);
    }
    catch (std::exception& s)
    {
      std::cerr << libport::program_name()
                << ": "
                << s.what()
                << std::endl
                << libport::exit(exit_failure);
    }
  }
}
