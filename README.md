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

Constructor:
Lee los archivos enviados y verifica si estan vacios o no, con el fin de llenar la data del head en caso este vacio y si no utilizar la data rellenada.

Search:
Realiza busquedas por indice. Solo es capaz de retornar un archivo por busqueda; sin embargo, no quita que despues se le pueda implementar una busqueda por rango o por otra categoria. Para la busqueda por rango se recomienda añadir un atributo de punteros enlazados para despues de encontrar los archivos realizar una busqueda lineal. La complejidad de la función de busqueda es O(logN). Esto debido a que utliza el archivo ordenado y el indice para realizar una busqueda binaria, sin tomar en cuenta el archivo auxiliar ya que posee una cantidad fija de archivos convirtiendo su busqueda en una constante. Recomendaciones para manejar la busqueda es mantener la capacidad del archivo auxiliar la menor posible para evitar tener una busqueda costosa.

Insert:
El insert realiza un calculo de inserción sencillo, inserta la data en el archivo auxiliar y las añade al archivo de la data, cuando este llegue a su tope, manteniendo la data de dicho archivo ordenada. A su vez este detecta si existen archivos eliminados, que si existiesen en el archivo auxiliar lo sobreescribiria y si lo hubiese en el archivo ordenado lo llenaria solo en caso sea nesesario para no romper el orden. El costo de inserción es O(N) en el archivo ordenado devido a la reconstrucción y el costo del archivo auxiliar es constante debido a su tamaño fijo. Recomendación para el insert saber manejar la cantidad de registros que debe tener el auxiliar para evitar tener que reconstruir mucho y si es posible reconstruir en momentos especificos ya que con muchisima cantidad de registros la reconstrucción puede ser muy costosa.

Delete:
El delete es muy sencillo y solo requiere de por medio de search encontrar la posición del archivo y modificar el parametro index_delete el cual almacena la posición del siguiente eliminado, -1 si es el final de los eliminados y -2 si no ha sido eliminado (no se uso 0 debido a que 0 es una posción de un registro). El costo es O(1) debido a que solo cambia el puntero del head para que apunte al archivo nuevo eliminado y este apunte al que apuntaba head.



### ISAM
Para el ISAM se empleó el Dense Index File, también se requirió de 2 archivos, uno para la data sin orden u organización alguna, y el otro como indice que guarda la posición de los registros con su key.


##Resultados experimentales

###Sequential File

|        | P1 | P2 | P3 | P4 | P5 | P6 | P7 | P8 | P9 | P10 |
|--------|----|----|----|----|----|----|----|----|----|-----|
| Fill   |    |    |    |    |    |    |    |    |    |     |
| Insert |    |    |    |    |    |    |    |    |    |     |
| Search |    |    |    |    |    |    |    |    |    |     |
| Delete |    |    |    |    |    |    |    |    |    |     |


Promedio:
|        | Sequential File | ISAM |
|--------|-----------------|------|
| Fill   |                 |      |
| Insert |                 |      |
| Search |                 |      |
| Delete |                 |      |

## Data


