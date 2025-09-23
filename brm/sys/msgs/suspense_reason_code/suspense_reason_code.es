##########################################################################
#
# Copyright (c) 2003, 2009, Oracle and/or its affiliates. All rights reserved. 
#     This material is the confidential property of Oracle Corporation 
#     or its  licensors and may be used, reproduced, stored
#     or transmitted only in accordance with a valid Oracle license or 
#     sublicense agreement.
#
##########################################################################
#
#   File:  suspense_reason_code.ES
#
#   Description:
#
#	This file contains localized string object definitions.
#
#	Locale:  ES ( Español )
#
#	Domain:  config_suspense_reason_code (version = 1)
#
#   Rules:
#
#	[1] Uniqueness:
#
#	    The combination of locale, domain, string ID, and string
#	    version must uniquely define a string object within the
#	    universe of all string objects.
#	    
#	    NOTE:
#	    =====
#	    This uniqueness must be ensured while creating or updating
#	    this Suspense Reason Code file. If there are duplicate entries,
#	    the programs behaviour depends on the option used. If the
#	    -f option is used then the last duplicate string definition
#	    will overwrite any previous ones. If the -f option is not
#	    used then, duplicate string definitions will be rejected.
#
#	[2] Locale
#
#	    Only one locale may be specified in this file.  The locale
#	    definition must be the first non-comment statement.
#
#	[3] Domain
#
#	    There may be multiple domains specified in this file.  The
#	    domain will apply to all string definitions that follow
#	    until the next domain definition statement appears.
#
#	[4] String Format:
#
#	    Within this file all strings must be delimited by an opening
#	    and closing double-quote character.  The quotes will not be
#	    part of the string stored in the database.  A double-quote
#	    character can be an element of the string if it is escaped
#	    with a backslash, for example "Delete \"unneeded\" files."
#	    will be stored as "Delete "unneeded" files.".
#
#	    Substitution parameters can be specified with %i, where i is
#	    an integer from 0 to 99.  The percent character can be an
#	    element of the string if it is escaped with a backslash, for
#	    example "It is 100\% good.".  Here is an example of an error
#	    string that specifies one substitution parameter:
#
#				File %0 not found.
#
#	    If the substitution string is "pin.conf" the formatted
#	    string will be "File pin.conf not found.".
#
#         The STRING and optional HELPSTR are localizable.  This file must
#	    be in UTF-8 encoding. The LOCALE and DOMAIN strings are assumed
#         to be ASCII (no extended-ASCII characters nor multiple byte
#         characters are allowed).
#
#	[5] String ID:
#
#	    A string ID must be unique within a domain.
#	    
#	    NOTES:
#	    =====
#	    This uniqueness must be ensured while creating or updating
#	    this Suspense Reason Code file. Please see the note about 
#	    uniqueness above.
#
#	    The id's and version's chosen in this file should be kept in
#	    sync with the pin_suspense_reason_code configuration file.
#
#	[6] String Version:
#
#	    Each string has an individual version, typically starting
#	    from 1, that the string owner assigns.
#
##########################################################################

LOCALE = "es" ;

DOMAIN = "suspense_reason" ;
STR
	ID = 0 ;
	VERSION = 1 ;
	STRING = "Otro error" ;
END
STR
	ID = 1 ;
	VERSION = 1 ;
	STRING = "Error de procesamiento del motor de tarificación por lote" ;
END
STR
	ID = 2 ;
	VERSION = 1 ;
	STRING = "Error de procesamiento de descuento" ;
END
STR
	ID = 3 ;
	VERSION = 1 ;
	STRING = "Error de tarificación" ;
END
STR
	ID = 4 ;
	VERSION = 1 ;
	STRING = "Error en la conexión de llamada" ;
END
STR
	ID = 5 ;
	VERSION = 1 ;
	STRING = "Problema de asignación" ;
END
STR
	ID = 6 ;
	VERSION = 1 ;
	STRING = "Error de registro de facturación" ;
END
STR
	ID = 7 ;
	VERSION = 1 ;
	STRING = "Error de división" ;
END
STR
	ID = 8 ;
	VERSION = 1 ;
	STRING = "Error de agregación de registro" ;
END
STR
	ID = 9 ;
	VERSION = 1 ;
	STRING = "Error del cliente" ;
END
STR
	ID = 10 ;
	VERSION = 1 ;
	STRING = "Error de registros de itinerancia" ;
END
STR
        ID = 11 ;
        VERSION = 1 ;
        STRING = "Suspensión interna" ;
END
STR
        ID = 12 ;
        VERSION = 1 ;
        STRING = "Error de conjunto de filtros" ;
END
STR
	ID = 65534 ;
	VERSION = 1 ;
	STRING = "Sin reciclado de prueba" ;
END
STR
	ID = 65535 ;
	VERSION = 1 ;
	STRING = "No está suspendido" ;
END


DOMAIN = "suspense_subreason_0" ;
STR
	ID = 0 ;
	VERSION = 1 ;
	STRING = "Otro error" ;
END

DOMAIN = "suspense_subreason_1" ;
STR
	ID = 0 ;
	VERSION = 1 ;
	STRING = "Error de procesamiento del motor de tarificación por lote" ;
END
STR
	ID = 1 ;
	VERSION = 1 ;
	STRING = "Error de asignación de registro" ;
END

STR
	ID = 2 ;
	VERSION = 1 ;
	STRING = "Error en la base de datos" ;
END

STR
	ID = 3 ;
	VERSION = 1 ;
	STRING = "No hay suficiente memoria disponible" ;

END

DOMAIN = "suspense_subreason_2" ;
STR
	ID = 0 ;
	VERSION = 1 ;
	STRING = "Error de procesamiento de descuento" ;
END

DOMAIN = "suspense_subreason_3" ;
STR
	ID = 0 ;
	VERSION = 1 ;
	STRING = "Plan de tarifas no válido" ;
END
STR
	ID = 1 ;
	VERSION = 1 ;
	STRING = "Modelo de precios no válido" ;
END
STR
	ID = 2 ;
	VERSION = 1 ;
	STRING = "Modelo horario o zona horaria no válidos" ;
END
STR
	ID = 3 ;
	VERSION = 1 ;
	STRING = "Otro error de tarificación principal" ;
END
STR
	ID = 4 ;
	VERSION = 1 ;
	STRING = "Error de tarificación del cliente" ;
END
STR
	ID = 5 ;
	VERSION = 1 ;
	STRING = "Error en la tarifa de cambio" ;
END


DOMAIN = "suspense_subreason_4" ;
STR
	ID = 0 ;
	VERSION = 1 ;
	STRING = "Error en la conexión de llamada" ;
END

DOMAIN = "suspense_subreason_5" ;
STR
	ID = 0 ;
	VERSION = 1 ;
	STRING = "Error en la actividad comercial del mapa PLMN" ;
END

DOMAIN = "suspense_subreason_6" ;
STR
	ID = 0 ;
	VERSION = 1 ;
	STRING = "Fallo al añadir información sobre el saldo de descuento" ;
END

DOMAIN = "suspense_subreason_7" ;
STR
	ID = 0 ;
	VERSION = 1 ;
	STRING = "Error de división" ;
END

DOMAIN = "suspense_subreason_8" ;
STR
	ID = 0 ;
	VERSION = 1 ;
	STRING = "Error de agregación" ;
END
DOMAIN = "suspense_subreason_9" ;
STR
	ID = 0 ;
	VERSION = 1 ;
	STRING = "Error del cliente" ;
END
STR
	ID = 1 ;
	VERSION = 1 ;
	STRING = "Error interno" ;
END


DOMAIN = "suspense_subreason_10" ;
STR
	ID = 0 ;
	VERSION = 1 ;
	STRING = "Error de registro CIBER" ;
END
STR
	ID = 1 ;
	VERSION = 1 ;
	STRING = "Error de registro TAP3" ;
END

DOMAIN = "suspense_subreason_11" ;
STR
        ID = 0 ;
        VERSION = 1 ;
        STRING = "Se ha migrado la cuenta" ;
END
STR
        ID = 1 ;
        VERSION = 1 ;
        STRING = "Facturación de cuenta en espera" ;
END
STR
        ID = 2 ;
        VERSION = 1 ;
        STRING = "El uso de la cuenta se ha vuelto a tarificar" ;
END

DOMAIN = "suspense_subreason_12" ;
STR
        ID = 0 ;
        VERSION = 1 ;
        STRING = "ID de descuento no válida" ;
END
STR
        ID = 1 ;
        VERSION = 1 ;
        STRING = "Id de producto no válida" ;
END

DOMAIN = "suspense_subreason_65534" ;
STR
	ID = 65534 ;
	VERSION = 1 ;
	STRING = "Sin reciclado de prueba" ;
END

DOMAIN = "suspense_subreason_65535" ;
STR
	ID = 65535 ;
	VERSION = 1 ;
	STRING = "No está suspendido" ;
END
