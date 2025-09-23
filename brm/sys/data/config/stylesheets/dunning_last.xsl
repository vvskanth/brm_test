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

<body bgcolor="#FFFFFF">
<!-- Function to get the date from passed in value from Portal Data -->
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

<!-- Function to get the current date -->
<script language="JavaScript">
	<xsl:comment>
		<![CDATA[
				function current_date(){
					var dt = new Date();
					//Parse String and output month Date Year.
					//Output is of the format :
					//Day Month Date Time Zone Year
					//var dtarr = dt.toString().split(' ');
					var m = dt.getMonth();
					var d = dt.getDate();
					var y = dt.getYear();
					//return dtarr[1] + ' ' + dtarr[2] + ', ' + dtarr[5];
					return (m+1)+"/"+d+"/"+y;
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
<table border="0" cellpadding="0" align="left">
<tr>
<td  width="122" valign="top">
<br clear="all"></br>
	  <table border="0" cellpadding="0" valign="top" >
	 <tr>
	  <td>
	  <IMG src="http://priya420/test/duke.gif" />
	  </td>
	 </tr>
	</table>
</td>

<td  valign="top">
<br clear="all"></br>
		<table border="0" cellspacing="0" cellpadding="0" valign="top" width="100%">
		 <xsl:call-template name="COMPANY"/>
		  <tr>
		  	<td>
		  		<br>
		  			<script language="JavaScript">
						document.write(current_date());
					</script> 
				</br>
		 	 </td>
		 </tr>
		 <xsl:call-template name="ADDRESS" />
		<tr>
			  <td><br>
			  		<h3> 
			  			<u>
			  			FINAL NOTICE (Re:Account No. <xsl:value-of select="/DUNNING/ACCTINFO/ACCOUNT_NO"/>)
						</u>
			  		</h3>
			  </br></td>
		 </tr> 
		
		 <tr>
			  <td>
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
			  </td>
		 </tr>
		
		 <tr>
			  <td>
			       <br>
			  		      You have been given more than ample time and notices regarding your now seriously overdue debt of 								<xsl:value-of select="$currency-symbol"/><xsl:value-of 														select="/DUNNING/DUE"/> with us. We have instructed our offices to stop all supplies to your 							company till this account is cleared. Your immediate attention is sought to this urgent matter.
				 </br>
			  </td>
		 </tr>
		 
		  <tr>
			  <td>
			  	<br>	  	
					Please understand that this is our final notice to you. However, we give you one last opportunity to clear the 					account by 	June 14th, 2002. After this date it is our normal policy to pass-on seriously delinquent accounts to a 					third party collection	agency or our legal counsel. This could further jeopardize your credit rating in the industry.
					We trust that this will not be necessary.
				</br>
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
					<font face="Script" size="6"><b><i>Carrie Thomas</i></b></font>
			  	</br>
			  </td>
		 </tr>
		 <tr>
			  <td>
			  	Manager
			  </td>
		 </tr>
		  <tr>
			  <td>	  	
			  	Collections Department
			  </td>
		 </tr>
		
		</table>
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
	 	 	<font size="2.5">Portal Information Network</font>
 	 </td>
</tr>
 <tr align="left">
	  <td>
	  		<font size="2.5">10200, S De Anza Blvd</font>
	  </td>
 </tr> 
 <tr align="left">
	  <td>
	  		<font size="2.5">Cupertino , CA 95014</font>
	  </td>
 </tr> 
  <tr align="left">
	  <td>
	  		<font size="2.5">Phone : 408-572-2000</font>
	  </td>
 </tr>
   <tr align="left">
	  <td>
	  		<font size="2.5">Fax : 408-678-0987</font>
	  </td>
 </tr>
    <tr align="left">
	  <td>
  		<font size="2.5">http://www.portal.com</font>
	  </td>
 </tr>
</xsl:template>
</xsl:stylesheet>
