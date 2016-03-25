#include <cstdio> 
#include <iostream> 
#include <string> 

#ifndef FILEHASHMAP
#define FILEHASHMAP


const char UNUSED = 255;
const char USED = 254;

template<typename K,typename V,typename Hash>
class FileHashMap {
    Hash hashFunction;
    // File name
    std::string fileName;
    FILE *file;
    unsigned int sz; // Number of records stored
    unsigned int capacity;    // Capacity of the file
    char full = USED; 
    // Placed here to prevent copies.
    FileHashMap(const FileHashMap<K,V,Hash> &that);
    FileHashMap<K,V,Hash> &operator=(const FileHashMap<K,V,Hash> &that);

    // Helper functions recommended
    void writeStart(){
        writeSize();
        writeCapacity();
        for(int x=0;x<capacity;x++){
           writeUnused(x);
        }        
    }

    void readSize(){
        std::fseek(file,0,SEEK_SET);
        std::fread(&sz,sizeof(int),1,file);
    }
    void readCapacity(){
        std::fseek(file,sizeof(int),SEEK_SET);
        std::fread(&capacity,sizeof(int),1,file);
    }

    K readKey(int loc){
        K keytemp;
        std::fseek(file,(sizeof(int)*2+(sizeof(K)+sizeof(V)+sizeof(char))*loc)+sizeof(char),SEEK_SET);
        std::fread(&keytemp,sizeof(K),1,file);
        return keytemp;
    }
    std::pair<K,V> readPair(int loc){
        K keytemp; 
        V valtemp; 
        keytemp = readKey(loc);
        std::fseek(file,(sizeof(int)*2+(sizeof(K)+sizeof(V)+sizeof(char))*loc)+sizeof(char)+sizeof(K),SEEK_SET);
        std::fread(&valtemp,sizeof(V),1,file);
        return std::make_pair(keytemp,valtemp);
    }
    void writeSize(){
        std::fseek(file,0,SEEK_SET);
        std::fwrite(&sz,sizeof(int),1,file);
    }
    void writeCapacity(){
        std::fseek(file,sizeof(int),SEEK_SET);
        std::fwrite(&capacity,sizeof(int),1,file);
    }
    void writeUnused(int loc){
        std::fseek(file,sizeof(int)*2+(sizeof(K)+sizeof(V)+sizeof(char))*loc,SEEK_SET);
        std::fwrite(&UNUSED,sizeof(char),1,file);
    }

    void writePair(std::pair<K,V> vtype){
        auto bin = hashFunction(vtype.first) % capacity;
        while(full != UNUSED){
            std::fseek(file,sizeof(int)*2+(sizeof(K)+sizeof(V)+sizeof(char))*bin,SEEK_SET);
            std::fread(&full,sizeof(char),1,file);       
            bin += 1;
        }
        full = USED;
        std::fseek(file,sizeof(int)*2+(sizeof(K)+sizeof(V)+sizeof(char))*bin,SEEK_SET);
        std::fwrite(&full,sizeof(char),1,file);
        std::fseek(file,(sizeof(int)*2+(sizeof(K)+sizeof(V)+sizeof(char))*bin)+sizeof(char),SEEK_SET);
        std::fwrite(&vtype.first,sizeof(K),1,file);
        std::fseek(file,(sizeof(int)*2+(sizeof(K)+sizeof(V)+sizeof(char))*bin)+sizeof(char)+sizeof(K),SEEK_SET);
        std::fwrite(&vtype.second,sizeof(V),1,file);
    }
    char readChar(int loc){
              char fill;
              std::fseek(file,sizeof(int)*2+(sizeof(K)+sizeof(V)+sizeof(char))*loc,SEEK_SET);
              std::fread(&fill,sizeof(char),1,file);
              return fill;    
    }


public:
    typedef K key_type;
    typedef V mapped_type;
    typedef std::pair<K,V> value_type;

    class const_iterator {
        FILE *file;
        unsigned cap;
        unsigned loc;
        char itrChar(int loc){
              char itrfill;
              std::fseek(file,sizeof(int)*2+(sizeof(K)+sizeof(V)+sizeof(char))*loc,SEEK_SET);
              std::fread(&itrfill,sizeof(char),1,file);
              return itrfill;    
        }
        void advance(){
            // Code to move forward to the next used element after loc.
            while(itrChar(loc) == UNUSED && loc != cap){
                loc+=1;
                std::cout << "\n" << loc;
            }
        }
    public:
        const_iterator(FILE *f,unsigned c):file(f),cap(c),loc(0) {
            advance();
        }
        const_iterator(FILE *f,unsigned c,unsigned i):file(f),cap(c),loc(i) {}

        bool operator==(const const_iterator &i) const { return file==i.file && loc==i.loc; }
        bool operator!=(const const_iterator &i) const { return !(*this==i); }
        const std::pair<K,V> operator*() const {
            // seek
            K keytemp; 
            V valtemp; 
            std::fseek(file,(sizeof(int)*2+(sizeof(K)+sizeof(V)+sizeof(char))*loc)+sizeof(char),SEEK_SET);
            std::fread(&keytemp,sizeof(K),1,file);
            std::fseek(file,(sizeof(int)*2+(sizeof(K)+sizeof(V)+sizeof(char))*loc)+sizeof(char)+sizeof(K),SEEK_SET);
            std::fread(&valtemp,sizeof(V),1,file);
    
            std::pair<K,V> p;
            p = std::make_pair(keytemp,valtemp);
            // read
            return p;
        }
        const_iterator &operator++() {
            ++loc;
            advance();
            return *this;
        }
        const_iterator operator++(int) {
            const_iterator tmp(*this);
            ++(*this);
            return tmp;
        }

        int rtnloc{
            return loc;
        }
    };

    FileHashMap(const std::string &fname,const Hash &hf):hashFunction(hf),fileName(fname),file(fopen(fname.c_str(),"r+b")) {
        if(file==nullptr) {
            file = std::fopen(fname.c_str(),"w+b");
            sz = 0;
            capacity = 10;
            writeStart();
        }else {
            // read header information
            readCapacity();
            readSize();
        }
    }

    ~FileHashMap() {
        // close the file
        std::fclose(file);
    }

    bool empty() const{
        return sz == 0;
    }
    unsigned size() const{
        return sz;
    }

    const_iterator find(const key_type& k) const{ 
        auto bin = hashFunction(k) % capacity; 
        auto a = begin()+bin;
        while(readChar())

        for(auto a = begin()+bin; a != end(); ++a){
            if((*a).first == k){
                return a;
            }
        }
        return end();
    }

    int count(const key_type& k) const{
        if (find(k) != end()){
            return 1;
        }
        else return 0; 
    }

    std::pair<const_iterator,bool> insert(const value_type& val){
       auto found = find(val.first);
       if(found != end()){
        return std::make_pair(found,false);
       }
       writePair(val); 
       found = find(val.first);
       return std::make_pair(found,true);
        
    }

    template <class InputIterator>
    void insert(InputIterator first, InputIterator last) {
        for(auto iter = first; iter!=last; ++iter) {
            insert(*iter);
        }
    }

    void clear(){
        writeStart();
    }

    const mapped_type operator[](const K &key) const;

    const_iterator set(const value_type &val);

    bool operator==(const FileHashMap<K,V,Hash>& rhs) const;

    bool operator!=(const FileHashMap<K,V,Hash>& rhs) const;

    const_iterator begin() const { return const_iterator(file,capacity); }

    const_iterator end() const { return const_iterator(file,capacity,capacity); }

    const_iterator cbegin() const { return const_iterator(file,capacity); }

    const_iterator cend() const { return const_iterator(file,capacity,capacity); }

private:
    void growTableAndRehash();
};

#endif