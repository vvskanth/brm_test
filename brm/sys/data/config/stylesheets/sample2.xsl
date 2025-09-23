<?xml version="1.0"?>
<!-- ================================================================= -->
<!-- Copyright (c) 2000, 2009, Oracle and/or its affiliates. 
All rights reserved. -->

<!--                                                                   -->
<!--    This material is the confidential property of Oracle           -->
<!--	Corporation or its licensors and may be used, reproduced,      -->	
<!--	stored or transmitted only in accordance with a valid	       -->
<!--    Oracle license or sublicense agreement.			       -->
<!-- ================================================================= -->
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
      <xsl:text> - </xsl:text>
      <xsl:value-of select="invoice/NAMEINFO/FIRST_NAME" /> 
      <xsl:text>&#x20;</xsl:text>
      <xsl:value-of select="invoice/NAMEINFO/LAST_NAME" />
    </TITLE>

    <!-- Inline CSS Style element for formatting Invoice Elements -->
    <STYLE type="text/css">
        body { background: #FFEEDD; 
	       font-family: Verdana, Tahoma, Arial, Helvetica;
               font-size: 2pt;
	       margin-left: 10%; margin-right: 10% }
	.head { background: #FFCC99
	      }
	.accountsum.leftpane { background: #FFCC99
	      }
	#secthead { background: #FF6633;
	     color: #FFFFFF
	   }
	TH { background: #FF9999;
	     color: #FFFFFF
	   }
    </STYLE>
    </HEAD>

  <BODY>

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

	function toggle(widget) {
	  if (widget.style.display == "none") {
		widget.style.display = "";
	  } else {
		widget.style.display = "none";
	  }
	}

    ]]>
    </xsl:comment>
  </SCRIPT>

  <!-- Corp Logo -->
  <P>
  <TABLE class="head" border="0">
    <TR valign="middle">
      <TD width="10%" align="left" rowspan="3"><IMG src="./image.gif" /></TD>
		<!-- Corp Address -->
      <TD align="right"> Portal Software Inc. </TD>
    </TR>
    <TR> <TD align="right">10200 S. De Anza Blvd. </TD> </TR>
    <TR> <TD align="right">Cupertino, CA 95014 </TD> </TR>
  </TABLE>
  </P>

  <!-- Cust Address -->
  <P>
  <SPAN class="tearbill" >
  <TABLE width="40%" align="left" border="0">
    <TR valign="top">
      <TD align="left">
  	      <xsl:value-of select="invoice/NAMEINFO/SALUTATION" /><xsl:text>&#x20;</xsl:text><xsl:value-of select="invoice/NAMEINFO/FIRST_NAME" /><xsl:text>&#x20;</xsl:text><xsl:value-of select="invoice/NAMEINFO/LAST_NAME" /> 
      </TD>
     </TR>
     <xsl:call-template name="replaceLineBreaks">
  		<xsl:with-param name="string" 
			select="invoice/NAMEINFO/ADDRESS" />
     </xsl:call-template>
     <TR>
      <TD align="left">
              <xsl:value-of select="invoice/NAMEINFO/CITY" />,<xsl:text>&#x20;</xsl:text><xsl:value-of select="invoice/NAMEINFO/STATE" /><xsl:text>&#x20;</xsl:text><xsl:value-of select="invoice/NAMEINFO/ZIP" /> 
      </TD>
    </TR>
  </TABLE>


  <!--  Amount Due Box  -->
  <TABLE align="right" border="1" width="50%">
       <TR valign="top" >
          <TD width="60%" align="left">
		Bill Date 
          </TD>
          <TD align="right">
		  <SCRIPT language="JavaScript">
		    document.write(format_date( <xsl:value-of select="invoice/BILLINFO/MOD_T" /> ));
		  </SCRIPT>
          </TD>
         </TR>
         <TR>
          <TD width="60%" align="left">
		Invoice Number 
          </TD>
          <TD align="right">
		<xsl:value-of select="invoice/BILLINFO/BILL_NO" />
          </TD>
	 </TR>
         <TR>
          <TD width="60%" align="left">
		Account Number 
          </TD>
          <TD align="right">
		<xsl:value-of select="invoice/ACCTINFO/ACCOUNT_NO" />
          </TD>
	 </TR>
         <TR>
          <TD width="60%" align="left">
		Payment Due 
          </TD>
          <TD align="right">
		  <SCRIPT language="JavaScript">
		    document.write(format_date( <xsl:value-of select="invoice/BILLINFO/END_T" /> ));
		  </SCRIPT>
          </TD>
	 </TR>
         <TR>
          <TD width="60%" align="left">
		Amount Due 
          </TD>
          <TD align="right">
		    <xsl:value-of select="format-number(invoice/BILLINFO/TOTAL_DUE, '0.00')" /> 
          </TD>
	 </TR>
         <TR>
          <TD width="60%" align="left">
		Amount Enclosed 
          </TD>
          <TD align="left">
		<xsl:value-of select="$primary-currency-symbol" />&#x20;&#x20;&#x20;&#x20;&#x20;&#x20;
          </TD>
      </TR>
  </TABLE>
  </SPAN>
  </P>

  <BR clear="all"/>
  <BR clear="all"/>
  <HR />
  <BR clear="all"/>

  <!-- Account Summary -->
  <TABLE class="accountsum" width="100%" cellspacing="0" border="0">
  <TR>
	<!-- Left Pane -->
  <TD class="leftpane" valign="top" width="20%" rowspan="100">
	<BR clear="all" />
        <a href="javascript: alert('Infranet Customer Care');"><img border="0" src="./cc.jpg" alt="Infranet Customer Care" width="130"/></a>
	<BR clear="all" />
        <a href="javascript: alert('Infranet Online Payment');"><img border="0" src="./op.jpg" alt="Infranet Online Payment" width="130"/></a>
	<BR clear="all" />
        <a href="javascript: alert('Report Problems / Provide Feedback');"><img border="0" src="./fb.jpg" alt="Report Problems / Provide Feedback" width="130"/></a>
	<BR clear="all" />
	<BR clear="all" />
	<BR clear="all" />
	<B>Special Offers</B>
	<BR clear="all" />
	<BR clear="all" />
        <a href="http://www.portal.com/markets_industries/icp/index.html"><img border="0" src="./sni_icp.gif" alt="Infranet ICP" width="130" height="70" /></a>
	<BR clear="all" />
        <a href="http://www.portal.com/news_events/events.htm"><img src="./sh_events3.gif" alt="Portal Events" border="0" width="130" height="70" /></a>
	<BR clear="all" />
        <a href="http://www.portal.com/solution_sets/index.html"><img border="0" src="./sni_sets.gif" alt="Infranet Solution Sets" width="130" height="70" /></a>
  </TD>
  <TD valign="top" width="2%" rowspan="100">
  </TD>

  <TD>
  <TABLE class="rightpane" width="100%" cellspacing="0" border="0">
    <TR valign="top">
      <TH id="secthead" align="left">
        Account Summary
      </TH>
    </TR>
    <TR valign="top">
      <TD>
	  <TABLE width="100%" border="0">
  	    <TR valign="top" align="left" >
		<TH align="left" >Balances</TH>
		<TH width="12%" align="right">
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
  </TD>
  </TR>
  <TR>
		<!-- Summary of Charges -->
  <TD>
  <TABLE class="itemsum" width="100%" cellspacing="0" border="0">
    <TR valign="top">
      <TH id= "secthead" align="left">Summary of Charges</TH>
    </TR>
    <TR valign="top">
	<TD>
	  <TABLE width="100%" border="0">
	    <TR valign="top" align="left">
		<TH width="15%" align="left">Item No.</TH>
		<TH align="left">Description</TH>
		<TH width="12%" align="right"><xsl:value-of select="$primary-currency-symbol" /> Total </TH>
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
  </TD>
  </TR>
  </TABLE>
  </BODY>
  </HTML>
  </xsl:template> <!-- End of Main Driver -->

  <!-- Template rule to format Event Tables -->
  <xsl:template name="event-table-template">
    <xsl:param name="item-list" />
  <BR clear="all"/>
  <xsl:element name="SPAN">
    <xsl:attribute name="id"><xsl:call-template name="item-id-template"><xsl:with-param name="item-no" select="$item-list/ITEM_NO" /></xsl:call-template></xsl:attribute>
    <xsl:attribute name="STYLE">display: none; background: #ffffcc</xsl:attribute>
  <TABLE width="100%" cellspacing="0" border="1">
    <TR valign="top">
	<TH id="secthead" align="left">Event Details</TH>
    </TR>
    <TR valign="top">
	<TD>
	  <TABLE width="100%" border="0">
	    <TR valign="top"  align="left">
		<TH width="15%" align="left">Date</TH>
		<TH align="left">Description</TH>
		<TH width="30%" align="left">Rate Description</TH>
		<TH width="12%" align="right"><xsl:value-of select="$primary-currency-symbol" /> Total </TH>
	    </TR>
	    <xsl:call-template name="event-template">
	  	<xsl:with-param name="item-list" select="$item-list" />
	    </xsl:call-template>
	  </TABLE>
      </TD>
     </TR>
  </TABLE>
  </xsl:element>
  </xsl:template>

  <!-- Template rule to format ITEMs -->
  <xsl:template name="item-template">
    <xsl:param name="item-list" /> 
    <xsl:for-each select="$item-list">
      <xsl:sort select="ITEM_NO"/>
      <TR valign="top" align="left">
        <TD><xsl:value-of select="ITEM_NO"/></TD>
	   <TD>
	   <xsl:element name="SPAN">
	     <xsl:attribute name="STYLE">cursor: hand; color: blue</xsl:attribute>
	     <xsl:attribute name="onClick">toggle( <xsl:call-template name="item-id-template"><xsl:with-param name="item-no" select="ITEM_NO" /></xsl:call-template>)</xsl:attribute>
	     <xsl:value-of select="NAME"/>
	   </xsl:element>
	   </TD>
	  <TD align="right"><xsl:value-of select="format-number(ITEM_TOTAL, '0.00')" /></TD>
      </TR> 
      <TR>
	<TD colspan="3">
	 <xsl:call-template name="event-table-template">
	   <xsl:with-param name="item-list" select="." />
	 </xsl:call-template>
        </TD>
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
          <TD>
		<SCRIPT language="JavaScript">
		    document.write(format_date( <xsl:value-of select="END_T" /> ));
		</SCRIPT>
	      </TD>
	    <TD><xsl:value-of select="LABEL"/></TD>
	    <TD><xsl:value-of select="SYS_DESCR"/></TD>
	    <TD align="right">
	      <xsl:if test="TOTAL[@elem  = $primary-currency-id]" >
	        <xsl:value-of select="format-number(TOTAL/AMOUNT, '0.00')" />
	      </xsl:if>
	    </TD>
        </TR> 
       </xsl:for-each>
     </xsl:for-each>
  </xsl:template>

  <!-- Template rule to (JavaScript acceptable) IDify a ITEM_NO -->
  <xsl:template name="item-id-template">
    <xsl:param name="item-no" />
    <xsl:variable name="item-id" select="translate($item-no, '-,', '__')" />
    <xsl:value-of select="$item-id" />
  </xsl:template>

  <xsl:template name="replaceLineBreaks">
    <xsl:param name="string" select="."/>
    <xsl:choose>
         <!-- if the string contains a line break -->
         <xsl:when test="contains($string, '&#xA;')" >
              <!-- Give the part before the line break -->
              <TR><TD align="left">
              <xsl:value-of select="substring-before($string, '&#xA;')"/>
              <!-- then a break element -->
              </TD></TR>
              <!-- then recursively call this template on the rest of string-->
              <xsl:call-template name="replaceLineBreaks">
                   <xsl:with-param name="string"
                                   select="substring-after($string, '&#xA;')"/>
              </xsl:call-template>
         </xsl:when>
         <!-- if the string does not contain a line break -->
         <xsl:otherwise>
              <xsl:if test="string-length(normalize-space($string))&gt;0">
                  <TR><TD align="left">
                  <xsl:value-of select="$string" />
                  </TD></TR>
              </xsl:if>
         </xsl:otherwise>
    </xsl:choose>
  </xsl:template>
</xsl:stylesheet>
