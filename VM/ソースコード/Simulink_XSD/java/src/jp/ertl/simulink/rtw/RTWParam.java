// -*- mode: java; tab-width: 4; -*-

package jp.ertl.simulink.rtw;

import java.util.List;
import java.util.ArrayList;

/**
 * RTW Parameter class
 */
public class RTWParam {
	/**
	 * a parameter name
	 */
    private String name;

	/**
	 * a parameter value
	 */
    private Object param;

	/**
	 * Constructor
	 *
	 * @param n parameter name
	 * @param p parameter value
	 */
    public RTWParam(String n, Object p) {
		name = n;
		param = p;
    }

	/**
	 * get a parameter name
	 *
	 * @return a parameter name
	 */
    public String getName() {
		return name;
    }

	/**
	 * get raw parameter value
	 *
	 * @return Object for value
	 */
    public Object getRawValue() {
		return param;
    }

	/**
	 * check a value type is String
	 *
	 * @return <code>true</code> a type is String
	 */
    public boolean isString() {
		return param instanceof String;
    }

	/**
	 * check a value type is List
	 *
	 * @return <code>true</code> a type is List
	 */
    public boolean isList() {
		return param instanceof List;
    }

	/**
	 * get parameter value as String
	 *
	 * @return String value
	 */
    public String getStringValue() {
		if (isString()) {
			return (String)param;
		}
		return null;
    }

	/**
	 * get parameter value as List
	 *
	 * @return List value
	 */
    public List<?> getListValue() {
		if (isList()) {
			return (List<?>)param;
		}
		return null;
    }

	/**
	 * debug function for dump
	 */
	public void dump() {
		System.out.println(toString());
	}

	/**
	 * list format
	 *
	 * @param l parameter value (list)
	 *
	 * @return a formatted string
	 */
	private String listFormat(List<?> l) {
		String s = new String();
		int index = 0;
		int size = l.size ();
		for (Object o : l) {
			if (o instanceof String) {
				s += (String)o;
			} else if (o instanceof List) {
				List<?> sl = (List<?>)o;
				s += "[";
				s += listFormat(sl);
				s += "]";
			}
			if (++index != size) {
				s += ",";
			}
		}
		return s;
	}

	/**
	 * to string
	 *
	 * @return a formatted string
	 */
	public String toString() {
		String a = new String(name);
		a += "\t";
		if (isList()) {
			List<?> l = getListValue();
			a += "[";
			a += listFormat(l);
			a += "]@";
			a += l.size();
		} else {
			String x = getStringValue();
			if (x == null || x.length() == 0) {
				a += "\"\"";
			} else {
				a += x;
			}
		}
		return a;
	}
}
