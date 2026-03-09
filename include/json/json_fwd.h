#ifndef __JSON_FWD_H__
#define __JSON_FWD_H__

#include <memory>
#include <string>
#include "token.h"
struct object;
struct list;

class jsonObject;
class jsonString;
class jsonInteger;
class jsonBoolean;
class jsonArray;
class visitor;

class Serializable{

    public:
        typedef std::string serialize_type;
        virtual std::string serialize() = 0;
};

// base abstract type for json
class basic_json : public Serializable{};


template<typename T>
class jsonType  : public basic_json{
    public:
    // each corresponding concrete class should take care of initializing its value with the given 
    // datatypes which do not support certain methods and operatos will throw an runtime exception
    typedef T value_type;
    typedef T* pointer;
    typedef T& reference;

    virtual T& operator=(T &value) = 0;
    virtual T& get() = 0;
    virtual jsonType<T>& clear() = 0;

    // serialize itself and return as string
    //virtual void acceptVisitor(visitor & v) = 0; 
};

template<typename T>
class Transformable{
    public:
        virtual basic_json &operator[](T key) = 0;
        virtual jsonArray & getAsArray(T key) = 0;
        virtual jsonObject & getAsObject(T key) = 0;
        virtual jsonString & getAsString(T key) = 0;
        virtual jsonBoolean & getAsBoolean(T key) = 0;
        virtual jsonInteger & getAsInteger(T key) = 0;
};

template<typename T>
class Extendable{
    public:
      virtual T& push(basic_json &) {throw std::runtime_error("Operation Not Permitted"); }
      virtual T& push(const char * key ,  std::shared_ptr<basic_json>){throw std::runtime_error("Operation Not Permitted");}
      virtual T& push(const char * key , basic_json & ){throw std::runtime_error("Operation Not Permitted");}
      virtual T & push(std::shared_ptr<basic_json> ){throw std::runtime_error("Operation Not Permitted");};
};

class jsonArray : public jsonType<list> , 
                  public Transformable<int> , 
                  public Extendable<jsonType<list>>
{

    list &value_;
    public:
        explicit  jsonArray(Tokenizer &);
        explicit jsonArray() ;
        explicit jsonArray(jsonArray & array);
        basic_json& operator[](int idx) override;
        list & operator=(list &arr) override ;
        list & get() override ;
        std::string serialize() override;
        jsonObject & getAsObject(int idx) override;
        jsonArray & getAsArray(int idx)override;
        jsonInteger & getAsInteger(int idx) override ;
        jsonString & getAsString(int idx) override;
        jsonBoolean & getAsBoolean(int idx) override;
        jsonArray & push(std::shared_ptr<basic_json>) override;
        jsonArray & push(basic_json &) override;
        jsonArray & clear() override;
        //void acceptVisitor(visitor & v) override;
};

class jsonObject : public jsonType<object> ,
                   public Transformable<const char *> , 
                   public Extendable<jsonType<object>>
{
    object &value_;
    public:
        jsonObject(Tokenizer &);
        jsonObject() ;
        basic_json & operator[](const char * key) override  ;
        object & operator=(object & value) override ;
        object &  get() override;
        std::string serialize() override;
        jsonObject & getAsObject(const char * key) override;
        jsonInteger & getAsInteger(const char * key) override;
        jsonBoolean & getAsBoolean(const char * key) override;
        jsonArray & getAsArray(const char * key) override;
        jsonString & getAsString(const char * key) override;
        //virtual void acceptVisitor(visitor & v) override;
        jsonObject & push(const char * key , std::shared_ptr<basic_json>) override;
        jsonObject & push(const char * key , basic_json & val) override;
        jsonObject & clear() override;
};

class jsonString : public jsonType<std::string>
{
    std::string  value_;
    public:
        explicit jsonString(Tokenizer & tokenizer , token * current);
        explicit jsonString(Tokenizer & tokenizer );
        explicit jsonString(const char * value_) ;
        std::string & operator=(std::string &str) override;
        std::string & get()override;
        std::string serialize() override;
        jsonString & clear() override;
        //virtual void acceptVisitor(visitor & v) override;

};

class jsonInteger : public jsonType<int> 
{
    int &value_;
    public:
        jsonInteger(Tokenizer & , token*);
        jsonInteger(Tokenizer &);
        jsonInteger(int l) : value_(*new int(l)){}
        int & get() override;
        int & operator=(int & a) override;
        std::string serialize() override;
        jsonInteger & clear() override;
        //virtual void acceptVisitor(visitor & v) override;
};

class jsonBoolean : public jsonType<bool> 
{
    bool &value_;
    public:
        jsonBoolean(Tokenizer &tokensize , token *);
        jsonBoolean(bool val) : value_(*new bool(val)){}
        bool & operator=(bool &val) override ;
        bool & get() override;
        std::string serialize() override;
        jsonBoolean & clear() override;
        //virtual void acceptVisitor(visitor & v) override;
};

#include "types.h"
#include "jsonObject.h"
#include "jsonArray.h"
#include "jsonString.h"
#include "jsonBool.h"
#include "jsonInteger.h"
#include "json.h"

#endif
