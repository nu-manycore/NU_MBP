#include <iostream>

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

#include "SimulinkRTW.h"

#ifdef USE_DMALLOC
#include <dmalloc.h>
#endif	// USE_DMALLOC

using namespace std;
using namespace SimulinkModel;
using namespace SimulinkModel::XSD;
using namespace Simulink::RTW;

void
add_test (void)
{
  Blocks bn;

#if 0
  BlockNameEnt be;

  be.set ("<S1>/Product", "AAA/Product");
  bn.blockname_entry().push_back (be);

  be.set ("<S2>/IOport", "AAA/Product/IOport");
  bn.add_blockname_entry(be.blockname(), be);
#else
  bn.add_blockname_entry("<S1>/Product",
			 BlockNameEnt("<S1>/Product", "AAA/Product"));
  bn.add_blockname_entry("<S2>/IOport",
			 BlockNameEnt("<S2>/IOport", "AAA/Product/IOport"));
#endif

  for (blockname_map_const_iterator bi (bn.blockname_entry().begin());
       bi != bn.blockname_entry().end(); bi++)
    {
      const BlockNameEnt& b = bi->second;
      b.dump();
    }

  cout << "================================" << endl;

  Blocks xn (bn);
  for (blockname_map_iterator bi (xn.blockname_entry().begin());
       bi != xn.blockname_entry().end(); bi++)
    {
      BlockNameEnt& b = bi->second;
      b.blockname () = "<SX>/PPP";
      b.dump();
    }
  cout << "================================" << endl;
  for (blockname_map_const_iterator bi (bn.blockname_entry().begin());
       bi != bn.blockname_entry().end(); bi++)
    {
      const BlockNameEnt& b = bi->second;
      b.dump();
    }

}


int
main(int argc, char * const *argv)
{
  int c;
  const char* c_source = NULL;
  const char* c_data = NULL;
  const char* c_header = NULL;
  const char* xml_input = NULL;
  const char* xml_output = NULL;
  int validation = 1;
  int do_add_test = 0;

  while ((c = getopt (argc, argv, "c:d:h:anP:")) != -1)
    {
      switch (c)
	{
	case 'c':
	  c_source = optarg;
	  break;
	case 'd':
	  c_data = optarg;
	  break;
	case 'h':
	  c_header = optarg;
	  break;
	case 'a':
	  do_add_test = 1;
	  break;
	case 'n':
	  validation = 0;
	  break;
	case 'P':		// parse options
	  for (const char* opt = optarg; *opt; opt++)
	    {
	      if (*opt == 'x')
		{
		  // cut down every block (Triggered/Enabled SubSystem)
		  set_parse_option (PARSE_OPTION_CUT_TRIGGERED_SUBSYSTEM);
		  continue;
		}
	      else if (*opt == 'a')
		{
		  // cut down every block (Atomic SubSystem)
		  set_parse_option (PARSE_OPTION_CUT_ATOMIC_SUBSYSTEM);
		  continue;
		}
	      else
		goto usage_exit;
	    }
	  break;

	case '?':
	usage_exit:;
	  std::cerr << "usage: " << argv[0]
		    << " [-n]"
		    << " [-c c_source]"
		    << " [-d c_data]"
		    << " [-h c_header]"
		    << " [-Px]"
		    << " [input-XML]"
		    << " [output-XML]"
		    << std::endl;
	  return 1;
	}
    }

  argv += optind;

  if (*argv)
    {
      xml_input = *argv++;
      if (*argv)
	xml_output = *argv;
    }

#if 1
  Blocks_PTR bn (parse_all_code (xml_input, c_header, c_source, c_data,
				 validation != 0));
  if (!*bn)
    return -1;

  if (bn->blockname_entry().size())
    {
      cout << "-- all system --" << endl;

      for (blockname_map_const_iterator bi (bn->blockname_entry().begin());
	   bi != bn->blockname_entry().end(); bi++)
	{
	  bi->second.dump();
	}
    }

  if (bn->auto_variable_entry().size())
    {
      cout << "-- dump all auto variables --" << endl;

      for (c_variable_map_const_iterator vi (bn->auto_variable_entry().begin());
	   vi != bn->auto_variable_entry().end(); vi++)
	{
	  vi->second.dump();
	}
    }

  if (bn->ext_variable_entry().size())
    {
      cout << "-- dump all external variables --" << endl;

      for (c_variable_map_const_iterator vi (bn->ext_variable_entry().begin());
	   vi != bn->ext_variable_entry().end(); vi++)
	{
	  vi->second.dump();
	}
    }

  if (bn->signal_entry().size())
    {
      cout << "-- dump all signals --" << endl;

      cout << "Block Signal Type=" << bn->signal_type() << std::endl;
      cout << "Block Signal Name=" << bn->signal_name() << std::endl;
      for (c_variable_map_const_iterator vi (bn->signal_entry().begin());
	   vi != bn->signal_entry().end(); vi++)
	{
	  vi->second.dump();
	}

      cout << "-- signal appear order --" << endl;
      for (var_list_const_iterator vi (bn->signal_order().begin());
	   vi != bn->signal_order().end(); vi++)
	{
	  (*vi)->dump();
	}
    }

  if (bn->state_entry().size())
    {
      cout << "-- dump all states --" << endl;

      cout << "State Type=" << bn->state_type() << std::endl;
      cout << "State Name=" << bn->state_name() << std::endl;
      for (c_variable_map_const_iterator vi (bn->state_entry().begin());
	   vi != bn->state_entry().end(); vi++)
	{
	  vi->second.dump();
	}

      cout << "-- state appear order --" << endl;
      for (var_list_const_iterator vi (bn->state_order().begin());
	   vi != bn->state_order().end(); vi++)
	{
	  (*vi)->dump();
	}
    }

  if (bn->param_entry().size())
    {
      cout << "-- dump all params --" << endl;

      cout << "Param Type=" << bn->param_type() << std::endl;
      cout << "Param Name=" << bn->param_name() << std::endl;
      for (c_variable_map_const_iterator vi (bn->param_entry().begin());
	   vi != bn->param_entry().end(); vi++)
	{
	  vi->second.dump();
	}

      cout << "-- param appear order --" << endl;
      for (var_list_const_iterator vi (bn->param_order().begin());
	   vi != bn->param_order().end(); vi++)
	{
	  (*vi)->dump();
	}
    }

  if (bn->constant_param_entry().size())
    {
      cout << "-- dump all constant params --" << endl;

      cout << "Constant Param Type="
	   << bn->constant_param_type() << std::endl;
      cout << "Constant Param Name="
	   << bn->constant_param_name() << std::endl;
      for (c_variable_map_const_iterator
	     vi (bn->constant_param_entry().begin());
	   vi != bn->constant_param_entry().end(); vi++)
	{
	  vi->second.dump();
	}

      cout << "-- constant param appear order --" << endl;
      for (var_list_const_iterator vi (bn->constant_param_order().begin());
	   vi != bn->constant_param_order().end(); vi++)
	{
	  (*vi)->dump();
	}
    }

  cout << "-- dump all --" << endl;
  for (block_map_const_iterator bi (bn->block_map().begin());
       bi != bn->block_map().end(); bi++)
    {
      bi->second.dump();
    }

  if (xml_output)
    bn->out_xml(xml_output);
  else
    bn->out_xml();

#else
  Blocks_PTR bn;

  try
    {
      blocks_T_PTR blks;
      if (xml_input)
	{
	  try {
	    if (validation)
	      blks.reset (blocks (xml_input).release ()); // XML
	    else
	      blks.reset (blocks (xml_input,
				  xml_schema::flags::dont_validate)
			  .release ());
	  }
	  catch (const xml_schema::exception& e)
	    {
	      cerr << e << endl;
	      return 1;
	    }
	  catch (const bad_exception& e)
	    {
	      cerr << "Error Bad Exception" << endl;
	      return -1;
	    }
	  catch (...)
	    {
	      cerr << "Error" << endl;
	      return -1;
	    }
	}
      else
	{
	  if (validation)
	    blks.reset (new blocks_T());
	  else
	    blks.reset (new blocks_T(xml_schema::flags::dont_validate));
	}

      if (c_header)
	{
	  bn = parse_header (c_header, blks); // header

	  if (*bn && xml_input != NULL)
	    bn->set_filename (Blocks::XF_XML, xml_input);

	  cout << "-- all system --" << endl;

	  for (blockname_map_const_iterator bi (bn->blockname_entry().begin());
	       bi != bn->blockname_entry().end(); bi++)
	    {
	      bi->second.dump();
	    }

#if 0
	  try {
	    cout << "-- search <S1> --" << endl;
	    blockname_map_const_iterator bi (bn->blockname_entry().find ("<S1>"));
	    if (bi != bn->blockname_entry().end())
	      {
		bi->second.dump();
	      }
	  }
	  catch (...)
	    {
	      cerr << "Error search <S1>" << endl;
	    }
#endif
	}

      if (*bn && c_source != NULL)
	{
	  parse_code (*bn, c_source); // C Code

#if 0
	  cout << "-- find numShape (auto var) --" << endl;

	  c_variable_map_const_iterator vi (bn->find_auto_var("numShape"));
	  if (vi != bn->auto_variable_entry().end())
	    vi->second.dump();
#endif

	  if (bn->auto_variable_entry().size())
	    {
	      cout << "-- dump all auto variables --" << endl;

	      for (c_variable_map_const_iterator
		     vi (bn->auto_variable_entry().begin());
		   vi != bn->auto_variable_entry().end(); vi++)
		{
		  vi->second.dump();
		}
	    }

	  if (bn->ext_variable_entry().size())
	    {
	      cout << "-- dump all external variables --" << endl;

	      for (c_variable_map_const_iterator
		     vi (bn->ext_variable_entry().begin());
		   vi != bn->ext_variable_entry().end(); vi++)
		{
		  vi->second.dump();
		}
	    }

	  if (bn->state_entry().size())
	    {
	      cout << "-- dump all states --" << endl;

	      cout << "State Type=" << bn->state_type() << std::endl;
	      cout << "State Name=" << bn->state_name() << std::endl;
	      for (c_variable_map_const_iterator vi (bn->state_entry().begin());
		   vi != bn->state_entry().end(); vi++)
		{
		  vi->second.dump();
		}

	      cout << "-- state appear order --" << endl;
	      for (var_list_const_iterator vi (bn->state_order().begin());
		   vi != bn->state_order().end(); vi++)
		{
		  (*vi)->dump();
		}
	    }

	  if (bn->param_entry().size())
	    {
	      cout << "-- dump all params --" << endl;

	      cout << "Param Type=" << bn->param_type() << std::endl;
	      cout << "Param Name=" << bn->param_name() << std::endl;
	      for (c_variable_map_const_iterator
		     vi (bn->param_entry().begin());
		   vi != bn->param_entry().end(); vi++)
		{
		  vi->second.dump();
		}

	      cout << "-- param appear order --" << endl;
	      for (var_list_const_iterator vi (bn->param_order().begin());
		   vi != bn->param_order().end(); vi++)
		{
		  (*vi)->dump();
		}
	    }

	  if (bn->constant_param_entry().size())
	    {
	      cout << "-- dump all constant params --" << endl;

	      cout << "Constant Param Type="
		   << bn->constant_param_type() << std::endl;
	      cout << "Constant Param Name="
		   << bn->constant_param_name() << std::endl;
	      for (c_variable_map_const_iterator
		     vi (bn->constant_param_entry().begin());
		   vi != bn->constant_param_entry().end(); vi++)
		{
		  vi->second.dump();
		}

	      cout << "-- constant param appear order --" << endl;
	      for (var_list_const_iterator
		     vi (bn->constant_param_order().begin());
		   vi != bn->constant_param_order().end(); vi++)
		{
		  (*vi)->dump();
		}
	    }
	}

      if (*bn && c_data != NULL)
	{
	  parse_data (*bn, c_data); // params/constant params
	}

      if (*bn)
	{
	  bn->trace_flow();

	  cout << "-- dump all --" << endl;
	  for (block_map_const_iterator bi (bn->block_map().begin());
	       bi != bn->block_map().end(); bi++)
	    {
	      bi->second.dump();
	    }

	  if (bn->xml())
	    bn->xml_complete();
	}

      if (bn->xml())
	{
	  if (xml_output)
	    bn->out_xml(xml_output);
	  else
	    bn->out_xml();
	}
    }
  catch (...)
    {
      cerr << "Error" << endl;
    }
#endif
  if (do_add_test)
    add_test();

  return 0;
}
