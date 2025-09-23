<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
<xsl:output method="html" indent = "yes"/>
<xsl:template match="/">
<html>
<head>
      <title>
		Collections Letter
	</title>
</head>
<!-- Primary Currency ID -->
<xsl:variable name="currency-id">
	<xsl:value-of select="/DUNNING/ACCTINFO/CURRENCY" />
</xsl:variable>
<xsl:variable name="currency-symbol">
	<xsl:value-of select="/DUNNING/CURRENCIES/SYMBOL[../@elem = $currency-id]"/>
</xsl:variable>

<body bgcolor="#CCFFDD">

<script language="JavaScript">
	<xsl:comment>
		<![CDATA[
				function format_date(utc_date){
					var dt = new Date(utc_date * 1000);
					//Parse String and output month Date Year.
					//Output is of the format :
					//Day Month Date Time Zone Year
					var dtarr = dt.toString().split(' ');
					return dtarr[1] + ' ' + dtarr[2] + ', ' + dtarr[5];
				}
		]]>			
	</xsl:comment>	
</script>
<script language="JavaScript">
	<xsl:comment>
		<![CDATA[
				function current_date(){
					var dt = new Date();;
					//Parse String and output month Date Year.
					//Output is of the format :
					//Day Month Date Time Zone Year
					var dtarr = dt.toString().split(' ');
					return dtarr[1] + ' ' + dtarr[2] + ', ' + dtarr[5];
				}
		]]>			
	</xsl:comment>	
</script>

<!--
<h1 align="center" style="text">
	Collections
</h1>
-->
  <!-- Corp Logo -->
  <P align="LEFT">
  <TABLE align="left" border="0">
    <TR valign="top">
      <TD><IMG src="http://priya420/test/duke.gif" /></TD>
    </TR>
  </TABLE>
  </P>

  <HR size="1" />

  <BR clear="all" />
  <BR clear="all" />

<table border="0" cellspacing="0" cellpadding="0" align="left" width="100%">
 <tr>
  	<td>
  		<br>
  			<script language="JavaScript">
				document.write(current_date());
			</script> 
		</br>
 	 </td>
 </tr>
 <xsl:call-template name="COMPANY"/>
 <xsl:call-template name="ADDRESS" />
<tr>
	  <td>
	  <br>
	  		<b> 
	  			<u>
	  			RE : Account No. <xsl:value-of select="/DUNNING/ACCTINFO/ACCOUNT_NO"/> ,
				 Amount of <xsl:value-of select="$currency-symbol"/> <xsl:value-of select="/DUNNING/DUE"/> due on			 		<script language="JavaScript">
						document.write(format_date(<xsl:value-of select="/DUNNING/DUE_T" />));
				</script>
				</u>
	  		</b>
	  	</br>
	  </td>
 </tr> 

 <tr>
	  <td>
	  <br>	
	  		Dear 
	  	 	<xsl:if test="/DUNNING/ACCTINFO/NAMEINFO/SALUTATION[(text())]" >
	  			<xsl:value-of select="/DUNNING/ACCTINFO/NAMEINFO/SALUTATION"/>.
	  			<xsl:value-of select="/DUNNING/ACCTINFO/NAMEINFO/LAST_NAME"/>, 	
	    		</xsl:if>
	  		<xsl:if test="/DUNNING/ACCTINFO/NAMEINFO/SALUTATION[not(text())]" >
	  			<xsl:value-of select="/DUNNING/ACCTINFO/NAMEINFO/FIRST_NAME"/>
	  			<xsl:text disable-output-escaping="yes">&amp;nbsp;</xsl:text>
	  			<xsl:value-of select="/DUNNING/ACCTINFO/NAMEINFO/LAST_NAME"/>,
	  	  	</xsl:if>
	  </br>
	  </td>
 </tr>

 <tr>
	  <td>		
	  			<br>
	  			Our records indicate that your account, <xsl:value-of select="/DUNNING/ACCTINFO/ACCOUNT_NO"/>,
	  			is now past due in the amount of  <xsl:value-of select="$currency-symbol"/><xsl:value-of 							select="/DUNNING/DUE"/> which was due on 
				<SCRIPT language="JavaScript">
						document.write(format_date(<xsl:value-of select="/DUNNING/DUE_T" />));
				</SCRIPT> . Please send in your payment promptly to avoid further collection actions. 
	  		       If you have already sent in your payment, please disregard this notice.
	  		       </br>
	  </td>
 </tr>
 <tr>
	  <td>

	 	 	 If you have questions regarding your account, you may contact us at
	  		(650)678-6789 between the hours of 8 a.m. and 10 p.m.

  </td>
</tr>
 <tr> 
	  <td>	  	  
	  	<br>
	  		Sincerely,
	       </br>	  	
  </td>
 </tr>
  <tr>
	  <td>	 
	 	<br>
	  	Carrie Thomas
	  	</br>
	  </td>
 </tr>
 <tr>
	  <td>	  	
	     <br>
	  	Manager
	  	</br>
	  </td>
 </tr>
  <tr>
	  <td>	  	
	  	Collections Department
	  </td>
 </tr>

</table>
</body>

</html>
</xsl:template>

<!--Address Template -->
<xsl:template name="ADDRESS">
<tr align="left">
  	<td>
	  	<br>
	 	 		<xsl:value-of select="/DUNNING/ACCTINFO/NAMEINFO/LAST_NAME"/>, 
	 	 		<xsl:value-of select="/DUNNING/ACCTINFO/NAMEINFO/FIRST_NAME"/>
	 	 </br>
 	 </td>
 </tr>
 	
 <tr align="left">
  	<td>
  			<xsl:value-of select="/DUNNING/ACCTINFO/NAMEINFO/ADDRESS"/>
  	</td>
 </tr>
 <tr align="left">
	  <td>   
	              <xsl:value-of select="/DUNNING/ACCTINFO/NAMEINFO/CITY"/>,
		  	 <xsl:value-of select="/DUNNING/ACCTINFO/NAMEINFO/STATE"/>,
		  	 <xsl:value-of select="/DUNNING/ACCTINFO/NAMEINFO/ZIP"/> 
	  </td>
 </tr> 
</xsl:template>

<!--Company Template -->
<xsl:template name="COMPANY">
<tr align="left">
  	<td>
	  	<br>
	 	 		PS MOBILE Inc.	
	 	 </br>
 	 </td>
</tr>
 <tr align="left">
	  <td>
	  		10200, S De Anza Blvd
	  </td>
 </tr> 
 <tr align="left">
	  <td>
	  		Cupertino , CA 95014 
	  </td>
 </tr> 
  <tr align="left">
	  <td>
	  		Phone : 408-572-2000
	  </td>
 </tr>
   <tr align="left">
	  <td>
	  		Fax : 408-678-0987
	  </td>
 </tr>
    <tr align="left">
	  <td>
	  		http://www.portal.com
	  </td>
 </tr> 
</xsl:template>
</xsl:stylesheet>
