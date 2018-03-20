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

import jp.ertl.simulink.BLXML.SchemaDefine;
import jp.ertl.utils.Getopts;
import jp.ertl.utils.SimpleCSV;

/**
 * set block measureing result
 *
 * <pre>
 * usage: SetMeasuringResult -i inputXML [-o outputXML] [-v] [CSV]
 * </pre>
 *
 * @version 0.1
 */
public class SetMeasuringResult {
	/**
	 * measuring result
	 */
	private static class MeasuringResult {
		/**
		 * BLXML Block Name
		 */
		private String blockName;

		/**
		 * measuring result value
		 */
		private Integer result;

		/**
		 * core number
		 */
		private int coreNumber;

		/**
		 *
		 */
		private boolean used;

		/**
		 * Constructor
		 *
		 * @param blockName BLXML block name
		 * @param result measuring result
		 * @param coreNumber core number
		 */
		public MeasuringResult(String blockName,
							   Integer result, int coreNumber) {
			this.blockName = blockName;
			this.result = result;
			this.coreNumber = coreNumber;
			this.used = false;
		}

		/**
		 * get BLXML block name
		 *
		 * @return BLXML block name
		 */
		public String getBlockName() {
			return blockName;
		}

		/**
		 * get measuring result
		 *
		 * @return measuring result
		 */
		public Integer getResult() {
			return result;
		}

		/**
		 * get core number
		 *
		 * @return core number
		 */
		public int getCoreNumber() {
			return coreNumber;
		}

		/**
		 * set used
		 */
		public void setUsed() {
			used = true;
		}

		/**
		 * is used
		 *
		 * @return <code>true</code> is used
		 */
		public boolean isUsed() {
			return used;
		}
	}

	/**
	 * verbose flag
	 */
	private static int verbose = 0;

	/**
	 * map of Block's MeasuringResult (CSV)
	 */
	private static HashMap<String, MeasuringResult>
		blockResultMap = new HashMap<String, MeasuringResult>();

	/**
	 * load CSV file, and build blockResultMap
	 *
	 * @param csvFile a CSV File, included Block-name and MeasuringResult
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

		if (verbose > 0) {
			System.out.println("+ -- Load CSV --");
		}

		String cols[];
		while ((cols = csv.getColumns()) != null) {
			if (cols.length < 3) {
				continue;
			}
			if (cols[0].equals("")) {
				if (verbose > 0) {
					System.out.println("+ (OverHead): " + cols[1]);
				}
			} else {
				Integer result = null;
				int coreNum = -1;

				if (!cols[1].equals("")) {
					result = new Integer(cols[1]);
				}
				if (!cols[2].equals("")) {
					coreNum = Integer.parseInt(cols[2]);
				}
				MeasuringResult
					measuringResult = new MeasuringResult(cols[0],
														  result, coreNum);
				blockResultMap.put(cols[0], measuringResult);
				if (verbose > 0) {
					System.out.println("+ " + cols[0]
									   + ": " + cols[1]
									   + ":" + cols[2]);
				}
			}
		}
		if (verbose > 0) {
			System.out.println("+ -- End Load CSV --");
		}
	}

	/**
	 * process
	 *
	 * @param blocks &lt;blocks&gt; node
	 */
	private static void process(BlocksT blocks) {
		for (BlockT block : blocks.getBlock()) {
			MeasuringResult
				measuringResult = blockResultMap.get(block.getName());

			// measuringResult
			Integer oldMeasuringResult = block.getMeasuringResult();
			Integer newMeasuringResult = null;
			Integer newPeinfo = null;

			if (measuringResult != null) {
				measuringResult.setUsed();
				newMeasuringResult = measuringResult.getResult();
				newPeinfo = new Integer(measuringResult.getCoreNumber());
			}

			if (verbose > 0) {
				if (oldMeasuringResult != null) {
					if (newMeasuringResult == null) {
						System.out.println("+ " + block.getBlocktype() + ":"
										   + block.getName() + ": "
										   + "remove measuringResult("
										   + oldMeasuringResult
										   + ")");
					} else {
						System.out.println("+ " + block.getBlocktype() + ":"
										   + block.getName() + ": "
										   + "set measuringResult("
										   + oldMeasuringResult
										   + ") => " + newMeasuringResult);
					}
				} else if (newMeasuringResult == null) {
					if (verbose > 1) {
						System.out.println("+ " + block.getBlocktype() + ":"
										   + block.getName() + ": "
										   + "no result found");
					}
				} else {
					System.out.println("+ " + block.getBlocktype() + ":"
									   + block.getName() + ": "
									   + "set measuringResult "
									   + newMeasuringResult);
				}
			}
			block.setMeasuringResult(newMeasuringResult);

			// peinfo
			String oldPeinfo = block.getPeinfo();

			if (verbose > 0) {
				if (oldPeinfo != null) {
					if (newPeinfo == null) {
						System.out.println("+ " + block.getBlocktype() + ":"
										   + block.getName() + ": "
										   + "remove peinfo("
										   + oldPeinfo
										   + ")");
					} else {
						System.out.println("+ " + block.getBlocktype() + ":"
										   + block.getName() + ": "
										   + "set peinfo(" + oldPeinfo
										   + ") => " + newPeinfo);
					}
				} else if (newPeinfo == null) {
					if (verbose > 1) {
						System.out.println("+ " + block.getBlocktype() + ":"
										   + block.getName() + ": "
										   + "no peinfo found");
					}
				} else {
					System.out.println("+ " + block.getBlocktype() + ":"
									   + block.getName() + ": "
									   + "set peinfo "
									   + newPeinfo);
				}
			}
			if (newPeinfo == null) {
				block.setPeinfo(null);
			} else {
				block.setPeinfo(newPeinfo.toString());
			}

			for (BlocksT subBlocks : block.getBlocks()) {
				process(subBlocks);
			}
		}
	}

	/**
	 * usage
	 */
	private static void usage() {
		System.err.println("usage: SetMeasureingResult"
						   + " -i inputXML [-v] [-o outputXML] [CSV]");
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
		while ((c = getOpts.getopt(args, "i:o:v")) != '\0') {
			switch (c) {
			case 'i':
				inputXML = getOpts.optarg;
				break;
			case 'o':
				outputXML = getOpts.optarg;
				break;
			case 'v':
				verbose++;
				break;
			case '?':
				usage();
				break;
			}
		}

		if (inputXML == null) {
			usage();
		}

		if (verbose > 0 && outputXML == null) {
			System.err.println("no -o option, ignore -v");
			verbose = 0;
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

			process(xmlBlocks);

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

			if (verbose > 0) {
				for (MeasuringResult result : blockResultMap.values()) {
					if (!result.isUsed()) {
						System.out.println("+ Unused: "
										   + result.getBlockName() + ": "
										   + result.getResult()
										   + "," + result.getCoreNumber());
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
