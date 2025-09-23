##########################################################################
#
# Copyright (c) 2000, 2015, Oracle and/or its affiliates. All rights reserved.
# This material is the confidential property of Oracle Corporation
# or its licensors and may be used, reproduced, stored
# or transmitted only in accordance with a valid Oracle license or
# sublicense agreement.
#
##########################################################################
#
#   File:  errors.es
#
#   Description:
#
#	This file contains localized string object definitions.
#
#	Locale:  es ( Spanish )
#
#	Domain:  Errors
#
#   Rules:
#
#	[1] Uniqueness:
#
#	    The combination of locale, domain, string ID, and string
#	    version must uniquely define a string object within the
#	    universe of all string objects.
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
#	    The STRING and optional HELPSTR are localizable.  This file must be 
#	    in UTF-8 encoding. The LOCALE and DOMAIN strings are assumed
#	    to be ASCII (no extended-ASCII characters nor multiple byte 
#	    characters are allowed).
#
#	[5] String ID:
#
#	    A string ID must be unique within a domain.
#
#	[6] String Version:
#
#	    Each string has an individual version, typically starting
#	    from 1, that the string owner assigns.
#
##########################################################################

LOCALE = "es" ;

DOMAIN = "Errors" ;
STR
	ID = 0 ;
	VERSION = 1 ;
	STRING = "Sin errores" ;
	HELPSTR = "" ;
END
STR
	ID = 1 ;
	VERSION = 1 ;
	STRING = "Memoria insuficiente" ;
	HELPSTR = "Memoria insuficiente para completar la operación. Compruebe la memoria del sistema y los fallos de memoria." ;
END
STR
	ID = 2 ;
	VERSION = 1 ;
	STRING = "No se encuentran coincidencias" ;
	HELPSTR = "Infranet no pudo encontrar el valor que estaba buscando." ;
END
STR
	ID = 3 ;
	VERSION = 1 ;
	STRING = "No se encontró la entrada" ;
	HELPSTR = "Infranet no pudo encontrar un valor." ;
END
STR
	ID = 4 ;
	VERSION = 1 ;
	STRING = "Argumento no válido para la función" ;
	HELPSTR = "Uno de los campos requeridos de un flist es incorrecto. Error muy grave de programación." ;
END
STR
	ID = 5 ;
	VERSION = 1 ;
	STRING = "Datos XDR erróneos" ;
	HELPSTR = "La aplicación ha perdido inesperadamente la conexión con la base de datos de Infranet." ;
END
STR
	ID = 6 ;
	VERSION = 1 ;
	STRING = "No utilizado" ;
	HELPSTR = "No utilizado." ;
END
STR
	ID = 7 ;
	VERSION = 1 ;
	STRING = "Ha ocurrido un error durante una operación de lectura" ;
	HELPSTR = "Infranet no ha podido leer desde la red o algún otro dispositivo de e/s, debido probablemente a que la comunicación ha terminado inesperadamente." ;
END
STR
	ID = 8 ;
	VERSION = 1 ;
	STRING = "Error al crear Socket" ;
	HELPSTR = "La maquina o el proceso podrían haberse sobrecargado y alcanzado el límite en los descriptores de socket/archivo." ;
END
STR
	ID = 9 ;
	VERSION = 1 ;
	STRING = "Tipo de archivo u objeto no válido" ;
	HELPSTR = "Infranet ha encontrado un tipo de campo u objeto erróneo." ;
END
STR
	ID = 10 ;
	VERSION = 1 ;
	STRING = "Objeto de almacenamiento duplicado" ;
	HELPSTR = "Infranet no pudo crear un objeto de almacenamiento debido a que el ID solicitado ya se ha utilizado." ;
END
STR
	ID = 11 ;
	VERSION = 1 ;
	STRING = "Código que no indica error" ;
	HELPSTR = "Este código de error no indica que se haya producido un error. El FM de facturación utiliza este código para operaciones internas." ;
END
STR
	ID = 12 ;
	VERSION = 1 ;
	STRING = "Código que no indica error" ;
	HELPSTR = "Este código de error no indica que se haya producido un error. El FM de facturación utiliza este código para operaciones internas." ;
END
STR
	ID = 13 ;
	VERSION = 1 ;
	STRING = "Falta argumento" ;
	HELPSTR = "Falta un argumento requerido. Si el archivo de registro no indica el campo, véase la especificación del CódOp." ;
END
STR
	ID = 14 ;
	VERSION = 1 ;
	STRING = "Tipo de POID incorrecto" ;
	HELPSTR = "Infranet ha encontrado un tipo de objeto erróneo." ;
END
STR
	ID = 15 ;
	VERSION = 1 ;
	STRING = "No es posible descifrar/verificar cabecera" ;
	HELPSTR = "No es posible descifrar/verificar cabecera." ;
END
STR
	ID = 16 ;
	VERSION = 1 ;
	STRING = "Error al escribir en el socket" ;
	HELPSTR = "Error al escribir en el socket." ;
END
STR
	ID = 17 ;
	VERSION = 1 ;
	STRING = "Subestructura duplicada" ;
	HELPSTR = "Subestructura duplicada." ;
END
STR
	ID = 18 ;
	VERSION = 1 ;
	STRING = "Argumento de búsqueda erróneo" ;
	HELPSTR = "Argumento desconocido/erróneo en la búsqueda de plantilla/flist." ;
END
STR
	ID = 19 ;
	VERSION = 1 ;
	STRING = "ID de registro no válido" ;
	HELPSTR = "ID de registro erróneo en la búsqueda." ;
END
STR
	ID = 20 ;
	VERSION = 1 ;
	STRING = "Argumento de búsqueda duplicado" ;
	HELPSTR = "Entrada duplicada en la lista de argumentos." ;
END
STR
	ID = 21 ;
	VERSION = 1 ;
	STRING = "Valor de POID inexistente" ;
	HELPSTR = "Infranet no puede encontrar el objeto de almacenamiento en la base de datos." ;
END
STR
	ID = 22 ;
	VERSION = 1 ;
	STRING = "El número de BD de POID es cero" ;
	HELPSTR = "Asegúrese de que la rutina está transmitiendo un número de base de datos válido que coincide con el número del archivo de configuración." ;
END
STR
	ID = 23 ;
	VERSION = 1 ;
	STRING = "N.º de BD no válido" ;
	HELPSTR = "Número de BD del Gestor de datos no válido para realizar la conexión." ;
END
STR
	ID = 24 ;
	VERSION = 1 ;
	STRING = "Error al crear Socket" ;
	HELPSTR = "La maquina o el proceso podrían haberse sobrecargado y alcanzado el límite en los descriptores de socket/archivo." ;
END
STR
	ID = 25 ;
	VERSION = 1 ;
	STRING = "No es posible la conexión con el Gestor de datos" ;
	HELPSTR = "Infranet no pudo encontrar la dirección IP del ordenador que ejecuta el Gestor de datos. Compruebe el alias de la base de datos" ;
END
STR
	ID = 26 ;
	VERSION = 1 ;
	STRING = "No es posible la conexión con el Gestor de datos" ;
	HELPSTR = "Infranet no ha podido conectar con el Gestor de datos. Es posible que el archivo de configuración de CM haga referencia al Gestor de datos erróneo o que éste no se haya iniciado." ;
END
STR
	ID = 27 ;
	VERSION = 1 ;
	STRING = "No se puede conectar con CM" ;
	HELPSTR = "Es posible que el archivo de configuración de la aplicación esté haciendo referencia a un CM erróneo o que éste no se haya iniciado; o bien que no vuelva a haber más conexiones disponibles." ;
END
STR
	ID = 28 ;
	VERSION = 1 ;
	STRING = "ID de registro no válido" ;
	HELPSTR = "El ID del elemento especificado en la matriz no es válido. Es posible que el ID especificado sea superior al ID de registro máximo." ;
END
STR
	ID = 29 ;
	VERSION = 1 ;
	STRING = "Entrada de pin.conf caducada" ;
	HELPSTR = "Entrada de pin.conf no válida." ;
END
STR
	ID = 30 ;
	VERSION = 1 ;
	STRING = "Entrada de pin.conf no válida" ;
	HELPSTR = "Posible error de sintaxis en el archivo pin.conf." ;
END
STR
	ID = 31 ;
	VERSION = 1 ;
	STRING = "BD errónea" ;
	HELPSTR = "Número de BD de POID erróneo." ;
END
STR
	ID = 32 ;
	VERSION = 1 ;
	STRING = "Argumentos duplicados" ;
	HELPSTR = "Existen campos duplicados en el flist." ;
END
STR
	ID = 33 ;
	VERSION = 1 ;
	STRING = "No se puede establecer el ID de objeto" ;
	HELPSTR = "Infranet no ha podido asignar un ID de objeto de almacenamiento." ;
END
STR
	ID = 34 ;
	VERSION = 1 ;
	STRING = "No es posible crear el objeto" ;
	HELPSTR = "La rutina no ha podido crear el objeto." ;
END
STR
	ID = 35 ;
	VERSION = 1 ;
	STRING = "Nombre de campo erróneo" ;
	HELPSTR = "No se puede asignar el nombre del campo a un tipo válido de Infranet." ;
END
STR
	ID = 36 ;
	VERSION = 1 ;
	STRING = "CódOp no válido" ;
	HELPSTR = "El CódOp llamado no pertenece al conjunto de códigos de operación básicos de Infranet." ;
END
STR
	ID = 37 ;
	VERSION = 1 ;
	STRING = "Esta transacción ya está abierta" ;
	HELPSTR = "Una aplicación ha intentado abrir una transacción cuando ya había otra abierta en el mismo objeto de almacenamiento." ;
END
STR
	ID = 38 ;
	VERSION = 1 ;
	STRING = "No hay ninguna transacción abierta" ;
	HELPSTR = "Una aplicación ha intentado grabar o cancelar una transacción, pero no había ninguna abierta." ;
END
STR
	ID = 39 ;
	VERSION = 1 ;
	STRING = "Valor nulo" ;
	HELPSTR = "Un rutina no ha podido obtener un valor debido a que estaba establecido como \"null\"." ;
END
STR
	ID = 40 ;
	VERSION = 1 ;
	STRING = "No se ha podido liberar memoria" ;
	HELPSTR = "Una rutina ha intentado liberar memoria que ya no era necesaria pero la operación ha fallado." ;
END
STR
	ID = 41 ;
	VERSION = 1 ;
	STRING = "Error de e/s en el archivo" ;
	HELPSTR = "Error al leer/escribir en un archivo o cargar/abrir una biblioteca." ;
END
STR
	ID = 42 ;
	VERSION = 1 ;
	STRING = "El elemento no existe" ;
	HELPSTR = "La matriz del objeto de almacenamiento especificado no contiene el elemento especificado." ;
END
STR
	ID = 43 ;
	VERSION = 1 ;
	STRING = "Se ha perdido la conexión con la BD" ;
	HELPSTR = "Infranet ha perdido la conexión con la base de datos mientras se realizaba la transacción." ;
END
STR
	ID = 44 ;
	VERSION = 1 ;
	STRING = "Se han intentado realizar demasiadas transacciones" ;
	HELPSTR = "Infranet ha intentado realizar transacciones en demasiados Gestores de datos." ;
END
STR
	ID = 45 ;
	VERSION = 1 ;
	STRING = "Transacción perdida" ;
	HELPSTR = "Se ha perdido la transacción. Se ha producido un fallo en el Gestor de datos durante una transacción." ;
END
STR
	ID = 46 ;
	VERSION = 1 ;
	STRING = "No es posible interpretar datos" ;
	HELPSTR = "Infranet no ha podido interpretar los datos de la base de datos. Los datos no son válidos en el contexto actual e Infranet no puede resolver el conflicto." ;
END
STR
	ID = 47 ;
	VERSION = 1 ;
	STRING = "Procesamiento parcial de tarjetas de crédito" ;
	HELPSTR = "Al enviar un lote de transacciones a los Gestores de datos de tarjetas de crédito, algunas de las tarjetas de crédito se procesaron, pero otras no." ;
END
STR
	ID = 48 ;
	VERSION = 1 ;
	STRING = "Operación incompleta" ;
	HELPSTR = "Infranet no ha completado todavía una operación (como un CódOp o transacción)." ;
END
STR
	ID = 49 ;
	VERSION = 1 ;
	STRING = "Error de e/s en el flujo de datos" ;
	HELPSTR = "La aplicación ha encontrado un error al enviar datos a o desde la base de datos de Infranet. Normalmente, este código de error significa que se ha perdido la conexión con la base de datos. Si la red no funciona correctamente, es posible que el servidor haya dejado de funcionar." ;
END
STR
	ID = 50 ;
	VERSION = 1 ;
	STRING = "EOF en el flujo de datos" ;
	HELPSTR = "La aplicación ha perdido inesperadamente la conexión con la base de datos de Infranet. Normalmente, este código de error significa que se ha perdido la conexión con la base de datos. Si la red no funciona correctamente, es posible que el servidor haya dejado de funcionar." ;
END
STR
	ID = 51 ;
	VERSION = 1 ;
	STRING = "No existe ninguna operación en curso" ;
	HELPSTR = "No existe ninguna operación en curso." ;
END
STR
	ID = 52 ;
	VERSION = 1 ;
	STRING = "Operación en curso" ;
	HELPSTR = "Esta operación ya está en curso." ;
END
STR
	ID = 53 ;
	VERSION = 1 ;
	STRING = "Esta operación ya se ha realizado" ;
	HELPSTR = "Esta operación ya se ha realizado; no se ha establecido ninguna llamada." ;
END
STR
	ID = 54 ;
	VERSION = 1 ;
	STRING = "No existen campos de datos en el flist" ;
	HELPSTR = "El Gestor de datos ha recibido un flist sin campos de datos. Compruebe el flist de entrada." ;
END
STR
	ID = 55 ;
	VERSION = 1 ;
	STRING = "Campos no válidos" ;
	HELPSTR = "Infranet no ha podido crear un objeto debido a que uno o más valores no son válidos o los campos no permiten actualización." ;
END
STR
	ID = 56 ;
	VERSION = 1 ;
	STRING = "No es posible la conexión con CM" ;
	HELPSTR = "La aplicación no ha podido conectar con el CM. Compruebe el nombre de acceso y la contraseña." ;
END
STR
	ID = 57 ;
	VERSION = 1 ;
	STRING = "No se puede encontrar la dirección del CM" ;
	HELPSTR = " La aplicación no ha podido encontrar el ordenador que ejecuta el CM. El servicio de enlace (o DNS) está haciendo referencia a una dirección TCP/IP errónea o existen problemas en la red. Intente hacer ping en el CM para verificar la conexión con la red." ;
END
STR
	ID = 58 ;
	VERSION = 1 ;
	STRING = "No se puede redirigir la conexión" ;
	HELPSTR = "La información de redirección de conexión es incorrecta." ;
END
STR
	ID = 59 ;
	VERSION = 1 ;
	STRING = "Hay demasiadas redirecciones de código de entrada" ;
	HELPSTR = "Hay demasiadas redirecciones de conexión [??Revisor: ¿Entre qué elementos desea realizar la redirección???]
Es posible que este error se haya producido debido a un bucle en la configuración. Compruebe los archivos de configuración de la aplicación, de CM y DM." ;
END
STR
	ID = 60 ;
	VERSION = 1 ;
	STRING = "Error durante la búsqueda por pasos" ;
	HELPSTR = "La operación de búsqueda por pasos no ha podido encontrar un STEP_NEXT/STEP_END previsto." ;
END
STR
	ID = 61 ;
	VERSION = 1 ;
	STRING = "Se ha perdido la conexión con la BD" ;
	HELPSTR = "Infranet ha perdido la conexión con la base de datos mientras se realizaba la transacción y no ha podido restablecerla." ;
END
STR
	ID = 62 ;
	VERSION = 1 ;
	STRING = "No se trata de la raíz de un grupo" ;
	HELPSTR = "No se trata de la raíz de un grupo." ;
END
STR
	ID = 63 ;
	VERSION = 1 ;
	STRING = "Error de bloqueo del SO" ;
	HELPSTR = "Error de bloqueo en el sistema operativo." ;
END
STR
	ID = 64 ;
	VERSION = 1 ;
	STRING = "Autorización no realizada" ;
	HELPSTR = "Autorización no realizada." ;
END
STR
	ID = 65 ;
	VERSION = 1 ;
	STRING = "No editable" ;
	HELPSTR = "No se puede modificar el campo." ;
END
STR
	ID = 66 ;
	VERSION = 1 ;
	STRING = "Excepción C++ desconocida" ;
	HELPSTR = "Excepción C++ desconocida." ;
END
STR
	ID = 67 ;
	VERSION = 1 ;
	STRING = "Fallo al iniciar los servidores" ;
	HELPSTR = "Infranet no ha podido iniciar el proceso." ;
END
STR
	ID = 68 ;
	VERSION = 1 ;
	STRING = "Error al parar los servidores" ;
	HELPSTR = "Infranet no ha podido parar el proceso." ;
END
STR
	ID = 69 ;
	VERSION = 1 ;
	STRING = "Cola no válida" ;
	HELPSTR = "Cola no válida." ;
END
STR
	ID = 70 ;
	VERSION = 1 ;
	STRING = "Existe algo demasiado grande" ;
	HELPSTR = "Existe algo demasiado grande." ;
END
STR
	ID = 71 ;
	VERSION = 1 ;
	STRING = "Configuración regional no válida"  ;
	HELPSTR = "Infranet no puede interpretar la configuración regional. Compruebe la configuración regional del ordenador en el que se está ejecutando la aplicación." ;
END
STR
	ID = 72 ;
	VERSION = 1 ;
	STRING = "Error de conversión de codificación multibyte" ;
	HELPSTR = "Una aplicación cliente ha tenido un problema al convertir datos del formato UTF-8 a formato multibyte al encontrarse en la base de datos de Infranet. O la configuración regional del cliente es errónea, o bien los datos están dañados." ;
END
STR
	ID = 73 ;
	VERSION = 1 ;
	STRING = "Error de conversión de codificación unicode" ;
	HELPSTR = "Una aplicación cliente ha tenido un problema al convertir datos del formato UTF-8 a formato unicode al encontrarse en la base de datos de Infranet. O la configuración regional del cliente es errónea, o bien los datos están dañados." ;
END
STR
	ID = 74 ;
	VERSION = 1 ;
	STRING = "Formato MBCS no válido" ;
	HELPSTR = "El flist de entrada contiene una cadena que no está en formato multibyte válido." ;
END
STR
	ID = 75 ;
	VERSION = 1 ;
	STRING = "Formato Unicode no válido" ;
	HELPSTR = "El flist de entrada contiene una cadena que no está en formato unicode válido." ;
END
STR
	ID = 76 ;
	VERSION = 1 ;
	STRING = "Error al convertir a formato canónico" ;
	HELPSTR = "Error de conversión en formato canónico." ;
END
STR
	ID = 77 ;
	VERSION = 1 ;
	STRING = "Configuración regional no admitida al convertir a formato canónico" ;
	HELPSTR = "Infranet no admite la conversión a formato canónico para la configuración regional de la aplicación cliente." ;
END
STR
	ID = 78 ;
	VERSION = 1 ;
	STRING = "Error de concordancia de moneda entre principal/derivada" ;
	HELPSTR = "Una cuenta subordinada o una patrocinada tiene una moneda de cuenta diferente a la cuenta principal o patrocinada." ;
END
STR
	ID = 79 ;
	VERSION = 1 ;
	STRING = "La transacción se ha anulado debido a un conflicto de recursos" ;
	HELPSTR = "Dos subprocesos de una base de datos del servidor SQL han intentado acceder al mismo recurso de memoria. Uno de ellos ha fallado y la transacción se ha anulado. El Gestor de datos recuperará automáticamente la transacción. Si detecta este error sin que haya una notificación posterior de que el Gestor de datos ha intentado recuperar la operación, póngase en contacto con el representante de Software de Portal." ;
END
STR
	ID = 80 ;
	VERSION = 1 ;
	STRING = "Error de retraso" ;
	HELPSTR = "Infranet no puede retrasar el ajuste, la cancelación o cualquier otra transacción debido a que el informe de Contabilidad General ya se ha enviado." ;
END
STR
	ID = 81 ;
	VERSION = 1 ;
	STRING = "Límite de crédito excedido" ;
	HELPSTR = "El informe de contabilidad ya se ha enviado." ;
END
STR
	ID = 82 ;
	VERSION = 1 ;
	STRING = "El valor es \"NULL\" - no establecido." ;
	HELPSTR = "" ;
END
STR
	ID = 83 ;
	VERSION = 1 ;
	STRING = "Error detallado - msg_id utiliza ebuf mejorado" ;
	HELPSTR = "" ;
END
STR
	ID = 84 ;
	VERSION = 1 ;
	STRING = "Resuelva las acciones de C/C para billinfo, antes de mover el grupo de saldo." ;
	HELPSTR = "" ;
END
STR
        ID = 85 ;
        VERSION = 1 ;
        STRING = "BRM no puede retrasar la transacción más allá de la fecha de creación de la cuenta.";
        HELPSTR = "BRM no puede retrasar la transacción más allá de la fecha de creación de la cuenta.";
END
STR
        ID = 86 ;
        VERSION = 1 ;
        STRING = "BRM no puede retrasar la transacción debido a que el informe de contabilidad general ya se ha enviado.";
        HELPSTR = "BRM no puede retrasar la transacción debido a que el informe de contabilidad general ya se ha enviado.";
END
STR
        ID = 87 ;
        VERSION = 1 ;
        STRING = "BRM no puede retrasar la transacción más allá de la fecha de creación del servicio.";
        HELPSTR = "BRM no puede retrasar la transacción más allá de la fecha de creación del servicio.";
END
STR
        ID = 88 ;
        VERSION = 1 ;
        STRING = "BRM no puede retrasar la transacción más allá de la fecha de creación del producto o del descuento.";
        HELPSTR = "BRM no puede retrasar la transacción más allá de la fecha de creación del producto o del descuento.";
END
STR
        ID = 89 ;
        VERSION = 1 ;
        STRING = "BRM no puede retrasar la fecha de inicio porque ya se han aplicado las cuotas de compra o de ciclo.";
        HELPSTR = "BRM no puede retrasar la fecha de inicio porque ya se han aplicado las cuotas de compra o de ciclo.";
END
STR
        ID = 90 ;
        VERSION = 1 ;
        STRING = "BRM no puede antedatar la transacción porque se ha producido un cambio de estado.";
        HELPSTR = "BRM no puede antedatar la transacción porque se ha producido un cambio de estado.";
END
STR
        ID = 91 ;
        VERSION = 1 ;
        STRING = "BRM no puede antedatar la transacción a antes de la hora de inicio de la compra.";
        HELPSTR = "BRM no puede antedatar la transacción a antes de la hora de inicio de la compra.";
END
STR
        ID = 92 ;
        VERSION = 1 ;
        STRING = "BRM no puede ejecutar la facturación porque la diferencia entre la hora de facturación y la hora actual es mayor que el valor especificados en las entradas de cm pin.conf \valid_forward_interval\ o \valid_backward_interval\."; 
        HELPSTR = "BRM no puede ejecutar la facturación porque la diferencia entre la hora de facturación y la hora actual es mayor que el valor especificados en las entradas de cm pin.conf \valid_forward_interval\ o \valid_backward_interval\."; 
END
STR
        ID = 93 ;
        VERSION = 1 ;
        STRING = "El objeto de factura no está en formato Flist.";
        HELPSTR = "El objeto de factura no está en formato Flist.";
END
STR
        ID = 94 ;
        VERSION = 1 ;
        STRING = "No se puede cambiar el grupo de saldo predeterminado de billinfo.";
        HELPSTR = "No se puede cambiar el grupo de saldo predeterminado de billinfo.";
END
STR
        ID = 95 ;
        VERSION = 1 ;
        STRING = "Can not repoint the account level balance group.";
        HELPSTR = "No se puede cambiar el grupo de saldo de nivel de cuenta.";
END
STR
        ID = 96 ;
        VERSION = 1 ;
        STRING = "Fallo de validación de perfil.";
        HELPSTR = "Fallo de validación de perfil.";
END
STR
        ID = 97 ;
        VERSION = 1 ;
        STRING = "La función de facturación correctiva está desactivada.";
        HELPSTR = "La función de facturación correctiva está desactivada.";
END
STR
        ID = 98 ;
        VERSION = 1 ;
        STRING = "No se puede generar una facturación correctiva para una facturación subordinada.";
        HELPSTR = "No se puede generar una facturación correctiva para una facturación subordinada.";
END
STR
        ID = 99 ;
        VERSION = 1 ;
        STRING = "Debe finalizarse la facturación para generar una facturación correctiva.";
        HELPSTR = "Debe finalizarse la facturación para generar una facturación correctiva.";
END
STR
        ID = 100 ;
        VERSION = 1 ;
        STRING = "Los campos REASON_ID y REASON_DOMAIN_ID deben presentarse en la entrada de datos.";
        HELPSTR = "Los campos REASON_ID y REASON_DOMAIN_ID deben presentarse en la entrada de datos.";
END
STR
        ID = 101 ;
        VERSION = 1 ;
        STRING = "El tipo de factura debe ser 'Sustitución', si no hay cambios de C/C.";
        HELPSTR = "El tipo de factura debe ser 'Sustitución', si no hay cambios de C/C.";
END
STR
        ID = 102 ;
        VERSION = 1 ;
        STRING = "No hay cargos de C/C.";
        HELPSTR = "No hay cargos de C/C.";
END
STR
        ID = 103 ;
        VERSION = 1 ;
        STRING = "La facturación no es una última para el tipo contable balance_forward.";
        HELPSTR = "La facturación no es una última para el tipo contable balance_forward.";
END
STR
        ID = 104 ;
        VERSION = 1 ;
        STRING = "La facturación está pagada por completo.";
        HELPSTR = "La facturación está pagada por completo.";
END
STR
        ID = 105 ;
        VERSION = 1 ;
        STRING = "La facturación está pagada.";
        HELPSTR = "La facturación está pagada.";
END
STR
        ID = 106 ;
        VERSION = 1 ;
        STRING = "Los cargos de C/C son demasiado pequeños.";
        HELPSTR = "Los cargos de C/C son demasiado pequeños.";
END
STR
        ID = 108 ;
        VERSION = 1 ;
        STRING = "El REASON_ID o el REASON_DOMAIN_ID no es correcto.";
        HELPSTR = "El REASON_ID o el REASON_DOMAIN_ID no es correcto.";
END
STR
        ID = 109 ;
        VERSION = 1 ;
        STRING = "Debe crearse la factura para la facturación antes de generar la facturación correctiva.";
        HELPSTR = "Debe crearse la factura para la facturación antes de generar la facturación correctiva.";
END
STR
        ID = 110 ;
        VERSION = 1 ;
        STRING = "No hay cargos de C/C, excepto pagos.";
        HELPSTR = "No hay cargos de C/C, excepto pagos.";
END
STR
        ID = 122 ;
        VERSION = 1 ;
        STRING = "Contraseña no válida. La contraseña comienza por un prefijo no válido.";
        HELPSTR = "Contraseña no válida. La contraseña comienza por un prefijo no válido.";
END
STR
        ID = 123 ;
        VERSION = 1 ;
        STRING = "Sólo uno entre el importe de descuento y el porcentaje permitido.";
        HELPSTR = "Sólo uno entre el importe de descuento y el porcentaje permitido.";
END
STR
        ID = 124 ;
        VERSION = 1 ;
        STRING = "No se puede cancelar una acción obligatoria.";
        HELPSTR = "No se puede cancelar una acción obligatoria.";
END
STR
        ID = 125 ;
        VERSION = 1 ;
        STRING = "No se pueden insertar acciones antes/entre acciones de cancelación.";
        HELPSTR = "No se pueden insertar acciones antes/entre acciones de cancelación.";
END
STR
	ID = 126 ;
	VERSION = 1 ;
	STRING = "Esta información de facturación ya forma parte de un grupo de cobros.";
	HELPSTR = "Esta información de facturación ya forma parte de un grupo de cobros.";
END
STR
        ID = 127 ;
	VERSION = 1 ;
	STRING = "Esta información de facturación ya tiene una obligación de promesa de pago. No se puede invocar otra promesa.";
	HELPSTR = "Esta información de facturación ya tiene una obligación de promesa de pago. No se puede invocar otra promesa.";
END
STR
        ID = 128 ;
	VERSION = 1 ;
	STRING = "No se puede invocar una promesa con una fecha pasada.";
	HELPSTR = "No se puede invocar una promesa con una fecha pasada.";
END
STR        
	ID = 129 ;
	VERSION = 1 ;
	STRING = "No se puede añadir esta acción en el período de promesa de pago.";
	HELPSTR = "No se puede añadir esta acción en el período de promesa de pago.";
END
STR
	ID = 130 ;
	VERSION = 1 ;
	STRING = "No se pueden añadir acciones de promesa de pago cuando la información de facturación no tiene una obligación de promesa de pago.";
	HELPSTR = "No se pueden añadir acciones de promesa de pago cuando la información de facturación no tiene una obligación de promesa de pago.";
END
STR
	ID = 131 ;
	VERSION = 1 ;
	STRING = "Solo se pueden añadir acciones de promesa de pago durante el período de promesa de pago.";
	HELPSTR = "Solo se pueden añadir acciones de promesa de pago durante el período de promesa de pago.";
END
STR
	ID = 132 ;
	VERSION = 1 ;
	STRING = "La acción no está en estado pendiente. No se puede actualizar el importe de objetivo.";
	HELPSTR = "La acción no está en estado pendiente. No se puede actualizar el importe de objetivo.";
END
STR
	ID = 133 ;
	VERSION = 1 ;
	STRING = "No se encuentra ni el importe de objetivo ni el número de objetivos. Debería especificarse uno de estos valores.";
	HELPSTR = "No se encuentra ni el importe de objetivo ni el número de objetivos. Debería especificarse uno de estos valores.";
END
STR
	ID = 134 ;
	VERSION = 1 ;
	STRING = "El importe de objetivo debería ser inferior o igual al importe pendiente, o debería ser un mínimo del importe de salida.";
	HELPSTR = "El importe de objetivo debería ser inferior o igual al importe pendiente, o debería ser un mínimo del importe de salida.";
END
STR
	ID = 135 ;
	VERSION = 1 ;
	STRING = "No se encuentra ni el intervalo de objetivo ni los días totales. Debería especificarse uno de estos valores.";
	HELPSTR = "No se encuentra ni el intervalo de objetivo ni los días totales. Debería especificarse uno de estos valores.";
END
STR
	ID = 136 ;
	VERSION = 1 ;
	STRING = "Uno de los parámetros de entrada es cero.";
	HELPSTR = "Uno de los parámetros de entrada es cero.";
END
STR
	ID = 136 ;
	VERSION = 1 ;
	STRING = "Uno de los parámetros de entrada es cero.";
	HELPSTR = "Uno de los parámetros de entrada es cero.";
END
STR
	ID = 137 ;
	VERSION = 1 ;
	STRING = "El importe de promesa debe ser mayor que el importe de salida.";
	HELPSTR = "El importe de promesa debe ser mayor que el importe de salida.";
END
STR
        ID = 138 ;
        VERSION = 1 ;
        STRING = "No se puede eximir billinfo porque es miembro del grupo collections_targets.";
        HELPSTR = "No se puede eximir billinfo porque es miembro del grupo collections_targets.";
END
STR
        ID = 139 ;
        VERSION = 1 ;
        STRING = "Existe relación cíclica entre principal y miembro.";
        HELPSTR = "Existe relación cíclica entre principal y miembro.";
END
STR
        ID = 140 ;
        VERSION = 1 ;
        STRING = "Nombre de grupo duplicado. No se puede cambiar el grupo.";
        HELPSTR = "Nombre de grupo duplicado. No se puede cambiar el grupo.";
END
STR
        ID = 141 ;
        VERSION = 1 ;
        STRING = "Billinfo es subordinado. No se puede agregar al grupo.";
        HELPSTR = "Billinfo es subordinado. No se puede agregar al grupo.";
END
STR
        ID = 142 ;
        VERSION = 1 ;
        STRING = "Billinfo ya exenta. No se puede volver a aplicar exención.";
        HELPSTR = "Billinfo ya exenta. No se puede volver a aplicar exención.";
END
STR
        ID = 143 ;
        VERSION = 1 ;
        STRING = "Billinfo ya es un miembro de grupo de cobros. No se puede añadir al grupo.";
        HELPSTR = "Billinfo ya es un miembro de grupo de cobros. No se puede añadir al grupo.";
END
STR
        ID = 144 ;
        VERSION = 1 ;
        STRING = "Billinfo ya es un principal de grupo de cobros. No se puede añadir al grupo.";
        HELPSTR = "Billinfo ya es un principal de grupo de cobros. No se puede añadir al grupo.";
END
STR
        ID = 145 ;
        VERSION = 1 ;
        STRING = "Billinfo ya está en cobros. No se puede modificar el grupo.";
        HELPSTR = "Billinfo ya está en cobros. No se puede modificar el grupo.";
END
STR
        ID = 146 ;
        VERSION = 1 ;
        STRING = "Para el tipo de pago SEPA, la moneda de la cuenta debe ser EURO.";
        HELPSTR = "Para el tipo de pago SEPA, la moneda de la cuenta debe ser EURO."; 
END
STR
        ID = 147 ;
        VERSION = 1 ;
        STRING = "La longitud del IBAN no cumple el estándar ISO 13616:2007.";
        HELPSTR = "La longitud del IBAN no cumple el estándar ISO 13616:2007.";
END
STR
        ID = 148 ;
        VERSION = 1 ;
        STRING = "El IBAN no cumple el estándar ISO 13616:2007.";
        HELPSTR = "El IBAN no cumple el estándar ISO 13616:2007.";
END
STR
        ID = 149 ;
        VERSION = 1 ;
        STRING = "El IBAN no cumple mod97.";
        HELPSTR = "El IBAN no cumple mod97.";
END
STR
        ID = 150 ;
        VERSION = 1 ;
        STRING = "La longitud del código BIC no cumple los estándares. Debe tener 8 u 11 caracteres.";
        HELPSTR = "La longitud del código BIC no cumple los estándares. Debe tener 8 u 11 caracteres.";
END
STR
        ID = 151 ;
        VERSION = 1 ;
        STRING = "El BIC no cumple el estándar ISO 9362:2009.";
        HELPSTR = "El BIC no cumple el estándar ISO 9362:2009.";
END
STR
        ID = 152 ;
        VERSION = 1 ;
        STRING = "Este UMR ya está presente en el sistema. Corrija este valor o déjelo vacío para que se pueda generar otro.";
        HELPSTR = "Este UMR ya está presente en el sistema. Corrija este valor o déjelo vacío para que se pueda generar otro.";
END
STR
        ID = 153 ;
        VERSION = 1 ;
        STRING = "MANDATE_SIGNED_T no puede ser futuro ni anterior a la fecha de creación.";
        HELPSTR = "MANDATE_SIGNED_T no puede ser futuro ni anterior a la fecha de creación.";
END
STR
        ID = 154 ;
        VERSION = 1 ;
        STRING = "No se encontró el ID de acreedor; el nombre de acreedor es el mismo.";
        HELPSTR = "No se encontró el ID de acreedor; el nombre de acreedor es el mismo.";
END
STR
        ID = 155 ;
        VERSION = 1 ;
        STRING = "El ID de acreedor no existe.";
        HELPSTR = "El ID de acreedor no existe.";
END
STR
        ID = 156 ;
        VERSION = 1 ;
        STRING = "La combinación de ID de acreedor/Nombre de acreedor no existe.";
        HELPSTR = "La combinación de ID de acreedor/Nombre de acreedor no existe.";
END
STR
        ID = 157 ;
        VERSION = 1 ;
        STRING = "Esta información de pago tiene una solicitud SEPA, por lo que no se puede suprimir.";
        HELPSTR = "Esta información de pago tiene una solicitud SEPA, por lo que no se puede suprimir.";
END
