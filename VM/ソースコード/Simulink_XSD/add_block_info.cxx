/*
 * block info
 *
 */

#include <string.h>
#include <errno.h>
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
#define CELL_PEINFO		2
#define CELL_BIND_BLOCK		3
#define CELL_BIND_BLOCK_ID	4

namespace TaskBinding
{
  typedef std::vector < std::string > CsvLine;

  class Csv : public std::vector < CsvLine >
  {
  public:
    void load (std::istream& is);
    void load (const char* f);
  };

  void
  Csv::load (const char* f)
  {
    std::ifstream is (f);
    if (!is)
      {
	std::cerr << "Error: Couldn't open " << f
		  << ": " << strerror (errno) << std::endl;
      }
    load (is);
  }

  void
  Csv::load (std::istream& is)
  {
    try {
      std::string buf;
      while (std::getline (is, buf))
	{
	  if (buf[0] == '#' || buf[0] == 0)
	    continue;

	  std::string::size_type l = buf.find_last_of ('\r');
	  if (l != std::string::npos)
	    buf[l] = 0;

	  std::istringstream st (buf);

	  push_back (CsvLine());
	  CsvLine& c = back ();

	  std::string tmp;
	  while (std::getline (st, tmp, ','))
	    c.push_back (tmp);
	}
    }

    catch (const std::exception& e)
      {
	std::cerr << "Error in " << __FUNCTION__ << ":"
		  << e.what () << std::endl;
      }

    catch (...)
      {
	std::cerr << "Error in " << __FUNCTION__ << std::endl;
      }
  }

  typedef std::map < const std::string,
		     std::pair < int, block_T* > > block_map_T;
  typedef std::vector < block_T* > block_list_T;

  int max_id = -1;

  void
  process_blocks (blocks_T& blks, block_map_T& map)
  {
    for (blocks_T::block_iterator bs (blks.block ().begin ());
	 bs != blks.block ().end (); bs++)
      {
	block_T& b = *bs;
	int bid = -1;
	block_T::id_optional& oid = b.id ();
	if (oid)
	  {
	    bid = *oid;
	    if (bid >= 0 && bid > max_id)
	      max_id = bid;
	  }

	map.insert (std::make_pair (b.name (), std::make_pair (bid, &b)));

	for (block_T::blocks_iterator i (b.blocks ().begin ());
	     i != b.blocks ().end (); i++)
	  process_blocks (*i, map);
      }
  }
}

using namespace TaskBinding;

int
main (int argc, char* const* argv)
{
  int c;
  const char* inx = NULL;
  const char* outx = NULL;

  while ((c = getopt (argc, argv, "i:o:")) != -1)
    {
      switch (c)
	{
	case 'i':
	  inx = optarg;
	  break;
	case 'o':
	  outx = optarg;
	  break;

	default:
	usage:
	  std::cerr << "usage: " << argv[0]
		    << " -i input-xml [-o output-xml] [csv-file]"
		    << std::endl;
	  return 1;
	}
    }

  argv += optind - 1;
  argc -= optind - 1;

  Csv csv;

  if (inx == NULL)
    goto usage;

  try
    {
#ifdef XSD_CXX11
      std::unique_ptr < blocks_T >
	blks (blocks (inx, xml_schema::flags::dont_validate));
#else
      std::auto_ptr < blocks_T >
	blks (blocks (inx, xml_schema::flags::dont_validate));
#endif

      block_map_T block_map;

      process_blocks (*blks, block_map);

      block_list_T block_list;

      if (max_id >= 0)
	{
	  block_list.resize (max_id + 1, NULL);
	  for (block_map_T::iterator i (block_map.begin ());
	       i != block_map.end (); i++)
	    block_list[i->second.first] = i->second.second;
	}

      if (argc > 1)
	csv.load (argv[1]);
      else
	csv.load (std::cin);

      for (Csv::const_iterator i (csv.begin ()); i != csv.end (); i++)
	{
	  const CsvLine& l = *i;

	  std::string bname;
	  std::string pe;
	  std::string bind;

	  int bid = -1;
	  int bind_bid = -1;

	  block_T* b1 = NULL;
	  block_T* b2 = NULL;

	  if (l.size () > CELL_BLOCK_NAME)
	    bname = l[CELL_BLOCK_NAME];

	  if (l.size () > CELL_BLOCK_ID)
	    {
	      if (l[CELL_BLOCK_ID] != "")
		{
		  std::istringstream iss (l[CELL_BLOCK_ID]);
		  iss >> bid;
		}
	    }

	  if (l.size () > CELL_PEINFO)
	    pe = l[CELL_PEINFO];

	  if (l.size () > CELL_BIND_BLOCK)
	    bind = l[CELL_BIND_BLOCK];

	  if (l.size () > CELL_BIND_BLOCK_ID)
	    {
	      if (l[CELL_BIND_BLOCK_ID] != "")
		{
		  std::istringstream iss (l[CELL_BIND_BLOCK_ID]);
		  iss >> bind_bid;
		}
	    }

	  if (bname != "")
	    {
	      block_map_T::iterator x = block_map.find (bname);
	      if (x == block_map.end ())
		{
		  std::cerr << "Error: Block " << bname << ": not found"
			    << std::endl;
		  continue;
		}

	      b1 = x->second.second;
	    }
	  else if (bid != -1
		   && static_cast <size_t> (bid) < block_list.size ())
	    {
	      b1 = block_list[bid];
	      if (b1 == NULL)
		{
		  std::cerr << "Error: BlockID " << bid << ": not found"
			    << std::endl;
		  continue;
		}
	    }
	  else
	    {
	      std::cerr << "Error: Block not specified"
			<< std::endl;
	      continue;
	    }

	  if (bind != "")
	    {
	      block_map_T::iterator x = block_map.find (bind);
	      if (x == block_map.end ())
		{
		  std::cerr << "Error: BindBlock " << bind
			    << ": not found" << std::endl;
		  continue;
		}
	      b2 = x->second.second;
	    }
	  else if (bind_bid != -1
		   && static_cast <size_t> (bind_bid) < block_list.size ())
	    {
	      b2 = block_list[bind_bid];
	      if (b2 == NULL)
		{
		  std::cerr << "Error: Bind BlockID " << bind_bid
			    << ": not found" << std::endl;
		  continue;
		}
	    }

	  if (pe != "")
	    b1->peinfo (pe);

	  if (b2)
	    {
	      b1->bound_to ().push_back (block_name_T (b2->name ()));
	      b2->bind ().push_back (block_name_T (b1->name ()));
	    }
	}

      xml_schema::namespace_infomap map;
      map["sm"].name = SIMULINKMODEL_NS;
      map["sm"].schema = "SimulinkModel.xsd";

      if (outx)
	{
	  std::ofstream os (outx);
	  blocks (os, *blks, map);
	}
      else
	blocks (std::cout, *blks, map);
    }

  catch (const xml_schema::exception& e)
    {
      std::cerr << e << std::endl;
      return 1;
    }

  catch (...)
    {
      std::cerr << "ERROR" << std::endl;
      return 1;
    }

  return 0;
}
