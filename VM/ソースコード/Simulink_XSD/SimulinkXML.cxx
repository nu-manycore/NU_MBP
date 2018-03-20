#include <iostream>
#include <string.h>
#include <assert.h>

#include "SimulinkXML.h"

#ifdef __GNUC__
// TR1 using BOOST Project
// libstdc++-V3 doesn't work
#include <boost/tr1/regex.hpp>
#else
#include <tr1/regex>
#endif

#ifdef USE_DMALLOC
#include <dmalloc.h>
#endif	// USE_DMALLOC

using namespace SimulinkModel;
using namespace SimulinkModel::XSD;

namespace Simulink
{
  namespace XML
  {
    unsigned int base_BlockInfo::seq = 0;

    const char*
    nextline (const char* line, size_t& size)
    {
      const char* end = line + size;
      while (line < end)
	{
	  --size;
	  if (*line++ == '\n')
	    break;
	}
      return line;
    }

    const char*
    comment_check (const char*& beg, const char* end,
		   char& dir, char& type,
		   unsigned int& num, std::string& name)
    {
      try
	{
	  std::tr1::regex mark("^"
			       "/\\*"
			       "(<<<|>>>)"		// %1 -- dir
			       "[[:space:]]"
			       "([ESX])"		// %2 -- type
			       "[[:space:]]"
			       "([[:digit:]]+)"		// %3 -- num
			       "[[:space:]]"
			       "("			// %4 -- block
			       "none"
			       "|"
			       "([_[:alnum:]]+)"	// %5 -- name
			       "[[:space:]]"
			       "'<[^>]+>/[^']+'"
			       ")"			// %4(close)
			       "[[:space:]]"
			       "(<<<|>>>)"		// %6 -- dir
			       "\\*/");

	  std::tr1::cmatch mr;

	  if (std::tr1::regex_search (beg, end, mr, mark))
	    {
	      if (*mr[1].first != *mr[6].first)
		return NULL;
	      dir = *mr[1].first;
	      type = *mr[2].first;
	      std::istringstream ss (mr[3]);
	      ss >> num;
	      if (mr[4] == "none")
		name = mr[4];
	      else
		name = mr[5];

	      beg = mr[0].first; // begin of match

	      const char* p = mr[0].second; // end of match
	      size_t s = end - p;
	      return nextline (p, s);
	    }
	}

      catch (const std::tr1::regex_error& e)
	{
	  std::cerr << "Error in " << __FUNCTION__ << ":"
		    << e.what() << std::endl;
	}
      catch (const std::bad_exception& e)
	{
	  std::cerr << "Error in " << __FUNCTION__ << std::endl;
	}
      catch (...)
	{
	  std::cerr << "Error in " << __FUNCTION__ << std::endl;
	}

      return NULL;
    }

    const char*
    cut_code (const char*& cbeg, const char* cend,
	      const char*& tbeg, const char*& tend,
	      char& type, unsigned int& num, std::string& block)
    {
      char xd;

      const char* n = comment_check (cbeg, cend, xd, type, num, block);
      if (n == NULL || xd != '<')
	return NULL;

      tbeg = n;			// after comment
      const char* r = n;

      while (r < cend)
	{
	  char xt;
	  unsigned int xn;
	  std::string xb;

	  n = comment_check (r, cend, xd, xt, xn, xb);
	  if (type == 'X')
	    {
	      if (n)
		{
		  tend = r;	// before comment
		  return r;	// before comment
		}
	      else
		{
		  tend = cend;	// end
		  return cend;	// end
		}
	    }
	  else if (n != NULL
		   && (xd == '>' && xt == type && xn == num && xb == block))
	    {
	      tend = r;		// before comment
	      return n;		// skip comment
	    }

	  break;
	}

      return NULL;
    }

    void
    codelist::process (const char* beg, const char* end)
    {
      while (beg < end)
	{
	  char type;
	  unsigned int num;
	  std::string block;
	  const char* tbeg = NULL;
	  const char* tend = NULL;
	  const char* e = cut_code (beg, end, tbeg, tend, type, num, block);
	  if (e == NULL)
	    break;
	  list.push_back (c_mark (beg, e, tbeg, tend, type, num, block));
	  beg = e;
	}
    }

    IOPort::IOPort ()
      : iop_block_tag (NULL), iop_ioport_tag (NULL)
    {
    }

    IOPort::IOPort (block_T* block, ioport_T* ioport)
      : iop_block_tag (block), iop_ioport_tag (ioport)
    {
    }

    void
    IOPort::dump (void) const
    {
      std::cout << "block_T " << block_tag()->name() << ":"
		<< ioport_tag()->port() << std::endl;
    }

    IOPort&
    base_Reader::add_input_map (block_T* block, ioport_T* ioport)
    {
      ioport_map_iterator
	x = r_input_map.insert (std::make_pair (ioport->port(),
						IOPort (block, ioport)));
      return x->second;
    }

    IOPort&
    base_Reader::add_output_map (block_T* block, ioport_T* ioport)
    {
      ioport_map_iterator
	x = r_output_map.insert (std::make_pair (ioport->port(),
						 IOPort (block, ioport)));
      return x->second;
    }

    const IOPort*
    base_Reader::find_input_port (const std::string& block,
				  const std::string& port) const
    {
      ioport_map_const_iterator_pair pi = r_input_map.equal_range (port);
      for (ioport_map_const_iterator i = pi.first; i != pi.second; i++)
	{
	  if (i->second.match (block, port))
	    return &i->second;
	}
      return NULL;
    }

    IOPort*
    base_Reader::find_input_port (const std::string& block,
				  const std::string& port)
    {
      ioport_map_iterator_pair pi = r_input_map.equal_range (port);
      for (ioport_map_iterator i = pi.first; i != pi.second; i++)
	{
	  if (i->second.match (block, port))
	    return &i->second;
	}
      return NULL;
    }

    const IOPort*
    base_Reader::find_output_port (const std::string& block,
				   const std::string& port) const
    {
      ioport_map_const_iterator_pair pi (r_output_map.equal_range (port));
      for (ioport_map_const_iterator i = pi.first; i != pi.second; i++)
	{
	  if (i->second.match (block, port))
	    return &i->second;
	}
      return NULL;
    }

    IOPort*
    base_Reader::find_output_port (const std::string& block,
				   const std::string& port)
    {
      ioport_map_iterator_pair pi (r_output_map.equal_range (port));
      for (ioport_map_iterator i = pi.first; i != pi.second; i++)
	{
	  if (i->second.match (block, port))
	    return &i->second;
	}
      return NULL;
    }

    void
    base_Reader::process_port (blocks_T& blocks)
    {
      for (blocks_T::block_iterator bs (blocks.block().begin());
	   bs != blocks.block().end(); bs++)
	{
	  for (block_T::input_iterator i (bs->input().begin());
	       i != bs->input().end(); i++)
	    add_input_map (&*bs, &*i);

	  for (block_T::output_iterator i (bs->output().begin());
	       i != bs->output().end(); i++)
	    add_output_map (&*bs, &*i);

	  for (block_T::blocks_iterator i (bs->blocks().begin());
	       i != bs->blocks().end(); i++)
	    process_port (*i);
	}
    }
  }
}
