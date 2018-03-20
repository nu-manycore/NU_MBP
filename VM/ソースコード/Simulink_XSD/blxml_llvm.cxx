#include <iostream>
#include <fstream>
#include <memory>

#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>
#include <ctype.h>

#include "blxml_llvm.h"
#include "SimulinkModel.hxx"
#include "SimulinkModel_ns.h"

#if __cplusplus >= 201103L && !defined(XSD_CXX11)
#error "Not defined XSD_CXX11 in SimulinkModel.hxx, please Re-make it"
#elif __cplusplus < 201103L && defined (XSD_CXX11)
#error "defined XSD_CXX11 in SimulinkModel.hxx, please Re-make it"
#endif

namespace SimulinkModel
{
  BLXML::BLXML (const char* file, bool no_validation)
    : blocks (NULL)
  {
    std::ifstream ifs (file);
    if (!ifs)
      {
	std::cerr << "Error: Couldn't open " << file
		  << ": " << strerror (errno) << std::endl;
	return;
      }
    load (ifs, no_validation);
  }

  BLXML::BLXML (std::istream& is, bool no_validation)
    : blocks (NULL)
  {
    load (is, no_validation);
  }

  BLXML::~BLXML ()
  {
    if (blocks)
      delete blocks;
  }

  void
  BLXML::set_performance (const std::string& name,
			  const std::string& type,
			  double best, double typical, double worst)
  {
    if (!*this)
      return;

    block_map_iterator i = block_map.find (name);

    if (i == block_map.end())
      return;

    SimulinkModel::XSD::block_T* b = i->second;
    SimulinkModel::XSD::performance_T l (type, best, typical, worst);
    b->performance().push_back (l);
  }


  void
  BLXML::set_filename (file_type type, const std::string& name)
  {
    struct stat st;
    if (stat (name.c_str(), &st) == -1)
      {
	std::cerr << "Couldn't get status " << name
		  << ": " << strerror (errno) << std::endl;
	return;
      }

    struct tm *tmp = localtime (&st.st_mtime);
    if (tmp == NULL)
      {
	std::cerr << "Couldn't convert time " << st.st_mtime
		  << ": " << strerror (errno) << std::endl;
	return;
      }

    char iso8061[64];
    memset(iso8061, 0, sizeof iso8061);
#if defined(__MINGW32__) || defined(__MINGW64__)
    size_t s = strftime (iso8061, sizeof iso8061, "%Y-%m-%dT%H:%M:%S", tmp);
    if (s == 0)
      {
	std::cerr << "Couldn't convert ISO8061 time " << st.st_mtime
		  << " -> " << iso8061 << ": " << strerror (errno)
		  << std::endl;
	return;
      }
    if (timezone == 0)
      {
	iso8061[s] = 'Z';
	iso8061[s + 1] = 0;
      }
    else
      {
	long m;
	if (timezone < 0)
	  m = (-timezone) / 60;
	else
	  m = timezone / 60;
	long h = m / 60;
	m %= 60;
	snprintf (&iso8061[s], sizeof iso8061 - s, "%c%02ld%02ld",
		  timezone > 0 ? '-' : '+', h, m);
      }
#else
    if (strftime (iso8061, sizeof iso8061, "%FT%T%z", tmp) == 0)
      {
	std::cerr << "Couldn't convert time " << st.st_mtime
		  << ": " << strerror (errno) << std::endl;
	return;
      }
#endif

    std::string ftype;
    switch (type)
      {
      case FT_SHIM:
	ftype = "shim";
	break;
      case FT_SHIM_SCHEMA:
	ftype = "shim-schema";
	break;
      case FT_XML:
	ftype = "xml";
	break;
      }

    SimulinkModel::XSD::blocks_T& bs = *blocks;

    // replace
    for (SimulinkModel::XSD::blocks_T::file_iterator fi (bs.file().begin());
	 fi != bs.file().end(); fi++)
      {
	SimulinkModel::XSD::file_T& f = *fi;
	if (f.type() == ftype)
	  {
	    f.name() = name;
	    f.timestamp() = iso8061;
	    f.size() = st.st_size;
	    return;
	  }
      }

    // add
    const SimulinkModel::XSD::file_T nf (ftype, name, iso8061, st.st_size);
    bs.file().push_back(nf);
  }


  void
  BLXML::set_shim_param (element_type type, const std::string& name)
  {
    std::string etype;

    switch (type)
      {
      case ET_COMPONENTSET:
	etype = "componentset";
	break;
      case ET_MASTERCOMPONENT:
	etype = "mastercomponent";
	break;
      case ET_COMMONINSTRUCTIONSET:
	etype = "commoninstructionset";
	break;
      }

    SimulinkModel::XSD::blocks_T& bs = *blocks;

    // replace
    for (SimulinkModel::XSD::blocks_T::shim_params_iterator
	   spi (bs.shim_params().begin());
	 spi != bs.shim_params().end(); spi++)
      {
	SimulinkModel::XSD::shim_params_T& sp = *spi;
	if (sp.element() == etype)
	  {
	    sp.name() = etype;
	    return;
	  }
      }

    // add
    const SimulinkModel::XSD::shim_params_T nsp (etype, name);
    bs.shim_params().push_back (nsp);
  }


  int
  BLXML::output (std::ostream& os)
  {
    if (!*this)
      return -1;

    xml_schema::namespace_infomap map;
    map["sm"].name = SIMULINKMODEL_NS;
    map["sm"].schema = "SimulinkModel.xsd";

    SimulinkModel::XSD::blocks_T &bs = *blocks;
    SimulinkModel::XSD::blocks (os, bs, map);

    return 0;
  }

  int
  BLXML::output (const char* file)
  {
    if (!*this)
      return -1;

    std::ofstream ofs (file);

    if (!ofs)
      {
	std::cerr << "Error: Couldn't open " << file
		  << ": " << strerror (errno) << std::endl;
	return -1;
      }

    return output (ofs);
  }

  void
  BLXML::process_blocks (SimulinkModel::XSD::blocks_T* blks)
  {
    for (SimulinkModel::XSD::blocks_T::block_iterator
	   bi (blks->block().begin());
	 bi != blks->block().end(); bi++)
      {
	SimulinkModel::XSD::block_T &b = *bi;

	b.performance().clear();

	// check & replace invaid character(not alphanumeric) in identifier
	const std::string& orig (b.name ());
	char buf[orig.length() + 1];
	char *p = buf;

	for (const char *q = orig.c_str(); *q; q++)
	  {
	    unsigned char c = *q;
	    if (c == '_' || isalnum (c))
	      *p++ = c;
	    else if (c == '-')	// replace `-' to `_', and ignore others.
	      *p++ = '_';
	  }
	*p = 0;

	block_map.insert (std::make_pair (std::string (buf), &b));

	for (SimulinkModel::XSD::block_T::blocks_iterator
	       i (b.blocks().begin());
	     i != b.blocks().end(); i++)
	  process_blocks (&*i);
      }
  }

  int
  BLXML::load (std::istream& is, bool no_validation)
  {
#if __cplusplus >= 201103L
    std::unique_ptr< SimulinkModel::XSD::blocks_T > bs;
#else
    std::auto_ptr< SimulinkModel::XSD::blocks_T > bs;
#endif

    try
      {
	if (no_validation)
	  bs.reset (SimulinkModel::XSD::blocks (is,
						xml_schema::flags::dont_validate)
		    .release ());
	else
	  bs.reset (SimulinkModel::XSD::blocks (is).release ());
      }
    catch (const xml_schema::error& e)
      {
	std::cerr << "Error: " << e << std::endl;
	return -1;
      }
    catch (...)
      {
	std::cerr << "XML or Schema Error" << std::endl;
	return -1;
      }

#if __cplusplus >= 201103L
    if (!bs)
      return -1;
#else
    if (bs.get () == NULL)
      return -1;
#endif

    process_blocks (bs.get ());

    blocks = bs.release ();

    return 0;
  }
}
