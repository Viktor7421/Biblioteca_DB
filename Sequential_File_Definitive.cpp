#include <iostream>
#include <fstream>
#include <vector>
#include <iostream>
#include <string>
#include <cstring>


struct Aves {
    char index [2];
    char genus [30];
    char species [20];
    char taxa [20];
    long index_next;
    long index_delete;
};

struct Data
{
    std::string name;
    int size;
    long index_next;
    long index_delete;
    bool empty() {
        std::fstream stream(name, std::ios::binary | std::ios::in);
        int n = -1;
        stream.seekg(0);
        stream.read((char*)&n, sizeof(int));
        stream.close();
        if(n == -1) return 1;
        else return 0;
    }
};


template <class index_T, class Record>
class Sequential_File_Definitve {
private:
    Data data;
    Data new_data;
    index_T index;
public:
    Sequential_File_Definitve(std::string _data, std::string _new_data);
    ~Sequential_File_Definitve();
    
    Record search(index_T key);

    long searchDataPos(index_T key);

    long searchNewDataPos(index_T key);

    void insert(Record record);

    void _delete(index_T key);

    void rebuild();

    Record readRecord(Data _data,  long pos);
    void writeRecord(Data data, long pos, Record record);
    void fillNewFile(Data _data);
    int readSize(Data _data);
    int readIndexNext(Data _data);
    int readIndexDelete(Data _data);
    void updateIndexDelete(Data &_data, long pos, Record record);

    void insertionSort(std::vector<Record> &records);
};


int main()  {
    return 1;
}

template <class index_T, class Record>
Sequential_File_Definitve<index_T,Record>::Sequential_File_Definitve(std::string _data, std::string _new_data) {
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

template <class index_T, class Record>
Sequential_File_Definitve<index_T,Record>::~Sequential_File_Definitve() {}

template <class index_T, class Record>
Record Sequential_File_Definitve<index_T,Record>::search(index_T key) {
    for(long i = 0; i < new_data.size; i+=sizeof(Record)) {
        Record record = readRecord(new_data,i);
        if(record.index == key && record.index_delete == 0) return record;
    }
    if(data.index_next != -1) {
        
        long l = 0;
        long u = data.size-sizeof(Record);
        while (u >= l) {
            long m = (l+u)/2;
            Record record = readRecord(data, m); // dilema de escoger un archivo eliminado 
            for(int i = 0; record.index_delete != 0; i++) {
                Record record_temp = readRecord(data, m+i*sizeof(Record)); // Evalua no eliminado hacia abajo
                if(record_temp.index_delete != 0) {record = record_temp; m=m+i*sizeof(Record); break;}
                
                record_temp = readRecord(data, m-i*sizeof(Record)); // Evalua no eliminado hacia arriba
                if(record_temp.index_delete != 0) {record = record_temp; m=m-i*sizeof(Record); break;} 
            }
            switch (index_T) {
            case int:
                if (record.index < key) u = m-sizeof(Record); // cambiar record.index por el indice
                else if (record.index > key) l = m+sizeof(Record);
                else return record;
                break;
            
            case char*:
                if (0 < std::strcmp(records[j].nombre, key.nombre)) u = m-sizeof(Record); // cambiar record.index por el indice
                else if (0 > std::strcmp(records[j].nombre, key.nombre)) l = m+sizeof(Record);
                else return record;
                break;

            default:
                std::cout << "ERROR: No existe ese tipo de indice\n";
                return NULL;
            }
        }
    }
    std::cout << "ERROR: No existe ese indice en la BD\n";
    return NULL;
}

template <class index_T, class Record>
long Sequential_File_Definitve<index_T,Record>::searchDataPos(index_T key) {
    if(data.index_next != -1) {
        
        long l = 0;
        long u = data.size-sizeof(Record);
        while (u >= l) {
            long m = (l+u)/2;
            Record record = readRecord(data, m); // dilema de escoger un archivo eliminado 
            for(int i = 0; record.index_delete != 0; i++) {
                Record record_temp = readRecord(data, m+i*sizeof(Record)); // Evalua no eliminado hacia abajo
                if(record_temp.index_delete != 0) {record = record_temp; m=m+i*sizeof(Record); break;}
                
                record_temp = readRecord(data, m-i*sizeof(Record)); // Evalua no eliminado hacia arriba
                if(record_temp.index_delete != 0) {record = record_temp; m=m-i*sizeof(Record); break;} 
            }
            switch (index_T) {
            case int:
                if (record.index < key) u = m-sizeof(Record); // cambiar record.index por el indice
                else if (record.index > key) l = m+sizeof(Record);
                else return m;
                break;
            
            case char*:
                if (0 < std::strcmp(records[j].nombre, key.nombre)) u = m-sizeof(Record); // cambiar record.index por el indice
                else if (0 > std::strcmp(records[j].nombre, key.nombre)) l = m+sizeof(Record);
                else return m;
                break;

            default:
                std::cout << "ERROR: No existe ese tipo de indice\n";
                return NULL;
            }
        }
    }
    return -1;
}

template <class index_T, class Record>
long Sequential_File_Definitve<index_T,Record>::searchNewDataPos(index_T key) {
    for(long i = 0; i < new_data.size; i+=sizeof(Record)) {
        Record record = readRecord(new_data,i);
        if(record.index == key && record.index_delete == 0) return i;
    }
    return -1;
}

template <class index_T, class Record>
void Sequential_File_Definitve<index_T,Record>::insert(Record record) {
    if(new_data.size < 5*sizeof(Record)) {
        record.index_delete = 0;
        record.index_next = -1;
        std::fstream stream(new_data.name, std::ios::in | std::ios::out | std::ios::binary);    
        stream.seekp(3*sizeof(int)+new_data.size*sizeof(Record));
        stream.write((char*)&record, sizeof(Record));
        stream.close(); 

    } else {
        rebuild();
    }
}

template <class index_T, class Record>
void Sequential_File_Definitve<index_T,Record>::_delete(index_T key) {
    long pos = searchNewDataPos(key);
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

template <class index_T, class Record>
void Sequential_File_Definitve<index_T,Record>::rebuild() {
    std::vector<Record> new_records;
    std::fstream stream(data.name, std::ios::in | std::ios::out | std::ios::binary);
    for(long i = 0; i < new_data.size; i+=sizeof(Record)) {
        Record record = readRecord(new_data,i);
        new_records.push_back(record);
    }
    for(long i = 0; i < data.size; i+=sizeof(Record)) {
        Record record = readRecord(data,i);
        if(record.index > new_records[0].index) {
            writeRecord(data,i,new_records[0]);
            new_records[0] = record;
            insertion_sort(new_records);
        }
    }
}

template <class index_T, class Record>
Record Sequential_File_Definitve<index_T,Record>::readRecord(Data _data, long pos) {
    Record record;
    std::fstream stream(_data.name, std::ios::in | std::ios::binary);
    stream.seekg(3*sizeof(int)+pos);
    stream.read((char*)&record, sizeof(Record));
    stream.close();
    return record;
}

template <class index_T, class Record>
void Sequential_File_Definitve<index_T,Record>::writeRecord(Data _data, long pos, Record record) {
    std::fstream stream(_data.name, std::ios::in | std::ios::out | std::ios::binary);
    stream.seekp(3*sizeof(int)+pos);
    stream.write((char*)&record, sizeof(Record));
    data.size += sizeof(Record);
    stream.close();
}


template <class index_T, class Record>
void Sequential_File_Definitve<index_T,Record>::fillNewFile(Data _data) {
    int size = 0;
    int index_next = -1;
    int index_delete = -1;
    std::fstream stream(_data.name, std::ios::in | std::ios::out | std::ios::binary);
    stream.seekp(0);
    stream.write((char*)&size, sizeof(int));
    stream.write((char*)&index_next, sizeof(int));
    stream.write((char*)&index_delete, sizeof(int));
    stream.close();
}

template <class index_T, class Record>
int Sequential_File_Definitve<index_T,Record>::readSize(Data _data) {
    int size;
    std::fstream stream(_data.name, std::ios::in | std::ios::binary);
    stream.seekg(0);
    stream.read((char*)&size, sizeof(int));
    stream.close();
    return size;
}

template <class index_T, class Record>
int Sequential_File_Definitve<index_T,Record>::readIndexNext(Data _data) {
    int index_next;
    std::fstream stream(_data.name, std::ios::in | std::ios::binary);
    stream.seekg(sizeof(int));
    stream.read((char*)&index_next, sizeof(int));
    stream.close();
    return index_next;
}

template <class index_T, class Record>
int Sequential_File_Definitve<index_T,Record>::readIndexDelete(Data _data) {
    int index_delete;
    std::fstream stream(_data.name, std::ios::in | std::ios::binary);
    stream.seekg(2*sizeof(int));
    stream.read((char*)&index_delete, sizeof(int));
    stream.close();
    return index_delete;
}


template <class index_T, class Record>
void Sequential_File_Definitve<index_T,Record>::updateIndexDelete(Data &_data, long pos, Record record) {
    record.index_delete = _data.index_delete;
    _data.index_delete = pos;
    std::fstream stream(_data.name, std::ios::in | std::ios::out | std::ios::binary);
    stream.seekp(0, 2*sizeof(int));
    stream.write((char*)&pos, sizeof(int));
    stream.seekp(0, 3*sizeof(int)+pos);
    stream.write((char*)&record, sizeof(Record));
    stream.close();
}

template <class index_T, class Record>
void Sequential_File_Definitve<index_T,Record>::insertionSort(std::vector<Record> &records)  
{
    int i, j;
    Record key;
    for (i = 1; i < records.size(); i++) 
    {  
        bool cambio;
        key = records[i];  
        j = i - 1;  
        switch (index_T) {
        case int:
            cambio = records[j].index > key.index;
            break;
        case char*:
            cambio = 0 < std::strcmp(records[j].index, key.index);
            break;
        default:
            std::cout << "ERROR: No existe ese tipo de indice";
            return;
        }
        while (j >= 0 &&  cambio) 
        {  
            records[j + 1] = records[j];  
            j = j - 1;  
        }  
        records[j + 1] = key;  
    }
}
