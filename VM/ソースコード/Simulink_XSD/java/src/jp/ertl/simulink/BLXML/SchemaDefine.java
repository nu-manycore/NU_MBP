// -*- mode: java; tab-width: 4; -*-

package jp.ertl.simulink.BLXML;

public class SchemaDefine {
	/**
	 * for the xmlns
	 */
	private static String namespaceURL = "http://example.com/SimulinkModel";

	/**
	 * for the schemaLocation
	 */
	private static String BLXMLSchema = "SimulinkModel.xsd";

	/**
	 * get xmlns
	 *
	 * @return xmlns url
	 */
	public static String getNamespaceURL() {
		return namespaceURL;
	}

	/**
	 * get schema file
	 *
	 * @return schema
	 */
	public static String getBLXMLSchema() {
		return BLXMLSchema;
	}
}
