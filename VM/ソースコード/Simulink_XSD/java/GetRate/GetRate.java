// -*- mode: java; tab-width: 4; -*-

import java.io.FileReader;
import java.io.PrintStream;
import java.io.IOException;
import java.io.FileNotFoundException;

import jp.ertl.simulink.rtw.RTWParam;
import jp.ertl.simulink.rtw.RTWSectionBase;
import jp.ertl.simulink.rtw.RTWSampleTime;
import jp.ertl.simulink.rtw.RTWBlock;
import jp.ertl.simulink.rtw.RTWSystem;
import jp.ertl.simulink.rtw.RTWCompiledModel;
import jp.ertl.simulink.rtw.parser.RTWParser;
import jp.ertl.simulink.rtw.parser.RTWScanner;
import jp.ertl.simulink.rtw.parser.RTWParserSym;

import jp.ertl.utils.Getopts;

import java.util.List;
import java.util.ArrayList;

import java_cup.runtime.SymbolFactory;
import java_cup.runtime.ComplexSymbolFactory;
import java_cup.runtime.ComplexSymbolFactory.Location;
import java_cup.runtime.Symbol;

/**
 * Get block rate
 *
 * <pre>
 * usage: GetRate [-o outputCSV] filename.rtw
 * </pre>
 *
 * @version 0.1
 */
public class GetRate {
	/**
	 * verbose flag
	 */
	private static boolean verbose = false;

	/**
	 * dump sample time
	 *
	 * @param out output stream
	 * @param block Block
	 */
	private static void dumpSampleTime(PrintStream out, RTWBlock block) {
		// ignore not "Block { ... }", (e.g. "System" { ... })
		if (block.getClass() == RTWBlock.class) {
			if (block.hasSampleTime()) {
				RTWSampleTime st = block.getSampleTime();
				out.println(block.getBlockName() + "," + st.getStepSize());
				if (verbose) {
					System.out.println("+ " + block.getType() + ":"
									   + block.getBlockName()
									   + ": " + st.getStepSize());
				}
			} else if (verbose) {
				System.out.println("- " + block.getType() + ":"
								   + block.getBlockName()
								   + ": not has SampleTime");
			}
		}
		for (RTWSectionBase s : block.getSectionList()) {
			if (s instanceof RTWBlock) {
				dumpSampleTime(out, (RTWBlock)s);
			}
		}
	}

	/**
	 * usage
	 */
	private static void usage() {
		System.err.println("usage: GetRate [-o outputCSV] filename.rtw");
		System.exit(1);
	}

	/**
	 *
	 * main
	 *
	 * @param args command line arguments
	 */
	public static void main(String args[]) {
		PrintStream out = System.out;
		String outputCSV = null;
		Getopts getOpts = new Getopts();
		char c;
		while ((c = getOpts.getopt(args, "o:v")) != '\0') {
			switch (c) {
			case 'o':
				outputCSV = getOpts.optarg;
				break;
			case 'v':
				verbose = true;
				break;
			case '?':
				usage();
				break;
			}
		}

		if (args.length <= getOpts.optind) {
			usage();
		}

		String file = args[getOpts.optind];

		if (verbose && outputCSV == null) {
			System.err.println("no -o option, ignore -v");
			verbose = false;
		}

	    try {
			FileReader inFileReader = new FileReader(file);

			ComplexSymbolFactory symbolFactory = new ComplexSymbolFactory();

			RTWScanner scanner = new RTWScanner(symbolFactory, inFileReader);

			RTWParser parser = new RTWParser(scanner, symbolFactory);

			RTWSectionBase top = (RTWSectionBase)parser.parse().value;

			if (top == null) {
				System.err.println("Error: No load rtw file, exit.");
				System.exit(1);
			}

			List<RTWSectionBase> models = top.findSectionType("CompiledModel");
			if (models == null) {
				System.err.println("Error: No CompiledModel, exit.");
				System.exit(1);
			}

			RTWCompiledModel compiledModel = null;
			if (models.size() > 1) {
				System.err.println("Warning: multiple CompiledModel");
			}
			RTWSectionBase b = models.get(0);
			if (b instanceof RTWCompiledModel) {
				compiledModel = (RTWCompiledModel)b;
			} else {
				System.err.println("Error: invalid CompiledModel");
			}

			if (compiledModel.isSingleRate()) {
				System.err.println("Not multirate, exit.");
				System.exit(1);
			}

			if (outputCSV != null) {
				out = new PrintStream(outputCSV);
			}

			for (RTWSectionBase sys
					 : compiledModel.findSectionType("System")) {
				if (sys instanceof RTWSystem) {
					dumpSampleTime(out, (RTWBlock)sys);
				}
			}
			out.println("," + RTWSampleTime.getMinStep());
	    }
	    catch (Exception e) {
			System.out.flush();
			if (out != System.out) {
				out.flush();
			}
			System.err.println("Error: ----");
			e.printStackTrace();
			System.exit(1);
	    }

		System.exit(0);
	}
}
