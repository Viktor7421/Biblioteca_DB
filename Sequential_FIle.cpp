#include <iostream>
#include <vector>
#include <queue>
#include <fstream>

// Strucs

struct Matricula
{
    char codigo[5];
    int ciclo;
    float mensualidad;
};

std::istream & operator >> (std::istream & stream, Matricula & record)
{
    stream.read(record.codigo, 5);
    stream >> record.ciclo;
    stream >> record.mensualidad;
    stream.get();
    return stream;
}

std::ostream & operator << (std::ostream & stream, Matricula & record)
{
    stream.write(record.codigo, 5);
    stream << record.ciclo;
    stream << record.mensualidad;
    stream << std::flush;
    return stream;
}

// Classes

class FixedRecord 
{
private:
    std::string name;
    std::queue<int> freelist;
public:
    FixedRecord(std::string _name);
    std::vector<Matricula> load();
    void add(Matricula record);
    bool _delete(int pos);
};

// Main

int main() 
{
    FixedRecord Files("datos2.dat");
    std::vector<Matricula> records = Files.load();
    for (auto it = records.begin(); it != records.end(); it++)
    {
        std::cout << (*it).codigo << "\t" << (*it).ciclo << "\t" << (*it).mensualidad << "\n";
    }
    Files._delete(5);
    Matricula record = {"0007",1000,10.5};
    Files.add(record);
    return 0;
}

// Functions

FixedRecord::FixedRecord(std::string _name)
{
    name = _name;
}

std::vector<Matricula> FixedRecord::load()
{
    std::vector<Matricula> result;
    Matricula record;

    std::ifstream stream(name, std::ios_base::binary);
    while (!stream.eof()) 
    {
        stream.read((char*)&record, sizeof(Matricula));
        stream.get();
        result.push_back(record);
    }
    stream.close();
    result.pop_back();
    return result;
}

void FixedRecord::add(Matricula record)
{
    if (freelist.empty())
    {
        std::ofstream stream(name, std::ios_base::binary | std::ios::app);
        stream.write((char*)&record, sizeof(Matricula));
        stream << "\n";
        stream.close();
    } else {
        std::ofstream stream(name, std::ios::in | std::ios_base::binary);
        std::cout << sizeof(Matricula) << "\n";
        stream.seekp((freelist.front()) * (sizeof(Matricula)+1), std::ios::beg);
        stream.write((char*)&record, sizeof(Matricula));
        stream.close();
        freelist.pop();
    }
}

bool FixedRecord::_delete(int pos)
{
    freelist.push(pos-1);
}