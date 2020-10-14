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


