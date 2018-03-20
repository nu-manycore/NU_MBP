#include <unistd.h>

#include <iostream>
#include <fstream>
#include <memory>

#include "SimulinkModel.hxx"
#include "SimulinkModel_ns.h"

#if __cplusplus >= 201103L && !defined(XSD_CXX11)
#error "Not defined XSD_CXX11 in SimulinkModel.hxx, please Re-make it"
#elif __cplusplus < 201103L && defined (XSD_CXX11)
#error "defined XSD_CXX11 in SimulinkModel.hxx, please Re-make it"
#endif

using namespace std;
using namespace SimulinkModel;
using namespace SimulinkModel::XSD;

void
process_blocks (const blocks_T& blks, string path, string ind)
{
  cout << ind << "<<" << path << ">>" << endl;

  for (blocks_T::block_const_iterator bs (blks.block().begin ());
       bs != blks.block().end(); bs++)
    {
      if (bs->blocks().size())
	cout << ind << bs->blocktype() << "::" << bs->name() << " [Subsystem]";
      else
	cout << ind << bs->blocktype() << "::" << bs->name();

      const block_T::rate_optional& r = bs->rate();

      if (r)
	cout << " [rate=" << r << "]";
      cout << endl;

      for (block_T::input_const_iterator ii (bs->input().begin ());
	   ii != bs->input().end(); ii++)
	{
	  cout << ind << "    input line=" << ii->line()
	       << ", port=" << ii->port() << endl;
	  for (ioport_T::connect_const_iterator ci (ii->connect().begin());
	       ci != ii->connect().end(); ci++)
	    {
	      cout << ind << "      connect block="  << ci->block()
		   << ", port=" << ci->port() << endl;
	    }
	}

      for (block_T::output_const_iterator oo (bs->output().begin ());
	   oo != bs->output().end(); oo++)
	{
	  cout << ind << "    output line=" << oo->line()
	       << ", port=" << oo->port() << endl;
	  for (ioport_T::connect_const_iterator ci (oo->connect().begin());
	       ci != oo->connect().end(); ci++)
	    {
	      cout << ind << "      connect block="  << ci->block()
		   << ", port=" << ci->port() << endl;
	    }
	}

      for (block_T::code_const_iterator cc (bs->code().begin ());
	   cc != bs->code().end(); cc++)
	{
	  cout << ind << "    " << "code (type="
	       << cc->type()
	       << ")\n<<<<<<<<\n"
	       << *cc
	       << ">>>>>>>>" << endl;
	}

      for (block_T::var_const_iterator vv (bs->var().begin ());
	   vv != bs->var().end(); vv++)
	{
	  cout << ind << "    var name=" << vv->name()
	       << ", type=" << vv->type()
	       << ", mode=" << vv->mode() << endl;
	}

      for (block_T::param_const_iterator pp (bs->param().begin ());
	   pp != bs->param().end(); pp++)
	{
	  cout << ind << "    param name=" << pp->name()
	       << ", type=" << pp->type()
	       << ", storage=" << pp->storage()  << endl;
	}

      for (block_T::blocks_const_iterator bi (bs->blocks().begin ());
	   bi != bs->blocks().end(); bi++)
	{
	  const blocks_T::name_optional& n = bi->name();
	  if (n)
	    process_blocks (*bi, path + "/" + *n, ind + "--");
	  else
	    process_blocks (*bi, path + "/" + "???", ind + "--");
	}
    }
}


void
create_blocks (const char* file)
{
  blocks_T sb;
  sb.name("zzz");

  // Subblock

  ioport_T di("ZZZ_Line1","ZZZ_Port1");
  di.connect().push_back (connection_T("zzz_b1", "zzz_p1"));
  di.connect().push_back (connection_T("zzz_b2", "zzz_p2"));
  {
    block_T b("ZZZBlockType", "ZZZ");
    b.input().push_back (di);
    sb.block().push_back(b);
  }

  block_T b("DummyBlockType", "Dummy");

  b.blocks().push_back(sb);

  di.line ("Dummy_Line1");
  di.port ("Dummy_Port1");

  di.connect().clear();
  di.connect().push_back (connection_T("dummy_b1", "dummy_p1"));
  di.connect().push_back (connection_T("dummy_b2", "dummy_p2"));

  b.input().push_back (di);

  code_T cc("if (x > y)\n  x = y;");
  cc.type("task");
  b.code().push_back(cc);

  b.var().push_back (var_T("In1", "real_T", "extin"));

  blocks_T bs;
  bs.name("dummy");
  bs.block().push_back(b);


  xml_schema::namespace_infomap map;
  map["SM"].name = SIMULINKMODEL_NS;
  map["SM"].schema = "SimulinkModel.xsd";

  ofstream of (file);

  blocks (of, bs, map);
}


int
main (int argc, char *argv[])
{
  int c;
  bool validation = true;

  while ((c = getopt (argc, argv, "n")) != -1)
    {
      switch (c)
	{
	case 'n':
	  validation = false;
	  break;
	case '?':
	usage:
	  cerr << "Usage: "
	       << argv[0] << " [-n] input.xml [testout.xml]"
	       << endl;
	  return 1;
	}
    }

  if (optind >= argc)
    goto usage;

  try
    {
#if __cplusplus >= 201103L
      unique_ptr<blocks_T> blks;
#else
      auto_ptr<blocks_T> blks;
#endif

      if (validation)
	blks.reset (blocks (argv[optind]).release ());
      else
	blks.reset (blocks (argv[optind], xml_schema::flags::dont_validate)
		    .release ());

      const blocks_T::name_optional& n = blks->name();
      if (n)
	process_blocks (*blks, *n, "");
      else
	process_blocks (*blks, "<Root>", "");

      if (optind + 1 < argc)
	create_blocks (argv[optind + 1]);
    }

  catch (const xml_schema::exception& e)
    {
      cerr << e << endl;
      return 1;
    }

  catch (...)
    {
      cerr << "ERROR" << endl;
      return 1;
    }

  return 0;
}
