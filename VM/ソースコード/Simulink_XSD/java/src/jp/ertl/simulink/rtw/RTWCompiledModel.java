// -*- mode: java; tab-width: 4; -*-

package jp.ertl.simulink.rtw;

import jp.ertl.simulink.rtw.RTWParam;
import jp.ertl.simulink.rtw.RTWSectionBase;
import jp.ertl.simulink.rtw.RTWBlock;
import jp.ertl.simulink.rtw.RTWSampleTime;

import java.util.List;
import java.util.ArrayList;
import java.util.HashMap;

/**
 * RTW CompiledModel class
 */
public class RTWCompiledModel extends RTWBlock {
	/**
	 * "SampleTime" section indexed by TID
	 */
	private RTWSampleTime TIDSampleTime[];

	/**
	 * "SingleRate" param value
	 */
	private boolean singleRate = true;

	/**
	 * Map of hierarchy (hierarchy =&gt; name of BLXML block)
	 */
	private HashMap<String, String>
		hierarchyMap = new HashMap<String, String>();

	/**
	 * does this block have "SampleTime" section?
	 *
	 * @return <code>true</code> has "SampleTime"
	 */
	public boolean hasTIDSampleTime() {
		return TIDSampleTime != null && TIDSampleTime.length > 0;
	}

	/**
	 * Is SampleTime indexed by TID, valid?
	 *
	 * @param tid TID
	 *
	 * @return <code>true</code> valid
	 */
	public boolean isValidTIDSampleTime(int tid) {
		return (TIDSampleTime != null
				&& 0 <= tid && tid < TIDSampleTime.length
				&& TIDSampleTime[tid] != null);
	}

	/**
	 * set Sampletime and TID
	 *
	 * @param tid TID
	 * @param st SampleTime
	 */
	private void setTIDSampleTime(int tid, RTWSampleTime st) {
		TIDSampleTime[tid] = st;
	}

	/**
	 * get SampleTime indexed by TID
	 *
	 * @param tid TID
	 *
	 * @return SampleTime or null
	 */
	public RTWSampleTime getTIDSampleTime(int tid) {
		return TIDSampleTime[tid];
	}

	/**
	 * get step-size
	 *
	 * @param tid TID
	 *
	 * @return step-size
	 */
	public double getTIDStepSize(int tid) {
		return TIDSampleTime[tid].getStepSize();
	}

	/**
	 * setup SampleTime, TID
	 *
	 * @param s Section
	 */
	private void TIDsetup(RTWSectionBase s) {
		if (s instanceof RTWBlock) {
			RTWParam p = null;
			String TID = null;
			RTWBlock b = (RTWBlock)s;
			String tag = "TID";
			while (true) {
				p = b.findParam(tag);
				if (p != null && p.isString()) {
					String t = p.getStringValue();
					if (t.equals("Subsystem")) {
						tag = "SubsystemTID";
					} else if (t.equals("triggered")) {
						tag = "TriggerTID";
					} else {
						TID = t;
						break;
					}
				} else {
					break;
				}
			}
			if (TID != null) {
				try {
					int tid = Integer.parseInt(TID);
					if (0 <= tid && tid < TIDSampleTime.length) {
						RTWSampleTime st = TIDSampleTime[tid];
						if (st != null && st.isValid()) {
							b.setSampleTime(st);
						}
					}
				}
				catch (NumberFormatException e) {
					// none
				}
			}
		}
		for (RTWSectionBase ls: s.getSectionList()) {
			TIDsetup(ls);
		}
	}

	/**
	 * canonicalize for BLXML name format
	 *
	 * @param name
	 *		Block/Port/Line name of Simulink
	 *
	 * @return
	 *		canicalized name
	 */
	private static String canonicalize(String name) {
		char org[] = name.toCharArray();
		int len = name.length();
		int idx = 0;
		char tmp[] = new char[len];
		for (int i = 0; i < len; i++) {
			if (org[i] == '/' || org[i] == '-') {
				tmp[idx++] = '_';
			} else if (org[i] == '_' || Character.isLetterOrDigit(org[i])) {
				tmp[idx++] = org[i];
			}
		}
		return new String(tmp, 0, idx);
	}

	/**
	 * setup BLXML Block name
	 *
	 * @param b Block
	 */
	private void setupBlockName(RTWBlock b) {
		String name = b.getName();
		String blockName = findHierachyMap(name);
		if (blockName != null) {
			b.setBlockName(blockName);
		} else {
			String sys = null;
			String blk = null;
			int srash = name.indexOf("/");
			if (srash != -1) {
				sys = name.substring(0, srash);
				blk = name.substring(srash + 1);
				String sbn = findHierachyMap(sys);
				if (sbn != null) {
					blockName = canonicalize(new String(sbn + "_" + blk));
					hierarchyMap.put(name, blockName);
					b.setBlockName(blockName);
				} else {
					System.err.println("Error: Unknown Hierachy: " + name);
				}
			}
		}
		for (RTWSectionBase s : b.getSectionList()) {
			if (s instanceof RTWBlock) {
				RTWBlock sb = (RTWBlock)s;
				setupBlockName(sb);
			}
		}
	}

	/**
	 * setup hierarchy
	 *
	 * @param prefix Subsystem name
	 * @param b Sub-Block
	 *
	 * @return
	 *		<code>1</code> handling, <code>0</code> no change
	 */
	private int hierarchySetup(String prefix, RTWBlock b) {
		int flag = 0;

		String name = b.getName();
		if (name == null) {
			return 0;
		}

		String block = hierarchyMap.get(name);
		if (block == null) {
			String sys = null;
			String blk = null;
			int srash = name.indexOf("/");
			if (srash != -1) {
				sys = name.substring(0, srash);
				blk = name.substring(srash + 1);
			}

			if (sys == null) {
				// maybe root
				RTWParam p = b.findParam("SLName");
				if (p == null || !p.isString()) {
					return 0;
				}
				String slName = p.getStringValue();
				if (prefix == null) {
					block = canonicalize(slName);
				} else {
					block = canonicalize(new String(prefix + "_" + slName));
				}
			} else {
				String n = hierarchyMap.get(sys);
				if (n == null) {
					if (prefix != null) {
						hierarchyMap.put(sys, prefix);
					}
					n = prefix;
				}
				block = canonicalize(new String(n + "_" + blk));
			}
			hierarchyMap.put(name, block);
			flag = 1;
		}
		List<RTWSectionBase> l = b.getSectionList();
		if (l != null) {
			for (RTWSectionBase s : l) {
				if (s instanceof RTWBlock
					&& hierarchySetup(block, (RTWBlock)s) == 1) {
					flag = 1;
				}
			}
		}
		return flag;
	}

	/**
	 * setup hierarchy (toplevel)
	 */
	private void hierarchySetup() {
		List<RTWSectionBase> l = findSectionType("BlockHierarchyMap");
		if (l == null) {
			return;
		}
		int flag = 1;
		while (flag == 1) {
			flag = 0;
			for (RTWSectionBase s : l) {
				List<RTWSectionBase> ll = s.findSectionType("Subsystem");
				if (ll == null) {
					continue;
				}
				for (RTWSectionBase ss : ll) {
					if (ss instanceof RTWBlock) {
						RTWBlock b = (RTWBlock)ss;
						if (hierarchySetup(null, b) == 1) {
							flag = 1;
						}
					}
				}
			}
		}

		for (RTWSectionBase s : findSectionType("System")) {
			if (s instanceof RTWBlock) {
				setupBlockName((RTWBlock)s);
			}
		}
	}

	/**
	 * debug function to dump Hierarchy mapping
	 */
	public void dumpHierarchyMap() {
		System.out.println("---- dumpHierarchyMap() ----");
		for(String k : hierarchyMap.keySet()) {
			System.out.println(k + " => " + hierarchyMap.get(k));
		}
	}

	/**
	 * find name of BLXML block by hierarchy block
	 *
	 * @param n hierarchy block
	 *
	 * @return name of BLXML block
	 */
	public String findHierachyMap(String n) {
		return hierarchyMap.get(n);
	}

	/**
	 * Is this block "SingleRate=yes"?
	 *
	 * @return <code>true</code> yes
	 */
	public boolean isSingleRate() {
		return singleRate;
	}

	/**
	 * Is this CompiledModel valid?
	 *
	 * @return <code>true</code> valid
	 */
	public boolean isValid() {
		if (isSingleRate ()) {
			return true;		// OK
		}

		if (TIDSampleTime == null) {
			return false;
		}

		for (int i = 0; i < TIDSampleTime.length; i++) {
			RTWSampleTime st = TIDSampleTime[i];
			if (st == null || !st.isValid())
				return false;
		}

		return true;			// OK
	}

	/**
	 * constructor
	 *
	 * @param t a type of section (section identifier)
	 */
	public RTWCompiledModel(String t) {
		super(t);
	}

	/**
	 * constructor
	 *
	 * @param t a type of section (section identifier)
	 * @param slist list of sections
	 * @param plist list of parameters
	 */
	public RTWCompiledModel(String t,
							List<RTWSectionBase> slist,
							List<RTWParam> plist) {
		super(t, slist, plist);

		RTWParam p = findParam("SingleRate");
		if (p == null
			|| !p.isString()
			|| p.getStringValue().equals("yes")) {
			return;
		}

		singleRate = false;

		p = findParam("NumSampleTimes");
		if (p == null || !p.isString()) {
			return;
		}
		int numTID = Integer.parseInt(p.getStringValue());
		if (numTID < 1) {
			return;
		}

		TIDSampleTime = new RTWSampleTime[numTID];

		for (RTWSectionBase s : findSectionType("SampleTime")) {
			if (s instanceof RTWSampleTime) {
				RTWSampleTime st = (RTWSampleTime)s;
				if (st.isValid()) {
					setTIDSampleTime(st.getTID(), st);
				}
			}
		}
		for (RTWSectionBase st : getSectionList()) {
			TIDsetup(st);
		}

		hierarchySetup();
	}
}
