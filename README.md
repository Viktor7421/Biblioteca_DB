# Biblioteca_DB
## Integrantes
- Marlon Mejia
- Luis Cordero
- Víctor Ascuña

## Introduccion
El objetivo de este proyecto es experimentar y comparar dos tecnicas de indexación para bases de datos. Para ello se emplearán archivos .csv con gran cantidad de datos. Las técnicas a comparar en este experimento son: ISAM y el Sequential File, aunque ya se conoce que el ISAM es una de las mejores, queremos ver la magnitud de la mejora comparado con el Sequential File. Una de las razones por las que escogimos estas dos tecnicas es que ya estaban avanzadas gracias a los laboratorios. Además ambos son eficientes en diferentes aspectos: el ISAM en cuanto a tiempo de ejecución de consultas, y el Sequential File para espacio de almacenamiento en disco.

## Tecnicas a usar
### Sequential File
Para el Sequential File empleamos 2 archivos, uno representa toda la data con ordenada y el otro que es un auxiliar con nueva data que tambien forma parte de la base de datos. Para dicho ordenamiento se requiere de una llave que es designada por un "id" que viene en la data en este caso. 

#### Constructor:
Lee los archivos enviados y verifica si estan vacios o no, con el fin de llenar la data del head en caso este vacio y si no utilizar la data rellenada.	Lee los archivos enviados y verifica si estan vacios o no, con el fin de llenar la data del head en caso este vacio y si no utilizar la data rellenada.


#### Search:
Realiza busquedas por indice. Solo es capaz de retornar un archivo por busqueda; sin embargo, no quita que despues se le pueda implementar una busqueda por rango o por otra categoria. Para la busqueda por rango se recomienda añadir un atributo de punteros enlazados para despues de encontrar los archivos realizar una busqueda lineal. La complejidad de la función de busqueda es O(logN). Esto debido a que utliza el archivo ordenado y el indice para realizar una busqueda binaria, sin tomar en cuenta el archivo auxiliar ya que posee una cantidad fija de archivos convirtiendo su busqueda en una constante. Recomendaciones para manejar la busqueda es mantener la capacidad del archivo auxiliar la menor posible para evitar tener una busqueda costosa.	Realiza busquedas por indice. Solo es capaz de retornar un archivo por busqueda; sin embargo, no quita que despues se le pueda implementar una busqueda por rango o por otra categoria. Para la busqueda por rango se recomienda añadir un atributo de punteros enlazados para despues de encontrar los archivos realizar una busqueda lineal. La complejidad de la función de busqueda es O(logN). Esto debido a que utliza el archivo ordenado y el indice para realizar una busqueda binaria, sin tomar en cuenta el archivo auxiliar ya que posee una cantidad fija de archivos convirtiendo su busqueda en una constante. Recomendaciones para manejar la busqueda es mantener la capacidad del archivo auxiliar la menor posible para evitar tener una busqueda costosa.


#### Insert:
El insert realiza un calculo de inserción sencillo, inserta la data en el archivo auxiliar y las añade al archivo de la data, cuando este llegue a su tope, manteniendo la data de dicho archivo ordenada. A su vez este detecta si existen archivos eliminados, que si existiesen en el archivo auxiliar lo sobreescribiria y si lo hubiese en el archivo ordenado lo llenaria solo en caso sea nesesario para no romper el orden. El costo de inserción es O(N) en el archivo ordenado devido a la reconstrucción y el costo del archivo auxiliar es constante debido a su tamaño fijo. Recomendación para el insert saber manejar la cantidad de registros que debe tener el auxiliar para evitar tener que reconstruir mucho y si es posible reconstruir en momentos especificos ya que con muchisima cantidad de registros la reconstrucción puede ser muy costosa.	El insert realiza un calculo de inserción sencillo, inserta la data en el archivo auxiliar y las añade al archivo de la data, cuando este llegue a su tope, manteniendo la data de dicho archivo ordenada. A su vez este detecta si existen archivos eliminados, que si existiesen en el archivo auxiliar lo sobreescribiria y si lo hubiese en el archivo ordenado lo llenaria solo en caso sea nesesario para no romper el orden. El costo de inserción es O(N) en el archivo ordenado devido a la reconstrucción y el costo del archivo auxiliar es constante debido a su tamaño fijo. Recomendación para el insert saber manejar la cantidad de registros que debe tener el auxiliar para evitar tener que reconstruir mucho y si es posible reconstruir en momentos especificos ya que con muchisima cantidad de registros la reconstrucción puede ser muy costosa.


#### Delete:
El delete es muy sencillo y solo requiere de por medio de search encontrar la posición del archivo y modificar el parametro index_delete el cual almacena la posición del siguiente eliminado, -1 si es el final de los eliminados y -2 si no ha sido eliminado (no se uso 0 debido a que 0 es una posción de un registro). El costo es O(1) debido a que solo cambia el puntero del head para que apunte al archivo nuevo eliminado y este apunte al que apuntaba head.	El delete es muy sencillo y solo requiere de por medio de search encontrar la posición del archivo y modificar el parametro index_delete el cual almacena la posición del siguiente eliminado, -1 si es el final de los eliminados y -2 si no ha sido eliminado (no se uso 0 debido a que 0 es una posción de un registro). El costo es O(1) debido a que solo cambia el puntero del head para que apunte al archivo nuevo eliminado y este apunte al que apuntaba head.

### ISAM
Para el ISAM se empleó el Dense Index File, también se requirió de 2 archivos, uno para la data sin orden u organización alguna, y el otro como indice que guarda la posición de los registros con su key.

#### Constructor:
Lee los archivos enviados y verifica si estan vacios o no, con el fin de llenar la data del head en caso este vacio y si no utilizar la data rellenada.

#### Search:
La busqueda es por indice y no permite busqueda por rango. Tambien cabe mencionar al igual que el anterior la busqueda por rango se puede implementar con un puntero que apunte al siguiente ordenado o simplemente recorriendo el arreglo de indices. La complejidad es O(logN) realizando la busqueda en el archivo indexado para luego llamar al registro en el archivo con la data.

#### Insert:
La inserción es O(1) para debido a que siempre se inserta al final y el archivo con los indices se ordena en memoria. Comentar que el manejo de el ordenamiento en memoria se debe controlar ya que si llega al momento donde el archivo de indices llega a ser demasiado grande no se podra seguir ordenando en memoria y se tendra que ordenar por acceso a disco lo cual podria costar O(N). Mencionar que para el ordenamiento en memoria se usa insertion sort(O(N^2)).

#### Delete:
El delete es simple y por medio de search encuentra el registro marca el indice apuntando a nada y el registro se marca como elimindo. Los archivos eliminados siguen apuntado parecido al anterior el head apunta al registro eliminado y el registro eliminado apunta al que apuntaba el head.

## Resultados experimentales

### Sequential File

|        | P1 | P2 | P3 | P4 | P5 | P6 | P7 | P8 | P9 | P10 |
|--------|----|----|----|----|----|----|----|----|----|-----|
| Fill   |690ms|788ms|687ms|843ms|702ms|687ms|790ms|692ms|713ms|706ms|
| Insert |881ms|831ms|798ms|749ms|749ms|782ms|797ms|826ms|863ms|773ms|
| Search |749ms|689ms|691ms|668ms|701ms|670ms|671ms|700ms|695ms|664ms|
| Delete |825ms|814ms|840ms|864ms|834ms|806ms|845ms|861ms|828ms|811ms|

### Dense Index File

|        | P1 | P2 | P3 | P4 | P5 | P6 | P7 | P8 | P9 | P10 |
|--------|----|----|----|----|----|----|----|----|----|-----|
| Fill   |797ms|808ms|719ms|696ms|716ms|671ms|706ms|689ms|732ms|676ms|
| Insert |644ms|680ms|671ms|680ms|611ms|683ms|690ms|684ms|670ms|647ms|
| Search |748ms|765ms|821ms|729ms|768ms|822ms|764ms|750ms|742ms|783ms|
| Delete |812ms|873ms|811ms|823ms|748ms|835ms|778ms|803ms|759ms|794ms|

Promedio:
|        | Sequential File | ISAM |
|--------|-----------------|------|
| Fill   |     730ms            |  721ms    |
| Insert |     804ms            |  666ms    |
| Search |     690ms            |  769ms    |
| Delete |     839ms            |  804ms    |  

## Transacciones (Thread)
Se hicieron pruebas con hilos para simular transacciones. Se realizaron las siguientes pruebas para cada thread: Insert e Insert, Search y Search, Delte y Delete, Insert y Search, Insert y Delete, Search y Delete. Los resultados mostraron que existe sobreescritura al momento del doble Insert, esto se soluciona con un bloqueo cuando se llama a insert para evitar que se sobreescriba en la misma posición. No existe problemas con la busqueda en simultaneo debido a que solo realizan lectura. Tampoco exite problemas con el doble Delete ya que realizan la misma función de marcar como eliminado. El Insert y Search generan problemas de leer algo que aun no ha sido insertado, esto en principio no deberia ser un problema, pero en tal caso sea nesesario encontrar dicho registro simplemente se puede volver a llamar a la función hasta que se inserte el registro. El Insert y Delete no generan mucho problema, pero si existe una eliminación e inserción del mismo archivo puede generar que inserte el registro eliminado o elimine el registro recien insertado; se puede solucionar con exclusión mutua igual que con el doble Insert. El Search y Delete generan problemas de leer un archivo eliminado, para solucionarlo igual que en los anteriores realizar exclusión mutua. Cabe mencionar que en clase vimos muchisimos más errores que se pueden generar por transacciones, pero lo podemos solucionar con mecanismos como el PS, el PX y el PU.

## Data


666ms
