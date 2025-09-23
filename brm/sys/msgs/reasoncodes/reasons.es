###########################################################################
#
# Copyright (c) 2000, 2015, Oracle and/or its affiliates. All rights reserved.
# This material is the confidential property of Oracle Corporation
# or its licensors and may be used, reproduced, stored
# or transmitted only in accordance with a valid Oracle license or
# sublicense agreement.
#
###########################################################################
#
#   File:  reasons.es
#
#   Description:
#
#	This file contains localized string object definitions.
#
#	Locale:  es ( Spanish )
#
#	Domain:  Reason Codes-Active Status Reasons (version = 2)
#		 Reason Codes-Charge Reasons  (version = 5)
#		 Reason Codes-Closed Status Reasons (version = 4)
#		 Reason Codes-Credit Reasons (version = 8)
#		 Reason Codes-Credit Limit Reasons (version = 7)
#		 Reason Codes-Debit Reasons (version = 1)
#		 Reason Codes-Inactive Status Reasons (version = 3)
#		 Reason Codes-Refund Reasons (version = 6)
#		 
#		 Reason Domains for Payment Processing
#		 reserved from 13 - 20
#
#		 Reason Codes-Failed Payment Reasons (version = 13)
#		 Reason Codes-Suspense Payment Reasons (version = 14)
#		 Reason Codes-Payment Suspense Management Action Owner (version = 15)
#		 Reason Codes-Payment Suspense Management, Reversal Reasons (version = 16)
#
#		 Reason Domains for Adjustments, Disputes and settlements
#		 reserved from 21 - 50
#
#		 Reason Codes-Credit Reasons Non-Currency Account
#			      Adjustments(version = 21)
#		 Reason Codes-Debit Reasons Non-Currency Account
#			      Adjustments(version = 22)
#
#		 Reason Codes-Credit Reasons Bill Adjustments (version = 23)
#		 Reason Codes-Debit Reasons Bill Adjustments (version = 24)
#		 Reason Codes-Credit Reasons Bill Disputes (version = 25)
#		 Reason Codes-Debit Reasons Bill Disputes (version = 26)
#		 Reason Codes-Reasons Bill Settlements (version = 27)
#
#		 Reason Codes-Credit Reasons Currency Connection
#			      Adjustments (version = 28)
#		 Reason Codes-Debit Reasons Currency Connection
#			      Adjustments (version = 29)
#		 Reason Codes-Credit Reasons Non-Currency Connection
#			      Disputes (version = 30)
#		 Reason Codes-Debit Reasons Non-Currency Connection
#			      Disputes (version = 31)
#
#		 Reason Codes-Credit Reasons Item Adjustments (version = 32)
#		 Reason Codes-Debit Reasons Item Adjustments (version = 33)
#		 Reason Codes-Credit Reasons Item Disputes (version = 34)
#		 Reason Codes-Debit Reasons Item Disputes (version = 35)
#		 Reason Codes-Reasons Item Settlements (version = 36)
#		 
#		 Reason Codes-Credit Reasons Event Adjustments (version = 37)
#		 Reason Codes-Debit Reasons Event Adjustments (version = 38)
#		 Reason Codes-Credit Reasons Event Disputes (version = 39)
#		 Reason Codes-Debit Reasons Event Disputes (version = 40)
#		 Reason Codes-Reasons Event Settlements (version = 41)
#		 Reason Codes-Reasons Write-off level (version = 42)
#
#		 Reason Codes-Others (version = 100)
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
#	    this Reason Codes file. If there are duplicate entries,
#	    the programs behaviour depends on the option used. If the
#	    -f option is used then the last duplicate string definition
#	    will overwrite any previous ones. If the -f option is not
#	    used then, duplicate string definitions will be rejected,
#	    except for the Event-GLID mappings which are always 
#	    overwritten.
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
#           The STRING and optional HELPSTR are localizable.  This file must be
#           in UTF-8 encoding. The LOCALE and DOMAIN strings are assumed
#           to be ASCII (no extended-ASCII characters nor multiple byte
#           characters are allowed).
#
#	[5] String ID:
#
#	    A string ID must be unique within a domain.
#	    
#	    NOTE:
#	    =====
#	    This uniqueness must be ensured while creating or updating
#	    this Reason Codes file. Please see the note about 
#	    uniqueness above.
#
#	[6] String Version:
#
#	    For reason codes, this field will be used to specify the
#	    domain that the reason belongs to, for example, Credit or 
#           Debit. The values have been predefined; they must not be 
#	    changed.
#
#	[7] Event to G/L ID mapping:
#
#	    These mappings are specified in this file. 
#
#	    G/L IDs can now be assigned based on Reason Codes. This is
#	    currently applicable for Account level adjustments only.
#
#	    Events associated with specific Debit or Credit reasons are 
#	    listed under the corresponding reason code and they start
#	    with the header "EVENT-GLID" and end with "EVENT-GLID-END".
#	
#	    In the earlier releases of Infranet, G/L ID mappings
#	    for pre-rated events could be specified using the 
#	    Policy Configuration tool. Now these mappings must
#	    be specified in this file. A new domain named "Others"
#	    has been created for providing the G/L ID mappings
#	    for these events. 
#   
#	    All the mappings shown are examples and need to be
#	    changed appropriately during setup.
#
##########################################################################

LOCALE = "es" ;

DOMAIN = "Reason Codes-Active Status Reasons" ;
STR
	ID = 1 ;
	VERSION = 2 ;
	STRING = "Reactivación solicitada por el cliente" ;
END
STR
	ID = 2 ;
	VERSION = 2 ;
	STRING = "El cliente pagó los cargos vencidos" ;
END
STR
	ID = 3 ;
	VERSION = 2 ;
	STRING = "Tarjeta de crédito en funcionamiento" ;
END
STR
	ID = 4 ;
	VERSION = 2 ;
	STRING = "Otro problema resuelto" ;
END

DOMAIN = "Reason Codes-Charge Reasons" ;
STR
	ID = 11 ;
	VERSION = 5 ;
	STRING = "Cuenta reactivada" ;
END
STR
	ID = 12 ;
	VERSION = 5 ;
	STRING = "Tarjeta de crédito validada" ;
END
STR
	ID = 13 ;
	VERSION = 5 ;
	STRING = "Prepago solicitado por el cliente" ;
END
STR
	ID = 14 ;
	VERSION = 5 ;
	STRING = "Tarjeta de crédito nueva" ;
END

DOMAIN = "Reason Codes-Closed Status Reasons" ;
STR
	ID = 1 ;
	VERSION = 4 ;
	STRING = "Cuenta duplicada" ;
END
STR
	ID = 2 ;
	VERSION = 4 ;
	STRING = "Traslado de zona" ;
END
STR
	ID = 3 ;
	VERSION = 4 ;
	STRING = "No satisfecho con el servicio" ;
END
STR
	ID = 4 ;
	VERSION = 4 ;
	STRING = "Política de empresa infringida" ;
END

DOMAIN = "Reason Codes-Credit Reasons" ;
STR
	ID = 1 ;
	VERSION = 8 ;
	STRING = "Cliente insatisfecho con el servicio" ;
	EVENT-GLID
		"/event/billing/adjustment/account"		105 ;
	EVENT-GLID-END
END
STR
	ID = 2 ;
	VERSION = 8 ;
	STRING = "Cliente no informado de los cargos" ;
	EVENT-GLID
		"/event/billing/adjustment/account"		105 ;
	EVENT-GLID-END
END
STR
	ID = 3 ;
	VERSION = 8 ;
	STRING = "Cargado en esta cuenta por error" ;
	EVENT-GLID
		"/event/billing/adjustment/account"		105 ;
	EVENT-GLID-END
END
STR
	ID = 4 ;
	VERSION = 8 ;
	STRING = "Cancelación por función reversible de cancelación automática" ;
	EVENT-GLID
		"/event/billing/writeoff"			110 ;
	EVENT-GLID-END
END
STR
	ID = 5 ;
	VERSION = 8 ;
	STRING = "Superior patrocinado. Crédito patrocinado" ;
	EVENT-GLID
		"/event/billing/adjustment/account"		105 ;
	EVENT-GLID-END
END


DOMAIN = "Reason Codes-Debit Reasons" ;
STR
	ID = 1 ;
	VERSION = 1 ;
	STRING = "Cargos técnicos y de soporte" ;
	EVENT-GLID
		"/event/billing/adjustment/account"		105 ;
	EVENT-GLID-END
END
STR
	ID = 2 ;
	VERSION = 1 ;
	STRING = "Cargos de servicios" ;
	EVENT-GLID
		"/event/billing/adjustment/account"		105 ;
	EVENT-GLID-END
END
STR
	ID = 3 ;
	VERSION = 1 ;
	STRING = "Cuenta acreditada por error" ;
	EVENT-GLID
		"/event/billing/adjustment/account"		105 ;
	EVENT-GLID-END
END
STR
	ID = 4 ;
	VERSION = 1 ;
	STRING = "Superior patrocinado. Débito del patrocinador" ;
	EVENT-GLID
		"/event/billing/adjustment/account"		105 ;
	EVENT-GLID-END
END


DOMAIN = "Reason Codes-Credit Limit Reasons" ;
STR
	ID = 1 ;
	VERSION = 7 ;
	STRING = "Solicitado por el cliente" ;
END
STR
	ID = 2 ;
	VERSION = 7 ;
	STRING = "Crédito válido, límite excedido con frecuencia" ;
END
STR
	ID = 3 ;
	VERSION = 7 ;
	STRING = "Requerido por el aumento de los cargos de los servicios" ;
END
STR
	ID = 4 ;
	VERSION = 7 ;
	STRING = "Circunstancias especiales" ;
END

DOMAIN = "Reason Codes-Inactive Status Reasons" ;
STR
	ID = 1 ;
	VERSION = 3 ;
	STRING = "Desactivación solicitada por el cliente" ;
END
STR
	ID = 2 ;
	VERSION = 3 ;
	STRING = "Límite de crédito/problemas con tarjeta de crédito" ;
END
STR
	ID = 3 ;
	VERSION = 3 ;
	STRING = "Factura no pagada" ;
END
STR
	ID = 4 ;
	VERSION = 3 ;
	STRING = "Se sospecha conducta incorrecta por parte del cliente" ;
END

DOMAIN = "Reason Codes-Refund Reasons" ;
STR
	ID = 1 ;
	VERSION = 6 ;
	STRING = "Cliente insatisfecho con el servicio" ;
END
STR
	ID = 2 ;
	VERSION = 6 ;
	STRING = "Cliente no informado de los cargos" ;
END
STR
	ID = 3 ;
	VERSION = 6 ;
	STRING = "Apertura de cuenta no reconocida por el cliente" ;
END
STR
	ID = 4 ;
	VERSION = 6 ;
	STRING = "Cuenta cargada por error" ;
END

DOMAIN = "Reason Codes-Insert Action" ;
STR
        ID = 1 ;
        VERSION = 10 ;
        STRING = "El cliente se comprometió a pagar en una fecha determinada";
END
STR
        ID = 2 ;
        VERSION = 10 ;
        STRING = "No se ha podido contactar con el cliente";
END
STR
        ID = 3 ;
        VERSION = 10 ;
        STRING = "El cliente no ha recibido la solicitud de pago original";
END
STR
        ID = 4 ;
        VERSION = 10 ;
        STRING = "El cliente no devolvió la llamada como se le solicitó";
END

DOMAIN = "Reason Codes-Reschedule Action" ;
STR
        ID = 1 ;
        VERSION = 9 ;
        STRING = "No se ha podido contactar con el cliente";
END
STR
        ID = 2 ;
        VERSION = 9 ;
        STRING = "El cliente se comprometió a pagar en una fecha determinada";
END
STR
        ID = 3 ;
        VERSION = 9 ;
        STRING = "El cliente no ha recibido la solicitud de pago original";
END
STR
        ID = 4 ;
        VERSION = 9 ;
        STRING = "El cliente no devolvió la llamada como se le solicitó";
END

DOMAIN = "Reason Codes-Exempt Account" ;
STR
        ID = 1 ;
        VERSION = 11 ;
        STRING = "Solicitada declaración de bancarrota";
END
STR
        ID = 2 ;
        VERSION = 11 ;
        STRING = "Consulta legal pendiente";
END
STR
        ID = 3 ;
        VERSION = 11 ;
        STRING = "Juicio pendiente" ;
END
STR
        ID = 4 ;
        VERSION = 11 ;
        STRING = "Cuenta estratégica en términos de Contrato/Pago especiales";
END

DOMAIN = "Reason codes-Payment Failure Reasons" ;
STR
	ID = 1001 ;
	VERSION = 13 ;
	STRING = "Tarjeta no válida";
END
STR
	ID = 1002 ;
	VERSION = 13 ;
	STRING = "Cantidad no válida";
END
STR
	ID = 1003 ;
	VERSION = 13 ;
	STRING = "Error desconocido";
END
STR
	ID = 1004 ;
	VERSION = 13 ;
	STRING = "Error de conexión";
END
STR
	ID = 1005 ;
	VERSION = 13 ;
	STRING = "Fondos insuficientes / límite de crédito superado";
END
STR
	ID = 1006 ;
	VERSION = 13 ;
	STRING = "Tarjeta rechazada";
END
STR
	ID = 1007 ;
	VERSION = 13 ;
	STRING = "El emisor desea contacto telefónico con el titular";
END
STR
	ID = 1008 ;
	VERSION = 13 ;
	STRING = "Aprobar/Declinar";
END
STR
	ID = 1009 ;
	VERSION = 13 ;
	STRING = "El emisor de la tarjeta quiere que se le devuelva la tarjeta";
END
STR
	ID = 1010 ;
	VERSION = 13 ;
	STRING = "Tarjeta denunciada como perdida o robada";
END
STR
	ID = 1011 ;
	VERSION = 13 ;
	STRING = "La tarjeta ha caducado";
END
STR
	ID = 1012 ;
	VERSION = 13 ;
	STRING = "Declinación genérica: no hay más información proporcionada por el emisor";
END
STR
	ID = 1013 ;
	VERSION = 13 ;
	STRING = "La tarjeta no es válida, pero supera la rutina de Mod de 10 dígitos ";
END
STR
	ID = 1014 ;
	VERSION = 13 ;
	STRING = "La tarjeta ha caducado o la fecha es incorrecta. Confirmar que la fecha es correcta";
END
STR
	ID = 1015 ;
	VERSION = 13 ;
	STRING = "El emisor no permite este tipo de transacción";
END
STR
	ID = 1016 ;
	VERSION = 13 ;
	STRING = "La red no permite esta cantidad";
END
STR
	ID = 1017 ;
	VERSION = 13 ;
	STRING = "La tarjeta está restringida";
END
STR
	ID = 1018 ;
	VERSION = 13 ;
	STRING = "Paymentech recibido no responde a la red aut";
END
STR
	ID = 1019 ;
	VERSION = 13 ;
	STRING = "Editar error: los datos AVS no son válidos";
END
STR
	ID = 1020 ;
	VERSION = 13 ;
	STRING = "Ubicación especificada no válida";
END
STR
	ID = 1021 ;
	VERSION = 13 ;
	STRING = "Código postal especificado no válido";
END
STR
	ID = 1022 ;
	VERSION = 13 ;
	STRING = "La tarjeta no es válida, pero supera la rutina de Mod de 10 dígitos ";
END
STR
	ID = 1023 ;
	VERSION = 13 ;
	STRING = "No aceptación";
END
STR
	ID = 1024 ;
	VERSION = 13 ;
	STRING = "El sistema no está disponible";
END
STR
	ID = 1025 ;
	VERSION = 13 ;
	STRING = "Anulación";
END
STR
	ID = 1026 ;
	VERSION = 13 ;
	STRING = "No hay cantidad cargada";
END
STR
	ID = 1027 ;
	VERSION = 13 ;
	STRING = "Comando no válido";
END
STR
	ID = 1028 ;
	VERSION = 13 ;
	STRING = "Fallo al seleccionar artículos";
END
STR
	ID = 1029 ;
	VERSION = 13 ;
	STRING = "CVV no válido";
END
STR
	ID = 1030 ;
	VERSION = 13 ;
	STRING = "Fondos insuficientes / límite de crédito superado";
END
STR
	ID = 1031 ;
	VERSION = 13 ;
	STRING = "Problema lógico";
END
STR
	ID = 1032 ;
	VERSION = 13 ;
	STRING = "El formato del número de cuenta es incorrecto";
END
STR
	ID = 1033 ;
	VERSION = 13 ;
	STRING = "Hay caracteres inválidos en el número de cuenta";
END
STR
	ID = 1034 ;
	VERSION = 13 ;
	STRING = "Error del sistema o mal funcionamiento con el emisor";
END

DOMAIN = "Reason codes-Payment Suspense Management" ;
STR
	ID = 2999 ;
	VERSION = 14 ;
	STRING = "Id de razón predeterminada ";
	HELPSTR = "Id de razón no especificada" ;
END
STR
	ID = 2001 ;
	VERSION = 14 ;
	STRING = "Nº de cuenta no encontrado";
	HELPSTR = "Nº de cuenta no encontrado en la base de datos" ;
END
STR
	ID = 2002 ;
	VERSION = 14 ;
	STRING = "Estado de la cuenta cerrado ";
	HELPSTR = "Estado de la cuenta cerrado" ;
END
STR
	ID = 2003 ;
	VERSION = 14 ;
	STRING = "Factura no encontrada ";
	HELPSTR = "Factura no encontrada en la base de datos " ;
END
STR
	ID = 2004 ;
	VERSION = 14 ;
	STRING = "Id de transacción no válida ";
	HELPSTR = "Id de transacción de pago especificada no válida " ;
END
STR
	ID = 2005 ;
	VERSION = 14 ;
	STRING = "Falta Id de transacción ";
	HELPSTR = "Falta Id de transacción para pago erróneo" ;
END
STR
	ID = 2006 ;
	VERSION = 14 ;
	STRING  = "Importe debido a reciclaje";
	HELPSTR = "Importe debido a reciclaje";
	EVENT-GLID
		"/event/billing/payment"		113 ;
	EVENT-GLID-END
END
STR
	ID = 2007 ;
	VERSION = 14 ;
	STRING = "Es necesaria la distribución de información de varias facturaciones.";
	HELPSTR = "La distribución de información de varias facturaciones es necesaria para este pago.";
END

DOMAIN = "Reason Codes-Payment Suspense Management Action Owner" ;
STR
	ID = 3999 ;
	VERSION = 15 ;
	STRING = "Propietario de la acción predeterminada ";
	HELPSTR = "Propietario de la acción predeterminado para Centro de suspensión de pagos " ;
END

DOMAIN = "Reason Codes-Payment Suspense Management, Reversal Reasons" ;
STR
	ID = 4999 ;
	VERSION = 16 ;
	STRING	= "No es posible corregir el pago" ;
	HELPSTR = "No es posible corregir el pago" ; 
	EVENT-GLID
		"/event/billing/reversal"		112 ;
	EVENT-GLID-END
END
STR
	ID = 4001 ;
	VERSION = 16 ;
	STRING	= "Inversión debido a reciclaje" ;
	HELPSTR = "Inversión debido a reciclaje" ; 
	EVENT-GLID
		"/event/billing/reversal"		113 ;
	EVENT-GLID-END
END

DOMAIN = "Reason Codes-Credit Reasons Non-Currency Account Adjustments" ;
STR
        ID = 1 ;
        VERSION = 21 ;
        STRING = "Cliente insatisfecho con el servicio";
END

DOMAIN = "Reason Codes-Debit Reasons Non-Currency Account Adjustments" ;
STR
        ID = 1 ;
        VERSION = 22 ;
        STRING = "Cuenta acreditada por error";
END

DOMAIN = "Reason Codes-Credit Reasons Bill Adjustments" ;
STR
        ID = 1 ;
        VERSION = 23 ;
        STRING = "Cliente insatisfecho con el servicio";
END

DOMAIN = "Reason Codes-Debit Reasons Bill Adjustments" ;
STR
        ID = 1 ;
        VERSION = 24 ;
        STRING = "Cargos de servicios";
END

DOMAIN = "Reason Codes-Credit Reasons Bill Disputes" ;
STR
        ID = 1 ;
        VERSION = 25 ;
        STRING = "Cliente insatisfecho con el servicio";
END

DOMAIN = "Reason Codes-Debit Reasons Bill Disputes" ;
STR
        ID = 1 ;
        VERSION = 26 ;
        STRING = "Cuenta acreditada por error";
END

DOMAIN = "Reason Codes-Reasons Bill Settlements" ;
STR
        ID = 1 ;
        VERSION = 27 ;
        STRING = "Cliente insatisfecho con el servicio";
END

DOMAIN = "Reason Codes-Credit Reasons Currency Connection Adjustments" ;
STR
        ID = 1 ;
        VERSION = 28 ;
        STRING = "Cliente insatisfecho con el servicio";
END

DOMAIN = "Reason Codes-Debit Reasons Currency Connection Adjustments" ;
STR
        ID = 1 ;
        VERSION = 29 ;
        STRING = "Acreditado por error";
END

DOMAIN = "Reason Codes-Credit Reasons Non-Currency Connection Disputes" ;
STR
        ID = 1 ;
        VERSION = 30 ;
        STRING = "Cliente insatisfecho con el servicio";
END

DOMAIN = "Reason Codes-Debit Reasons Non-Currency Connection Disputes" ;
STR
        ID = 1 ;
        VERSION = 31 ;
        STRING = "Acreditado por error";
END

DOMAIN = "Reason Codes-Credit Reasons Item Adjustments" ;
STR
        ID = 1 ;
        VERSION = 32 ;
        STRING = "Cliente insatisfecho con el servicio";
END

DOMAIN = "Reason Codes-Debit Reasons Item Adjustments" ;
STR
        ID = 1 ;
        VERSION = 33 ;
        STRING = "Acreditado por error";
END

DOMAIN = "Reason Codes-Credit Reasons Item Disputes" ;
STR
        ID = 1 ;
        VERSION = 34 ;
        STRING = "Cliente insatisfecho con el servicio";
END

DOMAIN = "Reason Codes-Debit Reasons Item Disputes" ;
STR
        ID = 1 ;
        VERSION = 35 ;
        STRING = "Acreditado por error";
END

DOMAIN = "Reason Codes-Reasons Item Settlements" ;
STR
        ID = 1 ;
        VERSION = 36 ;
        STRING = "Cliente insatisfecho con el servicio";
END

DOMAIN = "Reason Codes-Credit Reasons Event Adjustments" ;
STR
        ID = 1 ;
        VERSION = 37 ;
        STRING = "Cliente insatisfecho con el servicio";
END

DOMAIN = "Reason Codes-Debit Reasons Event Adjustments" ;
STR
        ID = 1 ;
        VERSION = 38 ;
        STRING = "Acreditado por error";
END

DOMAIN = "Reason Codes-Credit Reasons Event Disputes" ;
STR
        ID = 1 ;
        VERSION = 39 ;
        STRING = "Cliente insatisfecho con el servicio";
END

DOMAIN = "Reason Codes-Debit Reasons Event Disputes" ;
STR
        ID = 1 ;
        VERSION = 40 ;
        STRING = "Acreditado por error";
END

DOMAIN = "Reason Codes-Reasons Event Settlements" ;
STR
        ID = 1 ;
        VERSION = 41 ;
        STRING = "Cliente insatisfecho con el servicio";
END

DOMAIN = "Reason Codes-Reasons Write-off level" ;
STR
        ID = 1 ;
        VERSION = 42 ;
        STRING = "Cancelar cuenta";
END
STR
        ID = 2 ;
        VERSION = 42 ;
        STRING = "Cancelación de información de facturación";
END

DOMAIN = "Reason Codes-Bill Correction Reasons" ;
STR
        ID = 0 ;
        VERSION = 43 ;
        STRING = "Otros";
END
STR
        ID = 1 ;
        VERSION = 43 ;
        STRING = "Actualización en la dirección de la factura";
END
STR
        ID = 2 ;
        VERSION = 43 ;
        STRING = "Ajuste manual";
END
STR
        ID = 3 ;
        VERSION = 43 ;
        STRING = "Corrección de precio";
END
DOMAIN = "Reason Codes-Tax-exemptions" ;
STR
	ID = 1 ;
	VERSION = 44 ;
	STRING = "Exención basada en estado de cliente" ;
END
STR
	ID = 2 ;
	VERSION = 44 ;
	STRING = "Exención basada en ubicación" ;
END
STR
	ID = 3 ;
	VERSION = 44 ;
	STRING = "Reclasificación de estado de exención" ;
END

DOMAIN = "Reason Codes-Write off" ;
STR
	ID = 1 ;
	VERSION = 45 ;
	STRING = "Cargos fraudulentos" ;
END
STR
	ID = 2 ;
	VERSION = 45 ;
	STRING = "Fallo de cobro" ;
END
STR
        ID = 3 ;
        VERSION = 45 ;
        STRING = "Cargos erróneos" ;
END

DOMAIN = "Reason Codes-Write off Reversal" ;
STR
	ID = 1 ;
	VERSION = 46 ;
	STRING = "Re-establish service" ;
END

DOMAIN = "Reason Codes-SEPA Reject Reasons" ;
STR
        ID = 1 ;
        VERSION = 51 ;
        STRING = "AC01";
	HELPSTR = "Identificador de cuenta incorrecto (por ejemplo: IBAN no válido) ";
END
STR
        ID = 2 ;
        VERSION = 51 ;
        STRING = "AC04";
	HELPSTR = "Cuenta cerrada ";
END
STR
        ID = 3 ;
        VERSION = 51 ;
        STRING = "AC06";
	HELPSTR = "Cuenta bloqueada. Cuenta bloqueada para domiciliación por el deudor ";
END
STR
        ID = 4 ;
        VERSION = 51 ;
        STRING = "AG01";
	HELPSTR = "Domiciliación prohibida en esta cuenta por motivos normativos ";
END
STR
        ID = 5 ;
        VERSION = 51 ;
        STRING = "AG02";
	HELPSTR = "Código de operación/transacción incorrecto, formato de archivo no válido. Se usará para indicar un código de operación/transacción incorrecto ";
END
STR
        ID = 6 ;
        VERSION = 51 ;
        STRING = "AM04";
	HELPSTR = "Fondos insuficientes ";
END
STR
        ID = 7 ;
        VERSION = 51 ;
        STRING = "AM05";
	HELPSTR = "Cobro duplicado ";
END
STR
        ID = 8 ;
        VERSION = 51 ;
        STRING = "BE01";
	HELPSTR = "El nombre del deudor no coincide con el nombre del titular de la cuenta ";
END
STR
        ID = 9 ;
        VERSION = 51 ;
        STRING = "FF01";
	HELPSTR = "Código de operación/transacción incorrecto, formato de archivo no válido. Se usará para indicar un formato de archivo no válido ";
END
STR
        ID = 10 ;
        VERSION = 51 ;
        STRING = "MD01";
	HELPSTR = "Sin orden ";
END
STR
        ID = 11 ;
        VERSION = 51 ;
        STRING = "MD02";
	HELPSTR = "Faltan datos de orden o son incorrectos";
END
STR
        ID = 12 ;
        VERSION = 51 ;
        STRING = "MD07";
	HELPSTR = "Deudor fallecido ";
END
STR
        ID = 13 ;
        VERSION = 51 ;
        STRING = "MS02";
	HELPSTR = "Rehusado por deudor ";
END
STR
        ID = 14 ;
        VERSION = 51 ;
        STRING = "MS03";
	HELPSTR = "Motivo no especificado ";
END
STR
        ID = 15 ;
        VERSION = 51 ;
        STRING = "RC01";
	HELPSTR = "Identificador de banco incorrecto (por ejemplo: BIC no válido) ";
END
STR
        ID = 16 ;
        VERSION = 51 ;
        STRING = "RR01";
	HELPSTR = "Motivo normativo ";
END
STR
        ID = 17 ;
        VERSION = 51 ;
        STRING = "RR02";
	HELPSTR = "Motivo normativo ";
END
STR
        ID = 18 ;
        VERSION = 51 ;
        STRING = "RR03";
	HELPSTR = "Motivo normativo ";
END
STR
        ID = 19 ;
        VERSION = 51 ;
        STRING = "RC04";
	HELPSTR = "Motivo normativo ";
END
STR
        ID = 20 ;
        VERSION = 51 ;
        STRING = "SL01";
	HELPSTR = "Servicio específico ofrecido por el banco del deudor ";
END
STR
        ID = 21 ;
        VERSION = 51 ;
        STRING = "DNOR";
	HELPSTR = "El banco del deudor no está registrado con este BIC en el sistema de compensación y liquidación ";
END
STR
        ID = 22 ;
        VERSION = 51 ;
        STRING = "CNOR";
	HELPSTR = "El banco del acreedor no está registrado con este BIC en el sistema de compensación y liquidación ";
END
STR
        ID = 23 ;
        VERSION = 51 ;
        STRING = "SL01";
	HELPSTR = "Servicio específico ofrecido por el banco del deudor ";
END

DOMAIN = "Reason Codes-SEPA Reversal Reasons" ;
STR
        ID = 1 ;
        VERSION = 52 ;
        STRING = "AM05";
	HELPSTR = "Entrada duplicada ";
END
STR
        ID = 2 ;
        VERSION = 52 ;
        STRING = "MS02";
	HELPSTR = "Motivo normativo ";
END

DOMAIN = "Others" ;
STR
	EVENT-GLID
		"/event/billing/adjustment"		105 ;
		"/event/billing/adjustment/account"	105 ;
		"/event/billing/adjustment/item"   	105 ;
		"/event/billing/payment"		109 ;
		"/event/billing/reversal"		109 ;
		"/event/billing/dispute"		107 ;
		"/event/billing/dispute/item"		107 ;
		"/event/billing/settlement"		108 ;
		"/event/billing/settlement/item"	108 ;
 		"/event/billing/writeoff"		106 ;
		"/event/billing/writeoff/item"		106 ;
		"/event/billing/refund"			109 ;
		"/event/billing/writeoff_reversal"	111 ;
		"/event/journal/epsilon"		1512 ;
	EVENT-GLID-END
END
