#include <iostream>
#include <fstream>
#include <vector>
#include <iostream>
#include <string>
#include <cstring>
#include <boost/algorithm/string.hpp>

#define index_T char*
#define N_new_data 5
#define LIVE -2

struct Record {
    char index [4];
    char genus [30];
    char species [20];
    char taxa [20];
    int index_delete;
};

std::istream & operator >> (std::istream & stream, Record & record) {
    stream.read(record.index, 4);
    stream.read(record.genus, 30);
    stream.read(record.species, 20);
    stream.read(record.taxa, 20);
    stream.get();
    return stream;
}

std::ostream & operator << (std::ostream & stream, Record & record) {
    stream.write(record.index, 4);
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


class Sequential_File {
private:
    Data data;
    Data new_data;
public:
    Sequential_File(std::string _data, std::string _new_data);
    ~Sequential_File();
    
    Record search(index_T key);

    int searchDataPos(index_T key);

    int searchNewDataPos(index_T key);

    std::vector<Record> get_csv(const std::string& filename);

    void insert(Record record);

    void _delete(index_T key);

    void rebuild();

    Record readRecord(Data _data,  int pos);
    void writeRecord(Data data, int pos, Record record);
    void fillNewFile(Data _data);
    int readSize(Data _data);
    int readIndexNext(Data _data);
    int readIndexDelete(Data _data);
    void updateIndexNewDelete(Data &_data, int pos, Record record);
    void updateIndexLoseDelete(Data &_data, Record record);
    void updateDataSize();
    void updateNewDataSize();

    void insertionSort(std::vector<Record> &records);
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
    Sequential_File DataBase("data_SF.dat","new_data_SF.dat");
    DataBase.get_csv("species.csv");
    /*Record ave_1 = createRecord("BQ","Amphispiza","bilineata","Bird");
    Record ave_2 = createRecord("LQ","Amphispiza","bilineata","Bird");
    Record ave_3 = createRecord("AQ","Amphispiza","bilineata","Bird");
    Record ave_4 = createRecord("TQ","Amphispiza","bilineata","Bird");
    Record ave_5 = createRecord("AQ","Amphispiza","bilineata","Bird");
    Record ave_6 = createRecord("VQ","Amphispiza","bilineata","Bird");
    DataBase.insert(ave_1);
    DataBase.insert(ave_2);
    DataBase.insert(ave_3);
    DataBase.insert(ave_4);
    DataBase.insert(ave_5);
    DataBase.insert(ave_6)
    Record ave_1 = DataBase.search(ave_3.index);
    std::cout << "Ave AQ: " << ave_1 << '\n';
    std::cout<<"Ave TQ: "<<ave_4<<'\n';
    //Record ave_2 = DataBase.search(ave_4.index);
    //std::cout << "Ave TS: " << ave_2 << ' ' << '\n';
    //DataBase._delete(ave_3.index);
    DataBase._delete(ave_4.index);*/
    ///Read a csv
    /*Sequential_File_Definitve Database2("data_SF2.dat","new_data_SF2.dat");
    std::vector<Record> species= Database2.get_csv("species.csv");
    for(auto i=0;i<species.size();i++){
        Record name=createRecord(species[i].index,species[i].genus,species[i].species,species[i].taxa);
        Database2.insert(name);
    }*/
    return 1;
}

Sequential_File::Sequential_File(std::string _data, std::string _new_data) {
    data.name = _data;
    if(data.empty()) {
        fillNewFile(data);
    }
    data.size = readSize(data); //encontrar el size en el archivo
    data.index_delete = readIndexDelete(data); //encontrar el delete en el archivo

    new_data.name = _new_data;
    if(new_data.empty()) {        
        fillNewFile(new_data);
    }
    new_data.size = readSize(new_data); //encontrar el size en el archivo
    new_data.index_delete = readIndexDelete(new_data); //encontrar el delete en el archivo
}

Sequential_File::~Sequential_File() {}

Record Sequential_File::search(index_T key) {
    Record record;
    int pos = 0;
    for(int i = 0; i < new_data.size; pos+=sizeof(Record)) {
        record = readRecord(new_data,pos);
        if(0 == std::strcmp(record.index, key) && record.index_delete == LIVE) return record;
        else if(record.index_delete == LIVE) i+=sizeof(Record);
    }
    int l = 0;
    int u = (data.size/sizeof(Record))-1;
    while (u >= l) {
        int m = (l+u)/2;
        record = readRecord(data, m*sizeof(Record)); 
        if (0 < std::strcmp(record.index, key)) u = m-1; 
        else if (0 > std::strcmp(record.index, key)) l = m+1;
        else if (record.index_delete != LIVE) break;
        else return record;
    }
    record = createRecord("","","","");
    return record;
}

int Sequential_File::searchDataPos(index_T key) {
    int l = 0;
    int u = (data.size/sizeof(Record))-1;
    while (u >= l) {
        int m = (l+u)/2;
        Record record = readRecord(data, m*sizeof(Record));
        if (0 < std::strcmp(record.index, key)) u = m-1; 
        else if (0 > std::strcmp(record.index, key)) l = m+1;
        else if (record.index_delete != LIVE) break;
        else return m*sizeof(Record);
    }
    return -1;
}

int Sequential_File::searchNewDataPos(index_T key) {
    int pos = 0;
    for(int i = 0; i < new_data.size; pos+=sizeof(Record)) {
        Record record = readRecord(new_data,pos);
        if(0 == std::strcmp(record.index, key) && record.index_delete == LIVE) return pos;
        else if(record.index_delete == LIVE) i+=sizeof(Record);
    }
    return -1;
}

void Sequential_File::insert(Record record) {
    int pos1 = searchNewDataPos(record.index);
    int pos2 = searchDataPos(record.index);
    record.index_delete = LIVE;
    if(pos1 != -1 || pos2 != -1) {std::cout << "Ese archivo ya existe.\n"; return;}
    if(new_data.size < N_new_data*sizeof(Record)) {
        if(new_data.index_delete == -1) {
            writeRecord(new_data,new_data.size,record);
            updateNewDataSize();
        } else {
            Record record_delete = readRecord(new_data,new_data.index_delete);
            writeRecord(new_data,new_data.index_delete,record);
            updateIndexLoseDelete(new_data,record_delete);
            updateNewDataSize();
        }
    } else {
        rebuild();
        insert(record);
    }
    std::cout << "data: " << data.size << " new_data: " << new_data.size << '\n' ;
}

void Sequential_File::_delete(index_T key) {
    int pos = searchNewDataPos(key);
    if(pos != -1) {
        Record record = readRecord(new_data, pos);
        updateIndexNewDelete(new_data, pos, record);
    }
    pos = searchDataPos(key);
    if(pos != -1) {
        Record record = readRecord(data, pos);
        updateIndexNewDelete(data, pos, record);
    }
}

void Sequential_File::rebuild() {
    std::vector<Record> new_records;
    int _size = new_data.size;
    for(int i = 0; i < _size; i+=sizeof(Record)) {
        Record record = readRecord(new_data,i);
        new_records.push_back(record);
        _delete(record.index);
    }
    insertionSort(new_records);
    for(int i = 0; i < data.size; i+=sizeof(Record)) {
        Record record = readRecord(data,i);
        if(0 <= std::strcmp(record.index, new_records[0].index) && record.index_delete != LIVE) {
            writeRecord(data,i,new_records[0]);
            new_records[0] = new_records.back();
            new_records.pop_back();
            insertionSort(new_records);
        } else if(0 < std::strcmp(record.index, new_records[0].index)) {
            writeRecord(data,i,new_records[0]);
            new_records[0] = record;
            insertionSort(new_records);
        }
    }
    for(int i = 0; i < new_records.size(); i++) {
        writeRecord(data,i*sizeof(Record)+data.size,new_records[i]);
    }
    new_data.size = 0;
    updateDataSize();
}

Record Sequential_File::readRecord(Data _data, int pos) {
    Record record;
    std::fstream stream(_data.name, std::ios::in | std::ios::binary);
    stream.seekg(2*sizeof(int)+pos, stream.beg);
    stream.read((char*)&record, sizeof(Record));
    stream.close();
    return record;
}

void Sequential_File::writeRecord(Data _data, int pos, Record record) {
    std::fstream stream(_data.name, std::ios::in | std::ios::out | std::ios::binary);
    stream.seekp(2*sizeof(int)+pos, stream.beg);
    stream.write((char*)&record, sizeof(Record));
    _data.size += sizeof(Record);
    stream.close();
}

void Sequential_File::fillNewFile(Data _data) {
    _data.size = 0;
    _data.index_delete = -1;
    std::fstream stream(_data.name, std::ios::in | std::ios::out | std::ios::binary);
    stream.seekp(0, stream.beg);
    stream.write((char*)&_data.size, sizeof(int));
    stream.write((char*)&_data.index_delete, sizeof(int));
    stream.close();
}

int Sequential_File::readSize(Data _data) {
    int size;
    std::fstream stream(_data.name, std::ios::in | std::ios::binary);
    stream.seekg(0, stream.beg);
    stream.read((char*)&size, sizeof(int));
    stream.close();
    return size;
}

int Sequential_File::readIndexDelete(Data _data) {
    int index_delete;
    std::fstream stream(_data.name, std::ios::in | std::ios::binary);
    stream.seekg(sizeof(int), stream.beg);
    stream.read((char*)&index_delete, sizeof(int));
    stream.close();
    return index_delete;
}

void Sequential_File::updateIndexNewDelete(Data &_data, int pos, Record record) {
    record.index_delete = _data.index_delete;
    _data.index_delete = pos;
    _data.size -= sizeof(Record);
    std::fstream stream(_data.name, std::ios::in | std::ios::out | std::ios::binary);
    stream.seekp(0, stream.beg);
    stream.write((char*)&_data.size, sizeof(int));
    stream.seekp(sizeof(int), stream.beg);
    stream.write((char*)&_data.index_delete, sizeof(int));
    stream.seekp(2*sizeof(int)+pos, stream.beg);
    stream.write((char*)&record, sizeof(Record));
    stream.close();
}

void Sequential_File::updateIndexLoseDelete(Data &_data, Record record) {
    _data.index_delete = record.index_delete;
    std::fstream stream(_data.name, std::ios::in | std::ios::out | std::ios::binary);
    stream.seekp(sizeof(int), stream.beg);
    stream.write((char*)&_data.index_delete, sizeof(int));
    stream.close();
}

void Sequential_File::updateNewDataSize() {
    new_data.size = new_data.size + sizeof(Record);
    std::fstream stream(new_data.name, std::ios::in | std::ios::out | std::ios::binary);
    stream.seekp(0, stream.beg);
    stream.write((char*)&new_data.size, sizeof(int));
    stream.close();
}

void Sequential_File::updateDataSize() {
    data.size = data.size + N_new_data*sizeof(Record);
    std::fstream stream(data.name, std::ios::in | std::ios::out | std::ios::binary);
    stream.seekp(0, stream.beg);
    stream.write((char*)&data.size, sizeof(int));
    stream.close();
}

void Sequential_File::insertionSort(std::vector<Record> &records) {
    int i, j;
    Record key;
    for (i = 1; i < records.size(); i++) {  
        bool cambio;
        key = records[i];  
        j = i - 1;  
        while (j >= 0 && 0 < std::strcmp(records[j].index, key.index)) {  
            records[j + 1] = records[j];  
            j = j - 1;  
        }  
        records[j + 1] = key;  
    }
}
std::vector<Record> Sequential_File::get_csv(const std::string &filename) {
    std::ifstream file(filename);
    std::vector<std::vector<std::string >>tempdata;
    std::vector<Record> species;
    std::string line=" ";
    while (getline(file,line)) {
        std::vector<std::string> temp;
        boost::algorithm::split(temp, line, boost::is_any_of(","));
        tempdata.push_back(temp);
    }
    file.close();
    for(std::vector<std::string> vec:tempdata){
        Record temp;
        strcpy(temp.index,vec[0].c_str());
        strcpy(temp.genus,vec[1].c_str());
        strcpy(temp.species,vec[2].c_str());
        strcpy(temp.taxa,vec[3].c_str());
        species.emplace_back(temp);
    }
    return species;
}
