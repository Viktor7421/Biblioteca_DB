#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>

#define index_T char*

struct Record {
    char index [2];
    char genus [30];
    char species [20];
    char taxa [20];
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


class ISAM {
private:
    Data data;
    Data index;
public:
    ISAM(std::string _data, std::string _index);
    ~ISAM();
    
    Record search(index_T key);

    int searchDataPos(index_T key);

    void insert(Record record);

    void _delete(index_T key);

    void rebuild();

    Record readRecord(Data _data,  int pos);
    void writeRecord(Data _data, int pos, Record record);
    void writeIndex(Data _data, int pos, index_T _index);
    void fillNewFile(Data _data);
    int readSize(Data _data);
    int readIndexNext(Data _data);
    int readIndexDelete(Data _data);
    void updateIndexNewDelete(Data &_data, int pos, Record record);
    void updateIndexLoseDelete(Data &_data, Record record);
    void insertionSort(std::vector<std::pair<char*, int>> &_indexs);
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
    ISAM DataBase("pruebaisam1.dat","pruebaisam11.dat");
    Record ave_1 = creacionRegistro("BA","Amphispiza","bilineata","Bird");
    Record ave_2 = creacionRegistro("LF","Amphispiza","bilineata","Bird");
    Record ave_3 = creacionRegistro("FX","Amphispiza","bilineata","Bird");
    Record ave_4 = creacionRegistro("TW","Amphispiza","bilineata","Bird");
    Record ave_5 = creacionRegistro("AQ","Amphispiza","bilineata","Bird");
    Record ave_6 = creacionRegistro("VV","Amphispiza","bilineata","Bird");
    DataBase.insert(ave_1);
    //std::cout<<DataBase.search("BA").index;
    DataBase.insert(ave_2);
    DataBase.insert(ave_3);
    DataBase.insert(ave_4);
    DataBase.insert(ave_5);
    DataBase.insert(ave_6);
    //Record ave_2 = DataBase.search(ave_1.index);
    //std::cout << ave_2;
    //DataBase._delete("BA");
    //std::cout<<DataBase.search("BA").species;
    return 1;
}

ISAM::ISAM(std::string _data, std::string _index) {
    data.name = _data;
    if(data.empty()) {
        std::cout << "Vacio\n";
        fillNewFile(data);
    }
    data.size = readSize(data); //encontrar el size en el archivo
    data.index_delete = readIndexDelete(data); //encontrar el delete en el archivo

    index.name = _index;
    if(index.empty()) {        
        fillNewFile(index);
    }
    index.size = readSize(index); //encontrar el size en el archivo
    index.index_delete = readIndexDelete(index); //encontrar el delete en el archivo
}

ISAM::~ISAM() {}

Record ISAM::search(index_T key) {
    Record record;
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
        
        if (record.index[0] > key[0]) u = m-sizeof(Record); // cambiar record.index por el indice
        else if (record.index[0] < key[0]) l = m+sizeof(Record);
        else if (record.index[1] > key[1]) u = m-sizeof(Record);
        else if (record.index[1] < key[1]) l = m+sizeof(Record);
        else return record;
    }
    return record;
}

int ISAM::searchDataPos(index_T key) {
    int l = 0;
    int u = (data.size/sizeof(Record))-1;
    while (u >= l) {
        int m = (l+u)/2;
        Record record = readRecord(data, m*sizeof(Record)); // dilema de escoger un archivo eliminado 
        for(int i = 0; record.index_delete != 0; i++) {
            Record record_temp = readRecord(data, m*sizeof(Record)+i*sizeof(Record)); // Evalua no eliminado hacia abajo
            if(record_temp.index_delete != 0) {record = record_temp; m=m+i*sizeof(Record); break;}
            
            record_temp = readRecord(data, m*sizeof(Record)-i*sizeof(Record)); // Evalua no eliminado hacia arriba
            if(record_temp.index_delete != 0) {record = record_temp; m=m-i*sizeof(Record); break;} 
        }
        if (record.index[0] > key[0]) u = m-sizeof(Record); // cambiar record.index por el indice
        else if (record.index[0] < key[0]) l = m+sizeof(Record);
        else if (record.index[1] > key[1]) u = m-sizeof(Record);
        else if (record.index[1] < key[1]) l = m+sizeof(Record);
        else return m*sizeof(Record);
    }
    return -1;
}

void ISAM::insert(Record record) {
    int pos2 = searchDataPos(record.index);
    record.index_delete = 0;
    if(index.index_delete == -1) {
        writeRecord(data,data.size,record);
        writeIndex(index,data.size,record.index);
    } else {
        std::cout << index.index_delete << '\n';
        Record record_delete = readRecord(index,index.index_delete);
        writeRecord(index,index.index_delete,record);
        writeIndex(index,index.index_delete,record.index);
        updateIndexLoseDelete(index,record_delete);
    }
}

void ISAM::_delete(index_T key) {
    int pos = searchDataPos(key);
    if(pos != -1) {
        Record record = readRecord(data, pos);
        updateIndexNewDelete(data, pos, record);
    }
}

Record ISAM::readRecord(Data _data, int pos) {
    Record record;
    std::fstream stream(_data.name, std::ios::in | std::ios::binary);
    stream.seekg(2*sizeof(int)+pos, stream.beg);
    stream.read((char*)&record, sizeof(Record));
    stream.close();
    return record;
}

void ISAM::writeRecord(Data _data, int pos, Record record) {
    std::fstream stream(_data.name, std::ios::in | std::ios::out | std::ios::binary);
    stream.seekp(2*sizeof(int)+pos, stream.beg);
    stream.write((char*)&record, sizeof(Record));
    _data.size += sizeof(Record);
    stream.close();
}

void ISAM::writeIndex(Data _data, int pos, index_T _index) {
    std::vector<std::pair<char*,int>> new_records;
    new_records.push_back({_index,pos});
    std::fstream stream1(_data.name, std::ios::in | std::ios::out | std::ios::binary);
    for(int i = 0; i < _data.size; i+=(sizeof(int)+2)) {
        int _pos;
        char __index[2];
        stream1.seekg(2 * sizeof(int) + i, stream1.beg);
        stream1.read((char*)&__index, 2);
        stream1.read((char*)&_pos, sizeof(int));
        new_records.push_back({__index,_pos});
    }
    stream1.close();

    insertionSort(new_records);

    std::fstream stream2(_data.name, std::ios::in | std::ios::out | std::ios::binary);
    for(int i = 0; i < new_records.size(); i++) {
        stream2.seekp(2 * sizeof(int) + i * (2 + sizeof(int)), stream2.beg);
        stream2.write((char*)&new_records[i].first, 2);
        stream2.write((char*)&new_records[i].second, sizeof(int));
    }
    stream1.close();
}

void ISAM::fillNewFile(Data _data) {
    _data.size = 0;
    _data.index_delete = -1;
    std::fstream stream(_data.name, std::ios::in | std::ios::out | std::ios::binary);
    stream.seekp(0, stream.beg);
    stream.write((char*)&_data.size, sizeof(int));
    stream.write((char*)&_data.index_delete, sizeof(int));
    stream.close();
}

int ISAM::readSize(Data _data) {
    int size;
    std::fstream stream(_data.name, std::ios::in | std::ios::binary);
    stream.seekg(0, stream.beg);
    stream.read((char*)&size, sizeof(int));
    stream.close();
    return size;
}

int ISAM::readIndexDelete(Data _data) {
    int index_delete;
    std::fstream stream(_data.name, std::ios::in | std::ios::binary);
    stream.seekg(sizeof(int), stream.beg);
    stream.read((char*)&index_delete, sizeof(int));
    stream.close();
    return index_delete;
}

void ISAM::updateIndexNewDelete(Data &_data, int pos, Record record) {
    record.index_delete = _data.index_delete;
    _data.index_delete = pos;
    _data.size-=sizeof(Record);
    std::fstream stream(_data.name, std::ios::in | std::ios::out | std::ios::binary);
    stream.seekp(0, stream.beg);
    stream.write((char*)&_data.size, sizeof(int));
    stream.seekp(sizeof(int), stream.beg);
    stream.write((char*)&pos, sizeof(int));
    stream.seekp(3*sizeof(int)+pos, stream.beg);
    stream.write((char*)&record, sizeof(Record));
    stream.close();
}

void ISAM::updateIndexLoseDelete(Data &_data, Record record) {
    _data.index_delete = record.index_delete;
    std::fstream stream(_data.name, std::ios::in | std::ios::out | std::ios::binary);
    stream.seekp(2*sizeof(int), stream.beg);
    stream.write((char*)&_data.index_delete, sizeof(int));
    stream.close();
}



void ISAM::insertionSort(std::vector<std::pair<char*, int>> &_indexs) {
    int i, j;
    std::pair<char*,int> key;
    for (i = 1; i < _indexs.size(); i++) {  
        bool cambio;
        key = _indexs[i];  
        j = i - 1;  
        while (j >= 0 && 0 < std::strcmp(_indexs[j].first, key.first)) {  
            _indexs[j + 1] = _indexs[j];  
            j = j - 1;  
        }  
        _indexs[j + 1] = key;  
    }
}
