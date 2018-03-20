function RTW_Sid2UrlHash() {
	this.urlHashMap = new Array();
	/* <Root>/x */
	this.urlHashMap["testmux3:127"] = "testmux3.c:21,40,48&testmux3.h:66";
	/* <Root>/Constant */
	this.urlHashMap["testmux3:559"] = "testmux3.c:24,44&testmux3.h:43,69&testmux3_data.c:26";
	/* <Root>/Constant1 */
	this.urlHashMap["testmux3:561"] = "testmux3.c:22,36&testmux3.h:40,67&testmux3_data.c:23";
	/* <Root>/Constant2 */
	this.urlHashMap["testmux3:565"] = "testmux3.c:26,52&testmux3.h:46,71&testmux3_data.c:29";
	/* <Root>/Product */
	this.urlHashMap["testmux3:560"] = "testmux3.c:23,39&testmux3.h:68";
	/* <Root>/Sum */
	this.urlHashMap["testmux3:558"] = "testmux3.c:25,47&testmux3.h:70";
	/* <Root>/Sum1 */
	this.urlHashMap["testmux3:564"] = "testmux3.c:27,55&testmux3.h:72";
	this.getUrlHash = function(sid) { return this.urlHashMap[sid];}
}
	RTW_Sid2UrlHash.instance = new RTW_Sid2UrlHash();
