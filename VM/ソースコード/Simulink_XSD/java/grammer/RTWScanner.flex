/* -*- mode: java; c-basic-offset: 4; tab-width: 4; -*-

/*
 * Scanner for .rtw file
 */

package jp.ertl.simulink.rtw.parser;

import jp.ertl.simulink.rtw.parser.RTWParserSym;

import java_cup.runtime.SymbolFactory;
import java_cup.runtime.ComplexSymbolFactory;
import java_cup.runtime.ComplexSymbolFactory.Location;
import java_cup.runtime.Symbol;

%%

%class RTWScanner
%public
%cup
%unicode
%line
%column
%char

%{
	public RTWScanner(ComplexSymbolFactory sf, java.io.Reader reader) {
		this(reader);
		symbolFactory = sf;
	}

	private StringBuilder string = new StringBuilder();

	private ComplexSymbolFactory symbolFactory;

	private Location leftLocation() {
		return new Location(yyline+1, yycolumn+1, yychar);
	}

	private Location rightLocation() {
		return new Location(yyline+1, yycolumn+yylength(),
							yychar+yylength());
	}

	private Symbol symbol(int type) {
		String name = RTWParserSym.terminalNames[type];
//		System.out.println(yyline + ":" + yycolumn + ":" + type);
		return symbolFactory.newSymbol(name, type,
									   leftLocation(), rightLocation());
	}

	private Symbol symbol(int type, Object value) {
		String name = RTWParserSym.terminalNames[type];
//		if (value instanceof String) {
//			System.out.println(yyline + ":" + yycolumn
//							   + ": #" + type + "/" + (String)value);
//		} else {
//			System.out.println(yyline + ":" + yycolumn
//							   + ": #" + type + ": (other)");
//		}
		return symbolFactory.newSymbol(name, type,
									   leftLocation(), rightLocation(),
									   value);
	}
%}

LineTerminator = \r\n|\r|\n
InputCharacter = [^\r\n]
WhiteSpace = {LineTerminator} | [ \t\f]

/* comments */
Comment = {EndOfLineComment}
EndOfLineComment = "#" {InputCharacter}* {LineTerminator}?

Identifier = [a-zA-Z_#$@] ([a-zA-Z_#$@0-9.-])*

/* Decimal */
DecIntegerLiteral = -? (0 | [1-9][0-9]*) [uU]?
DecLongLiteral = {DecIntegerLiteral} [lL]

/* Hexadecimal */
HexIntegerLiteral = 0[xX] [0-9a-fA-F]{1,8} [uU]?
HexLongLiteral = 0[xX] [0-9a-fA-F]{1,16} [lL]?

/* Octal */
OctIntegerLiteral = 0+ [1-3]? [0-7]{1,15} [uU]?
OctLongLiteral = 0+ [1-3]? [0-7]{1,21} [lL]?

/* floating point number */
DoubleLiteral = (-?[0-9]+\.[0-9]*|-?\.[0-9]+|-?[0-9]+) ([eE][+-]?[0-9]+)?
FloatLiteral = (-?[0-9]+\.[0-9]*|-?\.[0-9]+|-?[0-9]+) ([eE][+-]?[0-9]+)? [fF]

/* boolean */
BooleanLiteral = "on" | "off"

/* continuation `...' */
Continuation = \.\.\.{LineTerminator}

%state STRING

%%

<YYINITIAL> {
	"{"	{ return symbol(RTWParserSym.LBRACE); }
	"}"	{ return symbol(RTWParserSym.RBRACE); }
	"["	{ return symbol(RTWParserSym.LBRACK); }
	"]"	{ return symbol(RTWParserSym.RBRACK); }
	"("	{ return symbol(RTWParserSym.LPARA); }
	")"	{ return symbol(RTWParserSym.RPARA); }
	","	{ return symbol(RTWParserSym.COMMA); }
	":"	{ return symbol(RTWParserSym.COLON); }
	";"	{ return symbol(RTWParserSym.SEMICOLON); }
	
	\"	{ string.setLength(0); yybegin(STRING); }

	{BooleanLiteral}	{ return symbol(RTWParserSym.BOOLEAN_LITERAL,
										yytext()); }

	{DecIntegerLiteral}	{ return symbol(RTWParserSym.INT_LITERAL, yytext()); }
	{DecLongLiteral}	{ return symbol(RTWParserSym.INT_LITERAL, yytext()); }
	{HexIntegerLiteral}	{ return symbol(RTWParserSym.INT_LITERAL, yytext()); }
	{HexLongLiteral}	{ return symbol(RTWParserSym.INT_LITERAL, yytext()); }
	{OctIntegerLiteral}	{ return symbol(RTWParserSym.INT_LITERAL, yytext()); }
	{OctLongLiteral}	{ return symbol(RTWParserSym.INT_LITERAL, yytext()); }

	{FloatLiteral}		{ return symbol(RTWParserSym.FLOAT_LITERAL, yytext()); }
	{DoubleLiteral}		{ return symbol(RTWParserSym.FLOAT_LITERAL, yytext()); }

	{Identifier}		{ return symbol(RTWParserSym.IDENTIFIER, yytext()); }

	{Continuation}		{ /* ignore */ }

	{Comment}			{ /* ignore */ }

	{LineTerminator}	{ return symbol(RTWParserSym.NEWLINE, yytext()); }

	{WhiteSpace}		{ /* ignore */ }
}

<STRING> {

	\"{WhiteSpace}*\"	{ /* ignore, concatanated strings */ }

	\"					{ yybegin(YYINITIAL);
							return symbol(RTWParserSym.STRING_LITERAL,
										  string.toString()); }

	\\[btnfr\\\"\']		{ string.append(yytext()); }

	[^\n\r\"\\]+		{ string.append(yytext()); }

	{LineTerminator}	{ string.append(yytext()); }

	\\.					{ throw new RuntimeException("Illegal Escape: "
													 + yytext()
													 + " at line "
													 + yyline); }
}

[^]						{ throw new RuntimeException("Illegal Caracter: "
													 + yytext()
													 + " at line "
													 + yyline); }

<<EOF>>					{ return symbol(RTWParserSym.EOF); }
