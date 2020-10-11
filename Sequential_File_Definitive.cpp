#include <iostream>
#include <fstream>
#include <vector>
#include <iostream>
#include <string>
#include <cstring>

#define index_T char*

struct Record {
    char index [2];
    char genus [30];
    char species [20];
    char taxa [20];
    int index_next;
    int index_delete;
};

std::istream & operator >> (std::istream & stream, Record & record) {
    stream.read(record.index, 2);
    stream.read(record.genus, 30);
    stream.read(record.species, 20);
    stream.read(record.taxa, 20);
    stream.get();
    return stream;
}

std::ostream & operator << (std::ostream & stream, Record & record) {
    stream.write(record.index, 2);
    stream.write(record.genus, 30);
    stream.write(record.species, 20);
    stream.write(record.taxa, 20);
    stream << std::flush;
    return stream;
}

struct Data
{
    std::string name;
    int size;
    int index_next;
    int index_delete;
    bool empty() {
        std::fstream stream(name, std::ios::binary | std::ios::in);
        int n = -1;
        stream.seekg(0, stream.beg);
        stream.read((char*)&n, sizeof(int));
        stream.close();
        if(n == -1) return 1;
        else return 0;
    }
};


class Sequential_File_Definitve {
private:
    Data data;
    Data new_data;
public:
    Sequential_File_Definitve(std::string _data, std::string _new_data);
    ~Sequential_File_Definitve();
    
    Record search(index_T key);

    int searchDataPos(index_T key);

    int searchNewDataPos(index_T key);

    void insert(Record record);

    void _delete(index_T key);

    void rebuild();

    Record readRecord(Data _data,  int pos);
    void writeRecord(Data data, int pos, Record record);
    void fillNewFile(Data _data);
    int readSize(Data _data);
    int readIndexNext(Data _data);
    int readIndexDelete(Data _data);
    void updateIndexDelete(Data &_data, int pos, Record record);
    void updateDataSize();
    void updateNewDataSize();

    void insertionSort(std::vector<Record> &records);
};

Record creacionRegistro(std::string species_id, std::string genus, std::string species, std::string taxa) {
    Record record;
    for(int i = 0; i < 2; i++) {
        if(i < species_id.length()) {
            record.index[i] = species_id[i];
        } else {
            record.index[i] = ' ';
        }
    }
    for(int i = 0; i < 30; i++) {
        if(i < genus.length()) {
            record.genus[i] = genus[i];
        } else {
            record.genus[i] = ' ';
        }
    }
    for(int i = 0; i < 20; i++) {
        if(i < species.length()) {
            record.species[i] = species[i];
        } else {
            record.species[i] = ' ';
        }
    }
    for(int i = 0; i < 20; i++) {
        if(i < taxa.length()) {
            record.taxa[i] = taxa[i];
        } else {
            record.taxa[i] = ' ';
        }
    }
    return record;
}

int main()  {
    Sequential_File_Definitve DataBase("data_SF.dat","new_data_SF.dat");
    Record ave_1 = creacionRegistro("HB","Amphispiza","bilineata","Bird");
    DataBase.insert(ave_1);
    Record ave_2 = DataBase.search(ave_1.index);
    std::cout << ave_2;
    return 1;
}

Sequential_File_Definitve::Sequential_File_Definitve(std::string _data, std::string _new_data) {
    data.name = _data;
    if(data.empty()) {
        fillNewFile(data);
    }
    data.size = readSize(data); //encontrar el size en el archivo
    data.index_next = readIndexNext(data); //encontrar el next en el archivo
    data.index_delete = readIndexDelete(data); //encontrar el delete en el archivo

    new_data.name = _new_data;
    if(new_data.empty()) {        
        fillNewFile(new_data);
    }
    new_data.size = readSize(new_data); //encontrar el size en el archivo
    new_data.index_next = readIndexNext(new_data); //encontrar el next en el archivo
    new_data.index_delete = readIndexDelete(new_data); //encontrar el delete en el archivo
}

Sequential_File_Definitve::~Sequential_File_Definitve() {}

Record Sequential_File_Definitve::search(index_T key) {
    Record record;
    for(int i = 0; i < new_data.size; i+=sizeof(Record)) {
        record = readRecord(new_data,i);
        if(record.index == key && record.index_delete == 0) return record;
    }
    if(data.index_next != -1) {
        int l = 0;
        int u = data.size-sizeof(Record);
        while (u >= l) {
            int m = (l+u)/2;
            record = readRecord(data, m); // dilema de escoger un archivo eliminado 
            for(int i = 0; record.index_delete != 0; i++) {
                Record record_temp = readRecord(data, m+i*sizeof(Record)); // Evalua no eliminado hacia abajo
                if(record_temp.index_delete != 0) {record = record_temp; m=m+i*sizeof(Record); break;}
                
                record_temp = readRecord(data, m-i*sizeof(Record)); // Evalua no eliminado hacia arriba
                if(record_temp.index_delete != 0) {record = record_temp; m=m-i*sizeof(Record); break;} 
            }
            
            if (0 < std::strcmp(record.index, key)) u = m-sizeof(Record); // cambiar record.index por el indice
            else if (0 > std::strcmp(record.index, key)) l = m+sizeof(Record);
            else return record;
        }
    }
    return record;
}

int Sequential_File_Definitve::searchDataPos(index_T key) {
    if(data.index_next != -1) {
        
        int l = 0;
        int u = data.size-sizeof(Record);
        while (u >= l) {
            int m = (l+u)/2;
            Record record = readRecord(data, m); // dilema de escoger un archivo eliminado 
            for(int i = 0; record.index_delete != 0; i++) {
                Record record_temp = readRecord(data, m+i*sizeof(Record)); // Evalua no eliminado hacia abajo
                if(record_temp.index_delete != 0) {record = record_temp; m=m+i*sizeof(Record); break;}
                
                record_temp = readRecord(data, m-i*sizeof(Record)); // Evalua no eliminado hacia arriba
                if(record_temp.index_delete != 0) {record = record_temp; m=m-i*sizeof(Record); break;} 
            }
            if (0 < std::strcmp(record.index, key)) u = m-sizeof(Record); // cambiar record.index por el indice
            else if (0 > std::strcmp(record.index, key)) l = m+sizeof(Record);
            else return m;
        }
    }
    return -1;
}

int Sequential_File_Definitve::searchNewDataPos(index_T key) {
    for(int i = 0; i < new_data.size; i+=sizeof(Record)) {
        Record record = readRecord(new_data,i);
        std::cout << record.index << ' ' << key << '\n';
        if(record.index[0] == key[0] && record.index[1] == key[1] && record.index_delete == 0) return i;
    }
    return -1;
}

void Sequential_File_Definitve::insert(Record record) {
    int pos1 = searchNewDataPos(record.index);
    int pos2 = searchDataPos(record.index);
    if(pos1 != -1 || pos2 != -1) {std::cout << "Ese archivo ya existe.\n"; return;}
    if(new_data.size < 5*sizeof(Record)) {
        record.index_delete = 0;
        updateNewDataSize();     
        writeRecord(new_data,new_data.size-sizeof(Record),record);

    } else {
        rebuild();
    }
}

void Sequential_File_Definitve::_delete(index_T key) {
    int pos = searchNewDataPos(key);
    updateDataSize(); 
    if(pos != -1) {
        Record record = readRecord(new_data, pos);
        updateIndexDelete(new_data, pos, record);
    }
    pos = searchDataPos(key);
    if(pos != -1) {
        Record record = readRecord(data, pos);
        updateIndexDelete(data, pos, record);
    }
}

void Sequential_File_Definitve::rebuild() {
    std::vector<Record> new_records;
    for(int i = 0; i < new_data.size; i+=sizeof(Record)) {
        Record record = readRecord(new_data,i);
        new_records.push_back(record);
    }
    for(int i = 0; i < data.size; i+=sizeof(Record)) {
        Record record = readRecord(data,i);
        if(record.index > new_records[0].index) {
            writeRecord(data,i,new_records[0]);
            new_records[0] = record;
            insertionSort(new_records);
        }
    }
}

Record Sequential_File_Definitve::readRecord(Data _data, int pos) {
    Record record;
    std::fstream stream(_data.name, std::ios::in | std::ios::binary);
    stream.seekg(3*sizeof(int)+pos, stream.beg);
    stream.read((char*)&record, sizeof(Record));
    stream.close();
    return record;
}

void Sequential_File_Definitve::writeRecord(Data _data, int pos, Record record) {
    std::fstream stream(_data.name, std::ios::in | std::ios::out | std::ios::binary);
    stream.seekp(3*sizeof(int)+pos, stream.beg);
    stream.write((char*)&record, sizeof(Record));
    data.size += sizeof(Record);
    stream.close();
}

void Sequential_File_Definitve::fillNewFile(Data _data) {
    _data.size = 0;
    _data.index_next = -1;
    _data.index_delete = -1;
    std::fstream stream(_data.name, std::ios::in | std::ios::out | std::ios::binary);
    stream.seekp(0, stream.beg);
    stream.write((char*)&_data.size, sizeof(int));
    stream.write((char*)&_data.index_next, sizeof(int));
    stream.write((char*)&_data.index_delete, sizeof(int));
    stream.close();
}

int Sequential_File_Definitve::readSize(Data _data) {
    int size;
    std::fstream stream(_data.name, std::ios::in | std::ios::binary);
    stream.seekg(0, stream.beg);
    stream.read((char*)&size, sizeof(int));
    stream.close();
    return size;
}

int Sequential_File_Definitve::readIndexNext(Data _data) {
    int index_next;
    std::fstream stream(_data.name, std::ios::in | std::ios::binary);
    stream.seekg(sizeof(int), stream.beg);
    stream.read((char*)&index_next, sizeof(int));
    stream.close();
    return index_next;
}

int Sequential_File_Definitve::readIndexDelete(Data _data) {
    int index_delete;
    std::fstream stream(_data.name, std::ios::in | std::ios::binary);
    stream.seekg(2*sizeof(int), stream.beg);
    stream.read((char*)&index_delete, sizeof(int));
    stream.close();
    return index_delete;
}

void Sequential_File_Definitve::updateIndexDelete(Data &_data, int pos, Record record) {
    record.index_delete = _data.index_delete;
    _data.index_delete = pos;
    std::fstream stream(_data.name, std::ios::in | std::ios::out | std::ios::binary);
    stream.seekp(2*sizeof(int), stream.beg);
    stream.write((char*)&pos, sizeof(int));
    stream.seekp(sizeof(int)+pos, stream.beg);
    stream.write((char*)&record, sizeof(Record));
    stream.close();
}

void Sequential_File_Definitve::updateNewDataSize() {
    new_data.size = new_data.size + sizeof(Record);
    std::fstream stream(new_data.name, std::ios::in | std::ios::out | std::ios::binary);
    stream.seekp(0, stream.beg);
    stream.write((char*)&new_data.size, sizeof(int));
    stream.close();
}

void Sequential_File_Definitve::updateDataSize() {
    data.size = data.size + 5*sizeof(Record);
    std::fstream stream(new_data.name, std::ios::in | std::ios::out | std::ios::binary);
    stream.seekp(0, stream.beg);
    stream.write((char*)&new_data.size, sizeof(int));
    stream.close();
}

void Sequential_File_Definitve::insertionSort(std::vector<Record> &records) {
    int i, j;
    Record key;
    for (i = 1; i < records.size(); i++) {  
        bool cambio;
        key = records[i];  
        j = i - 1;  
        cambio = 0 < std::strcmp(records[j].index, key.index);
        
        while (j >= 0 &&  cambio) {  
            records[j + 1] = records[j];  
            j = j - 1;  
        }  
        records[j + 1] = key;  
    }
}
