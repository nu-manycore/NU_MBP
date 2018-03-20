// -*- mode: java; tab-width: 4; -*-

import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.util.HashMap;
import java.util.List;
import java.util.ArrayList;

import javax.xml.bind.JAXBContext;
import javax.xml.bind.JAXBElement;
import javax.xml.bind.JAXBException;
import javax.xml.bind.Marshaller;
import javax.xml.bind.Unmarshaller;
import javax.xml.namespace.QName;
import javax.xml.transform.stream.StreamSource;

import jp.ertl.simulink.SimulinkModel.BlocksT;
import jp.ertl.simulink.SimulinkModel.BlockT;
import jp.ertl.simulink.SimulinkModel.BlockNameT;

import jp.ertl.simulink.BLXML.SchemaDefine;

import jp.ertl.utils.Getopts;
import jp.ertl.utils.SimpleCSV;

/**
 * set block rate
 *
 * <pre>
 * usage: SetRate -i inputXML [-o outputXML] [-v] [CSV]
 * </pre>
 *
 * @version 0.1
 */
public class SetRate {
	/**
	 * verbose flag
	 */
	private static boolean verbose = false;

	/**
	 * inherit flag
	 */
	private static boolean do_inherit = false;

	/**
	 * set baserate flag
	 */
	private static boolean do_setBaseRate = false;

	/**
	 * An entry of block rates in CSV
	 */
	private static class Rate {
		/**
		 * base rate (String)
		 */
		private static String baseRate;

		/**
		 * base rate (Double)
		 */
		private static double baseRateValue;

		/**
		 * a name of Simulink Block in BLXML
		 */
		private String name;

		/**
		 * a Sample Time of Simulink Block (String)
		 */
		private String rate;

		/**
		 * a Sample Time of Simulink Block (double)
		 */
		private double rateValue;

		/**
		 * a Sample Time of Simulink Block (integer value by baserate)
		 */
		private int intRateValue;

		/**
		 * used flag
		 */
		private boolean used;	// used

		/**
		 * constructor
		 *
		 * @param b Block Name
		 * @param r Sample Time
		 */
		public Rate(String b, String r) {
			name = b;
			rate = r;
			rateValue = Double.parseDouble(r);
			used = false;
		}

		/**
		 * set base rate
		 *
		 * @param b base rate
		 */
		public static void setBaseRate(String b) {
			baseRate = b;
			baseRateValue = Double.parseDouble(b);
		}

		/**
		 * get base rate
		 *
		 * @return base rate string
		 */
		public static String getBaseRate() {
			return baseRate;
		}

		/**
		 * get base rate value
		 *
		 * @return base rate
		 */
		public static double getBaseRateValue() {
			return baseRateValue;
		}

		/**
		 * get Block Name
		 *
		 * @return block name
		 */
		public String getName() {
			return name;
		}

		/**
		 * get Sample Time
		 *
		 * @return Sample Time
		 */
		public String getRate() {
			return rate;
		}

		/**
		 * get Sample Time
		 *
		 * @return Sample Time
		 */
		public double getRateValue() {
			return rateValue;
		}

		/**
		 * get rate (int)
		 *
		 * @return rate(int)
		 */
		public int getIntRateValue() {
			if (intRateValue == 0) {
				intRateValue = (int)Math.round(getRateValue()
											   / getBaseRateValue());
			}
			return intRateValue;
		}

		/**
		 * is used this entry
		 *
		 * @return <code>true</code> this used, <code>false</code> unused.
		 */
		public boolean isUsed() {
			return used;
		}

		/**
		 * used
		 */
		public void setUsed () {
			used = true;
		}

		/**
		 * equals (Rate)
		 *
		 * @param r rate(Rate)
		 *
		 * @return <code>true</code> same value
		 */
		public boolean equals(Rate r) {
			return getIntRateValue() == r.getIntRateValue();
		}

		/**
		 * equals (double)
		 *
		 * @param d rate(double)
		 *
		 * @return <code>true</code> same value
		 */
		public boolean equals(double d) {
			int i = (int)Math.round(d / getBaseRateValue());
			return getIntRateValue() == i;
		}

		/**
		 * equals (String)
		 *
		 * @param s rate(String)
		 *
		 * @return <code>true</code> same value
		 */
		public boolean equals(String s) {
			try {
				int i = (int)Math.round(Double.parseDouble(s)
										/ getBaseRateValue());
				return getIntRateValue() == i;
			}
			catch (NumberFormatException e) {
				// none
			}
			return false;
		}
	}

	/**
	 * map of Block's Rate (CSV)
	 */
	private static HashMap<String, Rate>
		blockRateMap = new HashMap<String, Rate>();

	/**
	 * Information of Block in XML
	 */
	private static class BlockInfo {
		/**
		 * this &lt;block&gt; tag
		 */
		private BlockT block;

		/**
		 * upper block
		 */
		private BlockInfo upperInfo = null;

		/**
		 * merge owner (this block is merged by owner)
		 */
		private BlockInfo mergeOwnerInfo = null;

		/**
		 * merged blocks (this block is owner)
		 */
		private List<BlockInfo> mergeBlocks = null;

		/**
		 * Sampletime is fixed
		 */
		private boolean rateFixed = false;

		/**
		 * a Sampletime of this block
		 */
		private Rate rate;

		/*
		 * Constructor
		 *
		 * @param b block
		 * @param u upper block
		 */
		public BlockInfo(BlockT b, BlockInfo u) {
			block = b;
			upperInfo = u;
		}

		/**
		 * get block
		 *
		 * @return &lt;block&gt; tag
		 */
		public BlockT getBlock() {
			return block;
		}

		/**
		 * get upper BlockInfo
		 *
		 * @return Upper BlockInfo (SubSystem)
		 */
		public BlockInfo getUpperInfo() {
			return upperInfo;
		}

		/**
		 * set merge owner
		 *
		 * @param o owner
		 */
		public void setMergeOwnerInfo(BlockInfo o) {
			mergeOwnerInfo = o;
		}

		/**
		 * get merge owner
		 *
		 * @return BlockInfo of merge owner
		 */
		public BlockInfo getMergeOwnerInfo() {
			return mergeOwnerInfo;
		}

		/**
		 * add merge block
		 *
		 * @param b merged block
		 */
		public void addMergeBlock(BlockInfo b) {
			if (mergeBlocks == null) {
				mergeBlocks = new ArrayList<BlockInfo>();
			}
			mergeBlocks.add(b);
		}

		/**
		 * get merge blocks
		 *
		 * @return list of BlockInfo
		 */
		public List<BlockInfo> getMergeBlocks() {
			return mergeBlocks;
		}

		/**
		 * a sampletime is fixed
		 */
		public void rateFixed() {
			rateFixed = true;
		}

		/**
		 * is a sampletime fixed?
		 *
		 * @return <code>true</code> fixed(could not change)
		 */
		public boolean isRateFixed() {
			return rateFixed;
		}

		/**
		 * set Rate
		 *
		 * @param r Rate
		 */
		public void setRate(Rate r) {
			rate = r;
		}

		/**
		 * get Rate
		 *
		 * @return Rate (or null)
		 */
		public Rate getRate() {
			return rate;
		}

		/**
		 * get rate in merged Blocks
		 *
		 * @return Rate or null
		 */
		public Rate getRateInMergeBlocks() {
			List<BlockInfo> l = getMergeBlocks();
			if (l != null) {
				for (BlockInfo i : l) {
					if (i.isRateFixed()) {
						return i.getRate(); // return first
					}
				}
			}
			return null;
		}

		/**
		 * correct Rate in merge blocks
		 *
		 * @return <code>true</code> is no problem,
		 *         <code>false</code> is anerror
		 */
		public boolean correctRate() {
			boolean result = true;
			if (!isRateFixed()) {
				Rate rate = getRateInMergeBlocks();
				if (rate != null) {
					setRate(rate);
					rateFixed();
				}
			}
			if (isRateFixed()) {
				List<BlockInfo> l = getMergeBlocks();
				if (l != null) {
					for (BlockInfo i : l) {
						if (!i.isRateFixed()) {
							i.setRate(getRate());
							i.rateFixed();
						} else if (!i.getRate().equals(getRate())) {
							System.err.println("Error: incorrect SampleTime: "
											   + getBlock().getName()
											   + " and "
											   + i.getBlock().getName());
							result = false;
						}
					}
				}
			}
			return result;
		}
	}

	/**
	 * map of block infomation (XML)
	 */
	private static HashMap<String, BlockInfo>
		blockInfoMap = new HashMap<String, BlockInfo>();

	/**
	 * load CSV file, and build blockRateMap
	 *
	 * @param csvFile a CSV File, included Block-name and SampleTime
	 *
	 * @throws IOException exception of IO
	 */
	private static void loadCSV(String csvFile) throws IOException {
		SimpleCSV csv;
		if (csvFile == null) {
			csv = new SimpleCSV(System.in);
		} else {
			csv = new SimpleCSV(csvFile);
		}

		if (verbose) {
			System.out.println("+ -- Load CSV --");
		}

		String cols[];
		while ((cols = csv.getColumns()) != null) {
			if (cols.length < 2) {
				continue;
			}
			if (cols[0].equals("")) {
				Rate.setBaseRate(cols[1]);
				if (verbose) {
					System.out.println("+ (baseRate): " + cols[1]);
				}
			} else {
				Rate rate = new Rate(cols[0], cols[1]);
				blockRateMap.put(cols[0], rate);
				if (verbose) {
					System.out.println("+ " + cols[0] + ": " + cols[1]);
				}
			}

		}
		if (verbose) {
			System.out.println("+ -- End Load CSV --");
		}
	}

	/**
	 * make BlockInfos
	 *
	 * @param blocks &lt;blocks&gt; node
	 * @param upper upper subsystem &lt;block&gt; node
	 */
	private static void makeBlockInfo(BlocksT blocks, BlockInfo upper) {
		for (BlockT block : blocks.getBlock()) {
			BlockInfo blockInfo = new BlockInfo(block, upper);

			Rate rate = blockRateMap.get(block.getName());
			if (rate != null) {
				blockInfo.setRate(rate);
				blockInfo.rateFixed();
				rate.setUsed();
			}

			blockInfoMap.put(block.getName(), blockInfo);

			for (BlocksT subBlocks : block.getBlocks()) {
				makeBlockInfo(subBlocks, blockInfo);
			}
		}
	}

	/**
	 * parse BlockInfo
	 *
	 * @param blocks &lt;blocks&gt; node
	 */
	private static void correctBlockInfo(BlocksT blocks) {
		for (BlockT block : blocks.getBlock()) {
			BlockInfo blockInfo = blockInfoMap.get(block.getName());

			for (BlockNameT blockName : block.getMergeBlock()) {
				String mergeBlockName = blockName.getBlock();
				BlockInfo mergeBlockInfo = blockInfoMap.get(mergeBlockName);
				if (mergeBlockInfo == null) {
					System.err.println("Error: " + block.getName()
									   + ": merge block "
									   + mergeBlockName + ": not found");
					continue;
				}
				blockInfo.addMergeBlock(mergeBlockInfo);
			}

			for (BlockNameT blockName : block.getMergedTo()) {
				BlockInfo mergedToInfo = blockInfo.getMergeOwnerInfo();
				if (mergedToInfo != null) {
					System.err.println("Error: " + block.getName()
									   + ": already merged to "
									   + mergedToInfo.getBlock().getName());
					break;
				}
				String mergedToName = blockName.getBlock();
				mergedToInfo = blockInfoMap.get(mergedToName);
				if (mergedToInfo == null) {
					System.err.println("Error: " + block.getName()
									   + ": merged block to "
									   + mergedToName + ": not found");
					continue;
				}
				blockInfo.setMergeOwnerInfo(mergedToInfo);
			}

			for (BlocksT subBlocks : block.getBlocks()) {
				correctBlockInfo(subBlocks);
			}
		}
	}

	/**
	 * process
	 *
	 * @param blocks &lt;blocks&gt; node
	 */
	private static void process(BlocksT blocks) {
		for (BlockT block : blocks.getBlock()) {
			String oldRate = block.getRate();
			BlockInfo blockInfo = blockInfoMap.get(block.getName());
			if (!blockInfo.correctRate()) {
				continue;
			}
			Rate rate = blockInfo.getRate();
			if (rate != null) {
				String newRate = rate.getRate();
				if (verbose) {
					System.out.print("+ " + block.getBlocktype() + ":"
									 + block.getName() + ": ");
					if (oldRate != null) {
						System.out.print(oldRate + " => ");
					}
					System.out.println(newRate);
				}
				block.setRate(newRate);
			} else if (verbose) {
				if (oldRate == null) {
					System.out.println("- " + block.getBlocktype() + ":"
									   + block.getName()
									   + ": no rate in both .rtw and XML");
				} else {
					System.out.println("- " + block.getBlocktype() + ":"
									   + block.getName()
									   + ": rate(" + oldRate
									   + "): no rate in .rtw");
				}
			}
			for (BlocksT subBlocks : block.getBlocks()) {
				process(subBlocks);
			}
		}
	}

	/**
	 * inherit (TODO)
	 *
	 * @param blocks &lt;blocks&gt; node
	 * @param rate a inherit rate
	 */
	private static void inheritSampleTime(BlocksT blocks, String rate) {
		for (BlockT block : blocks.getBlock()) {
			String blockRate = block.getRate();
			if (rate != null) {
				if (blockRate == null || blockRate.equals("-1")) {
					block.setRate(rate);
					if (verbose) {
						System.out.print("+ " + block.getName() + ": ");
						if (blockRate != null) {
							System.out.print(blockRate + " => ");
						}
						System.out.println(rate + " (inherit)");
					}
				} else if (verbose) {
					System.out.println("- " + block.getName()
									   + ": (" + blockRate
									   + ") not inherit from [" + rate + "]");
				}
			}
			if (block.getBlocks().size() > 0) {
				blockRate = block.getRate();
				if (blockRate != null
					&& (blockRate.equals("-1")
						|| blockRate.equals("0")
						|| blockRate.equals("inf"))) {
					if (verbose) {
						System.out.println("- " + block.getName()
										   + ": no inherit subsystem rate ("
										   + blockRate + ")");
					}
					blockRate = null;
				}
				for (BlocksT subBlocks : block.getBlocks()) {
					inheritSampleTime(subBlocks, blockRate);
				}
			}
		}
	}

	/**
	 * set base rate
	 *
	 * @param blocks &lt;blocks&gt; node
	 */
	private static void setBaseRate(BlocksT blocks) {
		for (BlockT block : blocks.getBlock()) {
			String rate = block.getRate();
			if (rate != null && rate.equals("-1")) {
				block.setRate(Rate.getBaseRate());
			}
			for (BlocksT subBlocks : block.getBlocks()) {
				setBaseRate(subBlocks);
			}
		}
	}

	private static void usage() {
		System.err.println("usage: SetRate "
						   + "-i inputXML [-v] [-o outputXML] [CSV]");
		System.exit(1);
	}


	/**
	 *
	 * main
	 *
	 * @param args command line arguments
	 *
	 * @throws IOException exception of IO
	 * @throws JAXBException exception of JAXB
	 */
	public static void main(String args[])
		throws IOException, JAXBException {
		String inputCSV = null;
		String inputXML = null;
		String outputXML = null;

		Getopts getOpts = new Getopts();
		char c;
		while ((c = getOpts.getopt(args, "o:i:vIB")) != '\0') {
			switch (c) {
			case 'o':
				outputXML = getOpts.optarg;
				break;
			case 'i':
				inputXML = getOpts.optarg;
				break;
			case 'v':
				verbose = true;
				break;
			case 'I':
				do_inherit = true;
				break;
			case 'B':
				do_setBaseRate = true;
				break;
			case '?':
				usage();
				break;
			}
		}

		if (inputXML == null) {
			usage();
		}

		if (verbose == true && outputXML == null) {
			System.err.println("no -o option, ignore -v");
			verbose = false;
		}

		if (getOpts.optind < args.length) {
			inputCSV = args[getOpts.optind];
		}

		try {

			JAXBContext jc = JAXBContext.newInstance(BlocksT.class);

			Unmarshaller u = jc.createUnmarshaller();

			File inputFile = new File(inputXML);

			JAXBElement<BlocksT> xin = u.unmarshal(new StreamSource(inputFile),
												   BlocksT.class);

			BlocksT xmlBlocks = xin.getValue();

			loadCSV(inputCSV);

			makeBlockInfo(xmlBlocks, null);
			correctBlockInfo(xmlBlocks);
			process(xmlBlocks);
			if (do_inherit) {
				inheritSampleTime(xmlBlocks, null);
			}
			if (do_setBaseRate) {
				setBaseRate(xmlBlocks);
			}

			QName qname = new QName(SchemaDefine.getNamespaceURL(),
									"blocks", "sm");

			JAXBElement<BlocksT> elem = new JAXBElement<BlocksT>(qname,
																 BlocksT.class,
																 xmlBlocks);

			Marshaller m = jc.createMarshaller();

			m.setProperty(Marshaller.JAXB_SCHEMA_LOCATION,
						  SchemaDefine.getNamespaceURL()
						  + " "
						  + SchemaDefine.getBLXMLSchema());
			m.setProperty(Marshaller.JAXB_FORMATTED_OUTPUT, true);

			if (outputXML == null) {
				m.marshal(elem, System.out);
			} else {
				File outFile = new File(outputXML);

				outFile.createNewFile();
				outFile.setWritable(true);

				m.marshal(elem, outFile);
			}

			if (verbose) {
				for (Rate rate : blockRateMap.values()) {
					if (!rate.isUsed()) {
						System.out.println("+ Unused: "
										   + rate.getName() + ": "
										   + rate.getRate());
					}
				}
			}

			System.exit(0);
		}
		catch (IOException e) {
			e.printStackTrace();
		}
		catch (JAXBException e) {
			e.printStackTrace();
		}

		System.exit(1);
	}
}
