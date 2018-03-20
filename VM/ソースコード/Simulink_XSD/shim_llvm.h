// -*- mode: c++; -*-

#ifndef SHIM_INSN_H
#define SHIM_INSN_H

namespace SHIM
{
  class SystemConfiguration;
  class ComponentSet;

  namespace XML
  {
    class Latency
    {
    public:
      Latency (double b = 0, double t = 0, double w = 0)
	: l_best (b), l_typical (t), l_worst (w)
      {
      }

      ~Latency () {}

      double& best (void) { return l_best; }
      const double& best (void) const { return l_best; }

      double& typical (void) { return l_typical; }
      const double& typical (void) const { return l_typical; }

      double& worst (void) { return l_worst; }
      const double& worst (void) const { return l_worst; }

    private:
      double l_best;
      double l_typical;
      double l_worst;
    };

    typedef std::map < std::string, Latency > latency_map;
    typedef latency_map::iterator latency_map_iterator;
    typedef latency_map::const_iterator latency_map_const_iterator;

    class File
    {
    public:
      latency_map& latencies (void) { return xml_latencies; }
      const latency_map& latencies (void) const { return xml_latencies; }

      File () : xml_sc (NULL) {}
      File (const char* file, const std::string& component,
	    const std::string& master, const std::string& instset,
	    const char* schema);
      ~File ();

      operator bool () const { return xml_sc != NULL; }

      operator const SHIM::SystemConfiguration* () const
      {
	return xml_sc;
      }

      Latency* latency (const std::string &name);
      const Latency* latency (const std::string &name) const;

      const std::string&
      ComponentSet (void) const
      {
	return xml_ComponentSet;
      }

      const std::string&
      MasterComponent (void) const
      {
	return xml_MasterComponent;
      }

      const std::string&
      CommonInstructionSet (void) const
      {
	return xml_CommonInstructinSet;
      }

    private:
      int parse_ComponentSet (const SHIM::ComponentSet& cs,
			      const std::string& component,
			      const std::string& master,
			      const std::string& instset);

      int load_file (std::istream& is,
		     const std::string& component,
		     const std::string& master,
		     const std::string& instset, const char* schema);

      SHIM::SystemConfiguration* xml_sc;

      std::string xml_ComponentSet;
      std::string xml_MasterComponent;
      std::string xml_CommonInstructinSet;

      latency_map xml_latencies;
    };
  }
}

#endif	/* SHIM_INSN_H */
