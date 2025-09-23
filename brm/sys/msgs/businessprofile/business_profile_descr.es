##########################################################################
#
# Copyright (c) 2000, 2009, Oracle and/or its affiliates. All rights reserved. 
# This material is the confidential property of Oracle Corporation
# or its licensors and may be used, reproduced, stored
# or transmitted only in accordance with a valid Oracle license or
# sublicense agreement.
#
##########################################################################
#
#   File:  business_profile_descr.es
#
#   Description:
#
#   This file contains localized string object definitions.
#
#   Locale:  es ( Español )
#
#   Domain:  Business Profile Descriptions (version = 1)
#
#   Rules:
#
#   [1] Uniqueness:
#
#       The combination of locale, domain, string ID, and string
#       version must uniquely define a string object within the
#       universe of all string objects.
#       
#       NOTE:
#       =====
#	This uniqueness must be ensured while creating or updating
#	this Business Profile Descriptions file. If there are  
#	duplicate entries, the programs behaviour depends on the
#	option used. If the -f option is used then the last
#	duplicate string definition will overwrite any previous
#	ones. If the -f option is not used then, duplicate string
#	definitions will be rejected.
#
#   [2] Locale
#
#       Only one locale may be specified in this file.  The locale
#       definition must be the first non-comment statement.
#
#   [3] Domain
#
#       There may be multiple domains specified in this file.  The
#       domain will apply to all string definitions that follow
#       until the next domain definition statement appears.
#
#   [4] String Format:
#
#       Within this file all strings must be delimited by an opening
#       and closing double-quote character.  The quotes will not be
#       part of the string stored in the database.  A double-quote
#       character can be an element of the string if it is escaped
#       with a backslash, for example "Delete \"unneeded\" files."
#       will be stored as "Delete "unneeded" files.".
#
#       Substitution parameters can be specified with %i, where i is
#       an integer from 0 to 99.  The percent character can be an
#       element of the string if it is escaped with a backslash, for
#       example "It is 100\% good.".  Here is an example of an error
#       string that specifies one substitution parameter:
#
#               File %0 not found.
#
#       If the substitution string is "pin.conf" the formatted
#       string will be "File pin.conf not found.".
#
#       The STRING and optional HELPSTR are localizable.  This file must
#       be in UTF-8 encoding. The LOCALE and DOMAIN strings are assumed
#       to be ASCII (no extended-ASCII characters nor multiple byte
#       characters are allowed).
#
#   [5] String ID:
#
#       A string ID must be unique within a domain.
#       
#       NOTES:
#       =====
#       This uniqueness must be ensured while creating or updating
#       this Business Profile Descriptions file. Please see the note
#	about uniqueness above.
#
#   [6] String Version:
#
#       Each string has an individual version, typically starting
#       from 1, that the string owner assigns.
#
##########################################################################

LOCALE = "es" ;

DOMAIN = "Business Profiles" ;
STR
	ID = 1 ;
	VERSION = 1 ;
	STRING = "Convergente" ;
END
STR
	ID = 2 ;
	VERSION = 1 ;
	STRING = "Descripción de perfil de negocio convergente" ;
END
STR
	ID = 3 ;
	VERSION = 1 ;
	STRING = "Prepagado" ;
END
STR
	ID = 4 ;
	VERSION = 1 ;
	STRING = "Descripción de perfil de negocio de prepago" ;
END
STR
	ID = 5 ;
	VERSION = 1 ;
	STRING = "Pospago" ;
END
STR
	ID = 6 ;
	VERSION = 1 ;
	STRING = "Descripción de perfil de negocio de pospago" ;
END
STR
	ID = 7 ;
	VERSION = 1 ;
	STRING = "No consumo" ;
END
STR
        ID = 8 ;
        VERSION = 1 ;
        STRING = "DescripciC3n de perfil de negocio de no consumo" ;
END




