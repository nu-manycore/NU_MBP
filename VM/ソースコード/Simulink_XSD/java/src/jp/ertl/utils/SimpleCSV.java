// -*- mode: java; tab-width: 4; -*-

package jp.ertl.utils;

import java.io.IOException;
import java.io.FileNotFoundException;
import java.io.InputStream;
import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.FileReader;

/**
 * Simple CSV class
 * not supported '"' in quoted string, and multilined columns
 */
public class SimpleCSV {
	/**
	 * BufferedReader for CSV File
	 */
	private BufferedReader bufferedReader;

	/**
	 * Constructor
	 *
	 * @param csvFile CSV File Name
	 *
	 * @throws FileNotFoundException exception of "file not fund"
	 * @throws IOException exception of IO
	 */
	public SimpleCSV(String csvFile)
		throws FileNotFoundException, IOException {
		FileReader in = new FileReader(csvFile);
		bufferedReader = new BufferedReader(in);
	}

	/**
	 * Constructor
	 *
	 * @param inputStream input stream as "System.in"
	 *
	 * @throws IOException exception of IO
	 */
	public SimpleCSV(InputStream inputStream) throws IOException {
		InputStreamReader in = new InputStreamReader(inputStream);
		bufferedReader = new BufferedReader(in);
	}

	/**
	 * get columns
	 *
	 * @return columns or null(EOF)
	 *
	 * @throws IOException exception of IO
	 */
	public String[] getColumns() throws IOException {
		String line;
		line = bufferedReader.readLine();
		if (line == null) {
			return null;
		}

		String tmpCols[] = line.split(",");
		String cols[] = new String[tmpCols.length];
		for (int i = 0; i < tmpCols.length; i++) {
			String col = tmpCols[i];
			int len = col.length();
			if (len > 2
				&& col.charAt(0) == '"'
				&& col.charAt(col.length() - 1) == '"') {
				cols[i] = col.substring(1, col.length() - 1);
			} else {
				cols[i] = col;
			}
		}
		return cols;
	}
}
