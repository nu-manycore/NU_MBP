function RTW_Sid2UrlHash() {
	this.urlHashMap = new Array();
	/* <Root>/x */
	this.urlHashMap["testsys:127"] = "testsys.c:21,38&testsys.h:62";
	/* <Root>/Constant */
	this.urlHashMap["testsys:547"] = "testsys.c:24,42&testsys.h:42,65&testsys_data.c:26";
	/* <Root>/Product */
	this.urlHashMap["testsys:548"] = "testsys.c:25,45&testsys.h:66";
	/* <S1>/Constant */
	this.urlHashMap["testsys:545"] = "testsys.c:22,34&testsys.h:39,63&testsys_data.c:23";
	/* <S1>/Sum */
	this.urlHashMap["testsys:546"] = "testsys.c:23,37&testsys.h:64";
	this.getUrlHash = function(sid) { return this.urlHashMap[sid];}
}
	RTW_Sid2UrlHash.instance = new RTW_Sid2UrlHash();
