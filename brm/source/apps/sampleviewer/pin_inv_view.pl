#!/home/applmgr/app/brm111/BRM/15.0.1/ThirdParty/perl/5.38.2/bin/perl
##############################################################################
#
# @(#)pin_inv_view.pl 
#
# Copyright (c) 1999, 2018, Oracle and/or its affiliates. All rights reserved.
#
#       This material is the confidential property of Oracle Corporation
#       or its subsidiaries or licensors and may be used, reproduced, stored
#       or transmitted only in accordance with a valid Oracle license or
#       sublicense agreement.
##############################################################################

use lib '.';
use Carp;
use CGI;
use POSIX qw(mktime);
use Time::localtime;

##############################################################################
# pcmif is the perl pcm library
##############################################################################
use pcmif;

##############################################################################
# pin_inv_view.conf has the gif location, file and stylesheet array  
##############################################################################
require "./pin_inv_view.conf";

##############################################################################
# Globals
##############################################################################
my $dump = 1;
my $DB_NO;
my $pcm_ctxp;

						# map for data sent to browser
%outformats = ( "styledxml" => "xml",
		"rawxml"    => "",
		"html"	    => "html",
		"doc1"	    => "",
		"flist"	    => ""
	      );

						# map for opcode request
%invformats = ( "styledxml" => qq("text/xml"),
		"rawxml"    => qq("text/xml"),
		"html"	    => qq("text/html"),
		"doc1"	    => qq("text/doc1"),
		"flist"     => qq("text/pin_flist")
	      );

						# function callbacks
%funcmap = ( "start" => \&pin_start, 
	     "search" => \&pin_show_invoices,
	     "view"  => \&pin_grab_and_show_invoice
	   );

##############################################################################
# MAIN: Grab the state from the front end and perform appropriate callback
##############################################################################

  my ($q, $state);

  $q = new CGI;

  $state = $q->param('state');
  $state = "start" if !$state;

  pin_write_log_message("state=$state\n") if $dump;
  pin_init();

  &{$funcmap{$state}}($q);

  pin_exit();

  exit(0);

##############################################################################
# This subroutine constructs the Invoice viewer form for the first time
# and sends it back to the browser.  
##############################################################################
sub pin_start {
	my ($query) = @_;

						# reset account No.
	$query->param(-name=>'account_no', -value=>'');
						# some default values
	$query->param(-name=>'invoice_type', -value=>'regular');
	$query->param(-name=>'startdt', -value=>'01/01/1999');
	$query->param(-name=>'enddt', -value=>'12/31/1999');

	my $nextstate = "search";
	my $rightframestr = "&nbsp;&nbsp;Please specify an account number<BR>  &nbsp;&nbsp;Also specify the Date range in mm/dd/yyyy format and press Search Button";

	pin_construct_and_show_form($query, $nextstate, $rightframestr);

}

##############################################################################
# This subroutine searches for bills
# for the given timeperiod and returns the bill list if any
##############################################################################
sub pin_show_invoices {
	my ($query) = @_;

	my %param = ();
        my ($accountref, $result);
	my ($starttime, $endtime);
	my (@account_ref);
	my ($account_obj);
	my ($searchall);
        my (@date);
	my ($billref);
	my ($show_regular, $show_trial);
	my ($rightframestr);

	$param{"account_no"} =  '"' . $query->param('account_no') . '"';

	@account_ref = split('-', $query->param('account_no'));
	$account_obj = $account_ref[0] . " /account " . $account_ref[1];
					# now go and grab the bills for the
					# timeperiod
	$show_regular = $query->param('show_regular');
	$show_trial = $query->param('show_trial');
	pin_write_log_message("show_regular=$show_regular, show_trial=$show_trial\n") if $dump;
	if ($query->param('searchall') =~ /true/) {
		$searchall = 1;
	} else {
		$searchall = 0;
		@date = split('/',$query->param('startdt'));
		$starttime = timelocal(0,0,0,$date[1],$date[0]-1,$date[2]);
		@date = split('/',$query->param('enddt'));
		$endtime = timelocal(59,59,23,$date[1],$date[0]-1,$date[2]);
	}

	$billref = pin_search_invoice_objs ($account_obj, $show_regular, 
			$show_trial, $searchall, $starttime, $endtime);

	if (@$billref) {
		$rightframestr = pin_construct_invoice_list($billref);
		pin_construct_and_show_form($query,
					"view",
					$rightframestr, 
					&escape($account_obj));
	} else {
            	pin_construct_and_show_form($query,
			"search",
		"&nbsp;&nbsp;<B>No Bills Found for the Time period specified" .
		"</B></BR>&nbsp;&nbsp;Please change the Bill date range " .
		"and search again");
	}
	return;
}

##############################################################################
# This subroutine constructs a html table of invoice objects to be 
# shown in the right frame of the invoiceviewer
##############################################################################
sub pin_construct_invoice_list {
	my ($billref) = @_;

	my ($rightframestr);
	my ($onebill);
	my ($tm, $created_tm, $created_t, $billdate, $invoicepoid, $billno, $trial);
	my ($stylesheet);
	my $selected = 0;

	$rightframestr = <<EOT
<TABLE BORDER="0">
<TR>
<TD VALIGN="top" COLSPAN=2 ROWSPAN=2 WIDTH=400>
<B>Please choose a format and click on a bill</B><BR>
<INPUT TYPE="radio" NAME="format" VALUE="styledxml" CHECKED>XML with XSL</INPUT>
&nbsp; Stylesheet: 
<SELECT NAME="stylesheet">
EOT
	;

	for $stylesheet (@stylesheets) {
	  	$rightframestr .= '<OPTION VALUE="' . $stylesheet . 
				  ($selected?"\"":'" SELECTED') .
				  '>' . $stylesheet . '</OPTION>'; 
		$selected = 1 if !$selected;
	}
	
	$rightframestr .= <<EOT
</SELECT>
<INPUT TYPE="radio" NAME="format" VALUE="rawxml">Raw XML</INPUT>
<INPUT TYPE="radio" NAME="format" VALUE="html">HTML</INPUT> 
<INPUT TYPE="radio" NAME="format" VALUE="doc1">DOC1</INPUT> 
<INPUT TYPE="radio" NAME="format" VALUE="flist">FLIST</INPUT> 
</TD>
</TR>
<TR>&nbsp;</TR>
EOT
	;
	for $onebill (@$billref) {	
		$invoicepoid = &escape(@$onebill[0]);
		($billdate = @$onebill[1]) =~ s/.*\((.*)\).*/$1/;
		($billno = @$onebill[2]) =~ tr/\"/ /;
		($created_t = @$onebill[3]) =~ s/.*\((.*)\).*/$1/;
		if ($invoicepoid =~ /trial/) {
			$trial = "<B>T</B>";
		} else {
			$trial = "&nbsp;";
		} 
		$tm = localtime($billdate);
		$created_tm = localtime($created_t);
		pin_write_log_message("invoicepoid=$invoicepoid\tbilldate=$billdate\tcreated_t=$created_t\n") if $dump;
		$rightframestr .= "<TR><TD ALIGN=\"center\" WIDTH=\"10%\">$trial</TD><TD><A HREF=\"javascript:invoiceClickHandler('$invoicepoid')\">$billno - ${\($tm->mon+1)}/${\($tm->mday)}/${\($tm->year+1900)} ... Created on ${\($created_tm->mon+1)}/${\($created_tm->mday)}/${\($created_tm->year+1900)} ${\($created_tm->hour)}:${\($created_tm->min)}:${\($created_tm->sec)}</A></TD></TR>\n"
	}
		
	$rightframestr .= "</TABLE>";
}

##############################################################################
# This subroutine calls into Infranet with the selected bill object
# and the Invoice format requested and sends back the invoice to the 
# browser
##############################################################################
sub pin_grab_and_show_invoice {
	my ($query) = @_;

	my (%param);
	my ($accountref, $result);


					# compare Account cookie
	$prevaccount = $query->cookie(-name=>'aobj');
	$prevaccount = &unescape($prevaccount);

					# everything is OK.  Get Bill obj
	my (%invreqref);
        $invreqref{"inv_poid"} = &unescape($query->param('invoicepoid'));
        $invreqref{"inv_format"} = $invformats{$query->param('format')};
        $invreqref{"output_format"} = $outformats{$query->param('format')};
	$invreqref{"stylesheet"} = $query->param('stylesheet');

        $outputref = pin_grab_invoice(\%invreqref);
        pin_send_invoice(\%invreqref, $outputref);
	
	return;
}

##############################################################################
# This subroutine constructs the Invoice viewer form and sends it back to  
# the browser.  
##############################################################################
sub pin_construct_and_show_form {
	my ($query, $nextstate, $rightframestr, $cookieval) = @_;

 	my $cookie;

	my $hiddenstr = "<INPUT TYPE=\"hidden\" NAME=\"state\" VALUE=\"$nextstate\">\n";
	$hiddenstr .= "<INPUT TYPE=\"hidden\" NAME=\"invoicepoid\" VALUE=\"\">\n";
	$hiddenstr .= "<INPUT TYPE=\"hidden\" NAME=\"searchall\" VALUE=\"\">\n";
	my $account_no = $query->param('account_no');
	my $show_regular = $query->param('show_regular');
	my $show_trial = $query->param('show_trial');
	my $searchall = $query->param('searchall');
	my $startdt = $query->param('startdt');
	my $enddt = $query->param('enddt');

	my $checked_regular = "checked = \"(checked)\"" if $show_regular; 
	my $checked_trial = "checked = \"(checked)\"" if $show_trial; 

	if (defined $cookieval && $cookieval) {
	  	$cookie = $query->cookie(-name=>'aobj', -value=>$cookieval);
  	  	print $query->header(-type=>'text/html',
			       	     -cookie=>$cookie );
	} else {
		print $query->header(-type=>'text/html');
	}

						# construct and return form
	print <<EOT;
<HTML>
<SCRIPT LANGUAGE="javascript">
//---------------------------------------------------------------
//  Bills are shown as links in the right frame as a dummy
//  link.  We should set appropriate parameters and 
//  submit the form
//---------------------------------------------------------------
function invoiceClickHandler(invoicepid) {
				// set the hidden invoicepoid element
   var ele = document.forms[0].elements['invoicepoid'];

   ele.value = invoicepid;

				// set the state to view
   document.forms[0].elements['state'].value = "view";

   document.forms[0].submit();
				
				// we are done.. don't follow link
   return (false);
}
//---------------------------------------------------------------
//  Set the state to "login" when  Login button is pressed
//  irrespective of the state of the right frame
//---------------------------------------------------------------
function searchClickHandler() {

				// set the state to search
   document.forms[0].elements['state'].value = "search";

				// well, go ahead and login
   return(true);
}

function searchallClickHandler() {

				// set the state to search
   document.forms[0].elements['state'].value = "search";
   document.forms[0].elements['searchall'].value = "true";

				// well, go ahead and login
   return(true);
}
</SCRIPT>

<BODY>
<FORM NAME="viewer" ACTION="$scriptlocation/pin_inv_view.pl" METHOD="post">
<TABLE BORDER="1" CELLPADDING="10">
$hiddenstr
<TABLE BORDER="0" CELLSPACING="0">
<TR>
<TD><IMG SRC="$giflocation/$logogif"></TD>
<TD ALIGN="center" WIDTH="400"><B>PORTAL INVOICE VIEWER</B></TD>
</TR>
<TR><TD>&nbsp</TD><TD>&nbsp</TD></TR>
<TR>
<TD BGCOLOR="#ccffee"><B>Account No:</B>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<INPUT NAME="account_no" VALUE="$account_no" TYPE="text" LENGTH="60" SIZE="14"> </TD>
<TD ROWSPAN=10 VALIGN="top" ALIGN="LEFT" BGCOLOR="#eeffff">$rightframestr </TD>
</TR>
<TR><TD BGCOLOR="#ccffee">&nbsp;</TD></TR>
<TR><TD BGCOLOR="#ccffee"><INPUT TYPE="checkbox" Name="show_regular" value="1" $checked_regular><B>Regular Invoices</B></TD></TR>
<TD BGCOLOR="#ccffee"><INPUT TYPE="checkbox" Name="show_trial" value="1" $checked_trial><B>Trial Invoices</B></TD></TR>
<TR><TD ALIGN="center" BGCOLOR="#ccffee"><INPUT TYPE="submit" NAME="search" VALUE="Search" onClick="searchallClickHandler()"></TD></TR>
<TR><TD BGCOLOR="#ccffee">&nbsp;</TD></TR>
<TR><TD BGCOLOR="#ccffee"><B>Bill Date Range: (mm/dd/yyyy)</B></TD></TR>
<TR><TD BGCOLOR="#ccffee"><B>Start Date:</B> <INPUT NAME="startdt" VALUE="$startdt" LENGTH="10" SIZE="10"></TD></TR>
<TR><TD BGCOLOR="#ccffee"><B>End   Date:</B>&nbsp;&nbsp;&nbsp;<INPUT NAME="enddt" VALUE="$enddt" LENGTH="10" SIZE="10"></TD></TR>
<TR><TD BGCOLOR="#ccffee">&nbsp;</TD></TR>
<TR><TD ALIGN="center" BGCOLOR="#ccffee"><INPUT TYPE="submit" NAME="search" VALUE="Search" onClick="searchClickHandler()"></TD></TR>
</TABLE>
</TABLE>
</FORM>
</BODY>
</HTML>
EOT
	;
}

##############################################################################
#
# pin_search_invoice_objs
#
#	This subroutine searches for all the invoice objects for the given
#       and account object and returns them in an array
#
##############################################################################
sub pin_search_invoice_objs {
	my ($account_obj, $show_regular, $show_trial, $searchall, $start_time, $end_time) = @_;

	my ($input_flist, $opcode, $flags, $output_flist);
	my ($template, $args);

	my (@sorted_list);
	
	
	$template = "select X from /invoice where F1 = V1 ";
	$input_flist = <<"XXX"
	0 PIN_FLD_POID		POID [0] $DB_NO /search -1
	0 PIN_FLD_FLAGS		INT [0] 256
	0 PIN_FLD_ARGS		ARRAY [1]
	1 	PIN_FLD_ACCOUNT_OBJ	POID [0] $account_obj
XXX
	;
	$args = 2;

	if (!defined($show_regular) || !defined($show_trial)) {
		if ($show_regular) {
			$input_flist .= <<"XXX"
	0 PIN_FLD_ARGS		ARRAY [$args]
	1 	PIN_FLD_POID	POID [0] $DB_NO /invoice%trial -1
XXX
	;
			$template .= "and F$args not like V$args ";
			$args++;
		} elsif ($show_trial) {
			$input_flist .= <<"XXX"
	0 PIN_FLD_ARGS		ARRAY [$args]
	1 	PIN_FLD_POID	POID [0] $DB_NO /invoice%trial -1
XXX
	;
			$template .= "and F$args like V$args ";
			$args++;
		}
	}

	if ($searchall eq 0) {
		
		$input_flist .= <<"XXX"	
	0 PIN_FLD_ARGS		ARRAY [$args]
	1	PIN_FLD_BILL_DATE_T     TSTAMP [0] ($start_time)
	0 PIN_FLD_ARGS		ARRAY [${\($args+1)}]
	1	PIN_FLD_BILL_DATE_T	TSTAMP [0] ($end_time)
XXX
	;
		$template .= "and F$args >= V$args and F${\($args+1)} < V${\($args+1)} ";
	}

	$input_flist .= <<"XXX"
	0 PIN_FLD_TEMPLATE		STR [0] "$template"
	0 PIN_FLD_RESULTS		ARRAY [0]
	1	PIN_FLD_POID		POID [0]   NULL
	1	PIN_FLD_BILL_DATE_T	TSTAMP [0] (0)
	1	PIN_FLD_BILL_NO		STR [0] NULL 
	1	PIN_FLD_CREATED_T	TSTAMP [0] (0)
XXX
		;

        ##############################################################
        # DUMP
        ##############################################################
        if ($dump) {
            pin_write_log_message (
                "\nPerforming PCM_OP_SEARCH\n\n" .
                "INPUT $input_flist \n");
            }

        ##############################################################
        # Perform the opcode
        ##############################################################
        $opcode = "PCM_OP_SEARCH";
        $flags  = "PCM_OP_FLAG_READ_RESULT";
        $output_flist = pin_read_flist($opcode, $flags, \$input_flist);
 
        ##############################################################
        # DUMP
        ##############################################################
        if ($dump) {
            pin_write_log_message ("OUTPUT $$output_flist\n");
        }

        ##############################################################
        # Grab the valid bill objects and their end times resulting
	# from Search and construct a output array 
        ##############################################################
	$INFRANET = &pin_str_flist_to_hash($output_flist);

	local ($i) = 0;
	my ($outlist) = [];
	my ($innerlist);

	while (defined($INFRANET{"main.PIN_FLD_RESULTS[$i].PIN_FLD_POID"})) {
	  if ($INFRANET{"main.PIN_FLD_RESULTS[$i].PIN_FLD_BILL_DATE_T"} !=~ /null/) {
	    $innerlist = [];
	    push @$innerlist, 
			($INFRANET{"main.PIN_FLD_RESULTS[$i].PIN_FLD_POID"},
			 $INFRANET{"main.PIN_FLD_RESULTS[$i].PIN_FLD_BILL_DATE_T"},
			 $INFRANET{"main.PIN_FLD_RESULTS[$i].PIN_FLD_BILL_NO"},
			 $INFRANET{"main.PIN_FLD_RESULTS[$i].PIN_FLD_CREATED_T"});
	  }
	  push (@$outlist, $innerlist);
	  $i++;
	}

				# sort outlist by billno
	@sorted_list = sort {@$a[1] cmp @$b[1]} @$outlist;

	return (\@sorted_list);
}

##############################################################################
#
# pin_grab_invoice
#
#	This subroutine gets the invoice for the specified Bill Object
#	in the given format in the list form
#
##############################################################################
sub pin_grab_invoice {
	my ($inref) = @_;

	my ($input_flist, $opcode, $flags, $output_flist);
	my ($flistp, $ebufp);

	$input_flist = <<"XXX"
	0 PIN_FLD_POID		POID [0] @$inref{"inv_poid"}
	0 PIN_FLD_TYPE_STR	STR  [0] @$inref{"inv_format"}
XXX
	;
        ##############################################################
        # DUMP
        ##############################################################
        if ($dump) {
            pin_write_log_message (
                "\nPerforming PCM_OP_INV_VIEW_INVOICE\n\n" .
                "INPUT $input_flist \n");
            }
        ##############################################################
        # Perform the opcode
        ##############################################################
        $opcode = "PCM_OP_INV_VIEW_INVOICE";
        $flags  = "PCM_OP_FLAG_READ_RESULT";
        $output_flist = pin_read_flist($opcode, $flags, \$input_flist);


        ##############################################################
        # DUMP
        ##############################################################
        if ($dump) {
            pin_write_log_message ("OUTPUT $$output_flist\n");
        }

        ##############################################################
        # Grab the Formatted invoice from the output flist
	# Actually we need to grab PIN_FLD_FORMATS[0].PIN_FLD_BUFFER
	# element
        ##############################################################
			#---------------------------------------------
			# $$output_flist is actually the string form
			# of flist and buffer is printed as a hex
			# and character dump.  We need to convert them
			# accordingly
			#---------------------------------------------
	$INFRANET = &pin_str_flist_to_hash($output_flist);

			#---------------------------------------------
			# We asked for the text/xml content type 
			# invoice.  And Infranet will return just that
			# type.
			#---------------------------------------------
        my ($key, $invoice);
        for $key (keys %$INFRANET) {
           $invoice = @$INFRANET{"$key"}  if $key =~ /PIN_FLD_BUFFER/;
           last if $invoice;
        }

			#---------------------------------------------
			# For "flist" type invoice, we get it in the 
			# the compact form.  We need to convert it back
			# and forth to get the expanded form
			#---------------------------------------------
	if (@$inref{"inv_format"} =~ /flist/) {
		$ebufp = pcmif::pcm_perl_new_ebuf();	
	  	$flistp = pcmif::pin_perl_str_to_flist($invoice, $DB_NO, $ebufp);
  		if (pcmif::pcm_perl_is_err($ebufp)) {
        		pin_write_log_message("pin_grab_invoice: flist conversion to string failed\n") if $dump;
  		}
  		$invoice = pcmif::pin_perl_flist_to_str($flistp, $ebufp);
  		if (pcmif::pcm_perl_is_err($ebufp)) {
        		pin_write_log_message("pin_grab_invoice: string conversion to flist failed\n") if $dump;
  		}
	}
			#---------------------------------------------
			# For "html" type invoice, we need to set the
			# image correctly
			#---------------------------------------------
	if (@$inref{"inv_format"} =~ /html/) {
		$invoice =~ 
		  s/(.*)\.\/image\.gif(.*)/$1${giflocation}\/${logogif}$2/;
	}

	@inv = split("\n",$invoice);
					
                        # remove the last null element.
                        # for flist format, the coversion back and forth
                        # takes care of it
# PETS 34882
#        pop(@inv) if (@$inref{"inv_format"} !~ /flist/);

	return \@inv;
}

##############################################################################
#
# pin_send_invoice
#
#	This subroutine determines the content type and sends back the
#	invoice to the browser
#
##############################################################################
sub pin_send_invoice {

	my ($invreq, $invref) = @_;

	my ($ie5);
	my ($contenttype);
	my $showitashtml = 0;
	my $showitasplain = 0;
	my ($stylepi);

	$ie5 = ($ENV{"HTTP_USER_AGENT"} =~ /MSIE 6.0/);

	$contenttype = (@$invreq{"output_format"} =~ /ml/?
			@$invreq{"inv_format"}:"text\/plain");
	$contenttype =~ s/\"(.*)\".*/$1/;

					# browsers, browsers! 
	$showitashtml = (@$invreq{"inv_format"} =~ /doc1/);
	if (!$showitashtml) {
		$showitashtml = (@$invreq{"inv_format"} =~ /flist/);
	}
	$showitasplain = (@$invreq{"output_format"} =~ /xml/ && !$ie5);

	$contenttype = "text/html" if $showitashtml;
	$contenttype = "text/plain" if $showitasplain;
	
	print "Content-type:  $contenttype \n\n";

	print "<HTML><BODY><PRE>\n" if $showitashtml;

        ###############################################################
        # Attach style sheet if styled xml is asked for
        ###############################################################
	if (@$invreq{"output_format"} =~ /xml/) {
	  $stylepi = '<?xml-stylesheet href="' . "$stylesheetlocation/" . 
			@$invreq{"stylesheet"} .
			'" type="text/xsl" ?>';
	  print shift @$invref,"\n";
	  print $stylepi, "\n";
	}
	
	print join("\n",@$invref),"\n";

	print "</PRE></BODY></HTML>\n" if $showitashtml;
}

##############################################################################
#  U T I L I T Y   S U B  R O U T I N E S
##############################################################################

##############################################################################
#
# pin_init():
#	This routine initializes a connection to CM and
#	initializes some options to their defaults.
#
##############################################################################
sub pin_init {

	my ($ebufp);
	my ($sec);
	my ($min);
	my ($hour);
	my ($mday);
	my ($mon);
	my ($year);
	my ($wday);
	my ($yday);
	my ($isdst);

	######################################################################
	# Set autoflushing on stdout.
	######################################################################
	# $| = 1;

	######################################################################
	# Initialize defaults.
	######################################################################
	if ( $^O =~ /win/i ) {
		$cwd = `cd`;
		$cwd =~ s,\\,/,g;
	} else {
		$cwd = `pwd`;
	}
	chop($cwd);

	$logdir = "$cwd"."/log_dir";

	if (!(defined $printer)) {
		$printer = $ENV{"PRINTER"};
	}

	######################################################################
	# Make connection to CM. 
	######################################################################

        $DB_NO = pin_make_cm_connection($dump);

	return;
}

##############################################################################
#
# pin_str_flist_to_hash()
#	This routine splits the string form flist and converts
#	that to a hash.  This handles arrays and buffers cleanly
#
##############################################################################
sub pin_str_flist_to_hash {
    my ($strflist) = @_;

    my (@struct) =  ("main");			# to keep track of the
						# level

    my $INFRANET = {};				# output hash

    my (@flistlines) = split("\n", $$strflist);

    while ($line=(shift @flistlines)) {

	next if $line =~ /^\#/;			# ignore comments
	
	($level, $fldname, $fldtype, $element, $value) = 
		split(/\s+/, $line, 5);

   						# adjust levels
    	while (@struct > $level+1) {
	   pop (@struct);
    	}

        if ($fldtype =~ /ARRAY/) {
	   push @struct, $fldname . $element;
    	} elsif ($fldtype =~ /BUF/) {		# whole can of worms!
	   $key = join('.',@struct) . ".$fldname";

						# ignore the first line
						# it does'nt contain any data
	   $line = shift @flistlines;

	   do {

	     @buflines = split(/\s+/,$line);
	     $buflines[1] =~ s/([0-9a-fA-F][0-9a-fA-F])/pack("C", hex($1))/ieg;
	     $INFRANET{$key} .= $buflines[1];
	     $line = shift @flistlines;

	   } while ($line  =~ /^x/);

           unshift (@flistlines, $line) if $line;	

    	} else {
	   $key = join('.',@struct) . ".$fldname";
	   $INFRANET{$key} = $value;
    	}
    }

    return \%INFRANET;
}

#############################################################################
# unescape URL-encoded data
#############################################################################
sub unescape {
    my($todecode) = @_;
    $todecode =~ tr/+/ /;       # pluses become spaces
    $todecode =~ s/%([0-9a-fA-F]{2})/pack("c",hex($1))/ge;
    return $todecode;
}

#############################################################################
# URL-encode data
#############################################################################
sub escape {
    my($toencode) = @_;
    $toencode=~s/([^a-zA-Z0-9_\-.])/uc sprintf("%%%02x",ord($1))/eg;
    return $toencode;
}

#############################################################################
#
# pin_read_flist()
# 	sub-routine to process the FList output data
#
#############################################################################
sub pin_read_flist {
	my ($pcm_op, $flags, $in_flist_str) = @_;
	my ($flistp);  
	my ($ebufp);
	my ($ebuf_str);

	#####################################################################
	## note the "pcmif::" prefix - this is a class prefix, meaning that the
	## function "pcm_perl_new_ebuf()" is from the package/class "pcmif".
	##
	##
	## first thing, get an ebuf for error reporting.
	#####################################################################
	$ebufp = pcmif::pcm_perl_new_ebuf();
	
	#####################################################################
	## here is how one converts an ebuf to a printable string.
	#####################################################################
	$ebuf_str = pcmif::pcm_perl_ebuf_to_str($ebufp);
	
    pin_write_log_message ("EBUF:$ebuf_str\n");
	#####################################################################
	# ALWAYS remember to put $DB_NO in your POID!
	# ~~~~~~ 
	# Note: Array type is NOT -1, as with Testnap (sometimes).
	#
	#####################################################################

	$flistp = pcmif::pin_perl_str_to_flist($$in_flist_str, $DB_NO, $ebufp);

	if (pcmif::pcm_perl_is_err($ebufp)) {
	    pin_write_log_message (
  		"pin_read_flist(): flist conversion to check failed:\n" .
                "$$in_flist_str\n");
	    pcmif::pcm_perl_print_ebuf($ebufp);
	    pcmif::pcm_context_close($pcm_ctxp, 0, $ebufp);
	    carp;
	    exit(1);
	}
	#####################################################################	
	# Execute the opcode.
	#####################################################################
	$out_flistp = pcmif::pcm_perl_op($pcm_ctxp, $pcm_op,
		   	$flags, $flistp, $ebufp);

	if (pcmif::pcm_perl_is_err($ebufp)) {
	    pin_write_log_message ("pin_read_flist(): $pcm_op failed!\n");
	$ebuf_str = pcmif::pcm_perl_ebuf_to_str($ebufp);
	    pin_write_log_message ("EBUF:$ebuf_str\n");
  	    pcmif::pcm_perl_print_ebuf($ebufp);
	    pcmif::pcm_context_close($pcm_ctxp, 0, $ebufp);
	    carp;
  	    exit(1);
	}
	#####################################################################	
	# Set the out variable and return.
	#####################################################################
	$out = pcmif::pin_perl_flist_to_str($out_flistp, $ebufp);

	#####################################################################	
	# Cleanup 
	#####################################################################
	
	pcmif::pin_flist_destroy($flistp);
	pcmif::pin_flist_destroy($out_flistp);
	pcmif::pcm_perl_destroy_ebuf($ebufp);

	#####################################################################	
	# Return 
	#####################################################################
	return \$out;
}


#############################################################################
#
# pin_make_cm_connection
# 	sub-routine to connect to infranet
#
#############################################################################
sub pin_make_cm_connection {
        my ($dump) = @_;

	######################################################################
	# Initialize the ebufp. 
	######################################################################
	$ebufp = pcmif::pcm_perl_new_ebuf();

	######################################################################
	# Make connection to CM. 
	######################################################################
  	$pcm_ctxp = pcmif::pcm_perl_connect($DB_NO, $ebufp);

	######################################################################
	# check for errors - best to always do this.
	######################################################################
	if (pcmif::pcm_perl_is_err($ebufp)) {
	    pin_write_log_message (
  		"pin_make_cm_connection(): Connect to CM failed\n");
  		pcmif::pcm_perl_print_ebuf($ebufp);
		pcmif::pcm_context_close($pcm_ctxp, 0, $ebufp);
		carp;
  		exit(1);
	}

	######################################################################
	# Cleanup
	######################################################################
	pcmif::pcm_perl_destroy_ebuf($ebufp);
	return $DB_NO;
}


##############################################################################
#
# pin_exit():
#	This routine disconnects from the CM and does misc cleanup.
#
##############################################################################
sub pin_exit {
	my ($ebufp);

	######################################################################
	# Initialize the ebufp. 
	######################################################################
	$ebufp = pcmif::pcm_perl_new_ebuf();

	######################################################################
	# Disconnect from CM.
	######################################################################
	pcmif::pcm_context_close($pcm_ctxp, 0, $ebufp);

	######################################################################
	# check for errors - best to always do this.
	######################################################################
	if (pcmif::pcm_perl_is_err($ebufp)) {
	        pin_write_log_message ("pin_exit(): Disconnect to CM failed\n");
  		pcmif::pcm_perl_print_ebuf($ebufp);
		carp;
		exit(1);
	}
}

##############################################################################
#
# pin_write_log_message():
#	This routine writes messages to the log file
#
##############################################################################
sub pin_write_log_message {
	my ($msg) = @_;

	print STDERR $msg;
}

sub timelocal {
   return mktime (@_, undef, undef, -1);
}
