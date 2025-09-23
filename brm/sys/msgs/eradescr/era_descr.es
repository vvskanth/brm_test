
##########################################################################
#
# Copyright (c) 2001, 2012, Oracle and/or its affiliates. All rights reserved. 
#     This material is the confidential property of Oracle Corporation 
#     or its  licensors and may be used, reproduced, stored
#     or transmitted only in accordance with a valid Oracle license or 
#     sublicense agreement.
#
##########################################################################
#
#   File:  era_descr.es
#
#   Description:
#
#	This file contains localized string object definitions.
#
#	Locale:  es ( Spanish )
#
#	Domain:  era_descr (version = 1)
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
#	    this ERA name, description file. If there are duplicate entries,
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
#	    this ERA name, description file. Please see the note about 
#	    uniqueness above.
#
#	    The id's and version's chosen in this file should be kept in
#	    sync with the pin_device_state_sim configuration file.
#
#	[6] String Version:
#
#	    Each string has an individual version, typically starting
#	    from 1, that the string owner assigns.
#
##########################################################################

LOCALE = "es" ;

DOMAIN = "era_descr" ;
STR
	ID = 0 ;
	VERSION = 1 ;
	STRING = "Clasificación" ;
END
STR
	ID = 1 ;
	VERSION = 1 ;
	STRING = "Esta opción asigna la cuenta a un grupo de cuentas que pertenecen a una organización, como puede ser una organización comercial, una escuela o una organización privada. Se aplicarán descuentos a las llamadas realizadas entre los miembros del grupo. Una cuenta puede pertenecer a un único grupo. Para ofrecer esta promoción, escriba la palabra TIPO en el campo Nombre y el nombre del grupo en el campo Valor. Esta opción aplica igualmente varios descuentos definidos por la compañía. Una cuenta puede tener más de un tipo de opción de calidad. Para ofrecer esta promoción, introduzca la palabra CALIDAD en el campo Nombre y el código de la promoción de calidad en el campo Valor." ;
END
STR
	ID = 2 ;
	VERSION = 1 ;
	STRING = "Un día especial" ;
END
STR
	ID = 3 ;
	VERSION = 1 ;
	STRING = "Esta opción aplica descuentos a las llamadas realizadas en su cumpleaños. Una cuenta puede tener sólo una fecha de cumpleaños. Para ofrecer esta promoción, introduzca la palabra CUMPLEAÑOS en el campo Nombre y la fecha de cumpleaños del cliente en el campo Valor." ;
END
STR
	ID = 4 ;
	VERSION = 1 ;
	STRING = "Corporación" ;
END
STR
	ID = 5 ;
	VERSION = 1 ;
	STRING = "Esta opción asigna la cuenta a un grupo de cuentas que comparte algunas propiedades de telefonía como, por ejemplo, un grupo de cuentas que realiza llamadas internas dentro de la misma compañía. Se aplicarán descuentos a las llamadas realizadas entre los miembros del grupo. Una cuenta puede pertenecer a un único grupo. Para ofrecer esta promoción, escriba la palabra ACUERDO en el campo Nombre y el nombre de la promoción en el campo Valor." ;
END
STR
	ID = 6 ;
	VERSION = 1 ;
	STRING = "Base de datos" ;
END
STR
	ID = 7 ;
	VERSION = 1 ;
	STRING = "Esta opción identifica cuentas para recopilar información comercial. Una cuenta puede tener más de una promoción de este tipo. Para ofrecer esta promoción, introduzca la palabra SEGMENTO en el campo Nombre y el nombre de la promoción en el campo Valor." ;
END
STR
	ID = 8 ;
	VERSION = 1 ;
	STRING = "Grupo de usuarios cerrado" ;
END
STR
	ID = 9 ;
	VERSION = 1 ;
	STRING = "Esta opción crea un grupo de usuarios cerrado. Se aplican descuentos a las llamadas realizadas entre los números de teléfono del mismo grupo. Para ofrecer esta promoción, introduzca el número de teléfono o ID de código de entrada de la cuenta en el campo Nombre y el nombre del grupo de usuarios cerrado en el campo Valor." ;
END
STR
	ID = 10 ;
	VERSION = 1 ;
	STRING = "Nivel del servicio" ;
END
STR
	ID = 11 ;
	VERSION = 1 ;
	STRING = "Esta opción asigna una Calidad del servicio determinada a un servicio. Un servicio puede tener más de un tipo de opción de Calidad del servicio. Para ofrecer esta promoción, introduzca la palabra ACUERDO en el campo Nombre y el nombre de la promoción en el campo Valor." ;
END
STR
	ID = 12 ;
	VERSION = 1 ;
	STRING = "Amigos y familia" ;
END
STR
	ID = 13 ;
	VERSION = 1 ;
	STRING = "Esta opción proporciona descuentos a llamadas a números específicos como, por ejemplo, a amigos y familia. Una cuenta puede incluir varios números de teléfono de amigos y familia. Para ofrecer esta promoción, introduzca la palabra NÚMERO en el campo Nombre y el número de teléfono en el campo Valor. En lugar del número de teléfono también puede introducir el ID de código de entrada del cliente." ;
END
STR
	ID = 14 ;
	VERSION = 1 ;
	STRING = "Zona interna" ;
END
STR
	ID = 15 ;
	VERSION = 1 ;
	STRING = "Esta opción proporciona descuentos a las llamadas realizadas a determinados números o regiones como, por ejemplo, a todos los números de un país o código de área. Una cuenta puede incluir varios números de zona interna. Para ofrecer esta promoción, introduzca la palabra NÚMERO en el campo Nombre. En el campo Valor, introduzca el número de teléfono, un ID de código de entrada o un indicador regional como, por ejemplo, un código de país o de área." ;
END
STR
	ID = 16 ;
	VERSION = 1 ;
	STRING = "Grupo interno" ;
END
STR
	ID = 17 ;
	VERSION = 1 ;
	STRING = "Esta opción define las llamadas de un grupo interno de una cuenta. Las llamadas realizadas a números que se encuentren fuera del grupo interno se cobrarán de forma diferente a las nacionales. Una cuenta puede incluir varias entradas de grupos internos. Para ofrecer esta promoción, introduzca la palabra ID_GRUPO en el campo Nombre. En el campo Valor, introduzca el código del grupo interno." ;
END
STR
	ID = 18 ;
	VERSION = 1 ;
	STRING = "Plan de tarifas" ;
END
STR
	ID = 19 ;
	VERSION = 1 ;
	STRING = "Esta opción proporciona información adicional acerca de las tarifas que no se incluye en los productos del cliente. Para ofrecer esta promoción, introduzca la prioridad del plan de tarifas en el campo Nombre. Un número menor se traduce en una categoría mayor. Introduzca el plan de tarifas integrado en el campo Valor." ;
END
STR
	ID = 20 ;
	VERSION = 1 ;
	STRING = "Modelo de descuento" ;
END
STR
	ID = 21 ;
	VERSION = 1 ;
	STRING = "Esta opción proporciona información adicional acerca de las tarifas que no se incluye en los productos del cliente. Para ofrecer esta promoción, introduzca la palabra INTEGRADO en el campo Nombre.  Introduzca el código del modelo de descuento integrado en el campo Valor." ;
END
STR
	ID = 22 ;
	VERSION = 1 ;
	STRING = "Cuenta de descuentos" ;
END
STR
	ID = 23 ;
	VERSION = 1 ;
	STRING = "Esta opción permite especificar una cuenta que proporciona descuentos por volumen a varias cuentas. Una cuenta puede tener tan sólo una de estas promociones. Para ofrecer esta promoción, introduzca la palabra INTEGRADO en el campo Nombre y el ID de código de entrada de la cuenta de descuentos principal en el campo Valor." ;
END
STR
	ID = 24 ;
	VERSION = 1 ;
	STRING = "Paquete de descuento" ;
END
STR
	ID = 25 ;
	VERSION = 1 ;
	STRING = "Esta opción especifica el subconjunto de reglas de descuento que se tendrán en cuenta en la aplicación de descuentos. Las reglas de descuento seguirán un orden secuencial en cuanto a la prioridad definida para el modelo de descuento, pero sólo se aplicará cada regla de descuento si ya se han utilizado por completo las reglas de descuento de prioridad superior. Se tendrán en cuenta todas las reglas de descuento de un modelo de descuento, si no se ha especificado ningún atributo de tarificación extendida de DiscountBundle. Para ofrecer esta promoción, introduzca el código de regla de descuento en el campo VALUE.";
END
STR
	ID = 26 ;
	VERSION = 1 ;
	STRING = "Titular de descuento" ;
END
STR
	ID = 27 ;
	VERSION = 1 ;
	STRING = "Esta opción especifica el titular del saldo de descuento para una regla de descuento. Permite que varias cuentas compartan un descuento. La cuenta actual se tratará como el titular del descuento, si no se especifica ningún atributo de tarificación extendida de DiscountOwner. Para ofrecer esta promoción, introduzca el nombre de la norma de descuento en el campo NAME y el POID de cuenta del titular del descuento en el campo VALUE.";
END
STR
	ID = 28 ;
	VERSION = 1 ;
	STRING = "Llamada interrumpida" ;
END
STR
	ID = 29 ;
	VERSION = 1 ;
	STRING = "Esta opción proporciona descuentos a llamadas interrumpidas en llamadas continuas. Para obtener esta promoción, introduzca SAME_CALLED_PARTY, MAX_TIME_TO_CONTINUATION_CALL, MAX_INTERVENING_CALLS en el campo Nombre y sus valores correspondientes en campos Valor. SAME_CALLED_PARTY especifica si las llamadas de continuación deben ser al mismo número que las interrumpidas o si las llamadas a otros números pueden ser llamadas de continuación. Si no se especifica, las de continuación deben ser al mismo número que las interrumpidas. MAX_TIME_TO_CONTINUATION_CALL especifica la duración permitida, en segundos, entre el final de la llamada interrumpida y el inicio de la de continuación. Si no se especifica, no hay límite temporal entre ambas, siempre y cuando se produzcan dentro del mismo ciclo de facturación. MAX_INTERVENING_CALLS especifica el número de llamadas intermedias permitidas entre una llamada interrumpida y una de continuación. Si no se especifica, se tendrán en cuenta todas las llamadas.";
END
