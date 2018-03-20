// -*- mode: c++; -*-

#ifndef BLXML_H
#define BLXML_H

#include <map>
#include <memory>

namespace SimulinkModel
{
  namespace XSD
  {
    class blocks_T;
    class block_T;
  }

  class BLXML
  {
  public:

    BLXML () : blocks (NULL) {}
    BLXML (const char* file, bool no_validation = false);
    BLXML (std::istream& is, bool no_validation = false);
    BLXML (SimulinkModel::XSD::blocks_T* b) : blocks (b) {}

    ~BLXML ();

    enum file_type { FT_SHIM, FT_SHIM_SCHEMA, FT_XML };
    enum element_type
      {
	ET_COMPONENTSET, ET_MASTERCOMPONENT, ET_COMMONINSTRUCTIONSET
      };

    operator bool () const { return blocks != NULL; }

    operator const SimulinkModel::XSD::blocks_T* () const
    {
      return blocks;
    }

    void set_performance (const std::string& name,
			  const std::string& type,
			  double best, double typical, double worst);

    void set_filename (file_type type, const std::string& name);
    void set_shim_param (element_type type, const std::string& name);

    int output (std::ostream& os);
    int output (const char* file);

  private:
    int load (std::istream& is, bool no_validation = false);
    void process_blocks (SimulinkModel::XSD::blocks_T* blks);

    SimulinkModel::XSD::blocks_T* blocks;

    typedef std::map < std::string, SimulinkModel::XSD::block_T* >
    block_map_T;
    typedef block_map_T::iterator block_map_iterator;
    typedef block_map_T::const_iterator block_map_const_iterator;

    block_map_T block_map;
  };
}

#endif	/* BLXML_H */
