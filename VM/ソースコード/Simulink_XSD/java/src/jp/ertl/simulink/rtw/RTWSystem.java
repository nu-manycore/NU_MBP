// -*- mode: java; tab-width: 4; -*-

package jp.ertl.simulink.rtw;

import jp.ertl.simulink.rtw.RTWParam;
import jp.ertl.simulink.rtw.RTWSectionBase;
import jp.ertl.simulink.rtw.RTWBlock;

import java.util.List;
import java.util.ArrayList;

/**
 * RTW System class for "System" or "Subsystem"
 */
public class RTWSystem extends RTWBlock {
	/**
	 * constructor
	 *
	 * @param t a type of section (section identifier)
	 */
	public RTWSystem(String t) {
		super(t);
		// nothing
	}

	/**
	 * constructor
	 *
	 * @param t a type of section (section identifier)
	 * @param slist list of sections
	 * @param plist list of parameters
	 */
	public RTWSystem(String t,
					 List<RTWSectionBase> slist, List<RTWParam> plist) {
		super(t, slist, plist);
		// nothing
	}
}
