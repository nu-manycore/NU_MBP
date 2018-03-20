/*
 * dump debug print
 */

#include <unistd.h>
#include <stdio.h>
#include <assert.h>

#include <iostream>
#include <fstream>
#include <memory>
#include <sstream>
#include <string>

#include <vector>
#include <map>

#include "Simulink_XSD/SimulinkModel.hxx"

#if __cplusplus >= 201103L && !defined(XSD_CXX11)
#error "Not defined XSD_CXX11 in SimulinkModel.hxx, please Re-make it"
#elif __cplusplus < 201103L && defined (XSD_CXX11)
#error "defined XSD_CXX11 in SimulinkModel.hxx, please Re-make it"
#endif

using namespace SimulinkModel::XSD;

static int verbose = 0;
static int do_call = 0;

static std::vector < int > ids;

static std::string cindent;

static void dump_test_print (const blocks_T& blks);

typedef std::pair < std::string, int > vdef_T; // block, count
typedef std::map < std::string, vdef_T >  vmap_T; // vname

static vmap_T vmap;

static void
dump_test_print (const block_T& blk)
{
  if (blk.merged_to ().size ())
    return;

  if (blk.var ().size ())
    {
      bool out = false;
      for (block_T::var_const_iterator vi (blk.var ().begin ());
	   vi != blk.var ().end (); vi++)
	{
	  const var_T& v = *vi;
	  if (v.mode () == "output" || v.mode () == "extout")
	    {
	      if (out == false)
		{
		  const block_T::id_optional& bid = blk.id ();

		  assert (bid);

		  if (ids.size () > 0
		      && find (ids.begin (), ids.end (), *bid) == ids.end ())
		    continue;

		  if (do_call)
		    {
		      char nam[64];
		      snprintf (nam, sizeof nam,
				"TASK_EXECUTE_%04u_COMPLETED", *bid);
		      std::cout << "#ifdef " << nam
				  << " /* " << blk.blocktype ()
				  << ':' << blk.name () << " */\n"
				  << cindent << nam << "();\n"
				  << "#endif\n";
		      break;
		    }
		  else
		    {
		      char id[5];
		      snprintf (id, sizeof id, "%04u", *bid);
		      std::cout << "\n/* " << blk.blocktype ()
				  << ':' << blk.name () << " */\n"
				  << "#define TASK_EXECUTE_"
				  << id
				  << "_COMPLETED()\\\n"
				  << "  do {\\\n";
		    }
		  out = true;
		}

	      vmap_T::iterator i = vmap.find (v.name ());
	      if (i == vmap.end ())
		vmap.insert (std::make_pair (v.name (),
					       std::make_pair (blk.name (),
								 1)));
	      else
		{
		  const std::string& defblk = i->second.first;
		  int& num = i->second.second;
		  std::cerr << "Warning: " << v.name ()
			      << ": multiple defined at "
			      << blk.name ()
			      << ": the first in "
			      << defblk
			      << std::endl;
		  num++;
		}

	      std::cout << "    mcos_debug_printf(\"[%u]"
			  << v.name ()
			  << ":";

	      const var_T::size_optional& s = v.size ();
	      const var_T::storage_optional& sto = v.storage ();

	      bool complex = false;
	      const char* fmt = "%.7f";
	      const char* cast = "(double)";
	      const char* type = v.type ().c_str ();
	      if (strncmp (type, "int", 3) == 0
		  || strncmp (type, "byte", 4) == 0
		  || strncmp (type, "char", 4) == 0)
		{
		  fmt = "%d";
		  cast = "(int)";
		}
	      else if (strncmp (type, "uint", 4) == 0
		       || strncmp (type, "uchar", 5) == 0
		       || strncmp (type, "boolean", 7) == 0)
		{
		  fmt = "%u";
		  cast = "(unsigned int)";
		}
	      else if (strncmp (type, "ulong", 5) == 0)
		{
		  fmt = "%lu";
		  cast = "(unsigned long)";
		}
	      else if (strncmp (type, "creal", 5) == 0)
		complex = true;
	      else if (strncmp (type, "cint", 4) == 0)
		{
		  complex = true;
		  fmt = "%d";
		  cast = "(int)";
		}
	      else if (strncmp (type, "cuint", 5) == 0)
		{
		  complex = true;
		  fmt = "%u";
		  cast = "(unsigned int)";
		}

	      if (s && *s > 1)
		{
		  if (complex)
		    {
		      for (int i = 0; i < *s; i++)
			{
			  if (i)
			    std::cout << ',';
			  std::cout << '(' << fmt << ',' << fmt << ')';
			}
		      std::cout << "\\n\", mcos__cycle_step";

		      for (int i = 0; i < *s; i++)
			{
			  std::cout << "," << cast;
			  if (sto)
			    std::cout << *sto << '.';
			  std::cout << v.name ()
				      << '[' << i << "].re";
			  std::cout << "," << cast;
			  if (sto)
			    std::cout << *sto << '.';
			  std::cout << v.name ()
				      << '[' << i << "].im";
			}
		    }
		  else
		    {
		      for (int i = 0; i < *s; i++)
			{
			  if (i)
			    std::cout << ",";
			  std::cout << fmt;
			}
		      std::cout << "\\n\", mcos__cycle_step";

		      for (int i = 0; i < *s; i++)
			{
			  std::cout << "," << cast;
			  if (sto)
			    std::cout << *sto << '.';
			  std::cout << v.name ()
				      << '[' << i << ']';
			}
		    }
		}
	      else
		{
		  if (complex)
		    {
		      std::cout << '(' << fmt << ',' << fmt << ')'
				  << "\\n\", mcos__cycle_step, "
				  << cast;
		      if (sto)
			std::cout << *sto << '.';
		      std::cout << v.name () << ".re, " << cast;
		      if (sto)
			std::cout << *sto << '.';
		      std::cout << v.name () << ".im";
		    }
		  else
		    {
		      std::cout << fmt << "\\n\", mcos__cycle_step, "
				  << cast;
		      if (sto)
			std::cout << *sto << '.';
		      std::cout << v.name ();
		    }
		}
	      std::cout << ");\\\n";
	    }
	}

      if (out && do_call == 0)
	std::cout << "  } while (0)" << std::endl;
    }

  for (block_T::blocks_const_iterator bi (blk.blocks ().begin ());
       bi != blk.blocks ().end (); bi++)
    dump_test_print (*bi);
}

static void
dump_test_print (const blocks_T& blks)
{
  for (blocks_T::block_const_iterator bi (blks.block ().begin ());
       bi != blks.block ().end (); bi++)
    dump_test_print (*bi);
}

int
main (int argc, char* const* argv)
{
  int c;
  const char* xml = NULL;

  while ((c = ::getopt (argc, argv, "vc:b:")) != -1)
    {
      switch (c)
	{
	case 'v':
	  verbose = 1;
	  break;

	case 'b':
	  // dump specified block number only
	  ids.push_back ((int)strtol(optarg, NULL, 0));
	  break;

	case 'c':
	  // generate calls, for sequncial program
	  do_call = (int)strtol(optarg, NULL, 0);
	  if (do_call > 0)
	    {
	      cindent = std::string (do_call, ' ');
	      break;
	    }

	  /* FALTHROUGH */

	default:
	usage:
	  std::cerr << "usage: " << argv[0]
		      << " xml" << std::endl;
	  return 1;
	}
    }

  argv += optind - 1;
  argc -= optind - 1;

  if (argc < 2)
    goto usage;

  xml = argv[1];

  try
    {
#ifdef XSD_CXX11
      std::unique_ptr < blocks_T >
	blks (blocks (xml, xml_schema::flags::dont_validate));
#else
      std::auto_ptr < blocks_T >
	blks (blocks (xml, xml_schema::flags::dont_validate));
#endif

      dump_test_print (*blks);
    }

  catch (const xml_schema::exception& e)
    {
      std::cerr << e << std::endl;
      return 1;
    }

  catch (const std::ostream::failure e)
    {
      std::cerr << e.what () << std::endl;
      return 1;
    }

  catch (...)
    {
      std::cerr << "ERROR" << std::endl;
      return 1;
    }

  return 0;
}
