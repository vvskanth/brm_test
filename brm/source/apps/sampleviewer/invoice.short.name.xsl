<?xml version="1.0"?>
<!-- ==========================================================================  -->
<!-- Copyright (c) 2002, 2009, Oracle and/or its affiliates. 
All rights reserved. -->
<!--                                                                             -->
<!--      This material is the confidential property of Oracle Corporation 	 -->
<!--      or its subsidiaries or licensors and may be used, reproduced, stored   -->
<!--      or transmitted only in accordance with a valid Oracle license or       -->
<!--      sublicense agreement.                                                  -->
<!-- ==========================================================================  -->
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
		version="1.0">

  <xsl:output method="html" indent="yes" />

  <!-- Save the Primary Currency ID -->
  <xsl:variable name="primary-currency-id"> 
	<xsl:value-of select="/invoice/BILLINFO/CURRENCY" />
  </xsl:variable>

  <!-- Save the Primary Currency Symbol -->
  <xsl:variable name="primary-currency-symbol"> 
	<xsl:value-of select="invoice/CURRENCIES/SYMBOL[../@elem = $primary-currency-id]" />
  </xsl:variable>

  <!-- Main Driver Template -->
  <xsl:template match="/">
  <HTML>
    <HEAD><TITLE>
      <xsl:value-of select="invoice/BILLINFO/BILL_NO" />
    </TITLE></HEAD>

  <BODY bgcolor="#ffffff">

  <!-- JavaScript Function to Format Dates -->
  <SCRIPT language="JavaScript">
    <xsl:comment><![CDATA[
	function format_date(utc_date) {
	  var dt = new Date(utc_date * 1000);
	 		// parse string and output Month Date Year
			// Output from toString is of the following format
			// "Day Month Date Time Zone Year"
	  var dtarr = dt.toString().split(' ');
	  return dtarr[1] + ' ' + dtarr[2] + ' ' + dtarr[5];
	}
    ]]>
    </xsl:comment>
  </SCRIPT>

  <!-- Corp Logo -->
  <P align="LEFT">
  <TABLE align="left" border="0">
    <TR valign="top">
      <TD><IMG src="./image.gif" /></TD>
    </TR>
  </TABLE>
  </P>

  <HR size="1" />

  <BR clear="all" />
  <BR clear="all" />

  <!-- Corp Address -->
  <P align="left">
  <TABLE align="left" border="0" cellspacing="0">
    <TR valign="top">
      <TD align="left">
        <FONT size="3">
  		Oracle Corporation. <BR clear="all" />
  		20863 Steven's Creek Boulevard <BR clear="all" />
  		Suite 200 <BR clear="all" />
  		Cupertino, CA 95014 <BR clear="all" />
		<BR clear="all" />
	</FONT>
      </TD>
    </TR>
  </TABLE>
  </P>

  <!-- Cust Address -->
  <P align="right">
  <TABLE align="right" border="0">
    <TR valign="top">
      <TD align="left">
        <FONT size="3">
  	      <xsl:value-of select="invoice/NAMEINFO/SALUTATION" /><xsl:text>&#x20;</xsl:text><xsl:value-of select="invoice/NAMEINFO/FIRST_NAME" /><xsl:text>&#x20;</xsl:text><xsl:value-of select="invoice/NAMEINFO/LAST_NAME" /> <BR clear="all"/>
	      <xsl:call-template name="replaceLineBreaks">
  		  <xsl:with-param name="string" 
                                  select="invoice/NAMEINFO/ADDRESS" /> 
	      </xsl:call-template>
              <xsl:value-of select="invoice/NAMEINFO/CITY" />,<xsl:text>&#x20;</xsl:text><xsl:value-of select="invoice/NAMEINFO/STATE" /><xsl:text>&#x20;</xsl:text><xsl:value-of select="invoice/NAMEINFO/ZIP" /> <BR clear="all"/>
  		<BR clear="all"/>
        </FONT>
       </TD>
    </TR>
  </TABLE>
  </P>

  <BR clear="all" />
  <BR clear="all" />


  <!--  Amount Due Box  -->
  <TABLE width="100%" border="0" cellpadding="0">
   <TR>
    <TD align="left">
      <TABLE width="100%" border="1" cellspacing="0">
        <TR valign="top" align="center">
          <TD bgcolor="#cfcfcf" width="16%">
		<FONT size="2"> Bill Date </FONT>
          </TD>
          <TD bgcolor="#cfcfcf" width="16%">
		<FONT size="2"> Invoice Number </FONT>
          </TD>
          <TD bgcolor="#cfcfcf" width="16%">
		<FONT size="2"> Account Number </FONT>
          </TD>
          <TD bgcolor="#cfcfcf" width="16%">
		<FONT size="2"> Payment Due </FONT>
          </TD>
          <TD bgcolor="#cfcfcf" width="16%">
		<FONT size="2"> Amount Due </FONT>
          </TD>
          <TD bgcolor="#cfcfcf" width="16%">
		<FONT size="2"> Amount Enclosed </FONT>
          </TD>
	  </TR>
        <TR valign="top" >
          <TD align="center">
		<FONT size="2"> 
		  <SCRIPT language="JavaScript">
		    document.write(format_date( <xsl:value-of select="invoice/BILLINFO/MOD_T" /> ));
		  </SCRIPT>
		 </FONT>
          </TD>
          <TD align="center">
		<FONT size="2"> <xsl:value-of select="invoice/BILLINFO/BILL_NO" /></FONT>
          </TD>
          <TD align="center">
		<FONT size="2"> <xsl:value-of select="invoice/ACCTINFO/ACCOUNT_NO" /></FONT>
          </TD>
          <TD align="center">
		<FONT size="2"> 
		  <SCRIPT language="JavaScript">
		    document.write(format_date( <xsl:value-of select="invoice/BILLINFO/END_T" /> ));
		  </SCRIPT>
		 </FONT>
          </TD>
          <TD align="center">
		<FONT size="2"> 
		    <xsl:value-of select="format-number(invoice/BILLINFO/TOTAL_DUE, '0.00')" /> 
		</FONT>
          </TD>
          <TD align="left">
		<FONT size="2"> <xsl:value-of select="$primary-currency-symbol" /></FONT>
          </TD>
	  </TR>
      </TABLE>
    </TD>
   </TR>
  </TABLE>

  <!-- Account Summary -->
  <BR clear="all" />
  <TABLE width="100%" cellspacing="0" border="1">
    <TR valign="top">
      <TH align="left">
        <FONT size="3">Account Summary</FONT>
      </TH>
    </TR>
    <TR valign="top">
      <TD>
	  <TABLE width="100%" border="0">
  	    <TR valign="top" align="left" bgcolor="#cfcfcf">
		<TH align="left" bgcolor="#cfcfcf">Balances</TH>
		<TH width="12%" align="right" bgcolor="#cfcfcf">
		   <xsl:value-of select="$primary-currency-symbol" /> Total </TH>
          </TR>
	    <TR valign="top">
	      <TD align="left">Previous Balance:</TD>
	      <TD align="right">
		    <xsl:value-of select="format-number(invoice/BILLINFO/PREVIOUS_TOTAL, '0.00')"/> 
              </TD>	
	    </TR>
	    <TR valign="top">
	      <TD align="left">Subordinate Accounts:</TD>
	      <TD align="right">
		<xsl:value-of select="format-number(invoice/BILLINFO/SUBORDS_TOTAL, '0.00')" /> 
	      </TD>
	    </TR>
	    <TR valign="top">
	      <TD align="left">Current Balance:</TD>
	      <TD align="right">
		    <xsl:value-of select="format-number(invoice/BILLINFO/CURRENT_TOTAL, '0.00')"/> 
              </TD>
	    </TR>
	    <TR valign="top" >
		<TD></TD>
		<TD><HR size="1"/></TD>
	    </TR>
	    <TR valign="top" >
	  	<TD align="left"><B>Total Balance Due:</B></TD>
		<TD align="right">
                 <B>
		    <xsl:value-of select="format-number(invoice/BILLINFO/TOTAL_DUE, '0.00')"/> 
		 </B>
	       </TD>
	    </TR>
        </TABLE>
      </TD>
    </TR>
  </TABLE>

  <!-- Item Summary -->
  <BR clear="all" />
  <TABLE width="100%" cellspacing="0" border="1">
    <TR valign="top">
      <TH align="left"><FONT size="3">Item Summary</FONT></TH>
    </TR>
    <TR valign="top">
	<TD>
	  <TABLE width="100%" border="0">
	    <TR valign="top" align="left" bgcolor="#cfcfcf">
		<TH width="15%" align="left" bgcolor="#cfcfcf">Item No.</TH>
		<TH align="left" bgcolor="#cfcfcf">Description</TH>
		<TH width="12%" align="right" bgcolor="#cfcfcf"><xsl:value-of select="$primary-currency-symbol" /> Total </TH>
	    </TR>
            <xsl:if test="invoice/AR_ITEMS[@elem = 0]">
              <TR><TD COLSPAN="3"><B>Accounts/Receivable Items</B></TD></TR>
	      <xsl:call-template name="item-template">
		  <xsl:with-param name="item-list" select="invoice/AR_ITEMS" />
	      </xsl:call-template>
            </xsl:if>
            <xsl:if test="invoice/SUB_ITEMS[@elem = 0]">
              <TR><TD COLSPAN="3"><B>Subordinate Items</B></TD></TR>
	      <xsl:call-template name="item-template">
		  <xsl:with-param name="item-list" select="invoice/SUB_ITEMS" />
	      </xsl:call-template>
            </xsl:if>
            <xsl:if test="invoice/OTHER_ITEMS[@elem = 0]">
              <TR><TD COLSPAN="3"><B>Other Items</B></TD></TR>
	      <xsl:call-template name="item-template">
		  <xsl:with-param name="item-list" select="invoice/OTHER_ITEMS" />
	      </xsl:call-template>
            </xsl:if>
	  </TABLE>
      </TD>
    </TR>
  </TABLE>

  <!-- Event Summary -->
  <BR clear="all"/>
  <TABLE width="100%" cellspacing="0" border="1">
    <TR valign="top">
	<TH align="left"><FONT size="3">Event Details</FONT></TH>
    </TR>
    <TR valign="top">
	<TD>
	  <TABLE width="100%" border="0">
	    <TR valign="top"  align="left" bgcolor="#cfcfcf">
		<TH width="15%" align="left" bgcolor="#cfcfcf">Date</TH>
		<TH align="left" bgcolor="#cfcfcf">Description</TH>
		<TH width="30%" align="left" bgcolor="#cfcfcf">Rate Description</TH>
		<TH width="12%" align="right" bgcolor="#cfcfcf"><xsl:value-of select="$primary-currency-symbol" /> Total </TH>
	    </TR>
	    <xsl:call-template name="event-template">
	  	<xsl:with-param name="item-list" select="invoice/AR_ITEMS" />
	    </xsl:call-template>
	    <xsl:call-template name="event-template">
	  	<xsl:with-param name="item-list" select="invoice/SUB_ITEMS" />
	    </xsl:call-template>
	    <xsl:call-template name="event-template">
	  	<xsl:with-param name="item-list" select="invoice/OTHER_ITEMS" />
	    </xsl:call-template>
	  </TABLE>
      </TD>
     </TR>
  </TABLE>

  </BODY>
  </HTML>
  </xsl:template> <!-- End of Main Driver -->

  <!-- Template rule to format ITEMs -->
  <xsl:template name="item-template">
    <xsl:param name="item-list" /> 
    <xsl:for-each select="$item-list">
      <xsl:sort select="ITEM_NO"/>
      <TR valign="top" align="left">
        <TD><FONT size="3"><xsl:value-of select="ITEM_NO"/></FONT></TD>
	  <TD><FONT size="3"><xsl:value-of select="NAME"/></FONT></TD>
	  <TD align="right"><FONT size="3"><xsl:value-of select="format-number(ITEM_TOTAL, '0.00')" /></FONT></TD>
      </TR> 
    </xsl:for-each>
  </xsl:template>

  <!-- Template rule to format EVENTs -->
  <xsl:template name="event-template">
    <xsl:param name="item-list" />
    <xsl:for-each select="$item-list">
    <xsl:sort select="ITEM_NO"/>
      <xsl:for-each select="EVENTS">
        <TR valign="top" align="left">
          <TD><FONT size="3">
		<SCRIPT language="JavaScript">
		    document.write(format_date( <xsl:value-of select="END_T" /> ));
		</SCRIPT>
	      </FONT></TD>
	    <TD><FONT size="3"><xsl:value-of select="LABEL"/></FONT></TD>
	    <TD><FONT size="3"><xsl:value-of select="SYS_DESCR"/></FONT></TD>
	    <TD align="right"><FONT size="3">
	      <xsl:if test="TOTAL[@elem  = $primary-currency-id]" >
	        <xsl:value-of select="format-number(TOTAL/AMOUNT, '0.00')" />
	      </xsl:if>
	    </FONT></TD>
        </TR> 
       </xsl:for-each>
     </xsl:for-each>
  </xsl:template>

  <!-- Rule to remove line breaks  -->
  <xsl:template name="replaceLineBreaks">
    <xsl:param name="string" select="."/>
    <xsl:choose>
         <!-- if the string contains a line break -->
         <xsl:when test="contains($string, '&#xA;')" >
              <!-- Give the part before the line break -->
              <xsl:value-of select="substring-before($string, '&#xA;')"/>
              <!-- then a break element -->
              <BR/>
              <!-- then recursively call this template on the rest of string-->
              <xsl:call-template name="replaceLineBreaks">
                   <xsl:with-param name="string" 
                                   select="substring-after($string, '&#xA;')"/>
              </xsl:call-template>
         </xsl:when>
         <!-- if the string does not contain a line break -->
         <xsl:otherwise>
              <xsl:if test="string-length(normalize-space($string))&gt;0">
                  <xsl:value-of select="$string" />
                  <BR/>
              </xsl:if>
         </xsl:otherwise>
    </xsl:choose>
  </xsl:template>
</xsl:stylesheet>
