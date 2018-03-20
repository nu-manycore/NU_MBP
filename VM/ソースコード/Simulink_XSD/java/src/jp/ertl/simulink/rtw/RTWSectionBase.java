// -*- mode: java; tab-width: 4; -*-

package jp.ertl.simulink.rtw;

import jp.ertl.simulink.rtw.RTWParam;

import java.util.List;
import java.util.ArrayList;

import java.util.HashMap;

/**
 * RTW Section (base) class
 */
public class RTWSectionBase {
	/**
	 * Section Type(Identifier), as "CompiledModel", "System", "Block", ...
	 */
	private String sectionType;

	/**
	 * List of Parameters
	 */
	private List<RTWParam> paramList;

	/**
	 * Map of Parameters
	 */
	private HashMap<String, RTWParam> paramMap;


	/**
	 * List of Sections
	 */
	private List<RTWSectionBase> sectionList;

	/**
	 * Map of Section Type
	 */
	private HashMap<String, List<RTWSectionBase>> sectionTypeMap;

	/**
	 * parent section of this (may not same as SimulinkBlock hierarchy)
	 */
	private RTWSectionBase sectionParent;

	/**
	 * constructor
	 *
	 * @param t a type of section (section identifier)
	 */
	public RTWSectionBase(String t) {
		sectionType = t;
		paramList = new ArrayList<RTWParam>();
		paramMap = new HashMap<String, RTWParam>();
		sectionList = new ArrayList<RTWSectionBase>();
		sectionTypeMap = new HashMap<String, List<RTWSectionBase>>();
	}

	/**
	 * constructor
	 *
	 * @param t a type of section (section identifier)
	 * @param slist list of sections
	 * @param plist list of parameters
	 */
	public RTWSectionBase(String t,
					  List<RTWSectionBase> slist, List<RTWParam> plist) {
		sectionType = t;
		paramList = plist;
		sectionList = slist;
		paramMap = new HashMap<String, RTWParam>();
		sectionTypeMap = new HashMap<String, List<RTWSectionBase>>();
		for (RTWParam p : paramList) {
			paramMap.put(p.getName(), p);
		}
		for (RTWSectionBase s : sectionList) {
			List<RTWSectionBase> l = sectionTypeMap.get(s.getSectionType());
			if (l == null) {
				l = new ArrayList<RTWSectionBase>();
				sectionTypeMap.put(s.getSectionType(), l);
			}
			l.add(s);
			s.setSectionParent(this);
		}
	}

	/**
	 * get type of section type
	 *
	 * @return section type (section identifier)
	 */
	public String getSectionType() {
		return sectionType;
	}

	/**
	 * get parent section
	 *
	 * @return parent of this section
	 */
	public RTWSectionBase getSectionParent() {
		return sectionParent;
	}

	/**
	 * set parent section
	 *
	 * @param p parent of this section
	 */
	public void setSectionParent(RTWSectionBase p) {
		sectionParent = p;
	}

	/**
	 * add parameter (not used)
	 *
	 * @param n a name of parameter
	 * @param v value(string or list)
	 *
	 * @return new instance of RTWParam
	 */
	public RTWParam addParam(String n, Object v) {
		RTWParam p = new RTWParam(n, v);
		paramList.add(p);
		paramMap.put(n, p);
		return p;
	}

	/**
	 * add section (not used)
	 *
	 * @param n a type of section (section identifier)
	 *
	 * @return new instance of RTWSectionBase
	 */
	public RTWSectionBase addSection(String n) {
		RTWSectionBase s = new RTWSectionBase(n);
		sectionList.add(s);
		List<RTWSectionBase> l = sectionTypeMap.get(s.getSectionType());
		if (l == null) {
			l = new ArrayList<RTWSectionBase>();
			sectionTypeMap.put(s.getSectionType(), l);
		}
		l.add(s);
		return s;
	}

	/**
	 * get list of parameters
	 *
	 * @return a list of RTWParam
	 */
	public List<RTWParam> getParamList() {
		return paramList;
	}

	/**
	 * find parameter
	 *
	 * @param s a parameter name to search
	 *
	 * @return RTWParam or null
	 */
	public RTWParam findParam(String s) {
		return paramMap.get(s);
	}

	/**
	 * get list of sections
	 *
	 * @return a list of RTWSectionBase
	 */
	public List<RTWSectionBase> getSectionList() {
		return sectionList;
	}

	/**
	 * find sections which has Type
	 *
	 * @param s a type of section
	 *
	 * @return a list of RTWSectionBase or null
	 */
	public List<RTWSectionBase> findSectionType(String s) {
		return sectionTypeMap.get(s);
	}

	/**
	 * debug function to dump with indent
	 *
	 * @param indent level of indent (0, 1, 2, ...)
	 */
	public void dump(int indent) {
		StringBuilder ind = new StringBuilder();
		StringBuilder cind = new StringBuilder();
		for (int i = 0; i < indent / 4; i++) {
			ind.append("\t");
		}
		for (int i = 0; i < indent % 4; i++) {
			ind.append("  ");
		}
		for (int i = 0; i < (indent + 1) / 4; i++) {
			cind.append("\t");
		}
		for (int i = 0; i < (indent + 1) % 4; i++) {
			cind.append("  ");
		}
		System.out.println(ind + getSectionType() + " {");
		for (RTWParam p : getParamList()) {
			String s = p.toString();
			System.out.println(cind + s);
		}
		for (RTWSectionBase s : getSectionList()) {
			s.dump(indent + 1);
		}
		System.out.println(ind + "}");
	}

	/**
	 * debug function (wrapper)
	 */
	public void dump() {
		dump(0);
	}
}
