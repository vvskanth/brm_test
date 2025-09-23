##########################################################################
#
# Copyright (c) 2005, 2009, Oracle and/or its affiliates. All rights reserved. 
#     This material is the confidential property of Oracle Corporation 
#     or its  licensors and may be used, reproduced, stored
#     or transmitted only in accordance with a valid Oracle license or 
#     sublicense agreement.
#
##########################################################################
#
#   File:  license_error.es
#
#   Description:
#
#   This file contains localized string object definitions.
#
#   Locale: es ( Español )
#
#   Domain:  Feature Control Errors (version = 1)
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
#       This uniqueness must be ensured while creating or updating
#       this License Errors file. If there are duplicate entries,
#       the programs behaviour depends on the option used. If the
#       -f option is used then the last duplicate string definition
#       will overwrite any previous ones. If the -f option is not
#       used then, duplicate string definitions will be rejected.
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
#       this License Errors file. Please see the note about 
#       uniqueness above.
#
#       The id's chosen in this file SHOULD be kept in sync with
#       the file pin_license_map.c(feature_name-msg_id mapping file).
#
#   [6] String Version:
#
#       Each string has an individual version, typically starting
#       from 1, that the string owner assigns.
#
##########################################################################

LOCALE = "es" ;

DOMAIN = "Feature Control Errors" ;
STR
    ID = 1 ;
    VERSION = 1 ;
    STRING = "Error Esta función \"Facturas múltiples\" está actualmente desactivada y necesita una licencia adicional. Para mayor información, por favor, consulte la documentación o contacte con su distribuidor local." ;
END
STR
    ID = 2 ;
    VERSION = 1 ;
    STRING = "Error Esta función \Pagos múltiples\ está actualmente desactivada y necesita una licencia adicional. Para mayor información, por favor, consulte la documentación o contacte con su distribuidor local." ;
END
STR
    ID = 3 ;
    VERSION = 1 ;
    STRING = "Error Esta función \"Saldos múltiples\" está actualmente desactivada y necesita una licencia adicional. Para mayor información, por favor, consulte la documentación o contacte con su distribuidor local." ;
END
STR
    ID = 4 ;
    VERSION = 1 ;
    STRING = "Error Esta función \"Grupos de suscripción\" está actualmente desactivada y necesita una licencia adicional. Para mayor información, por favor, consulte la documentación o contacte con su distribuidor local." ;
END
STR
    ID = 5 ;
    VERSION = 1 ;
    STRING = "Error Esta función \"Grupos de suscriptor\" está actualmente desactivada y necesita una licencia adicional. Para mayor información, por favor, consulte la documentación o contacte con su distribuidor local." ;
END
STR
    ID = 6 ;
    VERSION = 1 ;
    STRING = "Error Esta función \"Superiores\" está actualmente desactivada y necesita una licencia adicional. Para mayor información, por favor, consulte la documentación o contacte con su distribuidor local." ;
END
STR
    ID = 7 ;
    VERSION = 1 ;
    STRING = "Error Esta función \"Superiores patrocinados\" está actualmente desactivada y necesita una licencia adicional. Para mayor información, por favor, consulte la documentación o contacte con su distribuidor local." ;
END
STR
    ID = 8 ;
    VERSION = 1 ;
    STRING = "Error Esta función \"Descuentos en tiempo real\" está actualmente desactivada y necesita una licencia adicional. Para mayor información, por favor, consulte la documentación o contacte con su distribuidor local." ;
END
STR
    ID = 9 ;
    VERSION = 1 ;
    STRING = "Error Esta función \"Descuentos por lotes\" está actualmente desactivada y necesita una licencia adicional. Para mayor información, por favor, consulte la documentación o contacte con su distribuidor local." ;
END
STR
    ID = 10 ;
    VERSION = 1 ;
    STRING = "Error Esta función \"Descuentos al facturar\" está actualmente desactivada y necesita una licencia adicional. Para mayor información, por favor, consulte la documentación o contacte con su distribuidor local." ;
END
STR
    ID = 11 ;
    VERSION = 1 ;
    STRING = "Error Esta función \"Jerarquía de cuentas\" está actualmente desactivada y necesita una licencia adicional. Para mayor información, por favor, consulte la documentación o contacte con su distribuidor local." ;
END
STR
    ID = 12 ;
    VERSION = 1 ;
    STRING = "Error Esta función \"Patrocinador\" está actualmente desactivada y necesita una licencia adicional. Para mayor información, por favor, consulte la documentación o contacte con su distribuidor local." ;
END
STR
    ID = 13 ;
    VERSION = 1 ;
    STRING = "Error Esta función \"Dependencias del producto\" está actualmente desactivada y necesita una licencia adicional. Para mayor información, por favor, consulte la documentación o contacte con su distribuidor local." ;
END
STR
    ID = 14 ;
    VERSION = 1 ;
    STRING = "Error. Esta función \Control de saldos\ está actualmente desactivada y necesita una licencia adicional. Para mayor información, consulte la documentación o póngase en contacto con su distribuidor local." ;
END
STR
    ID = 15 ;
    VERSION = 1 ;
    STRING = "Error. Esta función \Configuración de control automático\ está actualmente desactivada y necesita una licencia adicional. Para mayor información, consulte la documentación o póngase en contacto con su distribuidor local." ;
END
STR
    ID = 17 ;
    VERSION = 1 ;
    STRING = "Error Esta función \"Entorno de telecomunicaciones\" está actualmente desactivada y necesita una licencia adicional. Para mayor información, por favor, consulte la documentación o contacte con su distribuidor local." ;
END
STR
    ID = 18 ;
    VERSION = 1 ;
    STRING = "Error Esta función \"Transacciones C/P\" está actualmente desactivada y necesita una licencia adicional. Para mayor información, por favor, consulte la documentación o contacte con su distribuidor local." ;
END
STR
    ID = 19 ;
    VERSION = 1 ;
    STRING = "Error Esta función \"Informe del libro general de contabilidad \" está actualmente desactivada y necesita una licencia adicional. Para mayor información, por favor, consulte la documentación o contacte con su distribuidor local." ;
END
STR
    ID = 20 ;
    VERSION = 1 ;
    STRING = "Error Esta función \"Administrador del conjunto de cambios\" está actualmente desactivada y necesita una licencia adicional. Para mayor información, por favor, consulte la documentación o contacte con su distribuidor local." ;
END
STR
    ID = 21 ;
    VERSION = 1 ;
    STRING = "Error Esta función \"Administrador HA\" está actualmente desactivada y necesita una licencia adicional. Para mayor información, por favor, consulte la documentación o contacte con su distribuidor local." ;
END
STR
    ID = 22 ;
    VERSION = 1 ;
    STRING = "Error Esta función \"Administrador de suspensión de pagos\" está actualmente desactivada y necesita una licencia adicional. Para mayor información, por favor, consulte la documentación o contacte con su distribuidor local." ;
END
STR
    ID = 23 ;
    VERSION = 1 ;
    STRING = "Error Esta función \"Descuentos basados en volumen\" está actualmente desactivada y necesita una licencia adicional. Para mayor información, por favor, consulte la documentación o contacte con su distribuidor local." ;
END
STR
    ID = 24 ;
    VERSION = 1 ;
    STRING = "Error Esta función \"Mejores precios\" está actualmente desactivada y necesita una licencia adicional. Para mayor información, por favor, consulte la documentación o contacte con su distribuidor local." ;
END
