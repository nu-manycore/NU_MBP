// -*- mode: java; tab-width: 4; -*-

package jp.ertl.simulink.rtw;

import jp.ertl.simulink.rtw.RTWParam;
import jp.ertl.simulink.rtw.RTWSectionBase;
import jp.ertl.simulink.rtw.RTWSampleTime;

import java.util.List;
import java.util.ArrayList;
import java.util.HashMap;

/**
 * RTW Block class (base of RTWSystem and RTWCompiledModel)
 */
public class RTWBlock extends RTWSectionBase {
	/**
	 * value of "Name"
	 */
	private String name;

	/**
	 * value of "Type"
	 */
	private String type;

	/**
	 * Block name of BLXML
	 */
	private String blockName;

	/**
	 * Upper SubSystem ("Block" =&gt; "System" only)
	 */
	private RTWBlock parent;

	/**
	 * Map of Block (Hash Key is "Name")
	 */
	private HashMap<String, RTWBlock>
		blockMap = new HashMap<String, RTWBlock>();

	/**
	 * SampleTime
	 */
	private RTWSampleTime sampleTime;

	/**
	 * set SampleTime
	 *
	 * @param st SampleTime (in CompiledModel)
	 */
	public void setSampleTime(RTWSampleTime st) {
		sampleTime = st;
	}

	/**
	 * get SampleTime
	 *
	 * @return SampleTime
	 */
	public RTWSampleTime getSampleTime() {
		return sampleTime;
	}

	/**
	 * does this block have SampeTime?
	 *
	 * @return <code>true</code> has SampleTime
	 */
	public boolean hasSampleTime() {
		return sampleTime != null;
	}

	/**
	 * set name ("Name")
	 *
	 * @param n value of "Name"
	 */
	public void setName(String n) {
		name = n;
	}

	/**
	 * get name ("Name")
	 *
	 * @return value of "Name"
	 */
	public String getName() {
		return name;
	}

	/**
	 * set type ("Type")
	 *
	 * @param t value of "Type"
	 */
	public void setType(String t) {
		type = t;
	}

	/**
	 * get type ("Type")
	 *
	 * @return value of "Type"
	 */
	public String getType() {
		return type;
	}

	/**
	 * set blockNmae (BLXML Block name)
	 *
	 * @param b name of BLXML Block
	 */
	public void setBlockName(String b) {
		blockName = b;
	}

	/**
	 * get blockName (BLXML Block name)
	 *
	 * @return name of BLXML Block
	 */
	public String getBlockName() {
		return blockName;
	}

	/**
	 * set parent
	 *
	 * @param p this block's parent
	 */
	public void setParent(RTWBlock p) {
		parent = p;
	}

	/**
	 * get parent
	 *
	 * @return this block's parent ("Block"=&gt;"System" only)
	 */
	public RTWBlock getParent() {
		return parent;
	}

	/**
	 * find block
	 *
	 * @param n name of block to search
	 *
	 * @return RTWBlock or null
	 */
	public RTWBlock findBlock(String n) {
		return blockMap.get(n);
	}

	/**
	 * constructor
	 *
	 * @param t a type of section (section identifier)
	 */
	public RTWBlock(String t) {
		super(t);
		parent = null;
	}

	/**
	 * constructor
	 *
	 * @param t a type of section (section identifier)
	 * @param slist list of sections
	 * @param plist list of parameters
	 */
	public RTWBlock(String t,
					List<RTWSectionBase> slist, List<RTWParam> plist) {
		super(t, slist, plist);

		RTWParam pn = findParam("Name");
		if (pn != null && pn.isString()) {
			setName(pn.getStringValue());
		}

		RTWParam pt = findParam("Type");
		if (pt != null && pt.isString()) {
			setType(pt.getStringValue());
		}

		for (RTWSectionBase s : slist) {
			if (s instanceof RTWBlock) {
				RTWBlock b = (RTWBlock)s;
				b.setParent(this);
				blockMap.put(b.getName(), b);
			}
		}
	}

	/**
	 * debug function to dump with indent
	 *
	 * @param indent level of indent (0, 1, 2, ...)
	 */
	public void dump(int indent) {
		StringBuilder ind = new StringBuilder();
		for (int i = 0; i < indent / 4; i++) {
			ind.append("\t");
		}
		for (int i = 0; i < indent % 4; i++) {
			ind.append("  ");
		}
		System.out.print(ind + "@" + getSectionType()
						 + " type=\"" + getType()
						 + "\", name=\"" + getName() + "\"");
		if (hasSampleTime()) {
			RTWSampleTime st = getSampleTime();
			System.out.print(", TID=" + st.getTID());
			System.out.print("[" + st.getStepSize() + "]");
		}

		RTWBlock p = getParent();
		if (p != null) {
			System.out.print(", parent="
							 + p.getSectionType() + ":" + p.getName());
		}
		System.out.print("\n");
		super.dump(indent);
	}
}
