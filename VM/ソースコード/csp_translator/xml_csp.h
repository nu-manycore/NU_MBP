// -*- mode: c++; coding: utf-8-unix; -*-
#ifndef XML_CSP_H
#define XML_CSP_H

/// \file xml_csp.h
///
/// \brief CSP内部データを構成するための型、クラスの宣言

/// \addtogroup CSP_TRANSLATOR
///
/// @{

/// \defgroup XMLCSP CSP内部データ生成
///
/// \brief BLXMLからCSP内部データを生成するモジュール
///
/// - BLXMLの読み込み\n
///   Simulink::CspTrans::CspReader::input_xml()
/// - Dotファイルの出力\n
///   Simulink::CspTrans::CspReader::output_csp()
///
/// BLXMLからCSP内部データを作成するが、それ以外にBLXMLにある、
/// ブロック以外のコードについても処理し、それらはCコード出力用の
/// Simulink::CspTrans::C_CodeWriter
/// に様々な情報を設定する
///
/// \sa Simulink::CspTrans::CspReader::input_xml()
/// \sa Simulink::CspTrans::CspReader::output_csp()
/// \sa Simulink::CspTrans::C_CodeWriter
///
/// @{

/// \defgroup TASK_INFO BlockInfoのタスク属性
///
/// \brief BlockInfoに設定するタスク属性
///
/// \sa Simulink::CspTrans::base_CspBlockInfo::task_info()
///
/// @{

/// \brief コードを持たない
#define TASK_INFO_NOCODE		0x00000001

/// \brief 定数(ブロックはConstant)
#define TASK_INFO_CONSTANT		0x00000002

/// \brief 後続ブロックなし
#define TASK_INFO_ZFORW			0x00000004

/// \brief 後続ブロックは一つ
#define TASK_INFO_1FORW			0x00000008

/// \brief 先行ブロックなし
#define TASK_INFO_ZBACKW		0x00000010

/// \brief 先行ブロックは一つ
#define TASK_INFO_1BACKW		0x00000020

/// \brief ブロックは外部入力を持つ
#define TASK_INFO_EXTIN			0x00000040

/// \brief ブロックは外部出力を持つ
#define TASK_INFO_EXTOUT		0x00000080

/// \brief ブロックはUnitDelay等のUPDATE部を持つ
#define TASK_INFO_HAS_UPDATE		0x00000100

/// \brief ブロックはS-Functionである
#define TASK_INFO_SFUNCTION		0x00000200

/// \brief ブロックはIterator SubSystem等のトップブロック(未使用)
#define TASK_INFO_CODE_TOP		0x00000400 // top IteratorSubSystem

/// \brief ブロックのコードは他のブロックに含まれている(未使用)
#define TASK_INFO_CONTAINED		0x00000800 // included, no code

/// \brief ブロックのコードは他のブロックと統合されている
/// (BLXMLでmerged_toが指定されているブロック)
#define TASK_INFO_MERGED		0x00001000 // eg "Merge", "DotProduct"

/// \brief ブロックは他のブロックをマージしている
/// (BLXMLでmerge_blockを持つブロック)
#define TASK_INFO_MERGE_HEAD		0x00002000

/// \brief ブロックの結合が指示されている先頭ブロック
/// (BLXMLでbind_blocksを持つブロック)
#define TASK_INFO_BIND_HEAD		0x00004000

/// \brief 結合可能なブロック
#define TASK_INFO_CONCATENATABLE	0x00010000

/// \brief ブロックを結合する際、先頭になれないブロック
#define TASK_INFO_NO_CONCAT_BASE	0x00020000

/// \brief 結合できないブロック
#define TASK_INFO_NOT_CONCAT		0x00040000

/// \brief 低レート化可能なPulseGenerator
#define TASK_INFO_LOWRATE_PULSEGEN	0x00100000

/// \brief DataStoreMemoryのブロック
#define TASK_INFO_HAS_DATASTOREMEMORY	0x00200000

/// \brief DataStoreReadまたはDataStoreWriteのブロック
#define TASK_INFO_USE_DATASTOREMEMORY	0x00400000

/// \brief 状態を持つSubSystemをマージしているブロック
#define TASK_INFO_MERGE_STATE_SUBSYS	0x00800000

/// \brief 条件付きSubSystemの初期値を持つVirtualOutportStartブロック
#define TASK_INFO_VIRTUALOUTPORT_START	0x01000000

/// \brief 低レートから高レートへの単位遅延のRateTransitionのブロック
#define TASK_INFO_DLY_RATETRANSITION	0x02000000

/// \brief コード生成に影響せず、無視可能なブロック
#define TASK_INFO_IGNORE_BLOCK		0x08000000 // e.g. toWorkspace

/// \brief CSPノードとして作成されたブロック
#define TASK_INFO_NODE_CREATED		0x80000000 // created CSP Node

/// @}

/// \defgroup VAR_MODE VAR_MODE属性
///
/// \brief 変数の属性
///
/// \sa Simulink::CspTrans::base_CspVar::mode()
///
/// @{

/// \brief 属性なし(初期値)
#define VAR_MODE_NONE			0x00000000

/// \brief 入力変数
#define VAR_MODE_IN			0x00000001

/// \brief 出力変数
#define VAR_MODE_OUT			0x00000002

/// \brief 入力かつ出力変数
#define VAR_MODE_BOTH			0x00000003

/// \brief UPDATE部へ入力する変数
#define VAR_MODE_UPDATE			0x00000004

/// \brief Demuxにより分割された配列
#define VAR_MODE_PARTIAL_ARRAY		0x00000008

/// \brief レートが異なる入力変数
#define VAR_MODE_MULTIRATE		0x00000010

/// \brief 使用しない変数(未使用)
#define VAR_MODE_NODEF_VAR		0x00000020 // not used

/// \brief 他のSubSystemからの入力(未使用)
#define VAR_MODE_OTHER_SUBSYSTEM	0x00000040 // not used

/// \brief 自動変数
#define VAR_MODE_AUTO			0x00010000

/// \brief 外部入力変数
#define VAR_MODE_EXTIN			0x00020000

/// \brief 外部出力変数
#define VAR_MODE_EXTOUT			0x00040000

/// \brief 構造体になっているシグナル
#define VAR_MODE_BIOSIG			0x00080000

/// \brief DataStoreMemoryの変数
#define VAR_MODE_DSMEM			0x00100000

/// \brief 浮動小数点型の変数
#define VAR_MODE_REAL			0x10000000

/// \brief 複素数(浮動小数点型)の変数
#define VAR_MODE_CREAL			0x20000000

/// @}

/// \defgroup CSP_NODE_INFO CSPノード属性
///
/// \brief CSPノードのタスク属性
///
/// @{

/// \brief 属性なし(初期値)
#define CSP_NODE_INFO_NONE			0x00000000

/// \brief UPDATE部への入力がある
#define CSP_NODE_INFO_HAS_UPDATE		0x00000001

/// \brief 入力変数はUPDATE部のものだけ
#define CSP_NODE_INFO_INVAR_UPDATE		0x00000002

/// \brief (Atomicなど)単一のSubSystem
#define CSP_NODE_INFO_ONE_SUBSYSTEM		0x00000004

/// \brief 低レート化するPulse Generatorブロック
#define CSP_NODE_INFO_LOWRATE_PULSEGEN		0x00000008

/// \brief 可変レートを持つ
#define CSP_NODE_INFO_CHANGE_RATE		0x00000010

/// \brief Real-Time Model Objectを必要とする
#define CSP_NODE_INFO_NEED_RTM			0x00000020

/// \brief DataStoreMemoryにアクセスする
#define CSP_NODE_INFO_HAS_DSMEM			0x00000040

/// \brief 外部入力を持つ
#define CSP_NODE_INFO_EXTIN			0x00000080

/// \brief 外部出力を持つ
#define CSP_NODE_INFO_EXTOUT			0x00000100

/// \brief task0(全体同期タスク)として生成されたノード
#define CSP_NODE_INFO_TASK_0			0x00000200

/// \brief 外部出力同期用に生成されたノード
#define CSP_NODE_INFO_TASK_EXTOUT		0x00000400

/// \brief レートが切り替わる先頭のノード
#define CSP_NODE_INFO_MULTIRATE_HEAD		0x00000800

/// \brief Inportブロックのノード
#define CSP_NODE_INFO_INPORT			0x00001000

/// \brief Outportブロックのノード
#define CSP_NODE_INFO_OUTPORT			0x00002000

/// \brief Switch-Caseブロックのノード
#define CSP_NODE_INFO_SWITCHCASE		0x00004000

/// \brief Ifブロックのノード
#define CSP_NODE_INFO_IF			0x00008000

/// \brief Atomic SubSystemのノード
#define CSP_NODE_INFO_ATOMIC_SUBSYSTEM		0x00010000

/// \brief Iterator SubSystemのノード
#define CSP_NODE_INFO_ITERATOR_SUBSYSTEM	0x00020000

/// \brief StateFlow Subsystemのノード
#define CSP_NODE_INFO_STATEFLOW_SUBSYSTEM	0x00040000

/// \brief Action SubSystemのノード
#define CSP_NODE_INFO_ACTION_SUBSYSTEM		0x00080000

/// \brief Enabled SubSystemのノード
#define CSP_NODE_INFO_ENABLED_SUBSYSTEM		0x00100000

/// \brief Triggered SubSystemのノード
#define CSP_NODE_INFO_TRIGGERED_SUBSYSTEM	0x00200000

/// \brief 条件付きSubSystemの初期値を送信するノード
#define CSP_NODE_INFO_SEND_INITVAL		0x00400000

/// \brief 条件付きSubSystemの初期値を受信するノード
#define CSP_NODE_INFO_RECV_INITVAL		0x00800000

/// \brief Constant Parameterの初期値が設定されているノード
#define CSP_NODE_INFO_CPARAM_INITVALUE_OK	0x01000000

/// \brief Invariant Block Signalの初期値が設定されているノード
#define CSP_NODE_INFO_INVSIG_INITVALUE_OK	0x02000000

/// \brief Parameterの初期値が設定されているノード
#define CSP_NODE_INFO_PARAM_INITVALUE_OK	0x04000000

/// \brief RateTransitionのノード
#define CSP_NODE_INFO_DLY_RATETRANSITION	0x08000000

/// @}

/// \defgroup CSP_EDGE_INFO CSPエッジ属性
///
/// \brief CSPエッジの属性
///
/// \sa Simulink::CspTrans::CspEdge::info()
///
/// @{

/// \brief 属性なし(初期値)
#define CSP_EDGE_INFO_NONE			0x00000000

/// \brief Triggered/Enabled SubSystemからのイベントを持つ
#define CSP_EDGE_INFO_HAS_SUBSYSTEM_EVENT	0x00000001

/// \brief UPDATE部への入力を持つ
#define CSP_EDGE_INFO_HAS_UPDATE		0x00000002

/// \brief 同期イベントを持つ(未使用)
#define CSP_EDGE_INFO_SYNC_EVENT		0x00000004

/// \brief 非同期入力を持つ
#define CSP_EDGE_INFO_ASYNC_INPUT		0x00000008

/// \brief 条件付きSubSystemの初期値送受信を行う
#define CSP_EDGE_INFO_INITIAL_VALUE		0x00000010

/// \brief ダブルバッファを使用(機種依存)
#define CSP_EDGE_INFO_DOUBLE_BUFFER		0x00000020

/// \brief ダブルバッファで初期値送信を実行
#define CSP_EDGE_INFO_DOUBLE_BUFFER_INIT_SEND	0x00000040

/// @}

#include <iostream>

#include "c_codewriter.h"

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/assign/list_of.hpp>

/// @}

/// @}

/// \brief Boostの名前空間への追加定義
namespace boost
{
  /// \addtogroup XMLCSP
  ///
  /// @{

  /// \defgroup BGL_PROP BGLプロパティ
  ///
  /// \brief Boost Graph Libraryのプロパティの拡張
  ///
  /// ここで定義されたものはBGLグラフの設定で使用される
  ///
  /// \sa BGL_SETTING
  ///
  /// @{

  /// \brief vartexの CspNode のプロパティ識別
  enum vertex_CspNode_t { vertex_CspNode };

  /// \brief vertexのBlock番号のプロパティ識別
  enum vertex_BlockID_t { vertex_BlockID };

  /// \brief vertexのShapeのプロパティ識別
  enum vertex_Shape_t { vertex_Shape };

  /// \brief vertexのラベル用プロパティ識別
  enum vertex_Label_t { vertex_Label };

  /// \brief vertexのStyle用プロパティ識別
  enum vertex_Style_t { vertex_Style };

  /// \brief vertexのFillColor用プロパティ識別
  enum vertex_FillColor_t { vertex_FillColor };

  /// \brief vertexのFontColor用プロパティ識別
  enum vertex_FontColor_t { vertex_FontColor };

  /// \brief edgeの CspEdge のプロパティ識別
  enum edge_CspEdge_t { edge_CspEdge };

  /// \brief edgeのStyle用プロパティ識別
  enum edge_Style_t { edge_Style };

  /// \brief edgeのラベル用プロパティ識別
  enum edge_Label_t { edge_Label };

  /// \brief vartexの CspNode のプロパティ
  BOOST_INSTALL_PROPERTY (vertex, CspNode);

  /// \brief vertexのBlock番号のプロパティ
  BOOST_INSTALL_PROPERTY (vertex, BlockID);

  /// \brief vertexのShapeのプロパティ
  BOOST_INSTALL_PROPERTY (vertex, Shape);

  /// \brief vertexのラベル用プロパティ
  BOOST_INSTALL_PROPERTY (vertex, Label);

  /// \brief vertexのStyle用プロパティ
  BOOST_INSTALL_PROPERTY (vertex, Style);

  /// \brief vertexのFillColor用プロパティ
  BOOST_INSTALL_PROPERTY (vertex, FillColor);

  /// \brief vertexのFontColor用プロパティ
  BOOST_INSTALL_PROPERTY (vertex, FontColor);

  /// \brief edgeの CspEdge のプロパティ
  BOOST_INSTALL_PROPERTY (edge, CspEdge);

  /// \brief edgeのStyle用プロパティ
  BOOST_INSTALL_PROPERTY (edge, Style);

  /// \brief edgeのラベル用プロパティ
  BOOST_INSTALL_PROPERTY (edge, Label);

  /// @}

  /// @}
}

/// \brief Simulinkに関連するツールの名前空間
namespace Simulink
{
  /// \brief CSPトランスレータの名前空間
  namespace CspTrans
  {
    /// \addtogroup XMLCSP
    ///
    /// @{

    /// \defgroup BGL_SETTING Boost Graph Libraryの設定
    ///
    /// \brief BGL用定義
    ///
    /// \sa BGL_PROP
    ///
    /// @{

    /// \brief Edgeプロパティ
    typedef boost::property <
      boost::edge_color_t, std::string,
      boost::property <
	boost::edge_CspEdge_t, CspEdge*,
	boost::property <
	  boost::edge_Style_t, std::string,
	  boost::property <
	    boost::edge_Label_t, std::string
	    > > > > CspEdge_property;

    /// \brief Vertexプロパティ
    typedef boost::property <
      boost::vertex_name_t, std::string,
      boost::property <
	boost::vertex_color_t, std::string,
	boost::property <
	  boost::vertex_CspNode_t, CspNode*,
	  boost::property <
	    boost::vertex_BlockID_t, long,
	    boost::property <
	      boost::vertex_Shape_t, std::string,
	      boost::property <
		boost::vertex_Label_t, std::string,
		boost::property <
		  boost::vertex_Style_t, std::string,
		  boost::property <
		    boost::vertex_FillColor_t, std::string,
		    boost::property <
		      boost::vertex_FontColor_t, std::string
		      > > > > > > > > > CspVertex_property;

    /// \brief Graphプロパティ
    typedef boost::property <
      boost::graph_name_t, std::string > CspGraph_property;

    /// \brief Graphの型
    typedef boost::adjacency_list <
      boost::listS, boost::listS, boost::bidirectionalS,
      CspVertex_property, CspEdge_property, CspGraph_property > CspGraph;

    /// \brief Vertexの型
    typedef boost::graph_traits < CspGraph >::vertex_descriptor CspGraphVertex;

    /// \brief Edgeの型
    typedef boost::graph_traits < CspGraph >::edge_descriptor CspGraphEdge;

    /// \brief Vertexの反復子
    typedef boost::graph_traits < CspGraph >::vertex_iterator
    CspGraphVertexIterator;

    /// \brief Edgeの反復子
    typedef boost::graph_traits < CspGraph >::edge_iterator
    CspGraphEdgeIterator;

    /// \brief Vertexから出るEdgeの反復子
    typedef boost::graph_traits < CspGraph >::out_edge_iterator
    CspGraphOutEdgeIterator;

    /// \brief Vertexに入るEdgeの反復子
    typedef boost::graph_traits < CspGraph >::in_edge_iterator
    CspGraphInEdgeIterator;

    /// \brief BGLのgraphvizのvertexのラベル用ライタクラス
    template <class G>
    class label_writer {
    public:
      /// \brief label_writer コンストラクタ
      ///
      /// \param[in] g グラフ
      label_writer(const G& g) : graph(g) {}

      /// \brief ラベル出力用叙述関数
      ///
      /// \param[in] out 出力ストリーム
      /// \param[in] v vertex
      template <class V>
      void operator()(std::ostream& out, const V& v) const {
	bool comma = false;
	const std::string& label = boost::get(boost::vertex_Label, graph, v);
	const std::string& shape = boost::get(boost::vertex_Shape, graph, v);
	const std::string& color = boost::get(boost::vertex_color, graph, v);
	const std::string& style = boost::get(boost::vertex_Style, graph, v);
	const std::string&
	  fillcolor = boost::get(boost::vertex_FillColor, graph, v);
	const std::string&
	  fontcolor = boost::get(boost::vertex_FontColor, graph, v);

	if (label.length () == 0
	    && shape.length () == 0
	    && color.length () == 0
	    && style.length () == 0
	    && fillcolor.length () == 0
	    && fontcolor.length () == 0)
	  return;

	out << '[';

	if (label.length ())
	  {
	    out << "label=";

#if BOOST_VERSION >= 104400
	    out << ::boost::escape_dot_string (label);
#else
	    out << "\"" << label << "\"";
#endif
	    comma = true;
	  }

	if (shape.length ())
	  {
	    if (comma)
	      out << ',';
	    else
	      comma = true;

	    out << "shape=";
#if BOOST_VERSION >= 104400
	    out << ::boost::escape_dot_string (shape);
#else
	    out << "\"" << shape << "\"";
#endif
	  }

	if (color.length ())
	  {
	    if (comma)
	      out << ',';
	    else
	      comma = true;

	    out << "color=";
#if BOOST_VERSION >= 104400
	    out << ::boost::escape_dot_string (color);
#else
	    out << "\"" << color << "\"";
#endif
	  }

	if (style.length ())
	  {
	    if (comma)
	      out << ',';
	    else
	      comma = true;

	    out << "style=";
#if BOOST_VERSION >= 104400
	    out << ::boost::escape_dot_string (style);
#else
	    out << "\"" << style << "\"";
#endif
	  }

	if (fillcolor.length ())
	  {
	    if (comma)
	      out << ',';
	    else
	      comma = true;

	    out << "fillcolor=";
#if BOOST_VERSION >= 104400
	    out << ::boost::escape_dot_string (fillcolor);
#else
	    out << "\"" << fillcolor << "\"";
#endif
	  }

	if (fontcolor.length ())
	  {
	    if (comma)
	      out << ',';
	    else
	      comma = true;

	    out << "fontcolor=";
#if BOOST_VERSION >= 104400
	    out << ::boost::escape_dot_string (fontcolor);
#else
	    out << "\"" << fontcolor << "\"";
#endif
	  }

	out << ']';
      }

    private:
      /// \brief グラフ記述子のリファレンス
      const G& graph;
    };

    /// \brief BGLのgraphvizのedge用ライタクラス
    template <class G>
    class edge_writer {
    public:
      /// \brief edge_write コンストラクタ
      ///
      /// \param[in] g グラフ
      edge_writer(const G& g) : graph(g) {}

      /// \brief エッジのラベル出力用叙述関数
      ///
      /// \param[in] out 出力ストリーム
      /// \param[in] e エッジ
      template <class E>
      void operator()(std::ostream& out, const E& e) const {
	const std::string& label = boost::get(boost::edge_Label, graph, e);
	const std::string& color = boost::get(boost::edge_color, graph, e);
	const std::string& style = boost::get(boost::edge_Style, graph, e);
	bool comma = false;

	if (label.length () == 0
	    && color.length () == 0
	    && style.length () == 0)
	  return;

	out << '[';

	if (label.length ())
	  {
	    out << "label=";
#if BOOST_VERSION >= 104400
	    out << ::boost::escape_dot_string (label);
#else
	    out << "\"" << label << "\"";
#endif
	    comma = true;
	  }

	if (color.length ())
	  {
	    if (comma)
	      out << ',';
	    else
	      comma = true;
	    out << "color=";
#if BOOST_VERSION >= 104400
	    out << ::boost::escape_dot_string (color);
#else
	    out << "\"" << color << "\"";
#endif
	  }

	if (style.length ())
	  {
	    if (comma)
	      out << ',';
	    else
	      comma = true;
	    out << "style=";
#if BOOST_VERSION >= 104400
	    out << ::boost::escape_dot_string (style);
#else
	    out << "\"" << style << "\"";
#endif
	  }

	out << ']';
      }

    private:
      /// \brief グラフ記述子のリファレンス
      const G& graph;
    };

    /// \brief BGLのgraphvizのgraph用ライタクラス
    template <class G>
    class graph_writer {
    public:
      /// \brief graph_writer コンストラクタ
      ///
      /// \param[in] g グラフ
      graph_writer(const G& g) : graph (g) {}

      /// \brief グラフ出力用叙述関数
      ///
      /// \param[in] out 出力ストリーム
      void operator()(std::ostream& out) const {
	const std::string&
	  name = ::boost::get_property(graph, ::boost::graph_name);

	out << "graph [rankdir=LR";
	if (name.length())
	  {
	    out << ",label=";
#if BOOST_VERSION >= 104400
	    out << ::boost::escape_dot_string (name);
#else
	    out << "\"" << name << "\"";
#endif
	  }
	out << "];\n";
      }
    private:
      /// \brief グラフ記述子のリファレンス
      const G& graph;
    };

    /// \brief Vertexのラベルライタ生成関数
    ///
    /// \param[in] g グラフ
    template <class G> inline label_writer<G>
    my_make_label_writer(G g) { return label_writer<G>(g); }

    /// \brief Edgeライタ生成関数
    ///
    /// \param[in] g グラフ
    template <class G> inline edge_writer<G>
    my_make_edge_writer(G g) { return edge_writer<G>(g); }

    /// \brief Graphライタの生成関数
    ///
    /// \param[in] g グラフ
    template <class G> inline graph_writer<G>
    my_make_graph_writer(G g) { return graph_writer<G>(g); }

    /// @}

    /// \defgroup XMLCSP_MAIN CSP内部データ生成モジュール
    ///
    /// \brief XMLからCSP内部データを生成する処理モジュール
    ///
    /// - BLXMLの読み込み\n
    ///   Simulink::CspTrans::CspReader::input_xml()
    /// - Dotファイルの出力\n
    ///   Simulink::CspTrans::CspReader::output_csp()
    ///
    /// 主要クラスの概要
    /// - CspReader\n
    ///   構築された情報が格納されたクラス。これがコード生成に使われる。
    ///   (一部は各クラス(特に CspNode の)staticメンバに保持される
    /// - CspNode\n
    ///   CSPのプロセスとなるクラス
    /// - CspEdge\n
    ///   CSPのプロセスの接続情報のクラス
    /// - CspNodeList\n
    ///   CspNode のポインタのベクタを継承したクラス
    ///
    /// また、BLXMLのブロック以外の情報も読み、コード生成に関わるものは
    /// C_CodeWriter に設定を行う
    ///
    /// \sa CspReader::input_xml()
    /// \sa CspReader::output_csp()
    /// \sa C_CodeWriter
    ///
    /// @{

    /// \brief SampleTime(Simulinkのサンプル時間)
    class SampleTime
    {
    public:
      /// \brief サンプル時間の種別
      typedef enum {
	ST_TYPE_NONE,
	ST_TYPE_DISCRETE, ST_TYPE_CONTINUOUS, ST_TYPE_FIXED,
	ST_TYPE_INHERITED, ST_TYPE_CONSTANT,
	ST_TYPE_VARIABLE, ST_TYPE_HYBRID, ST_TYPE_TRIGGERED,
	ST_TYPE_ASYNCHRONOUS,
	ST_TYPE_INVALID
      } time_type_T;

      /// \brief SampleTime のデフォルトコンストラクタ
      SampleTime () : st_type(ST_TYPE_NONE),
		      st_dvalue(0), st_doffset(0),
		      st_cvalue(0), st_coffset(0),
		      st_inherited(false), st_baserate (false),
		      st_mode (3)
      {
      }

      /// \brief SampleTime のコンストラクタ(値指定)
      ///
      /// \param[in] v サンプル時間
      SampleTime (double v) : st_type (ST_TYPE_DISCRETE),
			      st_dvalue (v), st_doffset (0),
			      st_cvalue (0), st_coffset (0),
			      st_inherited (false), st_baserate (false),
			      st_mode (0)
      {
      }

      /// \brief SampleTime のコンストラクタ(文字列指定)
      ///
      /// \param[in] str サンプル時間の文字列
      SampleTime (const std::string& str);

      /// \brief サンプル時間設定関数
      ///
      /// \param[in] str サンプル時間の文字列
      void set (const std::string& str);

      /// \brief サンプル時間の種別取得(参照用)
      ///
      /// \return サンプル時間の種別
      ///
      /// \sa time_type_T
      time_type_T type (void) const { return st_type; }

      /// \brief サンプル時間の種別取得(設定用)
      ///
      /// \return サンプル時間の種別のリファレンス
      time_type_T& type (void) { return st_type; }

      /// \brief サンプル時間の実数値取得(参照用)
      ///
      /// \return サンプル時間(実数値)
      double dvalue (void) const { return st_dvalue; }

      /// \brief サンプル時間の実数値取得(設定用)
      ///
      /// \return サンプル時間(実数値)のリファレンス
      double& dvalue (void)  { return st_dvalue; }

      /// \brief 開始オフセットの実数値取得(参照用)
      ///
      /// \return 開始オフセット(実数値)
      double doffset (void) const { return st_doffset; }

      /// \brief 開始オフセットの実数値取得(設定用)
      ///
      /// \return 開始オフセット(実数値)のリファレンス
      double& doffset (void)  { return st_doffset; }

      /// \brief サンプル時間の整数値(ベースレートの倍数)取得(参照用)
      ///
      /// \return サンプル時間(整数値)
      unsigned long cvalue (void) const { return st_cvalue; }

      /// \brief サンプル時間の整数値(ベースレートの倍数)取得(設定用)
      ///
      /// \return サンプル時間(整数値)のリファレンス
      unsigned long& cvalue (void)  { return st_cvalue; }

      /// \brief 開始オフセットの整数値取得(参照用)
      ///
      /// \return 開始オフセット(整数値)
      unsigned long coffset (void) const { return st_coffset; }

      /// \brief 開始オフセットの整数取得(設定用)
      ///
      /// \return 開始オフセット(整数値)のリファレンス
      unsigned long& coffset (void)  { return st_coffset; }

      /// \brief サンプル時間の文字列取得(参照用)
      ///
      /// \return サンプル時間(文字列)のリファレンス
      const std::string& svalue (void) const { return st_svalue; }

      /// \brief サンプル時間の文字列取得(設定用)
      ///
      /// \return サンプル時間(文字列)のリファレンス
      std::string& svalue (void) { return st_svalue; }

      /// \brief 開始オフセットの文字列取得(参照用)
      ///
      /// \return 開始オフセット(文字列)のリファレンス
      const std::string& soffset (void) const { return st_soffset; }

      /// \brief 開始オフセットの文字列取得(設定用)
      ///
      /// \return 開始オフセット(文字列)のリファレンス
      std::string& soffset (void) { return st_soffset; }

      /// \brief 継承したものかどうかを取得(参照用)
      ///
      /// \retval true 継承したもの
      /// \retval false 元々設定されている
      bool inherited (void) const { return st_inherited; }

      /// \brief 継承したものかどうかを取得(設定用)
      ///
      /// \return 継承したかどうかの真理値のリファレンス
      bool& inherited (void) { return st_inherited; }

      /// \brief ベースレートかどうかを取得(参照用)
      ///
      /// \retval true ベースレート
      /// \retval false ベースレートではない
      bool baserate (void) const { return st_baserate; }

      /// \brief ベースレートかどうかを取得(設定用)
      ///
      /// \return ベースレートかどうかの真理値のリファレンス
      bool& baserate (void) { return st_baserate; }

      /// \brief 入力側から継承できるかどうか
      ///
      /// \retval true 入力側から継承可
      /// \retval false 入力側から継承不可
      bool is_input_acceptable (void) const { return (st_mode & 1) != 0; }

      /// \brief 出力側から継承できるかどうか
      ///
      /// \retval true 出力側から継承可
      /// \retval false 出力側から継承不可
      bool is_output_acceptable (void) const { return (st_mode & 2) != 0; }

      /// \brief 入力側から継承できるかどうか設定する
      ///
      /// \param[in] flag 設定値
      void input_acceptable (bool flag)
      {
	if (flag)
	  st_mode |= 1;
	else
	  st_mode &= ~1;
      }

      /// \brief 出力側から継承できるかどうかを設定する
      ///
      /// \param[in] flag 設定値
      void output_acceptable (bool flag)
      {
	if (flag)
	  st_mode |= 2;
	else
	  st_mode &= ~2;
      }

      /// \brief 対象が継承可能な種別かどうか
      ///
      /// \retval true 継承可能
      /// \retval false 継承不可
      bool is_inheritable (void) const
      {
	return (type () == ST_TYPE_CONSTANT
		|| type () == ST_TYPE_CONTINUOUS
		|| type () == ST_TYPE_INHERITED);
      }

      /// \brief 継承することができるレートかどうか
      ///
      /// \param[in] x 対象のサンプル時間
      ///
      /// \retval true 継承可能なサンプル時間
      /// \retval false 継承できないサンプル時間
      bool acceptable_rate (const SampleTime& x) const
      {
	return (st_type == ST_TYPE_DISCRETE
		&& x.st_type == ST_TYPE_DISCRETE
		&& x.st_cvalue > st_cvalue
		&& x.st_cvalue % st_cvalue == 0);
      }

      /// \brief 最小のチック(サンプル時間)を取得する
      ///
      /// \return 最小チック
      ///
      /// \sa #st_tick
      static double get_tick (void) { return st_tick; }

      /// \brief 最小チック(サンプル時間)を設定する
      ///
      /// \param[in] t チック
      ///
      /// \sa #st_tick
      static void set_tick (double t) { st_tick = t; }

      /// \brief サンプル時間のシンボルが参照可能かどうか調べる
      ///
      /// \param[in] param シンボル文字列
      ///
      /// \retval true \a param に対する値がある
      /// \retval false 参照できない
      static bool check_param (const std::string& param);

      /// \brief サンプル時間のシンボルと値を登録する
      ///
      /// \param[in] param シンボル文字列
      /// \param[in] val 値
      static void set_param (const std::string& param, double val);

      /// \brief サンプル時間のシンボルの値を取得する
      ///
      /// \param[in] param シンボル文字列
      ///
      /// \return \a param に対するサンプル時間の値
      static double get_param (const std::string& param);

      /// \brief 最大公約数を計算する
      ///
      /// \param[in] x 値1
      /// \param[in] y 値2
      ///
      /// \return \a x と \a y の最大公約数
      static unsigned long gcd (unsigned long x, unsigned long y);

      /// \brief equalのオペレータ
      ///
      /// \return true \a x と一致
      /// \return false \a x と不一致
      bool operator== (const SampleTime& x) const;

      /// \brief not equalのオペレータ
      ///
      /// \return true \a x と不一致
      /// \return false \a x と一致
      bool operator!= (const SampleTime& x) const { return ! (*this == x); }

      /// \brief less thanのオペレータ
      ///
      /// \return true \a x より小さい
      /// \return false \a x と同じか大きい
      bool operator< (const SampleTime& x) const
      {
	return st_cvalue < x.st_cvalue;
      }

      /// \brief less equalのオペレータ
      ///
      /// \return true \a x と同じか小さい
      /// \return false \a x より大きい
      bool operator<= (const SampleTime& x) const
      {
	return st_cvalue <= x.st_cvalue;
      }

      /// \brief greater thanのオペレータ
      ///
      /// \return true \a x より大きい
      /// \return false \a x と同じか小さい
      bool operator> (const SampleTime& x) const
      {
	return st_cvalue > x.st_cvalue;
      }

      /// \brief greater equalのオペレータ
      ///
      /// \return true \a x と同じか大きい
      /// \return false \a x より小さい
      bool operator>= (const SampleTime& x) const
      {
	return st_cvalue >= x.st_cvalue;
      }

      /// \brief 文字列へのキャスト
      ///
      /// \return SampleTimeの値を文字列にしたもの
      operator std::string () const;

    private:

      /// \brief トークン処理用
      const char* tok (const char*& s, int& n);

      /// \brief 全体の最小サンプル時間(初期値は1us)
      ///
      /// \sa set_tick()
      /// \sa get_tick()
      static double st_tick;

      /// \brief シンボルの値を解決するためのマップ
      static std::map < const std::string, double > param_map;

      /// \brief サンプル時間の種別
      time_type_T st_type;

      /// \brief サンプル時間(実数値)
      double st_dvalue;

      /// \brief サンプル開始オフセット(実数値)
      double st_doffset;

      /// \brief 最小サンプル時間を単位とした値
      unsigned long st_cvalue;

      /// \brief 開始オフセット
      unsigned long st_coffset;

      /// \brief  サンプル時間(設定されていた文字列)
      std::string st_svalue;

      /// \brief 開始オフセット(設定されていた文字列)
      std::string st_soffset;

      /// \brief 継承されたかどうかの真理値
      bool st_inherited;

      /// \brief ベースレートかどうかの真理値
      bool st_baserate;

      /// サンプル時間の継承モード
      unsigned int st_mode;
    };

    /// \brief SampleTime の出力オペレータ
    ///
    /// \param[in] s 出力ストリーム
    /// \param[in] x SampleTime
    ///
    /// \return 出力ストリーム
    std::ostream& operator<< (std::ostream& s, const SampleTime& x);


    /// \brief ソートされるサンプル時間
    ///

    /// CspReader では CspReader::sort_sampletime_func()
    /// により降順(遅い順)になる
    /// (サンプル時間の継承を遅い順に決定するため)
    class sorted_SampleTime
    {
    public:
      /// \brief sorted_SampleTime のコンストラクタ
      ///
      /// \param[in] id TIDの配列のインデクス
      /// \param[in] st 継承元となる SampleTime のポインタ
      sorted_SampleTime (size_t id, SampleTime* st)
	: sst_id (id), sst_st (st)
      {
      }

      /// \brief
      /// 格納している SampleTime のポインタを返すためのオペレータ
      SampleTime* operator-> () const { return sst_st; }

      /// \brief
      /// 格納している SampleTime のリファレンスを返すためのオペレータ
      SampleTime& operator* () const { return *sst_st; }

      /// \brief SampleTime のポインタを返すキャスト
      operator SampleTime* () const { return sst_st; }

      /// \brief TIDの配列のインデクスを返すキャスト
      operator size_t () const { return sst_id; }

      /// \brief サンプル時間が等しいかを調べる等号オペレータ
      bool operator== (const sorted_SampleTime& x) const
      {
	return *sst_st == *x.sst_st;
      }

      /// \brief サンプル時間の比較(less than)を行うオペレータ
      bool operator< (const sorted_SampleTime& x) const
      {
	return *sst_st < *x.sst_st;
      }

    private:
      /// \brief TIDの配列のインデクス
      size_t sst_id;

      /// \brief 継承元となる SampleTime のポインタ
      SampleTime* sst_st;
    };

    //
    // BlockInfo
    //

    class base_CspBlockInfo;

    /// \brief BlockConnect の拡張クラス
    ///
    /// 現在は空
    ///
    /// \sa CspBlockConnect
    class base_CspBlockConnect
    {
    };

    /// \brief CspVar のリストの型
    ///
    /// CspBlockInfo 内の変数のリストがこの型になる
    ///
    /// \sa CspVar
    /// \sa base_CspVar
    typedef Simulink::XML::Tmpl_var_list_T<base_CspVar> csp_var_list_T;

    /// \brief CspBlockConnect の型
    ///
    /// CspBlockConnect は base_CspBlockConnect を継承したクラス
    ///
    /// \sa CspBlockInfo
    /// \sa base_CspBlockConnect
    /// \sa base_CspVar
    typedef Simulink::XML::Tmpl_BlockConnect< CspBlockInfo,
					      base_CspBlockConnect,
					      base_CspVar
					      > CspBlockConnect;

    /// \brief CspBlockConnect のリストの型
    ///
    /// CspBlockConnect は CspBlockInfo の接続情報を持ち、伝播する変数
    /// は base_CspVar を継承している
    ///
    /// \sa CspBlockInfo
    /// \sa base_CspBlockConnect
    /// \sa base_CspVar
    typedef
    Simulink::XML::Tmpl_block_connect_list_T< CspBlockInfo,
					      base_CspBlockConnect,
					      base_CspVar
					      > csp_block_connect_list_T;

    /// \brief CspBlockConnect のポインタのベクタの型
    ///
    /// \sa csp_edge_T
    /// \sa CspEdge
    typedef std::vector < const CspBlockConnect* >
    csp_const_block_connect_list_T;

    /// \brief CspBlockInfo 等を使用する Reader クラスの型
    ///
    /// 実際は、この型を継承した CspReader を使用する
    ///
    /// \sa CspReader
    typedef Simulink::XML::Tmpl_Reader< CspBlockInfo,
					base_CspBlockConnect,
					base_CspVar > base_CspReader;

    /// \brief CspBlockInfo のペアの型
    typedef std::pair < CspBlockInfo*, CspBlockInfo* > blockinfo_pair_T;

    /// \brief CspBlockInfo のペアのベクタの型
    typedef std::vector < blockinfo_pair_T > blockinfo_pair_list_T;

    //
    // 変数の属性
    //

    /// \brief 拡張する変数属性のクラス
    ///
    /// base_CspVar は、SimulinkXMLのテンプレートクラスを介し、
    /// BLXMLのvarタグのコンテナ(型としては CspVar と同一)
    /// から継承される。
    ///
    /// また、Cコード出力用に var_writer を継承している
    ///
    /// \sa var_writer
    class base_CspVar
      : public var_writer
    {
    public:
      /// \brief base_CspVar のコンストラクタ
      ///
      /// \param[in] v BLXMLのvarタグのポインタ
      /// \param[in] m 変数の属性
      base_CspVar(const SimulinkModel::XSD::var_T* v,
		  var_mode_T m = VAR_MODE_NONE)
	: cv_mode (m), cv_size (0)
      {
	if (v)
	  setup (v);
      }

      /// \brief クラスのセットアップ関数
      ///
      /// \param[in] v BLXMLのvarタグのポインタ
      ///
      /// var_basic_writer に、コード生成時に使用する構造体名を設定し、
      /// 配列であればそのサイズを #cv_size に設定する
      void setup (const SimulinkModel::XSD::var_T* v)
      {
	if (v)
	  {
	    const SimulinkModel::XSD::var_T::storage_optional&
	      sto = v->storage ();
	    if (sto)
	      set_storage (*sto);
	    const SimulinkModel::XSD::var_T::size_optional&
	      szo = v->size ();
	    if (szo)
	      cv_size = *szo;
	  }
      }

      /// \brief 変数の属性を返す
      ///
      /// \return 変数の属性
      ///
      /// \sa VAR_MODE
      var_mode_T mode (void) const { return cv_mode; }

      /// \brief 変数の属性のリファレンスを返す
      ///
      /// \return 変数の属性リファレンス
      var_mode_T& mode (void) { return cv_mode; }

      /// \brief
      /// base_CspVar を継承している CspVar のポインタのダウンキャスト
      ///
      /// \return CspVar のポインタ
      operator CspVar* () {
	return static_cast< CspVar* > (this);
      }

      /// \brief

      /// base_CspVar を継承しているconstな
      /// CspVar のポインタのダウンキャスト
      ///
      /// \return constな CspVar のポインタ
      operator const CspVar* () const
      {
	return static_cast< const CspVar* > (this);
      }

      /// \brief BLXMLのvarタグのポインタを返す純粋仮想関数
      ///
      /// \return BLXMLのvarタグのポインタ
      ///
      /// この関数の実体は、
      /// SimulinkModel::XSD::SimulinkXML::Tmpl_var_T_wrapper にある
      virtual const SimulinkModel::XSD::var_T* var (void) const = 0;

      /// \brief
      /// constなBLXMLのvarタグのポインタを返すオペレータ
      ///
      /// \return constなBLXMLのvarタグのポインタ
      const SimulinkModel::XSD::var_T* operator-> () const { return var (); }

      /// \brief
      /// constなBLXMLのvarタグのリファレンスを返すオペレータ
      ///
      /// \return constなBLXMLのvarタグのリファレンス
      const SimulinkModel::XSD::var_T& operator* () const { return *var (); }

      /// \brief 変数の型を返す
      ///
      /// \return 変数の型
      ///
      /// \sa var_basic_writer::type()
      ///
      /// これは var_basic_writer の純粋仮想関数の実体である
      const std::string& type (void) const { return var ()->type (); }

      /// \brief 変数名を返す
      ///
      /// \return 変数名
      ///
      /// \sa var_basic_writer::name()
      ///
      /// これは var_basic_write の純粋仮想関数の実体である
      const std::string& name (void) const { return var ()->name (); }

      /// \brief 配列のサイズを返す
      ///
      /// \retval 0 配列ではない
      /// \retval 0以外 配列のサイズ
      ///
      /// \sa var_basic_writer::size()
      ///
      /// これは var_basic_write の純粋仮想関数の実体である
      size_t size (void) const { return cv_size; }

    private:
      /// \brief 変数の属性
      ///
      /// \sa VAR_MODE
      var_mode_T cv_mode;

      /// \brief 配列のサイズ
      ///
      /// 変数が配列でない場合は0
      size_t cv_size;
    };

    /// \brief CspNodeのポインタのリスト
    typedef std::list< CspNode* > CspNodePtrList;

    /// \brief CspBlockInfo が継承するクラス
    ///
    /// CspBlockInfo の基底クラスとして、各ブロック毎の、
    /// トランスレータが使用する各種情報を格納する
    class base_CspBlockInfo
    {
    public:
      /// \brief base_CspBlockInfo のコンストラクタ
      base_CspBlockInfo()
	: cbi_lpos (-1), cbi_tid (-1), cbi_task_info (0)
      {
      }

      /// \brief このブロックが含まれる CspNode のポインタを設定する
      ///
      /// \param[in] n CspNode のポインタ
      void set_node(CspNode* n) { cbi_nodes.push_back (n); }

      /// \brief このブロックが含まれていた CspNode のポインタを削除する
      ///
      /// \param[in] n CspNode のポインタ
      void remove_node (CspNode* n) { cbi_nodes.remove (n); }

      /// \brief この CspBlockInfo が含まれる
      /// CspNode のポインタを取得する
      ///
      /// \return CspNode のポインタ
      CspNode* get_node (void) const
      {
	// 二つ以上のブロックにマージされている場合を考慮するコードでは
	// 使用してはならない
	assert (cbi_nodes.size () <= 1);

	CspNodePtrList::const_iterator i (cbi_nodes.begin ());
	if (i == cbi_nodes.end ())
	  return NULL;
	return *i;
      }

      /// \brief この CspBlockInfo が含まれる
      /// CspNode のポインタのリストを取得する(参照用)
      ///
      /// \return constな CspNodePtrList の参照
      const CspNodePtrList& get_nodes(void) const { return cbi_nodes; }

      /// \brief この CspBlockInfo が含まれる
      /// CspNode のポインタのリストを取得する(設定用)
      ///
      /// \return CspNodePtrList の参照
      CspNodePtrList& get_nodes(void) { return cbi_nodes; }

      /// \brief この CspBlockInfo が含まれる
      /// CspNode のポインタへのキャスト
      ///
      /// \return CspNode のポインタ
      operator CspNode*() const { return get_node (); }

      /// \brief
      /// このブロックのサンプル時間のconstなリファレンスを取得(参照用)
      ///
      /// \return constな SampleTime のリファレンス
      const SampleTime& get_SampleTime(void) const { return cbi_stime; }

      /// \brief
      /// このブロックのサンプル時間のconstなリファレンスを取得(設定用)
      ///
      /// \return SampleTime のリファレンス
      SampleTime& get_SampleTime(void) { return cbi_stime; }

      /// \brief
      /// このブロックのコードが現れるソースファイルの行番号を取得する
      ///
      /// \return 行番号
      ///
      /// この行番号は CspNode 内のブロックのソートにも使用される
      int line_position (void) const { return cbi_lpos; }

      /// \brief このブロックのコードの行番号を設定する
      ///
      /// \param[in] l ソースファイル中の行番号
      void line_position (int l) { cbi_lpos = l; }

      /// \brief このブロックのコード中にあるTIDの番号を返す
      ///
      /// \retval -1 TIDを含むコードがない
      /// \retval それ以外 TIDの番号
      ///
      /// TIDのコードが含まれていれば、
      /// サンプル時間を解決するためのヒントとして使われる
      int tid (void) const { return cbi_tid; }

      /// \brief このブロックのコード中にあるTIDの番号を設定する
      ///
      /// \param[in] t TIDの番号
      void tid (int t) { cbi_tid = t; }

      /// \brief このブロックをマージしているブロックの
      /// constなリストのリファレンスの取得(参照用)
      ///
      /// \return constな CspBlockInfo のリスト
      /// csp_block_list_T のリファレンス
      ///
      /// 通常空か、あっても一つだが、
      /// Constant等では複数のブロックがありうる
      const csp_block_list_T& merge_to (void) const { return cbi_merge_to; }

      /// \brief
      /// このブロックをマージしているブロックのリストのリファレンスの取得
      /// (設定用)
      ///
      /// \return CspBlockInfo のリスト csp_block_list_T のリファレンス
      csp_block_list_T& merge_to (void) { return cbi_merge_to; }

      /// \brief タスク属性の取得(参照用)
      ///
      /// \return タスク属性
      ///
      /// \sa TASK_INFO
      unsigned int task_info (void) const { return cbi_task_info; }

      /// \brief タスク属性のリファレンス取得(設定用)
      ///
      /// \return タスク属性のリファレンス
      unsigned int& task_info (void) { return cbi_task_info; }

      /// \brief タスク属性の設定
      ///
      /// \param[in] i タスク属性
      ///
      /// \sa TASK_INFO
      void task_info (unsigned int i) { cbi_task_info = i; }

      /// \brief BLXMLのblock内で、一致する変数名のvarタグを取得する
      ///
      /// \retval NULL 変数がない
      /// \retval NULL以外 varタグのポインタ
      const SimulinkModel::XSD::var_T*
      find_var (const std::string& name) const;

    private:
      /// \brief このブロックが含まれる CspNode のポインタ
      CspNodePtrList cbi_nodes;

      /// \brief このブロックのサンプル時間
      SampleTime cbi_stime;

      /// \brief このブロックのコードのソースファイル中の行番号
      int cbi_lpos;

      /// \brief このブロックのコードが持つTIDの番号
      int cbi_tid;

      /// \brief このブロックのタスク属性
      /// \sa TASK_INFO
      unsigned int cbi_task_info;

      /// \brief このブロックをマージしているブロックのリスト
      csp_block_list_T cbi_merge_to;
    };

    /// \brief param_ent_T が継承するクラス
    ///
    /// param_T または var_T のポインタ、
    /// およびそれらが保持する情報へのアクセッサ
    ///
    /// Cコード出力用に var_writer を継承している
    ///
    /// \sa var_writer
    class param_var_wrapper
      : public var_writer
    {
    public:
      /// \brief param_T による param_var_wrapper のコンストラクタ
      ///
      /// \param[in] param BLXMLのparam,const_param,state等のタグ
      ///
      /// var_basic_writer に、
      /// コード生成時に使用する構造体名と初期値を設定する
      param_var_wrapper (const SimulinkModel::XSD::param_T* param)
	: pe_param (param), pe_var (NULL)
      {
	if (param)
	  {
	    set_storage (param->storage ());

	    const SimulinkModel::XSD::param_T::initcode_optional&
	      io = param->initcode ();
	    if (io)
	      set_initcode (*io);
	  }
      }

      /// \brief var_T による param_var_wrapper のコンストラクタ
      ///
      /// \param[in] var BLXMLのvarのタグ
      ///
      /// var_basic_writer に、
      /// コード生成時に使用する構造体名と初期値を設定する
      ///
      /// これはsignalを代替するvar(signalを参照する)が使用する
      param_var_wrapper (const SimulinkModel::XSD::var_T* var)
	: pe_param (NULL), pe_var (var)
      {
	if (var)
	  {
	    const SimulinkModel::XSD::var_T::storage_optional&
	      so = var->storage ();
	    if (so)
	      set_storage (*so);
	  }
      }

      /// \brief 構造体オブジェクト名を返す
      ///
      /// \return 構造体オブジェクト名の参照
      const
      std::string& storage (void) const
      {
	if (pe_param)
	  return pe_param->storage ();
	const SimulinkModel::XSD::var_T::storage_optional&
	  so = pe_var->storage ();
	assert (so);
	return *so;
      }

      /// \brief
      /// constなBLXMLのparam等のタグのポインタへ変換するオペレータ
      ///
      /// \return constなBLXMLのparam等のタグのポインタ
      operator const SimulinkModel::XSD::param_T* () const { return pe_param; }

      /// \brief
      /// constなBLXMLのvarのタグのポインタへ変換するオペレータ
      ///
      /// \return constなBLXMLのvarのタグのポインタ
      operator const SimulinkModel::XSD::var_T* () const { return pe_var; }

      /// \brief BLXMLのparam等のタグのポインタを取得する(参照用)
      ///
      /// \return constなBLXMLのparam等のタグのポインタ
      const SimulinkModel::XSD::param_T* param (void) const { return pe_param; }

      /// \brief
      /// BLXMLのparam等のタグのポインタのリファレンスを取得する(設定用)
      ///
      /// \return BLXMLのparam等のタグのポインタのリファレンス
      const SimulinkModel::XSD::param_T*& param (void) { return pe_param; }

      /// \brief BLXMLのvarのタグのポインタを取得する(参照用)
      ///
      /// \return constなBLXMLのvarのタグのポインタ
      const SimulinkModel::XSD::var_T* var (void) const { return pe_var; }

      /// \brief
      /// BLXMLのvarのタグのポインタのリファレンスを取得する(設定用)
      ///
      /// \return BLXMLのvarのタグのポインタのリファレンス
      const SimulinkModel::XSD::var_T*& var (void) { return pe_var; }

      /// \brief 変数の型を取得
      ///
      /// \return 変数の型
      ///
      /// \sa var_basic_writer::type()
      ///
      /// これは var_basic_writer の純粋仮想関数の実体である
      const
      std::string& type (void) const
      {
	return pe_param ? pe_param->type () : pe_var->type ();
      }

      /// \brief 変数名を取得
      ///
      /// \return 変数名
      ///
      /// \sa var_basic_writer::name()
      ///
      /// これは var_basic_write の純粋仮想関数の実体である
      const
      std::string& name (void) const
      {
	return pe_param ? pe_param->name () : pe_var->name ();
      }

      /// \brief 配列のサイズを取得
      ///
      /// \retval 0以外 配列のサイズ
      /// \retval 0 配列でない(スカラ)
      ///
      /// \sa var_basic_writer::size()
      ///
      /// これは var_basic_write の純粋仮想関数の実体である
      size_t size (void) const
      {
	if (pe_param)
	  {
	    const SimulinkModel::XSD::param_T::size_optional&
	      so = pe_param->size ();
	    if (so)
	      return *so;
	  }
	else
	  {
	    const SimulinkModel::XSD::var_T::size_optional&
	      so = pe_var->size ();
	    if (so)
	      return *so;
	  }
	return 0;
      }

    private:

      /// \brief BLXMLのparam等のタグのポインタ
      const SimulinkModel::XSD::param_T* pe_param;

      /// \brief BLXMLのvarのタグのポインタ(主にsignalを参照する)
      const SimulinkModel::XSD::var_T* pe_var;
    };

    /// \brief BLXMLのparam,const_param,state等のタグ、
    /// およびsignalを代替するvarを格納するクラス
    ///
    /// \sa param_var_wrapper
    class param_ent_T
      : public param_var_wrapper
    {
    public:
      /// \brief param_T による param_ent_T のコンストラクタ
      ///
      /// \param[in] id パラメタを持つブロックのID
      /// \param[in] p BLXMLのparam,const_param,state等のタグ
      ///
      param_ent_T (long id = 0, const SimulinkModel::XSD::param_T* p = NULL)
	: param_var_wrapper (p), pe_id (id)
      {
      }

      /// \brief var_T による param_ent_T のコンストラクタ
      ///
      /// \param[in] id パラメタを持つブロックのID
      /// \param[in] v BLXMLのvarのタグ
      ///
      /// var_basic_writer に、
      /// コード生成時に使用する構造体名と初期値を設定する
      param_ent_T (long id, const SimulinkModel::XSD::var_T* v)
	: param_var_wrapper (v), pe_id (id)
      {
      }

      /// \brief
      /// constな param_var_wrapper のポインタへ変換するオペレータ
      ///
      /// \return constな param_var_wrapper のポインタ
      const param_var_wrapper* operator-> () const { return this; }

      /// \brief
      /// constな param_var_wrapper のリファレンスへ変換するオペレータ
      ///
      /// \return constな param_var_wrapper のリファレンス
      const param_var_wrapper& operator* () const { return *this; }

      /// \brief パラメタを持つブロックのIDを返す(参照用)
      ///
      /// \return ブロックのID
      long id (void) const { return pe_id; }

      /// \brief パラメタを持つブロックのIDのリファレンスを返す(設定用)
      ///
      /// \return ブロックのIDのリファレンス
      long& id (void) { return pe_id; }

    private:
      /// \brief ブロックのID
      long pe_id;
    };

    ///
    /// \brief param_ent_T のリストのクラス
    ///
    /// \sa CspNode::signal()
    /// \sa CspNode::state()
    /// \sa CspNode::trigger_state()
    /// \sa CspNode::invariant_signal()
    /// \sa CspNode::param()
    /// \sa CspNode::const_param()
    class ParamList : public std::list < param_ent_T >
    {
    public:
      /// \brief param_ent_T をリストに追加する
      ///
      /// \param[in] p param_ent_T のインスタンス
      ///
      /// \return リストに追加された param_ent_T のリファレンス
      param_ent_T&
      add_param (const param_ent_T& p)
      {
	push_back (p);
	return back ();
      }

      /// \brief ブロックのIDとBLXMLのparam等のタグのポインタを指定し、
      /// param_ent_T をリストに追加する
      ///
      /// \param[in] id param等のタグを持つブロックのID
      /// \param[in] param BLXMLのparam等のタグのポインタ
      ///
      /// \return リストに追加された param_ent_T のリファレンス
      param_ent_T&
      add_param (long id, const SimulinkModel::XSD::param_T* param)
      {
	push_back (param_ent_T (id, param));
	return back ();
      }

      /// \brief 指定した ParamList を自身に追加する
      ///
      /// \param[in] p 追加する ParamList のリファレンス
      void
      merge (const ParamList& p)
      {
	for (ParamList::const_iterator pi (p.begin ()); pi != p.end (); pi++)
	  {
	    const param_ent_T& pe = *pi;
	    add_param (pe);
	  }
      }

      /// \brief 指定した名前の変数を探す
      ///
      /// \param[in] name 探索する変数の名前
      ///
      /// \return name と一致する要素のイテレータ
      const_iterator
      find_by_name (const std::string& name) const
      {
	const_iterator e = end ();
	for (const_iterator i (begin ()); i != e; i++)
	  {
	    if (i->name () == name)
	      return i;
	  }
	return e;
      }
    };

    /// \brief コードの断片を保持するクラス
    ///
    /// コードの断片は単に文字列なので、stringクラスを継承する。
    /// ただし、文字列ではない区切りを扱うため、
    /// 有効な文字列かそうでないかを区別するためのメンバを持つ
    class code_frag_T
      : public std::string
    {
    public:
      /// \brief code_frag_T のデフォルトコンストラクタ。
      /// 区切りを生成するために使用
      code_frag_T () : cf_valid (false) {} // 区切り

      /// \brief コードの断片(stringクラス)を指定したコンストラクタ
      ///
      /// \param[in] x コードの断片(stringクラス)
      code_frag_T (const std::string& x)
	: std::string (x), cf_valid (true) {}

      /// \brief コードの断片(文字列のポインタ)を指定したコンストラクタ
      ///
      /// \param[in] s コードの断片(文字列のポインタ)
      code_frag_T (const char* s)
	: std::string (s), cf_valid (true) {}

      /// \brief コードの断片(文字列のポインタと長さ)を指定したコンストラクタ
      ///
      /// \param[in] s コードの断片(文字列のポインタ)
      /// \param[in] n コードのサイズ
      code_frag_T (const char* s, size_t n)
	: std::string (s, n), cf_valid (true) {}

      /// \brief コードの断片かそうでないかを調べるための真理値へのキャスト
      ///
      /// \retval true コードの断片である
      /// \retval false コードの断片ではなく区切りである
      operator bool () const { return cf_valid; }

      /// \brief constなコードの断片のリファレンスを返すオペレータ
      ///
      /// \return constなコードの断片のリファレンス
      const std::string& operator* () const { return *this; }

      /// \brief コードの断片のリファレンスを返すオペレータ
      ///
      /// \return コードの断片のリファレンス
      std::string& operator* () { return *this; }

      /// \brief constなコードの断片のポインタを返すオペレータ
      ///
      /// \return constなコードの断片のポインタ
      const std::string* operator-> () const { return this; }

      /// \brief コードの断片のポインタを返すオペレータ
      ///
      /// \return コードの断片のポインタ
      std::string* operator-> () { return this; }

      /// \brief constなコードの断片のstringのリファレンスへのキャスト
      operator const std::string& () const { return *this; }

      /// \brief コードの断片のstringのリファレンスへのキャスト
      operator std::string& () { return *this; }

    private:
      /// \brief コードの断片(true)か区切り(false)かの真理値
      bool cf_valid;
    };

    /// \brief コードの断片のリスト
    class code_frag_list_T
      : public std::list< code_frag_T >
    {
    };

    /// \brief BLXMLのcodeタグを格納するクラス
    class code_ent_T
    {
    public:
      /// \brief code_ent_T のコンストラクタ
      ///
      /// \param[in] l コードの行番号
      /// \param[in] b このコードを持つ CspBlockInfo のポインタ
      /// \param[in] n コードを持つブロック名
      /// \param[in] c BLXMLのcodeタグのポインタ
      code_ent_T (int l, const CspBlockInfo* b, const std::string& n,
		  const SimulinkModel::XSD::code_T* c)
	: c_line (l), c_blockinfo (b), c_block (n), c_code (c) {}

      /// \brief コードの行番号を取得する
      ///
      /// \return 行番号
      int line (void) const { return c_line; }

      /// \brief コードを持つ CspBlockInfo のポインタを取得する
      ///
      /// \return CspBlockInfo のポインタ
      const CspBlockInfo* blockinfo (void) const { return c_blockinfo; }

      /// \brief コードを持つブロック名を取得する
      ///
      /// \return コードを持つブロック名
      const std::string& block (void) const { return c_block; }

      /// \brief BLXMLのcodeタグのポインタを取得
      ///
      /// \return BLXMLのcodeタグのポインタ
      const SimulinkModel::XSD::code_T* code () const { return c_code; }

      /// \brief 指示行に基づき断片化されたコードの
      /// constなリストのリファレンスを取得(参照用)
      ///
      /// \return 断片化されたコードのconstなリストのリファレンス
      const code_frag_list_T&
      code_frag_list (void) const { return c_code_frag_list; }

      /// \brief
      /// 指示行に基づき断片化されたコードのリストのリファレンスを取得
      /// (設定用)
      ///
      /// \return 断片化されたコードのストのリファレンス
      code_frag_list_T& code_frag_list (void) { return c_code_frag_list; }

      /// \brief BLXMLのcodeタグのconstなポインタに変換するオペレータ
      ///
      /// \return BLXMLのcodeタグのconstなポインタ
      const SimulinkModel::XSD::code_T* operator-> () const { return c_code; }

      /// \brief
      /// BLXMLのcodeタグのconstなリファレンスに変換するオペレータ
      ///
      /// \return BLXMLのcodeタグのconstなリファレンス
      const SimulinkModel::XSD::code_T& operator* () const { return *c_code; }

    private:
      /// \brief コードの行番号
      int c_line;

      /// \brief CspBlockInfo のポインタ
      const CspBlockInfo* c_blockinfo;

      /// \brief コードを持つブロックの名前
      std::string c_block;

      /// \brief BLXMLのcodeタグのポインタ
      const SimulinkModel::XSD::code_T* c_code;

      /// \brief コード中の指示に基づき断片化されたコードのリスト
      code_frag_list_T c_code_frag_list;
    };

    /// \brief code_ent_T のベクタの型
    ///
    /// \sa CodeList
    ///
    /// この型は CodeList に継承される
    typedef std::vector < code_ent_T > code_list_T;

    /// \brief CodeList クラス
    ///
    /// \sa code_list_T
    /// \sa CspNode::code()
    ///
    /// このクラスは code_list_T を継承し、メソッドを追加したもの
    class CodeList : public code_list_T
    {
      /// \brief コードの種類でオーダリングの順序を返す関数
      ///
      /// \param[in] x 対象の code_ent_T のリファレンス
      ///
      /// \return オーダリングの順番
      static int code_type_index (const code_ent_T& x)
      {
	const SimulinkModel::XSD::code_T* c = x.code ();
	if (c->type () == "task") return 0;
	if (c->type () == "update") return 1;
	if (c->type () == "const") return 2;
	if (c->type () == "param") return 3;
	if (c->type () == "init") return 4;
	if (c->type () == "state") return 5;
	if (c->type () == "invariant_signal") return 6;
	return 7;		// other
      }

      /// \brief コードの種別でソートするための比較関数
      ///
      /// \param[in] a 比較対象の code_ent_T
      /// \param[in] b 比較対象の code_ent_T
      ///
      /// \retval 負 \a a が先
      /// \retval 0 \a a と \a b は同じ
      /// \retval 正 \a a が後
      static int cmp_type (const code_ent_T& a, const code_ent_T& b)
      {
	return code_type_index (a) - code_type_index (b);
      }

      /// \brief コードの種別毎に、行番号の昇順に並べるための比較関数
      ///
      /// \param[in] a 比較対象の code_ent_T
      /// \param[in] b 比較対象の code_ent_T
      ///
      /// \retval true \a a が先
      /// \retval false \a a と \a b は同じか \a a が後
      static bool cmp_line (const code_ent_T& a, const code_ent_T& b)
      {
	int x = cmp_type (a, b);
	if (x)
	  return x < 0;
	return a.line () < b.line ();
      }

      /// \brief 目的のコード種別の開始位置を
      /// find_if()で見つけるための叙述関数のクラス
      ///
      /// CodeListはタイプ別、行番号順にソート済みなので最初に見つけた
      /// ものを返す
      class search_type
      {
      public:
	/// \brief search_type のコンストラクタ
	///
	/// \param[in] n コードの種別
	search_type (const strict_string& n) : st_type (n) {}

	/// \brief 叙述関数
	///
	/// \param[in] ce 探索中 code_ent_T のリファレンス
	///
	/// \retval true \a ce は目的の種別と一致
	/// \retval false \a ce は目的の種別と不一致
	bool operator() (const code_ent_T& ce) const
	{
	  return ce->type () == st_type;
	}

      private:
	/// \brief 探索対象のコードの種別
	const std::string& st_type;
      };

    public:
      /// \brief リストにBLXMLのcodeタグを追加する
      ///
      /// \param[in] binfo CspBlockInfo のポインタ
      /// \param[in] block コードを持つブロックの名前
      /// \param[in] c BLXMLのcodeタグのポインタ
      void add_code (const CspBlockInfo* binfo,
		     const std::string& block,
		     const SimulinkModel::XSD::code_T* c)
      {
	int line = INT_MAX;

	SimulinkModel::XSD::code_T::line_optional cl = c->line ();

	if (cl)
	  line = *cl;
	push_back (code_ent_T (line, binfo, block, c));
      }

      /// \brief 指定した CodeList を自身に追加する
      ///
      /// \param[in] c 追加する CodeList のリファレンス
      void
      merge (const CodeList& c)
      {
	for (const_iterator ci (c.begin ()); ci != c.end (); ci++)
	  push_back (*ci);
      }

      /// \brief コードの種別毎に、行番号の昇順にソート
      void
      sort (void)
      {
	std::sort (begin (), end (), cmp_line);
      }

      /// \brief 目的のコード種別の開始位置を探索する(参照用)
      ///
      /// \param[in] type 目的のコード種別
      ///
      /// \retval end() 見つからなかった
      /// \retval それ以外 目的のコード種別の開始位置
      const_iterator find_type_code (const std::string& type) const
      {
	return find_if (begin (), end (), search_type (type));
      }

      /// \brief 目的のコード種別の開始位置を探索する(変更用)
      ///
      /// \param[in] type 目的のコード種別
      ///
      /// \retval end() 見つからなかった
      /// \retval それ以外 目的のコード種別の開始位置
      iterator find_type_code (const std::string& type)
      {
	return find_if (begin (), end (), search_type (type));
      }
    };

    /// \brief BLXMLのvarタグを格納するクラス
    ///
    /// 実体は CspVar で、変数属性を設定するためのコンストラクタのみ追加
    ///
    /// \sa CspVar
    /// \sa VarMap
    class var_ent_T
      : public CspVar
    {
    public:
      /// \brief var_ent_T のコンストラクタ
      ///
      /// \param[in] v BLXMLのvarタグ
      /// \param[in] m 変数の属性
      var_ent_T (const SimulinkModel::XSD::var_T* v = NULL,
		 var_mode_T m = VAR_MODE_NONE)
	: CspVar (v)
      {
	mode () = m;
      }
    };

    /// \brief var_ent_T の変数名による連想配列(map)の型
    ///
    /// \sa VarMap
    ///
    /// この型は VarMap に継承される
    typedef std::map < const std::string, var_ent_T > var_map_T;

    /// \brief var_map_T の const_iterator の型
    typedef var_map_T::const_iterator var_map_const_iterator;

    /// \brief var_map_T の iterator の型
    typedef var_map_T::iterator var_map_iterator;

    /// \brief var_map_T の insert() の戻り値の型
    typedef std::pair < var_map_iterator, bool > var_map_res;

    /// \brief 変数の連想配列のクラス
    ///
    /// \sa var_map_T
    /// \sa CspNode::var()
    ///
    /// ブロックのvarタグを保持するために使用する\par
    /// このクラスは var_map_T を継承し、BLXMLのvarタグのポインタを格納
    /// した var_ent_T を連想配列に追加するメソッドを持ったクラス
    class VarMap : public var_map_T
    {
    public:
      /// \brief 変数の名前とBLXMLのvarタグのポインタ、変数の属性を指定
      /// して連想配列に追加する
      ///
      /// \param[in] name 変数名
      /// \param[in] var BLXMLのvarタグのポインタ
      /// \param[in] m 変数の属性
      var_map_res
      add_var_map (const std::string& name,
		   const SimulinkModel::XSD::var_T* var,
		   var_mode_T m)
      {
	return insert (std::make_pair (name, var_ent_T (var, m)));
      }
    };

    /// \brief アクセスするDataStoreMemoryの情報を保持するためのクラス
    ///
    /// \sa CspReader::dsm_map()
    ///
    /// このクラスは単に名前とその種類を保持するだけ
    /// 実際にはこの名前を元に CspReader の CspReader::dsm_map() にて格
    /// 納されたDataStoreMemoryを参照する
    class dsm_info : public std::string
    {
    public:
      /// \brief dsm_infoのコンストラクタ
      ///
      /// \param[in] s DataStoreMemoryを識別するための名前
      /// \param[in] a アクセスの属性
      dsm_info (const std::string& s, unsigned long a)
	: std::string (s), attr (a) {}

      /// \brief 属性を取得する
      ///
      /// \retval 1 DataStoreReadを示す
      /// \retval 2 DataStoreWriteを示す
      unsigned long get_attr (void) const { return attr; }

    private:
      /// \brief DataStoreMemoryをアクセスする属性
      ///
      /// 1はDataStoreReadで、2はDataStoreWriteを示す
      unsigned long attr;
    };

    /// \brief アクセスするDataStoreMemoryのリスト
    ///
    /// \sa CspNode::dsm()
    typedef std::list< dsm_info > dsm_info_list_T;

    //
    // 入出力する変数
    //

    /// \brief 分割された配列の要素の情報
    class PartialArray
    {
    public:
      /// PartialArray のコンストラクタ
      ///
      /// \param[in] start 分割された配列の開始位置
      /// \param[in] size 分割された配列の要素数
      PartialArray (int start = 0, int size = 0)
	: pa_start (start), pa_size (size)
      {
      }

      /// \brief 分割された配列の開始位置を取得(参照用)
      /// \return 分割された配列の開始位置
      int start (void) const { return pa_start; }

      /// \brief 分割された配列の開始位置のリファレンスを取得(設定用)
      ///
      /// \return 分割された配列の開始位置のリファレンス
      int& start (void) { return pa_start; }

      /// \brief 分割された配列の要素数を取得(参照用)
      ///
      /// \return 分割された配列の要素数
      int size (void) const { return pa_size; }

      /// \brief 分割された配列の要素数のリファレンスを取得(設定用)
      ///
      /// \return 分割された配列の要素数のリファレンス
      int& size (void) { return pa_size; }

      /// 一致を判定する等号オペレータ
      ///
      /// \param[in] x 比較する PartialArray
      ///
      /// \retval true \a x と一致
      /// \retval false \a x と不一致
      bool operator== (const PartialArray& x) const
      { return pa_start == x.pa_start && pa_size == x.pa_size; }

      /// ソートに使用する比較(less than)オペレータ
      ///
      /// \param[in] x 比較する PartialArray
      ///
      /// \retval true \a x が後
      /// \retval false \a x が前か同じ
      bool operator< (const PartialArray& x) const
      { return pa_start < x.pa_start; }

    private:
      /// \brief 分割された配列の開始位置
      int pa_start;

      /// \brief 分割された配列の要素数
      int pa_size;
    };

    /// \brief PartialArray のリスト(ベクタ)
    class PartialArrayList
      : public std::vector< PartialArray >
    {
    public:
      /// \brief 連続する分割された配列の情報を結合する
      void compact (void);
    };

    /// \brief 伝播する乳出力変数のクラス
    ///
    /// このクラスでは CspVar (つまりはBLXMLのvarタグ)のポインタと、
    /// PartialArrayList (分割された配列の情報)を持つ
    ///
    /// PropagateVar は、ブロックのvarタグから生成されるものと、ブロッ
    /// クのforward/backwardタグ中のvarタグ(これは CspBlockConnect に格
    /// 納される)から生成されるものがある
    class PropagateVar
    {
    public:
      /// \brief PropagateVar のコンストラクタ
      ///
      /// \param[in] v CspVar のポインタ
      PropagateVar(CspVar* v) : pv_var (v) {}

      /// \brief CspVar のポインタに変換するオペレータ
      ///
      /// \return CspVar のポインタ
      CspVar* operator-> () const { return pv_var; }

      /// \brief CspVar のリファレンスに変換するオペレータ
      ///
      /// \return CspVar のリファレンス
      CspVar& operator* () const { return *pv_var; }

      /// \brief constな CspVar のポインタへのキャスト
      ///
      /// \return constな CspVar のポインタ
      operator CspVar* () const { return pv_var; }

      /// \brief CspVar のポインタへのキャスト
      ///
      /// \return CspVar のポインタ
      CspVar* var (void) const { return pv_var; }

      /// \brief 分割された配列であるかどうか
      ///
      /// \retval true 分割された配列
      /// \retval false 分割された配列か単一の変数
      bool is_partial_array (void) const
      {
	return pv_partial_array.size () != 0;
      }

      /// \brief constな PartialArrayList のリファレンスを取得(参照用)
      ///
      /// \return constな PartialArrayList のリファレンス
      const PartialArrayList& partial_array (void) const
      {
	return pv_partial_array;
      }

      /// \brief PartialArrayList のリファレンスを取得(設定用)
      ///
      /// \return PartialArrayList のリファレンス
      PartialArrayList& partial_array (void) { return pv_partial_array; }

      /// \brief 分割された配列の要素を結合する
      void compact_partial_array (void);

    private:
      /// \brief 伝播する変数( CspVar のポインタ)
      CspVar* pv_var;

      /// \brief 分割された配列の要素のリスト
      PartialArrayList pv_partial_array;
    };

    /// \brief PropagateVar のポインタのベクタの型
    ///
    /// \sa PropagateVarMap
    ///
    /// この型は PropagateVarMap から PropagateVar の出現順序を保持する
    /// 目的で使用される
    typedef std::vector< PropagateVar* > PropagateVarList;

    /// \brief PropagateVar の変数名による連想配列のクラス
    ///
    /// \sa PropagateVar
    /// \sa PropagateVarList
    ///
    /// このクラスでは、継承した連想配列(map)に PropagateVar の本体を格
    /// 納し、出現順は #pvm_order にそのポインタを格納する
    class PropagateVarMap
      : public std::map< std::string, PropagateVar >
    {
    public:
      /// \brief CspVar のポインタを指定し PropagateVar を連想配列に格
      /// 納する
      ///
      /// \param[in] cv CspVar のポインタ
      ///
      /// \return 追加した PropagateVar のリファレンス
      PropagateVar& add_varent (CspVar* cv);

      /// \brief 出現順に格納された PropagateVar のポインタのリストのリ
      /// ファレンスを取得
      ///
      /// \return 出現順に格納された PropagateVar のポインタのリストの
      /// リファレンス
      const PropagateVarList& order (void) const { return pvm_order; }

    private:
      /// \brief 出現順に格納された PropagateVar のポインタのリスト
      PropagateVarList pvm_order;
    };

    //
    // CspNode
    //

    /// \brief CspNode のリスト
    ///
    /// このリストは作成された CspNode 本体を保持する
    ///
    /// \sa CspNode::csp_node()
    typedef std::list < CspNode > cspnode_list_T;

    /// \brief CspNode のリストの const_iterator の型
    typedef cspnode_list_T::const_iterator cspnode_list_const_iterator;

    /// \brief CspNode のリストの iterator の型
    typedef cspnode_list_T::iterator cspnode_list_iterator;

    /// \brief CspEdge のリストの型
    typedef std::list < CspEdge > CspEdgeList;

    /// \brief CspNodeのクラス
    ///
    /// このクラスは複数の CspBlockInfo を内部に持ち、基本的に一つのタ
    /// スクの単位となる。先頭のブロックは、ブロックを代表するもので、
    /// それ以外の他のブロックは、この先頭のブロックと結合する
    class CspNode
    {
    public:
      /// \brief CspNode のコンストラクタ
      ///
      /// \param[in] g グラフ
      /// \param[in] b CspNode の先頭(代表)となる CspBlockInfo のポインタ
      CspNode (CspGraph& g, CspBlockInfo* b)
	: n_index (-1), n_id (-1),
	  n_oneblock (false), n_info (0), n_head_block (b),
	  n_core (-1), n_coretype (SOFTWARE_CORE),
	  n_pos (-1),
	  n_event_subsystem (NULL),
	  n_graph(g), n_valid_vertex(false),
	  n_taskset (NULL),
	  n_rate (0, 0)
      {
      }

      /// \brief 反復子で指定した CspNode を削除する静的メンバ関数
      ///
      /// \param[in] iter 削除する CspNode の位置
      ///
      /// \return 削除した CspNode の次の位置
      static cspnode_list_iterator remove_node (cspnode_list_iterator iter);

      /// \brief 指定した CspNode の削除する静的メンバ関数
      ///
      /// \param[in] x 削除する CspNode
      static void remove_node (CspNode& x);

      /// \brief CspNode を作成する静的メンバ関数
      ///
      /// \param[in] g グラフ
      /// \param[in] binfo CspNode の先頭(代表)となる CspBlockInfo のポインタ
      ///
      /// \return 作成した CspNode のリファレンス
      static CspNode& add_node (CspGraph& g, CspBlockInfo* binfo);

      /// \brief 作成された CspNode のリストのリファレンスを取得する
      /// 静的メンバ関数
      ///
      /// \return 作成された CspNode のリストのリファレンス
      static cspnode_list_T& csp_node (void) { return n_csp_node; }

      /// \brief 作成された CspNode の個数を取得する静的メンバ関数
      ///
      /// \return 作成された CspNode の個数
      static int csp_node_count (void) { return n_csp_node_count; }

      /// \brief CspNode のポインタのconstなベクタのリファレンス取得す
      /// る静的メンバ関数
      ///
      /// \return CspNode のポインタのconstなベクタのリファレンス
      ///
      /// \sa make_nodelist()
      /// \sa index()
      static const CspNodeList& csp_nodelist (void) { return n_csp_nodelist; }

      /// \brief 作成された全ての CspNode に対し CspNode の correct()
      /// を呼び出す静的メンバ関数
      ///
      /// \sa correct()
      static void correct_nodes (void);

      /// \brief 作成された CspNode を元に、ブロックのIDをインデクスと
      /// した CspNode のポインタのベクタを生成する静的メンバ関数
      ///
      /// \sa csp_nodelist()
      static void make_nodelist (void);

      /// \brief 同じ CspNode か調べる(ポインタの一致を判定)
      ///
      /// \param[in] x 比較する CspNode
      ///
      /// \retval true \a x は *this と同一の CspNode
      /// \retval false \a x は *this 異なる CspNode
      bool operator== (const CspNode& x) const { return this == &x; }

      /// \brief この CspNode の  CspNode のポインタのベクタ
      /// #n_csp_nodelist のインデクスを取得
      ///
      /// \return CspNode のポインタのベクタのインデクス
      ///
      /// \sa csp_nodelist()
      int index (void) const { return n_index; }

      /// \brief この CspNode の先頭(代表)の CspBlockInfo のIDを取得
      ///
      /// \return CspBlockInfo のID
      ///
      /// この値は、タスク番号として使用される
      int id (void) const { return n_id; }

      /// \brief この CspNode の先頭(代表)の CspBlockInfo のIDのリファ
      /// レンスを取得
      ///
      /// \return CspBlockInfo のIDのリファレンス
      int& id (void) { return n_id; }

      /// \brief
      /// この CspNode はブロックをマージしていても
      /// 単一のコードのブロックかどうかの判定(SubSystem,SwitchCase,If)
      ///
      /// \retval true 単一のコードのブロック
      /// \retval false それ以外
      bool oneblock (void) const { return n_oneblock; }

      /// \brief
      /// 単一のコードのブロックかどうかの真理値のリファレンスを取得
      ///
      /// \return 真理値のリファレンス
      bool& oneblock (void) { return n_oneblock; }

      /// \brief 単一のコードのブロックかどうかの真理値を設定
      ///
      /// \param[in] o 単一のコードのブロックかどうかの真理値
      void oneblock (bool o) { n_oneblock = o; }

      /// \brief CspNode の先頭(代表)の CspBlockInfo の取得(参照用)
      ///
      /// \return CspNode の先頭(代表)の CspBlockInfo のポインタ
      CspBlockInfo* head_block (void) const { return n_head_block; }

      /// \brief
      /// CspNode の先頭(代表)の CspBlockInfo のリファレンス取得(設定用)
      ///
      /// \return CspNode の先頭(代表)の CspBlockInfo
      /// のポインタのリファレンス
      CspBlockInfo*& head_block (void) { return n_head_block; }

      /// \brief
      /// この CspNode から変数を伝播する後段へのエッジの
      /// constなリストのリファレンスを取得する(参照用)
      ///
      /// \return この CspNode から変数を伝播する後段へのエッジの
      //// constなリストのリファレンス
      const CspEdgeList& forw_edges (void) const { return n_forw_edges; }

      /// \brief
      /// この CspNode から変数を伝播する後段へのエッジのリストの
      /// リファレンスを取得する(設定用)
      ///
      /// \return この CspNode から変数を伝播する後段へのエッジのリスト
      /// のリファレンス
      CspEdgeList& forw_edges (void) { return n_forw_edges; }

      /// \brief
      /// この CspNode へ変数を伝播する前段からのエッジの
      /// constなリストのリファレンスを取得する(参照用)
      ///
      /// \return この CspNode へ変数を伝播する前段からのエッジの
      /// constなリストのリファレンス
      const CspEdgeList& back_edges (void) const { return n_back_edges; }

      /// \brief
      /// この CspNode へ変数を伝播する前段からのエッジのリストの
      /// リファレンスを取得する(参照用)
      ///
      /// \return この CspNode へ変数を伝播する前段からのエッジのリスト
      /// のリファレンス
      CspEdgeList& back_edges (void) { return n_back_edges; }

      /// \brief
      /// この CspNode から指定した CspNode へのエッジを作成する
      ///
      /// \param[in] c エッジを設定する CspNode (エッジのhead)
      ///
      /// \return 作成された CspEdge のポインタのペア。firstはforwで、
      /// secondはback
      std::pair < CspEdge*, CspEdge* > add_edge (CspNode* c);

      /// \brief CspEdge にグラフの頂点(vertex)を設定する
      ///
      /// \return グラフの頂点記述子のリファレンス
      CspGraphVertex& setup_vertex (void)
      {
	n_vertex = boost::add_vertex (n_graph);
	if (n_head_block)
	  {
	    boost::put (boost::vertex_name, n_graph, n_vertex,
			n_head_block->block ()->name ());
	    boost::put (boost::vertex_BlockID, n_graph, n_vertex,
			n_head_block->id ());
	  }
	boost::put (boost::vertex_CspNode, n_graph, n_vertex, this);
	n_valid_vertex = true;
	return n_vertex;
      }

      /// \brief ブロックの CspNode のポインタをクリアし、
      /// グラフの頂点も削除する
      void clear (void);

      /// \brief グラフの頂点として作成済みか調べる
      ///
      /// \retval true グラフの頂点として有効
      /// \retval false グラフの頂点として設定されていない
      bool is_valid_vertex (void) const { return n_valid_vertex; }

      /// \brief
      /// 前段の CspNode を指定して対応するconstな CspEdgeを取得(参照用)
      ///
      /// \return backwardのconstな CspEdge のポインタ
      const CspEdge*
      get_back_edge (const CspNode* c) const
      {
	CspEdgeList::const_iterator
	  l = std::find (back_edges ().begin (),
			   back_edges ().end (), this);
	if (l != back_edges ().end ())
	  return &*l;
	return NULL;
      }

      /// \brief
      /// 前段の CspNode を指定して対応する CspEdgeを取得(設定用)
      ///
      /// \return backwardの CspEdge のポインタ
      CspEdge*
      get_back_edge (CspNode* c)
      {
	CspEdgeList::iterator l = std::find (back_edges ().begin (),
					     back_edges ().end (), c);
	if (l != back_edges ().end ())
	  return &*l;
	return NULL;
      }

      /// \brief
      /// 後段の CspNode を指定して対応するconstな CspEdgeを取得(参照用)
      ///
      /// \return forwardのconstな CspEdge のポインタ
      const CspEdge*
      get_forw_edge (const CspNode* c) const
      {
	CspEdgeList::const_iterator
	  l = std::find (forw_edges ().begin (),
			   forw_edges ().end (), this);
	if (l != forw_edges ().end ())
	  return &*l;
	return NULL;
      }

      /// \brief
      /// 後段の CspNode を指定して対応する CspEdgeを取得(設定用)
      ///
      /// \return forwardの CspEdge のポインタ
      CspEdge*
      get_forw_edge (CspNode* c)
      {
	CspEdgeList::iterator l = std::find (forw_edges ().begin (),
					       forw_edges ().end (), c);
	if (l != forw_edges ().end ())
	  return &*l;
	return NULL;
      }

      /// \brief
      /// 後段の CspNode を指定して、forwardおよび後段の
      /// CspNode からのbackwardのconstな CspEdge のペアを取得
      ///
      /// \return CspEdge のペア。\n
      /// firstはこの CspNode からのforwardのconstな CspEdge のポインタ、\n
      /// secondは後段の CspNode からのbackwardのconstな CspEdge のポインタ
      std::pair < const CspEdge*, const CspEdge* >
      get_edge (const CspNode* c) const
      {
	const CspEdge* b = NULL;
	const CspEdge* f = get_forw_edge (c);
	if (f)
	  {
	    b = c->get_back_edge (this);
	    assert (b);
	  }
	return std::make_pair (f, b);
      }

      /// \brief
      /// 後段の CspNode を指定して、forwardおよび後段の
      /// CspNode からのbackwardの CspEdge のペアを取得
      ///
      /// \return CspEdge のペア。\n
      /// firstはこの CspNode からのforwardのCspEdge のポインタ、\n
      /// secondは後段の CspNode からのbackwardのCspEdge のポインタ
      std::pair < CspEdge*, CspEdge* >
      get_edge (CspNode* c)
      {
	CspEdge* b = NULL;
	CspEdge* f = get_forw_edge (c);
	if (f)
	  {
	    b = c->get_back_edge (this);
	    assert (b);
	  }
	return std::make_pair (f, b);
      }

      /// \brief
      /// この CspNode から指定した後段の CspNode のエッジを削除する
      ///
      /// \param[in] c 後段の CspNode のポインタ
      void
      del_forw_edge (CspNode* c)
      {
	CspEdgeList::iterator
	  i = std::find (n_forw_edges.begin (), n_forw_edges.end (), c);
	if (i != n_forw_edges.end ())
	  n_forw_edges.erase (i);
      }

      /// \brief
      /// この CspNode から指定した前段の CspNode のエッジを削除する
      ///
      /// \param[in] c 前段の CspNode のポインタ
      void
      del_back_edge (CspNode* c)
      {
	CspEdgeList::iterator
	  i = std::find (n_back_edges.begin (), n_back_edges.end (), c);
	if (i != n_back_edges.end ())
	  n_back_edges.erase (i);
      }

      /// \brief
      /// この CspNode と、指定した前段の CspNode とのエッジ
      /// (forward/backward双方)を削除する
      ///
      /// \param[in] c 後段の CspNode のポインタ
      void
      del_edge (CspNode* c)
      {
	del_forw_edge (c);
	c->del_back_edge (c);
      }

      /// \brief
      /// この CspNode を、接続する全ての CspNode と切断する
      /// (全ての CspEdge を削除する)
      void disconnect (void);

      /// \brief
      /// 指定した CspNode に、この CspNode の全てのエッジをコピーし、
      /// この CspNode に対する他の CspNode からのエッジを全て削除する
      ///
      /// \param[in] n エッジをコピーする対象の CspNode のポインタ
      void reconnect (CspNode* n);

      /// \brief この CspNode のコア番号指定を取得する(参照用)
      ///
      /// \return コア番号
      int core (void) const { return n_core; }

      /// \brief この CspNode のコア番号のリファレンスを取得する(設定用)
      ///
      /// \return コア番号のリファレンス
      int& core (void) { return n_core; }

      /// \brief この CspNode のコア番号を指定する
      ///
      /// \param[in] n コア番号
      void core (int n) { n_core = n; }

      /// \brief この CspNode のコード配置対象を取得する(参照用)
      ///
      /// \return コード配置対象
      CoreType coretype (void) const { return n_coretype; }

      /// \brief この CspNode のコード配置対象のリファレンスを取得する(設定用)
      ///
      /// \return コード配置対象のリファレンス
      CoreType& coretype (void) { return n_coretype; }

      /// \brief この CspNode のコード配置対象を指定する
      ///
      /// \param[in] t コード配置対象
      void coretype (CoreType t) { n_coretype = t; }

      /// \brief CspNode 全体に対するこの CspNode の位置を取得(参照用)
      ///
      /// \return ベクタ内の位置
      ///
      /// この値は、全 CspNode を、コードの行番号でソートした際の出現順であり、
      /// 複数の CspNode のコードの出力順序を決定するために使用する
      int pos (void) const { return n_pos; }

      /// \brief
      /// CspNode 全体に対するこの CspNode の位置のリファレンスを取得
      /// (設定用)
      ///
      /// \return ベクタ内の位置のリファレンス
      int& pos (void) { return n_pos; }

      /// \brief CspNode 全体に対するこの CspNode の位置を設定
      ///
      /// \param[in] n ベクタ内の位置
      void pos (int n) { n_pos = n; }

      /// \brief
      /// CspNode に含まれる CspBlockInfo
      /// のポインタのconstなベクタのリファレンスを取得(参照用)
      ///
      /// \return CspNode に含まれる CspBlockInfo
      /// のポインタのconstなベクタのリファレンス
      const csp_block_list_T&
      contain_blocks (void) const
      {
	return n_contain_blocks;
      }

      /// \brief
      /// CspNode に含まれる
      /// CspBlockInfo のポインタのベクタのリファレンスを取得
      ///
      /// \return
      /// CspNode に含まれる
      /// CspBlockInfo のポインタのタのリファレンス
      csp_block_list_T& contain_blocks (void) { return n_contain_blocks; }

      /// \brief
      /// 指定した CspBlockInfo がこの CspNode に含まれるかどうか調べる
      ///
      /// \retval true \a b は、この CspNode に含まれるブロック
      /// \retval false \a b は、この CspNode に含まれないブロック
      bool
      check_block (const CspBlockInfo* b)
      {
	csp_block_list_const_iterator
	  i = std::find (contain_blocks ().begin (),
			 contain_blocks ().end (), b);
	return i != contain_blocks ().end ();
      }

      /// \brief 指定した CspBlockInfo をこの CspNode に追加する
      ///
      /// \param[in] b 追加する CspBlockInfo のポインタ
      void
      add_block (CspBlockInfo* b)
      {
	csp_block_list_T::iterator
	  i = std::find (contain_blocks ().begin (),
			 contain_blocks ().end (), b);
	if (i == contain_blocks ().end ())
	  {
	    contain_blocks ().push_back (b);
	    b->set_node(this);
	  }
      }

      /// \brief
      /// この CspNode に含まれるブロックのvarタグの
      /// constな連想配列を取得(参照用)
      ///
      /// \return

      /// CspNode に含まれるブロックのvarタグのconstな連想配列 VarMap
      /// のリファレンス
      ///
      /// \sa VarMap
      const VarMap& var (void) const { return n_var; }

      /// \brief
      /// この CspNode に含まれるブロックのvarタグの連想配列を取得
      /// (設定用)
      ///
      /// \return CspNode に含まれるブロックのvarタグの連想配列 VarMap
      /// のリファレンス
      ///
      /// \sa VarMap
      VarMap& var (void) { return n_var; }

      /// \brief
      /// 構造体になっているブロックシグナルのリストを取得(参照用)
      ///
      /// \return ブロックシグナルのリストのconstなリファレンス
      ///
      /// \sa ParamList
      const ParamList& signal (void) const { return n_signal; }

      /// \brief
      /// 構造体になっているブロックシグナルのリストを取得(設定用)
      ///
      /// \return ブロックシグナルのリストのリファレンス
      ///
      /// \sa ParamList
      ParamList& signal (void) { return n_signal; }

      /// \brief 状態変数のリストを取得(参照用)
      ///
      /// \return 状態変数のリストのconstなリファレンス
      ///
      /// \sa ParamList
      const ParamList& state (void) const { return n_state; }

      /// \brief 状態変数のリストを取得(設定用)
      ///
      /// \return 状態変数のリストのリファレンス
      ///
      /// \sa ParamList
      ParamList& state (void) { return n_state; }

      /// \brief Trigger Stateのリストを取得(参照用)
      ///
      /// \return Trigger Stateのリストのconstなリファレンス
      ///
      /// \sa ParamList
      const ParamList& trigger_state (void) const { return n_trigger_state; }

      /// \brief Trigger Stateのリストを取得(設定用)
      ///
      /// \return Trigger Stateのリストのリファレンス
      ///
      /// \sa ParamList
      ParamList& trigger_state (void) { return n_trigger_state; }

      /// \brief invariant block signalのリストを取得(参照用)
      ///
      /// \return invariant block signalのリストのconstなリファレンス
      ///
      /// \sa ParamList
      const ParamList& invariant_signal (void) const
      { return n_invariant_signal; }

      /// \brief invariant block signalのリストを取得(設定用)
      ///
      /// \return invariant block signalのリストのリファレンス
      ///
      /// \sa ParamList
      ParamList& invariant_signal (void) { return n_invariant_signal; }

      /// \brief パラメタのリストを取得(参照用)
      ///
      /// \return パラメタのリストのconstなリファレンス
      ///
      /// \sa ParamList
      const ParamList& param (void) const { return n_param; }

      /// \brief パラメタのリストを取得(設定用)
      ///
      /// \return パラメタのリストのリファレンス
      ///
      /// \sa ParamList
      ParamList& param (void) { return n_param; }

      /// \brief 定数パラメタのリストを取得(参照用)
      ///
      /// \return 定数パラメタのリストのconstなリファレンス
      ///
      /// \sa ParamList
      const ParamList& const_param (void) const { return n_const_param; }

      /// \brief 定数パラメタのリストを取得(設定用)
      ///
      /// \return 定数パラメタのリストのリファレンス
      ///
      /// \sa ParamList
      ParamList& const_param (void) { return n_const_param; }

      /// \brief コードのリストを取得(参照用)
      ///
      /// \return コードのリストのconstなリファレンス
      const CodeList& code (void) const { return n_code; }

      /// \brief コードのリストを取得(設定用)
      ///
      /// \return コードのリストのリファレンス
      CodeList& code (void) { return n_code; }

      /// \brief イベントの送出元のSubSystemの CspNode を取得(参照用)
      ///
      /// \retval NULL イベントは不要
      /// \retval それ以外 イベントの送出元の CspNode のconstなポインタ
      ///
      /// Enabled/Triggered Subsystem等では、SubSystem内のブロックの実
      /// 行は、条件が成立した時に行う。そのためEnabled/Triggered
      /// SubSystem等のブロックと、そのSubSystem内のブロックが異なる
      /// CspNode の場合、ブロックを実行するためのイベントを送出する必
      /// 要がある。
      CspNode* event_subsystem (void) const { return n_event_subsystem; }

      /// \brief

      /// イベントの送出元のSubSystemの CspNode
      /// のポインタのリファレンスを取得(設定用)
      ///
      /// \return イベントの送出元のSubSystemの
      /// CspNode のポインタのリファレンス
      CspNode*& event_subsystem (void) { return n_event_subsystem; }

      /// \brief アクセスする dsm_info (DataStoreMemory)のリスト
      /// dsm_info_list_T のconstなリファレンスを取得(参照用)
      ///
      /// \return dsm_info_list_T のconstなリファレンス
      ///
      /// \sa dsm_info_list_T
      const dsm_info_list_T& dsm (void) const { return n_dsm; }

      /// \brief アクセスする dsm_info (DataStoreMemory)のリスト
      /// (sm_info_list_T のリファレンスを取得(設定用)
      ///
      /// \return dsm_info_list_T のリファレンス
      ///
      /// \sa dsm_info_list_T
      dsm_info_list_T& dsm (void) { return n_dsm; }

      /// \brief
      /// この CspNode に関連付けられた TaskSet のポインタを取得(参照用)
      ///
      /// \return TaskSet のポインタ
      ///
      /// \sa TaskSet
      ///
      /// TaskSet は、例えばコア割り当てされた CspNode
      /// をまとめるためのもので、そうでない通常の場合は CspNode
      /// と TaskSetは1対1である。
      TaskSet* taskset (void) const { return n_taskset; }

      /// \brief
      /// この CspNode に関連付けられた TaskSet
      /// のポインタのリファレンスを取得(設定用)
      ///
      /// \return TaskSet のポインタ
      TaskSet*& taskset (void) { return n_taskset; }

      /// \brief

      /// この CspNode のinputとなる、blockのvarタグの PropagateVar
      /// の連想配列のconstなリファレンスを取得(参照用)
      ///

      /// \return inputとなる、blockのvarタグの PropagateVar
      /// の連想配列のconstなリファレンス
      ///
      /// \sa PropagateVarMap
      const PropagateVarMap& input_var (void) const { return n_input_var; }

      /// \brief この CspNode のinputとなる、blockのvarタグの
      /// PropagateVar の連想配列のリファレンスを取得(設定用)
      ///
      /// \return inputとなる、blockのvarタグの PropagateVar
      /// の連想配列のリファレンス
      ///
      /// \sa PropagateVarMap
      PropagateVarMap& input_var (void) { return n_input_var; }

      /// \brief この CspNode のoutputとなる、blockのvarタグの
      /// PropagateVar の連想配列のconstなリファレンスを取得(参照用)
      ///
      /// \return outputとなる、blockのvarタグの PropagateVar
      /// の連想配列のconstなリファレンス
      ///
      /// \sa PropagateVarMap
      const PropagateVarMap& output_var (void) const { return n_output_var; }

      /// \brief この CspNode のoutputとなる、blockのvarタグの
      /// PropagateVar の連想配列のリファレンスを取得(設定用)
      ///
      /// \return outputとなる、blockのvarタグの PropagateVar
      /// の連想配列のリファレンス
      ///
      /// \sa PropagateVarMap
      PropagateVarMap& output_var (void) { return n_output_var; }

      /// \brief この CspNode のinputでもなくoutputでもなく、
      /// 自動変数となるblockのvarタグの PropagateVar
      /// の連想配列のconstなリファレンスを取得(参照用)
      ///
      /// \return 自動変数となる、blockのvarタグの PropagateVar
      /// の連想配列のconstなリファレンス
      ///
      /// \sa PropagateVarMap
      const PropagateVarMap& auto_var (void) const { return n_auto_var; }

      /// \brief この CspNode のinputでもなくoutputでもなく、
      /// 自動変数となるblockのvarタグの PropagateVar
      /// の連想配列のリファレンスを取得(参照用)
      ///
      /// \return 自動変数となる、blockのvarタグの PropagateVar
      /// の連想配列のリファレンス
      ///
      /// \sa PropagateVarMap
      PropagateVarMap& auto_var (void) { return n_auto_var; }

      /// \brief この CspNode の外部入力となる、blockのvarタグの
      /// PropagateVar のポインタのベクタの
      /// constなリファレンスを取得(参照用)
      ///
      /// \return 外部入力となる、blockのvarタグの PropagateVar
      /// のポインタのベクタのconstなリファレンス
      ///
      /// \sa PropagateVarList
      /// \sa input_var()
      ///
      /// このポインタのベクタに格納されているポインタは output_var()
      /// で取得される連想配列で、extinとなっている PropagateVar
      /// のポインタである
      const PropagateVarList& extin (void) const { return n_extin; }

      /// \brief この CspNode の外部入力となる、blockのvarタグの
      /// PropagateVar のポインタのベクタのリファレンスを取得(設定用)
      ///
      /// \return 外部入力となる、blockのvarタグの PropagateVar
      /// のポインタのベクタのリファレンス
      ///
      /// \sa PropagateVarList
      PropagateVarList& extin (void) { return n_extin; }

      /// \brief この CspNode の外部出力となる、blockのvarタグの
      /// PropagateVar のポインタのベクタの
      /// constなリファレンスを取得(参照用)
      ///
      /// \return 外部出力となる、blockのvarタグの PropagateVar
      /// のポインタのベクタのconstなリファレンス
      ///
      /// \sa PropagateVarList
      /// \sa output_var()
      ///
      /// このポインタのベクタに格納されているポインタは output_var()
      /// で取得される連想配列で、extoutとなっている PropagateVar
      /// のポインタである
      const PropagateVarList& extout (void) const { return n_extout; }

      /// \brief この CspNode の外部出力となる、blockのvarタグの
      /// PropagateVar のポインタのベクタのリファレンスを取得(設定用)
      ///
      /// \return 外部出力となる、blockのvarタグの PropagateVar
      /// のポインタのベクタのリファレンス
      ///
      /// \sa PropagateVarList
      PropagateVarList& extout (void) { return n_extout; }

      /// \brief この CspNode のブロックから呼び出す関数のリストの
      /// constなリファレンスの取得(参照用)
      ///
      /// \return function_ptr_list_T のconstなリファレンス
      ///
      /// \sa function_ptr_list_T
      /// \sa C_Codewriter::functions()
      ///
      /// この functin_ptr_list_T に設定される functin_ent_T
      /// のポインタは C_Codewriter の C_Codewriter::functions()
      /// の連想配列に格納されている function_ent_T のポインタである。
      const function_ptr_list_T& functions (void) const { return n_functions; }

      /// \brief
      /// この CspNode
      /// のブロックから呼び出す関数のリストのリファレンスの取得(設定用)
      ///
      /// \return function_ptr_list_T のリファレンス
      function_ptr_list_T& functions (void) { return n_functions; }

      /// \brief CspNode の周期の取得(参照用)
      ///
      /// \return 周期
      ///
      /// \sa SampleTime::cvalue()
      /// \sa SampleTime::coffset()
      /// \sa rate_T
      ///
      /// この値は CspBlockInfo の SampleTime::cvalue() と
      /// SampleTime::coffset() が設定される
      const rate_T& rate (void) const { return n_rate; }

      /// \brief CspNode の周期のリファレンスの取得(設定用)
      ///
      /// \return 周期のリファレンス
      rate_T& rate (void) { return n_rate; }

      /// \brief 指定した CspNode をこの CspNode に結合し、指定した
      /// CspNode は破棄する
      ///
      /// \param[in] x 結合し破棄する CspNode
      void join (CspNode& x);

      /// \brief 指定した CspNode に含まれる CspBlockInfo をこの
      /// CspNode にコピーする
      ///
      /// \param[in] x コピー元の CspNode
      void copy_contain_blocks (const CspNode& x);

      /// \brief 作成したこの CspNode に対し、BLXMLの各タグの内部データ
      /// 化を行う
      ///
      /// \sa correct_nodes()
      void correct (void);

      /// \brief 指定した CspBlockInfo がこの CspNode
      /// に含まれるか調べる
      ///
      /// \retval true 含まれる
      /// \retval false 含まれない
      bool
      is_contained (const CspBlockInfo* bi) const
      {
	csp_block_list_const_iterator
	  i = find (n_contain_blocks.begin (), n_contain_blocks.end (), bi);
	return i != n_contain_blocks.end ();
      }

      /// \brief CspNode に設定されている属性 CSP_NODE_INFO を取得(参照用)
      ///
      /// \return CspNode の属性
      ///
      /// \sa CSP_NODE_INFO
      unsigned int info (void) const { return n_info; }

      /// \brief CspNode の属性のリファレンスを取得(設定用)
      ///
      /// \return CspNode の属性のリファレンス
      unsigned int& info (void) { return n_info; }

      /// \brief CspNode のデバッグのための内容のダンプ
      void dump (void) const;

      /// \brief グラフの取得
      ///
      /// \return グラフ記述子のリファレンス
      CspGraph& get_graph(void) const { return n_graph; }

      /// \brief CspNode のグラフの頂点のconstなリファレンス(参照用)
      ///
      /// \return グラフの頂点記述子のconstなリファレンス
      const CspGraphVertex& get_vertex(void) const { return n_vertex; }

      /// \brief CspNode のグラフの頂点のなリファレンス(参照用)
      ///
      /// \return グラフの頂点記述子のリファレンス
      CspGraphVertex& get_vertex(void) { return n_vertex; }

      /// \brief イベントの対応が必要なSubSystemかどうか調べる
      ///
      /// \retval true イベントの対応が必要なSubSystem
      /// \retval false 対応不要
      ///
      /// Enabled/Triggerd SubSystemで、単一のSubSystemになっていないと
      /// き、実行条件が成立した際にSubSystem内のブロックをイベントで駆
      /// 動する必要がある
      bool
      need_subsystem_event (void) const
      {
	return ((n_info & (CSP_NODE_INFO_ENABLED_SUBSYSTEM
			   |CSP_NODE_INFO_TRIGGERED_SUBSYSTEM)) != 0
		&& (n_info & CSP_NODE_INFO_ONE_SUBSYSTEM) == 0);
      }

      /// \brief CspNode のブロック名を取得
      ///
      /// \param[in,out] tmp BLXMLにないブロックのタスク用の作業用
      ///
      /// \return ブロック名
      ///
      /// BLXMLのブロックであれば、そのブロック名を返し、
      /// そうでない場合はタスクIDから生成した名前を返す
      const std::string& get_block_name (std::string& tmp) const;

    private:
      /// \brief CspNode に含まれる CspBlockInfo
      /// を指定して、そのブロックの情報をこの CspNode へ取り込む
      ///
      /// \param[in] bi CspBlockInfo のポインタ
      void correct (const CspBlockInfo* bi);

      /// \brief 作成された CspNode のリスト。静的メンバ
      static cspnode_list_T n_csp_node;

      /// \brief 作成された CspNode の個数。静的メンバ
      static int n_csp_node_count;

      /// \brief CspNode のポインタのconstなベクタ。静的メンバ
      static CspNodeList n_csp_nodelist;

      /// \brief この CspNode の CspNode のポインタのベクタ
      /// #n_csp_nodelist のインデクス
      ///
      /// \sa n_csp_nodelist
      int n_index;		// index of CspNode::csp_nodelist

      /// \brief この CspNode の先頭(代表)の CspBlockInfo のID
      int n_id;			// task id

      /// \brief
      /// この CspNode はブロックをマージしていても
      /// 単一のコードのブロックかどうかの真理値
      bool n_oneblock;

      /// \brief CspNode に設定されている属性 CSP_NODE_INFO
      ///
      /// \sa CSP_NODE_INFO
      unsigned int n_info;

      /// \brief CspNode の先頭(代表)の CspBlockInfo
      ///
      /// 先頭のブロックは、ブロックを代表するもので、
      /// それ以外の他のブロックは、この先頭のブロックと結合する
      CspBlockInfo* n_head_block;

      /// \brief CspNode に含まれる CspBlockInfo のポインタのベクタ
      ///
      /// \sa csp_block_list_T
      csp_block_list_T n_contain_blocks;

      /// \brief この CspNode に含まれるブロックのvarタグの連想配列
      ///
      /// \sa VarMap
      VarMap n_var;

      /// \brief 構造体になっているブロックシグナルのリスト
      ParamList n_signal;

      /// \brief 状態変数のリスト
      ParamList n_state;

      /// \brief Trigger Stateのリスト
      ParamList n_trigger_state;

      /// \brief invariant block signalのリスト
      ParamList n_invariant_signal;

      /// \brief パラメタのリスト
      ParamList n_param;

      /// \brief 定数パラメタのリスト
      ParamList n_const_param;

      /// \brief コードのリスト
      CodeList n_code;

      /// \brief この CspNode から変数を伝播する後段へのエッジのリスト
      CspEdgeList n_forw_edges;

      /// \brief この CspNode へ変数を伝播する前段からのエッジリスト
      CspEdgeList n_back_edges;

      /// \brief この CspNode のコア番号指定
      int n_core;

      /// \brief この CspNode のコード配置対象の種別
      ///
      /// \sa CoreType
      CoreType n_coretype;

      /// \brief CspNode 全体に対するこの CspNode の位置
      int n_pos;

      /// \brief イベントの送出元のSubSystemの CspNode
      CspNode* n_event_subsystem;

      /// \brief アクセスする dsm_info (DataStoreMemory)のリスト
      dsm_info_list_T n_dsm;

      /// \brief グラフの記述子
      CspGraph& n_graph;

      /// \brief CspNode のグラフの頂点
      CspGraphVertex n_vertex;

      /// \brief グラフの頂点として作成済みかどうか
      bool n_valid_vertex;

      /// \brief この CspNode に関連付けられた TaskSet のポインタ
      TaskSet* n_taskset;

      /// \brief この CspNode のinputとなる、blockのvarタグの
      /// PropagateVarの連想配列
      PropagateVarMap n_input_var;

      /// \brief この CspNode のoutputとなる、blockのvarタグの
      /// PropagateVarの連想配列
      PropagateVarMap n_output_var;

      /// \brief この CspNode のinputでもなくoutputでもなく、自動変数と
      /// なるblockのvarタグの PropagateVar の連想配列
      PropagateVarMap n_auto_var;

      /// \brief この CspNode の外部入力となる、blockのvarタグの
      /// PropagateVar のポインタのベクタ
      PropagateVarList n_extin;

      /// \brief この CspNode の外部出力となる、blockのvarタグの
      /// PropagateVar のポインタのベクタ
      PropagateVarList n_extout;

      /// \brief この CspNode のブロックから呼び出す関数のリスト
      function_ptr_list_T n_functions;

      /// \brief CspNode の周期
      ///
      /// 周期は周期比(最小のサンプル時間の逓倍)
      /// または実行環境の時間単位(例えばOSのtick)の逓倍である
      ///
      rate_T n_rate;
    };

    /// \brief CspEdge のベースとなる型
    ///
    /// CspNode のEdge情報は、headのノードと、双方の CspNode
    /// に含まれるブロック間の CspBlockConnect
    /// の(ポインタの)リスト\n
    /// これは CspEdge で継承して使用される
    ///
    /// \sa CspNode
    /// \sa csp_const_block_connect_list_T
    /// \sa CspBlockConnect
    /// \sa CspEdge
    typedef std::pair < CspNode*, csp_const_block_connect_list_T > csp_edge_T;

    ///
    /// \brief Cspのエッジのクラス
    ///
    /// csp_edge_T を継承し、それに対するメソッドを追加したクラス
    ///
    /// \sa csp_edge_T
    class CspEdge : public csp_edge_T
    {
    public:
      /// \brief CspEdge のコンストラクタ
      ///
      /// \param[in] g グラフ
      /// \param[in] t エッジのtailの CspNode
      /// \param[in] h エッジのheadの CspNode
      /// \param[in] link グラフのエッジを作成するかどうかのフラグ
      CspEdge (CspGraph& g, CspNode& t, CspNode* h, bool link = false)
	: e_info (CSP_EDGE_INFO_NONE), e_pair (NULL), e_tail(t), e_graph(g)
      {
	if (link)
	  {
	    if (!t.is_valid_vertex ())
	      t.setup_vertex ();
	    if (!h->is_valid_vertex ())
	      h->setup_vertex ();

	    CspGraphVertex& vt = t.get_vertex(); // tail
	    CspGraphVertex& vh = h->get_vertex(); // head
	    e_edge = boost::add_edge(vt, vh, g).first;
	    boost::put(boost::edge_CspEdge, g, e_edge, this);
	  }
	first = h;
      }

      /// \brief CspNode のポインタへのキャスト
      ///
      /// \return エッジの頭(HEAD)の CspNode のポインタ
      operator CspNode* () const { return first; }

      /// \brief エッジの頭(HEAD)の CspNode のポインタを取得(参照用)
      ///
      /// \return CspNode のポインタ
      CspNode* node (void) const { return first; }

      /// \brief エッジの頭(HEAD)の CspNode
      /// のポインタのリファレンスを取得(設定用)
      ///
      /// \return エッジの頭(HEAD)の CspNode のポインタのリファレンス
      CspNode*& node (void) { return first; }

      /// \brief エッジに含まれる CspBlockConnect のポインタのベクタ
      /// csp_const_block_connect_list_T
      /// のconstなリファレンスを取得(参照用)
      ///
      /// \return csp_const_block_connect_list_T のconstなリファレンス
      ///
      /// \sa csp_const_block_connect_list_T
      const csp_const_block_connect_list_T&
      block_connect_list (void) const { return second; }

      /// \brief エッジに含まれる CspBlockConnect のポインタのベクタ
      /// csp_const_block_connect_list_T のリファレンスを取得(設定用)
      ///
      /// \return csp_const_block_connect_list_T のリファレンス
      ///
      /// \sa csp_const_block_connect_list_T
      csp_const_block_connect_list_T&
      block_connect_list (void) { return second; }

      /// \brief 指定された CspBlockConnect のポインタのベクタ
      /// csp_const_block_connect_list_T からこのエッジに
      /// CspBlockConnect を全てコピーする
      ///
      /// \param[in] l CspBlockConnect のポインタのベクタ
      void
      copy_connect (const csp_const_block_connect_list_T& l)
      {
	for (csp_const_block_connect_list_T::const_iterator i (l.begin ());
	     i != l.end (); i++)
	  block_connect_list ().push_back (*i);
      }

      /// \brief CSPエッジの属性を取得(参照用)
      ///
      /// \return CSPエッジの属性
      ///
      /// \sa CSP_EDGE_INFO
      unsigned int info (void) const { return e_info; }

      /// \brief CSPエッジの属性のリファレンス取得(設定用)
      ///
      /// \return CSPエッジの属性のリファレンス
      unsigned int& info (void) { return e_info; }

      /// \brief 新たな CspBlockConnect を作成、保持するリスト
      /// csp_block_connect_list_T のリファレンスの取得
      ///
      /// \return csp_block_connect_list_T のリファレンス
      ///
      /// \sa new_connect()
      ///
      /// new_connect() で使用する
      csp_block_connect_list_T& any_connect (void) { return e_any_connect; }

      /// \brief このエッジと対となる逆方法のエッジの取得(参照用)
      ///
      /// \return 対になる逆方法の CspEdge のポインタ
      CspEdge* pair (void) const { return e_pair; }

      /// \brief このエッジと対になる逆方法のエッジのリファレンスの取得
      /// (設定用)
      ///
      /// \return 対となる逆方法の CspEdge のポインタのリファレンス
      CspEdge*& pair (void) { return e_pair; }

      /// \brief このエッジで伝播する変数の連想配列のconstなリファレン
      /// スの取得(参照用)
      ///
      /// \return 伝播する変数の連想配列のconstなリファレンス
      ///
      /// この PropagateVarMap は CspBlockConnect の
      /// CspBlockConnect::var_list() つまりはblockのforward/backwardの
      /// varタグから生成されており CspNode の PropagateVar とは実体が
      /// 異なる
      const PropagateVarMap& var (void) const { return e_var; }

      /// \brief このエッジで伝播する変数の連想配列のリファレンスの取得
      /// (設定用)
      ///
      /// \return 伝播する変数の連想配列のリファレンス
      PropagateVarMap& var (void) { return e_var; }

      /// \brief 新たな BlockConnect を生成する
      ///
      /// \param[in] tail エッジの尾(tail)
      /// \param[in] head エッジの頭(head)
      ///
      /// \return 生成された CspBlockConnect のリファレンス
      ///
      /// 方向は、尾(tail)->頭(head)である。
      CspBlockConnect&
      new_connect (CspBlockInfo* tail, CspBlockInfo* head)
      {
	e_any_connect.push_back (CspBlockConnect (tail, head));
	CspBlockConnect& bc = e_any_connect.back ();
	block_connect_list ().push_back (&bc);
	return bc;
      }

      /// \brief このエッジの尾(tail)の CspNode を取得
      ///
      /// \return エッジの尾(tail)の CspNode
      CspNode& tail (void) const { return e_tail; }

      /// \brief グラフ記述子の取得
      ///
      /// \return グラフ記述子のリファレンス
      CspGraph& get_graph (void) const { return e_graph; }

      /// \brief エッジ記述子のconstなリファレンスの取得(参照用)
      ///
      /// \return エッジ記述子のconstなリファレンス
      const CspGraphEdge& get_edge (void) const { return e_edge; }

      /// \brief エッジ記述子のconstなリファレンスの取得(設定用)
      ///
      /// \return エッジ記述子のリファレンス
      CspGraphEdge& get_edge (void) { return e_edge; }

    private:
      /// \brief エッジの属性
      ///
      /// \sa CSP_EDGE_INFO
      unsigned int e_info;

      /// \brief 新たな CspBlockConnect を作成するためのリスト
      csp_block_connect_list_T e_any_connect;

      /// \brief このエッジと対になっている逆方法のエッジ
      CspEdge* e_pair;

      /// \brief このエッジで伝播する変数の連想配列
      PropagateVarMap e_var;

      /// \brief このエッジの尾(tail)の CspNode
      CspNode& e_tail;

      /// \brief グラフ記述子
      CspGraph& e_graph;

      /// \brief エッジ記述子
      CspGraphEdge e_edge;
    };

    //
    // DataStoreMemory
    //

    /// \brief DataStoreMemoryの情報を保持するクラス
    class dsm_entry
    {
    public:
      /// \brief dsm_entry のコンストラクタ
      ///
      /// \param[in] binfo DataStoreMemoryの CspBlockInfo のポインタ
      dsm_entry (const CspBlockInfo* binfo) : de_blockinfo (binfo) {}

      /// \brief DataStoreMemoryの CspBlockInfo のポインタ取得
      ///
      /// \return DataStoreMemoryの CspBlockInfo のポインタ
      const CspBlockInfo* blockinfo (void) const { return de_blockinfo; }

      /// \brief DataStoreMemoryの初期化コードのconstなリファレンスの取
      /// 得(参照用)
      ///
      /// \return DataStoreMemoryの初期化コードのconstなリファレンス
      const std::string& init (void) const { return de_init; }

      /// \brief DataStoreMemoryの初期化コードのリファレンスの取得(設定
      /// 用)
      ///
      /// \return DataStoreMemoryの初期化コードのリファレンス
      std::string& init (void) { return de_init; }

      /// \brief DataStoreMemoryの状態変数のリストのconstなリファレンス
      /// の取得(参照用)
      ///
      /// \return DataStoreMemoryの状態変数のリストのconstなリファレン
      /// ス
      const ParamList& dsm_state (void) const { return de_dsm_state; }

      /// \brief DataStoreMemoryの状態変数のリストのリファレンスの取得
      /// (設定用)
      ///
      /// \return DataStoreMemoryの状態変数のリストのconstなリファレン
      /// ス
      ParamList& dsm_state (void) { return de_dsm_state; }

      /// \brief DataStoreMemoryのパラメタのリストのconstなリファレンス
      /// の取得(参照用)
      ///
      /// \return DataStoreMemoryのパラメタのリストのconstなリファレン
      /// ス
      const ParamList& dsm_params (void) const { return de_dsm_params; }

      /// \brief DataStoreMemoryのパラメタのリストのリファレンスの取得
      /// (設定用)
      ///
      /// \return DataStoreMemoryのパラメタのリストのconstなリファレン
      /// ス
      ParamList& dsm_params (void) { return de_dsm_params; }

    private:
      /// \brief DataStoreMemoryの CspBlockInfo のポインタ
      const CspBlockInfo* de_blockinfo;

      /// \brief DataStoreMemoryの初期化コード
      std::string de_init;

      /// \brief DataStoreMemoryの状態変数のリスト
      ParamList de_dsm_state;

      /// \brief DataStoreMemoryのパラメタのリスト
      ParamList de_dsm_params;
    };

    /// \brief DataStoreMemoryの名前をキーとしたDataStoreMemoryの連想配
    /// 列(map)
    ///
    typedef std::map < std::string, dsm_entry > dsm_map_T;

    //
    // CspReader
    //

    /// \brief CspReader のクラス
    ///
    /// このクラスは base_CspReader を継承した、BLXMLを内部の情報に読み
    /// 込み、処理するクラスである
    ///
    /// \sa base_CspReader
    class CspReader : public base_CspReader
    {
    public:
      /// \brief BLXMLの主にblock以外の情報を読み込む
      ///
      /// \retval 0 正常
      /// \retval -1 エラー
      ///
      /// この関数では C_CodeWriter 内に、以下の読み込みを行う
      /// - Real-Time Model Objectの宣言や定義、初期化コード\n
      /// - 共通コード、共通初期化コード、自動変数、マクロ\n
      /// - Real-Time Model Objectの型や、その他の構造体の宣言や定義
      /// - インクルードファイル
      ///
      /// \sa C_CodeWriter
      int EC_code (void);

      /// \brief BLXMLの関数宣言・定義を C_CodeWriter に読み込む
      ///
      /// \retval 0 正常
      /// \retval -1 エラー
      ///
      /// この処理では、以下の事を行う
      /// -# BLXMLの関数の宣言や定義を読み込む
      /// -# それぞれの関数のコードから、呼び出している関数を見つけ、
      ///    その関数のデータとして登録する
      ///
      /// \sa C_CodeWriter
      int function_code (void);

      /// \brief 処理の準備としてクラス内のメンバの初期化を行う
      void prepare_process (void);

      /// \brief 継承されるサンプル時間の解決を行う
      ///
      /// \retval 0 正常
      /// \retval -1 エラー
      ///
      /// この処理では以下の事を行う
      /// -# 値をもつブロックのパラメタを抽出する
      /// -# 各ブロックに指定されているサンプル時間を設定する
      ///    - 一部のブロックについては、前段または後段からの継承を禁止する。\n
      ///      例えばTriggerPortのように後段から継承できないものは、
      ///      後段からの継承を禁止する
      /// -# Atomic SubSystemやTriggered SubSystemなどのブロックに対し、
      ///    後段から継承を禁止する
      /// -# DiscretePulseGeneratorで低レート可な場合は低レート化する
      /// -# TIDを持つコードでサンプル時間が決定している場合、
      ///    そのサンプル時間を同じTIDを持つブロックに対するヒントとし、
      ///    サンプル時間が継承であって同じTIDを持つブロックのサンプル時間を
      ///    決定する
      /// -# 変更がなくなるまでサンプル時間を決定するループ処理を行う
      ///    -# サンプル時間を決定しているブロックから後段へサンプル時間を
      ///       伝播させ、変化が生じている間はそれを繰り返す\n
      ///       inherit_forward()
      ///    -# 上記の処理の前から変更があったら、ループを最初から継続する
      ///    -# サンプル時間が決定しているブロックから前段へサンプル時間を
      ///       伝播させ、変化が生じている間はそれを繰り返す\n
      ///       inherit_backward()
      ///    -# 上記の処理の前から変更があったら、ループを最初から継続する
      ///    -# 遅いサンプル時間から色分けとサンプル時間の決定処理を行う\n
      ///       color_forward(),color_backward(),check_input_color()
      ///       -# サンプル時間が継承となっているか、色分けするサンプル
      ///          時間より遅いレートの入力を持つブロックを色分けする
      ///       -# 色分けしたブロックのサンプル時間を決定する
      ///    -# 上記の処理の前から変更があったら、ループを最初から継続する
      ///    -# 一度だけサンプル時間がinfのもの継承に変更する
      ///    -# ループを最初から継続する
      ///
      /// \sa SampleTime
      /// \sa Inherit_SampleTime(const CspBlockInfo* from,
      ///     CspBlockInfo* to, int dir)
      /// \sa Inherit_Subsystem_SampleTime()
      /// \sa inherit_forward()
      /// \sa inherit_backward()
      /// \sa color_forward()
      /// \sa color_backward()
      /// \sa check_input_color()
      int Inherit_SampleTime (void);

      /// \brief BLXMLのブロック情報からCspNodeを作成する
      ///
      /// この処理では以下の事を行う
      /// -# 外部入出力の判定とブロックのソート準備
      /// -# コード上マージされているブロックの識別
      /// -# ユーザーが結合と指示したブロックの識別
      /// -# ブロックのタスク属性設定
      /// -# CspNode の作成
      /// -# マージ対象のブロックを CspNode へ結合
      /// -# Delayブロック(UPDATEを持つ)を結合
      /// -# 定数伝播
      /// -# CspNode 内のブロックを出現順にソート
      /// -# CspNode に出現順を設定
      void Make_Nodes (void);

      /// \brief CspNode 間のエッジを生成する
      ///
      /// この処理では以下の事を行う
      /// -# Trigger/Enable Subsystemのイベントのエッジを設定する
      /// -# CspNode 内のブロックの接続関係からエッジを生成する
      /// -# 孤立したブロック、末端(出力がない)ブロックの削除
      void Make_Edges (void);

      /// \brief コード生成に必要な情報を生成する
      ///
      /// \retval 0 正常
      /// \retval -1 エラー
      ///
      /// この処理では以下の事を行う
      /// -# task0を生成するオプション指定がある場合
      ///    -# task0の作成
      ///    -# extoutの作成
      /// -# DataStorememoryのコード生成情報を C_CodeWrite へ設定
      /// -# CspNode と C_CodeWriter の情報設定
      /// -# CspEdge の設定
      /// -# CspNode のforward/backwardの CspEdge の伝播変数が分割され
      ///    た配列であれば、その結合
      int Make_Context (void);

      /// \brief CspNode の情報をダンプする(デバッグ用)
      void Dump_Nodes (void);

      /// \brief コードから呼び出す関数を各対象のリストに設定する
      ///
      /// \retval 0 正常
      /// \retval -1 エラー
      ///
      /// この処理では以下の事を行う
      /// -# 定義・宣言されている関数について、間数名の長い順にソートし、
      ///    関数呼び出しを見つけるための正規表現を作成する
      /// -# Real-Time Model Objectに関する共通コードから呼び出す関数を
      ///    調べる
      /// -# Real-Time Model Objectに関する共通初期化コードから呼び出す
      ///    関数を調べる
      /// -# 共通コードから呼び出す関数を調べる
      /// -# 共通初期化コードから呼び出す関数を調べる
      /// -# 各 CspNode のコードから呼び出す関数を調べる
      ///
      int set_function_info (void);

      /// \brief BLXMLを入力し、内部データを構築する
      ///
      /// \param[in] file 入力BLXMLファイル名
      /// \param[in] ns スキーマの名前空間
      /// \param[in] sc スキーマファイル名
      /// \param[in] validation スキーマとvalidationを行うかどうか
      ///
      /// \retval 0 正常
      /// \retval -1 エラー
      ///
      /// 以下の処理を行う
      /// -# BLXMLの入力 (SimulinkXML::load_xml())
      /// -# BLXMLの主にblock以外の情報の読み込み\n
      ///    EC_code(void)
      /// -# BLXMLの関数宣言・定義の読み込み\n
      ///    function_code(void)
      /// -# 処理の準備\n
      ///    prepare_process(void)
      /// -# サンプル時間の継承の解決\n
      ///    Inherit_SampleTime(void)
      /// -# CspNode の作成\n
      ///    Make_Nodes(void)
      /// -# CspNode 間の CspEdge の生成\n
      ///    Make_Edges(void)
      /// -# コード生成に必要な情報の生成\n
      ///    Make_Context(void)
      /// -# コードから呼び出す関数の対応\n
      ///    set_function_info(void)
      ///
      /// \sa EC_code()
      /// \sa function_code()
      /// \sa prepare_process()
      /// \sa Inherit_SampleTime()
      /// \sa Make_Nodes()
      /// \sa Make_Edges()
      /// \sa Make_Context()
      /// \sa Dump_Nodes()
      /// \sa set_function_info()
      int input_xml (const char* file,
		     const char* ns = NULL, const char* sc = NULL,
		     bool validation = false);

      /// \brief BLXMLを入力し、内部データを構築する
      ///
      /// \param[in] file 入力BLXMLファイル名
      /// \param[in] validation Schemaとvalidationを行うかどうか
      ///
      /// \retval 0 正常
      /// \retval -1 エラー
      int input_xml (const char* file, bool validation = false);

      /// \brief CSPのグラフをファイルにDOT形式で出力する
      ///
      /// \param[in] file 出力ファイル
      ///
      /// \retval 0 正常
      /// \retval -1 エラー
      int output_csp (const char* file);

      /// \brief CspReader からboolへのキャスト
      ///
      /// \retval true 利用可能な状態
      /// \retval false 利用不可
      ///
      /// CspReader が利用可能な状態にあるかどうかを調べる
      operator bool () const
      {
	const base_CspReader* r = this;
	return *r;
      }

      /// \brief Real-Time Model Objectのポインタ変数の名前を取得
      ///
      /// \return Real-Time Model Objectのポインタ変数の名前
      const std::string&
      rtm_variable (void) const { return c_rtm_variable; }

      /// \brief CspReader のコンストラクタ
      CspReader ()
	: single_rate (true), need_extout (false), need_extin (false),
	  has_error(false)
      {
	boost::set_property (get_graph (), boost::graph_name, "csp");
      }

      /// \brief 読み込まれたものは単一レートかどうかの真理値を取得(参
      /// 照用)
      ///
      /// \retval true 単一レート
      /// \retval false マルチレート
      bool is_single_rate (void) const { return single_rate; }

      /// \brief 読み込まれたものは単一レートかどうかの真理値のリファレ
      /// ンスを取得(設定用)
      ///
      /// \return シングルレートかどうかの真理値のリファレンス
      bool& is_single_rate (void) { return single_rate; }

      /// \brief DataStoreMemoryの名前をキーとしたDataStoreMemoryの連想
      /// 配列のconstなリファレンスを取得(参照用)
      ///
      /// \return DataStoreMemoryの連想配列のconstなリファレンス
      const dsm_map_T& dsm_map (void) const { return cr_dsm_map; }

      /// \brief DataStoreMemoryの名前をキーとしたDataStoreMemoryの連想
      /// 配列のリファレンスを取得(設定用)
      ///
      /// \return DataStoreMemoryの連想配列のリファレンス
      dsm_map_T& dsm_map (void) { return cr_dsm_map; }

#if 0
      /// \brief 全ブロックのレートのダンプ(デバッグ用)
      ///
      /// \param[in] inherited_only trueなら継承となっているものだけを
      /// 表示、falseなら全て
      void dump_rate (bool inherited_only = false) const;
#endif

      /// \brief 指定したブロックのレートを表示
      ///
      /// \param[in] binfo 指定したブロック CspBlockInfo のポインタ
      /// \param[in] ind 表示時のインデント(字下げ)
      /// \param[in] prefix ブロック名の前に表示するstring。
      /// 例えば"->"とか"<-"
      void print_block_rate (const CspBlockInfo* binfo, int ind,
			     const std::string prefix) const;

      /// \brief ブロックの入力側のブロックのレートを表示する
      ///
      /// \param[in] binfo 指定したブロック CspBlockInfo のポインタ
      /// \param[in] ind 表示時のインデント(字下げ)
      ///
      /// 入力側のブロックを再帰的にたどり、各ブロックについて
      /// print_block_rate() を呼び出す
      ///
      /// \sa print_block_rate()
      void dump_block_rate_input (const CspBlockInfo* binfo, int ind) const;

      /// \brief ブロックの出力側のブロックのレートを表示する
      ///
      /// \param[in] binfo 指定したブロック CspBlockInfo のポインタ
      /// \param[in] ind 表示時のインデント(字下げ)
      ///
      /// 出力側のブロックを再帰的にたどり、各ブロックについて
      /// print_block_rate() を呼び出す
      ///
      /// \sa print_block_rate()
      void dump_block_rate_output (const CspBlockInfo* binfo, int ind) const;

      /// \brief ブロックを再帰的にたどりレートを表示する
      ///
      /// \param[in] b BLXMLのblockのタグ
      /// \param[in] ind 表示時のインデント(字下げ)
      /// \param[in] no_io 入出力側それぞれのブロックを再帰的にたどるかどうか。
      /// trueなら再帰的に辿り、falseなら辿らない
      ///
      /// \sa print_block_rate()
      /// \sa dump_block_rate_input()
      /// \sa dump_block_rate_output()
      /// \sa dump_block_rates()
      void dump_block_rate (const SimulinkModel::XSD::block_T& b, int ind,
			    bool no_io = false) const;

      /// \brief SubSystemまたはトップレベルのブロックについてレートを
      /// 表示する
      ///
      /// \param[in] bs BLXMLのblocksタグ
      /// \param[in] ind 表示時のインデント(字下げ)
      /// \param[in] no_io 入出力側それぞれのブロックを再帰的にたどるかどうか。
      /// trueなら再帰的に辿り、falseなら辿らない
      ///
      /// \sa dump_block_rate()
      void dump_block_rates (const SimulinkModel::XSD::blocks_T& bs, int ind,
			     bool no_io = false) const;

      /// \brief 全てのブロックについてレートを表示する
      ///
      /// \param[in] no_io 入出力側それぞれのブロックを再帰的にたどるかどうか。
      /// trueなら再帰的に辿り、falseなら辿らない
      ///
      /// \sa dump_block_rates()
      void dump_block_rate (bool no_io = false) const
      {
	std::cout << "---- block rate (with";
	if (!no_io)
	  std::cout << "out";
	std::cout << " input/output) ----" << std::endl;
	dump_block_rates (xml_blocks (), 0, no_io);
      }

      /// \brief CspReader の処理でエラーがあったかどうかの真理値を取得
      /// する
      ///
      /// \retval true エラー発生
      /// \retval false エラーなし
      bool is_error(void) const { return has_error; }

      /// \brief constなグラフ記述子を取得する(参照用)
      ///
      /// \return constなグラフ記述子のリファレンス
      const CspGraph& get_graph (void) const { return csp_graph; }

      /// \brief グラフ記述子のリファレンスを取得する(設定用)
      ///
      /// \return グラフ記述子のリファレンス
      CspGraph& get_graph (void) { return csp_graph; }

      /// \brief グラフのエッジに変数名のラベルを設定する
      ///
      /// \param[in] e 対象のグラフのエッジ記述子
      /// \param[in] var 変数名
      void
      add_label_to_edge (CspGraphEdge& e, const std::string& var);

      /// \brief CspEdgeに変数名のラベルを設定する
      ///
      /// \param[in] e 対象の CspEdge
      /// \param[in] var 変数名
      void
      add_label_to_edge (CspEdge& e, const std::string& var);

      /// \brief CspEdgeで伝播する変数をグラフのエッジのラベルに設定する
      ///
      /// \param[in] ce 対象の CspEdge
      void
      add_var_list_label_to_edge (CspEdge& ce);

    private:
      /// \brief 指定したブロック名に対応する CspBlockInfo を見つける
      ///
      /// \param[in] block ブロック名
      ///
      /// \retval NULL ブロックが見つからなかった
      /// \retval それ以外 ブロックの CspBlockInfo のポインタ
      CspBlockInfo* find_block (const std::string& block) const
      {
	// ignore const
	const CspBlockInfo* binfo = base_CspReader::find_block (block);
	return const_cast< CspBlockInfo* > (binfo);
      }

      /// \brief BLXMLから入力したブロックのコードを
      /// CspNode の情報に取り込む
      ///
      /// \param[in] cn 対象の CspNode のポインタ
      /// \param[in] name ブロックの名前
      /// \param[in] ce code_ent_T
      ///
      /// この処理では以下の事を行う
      /// - コードが"task"または"update"の場合はブロックのコード中にあ
      ///   る指示に従って断片化したコードを取り込む
      ///   - Real-Time Model Objectを参照する必要があれば CspNode に、
      ///     その属性を設定する
      /// - それ以外のコードは、そのまま取り込む
      ///    - コードが"init", "state"であれば、自動変数があることが間
      ///      が得られるため、Cのブロックにする
      ///
      /// \sa code_ent_T
      void
      import_code (CspNode* cn, const std::string& name, code_ent_T& ce);

      /// \brief UPDATEの入力が CspNode 内の他のブロックの入力になって
      /// いないか調べる
      ///
      /// \param[in] cn_tail 出力元の CspNode のポインタ
      /// \param[in] cn_head 入力元のCspNodeのポインタ
      ///
      /// \retval true 他の入力になっている
      /// \retval false UPDATE部のみの入力
      ///
      /// このチェックでtrueとなった場合、UPDATEではなく通常の入力とし
      /// て扱う必要がある
      bool check_delay_as_other_input (const CspNode* cn_tail,
				       const CspNode* cn_head) const;

      /// \brief BlockConnectにある伝播する変数をCspEdgeに対して設定す
      /// る
      ///
      /// \param[in] bc 対象の CspBlockConnect
      /// \param[in] cn_tail 出力元の CspNode のポインタ
      /// \param[in] cbi_tail 出力元ブロックの CspBlockInfo のポインタ
      /// \param[in] cn_head 入力元の CspNode のポインタ
      /// \param[in] cbi_head 入力元ブロックの CspBlockInfo のポインタ
      /// \param[in,out] edge 対象の CspEdge
      /// \param[in] multirate 変数をマルチレートとして扱うかどうか
      ///
      /// \retval 0 正常
      /// \retval -1 エラー
      int assign_variables (CspBlockConnect& bc,
			    CspNode* cn_tail, CspBlockInfo* cbi_tail,
			    CspNode* cn_head, CspBlockInfo* cbi_head,
			    CspEdge& edge, bool multirate);

      /// \brief 探索開始ブロックから接続するブロックを探索し、探索開始
      /// ブロックにマージする
      ///
      /// \param[in] tail 探索開始ブロック(SwitchCase/If)
      /// \param[in] index 対象のブロックの、ブロックのポインタの
      /// ベクタのインデクス
      /// \param[in] subsys 探索開始ブロックが含まれるSubSystem
      /// のブロック
      /// \param[in] color グラフの訪問情報(gray,black,whiteを設定)
      ///
      /// 探索開始ブロック(SwitchCase/If)からdepth first searchでMerge
      /// まで探索し、それらを探索開始ブロックへマージする
      void depth_search_and_mark (CspBlockInfo* tail, int index,
				  const CspBlockInfo* subsys, int *color);

      /// \brief check_rtm_types()で見つけたいReal-Time Model Objectに
      /// 関連するパタンマッチの種類
      ///
      /// - CHECK_RTM_VAR
      ///   - Real-Time Model Objectのポインタのパタン
      /// - CHECK_RTM_TID
      ///   - Real-Time Model ObjectのTIDのパタン
      enum  rtm_type { CHECK_RTM_VAR, CHECK_RTM_TID };

      /// \brief 指定されたテキストの範囲に指定した種類のパタンがある
      /// か調べる
      ///
      /// \param[in] beg テキストの開始アドレス
      /// \param[in] end テキストの終了アドレス
      /// \param[in] t パタンマッチの種類
      /// \param[out] tid テキストからパタンマッチで抽出したTIDの番号を
      /// 格納する領域
      ///
      /// \retval 1 パタンマッチした
      /// \retval 0 パタンマッチしなかった
      /// \retval -1 エラー発生
      ///
      /// begからendの範囲で指定した種別でパタンマッチし、結果を返す。
      /// TIDを取得したい場合はtidに有効なポインタを指定する。
      int check_rtm_types (const char* beg, const char* end, rtm_type t,
			   int* tid = NULL) const;

      /// \brief 指定された文字列内に指定した種類のパタンがあるか調べ
      /// る
      ///
      /// \param[in] s 調べる文字列
      /// \param[in] t パタンマッチの種類
      /// \param[out] tid テキストからパタンマッチで抽出したTIDの番号を
      /// 格納する領域
      ///
      /// \retval 1 パタンマッチした
      /// \retval 0 パタンマッチしなかった
      /// \retval -1 エラー発生
      int check_rtm_types (const std::string& s, rtm_type t,
			   int* tid = NULL) const
      {
	const char* beg = s.c_str ();
	return check_rtm_types (beg, beg + s.length (), t, tid);
      }

      /// \brief 指定された文字列中にReal-Time Model Objectのポインタの
      /// パタンがあるかどうか調べる
      ///
      /// \param[in] s 調べる文字列
      /// \param[out] tid テキストからパタンマッチで抽出したTIDの番号を
      /// 格納する領域
      ///
      /// \retval 1 パタンマッチした
      /// \retval 0 パタンマッチしなかった
      /// \retval -1 エラー発生
      ///
      /// \sa check_rtm_types()
      int check_rtm (const std::string& s, int* tid = NULL) const
      {
	return check_rtm_types (s, CHECK_RTM_VAR);
      }

      /// \brief 指定されたテキストの範囲にReal-Time Model Objectのポイ
      /// ンタのパタンがあるかどうか調べる
      ///
      /// \param[in] beg テキストの開始アドレス
      /// \param[in] end テキストの終了アドレス
      ///
      /// \retval 1 パタンマッチした
      /// \retval 0 パタンマッチしなかった
      /// \retval -1 エラー発生
      ///
      /// \sa check_rtm_types()
      int check_rtm (const char* beg, const char* end) const
      {
	return check_rtm_types (beg, end, CHECK_RTM_VAR);
      }

      /// \brief 指定された文字列中にReal-Time Model ObjectのTIDのパタ
      /// ンがあるかどうか調べる
      ///
      /// \param[in] s 調べる文字列
      /// \param[out] tid テキストからパタンマッチで抽出したTIDの番号を
      /// 格納する領域
      ///
      /// \retval 1 パタンマッチした
      /// \retval 0 パタンマッチしなかった
      /// \retval -1 エラー発生
      ///
      /// \sa check_rtm_types()
      int check_tid (const std::string& s, int* tid = NULL) const
      {
	return check_rtm_types (s, CHECK_RTM_TID, tid);
      }

      /// \brief 指定されたテキストの範囲にReal-Time Model ObjectのTID
      /// のパタンがあるかどうか調べる
      ///
      /// \param[in] beg テキストの開始アドレス
      /// \param[in] end テキストの終了アドレス
      /// \param[out] tid テキストからパタンマッチで抽出したTIDの番号を
      /// 格納する領域
      ///
      /// \retval 1 パタンマッチした
      /// \retval 0 パタンマッチしなかった
      /// \retval -1 エラー発生
      ///
      /// \sa check_rtm_types()
      int check_tid (const char* beg, const char* end, int* tid = NULL) const
      {
	return check_rtm_types (beg, end, CHECK_RTM_TID, tid);
      }

      /// \brief connectタグのブロックに対する CspBlockInfo を取得する
      ///
      /// \param c BLXMLのconnectのタグ
      ///
      /// \retval NULL ブロックが見つからない
      /// \retval それ以外 目的のブロックの CspBlockInfo のポインタ
      CspBlockInfo*
      get_connect_block (const SimulinkModel::XSD::connection_T& c) const;

      /// \brief 出力(BLXMLのoutputタグ)のブロックに、サンプル時間が継
      /// 承が可能ならば、サンプル時間を継承する
      ///
      /// \param[in] binfo 継承元ブロック
      /// \param[in] output 出力(BLXMLのoutputタグ)
      /// \param[out] forward_change 変更したかどうか
      /// \param[out] inherited_change 変更したかどうか
      ///
      /// 出力(BLXMLのoutputタグ)のブロックに対し、以下を行う
      /// -# ブロックがパス要素ならそのブロックに対して、ブロック指定の
      ///    inherit_forward() を呼び出す
      /// -# ブロックがRateTransitionであれば処理しない
      /// -# ブロックがSubSystemでAtomic SubSystemでない場合、入力ポー
      ///    ト対象のブロックとする。ただし以下のポートであれば無視する
      ///    - TriggerPort
      ///    - ActionPort
      /// -# ブロックのSample時間が継承(INHERITED)か連続(CONTINUOUS)で
      ///    なく、かつ入力側からの継承を受け付けない場合は処理しない
      /// -# check_input_inheritable() を呼び出し、継承元のブロックのサ
      ///    ンプル時間が受け入れ可能でなければ処理しない
      /// -# 継承からサンプル時間を継承し、forward_changeと
      ///    inherited_changeをtrueにする
      ///
      /// \sa check_input_inheritable()
      void inherit_forward (CspBlockInfo* binfo,
			    const SimulinkModel::XSD::ioport_T& output,
			    bool& forward_change, bool& inherited_change);

      /// \brief ブロック(BLXMLのblockタグ)の出力に対し、サンプル時間が
      /// 継承が可能ならば、サンプル時間を継承する
      ///
      /// \param[in] binfo 継承元ブロック
      /// \param[in] b 対象のブロック(BLXMLのblockタグ)
      /// \param[out] forward_change 変更したかどうか
      /// \param[out] inherited_change 変更したかどうか
      ///
      /// blockのoutputタグについて inherit_forward() を呼び出す。
      /// また、forward_changeおよびinherited_changeは
      /// Inherit_SampleTime() のものである
      ///
      /// \sa Inherit_SampleTime()
      void inherit_forward (CspBlockInfo* binfo,
			    const SimulinkModel::XSD::block_T* b,
			    bool& forward_change, bool& inherited_change);

      /// \brief 入力(BLXMLのinputタグ)のブロックに、サンプル時間が継
      /// 承が可能ならば、サンプル時間を継承する
      ///
      /// \param[in] binfo 継承元ブロック
      /// \param[in] input 出力(BLXMLのinputタグ)
      /// \param[out] backward_change 変更したかどうか
      /// \param[out] inherited_change 変更したかどうか
      ///
      /// 入力(BLXMLのinputタグ)のブロックに対し、以下を行う
      /// -# ブロックがパス要素ならそのブロックに対して、ブロック指定の
      ///    inherit_backward() を呼び出す
      /// -# ブロックのSample時間が継承(INHERITED)か連続(CONTINUOUS)か
      ///    定数(CONSTANT)でなく、かつ出力側からの継承を受け付けない場
      ///    合は処理しない
      /// -# check_output_inheritable() を呼び出し、継承元のブロックのサ
      ///    ンプル時間が受け入れ可能でなければ処理しない
      /// -# 継承からサンプル時間を継承し、backward_changeと
      ///    inherited_changeをtrueにする
      ///
      /// \sa check_output_inheritable()
      void inherit_backward (CspBlockInfo* binfo,
			     const SimulinkModel::XSD::ioport_T& input,
			     bool& backward_change, bool& inherited_change);

      /// \brief ブロック(BLXMLのblockタグ)の入力に対し、サンプル時間が
      /// 継承が可能ならば、サンプル時間を継承する
      ///
      /// \param[in] binfo 継承元ブロック
      /// \param[in] b 対象のブロック(BLXMLのblockタグ)
      /// \param[out] backward_change 変更したかどうか
      /// \param[out] inherited_change 変更したかどうか
      ///
      /// blockのinputタグについて inherit_backward() を呼び出す。
      /// また、backward_changeおよびinherited_changeは
      /// Inherit_SampleTime() のものである
      ///
      /// \sa Inherit_SampleTime()
      void inherit_backward (CspBlockInfo* binfo,
			     const SimulinkModel::XSD::block_T* b,
			     bool& backward_change, bool& inherited_change);

      /// \brief 入力元ブロックのサンプル時間が受け入れ可能なサンプル時
      /// 間の候補となるか調べる
      ///
      /// \param[in] binfo 入力元ブロック
      /// \param[in] st 候補のサンプル時間
      /// \param[out] match 一致したサンプル時間
      ///
      /// \retval true 候補になる
      /// \retval false 候補にならない
      ///
      /// ブロックの入力に対して、inputを調べる
      /// is_input_inherit_candidate() を呼び出し、全てがtrueの場合に
      /// trueを返す
      bool is_input_inherit_candidate (const CspBlockInfo* binfo,
				       const SampleTime*& st,
				       const SampleTime*& match) const;

      /// \brief 入力(BLXMLのinputタグ)のサンプル時間が受け入れ可能なサ
      /// ンプル時間の候補となるか調べる
      ///
      /// \param[in] input 入力(BLXMLのinputタグ)
      /// \param[in] st 候補のサンプル時間
      /// \param[out] match 一致したサンプル時間
      ///
      /// \retval true 候補になる
      /// \retval false 候補にならない
      ///
      /// 入力元ブロックのサンプル時間が、候補のサンプル時間に対して継
      /// 承可能なレートでなく、また、継承可能ではないレートの場合は、
      /// 候補にできないのでfailseで終了する。また、候補と一致した場合
      /// は、入力ブロックのサンプル時間をmatchに格納しtrueで終了する
      bool
      is_input_inherit_candidate (const SimulinkModel::XSD::ioport_T& input,
				  const SampleTime*& st,
				  const SampleTime*& match) const;

      /// \brief 出力先ブロックのサンプル時間が受け入れ可能なサンプル時
      /// 間の候補となるか調べる
      ///
      /// \param[in] binfo 出力先ブロック
      /// \param[in] st 候補のサンプル時間
      /// \param[out] match 一致したサンプル時間
      ///
      /// \retval true 候補になる
      /// \retval false 候補にならない
      ///
      /// ブロックの出力に対して、outputを調べる
      /// is_output_inherit_candidate() を呼び出し、全てがtrueの場合に
      /// trueを返す
      bool is_output_inherit_candidate (const CspBlockInfo* binfo,
					const SampleTime*& st,
					const SampleTime*& match) const;

      /// \brief 出力(BLXMLのoutputタグ)のサンプル時間が受け入れ可能な
      /// サンプル時間の候補となるか調べる
      ///
      /// \param[in] output 入力(BLXMLのoutputタグ)
      /// \param[in] st 候補のサンプル時間
      /// \param[out] match 一致したサンプル時間
      ///
      /// \retval true 候補になる
      /// \retval false 候補にならない
      ///
      /// 出力先ブロックのサンプル時間が、候補のサンプル時間に対して継
      /// 承可能なレートでなく、また、継承可能ではないレートの場合は、
      /// 候補にできないのでfailseで終了する。また、候補と一致した場合
      /// は、入力ブロックのサンプル時間をmatchに格納しtrueで終了する
      bool
      is_output_inherit_candidate (const SimulinkModel::XSD::ioport_T& output,
				   const SampleTime*& st,
				   const SampleTime*& match) const;

      /// \brief 指定したサンプル時間が、対象のブロックのサンプル時間の
      /// 候補となるか調べる
      ///
      /// \param[in] binfo 対象のブロック
      /// \param[in] x 候補のサンプル時間
      ///
      /// \retval true 候補になる
      /// \retval false 候補にならない
      ///
      /// ブロックを指定した is_input_inherit_candidate() と
      /// is_output_inherit_candidate() を呼び、その論理積を返す
      ///
      /// \sa is_input_inherit_candidate()
      /// \sa is_output_inherit_candidate()
      bool is_inherit_candidate (const CspBlockInfo* binfo,
				 const SampleTime*& x) const;

      /// \brief マージしているブロックに対して、サンプル時間をコピーす
      /// る
      ///
      /// \param[in] binfo 対象のブロック
      ///
      /// \retval true サンプル時間をコピーしたブロックがあり、変更があった
      /// \retval false サンプル時間はコピーされず、変更はなかった
      ///
      /// Inherit_SampleTIme() から呼び出され、対象ブロックがマージして
      /// いるブロックに対し、対象ブロックのサンプル時間をコピーする。
      /// コピーが行われず変更が無かった場合はfalseが返り、それ以外では
      /// trueが返る
      ///
      /// \sa Inherit_SampleTime()
      bool inherit_merge_blocks (CspBlockInfo* binfo) const;

      /// \brief 対象のブロックから、指定したサンプル時間が継承可能な後
      /// 続のブロックをdepth first searchで探索する
      ///
      /// \param[in] head 探索を開始したブロック
      /// \param[in] binfo 現在探索中のブロック
      /// \param[in] st サンプル時間
      /// \param[in,out] color 訪問チェック用の配列
      ///
      /// 以下のように探索を行う
      /// -# 訪問チェック用の配列の探索中のブロックに-1(GRAY)を設定する
      /// -# ブロックのoutputタグを指定した color_forward() を呼び出す
      /// -# 訪問チェック用の配列の探索中のブロックに1(BLACK)を設定する
      ///
      /// 同じブロックから color_forward() と color_backward() で探索す
      /// る事で、そのブロックを含み、同じレートになれるブロックを抽出
      /// (色分け)することができる。また check_input_color() により、レー
      /// トを決定してはならないブロックを除外する
      ///
      /// \sa color_backward()
      /// \sa check_input_color()
      void color_forward (const CspBlockInfo* head, const CspBlockInfo* binfo,
			  const SampleTime& st, int *color);

      /// \brief 出力(BLXMLのoutputタグ)のブロックが、指定されたサンプ
      /// ル時間が受け入れ可能であれば探索を続行する
      ///
      /// \param[in] head 探索を開始したブロック
      /// \param[in] output 出力(BLXMLのoutputタグ)
      /// \param[in] st サンプル時間
      /// \param[in,out] color 訪問チェック用の配列
      ///
      /// 出力先のブロックが、指定されたサンプル時間を受け入れ可能であ
      /// り、0(WHITE)であれば、ブロックを指定した color_forward() を呼
      /// び出し探索を続行する
      void color_forward (const CspBlockInfo* head,
			  const SimulinkModel::XSD::ioport_T& output,
			  const SampleTime& st, int *color);

      /// \brief 対象のブロックから、指定したサンプル時間が継承可能な前
      /// 段のブロックをdepth first searchで探索する
      ///
      /// \param[in] head 探索を開始したブロック
      /// \param[in] binfo 現在探索中のブロック
      /// \param[in] st サンプル時間
      /// \param[in,out] color 訪問チェック用の配列
      ///
      /// 以下のように探索を行う
      /// -# 訪問チェック用の配列の探索中のブロックに-1(GRAY)を設定する
      /// -# ブロックのoutputタグを指定した color_forward() を呼び出す
      /// -# 訪問チェック用の配列の探索中のブロックに1(BLACK)を設定する
      ///
      /// 同じブロックから color_forward() と color_backward() で探索す
      /// る事で、そのブロックを含み、同じレートになれるブロックを抽出
      /// (色分け)することができる。また check_input_color() により、レー
      /// トを決定してはならないブロックを除外する
      ///
      /// \sa color_forward()
      /// \sa check_input_color()
      void color_backward (const CspBlockInfo* head, const CspBlockInfo* binfo,
			   const SampleTime& st, int *color);

      /// \brief 入力(BLXMLのinputタグ)のブロックが、指定されたサンプル
      /// 時間が受け入れ可能であれば探索を続行する
      ///
      /// \param[in] head 探索を開始したブロック
      /// \param[in] input 出力(BLXMLのinputタグ)
      /// \param[in] st サンプル時間
      /// \param[in,out] color 訪問チェック用の配列
      ///
      /// 出力先のブロックが、指定されたサンプル時間を受け入れ可能であ
      /// り、0(WHITE)であれば、ブロックを指定した color_backward()
      /// を呼び出し探索を続行する
      void color_backward (const CspBlockInfo* head,
			   const SimulinkModel::XSD::ioport_T& input,
			   const SampleTime& st, int *color);

      /// \brief 入力(BLXMLのinputタグ)に対し、レートが決定可能か調べる
      ///
      /// \param[in] input 入力(BLXMLのinputタグ)
      /// \param[in] color 訪問チェックで色分けされた配列
      ///
      /// \retval true 決定可能
      /// \retval false 決定不可
      ///
      /// 入力元のブロックが未訪問、かつサンプル時間が継承のままの場合
      /// は、レートを決定してはいけないので、falseを返す。そうでなけれ
      /// ばtrueを返す
      bool check_input_color (const SimulinkModel::XSD::ioport_T& input,
			      const int* color) const;

      /// \brief ブロックのレートが決定可能か調べる
      ///
      /// \param[in] binfo 対象のブロック
      /// \param[in] color 訪問チェックで色分けされた配列
      ///
      /// \retval true 決定可能
      /// \retval false 決定不可
      ///
      /// 対象のブロックのinputについて check_input_color() を呼び出し、
      /// falseだったらfalseを返す。全てtrueならtrueを返す
      ///
      /// falseを返す場合は、このブロックのレートを決定することはできな
      /// いので color_forward() と color_backward() で色分けされたブロッ
      /// クからこのブロックを除外する必要がある
      ///
      /// \sa color_forward()
      /// \sa color_backward()
      bool check_input_color (const CspBlockInfo* binfo,
			      const int* color) const;

      /// \brief 継承元からサンプル時間を継承可能であれば継承する
      ///
      /// \param[in] from 継承元のブロック
      /// \param[in,out] to 継承対象のブロック
      /// \param[in] dir 継承の方向(verbose時の表示のため)
      ///
      /// \retval true 継承し変更を行った
      /// \retval false 継承せず変更を行わなかった
      ///
      /// 継承対象のブロックのサンプル時間が継承可能であれば、継承元の
      /// サンプル時間をコピーする。マージしているブロックがあれば、
      /// inherit_merge_blocks() を呼び出す
      ///
      /// \sa inherit_merge_blocks()
      bool Inherit_SampleTime (const CspBlockInfo* from,
			       CspBlockInfo* to, int dir) const;

      /// \brief SubSystemのブロックにサンプル時間の継承を行う
      ///
      /// \param[in,out] subsys 継承対象のSubSystemのブロック
      /// \param[in] propagate 継承元のブロック
      /// \param[in] force 強制的に継承するかどうか(trueなら強制的に継承)
      ///
      /// \retval true 継承し変更を行った
      /// \retval false 継承せず変更を行わなかった
      ///
      /// 継承対象のSubSystem内のブロックが継承が可能であれば(forceが
      /// trueなら強制的に)継承元のサンプル時間をコピーする
      bool Inherit_Subsystem_SampleTime (CspBlockInfo* subsys,
					 const CspBlockInfo* propagate,
					 bool force = false) const;

      /// \brief 入力(BLXMLのinputタグ)のサンプル時間が継承する際に受け
      /// 入れ可能なサンプル時間か調べる
      ///
      /// \param[in] input 入力(BLXMLのinputのタグ)
      /// \param[in,out] from 候補となる継承元
      /// \param[in,out] st 候補となるサンプル時間
      /// \param[in] heuristic heuristicな手段を使うかどうか(trueなら使う)
      ///
      /// \retval true 受け入れ可能
      /// \retval false 受け入れ不可
      ///
      /// このinputは、対象としたブロックの出力先ブロックのinputである。
      /// (対象としたブロックの、ではない)
      ///
      /// Simulinkには、全ての入力のサンプル時間は、最速の入力サンプル
      /// 時間の整数倍、というルールがあるため、レートが以下の場合に受
      /// け入れ可能とする。
      /// - 候補のレートが、入力レートの整数倍(より速いレート)
      /// - 入力レートが、候補のレートの整数倍(より遅いレート)
      ///
      /// 基本的に、inputに接続するブロック(接続元ブロック)に対し、以下
      /// のように動作する
      /// -# 入力元が経路となるブロックは、ブロックに対する
      ///    check_input_inheritable() を呼び出し、trueだったら候補のブ
      ///    ロックとサンプル時間を置き換えて以下の処理を行う
      /// -# 入力元のブロックのサンプル時間が(決定すべき要素ではないも
      ///    のを除き)決定していない場合falseで終了
      ///    - ただしheuristicがtrueの場合、falseで終了せず、処理を継続
      ///      する
      /// -# stで指定されたサンプル時間がCONSTANTであれば、どのレートで
      ///    も受け入れ可能なので、fromを入力元ブロック、stをそのブロッ
      ///    クのサンプル時間に更新し、trueで終了する
      /// -# stで指定されたサンプル時間が、入力元ブロックのサンプル時間
      ///    の整数倍であれば、fromを入力元ブロック、stをそのブロックの
      ///    サンプル時間に更新し、trueで終了する
      /// -# 入力元ブロックのサンプル時間が、stで指定されたサンプル時間
      ///    の整数倍であれば、trueで終了する
      /// -# それ以外であればfalseで終了
      bool check_input_inheritable (const SimulinkModel::XSD::ioport_T& input,
				    const CspBlockInfo*& from,
				    const SampleTime*& st,
				    bool heuristic = false) const;

      /// \brief 入力側のブロックのサンプル時間が継承する際に受け入れ可
      /// 能なサンプル時間か調べる
      ///
      /// \param[in] binfo 入力側のブロック
      /// \param[in,out] from 候補となる継承元
      /// \param[in,out] st 候補となるサンプル時間
      /// \param[in] heuristic heuristicな手段を使うかどうか(trueなら使う)
      ///
      /// \retval true 受け入れ可能
      /// \retval false 受け入れ不可
      ///
      /// 入力側のブロック(対象としているブロックの出力先のブロック)の
      /// inputに対し、inputを調べる check_input_inheritable() を呼び出
      /// し、全てがtrueだった場合trueを返す。それ以外はfalseを返す。
      ///
      /// これは、対象としたブロックの、出力先のブロックが入力する他の
      /// 全てのブロックのサンプル時間について、サンプル時間の候補が継
      /// 承可能かどうか調べるために inherit_forrward() から呼ばれる
      ///
      /// \sa inherit_forward()
      bool check_input_inheritable (const CspBlockInfo* binfo,
				    const CspBlockInfo*& from,
				    const SampleTime*& st,
				    bool heuristic = false) const;

      /// \brief 出力(BLXMLのoutputタグ)のサンプル時間が継承する際に受
      /// け入れ可能なサンプル時間か調べる
      ///
      /// \param[in] output 出力(BLXMLのoutputのタグ)
      /// \param[in,out] to 候補となる継承元
      /// \param[in,out] st 候補となるサンプル時間
      ///
      /// \retval true 受け入れ可能
      /// \retval false 受け入れ不可
      ///
      /// このoutputは、対象としたブロックの、入力元ブロックのoutputで
      /// ある。(対象としたブロックの、ではない)
      ///
      /// Simulinkには、全ての入力のサンプル時間は、最速の入力サンプル
      /// 時間の整数倍、というルールがあるが、出力としては遅いレートか
      /// ら速いレートへの伝播を認めると、結果に影響があると考えられる
      /// ため、レートが以下の場合に受け入れ可能とする。
      /// - 候補のレートが、入力レートの整数倍(より速いレート)
      ///
      /// 基本的にoutputに接続するブロック(出力先ブロック)に対し、以下
      /// のように動作する
      /// -# ブロックがRateTransitionのように継承しないブロックや、
      ///    Sinkブロック等は以降の処理をしない
      /// -# 出力先が経路となるブロックは、ブロックに対する
      ///    check_output_inheritable() を呼び出し、trueだったら候補のブ
      ///    ロックとサンプル時間をその結果に置き換える
      /// -# 出力先がSubSystemの場合は、Atomic SubSystem以外は、
      ///    SubSystem内の接続するInportを特定、接続するブロックに置き
      ///    換える
      ///    - TriggerPortやActionPortである場合は以降の処理をしない
      /// -# 出力先ブロックのサンプル時間が(決定すべき要素ではないもの
      ///    を除き)決定していない場合falseで終了
      /// -# stで指定されたサンプル時間が、出力先ブロックのサンプル時間
      ///    の整数倍であれば、toを入力ポートに接続しているブロック、
      ///    stをそのブロックのサンプル時間に更新し、trueで終了する
      /// -# それ以外であればfalseで終了
      bool check_output_inheritable (const SimulinkModel::XSD::ioport_T& output,
				     const CspBlockInfo*& to,
				     const SampleTime*& st) const;

      /// \brief 出力側のブロックのサンプル時間が継承する際に受け入れ可
      /// 能なサンプル時間か調べる
      ///
      /// \param[in] binfo 出力側のブロック
      /// \param[in,out] to 候補となる継承元
      /// \param[in,out] st 候補となるサンプル時間
      ///
      /// \retval true 受け入れ可能
      /// \retval false 受け入れ不可
      ///
      /// 出力側のブロック(対象としているブロックの入力元のブロック)の
      /// 出力ポートに対し、出力ポートを調べる
      /// check_output_inheritable() を呼び出し、全てがtrueだった場合
      /// trueを返す。それ以外はfalseを返す。
      ///
      /// これは、対象としたブロックの、入力元のブロックが出力する他の
      /// 全てのブロックのサンプル時間について、サンプル時間の候補が継
      /// 承可能かどうか調べるために inherit_backward() から呼ばれる
      ///
      /// \sa inherit_backward()
      bool check_output_inheritable (const CspBlockInfo* binfo,
				     const CspBlockInfo*& to,
				     const SampleTime*& st) const;

      /// \brief BLXMLのblockタグについてInportと同様のブロックかどうか
      /// 調べる
      ///
      /// \param[in] b ブロック(BLXMLのblockタグ)
      ///
      /// \retval true Inportと同様なブロック
      /// \retval false それ以外
      ///
      /// 以下のものをInportと同様のブロックとみなす
      /// - Inport
      /// - TriggerPort
      /// - EnablePort
      /// - ActionPort
      static bool is_inport_type (const SimulinkModel::XSD::block_T* b);

      /// \brief ブロックがInportと同様のブロックかどうか調べる
      ///
      /// \param[in] binfo ブロック ( CspBlockInfo )
      ///
      /// \retval true Inportと同様なブロック
      /// \retval false それ以外
      ///
      /// BLXMLのblockタグを取得し、それに対して is_inport_type()
      /// を呼び出す
      static bool is_inport_type (const CspBlockInfo* binfo);

      /// \brief BLXMLのblockタグについてパス要素のブロックか調べる
      ///
      /// \param[in] b ブロック(BLXMLのblockタグ)
      ///
      /// \retval true パス要素のブロック
      /// \retval false それ以外
      ///
      /// 以下のものをパス要素と判定する
      /// - Demux
      /// - Mux
      /// - From
      /// - Goto
      /// - Merge
      /// - BusCreator
      /// - BusSelector
      static bool is_pass_block (const SimulinkModel::XSD::block_T* b);

      /// \brief ブロックがパス要素のブロックか調べる
      ///
      /// \param[in] binfo ブロック(CspBlockInfo)
      ///
      /// \retval true パス要素のブロック
      /// \retval false それ以外
      ///
      ///  BLXMLのblockタグを取得し、それに対して is_pass_block()
      /// を呼び出す
      static bool is_pass_block (const CspBlockInfo* binfo);

      /// \brief ブロックを行番号の昇順に並び替えるソート用比較関数
      ///
      /// \param[in] a ブロック1
      /// \param[in] b ブロック2
      ///
      /// \retval true ブロック1の行番号が先
      /// \retval false ブロック1の行番号が同じか後
      ///
      /// ブロック ( CspBlockInfo )を行番号順に並び替えるための比較関数
      static bool cmp_line (const CspBlockInfo* a, const CspBlockInfo* b);

      /// \brief sorted_SampleTime を遅い順に並び替えるソート用比較関数
      ///
      /// \param[in] a sorted_SampleTime 1
      /// \param[in] b sorted_SampleTime 2
      ///
      /// \retval true sorted_SampleTime 1の方が大きい(遅い)
      /// \retval false sorted_SampleTime 1の方が同じか小さい(速い)
      ///
      /// sorted_SampleTime を降順(遅い順)に並び替える為の比較関数
      static bool sort_sampletime_func (const sorted_SampleTime& a,
					const sorted_SampleTime& b)
      {
	return *a > *b;
      }

      /// \brief Real-Time Model Objectのポインタの変数名
      ///
      /// Cコードではこのポインタを経由してReal-Time Model Objectにアク
      /// セスする
      std::string c_rtm_variable;

      /// \brief TID番号をインデクスとしたサンプル時間のポインタのベクタ
      ///
      /// \sa SampleTime
      std::vector < SampleTime* > tid_values; // by tid

      /// \brief サンプル時間の大きい(遅い順)にソートされた
      /// sorted_SampleTime のベクタ
      ///
      /// \sa sorted_SampleTime
      /// \sa sort_sampletime_func()
      std::vector < sorted_SampleTime > sorted_sample_times;

      /// \brief 生成されたサンプル時間のポインタのベクタ
      ///
      /// 現状以下のものが含まれる
      /// - 低レート化したPulseGeneratorのPeriodとPulseWidthから算出さ
      ///   れた周期
      std::vector < SampleTime* > generated_sample_times;

      /// \brief 最速のサンプル時間
      SampleTime fast_sample_rate;

      /// \brief モデルがシングルレートかそうでないか
      bool single_rate;

      /// \brief 外部出力が必要かどうか
      bool need_extout;

      /// \brief 外部入力が必要かどうか
      bool need_extin;

      /// \brief DataStreMemoryの名前をキーとした
      /// DataStoreMemoryの連想配列
      dsm_map_T cr_dsm_map;

      /// \brief 処理の結果、エラーが発生したかどうか
      bool has_error;

      /// \brief グラフ記述子
      CspGraph csp_graph;
    };

    /// XMLを読み込み、情報を保持する静的なオブジェクト
    extern CspReader csp_reader;

    /// \brief CspNodeのコードの位置の比較用叙述関数
    struct CspNodePosLess
    {
      /// \brief CspNode 間のコードの位置の比較
      ///
      /// \param[in] a CspNode 1
      /// \param[in] b CspNode 2
      ///
      /// \retval true \a a は \a b の前
      /// \retval false そうではない
      bool operator() (CspNode* a, CspNode* b)
      {
	int ap = a->pos ();
	int bp = b->pos ();

	if (ap != -1 && bp != -1)
	  return ap < bp;
	return ap != -1;
      }
    };

    /// @}

    /// \defgroup XMLCSP_SUB CspNodeに対するサブルーチン
    ///
    /// \brief
    ///
    /// @{


    /// @}

    /// @}
  }
}

#endif	/* XML_CSP_H */
