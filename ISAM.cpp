#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>

#define index_T char*
#define INDEX_SIZE 2
#define SUB_INDEX_SIZE 1
#define index_pair std::pair<char,int>
#define LIVE -2
#define BUCKET_SIZE 27

struct Record {
    char index [4];
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

    Record readRecord(int pos);
    index_pair readIndex(int pos);
    void writeRecord(int pos, Record record);
    void writeIndex(int pos, index_T _index);
    void fillNewFile(Data _data);
    int readSize(Data _data);
    int readIndexNext(Data _data);
    int readIndexDelete(Data _data);
    void updateIndexNewDelete(Data &_data, int pos, Record record);
    void updateIndexLoseDelete(Data &_data, Record record);
    void updateDataSize();
    void insertionSort(std::vector<index_pair> &_indexs);
};

Record createRecord(std::string species_id, std::string genus, std::string species, std::string taxa) {
    Record record;
    for(int i = 0; i < sizeof(record.index)-1; i++) {
        if(i < species_id.length()) {
            record.index[i] = species_id[i];
        } else {
            record.index[i] = ' ';
        }
    }
    record.index[sizeof(record.index)-1] = '\0';
    for(int i = 0; i < sizeof(record.genus)-1; i++) {
        if(i < genus.length()) {
            record.genus[i] = genus[i];
        } else {
            record.genus[i] = ' ';
        }
    }
    record.genus[sizeof(record.genus)-1] = '\0';
    for(int i = 0; i < sizeof(record.species)-1; i++) {
        if(i < species.length()) {
            record.species[i] = species[i];
        } else {
            record.species[i] = ' ';
        }
    }
    record.species[sizeof(record.species)-1] = '\0';
    for(int i = 0; i < sizeof(record.taxa)-1; i++) {
        if(i < taxa.length()) {
            record.taxa[i] = taxa[i];
        } else {
            record.taxa[i] = ' ';
        }
    }
    record.taxa[sizeof(record.taxa)-1] = '\0';
    return record;
}

int main()  {
    ISAM DataBase("data_ISAM.dat","index_ISAM.dat");
    Record ave_1 = createRecord("BA","Amphispiza","bilineata","Bird");
    Record ave_2 = createRecord("LF","Amphispiza","bilineata","Bird");
    Record ave_3 = createRecord("FX","Amphispiza","bilineata","Bird");
    Record ave_4 = createRecord("TW","Amphispiza","bilineata","Bird");
    Record ave_5 = createRecord("AQ","Amphispiza","bilineata","Bird");
    Record ave_6 = createRecord("VV","Amphispiza","bilineata","Bird");
    DataBase.insert(ave_1);
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
    int u = (index.size/(sizeof(record.index)+sizeof(int)))-1;
    for(int i = 0; i < INDEX_SIZE; i++)
        while (u >= l) {
            int m = (l+u)/2;
            index_pair _index = readIndex(m*(SUB_INDEX_SIZE+sizeof(int)));
            if (_index.first > key[i]) u = m-1; 
            else if (_index.first < key[i]) l = m+1;
            else if (record.index_delete != LIVE) break;
            else {record = readRecord(_index.second); return record;}
        }
    record = createRecord("","","","");
    return record;
}

int ISAM::searchDataPos(index_T key) {
    Record record;
    int l = 0;
    int u = (index.size/(SUB_INDEX_SIZE+sizeof(int)))-1;
    for(int i = 0; i < INDEX_SIZE; i++)
        while (u >= l) {
            int m = (l+u)/2;
            index_pair _index = readIndex(m*(1+sizeof(int)));
            if (_index.first > key[i]) u = m-1;
            else if (_index.first < key[i]) l = m+1;
            else if (record.index_delete != LIVE) break;
            else return m*(SUB_INDEX_SIZE+sizeof(int));
        }
    return -1;
}

void ISAM::insert(Record record) {
    int pos = searchDataPos(record.index);
    record.index_delete = LIVE;
    if(pos != -1) {std::cout << "Ese archivo ya existe.\n"; return;}
    if(index.index_delete == -1) {
        writeRecord(data.size,record);
        writeIndex(data.size,record.index);
        updateDataSize();
    } else {
        Record record_delete = readRecord(data.index_delete);
        writeRecord(data.index_delete,record);
        updateIndexLoseDelete(data,record_delete);
        writeIndex(index.index_delete,record.index);
        updateIndexLoseDelete(index,record_delete);
        updateDataSize();
    }
}

void ISAM::_delete(index_T key) {
    int pos = searchDataPos(key);
    if(pos != -1) {
        Record record = readRecord(pos);
        updateIndexNewDelete(data, pos, record);
    }
}

Record ISAM::readRecord(int pos) {
    Record record;
    std::fstream stream(data.name, std::ios::in | std::ios::binary);
    stream.seekg(2*sizeof(int)+pos, stream.beg);
    stream.read((char*)&record, sizeof(Record));
    stream.close();
    return record;
}


index_pair ISAM::readIndex(int pos) {
    index_pair _index;
    std::fstream stream(index.name, std::ios::in | std::ios::binary);
    stream.seekg(2*sizeof(int)+pos, stream.beg);
    stream.read((char*)&_index.first, SUB_INDEX_SIZE);
    stream.read((char*)&_index.second, sizeof(int));
    stream.close();
    return _index;
}

void ISAM::writeRecord(int pos, Record record) {
    std::fstream stream(data.name, std::ios::in | std::ios::out | std::ios::binary);
    stream.seekp(2*sizeof(int)+pos, stream.beg);
    stream.write((char*)&record, sizeof(Record));
    data.size += sizeof(Record);
    stream.close();
}

void ISAM::writeIndex(int pos, index_T _index) {
    std::vector<index_pair> index_vector;
    index_vector.push_back({_index[0],pos});
    std::fstream stream1(index.name, std::ios::in | std::ios::out | std::ios::binary);
    for(int i = 0; i < index.size; i+=(sizeof(int)+SUB_INDEX_SIZE)) {
        int _pos;
        char __index;
        stream1.seekg(2*sizeof(int) + i, stream1.beg);
        stream1.read((char*)&__index, SUB_INDEX_SIZE);
        stream1.read((char*)&_pos, sizeof(int));
        index_vector.push_back({__index,_pos});
    }
    stream1.close();

    insertionSort(index_vector);

    std::fstream stream2(index.name, std::ios::in | std::ios::out | std::ios::binary);
    for(int i = 0; i < index_vector.size(); i++) {
        stream2.seekp(2 * sizeof(int) + i * (1 + sizeof(int)), stream2.beg);
        stream2.write((char*)&index_vector[i].first, 1);
        stream2.write((char*)&index_vector[i].second, sizeof(int));
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
    stream.seekp(sizeof(int), stream.beg);
    stream.write((char*)&_data.index_delete, sizeof(int));
    stream.close();
}

void ISAM::updateDataSize() {
    data.size += sizeof(Record);
    index.size += (sizeof(int)+SUB_INDEX_SIZE);
    std::fstream stream1(data.name, std::ios::in | std::ios::out | std::ios::binary);
    stream1.seekp(0, stream1.beg);
    stream1.write((char*)&data.size, sizeof(int));
    stream1.close();
    std::fstream stream2(index.name, std::ios::in | std::ios::out | std::ios::binary);
    stream2.seekp(0, stream2.beg);
    stream2.write((char*)&index.size, sizeof(int));
    stream2.close();
}

void ISAM::insertionSort(std::vector<index_pair> &index_vector) {
    int i, j;
    index_pair key;
    for (i = 1; i < index_vector.size(); i++) {  
        bool cambio;
        key = index_vector[i];  
        j = i - 1;  
        while (j >= 0 && index_vector[j].first < key.first) {  
            index_vector[j + 1] = index_vector[j];  
            j = j - 1;  
        }  
        index_vector[j + 1] = key;  
    }
}
