#include <iostream>
#include <fstream>
#include <map>
#include <memory>
#include <sstream>

#include <unistd.h>
#include <limits.h>
#include <string.h>
#include <errno.h>

#include "shim_llvm.h"
#include "shim.hxx"

#if __cplusplus >= 201103L && !defined(XSD_CXX11)
#error "Not defined XSD_CXX11 in shim.hxx, please Re-make it"
#elif __cplusplus < 201103L && defined(XSD_CXX11)
#error "defined XSD_CXX11 in shim.hxx, please Re-make it"
#endif

#ifndef SHIM_XSD_VERSION
#error "undefined SHIM_XSD_VERSION, please set it in Makefile"
#endif

namespace SHIM
{
  namespace XML
  {
    File::File (const char* file,
		const std::string& component, const std::string& master,
		const std::string& instset, const char* schema)
      : xml_sc (NULL)
    {
      std::ifstream ifs (file);
      if (!ifs)
	{
	  std::cerr << "Error: Couldn't open " << file
		    << ": " << strerror (errno) << std::endl;
	  return;
	}

      load_file (ifs, component, master, instset, schema);
    }

    File::~File ()
    {
      if (xml_sc)
	delete xml_sc;
    }

    Latency*
    File::latency (const std::string &name)
    {
      if (!*this)
	return NULL;
      latency_map_iterator i (latencies().find (name));
      if (i == latencies ().end ())
	return NULL;
      return &i->second;
    }

    const Latency*
    File::latency (const std::string &name) const
    {
      if (!*this)
	return NULL;
      latency_map_const_iterator i (latencies().find (name));
      if (i == latencies ().end ())
	return NULL;
      return &i->second;
    }

    int
    File::parse_ComponentSet (const SHIM::ComponentSet& cs,
			      const std::string& component,
			      const std::string& master,
			      const std::string& instset)
    {
      int match_componentset = 0;

      if (component != "")
	{
	  if (cs.name () != component)
	    goto try_nesting_componentset;
	  match_componentset = 1;
	}

      for (SHIM::ComponentSet::MasterComponent_const_iterator
	     mcci (cs.MasterComponent().begin());
	   mcci != cs.MasterComponent().end(); mcci++)
	{
	  const SHIM::MasterComponent &mc = *mcci;

	  if (master != "" && master != mc.name ())
	    continue;

#if SHIM_XSD_VERSION == 20140414
	  for (SHIM::MasterComponent::CommonInstructionSet_const_iterator
		 cisci (mc.CommonInstructionSet().begin());
	       cisci != mc.CommonInstructionSet().end(); cisci++)
	    {
	      const SHIM::CommonInstructionSet &cis = *cisci;

	      SHIM::CommonInstructionSet::name_optional n = cis.name();
	      if (instset != "")
		{
		  if (!n || *n != instset)
		    continue;
		}

	      xml_ComponentSet = cs.name();
	      xml_MasterComponent = mc.name();
	      xml_CommonInstructinSet = *n;

	      int inst_num = 0;
	      for (SHIM::CommonInstructionSet::Instruction_const_iterator
		     ici (cis.Instruction().begin());
		   ici != cis.Instruction().end(); ici++)
		{
		  const SHIM::Instruction &inst = *ici;
		  const SHIM::Latency &lt = inst.Latency ();

		  const SHIM::Instruction::name_optional &nam = inst.name ();
		  const SHIM::Latency::best_optional &bo = lt.best();
		  const SHIM::Latency::typical_optional &to = lt.typical();
		  const SHIM::Latency::worst_optional &wo = lt.worst();

		  std::string inst_name;
		  double best = -1, typical = -1, worst = -1;

		  if (nam)
		    inst_name = *nam;
		  else
		    {
		      std::ostringstream oss;
		      oss << "<none_" << inst_num++ << ">";
		      inst_name = oss.str();
		    }

		  if (to)
		    typical = *to;

		  if (wo)
		    worst = *wo;
		  else
		    worst = typical;

		  if (bo)
		    best = *bo;
		  else
		    best = typical;

#if 0
		  std::cout << inst_name << ":"
			    << best << "," << typical << "," << worst
			    << std::endl;
#endif

		  latencies().insert (std::make_pair (inst_name,
						      Latency (best,
							       typical,
							       worst)));
		}

	      return 1;		// found
	    }
#elif SHIM_XSD_VERSION == 20140908 || SHIM_XSD_VERSION == 20140924 || SHIM_XSD_VERSION >= 20150212
	  const SHIM::MasterComponent::CommonInstructionSet_optional
	    &cis_opt = mc.CommonInstructionSet ();
	  if (cis_opt)
	    {
	      const SHIM::CommonInstructionSet &cis = *cis_opt;
	      const SHIM::CommonInstructionSet::name_type n = cis.name();
	      if (instset != "" && n != instset)
		continue;

	      xml_ComponentSet = cs.name();
	      xml_MasterComponent = mc.name();
	      xml_CommonInstructinSet = n;

	      for (SHIM::CommonInstructionSet::Instruction_const_iterator
		     ici (cis.Instruction().begin());
		   ici != cis.Instruction().end(); ici++)
		{
		  const SHIM::Instruction &inst = *ici;
		  const SHIM::Performance &perform = inst.Performance ();
		  const SHIM::Latency &lt = perform.Latency ();

		  const SHIM::Instruction::name_type
		    &inst_name = inst.name ();
		  const SHIM::Latency::best_optional &bo = lt.best();
		  const SHIM::Latency::typical_type &to = lt.typical();
		  const SHIM::Latency::worst_optional &wo = lt.worst();

		  double best = -1, typical = -1, worst = -1;

		  typical = to;

		  if (wo)
		    worst = *wo;
		  else
		    worst = typical;

		  if (bo)
		    best = *bo;
		  else
		    best = typical;

#if 0
		  std::cout << inst_name << ":"
			    << best << "," << typical << "," << worst
			    << std::endl;
#endif

		  latencies().insert (std::make_pair (inst_name,
						      Latency (best,
							       typical,
							       worst)));
		}

	      return 1;		// found
	    }
#else
#error "unsupported SHIM Schema Version"
#endif	// SHIM_XSD_VERSION
	}

      if (match_componentset)
	return 0;		// not found in target ComponentSet

    try_nesting_componentset:

      // not found instructionset in MasterComponent of this ComponentSet
      // try under nesting ComponentSet
      for (SHIM::ComponentSet::ComponentSet1_const_iterator
	     cs1i (cs.ComponentSet1 ().begin ());
	   cs1i != cs.ComponentSet1 ().end (); cs1i++)
	{
	  const SHIM::ComponentSet& cs1 = *cs1i;
	  if (parse_ComponentSet (cs1, component, master, instset))
	    return 1;
	}

      return 0;
    }


    int
    File::load_file (std::istream& is, const std::string& component,
		     const std::string& master,
		     const std::string& instset, const char* schema)
    {
      try
	{
	  if (schema == NULL || *schema == 0)
	    {
	      xml_schema::flags f = xml_schema::flags::dont_validate;
	      xml_sc = SHIM::SystemConfiguration_ (is, f).release ();
	    }
	  else
	    {
	      xml_schema::properties props;
	      std::string xsd;
	      if (strncmp (schema, "file://", 7) == 0)
		xsd = schema;
	      else if (*schema == '/')
		xsd = "file:///" + std::string (schema);
	      else
		{
		  char cwd[PATH_MAX];
		  if (getcwd (cwd, PATH_MAX) == NULL)
		    {
		      std::cerr << "Error: getcwd: " << strerror (errno)
				<< std::endl;
		      return -1;
		    }
		  xsd = "file:///"
		    + std::string (cwd)
		    + "/"
		    + std::string (schema);
		}
	      props.no_namespace_schema_location (xsd);
	      xml_sc = SHIM::SystemConfiguration_ (is, 0, props).release ();
	    }

	  if (xml_sc == NULL)
	    {
	      std::cerr << "Error: ???" << std::endl;
	      return -1;
	    }
	}

      catch (const xml_schema::exception &e)
	{
	  std::cerr << "Error: " << e << std::endl;
	  return -1;
	}
      catch (...)
	{
	  std::cerr << "Error" << std::endl;
	  return -1;
	}

      const SHIM::ComponentSet &cs = xml_sc->ComponentSet();

      parse_ComponentSet (cs, component, master, instset);

      return 0;
    }
  }
}
