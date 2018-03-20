<?xml version="1.0" encoding="UTF-8"?><!-- -*- mode: nxml; -*- -->

<!-- m4 macro was here... (ignore me) m4_dnl
m4_changequote(`[[[',`]]]')m4_dnl
m4_ifdef([[[__NAMESPACE__]]],,m4_dnl
m4_define([[[__NAMESPACE__]]],[[[http://example.com/SimulinkModel]]]))m4_dnl
-->

<xsd:schema xmlns:xsd="http://www.w3.org/2001/XMLSchema"
	    targetNamespace="__NAMESPACE__"
	    xmlns:sns="__NAMESPACE__">

  <xsd:annotation>
    <xsd:documentation>Simmulink XML Model</xsd:documentation>
  </xsd:annotation>

  <xsd:element name="blocks" type="sns:blocks_T"/>

  <!-- Root -->
  <xsd:complexType name="blocks_T">
    <xsd:sequence>
      <xsd:element name="file" type="sns:file_T"
		   minOccurs="0" maxOccurs="unbounded">
	<xsd:annotation>
	  <xsd:documentation>Processed Input File List</xsd:documentation>
	</xsd:annotation>
      </xsd:element>

      <xsd:element name="shim-params" type="sns:shim_params_T"
		   minOccurs="0" maxOccurs="3">
	<xsd:annotation>
	  <xsd:documentation>SHIM parameters</xsd:documentation>
	</xsd:annotation>
      </xsd:element>

      <xsd:element name="block" type="sns:block_T"
		   minOccurs="0" maxOccurs="unbounded">
	<xsd:annotation>
	  <xsd:documentation>Simulink Block List</xsd:documentation>
	</xsd:annotation>
      </xsd:element>

      <xsd:element name="code" type="sns:code_T"
		   minOccurs="0" maxOccurs="unbounded">
	<xsd:annotation>
	  <xsd:documentation>Program Header/Source Code</xsd:documentation>
	</xsd:annotation>
      </xsd:element>

      <xsd:element name="object" type="sns:object_T"
		   minOccurs="0" maxOccurs="unbounded">
	<xsd:annotation>
	  <xsd:documentation>Program Specific Objects</xsd:documentation>
	</xsd:annotation>
      </xsd:element>

      <xsd:element name="deftype" type="sns:object_T"
		   minOccurs="0" maxOccurs="unbounded">
	<xsd:annotation>
	  <xsd:documentation>Program Specific Types</xsd:documentation>
	</xsd:annotation>
      </xsd:element>

      <xsd:element name="includes" type="sns:include_T"
		   minOccurs="0" maxOccurs="unbounded">
	<xsd:annotation>
	  <xsd:documentation>Header Files</xsd:documentation>
	</xsd:annotation>
      </xsd:element>

      <xsd:element name="function" type="sns:function_T"
		   minOccurs="0" maxOccurs="unbounded">
	<xsd:annotation>
	  <xsd:documentation>Sub Functions</xsd:documentation>
	</xsd:annotation>
      </xsd:element>

    </xsd:sequence>

    <xsd:attribute name="name" type="xsd:string"/>

  </xsd:complexType>

  <xsd:complexType name="file_T">
    <xsd:annotation>
      <xsd:documentation>Input File Information</xsd:documentation>
    </xsd:annotation>

    <xsd:attribute name="type" use="required">
      <xsd:simpleType>
	<xsd:restriction base="xsd:string">
	  <xsd:enumeration value="shim"/>
	  <xsd:enumeration value="shim-schema"/>
	  <xsd:enumeration value="c-source"/>
	  <xsd:enumeration value="c-header"/>
	  <xsd:enumeration value="c-data"/>
	  <xsd:enumeration value="xml"/>
	</xsd:restriction>
      </xsd:simpleType>
    </xsd:attribute>

    <xsd:attribute name="name" type="xsd:string" use="required"/>

    <xsd:attribute name="timestamp" type="xsd:string" use="required"/>
    <xsd:attribute name="size" type="xsd:int" use="required"/>

  </xsd:complexType>

  <xsd:complexType name="shim_params_T">
    <xsd:annotation>
      <xsd:documentation>SHIM Parameters</xsd:documentation>
    </xsd:annotation>

    <xsd:attribute name="element" use="required">
      <xsd:simpleType>
	<xsd:restriction base="xsd:string">
	  <xsd:enumeration value="componentset"/>
	  <xsd:enumeration value="mastercomponent"/>
	  <xsd:enumeration value="commoninstructionset"/>
	</xsd:restriction>
      </xsd:simpleType>
    </xsd:attribute>

    <xsd:attribute name="name" type="xsd:string" use="required"/>

  </xsd:complexType>

  <!-- block infomation -->
  <xsd:complexType name="block_T">
    <xsd:annotation>
      <xsd:documentation>Block Information</xsd:documentation>
    </xsd:annotation>

    <xsd:sequence>

      <xsd:element name="input" type="sns:ioport_T"
		   minOccurs="0" maxOccurs="unbounded">
	<xsd:annotation>
	  <xsd:documentation>input channel information</xsd:documentation>
	</xsd:annotation>
      </xsd:element>

      <xsd:element name="output" type="sns:ioport_T"
		   minOccurs="0" maxOccurs="unbounded">
	<xsd:annotation>
	  <xsd:documentation>output channel information</xsd:documentation>
	</xsd:annotation>
      </xsd:element>

      <!-- recursive <blocks> in <block> -->
      <xsd:element ref="sns:blocks" minOccurs="0" maxOccurs="unbounded"/>

      <xsd:element name="var" type="sns:var_T"
		   minOccurs="0" maxOccurs="unbounded">
	<xsd:annotation>
	  <xsd:documentation>input/output variables</xsd:documentation>
	</xsd:annotation>
      </xsd:element>

      <xsd:element name="signal" type="sns:param_T"
		   minOccurs="0" maxOccurs="unbounded">
	<xsd:annotation>
	  <xsd:documentation>signal</xsd:documentation>
	</xsd:annotation>
      </xsd:element>

      <xsd:element name="state" type="sns:param_T"
		   minOccurs="0" maxOccurs="unbounded">
	<xsd:annotation>
	  <xsd:documentation>states</xsd:documentation>
	</xsd:annotation>
      </xsd:element>

      <xsd:element name="trigger_state" type="sns:param_T"
		   minOccurs="0" maxOccurs="unbounded">
	<xsd:annotation>
	  <xsd:documentation>Trigger states</xsd:documentation>
	</xsd:annotation>
      </xsd:element>

      <xsd:element name="invariant_signal" type="sns:param_T"
		   minOccurs="0" maxOccurs="unbounded">
	<xsd:annotation>
	  <xsd:documentation>Trigger states</xsd:documentation>
	</xsd:annotation>
      </xsd:element>

      <xsd:element name="param" type="sns:param_T"
		   minOccurs="0" maxOccurs="unbounded">
	<xsd:annotation>
	  <xsd:documentation>parameters</xsd:documentation>
	</xsd:annotation>
      </xsd:element>

      <xsd:element name="const_param" type="sns:param_T"
		   minOccurs="0" maxOccurs="unbounded">
	<xsd:annotation>
	  <xsd:documentation>constant parameters</xsd:documentation>
	</xsd:annotation>
      </xsd:element>

      <xsd:element name="code" type="sns:code_T" minOccurs="0"
		   maxOccurs="unbounded">
	<xsd:annotation>
	  <xsd:documentation>Simulink C Code</xsd:documentation>
	</xsd:annotation>
      </xsd:element>

      <xsd:element name="performance" type="sns:performance_T"
		   minOccurs="0" maxOccurs="3">
	<xsd:annotation>
	  <xsd:documentation>SHIM Performance</xsd:documentation>
	</xsd:annotation>
      </xsd:element>

      <xsd:element name="forward" type="sns:link_T"
		   minOccurs="0" maxOccurs="unbounded">
	<xsd:annotation>
	  <xsd:documentation>CSP forward link</xsd:documentation>
	</xsd:annotation>
      </xsd:element>

      <xsd:element name="backward" type="sns:link_T"
		   minOccurs="0" maxOccurs="unbounded">
	<xsd:annotation>
	  <xsd:documentation>CSP backward link</xsd:documentation>
	</xsd:annotation>
      </xsd:element>

      <xsd:element name="merged_to" type="sns:block_name_T"
		   minOccurs="0" maxOccurs="unbounded">
	<xsd:annotation>
	  <xsd:documentation>Merged to</xsd:documentation>
	</xsd:annotation>
      </xsd:element>

      <xsd:element name="merge_block" type="sns:block_name_T"
		   minOccurs="0" maxOccurs="unbounded">
	<xsd:annotation>
	  <xsd:documentation>Merge blocks</xsd:documentation>
	</xsd:annotation>
      </xsd:element>

      <xsd:element name="bind" type="sns:block_name_T"
		   minOccurs="0" maxOccurs="unbounded">
	<xsd:annotation>
	  <xsd:documentation>Bind blocks</xsd:documentation>
	</xsd:annotation>
      </xsd:element>

      <xsd:element name="bound_to" type="sns:block_name_T"
		   minOccurs="0" maxOccurs="unbounded">
	<xsd:annotation>
	  <xsd:documentation>Bound to</xsd:documentation>
	</xsd:annotation>
      </xsd:element>

      <xsd:element name="function" type="sns:function_T"
		   minOccurs="0" maxOccurs="unbounded">
	<xsd:annotation>
	  <xsd:documentation>Block functions</xsd:documentation>
	</xsd:annotation>
      </xsd:element>

    </xsd:sequence>

    <!-- attributes for <block> -->
    <xsd:attribute name="blocktype" type="xsd:string" use="required"/>
    <xsd:attribute name="name" type="xsd:string" use="required"/>
    <xsd:attribute name="type" type="xsd:string" use="optional"/>
    <xsd:attribute name="rate" type="xsd:string" use="optional"/>
    <xsd:attribute name="Period" type="xsd:string" use="optional"/>
    <xsd:attribute name="PulseType" type="xsd:string" use="optional"/>
    <xsd:attribute name="PulseWidth" type="xsd:float" use="optional"/>
    <xsd:attribute name="peinfo" type="xsd:string" use="optional"/>
    <xsd:attribute name="id" type="xsd:int" use="optional"/>
    <xsd:attribute name="priority" type="xsd:int" use="optional"/>
    <xsd:attribute name="sequence" type="xsd:int" use="optional"/>
    <xsd:attribute name="stateflow" type="xsd:string" use="optional"/>
    <xsd:attribute name="DataStoreName" type="xsd:string" use="optional"/>
    <xsd:attribute name="TreatAsAtomicUnit" type="xsd:string" use="optional"/>
    <xsd:attribute name="SampleTimeType" type="xsd:string" use="optional"/>
    <xsd:attribute name="TriggerType" type="xsd:string" use="optional"/>
    <xsd:attribute name="TriggerSignalSampleTime" type="xsd:string"
		   use="optional"/>
    <xsd:attribute name="measuringResult" type="xsd:int" use="optional"/>
    <xsd:attribute name="forced" type="xsd:string" use="optional"/>

  </xsd:complexType>

  <!-- input/output infomation -->
  <xsd:complexType name="ioport_T">
    <xsd:sequence>
      <xsd:element name="connect" type="sns:connection_T"
		   minOccurs="0" maxOccurs="unbounded"/>
    </xsd:sequence>
    <xsd:attribute name="line" type="xsd:string" use="required"/>
    <xsd:attribute name="port" type="xsd:string" use="required"/>
    <xsd:attribute name="type" type="xsd:string" use="optional"/>
    <xsd:attribute name="usename" type="xsd:boolean" use="optional"/>
    <xsd:attribute name="dimensions" type="xsd:string" use="optional"/>
    <xsd:attribute name="selectedsignal" type="xsd:string" use="optional"/>
  </xsd:complexType>

  <!-- connection infomation -->
  <xsd:complexType name="connection_T">
    <xsd:attribute name="block" type="xsd:string" use="required"/>
    <xsd:attribute name="port" type="xsd:string" use="required"/>
  </xsd:complexType>

  <!-- i/o variable info -->
  <xsd:complexType name="var_T">
    <xsd:attribute name="storage" type="xsd:string" use="optional"/>
    <xsd:attribute name="name" type="xsd:string" use="required"/>
    <xsd:attribute name="type" type="xsd:string" use="required"/>
    <xsd:attribute name="mode" use="required">
      <xsd:simpleType>
	<xsd:restriction base="xsd:string">
	  <xsd:enumeration value="extin"/>
	  <xsd:enumeration value="extout"/>
	  <xsd:enumeration value="input"/>
	  <xsd:enumeration value="output"/>
	  <xsd:enumeration value="state"/>
	  <xsd:enumeration value="trigger_state"/>
	  <xsd:enumeration value="invariant_signal"/>
	</xsd:restriction>
      </xsd:simpleType>
    </xsd:attribute>
    <xsd:attribute name="line" type="xsd:string"/>
    <xsd:attribute name="port" type="xsd:string"/>
    <xsd:attribute name="size" type="xsd:int"/>
    <xsd:attribute name="start" type="xsd:int"/>
    <xsd:attribute name="end" type="xsd:int"/>
  </xsd:complexType>

  <!-- parameter infomation -->
  <xsd:complexType name="param_T">
    <xsd:sequence>
      <xsd:element name="initcode" type="sns:code_T"
		   minOccurs="0" maxOccurs="1">
	<xsd:annotation>
	  <xsd:documentation>initial code</xsd:documentation>
	</xsd:annotation>
      </xsd:element>
    </xsd:sequence>
    <xsd:attribute name="storage" type="xsd:string" use="required"/>
    <xsd:attribute name="name" type="xsd:string" use="required"/>
    <xsd:attribute name="type" type="xsd:string" use="required"/>
    <xsd:attribute name="size" type="xsd:int" use="optional"/>
    <xsd:attribute name="value" type="xsd:string" use="optional"/>
  </xsd:complexType>

  <!-- i/o variable type -->

  <!-- code type -->
  <xsd:complexType name="code_T">
    <xsd:simpleContent>
      <xsd:extension base="xsd:string">
	<xsd:attribute name="type" use="required">
	  <xsd:simpleType>
	    <xsd:restriction base="xsd:string">
	      <xsd:enumeration value="task"/>
	      <xsd:enumeration value="update"/>
	      <xsd:enumeration value="init"/>
	      <xsd:enumeration value="state"/>
	      <xsd:enumeration value="invariant_signal"/>
	      <xsd:enumeration value="param"/>
	      <xsd:enumeration value="const"/>
	      <xsd:enumeration value="func"/>
	      <xsd:enumeration value="autovars"/>
	      <!-- C Header/Source -->
	      <xsd:enumeration value="rt-macro"/>
	      <xsd:enumeration value="rt-type"/>
	      <xsd:enumeration value="rt-code"/>
	      <xsd:enumeration value="rt-init"/>
	      <xsd:enumeration value="common"/>
	      <xsd:enumeration value="common-init"/>
	      <xsd:enumeration value="c_macro"/>
	      <xsd:enumeration value="preamble"/>
	    </xsd:restriction>
	  </xsd:simpleType>
	</xsd:attribute>

	<xsd:attribute name="file" type="xsd:string" use="optional"/>
	<xsd:attribute name="line" type="xsd:int" use="optional"/>
	<xsd:attribute name="index" type="xsd:int" use="optional"/>
      </xsd:extension>
    </xsd:simpleContent>
  </xsd:complexType>

  <xsd:complexType name="performance_T">
    <xsd:attribute name="type" use="required">
      <xsd:simpleType>
	<xsd:restriction base="xsd:string">
	  <xsd:enumeration value="task"/>
	  <xsd:enumeration value="update"/>
	  <xsd:enumeration value="init"/>
	</xsd:restriction>
      </xsd:simpleType>
    </xsd:attribute>
    <xsd:attribute name="best" type="xsd:float" use="required"/>
    <xsd:attribute name="typical" type="xsd:float" use="required"/>
    <xsd:attribute name="worst" type="xsd:float" use="required"/>
    <xsd:attribute name="core" type="xsd:string" use="optional"/>
    <xsd:attribute name="method" type="xsd:string" use="optional"/>
  </xsd:complexType>

  <!-- csp link -->
  <xsd:complexType name="link_T">
    <xsd:sequence>
      <xsd:element name="var" type="sns:var_T"
		   minOccurs="0" maxOccurs="unbounded">
	<xsd:annotation>
	  <xsd:documentation>data flow (variable)</xsd:documentation>
	</xsd:annotation>
      </xsd:element>
    </xsd:sequence>
    <xsd:attribute name="block" type="xsd:string" use="required"/>
    <xsd:attribute name="type" use="required">
      <xsd:simpleType>
	<xsd:restriction base="xsd:string">
	  <xsd:enumeration value="data"/>
	  <xsd:enumeration value="event"/>
	  <xsd:enumeration value="port"/>
	  <xsd:enumeration value="init"/>
	</xsd:restriction>
      </xsd:simpleType>
    </xsd:attribute>
    <xsd:attribute name="label" type="xsd:string"/>
  </xsd:complexType>

  <!-- program specific object -->
  <xsd:complexType name="object_T">
    <xsd:simpleContent>
      <xsd:extension base="xsd:string">
	<xsd:attribute name="type" type="xsd:string" use="required"/>
	<xsd:attribute name="has_struct" type="xsd:boolean" use="optional"/>
	<xsd:attribute name="name" type="xsd:string" use="required"/>
	<xsd:attribute name="size" type="xsd:int" use="optional"/>
	<xsd:attribute name="value" type="xsd:string" use="optional"/>
	<xsd:attribute name="kind" use="optional">
	  <xsd:simpleType>
	    <xsd:restriction base="xsd:string">
	      <xsd:enumeration value="rtm-extern"/>
	      <xsd:enumeration value="rtm-pointer"/>
	      <xsd:enumeration value="rtm-object"/>
	      <xsd:enumeration value="bsignal"/>
	      <xsd:enumeration value="bstate"/>
	      <xsd:enumeration value="param"/>
	    </xsd:restriction>
	  </xsd:simpleType>
	</xsd:attribute>
      </xsd:extension>
    </xsd:simpleContent>
  </xsd:complexType>

  <!-- included header files -->
  <xsd:complexType name="include_T">
    <xsd:attribute name="index" type="xsd:int" use="required"/>
    <xsd:attribute name="from" type="xsd:string" use="optional"/>
    <xsd:attribute name="file" type="xsd:string" use="required"/>
    <xsd:attribute name="mark" type="xsd:boolean" use="optional"/>
  </xsd:complexType>

  <xsd:complexType name="function_T">
    <xsd:sequence>
      <xsd:element name="code" type="sns:code_T"
		   minOccurs="0" maxOccurs="1"/>
      <xsd:element name="function" type="sns:function_T"
		   minOccurs="0" maxOccurs="unbounded"/>
      <xsd:element name="block" type="sns:block_name_T"
		   minOccurs="0" maxOccurs="unbounded"/>
    </xsd:sequence>
    <xsd:attribute name="name" type="xsd:string" use="required"/>
    <xsd:attribute name="type" use="required">
      <xsd:simpleType>
	<xsd:restriction base="xsd:string">
	  <xsd:enumeration value="decl"/>
	  <xsd:enumeration value="defn"/>
	</xsd:restriction>
      </xsd:simpleType>
    </xsd:attribute>
    <xsd:attribute name="kind" use="optional">
      <xsd:simpleType>
	<xsd:restriction base="xsd:string">
	  <xsd:enumeration value="init"/>
	  <xsd:enumeration value="task"/>
	  <xsd:enumeration value="update"/>
	</xsd:restriction>
      </xsd:simpleType>
    </xsd:attribute>
  </xsd:complexType>

  <!-- block name for merge, bind -->
  <xsd:complexType name="block_name_T">
    <xsd:attribute name="block" type="xsd:string" use="required"/>
  </xsd:complexType>

</xsd:schema>
