#!/home/applmgr/app/brm111/BRM/15.0.1/ThirdParty/perl/5.38.2/bin/perl
#
#       @(#) % %
#
# Copyright (c) 1996, 2021, Oracle and/or its affiliates. All rights reserved.
#      
#       This material is the confidential property of Oracle Corporation or its
#       licensors and may be used, reproduced, stored or transmitted only in
#       accordance with a valid Oracle license or sublicense agreement.
#

#                                                                                         
#      file:  purge_audit_tables.pl                                                       
#
#      This script is used for archiving the old records in the audited tables based on a cutoff
#      time. All the output goes to the file archive.log in the current working directory.
##########################################################################################

#------------------------------------Variable Declaration---------------------------------
use POSIX qw(mktime);
use Getopt::Std;
$ME = $0;
$SQLPLUS = "sqlplus -s";
require "./purge_audit_tables.conf";
#open (STDOUT,">./archive.log");
$debug=1;
$debug = 0 unless  exists $ENV{ARCHIVE_DEBUG};
print $storage_clause, "\n" if $debug;

##########################################################################################
# This subroutine sets the db_alias parameter from the command line 
# if it is supplied.Otherwise it asks for the information interactively.
#
# Subroutine Name ---- set_database_parameter()
# Input           ---- db_alias if supplied in command line
# Output          ---- sets the global variable $PIN_DBNAME
##########################################################################################
sub set_database_parameter {
  print "Entering set_database_parameter subroutine\n" if $debug;
  print "Parameter passed ",@_,"\n" if $debug;
  local($login_parameter_value) = @_;
  if(!($login_parameter_value =~ /^$/)) 
  {
    ($PIN_LOGIN, $PIN_PASSWD, $PIN_DBNAME) = split(/\/|\@/, $opt_l);
    # check if all parameters are defined
    if($PIN_PASSWD || $PIN_LOGIN) {
      print "$ME: ERROR: do not pass login/password , format: </\@db_alias>\n";
      exit(1);
    }

    if($PIN_DBNAME =~ /^$/)
    {
      print "$ME: ERROR: login parameter format: </\@db_alias>\n";
      exit(1);
    }
  } 
  else
  {
    $PIN_LOGIN = '';
    $PIN_PASSWD = '';
    print "Database Alias: ";
    chomp($PIN_DBNAME = <STDIN>);
  }
}
##########################################################################################
# This subroutine prints to the standard output the different command line options 
# available to the user.
#
# Subroutine Name ---- usage()  
# Input           ---- None 
# Output          ---- Outputs the different options for executing the script. 
##########################################################################################
sub usage {
  print "\n\n\t\t\t\t\tINFRANET TABLE ARCHIVING TOOL\n\n";
  print "usage: $ME \n";
  print "\tcreate [ -t<object_list>] [-l</\@db_alias>] \t\t\t\t\t Create History tables\n";
  print "\trenametohist [ -t<object_list> ] [-l</\@db_alias>] \t\t\t\t\t Rename to History table\n";
  print "\tarchivedirect [ -t<object_list> -d<YYYY:MM:DD> -c<commit_size> ] [-l</\@db_alias>] \t Archive the table data\n";
  print "\tupdfromhist [ -t<object_list> -d<YYYY:MM:DD> -c<commit_size> ] [-l</\@db_alias>] \t Update data from Hist table\n";
  print "\tarchiveindirect [ -t<object_list> -d<YYYY:MM:DD> -c<commit_size> ] [-l</\@db_alias>] Move the table data\n";
  print "\treport [ -t<object_list> -d<YYYY:MM:DD>] [-l</\@db_alias>] \t\t\t\t Generate report\n"; 
  print "\thelp \t\t\t\t\t\t\t\t\t\t\t\t Usage of commands\n";
}
##########################################################################################
# This subroutine converts the date supplied in YYYY:MM:DD form to the localtime in seconds.
#
# Subroutine Name ---- get_unix_time()  
# Input           ---- Date in YYYY:MM:DD format 
# Output          ---- Local Time in Seconds 
##########################################################################################
sub get_unix_time {
  print "Entering get_unix_time subroutine\n" if $debug;
  print "Parameter passed ",$_[0],"\n" if $debug;
  @dt = split(/:/,$_[0]);
  $tm = timelocal(0,0,0,$dt[2],$dt[1]-1,$dt[0]-1900);
} 
##########################################################################################
# This routine takes a object name and returns an array of table names related to the object.
#
# Subroutine Name ---- get_table_list()  
# Input           ---- object name. (example - au_account) 
# Output          ---- list of table in a array related to the object 
##########################################################################################
sub get_table_list{
  print "Entering get_table_list subroutine\n" if $debug;
  local($object) = @_;
  if($object =~ /^$/) 
  {
    return; 
  }
  print "Object passed is ",$object,"\n" if $debug;
  my $query_string1 = '
  set heading off;
  set headsep off;
  set recsep off;
  set verify off;
  set feedback off;
  select ddf.sm_item_name from dd_objects_t dd, dd_objects_fields_t ddf
  where ddf.obj_id0=dd.obj_id0 and (dd.name = \'/$object\' or dd.name like \'/$object/%\')
  and ddf.rec_id != 0
  order by ddf.rec_id;';

  local $command_string = eval "qq~$query_string1~";
  local $result = exec_sqlcommand($PIN_LOGIN,$PIN_PASSWD,$PIN_DBNAME,$command_string);
  $result =~ s/^\n|^$//;
  local $table_string = $result;
  local @table_list = split(/\n+/,$result);
  print "Table list ",@table_list,"\n" if $debug;
  return @table_list;
}
#########################################################################################
# This routine takes a table list and forms two maps separating the existing and 
# non-existing history tables corresponding to the tables in the list.
#
# Subroutine Name ---- create_exist_map() 
# Input           ---- array of related tables of a object. 
# Output          ---- creates two global maps of tables whose history tables are present
#                      and not present. 
#########################################################################################
sub create_exist_map{
  print "Entering create_exist_map subroutine\n" if $debug;
  local(@table_list)=@_;
  if ((my $len = @table_list) == 0)
  {
    return; 
  }
  print "Table list ",@table_list,"\n" if $debug;
  %not_exist_hist_table_map =();
  %exist_hist_table_map =();
  local $table_string = join("'\,'",@table_list);
  local $hist_table_string = $table_string;
  $hist_table_string =~ s/^|$/'/g;
  $hist_table_string =~ s/'au_/'h_/g ;
  $hist_table_string =~ tr/a-z/A-Z/ ;
  print $hist_table_string,"\n" if $debug;

  # The following query is used to find the existance of the history tables
  # corresponding to the list of tables passed as parameter.

  my $query_string2 = '
  select table_name from user_tables where table_name in ($hist_table_string);';
  local $command_string = eval "qq~$query_string2~";
  local $result = exec_sqlcommand($PIN_LOGIN,$PIN_PASSWD,$PIN_DBNAME,$command_string);
  $result =~ s/^\n|^$//;
  local $exist_hist_table_string = $result;
  foreach $table_name (@table_list)
  {
    my $table = $table_name;
    my $hist_table = $table_name;
    $hist_table =~ s/^au_/h_/;
    print $hist_table,"\n" if $debug;

    if ($result =~ /$hist_table/i) {
      $exist_hist_table_map{$table} = $hist_table;
      ##print "I am inserting in exist map\n"
    }
    else{
      $not_exist_hist_table_map{$table} =$hist_table;
      ##print "I am inserting in not-exist map\n"
    }
  }
  print "Existing history tables ",%exist_hist_table_map,"\n" if $debug;
  print "Non existing history tables ",%not_exist_hist_table_map,"\n" if $debug;
}
#########################################################################################
# This routine takes the db_alias and command string.Executes the sql command
# string and returns the result. 
#
# Subroutine Name ---- exec_sqlcommand() 
# Input           ---- db_alias and command string. 
# Output          ---- result returned from the execution of the command string 
##########################################################################################
sub exec_sqlcommand{
    print "Entering exec_sqlcommand subroutine\n" if $debug;
    local ($login, $passwd,$db_connect,$command_string) = @_;
    local ($status) = 0;
    local ($exitcode) = 0;

     print $command_string,"\n" if $debug;

     open(SQLFILE,">./purge_audit_tables_$$.sql");
     print SQLFILE $command_string;
     print SQLFILE "\nexit;";
     close SQLFILE;
     $file = "purge_audit_tables_$$.sql";
     $sqlout = exec_sqlcommandfile($login,$passwd,$db_connect,$file);
     unlink("./purge_audit_tables_$$.sql");

     $exit_code = $?;
     $status = ($? >> 8);
    
     if (($sqlout =~ m/ERROR /) or ($sqlout =~ m/Errors /)) {
    	print("$ME: ERROR: SQLPLUS failed (exitcode = $exitcode)\n");
    	print $sqlout;
    	exit(1);
    }
    
     print $sqlout,"\n" if $debug;
     return $sqlout;
}
##########################################################################################
# This routine prints in a formatted manner the table name ,its history table name and 
# the existance of the history table in the database.
#
# Subroutine Name ---- report1() 
# Input           ---- None, reads from the global map exist_hist_table_map and not_exist_hist_table_map
# Output          ---- Prints the output in a formatted manner to the purge_tables.report file  
##########################################################################################
sub report1{
  print "Entering report1 subroutine\n" if $debug;
  foreach  $key (keys(%exist_hist_table_map))
  {
    local $table_name =$key;
    local $hist_table_name = $exist_hist_table_map{$table_name};
    local $exists= "YES";
    write REPORT;
  }
  foreach  $key (keys(%not_exist_hist_table_map))
  {
    local $table_name =$key;
    local $hist_table_name = $not_exist_hist_table_map{$table_name};
    local $exists= "NO";
    write REPORT;
  }
  print REPORT "\n"; 
}
##########################################################################################
# This routine is the main function which generates the various reports.
# 
# Subroutine Name ---- report() 
# Input           ---- List of object names for which the report is required. 
# Output          ---- calls the appropraite report subfunction to print the report. 
##########################################################################################
sub report{
  print "Entering REPORT method \n" if $debug;
  print "Object list passed ",@_,"\n" if $debug;
  local(@object_list)=@_;
  open(REPORT,"> ./purge_tables.report");

format REPORT_TOP =
          TABLE NAME                     HISTORY TABLE NAME         HISTORY TABLE EXISTS
===============================   ===============================  ======================
.

format REPORT =
@<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<   @<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<          @<<<<<
$table_name,$hist_table_name,$exists
.

format COUNT_REPORT_TOP =
                                                ELG ROWS FOR   PCT ELG FOR
          TABLE NAME               TOTAL COUNT    PURGING        PURGING(%)
===============================   ============   ==============  ============
.

format COUNT_REPORT =
@<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<   @###########    @############   @##.###
$table_name,$tot_count,$el_count,$pct_count
.
 
  foreach $object_name (@object_list)
  {
    local @table_list=get_table_list($object_name);
    create_exist_map(@table_list);
    report1();
  }
    print REPORT "\n\n\n";
    $ofh=select(REPORT);
    $~="COUNT_REPORT";
    $^="COUNT_REPORT_TOP";
    $-= 0;
    select($ofh);
  foreach $object_name (@object_list)
  {
    local @table_list1 = get_table_list($object_name);
    local @purge_count=find_purge_count(@table_list1);
    report2(@purge_count);
    print REPORT "\n";
  }

}
##########################################################################################
# This routine takes a list of related tables and archive the data from the tables to the 
# respective history tables.
# Subroutine Name ---- archive_table() 
# Input           ---- List of related tables from which the data needs to be archived. 
# Output          ---- data archived 
##########################################################################################
sub archive_table{
  print "Entering archive_table subroutine\n" if $debug;
  print "Table list passed ",@_,"\n" if $debug;
  local(@table_list) = @_;
  if ((my $len = @table_list) == 0)
  {
    return; 
  }

  # Following PL/SQL block contains the query to find out the eligible rows for archiving from the tables
  # and archives the data accordingly. The column $time which is used for comparing the time is configurable from the 
  # configuration file. The $time variable can be either effective_t or created_t column of the table. 
  # LOGIC: Archive all the revisions of the object whose validity end period is less then the specified date and
  # keep all those revisions which are valid at the specified time and after that. 

  local $cmd_string = '
  DECLARE 
  cursor c1 is
  select a.poid_id0
  from $p_table_name a,
          ( SELECT au_parent_obj_id0,
                   max($time) max_$time
            FROM $p_table_name
            WHERE $time < $unix_time
            group by au_parent_obj_id0
            having count(*) > 1 ) b
  where a.au_parent_obj_id0  = b.au_parent_obj_id0
  and   a.$time             < b.max_$time ;
  
  type poid_typ is table of $p_table_name.poid_id0%TYPE index by binary_integer;
  
  poid_arr        poid_typ;
  
  BEGIN
    open c1;
    LOOP
      fetch c1 bulk collect into poid_arr limit $commit_size;
      IF poid_arr.count <> 0 THEN
  
        forall i in poid_arr.first..poid_arr.last
                insert into $ph_table_name select * from $p_table_name where poid_id0 = poid_arr(i);
  
        forall i in poid_arr.first..poid_arr.last
                delete from $p_table_name where poid_id0 = poid_arr(i);
        $archive_child_table_string
  
        COMMIT;
        poid_arr.delete;
      END IF; 
      EXIT WHEN c1%NOTFOUND;
  
    END LOOP;
    COMMIT;
    close c1;
  
  EXCEPTION
    WHEN OTHERS THEN
      dbms_output.put_line(\'EXCEPTION \'||SQLCODE ||SQLERRM);
  
  END;
/'."\n";
  #===================================child table String===========================================#
  local $child_table_string = '
  forall i in poid_arr.first..poid_arr.last
                insert into $ch_table_name select * from $c_table_name where obj_id0 = poid_arr(i);
  forall i in poid_arr.first..poid_arr.last
                delete from $c_table_name where obj_id0 = poid_arr(i);'."\n";
  
  ########################################### Program logic ########################################
  $table = "";
    local $p_table_name=$table_list[0];
    local $ph_table_name=$exist_hist_table_map{$p_table_name};
    local  $cmd1="";
    print "Table list ",@table_list,"\n" if $debug;
    local $len = @table_list;
    print %exist_hist_table_map,"\n" if $debug;

    # forms the sql statements for all the child tables for the object.
    for($i=1;$i < $len; $i++)
    {
    local $c_table_name = $table_list[$i];
    local $ch_table_name = $exist_hist_table_map{$c_table_name};
    local $cmd2 = eval "qq~$child_table_string~";
    $cmd1 = $cmd1.$cmd2;
    print "************ $cmd1 *********\n" if $debug;
    }
    local $archive_child_table_string = $cmd1;
    local $cmd3= eval "qq~$cmd_string~";
    print $PIN_LOGIN,$PIN_PASSWD,$PIN_DBNAME,"\n" if $debug;
    print "************ $cmd3 *********\n" if $debug;
    print "*****************\n" if $debug;
    
    # execute the Pl/SQL statement block formed above for archiving the data.
    exec_sqlcommand($PIN_LOGIN,$PIN_PASSWD,$PIN_DBNAME,$cmd3);
}
##########################################################################################
# This routine is the main subroutine called for archive the data for a list of objects.
# This routine finds the related tables for each of the object and archive the data from these
# tables.
# Subroutine Name ---- archive() 
# Input           ---- List of objects from which the data needs to be archived. 
# Output          ---- data archived for all the objects 
##########################################################################################
sub archive{
   print "Entering the ARCHIVE method \n" if $debug;
   print "Object List passed ",$_[0],"\n" if $debug;
   local @object_list = split(/\,/,$_[0]); 
  foreach $object_name (@object_list)
  {
     local @table_list = get_table_list($object_name);
     create_exist_map(@table_list);
     local $len=(keys(%not_exist_hist_table_map));
     if ($len != 0)
     {
       print "some history tables are missing.Run the report option to find the table names\n";
       print "First run the create option to create the history tables and then run archive option again\n";
       exit(5);
     }
     archive_table(@table_list);
	if (!($rebuild_index eq 'no'))
	{	
     get_index(@table_list);
     rebuild_indexes();
	}
  }
}
##########################################################################################
# This routine takes a list of related tables and moves the data from the history tables 
# to the main tables.
# Subroutine Name ---- insert_entries() 
# Input           ---- List of tables to which the data needs to be inserted from the history tables. 
# Output          ---- Data moved from history tables to the main tables 
##########################################################################################
sub insert_entries{
  print "Entering insert_entries subroutine\n" if $debug;
  print "Table list passed ",@_,"\n" if $debug;
  local(@table_list) = @_;
  if ((my $len = @table_list) == 0)
  {
    return; 
  }
  local $cmd_string = '
  DECLARE 
  cursor c1 is
  select a.poid_id0
  from $ph_table_name a,
          ( SELECT au_parent_obj_id0,
                   max($time) max_$time
            FROM $ph_table_name
            WHERE $time <= $unix_time
            group by au_parent_obj_id0
          ) b
  where a.au_parent_obj_id0  = b.au_parent_obj_id0
  and   a.$time              = b.max_$time 
  UNION
  select poid_id0
  from $ph_table_name
  where $time > $unix_time;
  
  type poid_typ is table of $ph_table_name.poid_id0%TYPE index by binary_integer;
  
  poid_arr1        poid_typ;
  
  BEGIN
    open c1;
    LOOP
      fetch c1 bulk collect into poid_arr1 limit $commit_size;
      IF poid_arr1.count <> 0 THEN
  
        forall i in poid_arr1.first..poid_arr1.last
                insert into $p_table_name select * from $ph_table_name where poid_id0 = poid_arr1(i);
  
        forall i in poid_arr1.first..poid_arr1.last
                delete from $ph_table_name where poid_id0 = poid_arr1(i);

       $rename_child_table_string
       COMMIT;
       poid_arr1.delete;
     END IF;
     EXIT WHEN c1%NOTFOUND;

   END LOOP;
   COMMIT;
   close c1;
   
  
  EXCEPTION
    WHEN OTHERS THEN
      dbms_output.put_line(\'EXCEPTION \'||SQLCODE ||SQLERRM);
  
  END;
/'."\n";
  #===================================child table String===========================================#
  local $child_table_string = '
  forall i in poid_arr1.first..poid_arr1.last
                insert into $c_table_name select * from $ch_table_name where obj_id0 = poid_arr1(i);
  forall i in poid_arr1.first..poid_arr1.last
                delete from $ch_table_name where obj_id0 = poid_arr1(i);'."\n";
  
  ########################################### Program logic ########################################
  $table = "";
    local $p_table_name=$table_list[0];
    local $ph_table_name=$exist_hist_table_map{$p_table_name};
    local  $cmd1="";
    local $cmd2="";
    local $cmd3="";
    print @table_list,"\n" if $debug;
    local $len = @table_list;
    
    # Forms the sql statements for moving the data for all the child tables of the object.
    for($i=1;$i < $len; $i++)
    {
    local $c_table_name = $table_list[$i];
    local $ch_table_name = $exist_hist_table_map{$c_table_name};
    $cmd3 = eval "qq~$child_table_string~";
    $cmd1 = $cmd1.$cmd3;
    print "************ $cmd1 *********\n" if $debug;
    }
    local $rename_child_table_string = $cmd1;
    $cmd2= eval "qq~$cmd_string~";
    print $PIN_LOGIN,$PIN_PASSWD,$PIN_DBNAME,"\n" if $debug;
    print "************ $cmd2 *********\n" if $debug;
    print "*****************\n" if $debug;
    
    # Execute the PL/SQL block formed for moving the data from history table to audit tables.
    exec_sqlcommand($PIN_LOGIN,$PIN_PASSWD,$PIN_DBNAME,$cmd2);
}
##########################################################################################
# This routine is the main routine which is called for moving required data from history tables
# to the main tables.
# Subroutine Name ---- rename_insert() 
# Input           ---- List of objects 
# Output          ---- Data moved from history tables to the main tables for all the objects. 
##########################################################################################
sub rename_insert{
   print "Entering the rename_insert subroutine \n" if $debug;
   print "Object list passed ",$_[0],"\n" if $debug;
   local @object_list = split(/\,/,$_[0]); 
   foreach $object_name (@object_list)
   {
     local @table_list = get_table_list($object_name);
     create_exist_map(@table_list);
     local $len=(keys(%not_exist_hist_table_map));
     if ($len != 0)
     {
       print "Some history tables are missing.So data cannot be inserted from history\n";
       print "table to the main tables.run the report option to find the missing tables\n"; 
       exit(6);
     }
     insert_entries(@table_list);
   }
}
##########################################################################################
# This routine takes a list of related tables and moves the data from the history tables 
# to the main tables.
# Subroutine Name ---- insert_entries_test() 
# Input           ---- List of tables to which the data needs to be inserted from the history tables. 
# Output          ---- Data moved from history tables to the main tables 
##########################################################################################
sub insert_entries_test{
  print "Entering insert_entries_test subroutine\n" if $debug;
  print "Table list passed ",@_,"\n" if $debug;
  local(@table_list) = @_;
  if ((my $len = @table_list) == 0)
  {
    return; 
  }
#-----------------------------------------------------------------------------------#
  local $query_string1='
  cursor $cursor_name is
  select a.poid_id0
  from $query_table_name a,
          ( SELECT au_parent_obj_id0,
                   max($time) max_$time
            FROM $query_table_name
            WHERE $time < $unix_time
            group by au_parent_obj_id0
            having count(*) > 1 ) b
  where a.au_parent_obj_id0  = b.au_parent_obj_id0
  and   a.$time             < b.max_$time ;';

#-----------------------------------------------------------------------------------#
  local $query_string2='
  cursor $cursor_name is
  select a.poid_id0
  from $query_table_name a,
          ( SELECT au_parent_obj_id0,
                   max($time) max_$time
            FROM $query_table_name
            WHERE $time <= $unix_time
            group by au_parent_obj_id0
          ) b
  where a.au_parent_obj_id0  = b.au_parent_obj_id0
  and   a.$time              = b.max_$time ;
  UNION
  select poid_id0
  from $query_table_name
  where $time > $unix_time;';

#-----------------------------------------------------------------------------------#
  local $query_string3='
  cursor $cursor_name is
  select poid_id0  
  from $query_table_name;';

#-----------------------------------------------------------------------------------#
  $process_string='
    open $cursor_name;
    LOOP
      fetch $cursor_name bulk collect into poid_arr limit $commit_size;
      IF poid_arr.count <> 0 THEN
       $rename_parent_table_string
       $rename_child_table_string
       COMMIT;
       poid_arr.delete;
     END IF;
     EXIT WHEN $cursor_name%NOTFOUND;

   END LOOP;
   COMMIT;
   close $cursor_name;';

 
#-----------------------------------------------------------------------------------#
  local $cmd_string = '
  DECLARE 
  $query_string 

  type poid_typ is table of $query_table_name.poid_id0%TYPE index by binary_integer;
  
  poid_arr        poid_typ;
  
   BEGIN
   $complete_process_string 
   EXCEPTION
     WHEN OTHERS THEN
       dbms_output.put_line(\'EXCEPTION \'||SQLCODE ||SQLERRM);
  
  END;
/'."\n";
#-----------------------------------------------------------------------------------#
   local $parent_insert_table_string = '
   forall i in poid_arr.first..poid_arr.last
           insert into $to_parent_table_name select * from $from_parent_table_name where poid_id0 = poid_arr(i);'."\n";
   local $parent_delete_table_string = '
   forall i in poid_arr.first..poid_arr.last
           delete from $from_parent_table_name where poid_id0 = poid_arr(i);'."\n";
   local $child_insert_table_string = '
   forall i in poid_arr.first..poid_arr.last
           insert into $to_child_table_name select * from $from_child_table_name where obj_id0 = poid_arr(i);'."\n";
   local $child_delete_table_string = '
   forall i in poid_arr.first..poid_arr.last
           delete from $from_child_table_name where obj_id0 = poid_arr(i);'."\n";


#-----------------------------------------------------------------------------------#
#------------------------------------------------------------------------------------------#
# while preparing the query $query_table_name,$cursorname has to be initialised.           #
# The cursor name has to be different each time .                                          #
#------------------------------------------------------------------------------------------#
    $table = "";
    local $p_table_name=$table_list[0];
    local $query_table_name=$p_table_name;
    
    # The following if block is executed if the main table for the object has less then
    # $cutoff_for_purge percentage(supplied in the configuration file) records for archiving.
    if ($main_table_archive_mode == 1)
    {
      local $cursor_name= "cursor1";
      local $query_string= eval "qq~$query_string1~";
      local $rename_child_table_string="";
      local $rename_parent_table_string="";
      
      foreach $table_name (@archive_array)
      {
        my $history_table_name=$table_name;
        $history_table_name =~ s/^au_/h_/;
        if ($table_name eq $p_table_name)
        {
          local $to_parent_table_name=$history_table_name;
          local $from_parent_table_name=$table_name;  
          $rename_parent_table_string= eval "qq~$parent_insert_table_string~";
          local $cmd3= eval "qq~$parent_delete_table_string~";
          $rename_parent_table_string=$rename_parent_table_string.$cmd3;
        }
        else
        {
          local $to_child_table_name=$history_table_name;
          local $from_child_table_name=$table_name;
          local $cmd3 = eval "qq~$child_insert_table_string~";
          $rename_child_table_string=$rename_child_table_string.$cmd3;
          $cmd3= eval "qq~$child_delete_table_string~";
          $rename_child_table_string=$rename_child_table_string.$cmd3;
        }
     }
     foreach $table_name (@rename_array)
     {
       my $history_table_name=$table_name;
       $history_table_name =~ s/^au_/h_/;
       my $temp_table_name=$table_name;
       $temp_table_name =~ s/^au_/t_/;
       $to_child_table_name=$history_table_name;
       $from_child_table_name=$temp_table_name;
       $cmd3= eval "qq~$child_insert_table_string~";
       $rename_child_table_string=$rename_child_table_string.$cmd3;
     }
     local $complete_process_string = eval "qq~$process_string~";

     if (($len = @rename_array) != 0) 
     {
       $cursor_name= "cursor2";
       $rename_child_table_string="";
       $rename_parent_table_string="";
      
       local $cmd3 = eval "qq~$query_string3~";
       $query_string=$query_string.$cmd3;
       foreach $table_name (@rename_array)
       {
         my $temp_table_name=$table_name;
         $temp_table_name =~ s/^au_/t_/;
         $to_child_table_name=$table_name;
         $from_child_table_name=$temp_table_name;
         $cmd3= eval "qq~$child_insert_table_string~";
         $rename_child_table_string=$rename_child_table_string.$cmd3;
       } 
       $cmd3= eval "qq~$process_string~";
       $complete_process_string = $complete_process_string.$cmd3;
     }
     $command_string = eval "qq~$cmd_string~";
   }
   # This else block is executed when the main table for the object has more then 
   # $cutoff_for_purge percentage records to be moved to the history tables.
   # It forms the sql statements for moving the required data for the parent and all the child tables.
   else
   {
      $cursor_name= "cursor1";
      local $rename_parent_table_string ="";
      local $rename_child_table_string =""; 
      $query_string= eval "qq~$query_string2~";
      
      foreach $table_name (@rename_array)
      {
       local $temp_table_name=$table_name;
       $temp_table_name =~ s/^au_/t_/;
        if ($table_name eq $p_table_name)
        {
          local $to_parent_table_name=$table_name;
          local $from_parent_table_name=$temp_table_name;  
          $rename_parent_table_string= eval "qq~$parent_insert_table_string~";
          local $cmd3= eval "qq~$parent_delete_table_string~";
          $rename_parent_table_string=$rename_parent_table_string.$cmd3;
        }
        else
        {
          local $to_child_table_name=$table_name;
          local $from_child_table_name=$temp_table_name;
          local $cmd3 = eval "qq~$child_insert_table_string~";
          $rename_child_table_string= $rename_child_table_string.$cmd3; 
        }
      }
     $complete_process_string = eval "qq~$process_string~";
     $cursor_name= "cursor2";
     $cmd3 = eval "qq~$query_string3~";
     $query_string=$query_string.$cmd3;
     $rename_child_table_string="";
     $rename_parent_table_string="";

     foreach $table_name (@rename_array)
     {
       $history_table_name=$table_name;
       $history_table_name =~ s/^au_/h_/;
       $temp_table_name=$table_name;
       $temp_table_name =~ s/^au_/t_/;
        if ($table_name eq $p_table_name)
        {
          $to_parent_table_name=$history_table_name;
          $from_parent_table_name=$temp_table_name;  
          $rename_parent_table_string= eval "qq~$parent_insert_table_string~";
        }
        else
        {
           $to_child_table_name=$history_table_name;
           $from_child_table_name=$temp_table_name;
           $cmd3= eval "qq~$child_insert_table_string~";
           $rename_child_table_string=$rename_child_table_string.$cmd3;
        }
       
     } 
     
     foreach $table_name (@archive_array)
     {
       $history_table_name=$table_name;
       $history_table_name =~ s/^au_/h_/;
       $temp_table_name=$table_name;
       $temp_table_name =~ s/^au_/t_/;
       $to_child_table_name=$history_table_name;
       $from_child_table_name=$table_name;
       $cmd3= eval "qq~$child_insert_table_string~";
       $rename_child_table_string=$rename_child_table_string.$cmd3;
       $cmd3= eval "qq~$child_delete_table_string~";
       $rename_child_table_string=$rename_child_table_string.$cmd3;
     }
     $cmd3 = eval "qq~$process_string~";
     $complete_process_string=$complete_process_string.$cmd3;
     $command_string = eval "qq~$cmd_string~";
   }
   print $command_string,"\n" if $debug;
   exec_sqlcommand($PIN_LOGIN,$PIN_PASSWD,$PIN_DBNAME,$command_string);
}

##########################################################################################
# This routine is the main routine which is called for moving required data from history tables
# to the main tables.
# Subroutine Name ---- rename_move() 
# Input           ---- List of objects 
# Output          ---- Data moved from history tables to the main tables for all the objects. 
##########################################################################################
sub rename_move{
  print "Entering the rename_move method \n" if $debug;
  print "Object list passed ",$_[0],"\n" if $debug;
  local @object_list = split(/\,/,$_[0]);
  foreach $object_name (@object_list)
  {
     local @table_list = get_table_list($object_name);
     create_exist_map(@table_list);
     local $len=(keys(%not_exist_hist_table_map));
     if ($len != 0)
     {
       print "Some history tables needs to be created.Find the missing tables by running report option\n";
       print "Try the create option to create missing history tables\n";
       exit(4);
     }
     find_purge_tables(@table_list);  
     print "rename_tables\n",@rename_array,"\n","archive_tables\n",@archive_array,"\n" if $debug;
     get_index(@table_list);
     export_table(@rename_array);
     rename_tables(mtot,@rename_array);
     import_table("y");
     import_table("n");
     insert_entries_test(@table_list);
     create_indexes();
	if (!($rebuild_index eq 'no'))
	{
     rebuild_indexes();
	}
     drop_tables(@rename_array);
  }
}
##########################################################################################
# This routine is the main routine which is called for renaming the audit tables to corresponding History tables.
# Also it recreates the audit table from the  export dump taken before renaming.
# Subroutine Name ---- rename_to_hist_object() 
# Input           ---- List of objects 
# Output          ---- All the tables for the list of objects are renamed to their respective history tables. 
##########################################################################################
sub rename_to_hist_object{
  print "Entering the rename_to_hist_object method \n" if $debug;
  print "Object list passed ",$_[0],"\n" if $debug;
  local @object_list = split(/\,/,$_[0]);
  foreach $object_name (@object_list)
  {
     local @table_list = get_table_list($object_name);
     create_exist_map(@table_list);
     local $len=(keys(%exist_hist_table_map));
     if ($len != 0)
     {
       print "Some history tables are present currently.Find the existing history tables by running report option\n";
       print "Remove/backup the existing history tables before trying this command\n";
       exit(4);
     }
     print "rename_tables\n",@table_list,"\n" if $debug;
     get_index(@table_list);
     export_table(@table_list);
     rename_tables(mtoh,@table_list);
     import_table("y");
     import_table("n");
  }
}
##########################################################################################
# This is the main code of the script.It parses the command line arguments and calls the 
# appropriate subroutine to achive the result. 
##########################################################################################

$cmd = @ARGV[0];
if(!($cmd =~ /(create|renametohist|updfromhist|archiveindirect|archivedirect|report|help)/))
{
  usage();
  exit(1);
}
shift(@ARGV);

if ($cmd =~ /archivedirect/) 
{
  unless( getopts('d:t:c:l:')) 
  {
    print "$ME: ERROR: $cmd [-d <YYYY:MM:DD> -t <object_list> -c <commit_size> -l </\@db_alias >] \n";
    exit(1);
  }
  set_database_parameter($opt_l);
  if($opt_d =~ /^$/)
  {
    usage();
    exit(2);
  }
  else
  {
    $date = $opt_d;
  }
  if(!($opt_t =~ /^$/))
  {
    $table = $opt_t;
  }
  if(!($opt_c =~ /^$/))
  {
    $commit_size = $opt_c; 
  }
  else
  {
    $commit_size = 1000;
  }
  print "Date=$date, Object list= $table, commit size=$commit_size \n" if $debug;
  $unix_time = &get_unix_time($date);
  print "unix time ",$unix_time,"\n" if $debug;
  archive($table);
}
elsif ($cmd =~ /updfromhist/) 
{
  unless( getopts('d:t:c:l:')) 
  {
    print "$ME: ERROR: $cmd [-d <YYYY:MM:DD> -t <object_list> -c <commit_size> -l </\@db_alias >] \n";
    exit(1);
  }
  set_database_parameter($opt_l);
  if($opt_d =~ /^$/)
  {
    usage();
    exit(2);
  }
  else
  {
    $date = $opt_d;
  }
  if(!($opt_t =~ /^$/))
  {
    $table = $opt_t;
  }
  if(!($opt_c =~ /^$/))
  {
    $commit_size = $opt_c; 
  }
  else
  {
    $commit_size = 1000;
  }
  print "Date=$date, Object list= $table, commit size=$commit_size \n" if $debug;
  $unix_time = &get_unix_time($date);
  print "unix time ",$unix_time,"\n" if $debug;
  rename_insert($table);
}
elsif ($cmd =~ /renametohist/) 
{
  unless( getopts('t:l:')) 
  {
    print "$ME: ERROR: $cmd [ -t <object_list> -l </\@db_alias >] \n";
    exit(1);
  }
  set_database_parameter($opt_l);
  if(!($opt_t =~ /^$/))
  {
    $table = $opt_t;
  }
  print "Object list= $table \n" if $debug;
  rename_to_hist_object($table);
}
elsif ($cmd =~ /archiveindirect/) 
{
  unless( getopts('d:t:c:l:')) 
  {
    print "$ME: ERROR: $cmd [-d <YYYY:MM:DD> -t <object_list> -c <commit_size> -l </\@db_alias >] \n";
    exit(1);
  }
  set_database_parameter($opt_l);
  if($opt_d =~ /^$/)
  {
    usage();
    exit(2);
  }
  else
  {
    $date = $opt_d;
  }
  if(!($opt_t =~ /^$/))
  {
    $table = $opt_t;
  }
  if(!($opt_c =~ /^$/))
  {
    $commit_size = $opt_c; 
  }
  else
  {
    $commit_size = 1000;
  }
  print "Date=$date, Object list= $table, commit size=$commit_size \n" if $debug;
  $unix_time = &get_unix_time($date);
  print "unix time ",$unix_time,"\n" if $debug;
  rename_move($table);
}
elsif ($cmd =~ /report/)
{
  unless( getopts('d:t:l:')) 
  {
    print "$ME: ERROR: $cmd [-d <YYYY:MM:DD> -t <object_list> -l </\@db_alias >] \n";
    exit(1);
  }
  set_database_parameter($opt_l);
  if($opt_d =~ /^$/)
  {
    usage();
    exit(2);
  }
  else
  {
    $date = $opt_d;
  }
  if(!($opt_t =~ /^$/))
  {
    $table = $opt_t;
  }
  print "Date=$date, Object list= $table \n" if $debug;
  $unix_time = &get_unix_time($date);
  print "unix_time ",$unix_time,"\n" if $debug;
  @table_array = split("\,",$table);
  report(@table_array);
}
elsif ($cmd =~ /create/)
{
  unless( getopts('t:l:')) 
  {
        print "$ME: ERROR: $cmd [ -t <object_list> -l </\@db_alias >] \n";
        exit(1);
  }
  set_database_parameter($opt_l);
  if(!($opt_t =~ /^$/))
  {
    $table = $opt_t;
  }
  print "Object list= $table \n" if $debug;
  create($table); 

}
elsif ($cmd =~ /help/) 
{
  usage();
}
###########################################################################################
# This routine takes the db_alias and command file as arguments and executes
# the commands contained in the file in sqlplus and returns the result. 
# Subroutine Name ---- exec_sqlcommandfile() 
# Input           ---- db_alias,command file 
# Output          ---- result of the command executed in sqlplus 
###########################################################################################
sub exec_sqlcommandfile {
  print "Entering exec_sqlcommandfile subroutine\n" if $debug;
  local ($login, $passwd,$db_connect,$cmd_file) = @_;
    local ($status) = 0;
    local ($exitcode) = 0;

    #
    # SQLPLUS gives a zero status back if the user/passwd is
    # wrong. So, instead of using system(), we need to capture the output
    # of SQLPLUS and parse it to see if there was an error.
    #
    $sqlout = `$SQLPLUS /\@$db_connect < $cmd_file`;
    $exitcode = $?;
    $status = ($? >> 8);

    if (($sqlout =~ m/ERROR /) or ($sqlout =~ m/Errors /)) {
	print("$ME: ERROR: SQLPLUS failed (exitcode = $exitcode)\n");
	print $sqlout;
	exit(1);
    }
    print "Output ",$sqlout,"\n" if $debug;
    return $sqlout;
}
######################################################################################################
# This routine creates the non existing history tables for a object.
# Subroutine Name ---- create_table() 
# Input           ---- None it reads the global not_exist_hist_table_map to create the tables. 
# Output          ---- Create tables. 
######################################################################################################
sub create_table{
  print "Entering create_table subroutine\n" if $debug;
  local $query_string1 ='create table $hist_table $storage_clause as select * from $table where 1=2;\n';
  foreach  $keym ( keys(%not_exist_hist_table_map))
  {
     print $query_string1,"\n" if $debug;
     local $table = $keym;
     local $hist_table = $not_exist_hist_table_map{$table};
     local $command_string = eval "qq~$query_string1~";
     print "Creating  table $hist_table","\n" if $debug;
     print $command_string,"\n" if $debug;
     exec_sqlcommand($PIN_LOGIN,$PIN_PASSWD,$PIN_DBNAME,$command_string);
  } 
}
######################################################################################################
# This routine creates the non existing history tables for a object.
# Subroutine Name ---- create() 
# Input           ---- List of objects for which non existant history tables will be created.
# Output          ---- Create tables for all the objects. 
######################################################################################################
sub create{
  print "Entering create subroutine \n" if $debug;
  print "Object list passed ",$_[0],"\n" if $debug;
  local @object_list = split(/\,/,$_[0]); 
  foreach $object_name (@object_list)
  {
    local @table_list = get_table_list($object_name);
    create_exist_map(@table_list);
    create_table();
  }
}
######################################################################################################
# This routine finds the count of eligible records for archiving for each of the tables of a object .
# 
# Subroutine Name ---- find_purge_count() 
# Input           ---- List of tables 
# Output          ---- A list which contains the count of records eligible for purging 
######################################################################################################
sub find_purge_count{
  print "Entering find_purge_count subroutine \n" if $debug;
  print "Parameter passed ",@table_list,"\n" if $debug;
  local(@table_list) = @_;
  if ((my $len = @table_list) == 0)
  {
    return; 
  }
  local $query_string = '
  set serveroutput on;
  set verify off;
  set feedback off;
  DECLARE
  return_string VARCHAR(500);
  count2 integer :=0;
  count1 integer :=0;
  count3 integer :=0;
  command_string VARCHAR(500); 
  BEGIN
  execute immediate \'create global temporary table poid_id0_tbl on commit delete rows as \'||
                  \'select poid_id0 from $p_table_name where 1=0 \';

execute immediate \'insert into poid_id0_tbl \'||
       \'select a.poid_id0 \'||
       \'from $p_table_name a, \'||
         \'( SELECT au_parent_obj_id0, \'||
                   \'max($time) max_$time \'||
            \'FROM $p_table_name \'||
            \'WHERE $time < $unix_time \'||
            \'group by au_parent_obj_id0 \'||
            \'having count(*) > 1 ) b \'||
  \'where a.au_parent_obj_id0  = b.au_parent_obj_id0 \'||
  \'and   a.$time             < b.max_$time \';

execute immediate \'select count(*) from poid_id0_tbl\' into count2;
execute immediate \'select count(*) from $p_table_name\' into count1;

dbms_output.put_line(\'$p_table_name\'||\',\'||count1||\',\'||count2);
$child_count_string
execute immediate \'drop table poid_id0_tbl\' ;
EXCEPTION
  WHEN OTHERS THEN
    dbms_output.put_line(\'EXCEPTION ERROR \'||SQLCODE ||SQLERRM);
    execute immediate \'drop table poid_id0_tbl\' ;   
END;
/';

local $child_string ='
execute immediate \'select count(b.obj_id0) from poid_id0_tbl a, $c_table_name b where b.obj_id0 = a.poid_id0 \' into count2;
execute immediate \'select count(obj_id0) from $c_table_name\' into count1;
dbms_output.put_line(\'$c_table_name\'||\',\'||count1||\',\'||count2);';

local $len = @table_list;
local $p_table_name = $table_list[0];
local $child_count_string ="";
for( $i=1; $i < $len;$i++)
{
  local $c_table_name = $table_list[$i];
  local $cmd = eval "qq~$child_string~";
  $child_count_string = $child_count_string.$cmd;
}
 local $command_string = eval "qq~$query_string~";
 local $output = exec_sqlcommand($PIN_LOGIN,$PIN_PASSWD,$PIN_DBNAME,$command_string);
 @output_list = split(/\n+/,$output);
}
######################################################################################################
# This routine takes the output list of find_purge_count subroutine and prints the result in a formatted
# manner.
# Subroutine Name ---- report2() 
# Input           ---- List of records containing the count of rows eleigible for purging. 
# Output          ---- formatted output of the list inputted. 
######################################################################################################
sub report2{
 print "Entering report2 subroutine\n" if $debug;
 print "Parameter passed ",@_,"\n" if $debug;
 local(@output_list)=@_;
 foreach $count_string (@output_list)
 {
  local @count_array = split(/\,/,$count_string);
  $table_name= @count_array[0];
  $tot_count=@count_array[1];
  $el_count=@count_array[2];
  if ($tot_count == 0)
  {
    $pct_count=0;
  }
  else
  {
    $pct_count=($el_count/$tot_count)*100;
  } 
  write REPORT;
  print REPORT "\n";
 }
}
######################################################################################################
# This routine takes the table list and finds out which of the tables are eleigible for moving and which
# tables can be directly archived.
# Subroutine Name ---- find_purge_tables() 
# Input           ---- List of tables 
# Output          ---- List of tables eligble for moving and list of tables that can be directly archived. 
######################################################################################################
sub find_purge_tables{
 print "Entering find_purge_tables subroutine\n" if $debug;
 print "Parameter passed ",@_,"\n" if $debug;
 local @table_list=@_; 
 $main_table_archive_mode=0;
 if ((my $len = @table_list) == 0)
  {
    return; 
  }
 @rename_array=();
 @archive_array=();
 local(@output_list)=find_purge_count(@table_list);
 local $main_table = $table_list[0];
 print $main_table,"\n" if $debug;
 foreach $count_string (@output_list)
 {
  local @count_array = split(/\,/,$count_string);
  $table_name= @count_array[0];
  $tot_count=@count_array[1];
  $el_count=@count_array[2];
  if ($tot_count == 0)
  {
    $pct_count=0;
  }
  else
  {
    $pct_count=($el_count/$tot_count)*100;
  } 
  if ($pct_count > $cutoff_for_purge)
  {
    @rename_array=(@rename_array,$table_name);
  }
  else
  {
     print $table_name,"table name\n" if $debug;
     @archive_array=(@archive_array,$table_name);
    if ($table_name eq $main_table)
    { 
      $main_table_archive_mode=1;
      print $main_table_archive_mode, "archive_mode\n" if $debug;
    }
  }
  print @archive_array,"archive tables",@rename_array,"rename tables\n" if $debug;
 }
}
######################################################################################################
# This routine creates a file with all the indexes of the table list supplied which later can be directly
# fed to the sqlplus to rebuild the indexes.
# Subroutine Name ---- get_index_map() 
# Input           ---- List of tables 
# Output          ---- List of indexes that belongs to the table list input in a file rebuild_index_script.sql. 
######################################################################################################
sub get_index_map{
  print "Entering get_index_map subroutine\n" if $debug;
  print "Parameter passed ",@_,"\n" if $debug;
  local(@table_list) =@_;
  if ((my $len = @table_list) == 0)
  {
    return; 
  }
  local $query_string='
  set heading off;
  set headsep off;
  set recsep off;
  set verify off;
  set feedback off;
  select \'ALTER INDEX \'||TABLE_OWNER||\'.\'||index_name||\' REBUILD;\'
  from user_indexes where table_name in($table_string);',"\n"; 
  local $table_string = join("'\,'",@table_list);
  $table_string =~ s/^|$/'/g;
  $table_string =~ tr/a-z/A-Z/ ;
  local $command_string = eval "qq~$query_string~";
  local $result = exec_sqlcommand($PIN_LOGIN,$PIN_PASSWD,$PIN_DBNAME,$command_string);
  open (IFILE, ">>./rebuild_index_script.sql");
  print IFILE $result;
  close IFILE;
}
######################################################################################################
# This routine executes the file which contains all the rebuild commands for the indexes 
# Subroutine Name ---- rebuild_indexes() 
# Input           ---- rebuild_index_script.sql file 
# Output          ---- rebuild of indexes 
######################################################################################################
sub rebuild_indexes{
  print "Entering rebuild_indexes subroutine\n" if $debug;
  local $cmd_file = "./rebuild_index_script.sql";
  if (! -e $cmd_file )
  {
    return;
  }
  exec_sqlcommandfile($PIN_LOGIN,$PIN_PASSWD,$PIN_DBNAME,$cmd_file);
  unlink $cmd_file;
}
######################################################################################################
# This routine executes the file which contains all the index creation commands.  
# Subroutine Name ---- create_indexes() 
# Input           ---- create_index_script.sql file 
# Output          ---- create indexes for audit tables 
######################################################################################################
sub create_indexes{
  print "Entering create_indexes subroutine\n" if $debug;
  local $cmd_file = "./create_index_script.sql";
  if (! -e $cmd_file )
  {
    return;
  }
  exec_sqlcommandfile($PIN_LOGIN,$PIN_PASSWD,$PIN_DBNAME,$cmd_file);
  #unlink $cmd_file;
}
######################################################################################################
# This routine exports the structure of the tables that needs to be renamed.
# Subroutine Name ---- export_table() 
# Input           ---- Table list that needs to be renamed. 
# Output          ---- export.dat file 
######################################################################################################
sub export_table{
  print "Entering export_table subroutine\n" if $debug;
  print "Parameter passed ",@_,"\n" if $debug;
  local(@table_list)=@_;
  local $cmd_string = "";
  local ($status) = 0;
  local ($exitcode) = 0;
  if ((my $len = @table_list) == 0)
  {
    return; 
  }
  if (!defined($export_string))
  {
     $cmd_string = 'exp $PIN_LOGIN/$PIN_PASSWD\@$PIN_DBNAME file=./export.dat tables=$table_string rows=no;\n' ;
  }
  else
  {
    $cmd_string = $export_string;
  }
  local $table_string = join("\,",@table_list);
  local $command_string= eval "qq~$cmd_string~";
  local $output = `$command_string`;
  $exit_code = $?;
  $status = ($? >> 8);
  if (($output =~ m/ERROR /i) or ($output =~ m/Errors /)) 
  {
        print("$ME: ERROR: exp command failed (exitcode = $exitcode)\n");
        print $output;
        exit(1);
  }
  if ($status || $exitcode)
  {
    print("$ME: ERROR: exp command failed with status $status\n");
    print "THIS IS EXPORT COMMAND OUTPUT\n",$output;
    exit (1);
  }
  
}
######################################################################################################
# This routine imports the structure of the tables that are renamed.
# Subroutine Name ---- import_table() 
# Input           ---- export.dat file 
# Output          ---- creates the tables that are renamed. 
######################################################################################################
sub import_table{
 print "Entering import_table subroutine\n" if $debug;
 print "Parameter passed:",@_,"\n" if $debug;
 local ($create_only_index_file)= @_;
 print $create_only_index_file;
 
  local $cmd_string = "";
  local ($status) = 0;
  local ($exitcode) = 0;

 local $cmd_file = "./export.dat";
 if (! -e $cmd_file )
 {
   return;
 }

 if ($create_only_index_file =~ /Y/i)
 {
   $cmd_string = 'imp $PIN_LOGIN/$PIN_PASSWD\@$PIN_DBNAME file=./export.dat INDEXFILE=./create_index_script.sql;\n';
 }
 else 
 {
   $cmd_string = 'imp $PIN_LOGIN/$PIN_PASSWD\@$PIN_DBNAME file=./export.dat INDEXES=N ;\n';
 }
 local $command_string= eval "qq~$cmd_string~";
 local $output = `$command_string`;
 print $output,"\n";
 $exit_code = $?;
 $status = ($? >> 8);
 if (($output =~ m/ERROR /i) or ($output =~ m/Errors /))
 {
   print("$ME: ERROR: imp command failed (exitcode = $exitcode)\n");
   print $output;
   exit(1);
 }
 if ($status || $exitcode)
 {
   print("$ME: ERROR: imp command failed with status $status\n");
   print "THIS IS IMPORT COMMAND OUTPUT\n",$output;
   exit (1);
 }
} 
######################################################################################################
# This routine renames the tables to temporary tables. 
# Subroutine Name ---- rename_tables() 
# Input           ---- Table list to be renamed. 
# Output          ---- Renaming of the tables 
######################################################################################################
sub rename_tables{
 print "Entering rename_tables subroutine\n" if $debug;
 print "Parameter passed ",@_,"\n" if $debug;
 local($opt,@table_list)=@_;
 if ((my $len = @table_list) == 0)
  {
    return; 
  }
 local $cmd_string='
 set serveroutput on;
 set heading off;
 set headsep off;
 set recsep off;
 set verify off;
 set feedback off;
 BEGIN
 $query_string1
 COMMIT;
 EXCEPTION
   WHEN OTHERS THEN
     dbms_output.put_line(\'EXCEPTION ERROR \'||SQLCODE ||SQLERRM);
     rollback ;
 END;
/'."\n";
 local $query_string =' execute immediate \'ALTER TABLE $from_table_name rename to $to_table_name\';\n';
 local $query_string1 = "";
 foreach $table_name (@table_list)
 {
   if($opt =~ /mtoh/)
   {
     $from_table_name=$table_name;
     $to_table_name = $table_name;
     $to_table_name =~ s/^au_/h_/;
   }
   elsif($opt =~ /mtot/)
   {
     $from_table_name=$table_name;
     $to_table_name = $table_name;
     $to_table_name =~ s/^au_/t_/;
   }
   elsif($opt =~ /ttoh/)
   {
     $from_table_name=$table_name;
     $to_table_name = $table_name;
     $from_table_name =~ s/^au_/t_/;
     $to_table_name =~ s/^au_/h_/;
   }
   $query_string1 = $query_string1.eval "qq~$query_string~";
 }
 local $command_string= eval "qq~$cmd_string~";
 local $result= exec_sqlcommand($PIN_LOGIN,$PIN_PASSWD,$PIN_DBNAME,$command_string);
 print $result,"\n" if $debug;
} 
######################################################################################################
# This routine drops the temporary tables. 
# Subroutine Name ---- drop_tables() 
# Input           ---- Table list to be dropped. 
# Output          ---- Dropping of the tables 
######################################################################################################
sub drop_tables{
 print "Entering drop_tables subroutine\n" if $debug;
 print "Parameter passed ",@_,"\n" if $debug;
 local(@table_list)=@_;
 if ((my $len = @table_list) == 0)
  {
    return; 
  }
 local $cmd_string='
 set heading off;
 set headsep off;
 set recsep off;
 set verify off;
 set feedback off;
 $query_string1'."\n";
 local $query_string =' DROP TABLE $temp_table_name;\n';
 local $query_string1 = "";
 foreach $table_name (@table_list)
 {
   $temp_table_name = $table_name;
   $temp_table_name =~ s/^au_/t_/;
   $query_string1 = $query_string1.eval "qq~$query_string~";
 }
 local $command_string= eval "qq~$cmd_string~";
 local $result = exec_sqlcommand($PIN_LOGIN,$PIN_PASSWD,$PIN_DBNAME,$command_string);
 print $result,"\n" if $debug;
}
##########################################################################
sub get_index{
  print "Entering get_index subroutine\n" if $debug;
  print "Parameter passed ",@_,"\n" if $debug;
  local(@table_list) =@_;
  if ((my $len = @table_list) == 0)
  {
    return;
  }
  local $query_string='
  set heading off;
  set headsep off;
  set recsep off;
  set verify off;
  set feedback off;
  select TABLE_OWNER||\'.\'||index_name
  from user_indexes where table_name in($table_string);',"\n";
  local $table_string = join("'\,'",@table_list);
  $table_string =~ s/^|$/'/g;
  $table_string =~ tr/a-z/A-Z/ ;
  local $command_string = eval "qq~$query_string~";
  local $result = exec_sqlcommand($PIN_LOGIN,$PIN_PASSWD,$PIN_DBNAME,$command_string);
  $result =~ s/^\n|^$//;
  local @index_list = split(/\n+/,$result);
  create_index_file(@index_list);
}
sub create_index_file{
  print "Entering create_index_file subroutine\n" if $debug;
  print "Parameter passed ",@_,"\n" if $debug;
  local(@index_list) =@_;
  if ((my $len = @index_list) == 0)
  {
    return;
  }
  open (IFILE1,">./rebuild_index_script.sql");
  open (IFILE2,">./rename_index_script.sql");
  open (IFILE3,">./drop_index_script.sql");
  foreach $index_name(@index_list)
  {
    $new_index_name = $index_name;
    $new_index_name =~ s/\s+//g;
    $new_index_name =~ s/\.I_/\.H_/;
    $put_string1 = 'ALTER INDEX '.$index_name.' REBUILD;'."\n";
    $put_string2 = 'DROP INDEX '.$index_name.";\n";
    $put_string3 = 'ALTER INDEX '.$index_name.' RENAME TO '.$new_index_name.";\n";
    print IFILE1 $put_string1;
    print IFILE3 $put_string2;
    print IFILE2 $put_string3;
  }
  close IFILE1;
  close IFILE2;
  close IFILE3;
}
sub timelocal {
   return mktime (@_, undef, undef, -1);
}
