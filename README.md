# Biblioteca_DB
## Integrantes
- Marlon Mejia
- Luis Cordero
- Víctor Ascuña

## Introduccion
El objetivo de este proyecto es experimentar y comparar dos técnicas de indexación para bases de datos. Para ello se emplearán archivos .csv con gran cantidad de datos. Las técnicas a comparar en este experimento son: ISAM y el Sequential File, aunque ya se conoce que el ISAM es una de las mejores, queremos ver la magnitud de la mejora comparado con el Sequential File. Una de las razones por las que escogimos estas dos técnicas es que ya estaban avanzadas gracias a los laboratorios. Además ambos son eficientes en diferentes aspectos: el ISAM en cuanto a tiempo de ejecución de consultas, y el Sequential File para espacio de almacenamiento en disco.

## Tecnicas a usar
### Sequential File
Para el Sequential File empleamos 2 archivos, uno representa toda la data con ordenada y el otro que es un auxiliar con nueva data que también forma parte de la base de datos. Para dicho ordenamiento se requiere de una llave que es designada por un "id" que viene en la data en este caso. 

#### Constructor:
Lee los archivos enviados y verifica si están vacíos o no, con el fin de llenar la data del head en caso este vació y si no utilizar la data rellenada. 


#### Search:
Realiza búsquedas por indice. Solo es capaz de retornar un archivo por búsqueda; sin embargo, no quita que después se le pueda implementar una búsqueda por rango o por otra categoría. Para la búsqueda por rango se recomienda añadir un atributo de punteros enlazados para después de encontrar los archivos realizar una búsqueda lineal. La complejidad de la función de búsqueda es O(logN). Esto debido a que utiliza el archivo ordenado y el indice para realizar una búsqueda binaria, sin tomar en cuenta el archivo auxiliar ya que posee una cantidad fija de archivos convirtiendo su búsqueda en una constante. Recomendaciones para manejar la búsqueda es mantener la capacidad del archivo auxiliar la menor posible para evitar tener una búsqueda costosa. Realiza búsquedas por indice. Solo es capaz de retornar un archivo por búsqueda; sin embargo, no quita que después se le pueda implementar una búsqueda por rango o por otra categoría. Para la búsqueda por rango se recomienda añadir un atributo de punteros enlazados para después de encontrar los archivos realizar una búsqueda lineal. La complejidad de la función de búsqueda es O(logN). Esto debido a que utiliza el archivo ordenado y el ìndice para realizar una búsqueda binaria, sin tomar en cuenta el archivo auxiliar ya que posee una cantidad fija de archivos convirtiendo su búsqueda en una constante. Recomendaciones para manejar la búsqueda es mantener la capacidad del archivo auxiliar la menor posible para evitar tener una búsqueda costosa


#### Insert:
El insert realiza un calculo de inserción sencillo, inserta la data en el archivo auxiliar y las añade al archivo de la data, cuando este llegue a su tope, manteniendo la data de dicho archivo ordenada. A su vez este detecta si existen archivos eliminados, que si existiesen en el archivo auxiliar lo sobreescribirìa y si lo hubiese en el archivo ordenado lo llenaría solo en caso sea necesario para no romper el orden. El costo de inserción es O(N) en el archivo ordenado debido a la reconstrucción y el costo del archivo auxiliar es constante debido a su tamaño fijo. Recomendación para el insert saber manejar la cantidad de registros que debe tener el auxiliar para evitar tener que reconstruir mucho y si es posible reconstruir en momentos específicos ya que con muchísima cantidad de registros la reconstrucción puede ser muy costosa.	El insert realiza un calculo de inserción sencillo, inserta la data en el archivo auxiliar y las añade al archivo de la data, cuando este llegue a su tope, manteniendo la data de dicho archivo ordenada. A su vez este detecta si existen archivos eliminados, que si existiesen en el archivo auxiliar lo sobreescribiria y si lo hubiese en el archivo ordenado lo llenaría solo en caso sea necesario para no romper el orden. El costo de inserción es O(N) en el archivo ordenado debido a la reconstrucción y el costo del archivo auxiliar es constante debido a su tamaño fijo. Recomendación para el insert saber manejar la cantidad de registros que debe tener el auxiliar para evitar tener que reconstruir mucho y si es posible reconstruir en momentos específicos ya que con muchísima cantidad de registros la reconstrucción puede ser muy costosa.


#### Delete:
El delete es muy sencillo y solo requiere de por medio de search encontrar la posición del archivo y modificar el parámetro index_delete el cual almacena la posición del siguiente eliminado, -1 si es el final de los eliminados y -2 si no ha sido eliminado (no se uso 0 debido a que 0 es una posición de un registro). El costo es O(1) debido a que solo cambia el puntero del head para que apunte al archivo nuevo eliminado y este apunte al que apuntaba head.	El delete es muy sencillo y solo requiere de por medio de search encontrar la posición del archivo y modificar el parámetro index_delete el cual almacena la posición del siguiente eliminado, -1 si es el final de los eliminados y -2 si no ha sido eliminado (no se uso 0 debido a que 0 es una posición de un registro). El costo es O(1) debido a que solo cambia el puntero del head para que apunte al archivo nuevo eliminado y este apunte al que apuntaba head.

### ISAM
Para el ISAM se empleó el Dense Index File, también se requirió de 2 archivos, uno para la data sin orden u organización alguna, y el otro como indice que guarda la posición de los registros con su key.

#### Constructor:
Lee los archivos enviados y verifica si están vacíos o no, con el fin de llenar la data del head en caso este vació y si no utilizar la data rellenada.

#### Search: 
La búsqueda es por indice y no permite búsqueda por rango. Cambien cabe mencionar al igual que el anterior la búsqueda por rango se puede implementar con un puntero que apunte al siguiente ordenado o simplemente recorriendo el arreglo de ìndices. La complejidad es O(logN) realizando la búsqueda en el archivo indexado para luego llamar al registro en el archivo con la data.dat. Mencionar que al presentar la misma complejidad que la busqueda de un sequential file en algunas pruebas experimentales se registraran tiempos de ejecucion similares incluso un poco mayor en comparacion con la anterior tecnica puesto que este siempre tendrà una busqueda binaria.

#### Insert:
La inserción es O(1) para debido a que siempre se inserta al final y el archivo con los indices se ordena en memoria. Comentar que el manejo de el ordenamiento en memoria se debe controlar ya que si llega al momento donde el archivo de indices llega a ser demasiado grande no se podrá seguir ordenando en memoria y se tendrá que ordenar por acceso a disco lo cual podría costar O(N). Mencionar que para el ordenamiento en memoria se usa insertion sort(O(N^2)).

#### Delete:
El delete es simple y por medio de search encuentra el registro marca el indice apuntando a nada y el registro se marca como eliminado. Los archivos eliminados siguen apuntado parecido al anterior el head apunta al registro eliminado y el registro eliminado apunta al que apuntaba el head.

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

### Promedio:
|        | Sequential File | ISAM |
|--------|-----------------|------|
| Fill   |     730ms            |  721ms    |
| Insert |     804ms            |  666ms    |
| Search |     690ms            |  769ms    |
| Delete |     839ms            |  804ms    |  

## Transacciones (Thread)
Se hicieron pruebas con hilos para simular transacciones. Se realizaron las siguientes pruebas para cada thread: Insert e Insert, Search y Search, Delete y Delete, Insert y Search, Insert y Delete, Search y Delete. Los resultados mostraron que existe sobrescritura al momento del doble Insert, esto se soluciona con un bloqueo cuando se llama a insert para evitar que se sobrescriba en la misma posición. No existe problemas con la búsqueda en simultaneo debido a que solo realizan lectura. Tampoco existe problemas con el doble Delete ya que realizan la misma función de marcar como eliminado. El Insert y Search generan problemas de leer algo que aun no ha sido insertado, esto en principio no debería ser un problema, pero en tal caso sea nesesario encontrar dicho registro simplemente se puede volver a llamar a la función hasta que se inserte el registro. El Insert y Delete no generan mucho problema, pero si existe una eliminación e inserción del mismo archivo puede generar que inserte el registro eliminado o elimine el registro recien insertado; se puede solucionar con exclusión mutua igual que con el doble Insert. El Search y Delete generan problemas de leer un archivo eliminado, para solucionarlo igual que en los anteriores realizar exclusión mutua. Cabe mencionar que en clase vimos muchísimos más errores que se pueden generar por transacciones, pero lo podemos solucionar con mecanismos como el PS, el PX y el PU.

## Discución
### Diferencias
El Sequential File es más rapido en sus busquedas debido a que puede realizar una busqueda lineal constante (en otras palabras usa busqueda secuencial, pero que su costo maximo es fijo y no varia con el tiempo). Sin embargo, la busqueda del ISAM puede ser ejecutada desde memoria para disminuir sus costos de acceso a memoria. La inserción del ISAM es muchisimo más rapida porque (en nuestro caso) realiza el ordenamiento de los indices desde memoria. Por otro lado la inserción del Sequential File es más lenta debido a sus reconstrucciones, estas reconstrucciones se pueden controlar si se hacen en un horario o momento donde el acceso a la base de datos no sea nesesario se puede controlar. 

### Ventajas
El Sequential File es capaz de realizar busquedas en O(logN) y si busca algun registro que este en el archivo auxliar su busqueda sera mucho más rapida. Además su inserción es bastante rapida exeptuando cuando debe reconstruir. El ISAM posee los indices separados y ordenados, lo cual te permite realizar busquedas por más de un indice o llave principal. ISAM te permite realizar busquedas (en nuestro caso no incluiria, pero es posible) y ordenar la lista de indices durante la inserción en memoria.

### Desventajas
El Sequential File si no se controla puede llegar a generar reconstrucciones que sobrecarguen la base de datos. Además de poseer tiempos de complejidad O(logN) en disco que son muy lentos a comparación de las tecnicas hash y b+tree. El ISAM puede volverse un problema con bastantes registros, ya que sus archivos de indices podrian llegar a pesar más que el espacio disponible en memoria. Convirtiendo su busqueda en O(logN) siendo igual que la del Sequential File.

### Tipos de Datos
A partir de la discusión definimos que el Sequential File trabaja más comodo con muchisimos registros y pocos atributos, disminuyendo el espacio del arreglo auxiliar y que utilice principalmente haga busquedas de inserciones recientes. Por otro lado el ISAM se ve más beneficiado con multiples atributos y menor cantidad de registros (dependiendo del espacio de ram de la maquina); ya que pueden realizar busquedas en memoria de manera más eficiente.
