// -*- mode: java; tab-width: 4; -*-

package jp.ertl.simulink.rtw;

import java.util.List;

import jp.ertl.simulink.rtw.RTWParam;
import jp.ertl.simulink.rtw.RTWSectionBase;

/**
 * RTW SampleTime class (it is in CompiledModel)
 */
public class RTWSampleTime extends RTWSectionBase {
	/**
	 * period value of `PeriodAndOffset'
	 */
	private double period = 0.0;

	/**
	 * offset value of `PeriodAndOffset'
	 */
	private double offset = 0.0;

	/**
	 * value of `ClockTickStepSize'
	 */
	private double stepSize = 0.0;

	/**
	 * value of `TID'
	 */
	private int tid = -1;

	/**
	 * SampleTime(continus)
	 */
	private boolean continus = false;

	/**
	 * minumum step (SampleTime(continus) may use this)
	 */
	private static double minStep = 1.0; // big...

	/**
	 * set Period
	 *
	 * @param p period
	 */
	private void setPeriod(double p) {
		period = p;
	}

	/**
	 * set Offset
	 *
	 * @param o offset
	 */
	private void setOffset(double o) {
		offset = o;
	}

	/**
	 * is this SampleTime valid?
	 *
	 * @return <code>true</code> valid
	 */
	public boolean isValid() {
		return tid != -1 && stepSize != 0.0;
	}

	/**
	 * get period
	 *
	 * @return period
	 */
	public double getPeriod() {
		return period;
	}

	/**
	 * get offset
	 *
	 * @return offset
	 */
	public double getOffset() {
		return offset;
	}

	/**
	 * set step-size
	 *
	 * @param s step-size
	 */
	public void setStepSize(double s) {
		stepSize = s;
	}

	/**
	 * get step-size
	 *
	 * @return step-size
	 */
	public double getStepSize() {
		return stepSize;
	}

	/**
	 * is this SampleTime continus?
	 *
	 * @return <code>true</code> continus
	 */
	public boolean isContinus() {
		return continus;
	}

	/**
	 * set continus flag
	 *
	 * @param f continus flag
	 */
	private void setContinus(boolean f) {
		continus = f;
	}

	/**
	 * set minimum step-size
	 *
	 * @param s minimum step-size
	 */
	private static void setMinStep(double s) {
		minStep = s;
	}

	/**
	 * get minimum step-size
	 *
	 * @return minimum step-size
	 */
	public static double getMinStep() {
		return minStep;
	}

	/**
	 * get TID
	 *
	 * @return tid
	 */
	public int getTID() {
		return tid;
	}

	/**
	 * constructor
	 *
	 * @param t a type of section (section identifier)
	 * @param slist list of sections
	 * @param plist list of parameters
	 */
	public RTWSampleTime(String t,
						 List<RTWSectionBase> slist, List<RTWParam> plist) {
		super(t, slist, plist);

		RTWParam p = findParam("TID");
		if (p == null || !p.isString()) {
			return;
		}

		tid = Integer.parseInt(p.getStringValue());

		p = findParam("ClockTickStepSize");
		if (p == null || !p.isString()) {
			return;
		}

		setStepSize(Double.parseDouble(p.getStringValue()));
			
		p = findParam("PeriodAndOffset");
		if (p == null) {
			return;
		}

		if (p.isString()) {
			setPeriod(Double.parseDouble(p.getStringValue()));
		} else if (p.isList()) {
			List<?> l = p.getListValue();
			if (l.size() > 0) {
				Object v = l.get(0);
				if (v instanceof String) {
					setPeriod(Double.parseDouble((String)v));
				}
			}
			if (l.size() > 1) {
				Object v = l.get(1);
				if (v instanceof String) {
					setOffset(Double.parseDouble((String)v));
				}
			}
		} else {
			return;
		}

		if (period == 0.0 && offset == 0.0) {
			setContinus(true);
			setMinStep(stepSize);
		}
//		System.out.println("## TID=" + tid
//						   + "[" + getPeriod() + "," + getOffset() + "]<"
//						   + getStepSize() + ">");
	}
}
