/*
 * block info
 *
 */

#include <unistd.h>

#include <iostream>
#include <fstream>
#include <memory>
#include <sstream>
#include <string>

#include <vector>
#include <map>

#include "SimulinkModel.hxx"
#include "SimulinkModel_ns.h"

#if __cplusplus >= 201103L && !defined(XSD_CXX11)
#error "Not defined XSD_CXX11 in SimulinkModel.hxx, please Re-make it"
#elif __cplusplus < 201103L && defined (XSD_CXX11)
#error "defined XSD_CXX11 in SimulinkModel.hxx, please Re-make it"
#endif

using namespace SimulinkModel;
using namespace SimulinkModel::XSD;

/*
 * CSV Format:
 *  <block name>,<block-id>,<peinfo>[,<bind block>,<bind block-id>]
 */
#define CELL_BLOCK_NAME		0
#define CELL_BLOCK_ID		1

static int first = 1;

static void
process_block_out (std::ostream& os, blocks_T& blks)
{
  for (blocks_T::block_iterator bs (blks.block ().begin ());
       bs != blks.block ().end (); bs++)
    {
      block_T& b = *bs;

      // TODO: SwitchCase/If

      // ignore block has no flow / merged DotProduct / Constant
      if ((b.forward ().size () != 0
	   || b.backward ().size () != 0)
	  && b.merged_to ().size () == 0
	  && b.blocktype () != "Constant")
	{
	  int bid = -1;
	  block_T::id_optional& oid = b.id ();
	  if (oid)
	    bid = *oid;

	  if (first)
	    {
	      first = 0;
	      os << "#<block-name>,<block-id>,<peinfo>"
		"[,<bind block>,<bind block-id>]\r\n";
	    }
	  os << b.name () << ',';
	  if (bid > 0)
	    os << bid;
	  os << "\r\n";
	}

      // not enter Iterator SubSystem
      if (b.blocktype () == "SubSystem")
	{
	  block_T::type_optional to = b.type ();
	  if (to && *to == "iterator")
	    continue;
	}

      for (block_T::blocks_iterator i (b.blocks ().begin ());
	   i != b.blocks ().end (); i++)
	process_block_out (os, *i);
    }
}


int
main (int argc, char* const* argv)
{
  int c;
  const char* inxml = NULL;
  const char* outcsv = NULL;

  while ((c = getopt (argc, argv, "o:")) != -1)
    {
      switch (c)
	{
	case 'o':
	  outcsv = optarg;
	  break;

	default:
	  std::cerr << "usage: " << argv[0]
		    << " [-o csv-file] [input-xml]"
		    << std::endl;
	  return 1;
	}
    }

  argv += optind - 1;
  argc -= optind - 1;

  if (argc > 1)
    inxml = argv[1];

  try
    {
      std::ofstream ofs;
      std::ostream *os;

      if (outcsv)
	{
	  ofs.open (outcsv);
	  os = &ofs;
	}
      else
	os = &std::cout;

      os->exceptions (std::ostream::badbit | std::ostream::failbit);

#if __cplusplus >= 201103L
      std::unique_ptr < blocks_T > blks;
#else
      std::auto_ptr < blocks_T > blks;
#endif

      if (inxml)
	blks.reset (blocks (inxml,
			    xml_schema::flags::dont_validate).release ());
      else
	blks.reset (blocks (std::cin,
			    xml_schema::flags::dont_validate).release ());

      process_block_out (*os, *blks);
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
