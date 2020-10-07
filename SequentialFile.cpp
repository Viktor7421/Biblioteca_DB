#include <iostream>
#include <fstream>
#include <vector>
#include <iostream>
#include <string>
#include <cstring>

struct Registro
{
    char codigo [5];
    char nombre [20];
    char carrera [15];
    int ciclo;
};

std::istream & operator >> (std::istream & stream, Registro & record)
{
    stream.read(record.codigo, 5);
    stream.read(record.nombre, 20);
    stream.read(record.carrera, 15);
    stream.read((char*)&record.ciclo, sizeof(int));
    stream.get();
    return stream;
}

std::ostream & operator << (std::ostream & stream, Registro & record)
{
    stream.write(record.codigo, 5);
    stream.write(record.nombre, 20);
    stream.write(record.carrera, 15);
    stream << record.ciclo;
    stream << std::flush;
    return stream;
}

void insertionSortNombre(std::vector<Registro> &registros)  
{
    int i, j;
    Registro key;
    for (i = 1; i < registros.size(); i++) 
    {  
        key = registros[i];  
        j = i - 1;  

        while (j >= 0 &&  0 < std::strcmp(registros[j].nombre, key.nombre)) 
        {  
            registros[j + 1] = registros[j];  
            j = j - 1;  
        }  
        registros[j + 1] = key;  
    }
}

class SequentialFile
{
private:
    std::string name;
public:
    SequentialFile(std::string _name)
    {
        name = _name;
    }
    void insertAll(std::vector<Registro> registros)
    {
        insertionSortNombre(registros);
        std::ofstream stream(name, std::ios::binary);
        for(auto it = registros.begin(); it != registros.end(); it++)
        {
            stream.write ((char*)&(*it), sizeof (Registro));
            stream << "\n";
        }
        stream.close();
    }
    Registro search(std::string key)
    {
        Registro temp;
        if(key.length() <= 20)
        {
            Registro record;
            for(int i = 0; i < 20;i++)
            {
                if(i < key.length())
                {
                    record.nombre[i] = key[i];
                }
                else
                {
                    record.nombre[i] = ' ';
                }
            }
            std::ifstream stream(name, std::ios::binary);
            while(!stream.eof())
            {
                stream.read((char*)&temp, sizeof(Registro));
                stream.get();
                Registro record2;
                for(int i = 0; i < 20;i++)
                {
                    record2.nombre[i] = temp.nombre[i];
                }
                if(strcmp(record.nombre, record2.nombre) == 0)
                    {break;}
            }
            stream.close();
        }
        return temp;
    }

    std::vector<Registro> search(std::string begin, std::string end)
    {
        std::vector<Registro> result;
        Registro temp;
        if(begin.length() <= 20 && end.length() <= 20)
        {
            Registro _begin;
            Registro _end;
            for(int i = 0; i < 20;i++)
            {
                if(i < begin.length())
                {
                    _begin.nombre[i] = begin[i];
                }
                else
                {
                    _begin.nombre[i] = ' ';
                }
                if(i < end.length())
                {
                    _end.nombre[i] = end[i];
                }
                else
                {
                    _end.nombre[i] = ' ';
                }
            }
            std::ifstream stream(name, std::ios::binary);
            while(!stream.eof())
            {
                stream.read((char*)&temp, sizeof(Registro));
                stream.get();
                Registro record;
                for(int i = 0; i < 20;i++)
                {
                    record.nombre[i] = temp.nombre[i];
                }
                if(strcmp(_begin.nombre, record.nombre) == 0)
                {
                    result.push_back(temp);
                    while(!stream.eof())
                    {
                        stream.read((char*)&temp, sizeof(Registro));
                        stream.get();
                        for(int i = 0; i < 20;i++)
                        {
                            record.nombre[i] = temp.nombre[i];
                        }
                        result.push_back(temp);
                        if(strcmp(_end.nombre, record.nombre) == 0)
                            {break;}
                    }
                    break;
                }
            }
            stream.close();
        }
        return result;
    }

    void add(Registro registro)
    {
        std::string _name = "datosnuevo.dat";
        std::ifstream stream(_name, std::ios::binary);
        int lineas = 0;
        std::string _string;
        while(!stream.eof())
        {
            lineas++;
            std::getline(stream, _string);
        }
        stream.close();
        if(lineas <= 5)
        {
            std::ofstream stream(_name, std::ios::app | std::ios::binary);
            stream.write ((char*)&registro, sizeof (Registro));
            stream << "\n";
            stream.close();
        } 
        else 
        {
            std::vector<Registro> registros;
            std::ifstream stream(_name, std::ios::binary);
            Registro _registro;
            while(!stream.eof())
            {
                stream.read((char*)&_registro, sizeof(Registro));
                stream.get();
                registros.push_back(_registro);
            }
            registros.pop_back();
            stream.close();
            registros.push_back(registro);
            insertionSortNombre(registros);
            std::ofstream ostream(name, std::ios::in | std::ios::binary);
            std::ifstream istream(name, std::ios::binary);
            
            while(!istream.eof())
            {
                istream.read((char*)&_registro, sizeof(Registro));
                istream.get();
                if(0 > std::strcmp(registros[0].nombre, _registro.nombre))
                {
                    ostream.write((char*)&registros[0], sizeof(Registro));
                    ostream << "\n";
                    registros[0] = _registro;
                    insertionSortNombre(registros);
                } 
                else 
                {
                    ostream.seekp(sizeof(Registro)+1, std::ios::cur);
                }
            }

            registros.pop_back();
            for(auto it = registros.begin(); it != registros.end(); it++)
            {
                ostream.write((char*)&(*it), sizeof(Registro));
                ostream << "\n";
            }
            ostream.close();
            std::ofstream update(_name);
            update.clear();
            update.close();
        }
    }
};

Registro creacionRegistro(std::string codigo, std::string nombre, std::string carrera, int ciclo)
{
    Registro record;
    for(int i = 0; i < 5; i++)
    {
        if(i < codigo.length())
        {
            record.codigo[i] = codigo[i];
        }
        else
        {
            record.codigo[i] = ' ';
        }
    }
    for(int i = 0; i < 20; i++)
    {
        if(i < nombre.length())
        {
            record.nombre[i] = nombre[i];
        }
        else
        {
            record.nombre[i] = ' ';
        }
    }
    for(int i = 0; i < 15; i++)
    {
        if(i < carrera.length())
        {
            record.carrera[i] = carrera[i];
        }
        else
        {
            record.carrera[i] = ' ';
        }
    }
    record.ciclo = ciclo;
    return record;
}


int main()
{
    SequentialFile datafile("datos1.dat");
    
    std::vector<Registro> registros;
    registros.push_back(creacionRegistro("0001","Victor","CS",6));
    registros.push_back(creacionRegistro("0002","Angelo","Civil",1));
    registros.push_back(creacionRegistro("0003","Juan","Electronica",8));
    registros.push_back(creacionRegistro("0004","Alberto","Matematica",5));
    registros.push_back(creacionRegistro("0005","Luis","Bioingenieria",2));
    registros.push_back(creacionRegistro("0006","Nicolas","Administracion",3));
    registros.push_back(creacionRegistro("0007","Alejandro","Software",2));
    registros.push_back(creacionRegistro("0008","Rodrigo","Sistemas",1));
    registros.push_back(creacionRegistro("0009","Matias","Periodismo",4));
    registros.push_back(creacionRegistro("0010","Jose","Musica",6));
    datafile.insertAll(registros);
    
    Registro search1 = datafile.search("Juan");
    std::cout << search1 << "\n";
    
    std::vector<Registro> search2 = datafile.search("Angelo","Luis");
    for(auto it = search2.begin(); it != search2.end(); it++)
        std::cout << *it << "\n";
    datafile.add(creacionRegistro("0011","Frank","Negocios",5));
    datafile.add(creacionRegistro("0012","Gustavo","Dentista",2));
    datafile.add(creacionRegistro("0013","Josepth","Administracion",4));
    datafile.add(creacionRegistro("0014","Pedro","Turismo",7));
    datafile.add(creacionRegistro("0015","Sebastian","Ambiental",5));
    datafile.add(creacionRegistro("0016","Fabrizzio","Negocios Inter",1));
    return 0;
}