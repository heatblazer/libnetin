#ifndef TJSON_H
#define TJSON_H
#include <string>
#include <vector>
#include <iostream>
namespace tjson {

/**
    very poorman json formatter
 * {
 *   {
 *      a:"aaaa",
 *      b:"cccc",
 *      c:1000
 *   },
 *   {
 *      t: [1,2,3,4]
 *   },
 * }
 *
 */


struct JsonField
{
private:
    static std::string toString(const float& ref)
    {
        return std::to_string(ref);
    }

    static std::string toString(int ref)
    {
        return std::to_string(ref);
    }

    static std::string toString(const int& ref)
    {
        return std::to_string(ref);
    }

    static std::string toString(unsigned int ref)
    {
        return std::to_string(ref);
    }

    static std::string toString(const std::string& ref)
    {
        return std::string{"\""+ ref + "\""};
    }

public:
    std::string K, V;

    std::string full;

    JsonField(){}

    template<typename T>
    JsonField(std::string&& k, const T& v)
    {
        K = std::string{"\"" + k + "\""};
        V = JsonField::toString(v);
        full = std::string{K + ":" + V};
    }

    JsonField(const JsonField& other) { this->K = other.K; this->V = other.V; this->full = other.full;}

    JsonField operator=(const JsonField& other) {
        this->K = other.K;
        this->V = other.V;
        this->full = other.full;
        return *this;
    }

    operator const char*()
    {
        return full.c_str();
    }

};



struct JsonBuilder
{
    std::vector<JsonField> fields;

    JsonBuilder(){};

    JsonBuilder& add(const JsonField& f)
    {
        fields.push_back(f);
        return  *this;
    }

    std::string build()
    {
        std::string ret;
        size_t i=0;
        ret += "{\r\n";
        for(i=0; i < fields.size()-1; i++)
        {
            ret += fields[i].full;
            ret+=",";
        }
        ret += fields[i++].full;
        ret += "\r\n}";
        return ret;
    }
};


struct JsonSerializer
{
    std::string arrname;
    std::vector<JsonBuilder> builders;

    JsonSerializer() : arrname{"{\r\n\"default\":"}{}
    JsonSerializer(const std::string& arr) : arrname{"{\""+arr+"\":"} {}

    void add(const JsonBuilder& bld)
    {
        builders.push_back(bld);
    }


    std::string serialize()
    {
        std::string final;
        size_t i;
        final += arrname;
        final += "[\r\n";
        for (i=0; i < builders.size()-1; i++)
        {
            final += builders[i].build();
            final += ",";
        }
        final += builders[i++].build();
        final += "\r\n]\r\n}";
        return final;
    }
};


} //tjson

#endif // TJSON_H
