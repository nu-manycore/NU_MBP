// -*- mode: java; tab-width: 4; -*-

package jp.ertl.utils;

/**
 * Getopt class
 */
public class Getopts {
	public int optind = 0;
	public String optarg = null;

	/** current position of option */
	private int optptr = 0;

	/**
	 * get option type
	 *
	 * @param c current option charactor
	 * @param optstr option string
	 * @return option type, 1 has optarg, 0 single option, -1 error
	 */
	private int getOptType(char c, String optstr) {
		int len = optstr.length();
		for (int i = 0; i < len; i++) {
			if (optstr.charAt(i) == c) {
				if (i + 1 < len && optstr.charAt(i + 1) == ':')
					return 1;
				else
					return 0;
			}
		}
		return -1;
	}

	/**
	 * get options
	 *
	 * @param args command line arguments
	 * @param optstr option string
	 * @return option
	 */
	public char getopt(String args[], String optstr) {
		if (optind == args.length) {
			return '\0';
		}

		String curOpt = args[optind];
		int optLength = curOpt.length();

		if (optptr != 0 && optptr == optLength) {
			if (++optind == args.length)
				return '\0';
			curOpt = args[optind];
			optLength = curOpt.length();
			optptr = 0;
		}

		if (optptr == 0) {
			if (optLength == 0 || curOpt.charAt(0) != '-')
				return '\0';
			else if (optLength == 1)
				return '?';
			optptr = 1;
		}

		char optchar = curOpt.charAt(optptr++);
		int opttype = getOptType(optchar, optstr);

		if (opttype == -1)
			return '?';
		else if (opttype == 1) {
			if (optptr < optLength) {
				optarg = curOpt.substring(optptr);
			} else if (optind + 1 < args.length) {
				optarg = args[++optind];
			}
			optind++;
			optptr = 0;
		}
		return optchar;
	}
}
